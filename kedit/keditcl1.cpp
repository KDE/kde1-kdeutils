 /*

  $Id$
 
  kedit, a simple text editor for the KDE project

  Requires the Qt widget libraries, available at no cost at 
  http://www.troll.no
  
  Copyright (C) 1997 Bernd Johannes Wuebben   
  wuebben@math.cornell.edu

  parts:
  Alexander Sanda <alex@darkstar.ping.at>  
 
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
  
  KEdit, simple editor class, hacked version of the original by 

 
  */

#include "keditcl.h"

#include <klocale.h>
#include <kapp.h>



KEdit::KEdit(KApplication *a, QWidget *parent, const char *name, 
	     const char *fname) : QMultiLineEdit(parent, name){


    mykapp = a;
    filename = fname;
    filename.detach();

    modified = FALSE;

    // set some defaults

    line_pos = col_pos = 0;
    fill_column_is_set = TRUE;
    word_wrap_is_set = TRUE;
    fill_column_value = 80;
    autoIndentMode = false;

    current_directory = QDir::currentDirPath();

    make_backup_copies = TRUE;
    
    installEventFilter( this );     

    srchdialog = NULL;
    replace_dialog= NULL;
    file_dialog = NULL;
    gotodialog = NULL;

    connect(this, SIGNAL(textChanged()), this, SLOT(setModified()));
    setContextSens();
}



KEdit::~KEdit(){

}


int KEdit::currentLine(){

  computePosition();
  return line_pos;

};


int KEdit::currentColumn(){

  computePosition();
  return col_pos;

}


bool KEdit::WordWrap(){

  return word_wrap_is_set;

}

void  KEdit::setWordWrap(bool flag ){

  word_wrap_is_set = flag;
}
    
bool  KEdit::FillColumnMode(){

  return fill_column_is_set;
}

void  KEdit::setFillColumnMode(int line){

  if (line <= 0) {
    fill_column_is_set = FALSE;
    fill_column_value = 0;
  }
  else{
    fill_column_is_set = TRUE;
    fill_column_value = line;
  }

}


int KEdit::loadFile(QString name, int mode){

    int fdesc;
    struct stat s;
    char *addr;
    QFileInfo info(name);

    if(!info.exists()){
      QMessageBox::warning(
		  this,
		  klocale->translate("Sorry:"),
		  klocale->translate("The specified File does not exist"),
		  klocale->translate("OK"),
		  "",
		  "",
		  0,0
		  );

      return KEDIT_RETRY;
    }

    if(info.isDir()){
      QMessageBox::warning(
		   this,
		   klocale->translate("Sorry:"),
		   klocale->translate("You have specificated a directory"),
		   klocale->translate("OK"),
		  "",
		  "",
		  0,0
		   );		

      return KEDIT_RETRY;
    }


   if(!info.isReadable()){
      QMessageBox::warning(
		  this,
		  klocale->translate("Sorry"),
                  klocale->translate("You do not have read permission to this file."),
                  klocale->translate("OK"),
		  "",
		  "",
		  0,0
		  );


      return KEDIT_RETRY;
    }


    fdesc = open(name, O_RDONLY);

    if(fdesc == -1) {
        switch(errno) {
        case EACCES:

	  QMessageBox::warning(
		  this,
		  klocale->translate("Sorry"),
                  klocale->translate("You do not have read permission to this file."),
                  klocale->translate("OK"),
		  "",
		  "",
		  0,0
		  );

	  return KEDIT_OS_ERROR;

        default:
            QMessageBox::warning(
		  this, 
		  klocale->translate("Sorry"),				 
		  klocale->translate("An Error occured while trying to open this Document"),
                  klocale->translate("OK"),
		  "",
		  "",
		  0,0
		  );
            return KEDIT_OS_ERROR;
        }
    }
    
    emit loading();
    mykapp->processEvents();

    fstat(fdesc, &s);
    addr = (char *)mmap(0, s.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fdesc, 0);

    setAutoUpdate(FALSE);

    disconnect(this, SIGNAL(textChanged()), this, SLOT(setModified()));

// The following is a horrible hack that we need to put up with until
// Qt 1.3 comes along. The problem is that QMultiLineEdit::setText(char*)
// is O(n^2) which makes loading larger files very slow. We resort here to 
// a workaroud which load the file line by line. I use a memmap construction
// but probably a QTextStream is just about as fast.

    if(mode & OPEN_INSERT) {
      
      register long int  i;
      char *beginning_of_line;
      beginning_of_line = addr;
      int line, col;
      
      char c = '\n';

      if(s.st_size != 0){
	c = *(addr + s.st_size - 1 ); // save the last character of the file
      }
      
      // this boolean will indicate whether we already have inserted the first line.
      bool past_first_line = false;

      getCursorPosition(&line,&col);

      // let's save the position of the cursor
      int save_line = line;
      int save_col = col;

      for(i = 0; i < s.st_size; i++){

	if( *(addr + i) == '\n' ){

	  *(addr + i) = '\0';

	  if(!past_first_line){
	    QString string;
	    string = beginning_of_line;
	    string += '\n';
	    insertAt(string,line,col);
	    past_first_line = true;
	  }
	  else{
	    insertLine(beginning_of_line,line);
	  }

	  line ++;
	  beginning_of_line = addr + i + 1;

	}
      }

      // What if the last char of the file wasn't a newline?
      // In this case we have to manually insert the last "couple" of characters.
      // This routine could be avoided if I knew for sure that I could
      // memmap a file into a block of memory larger than the file size.
      // In that case I would simply put a zero after the last char in the file.
      // and the above would go through almost unmodified. Well, I don't, so 
      // here we go:

      if( c != '\n'){ // we're in here if s.st_size != 0 and last char != '\n'

	char* buf = (char*)malloc(addr + i - 1 - beginning_of_line + 2);
	strncpy(buf, beginning_of_line, addr + i - 1 - beginning_of_line +1);
	buf[ addr + i - 1 - beginning_of_line + 2 ] = '\0';
	append(buf);

	free(buf);
      
      }
      // restore the initial Curosor Position
      setCursorPosition(save_line,save_col);

    }
    else{ // Not inserting but loading a completely new file.

      register long int  i;
      char *beginning_of_line;
      beginning_of_line = addr;

      // eradicate the old text.
      this->clear();
      char c = '\n';

      if(s.st_size != 0){
	c = *(addr + s.st_size - 1 ); // save the last character of the file
      }

      for(i = 0; i < s.st_size; i++){
	if( *(addr + i) == '\n' ){
	  *(addr + i) = '\0';
	  append(beginning_of_line);
	  beginning_of_line = addr + i + 1;
	}
      }

      // Same consideration as above:

      if( c != '\n'){ // we're in here if s.st_size != 0 and last char != '\n'

	char* buf = (char*)malloc(addr + i - 1 - beginning_of_line + 2);
	strncpy(buf, beginning_of_line, addr + i - 1 - beginning_of_line +1);
	buf[ addr + i - 1 - beginning_of_line + 2 ] = '\0';
	append(buf);

	free(buf);
      
      }
    }

    setAutoUpdate(TRUE);
    repaint();

    connect(this, SIGNAL(textChanged()), this, SLOT(setModified()));    

    munmap(addr, s.st_size);
        
    if ( mode == OPEN_INSERT)
      toggleModified(TRUE);
    else
      toggleModified(FALSE);
    
    
    if(!(mode == OPEN_INSERT)){
        filename = name;
	filename.detach();
    }

    if( mode == OPEN_READONLY)
      this->setReadOnly(TRUE);
    else
      this->setReadOnly(FALSE);
    

    emit(fileChanged());
    setFocus();

    return KEDIT_OK;
}


int KEdit::insertFile(){

    QFileDialog *box;
    QString file_to_insert;

    box = getFileDialog(klocale->translate("Select Document to Insert"));

    box->show();
    
    if (!box->result()) {
      return KEDIT_USER_CANCEL;
    }
    
    if(box->selectedFile().isEmpty()) {  /* no selection */
      return KEDIT_USER_CANCEL;
    }
    
    file_to_insert = box->selectedFile();
    file_to_insert.detach();
    
    
    int result = loadFile(file_to_insert, OPEN_INSERT);

    if (result == KEDIT_OK )
      setModified();

    return  result;


}

int KEdit::openFile(int mode)
{
    QString fname;
    QFileDialog *box;

  int result;

    if( isModified() ) {           
      switch( QMessageBox::warning( 
			 this,
			 klocale->translate("Warning:"), 	
			 klocale->translate("The current Document has been modified.\n"\
					    "Would you like to save it?"),
			 klocale->translate("Yes"),
			 klocale->translate("No"),
			 klocale->translate("Cancel"),
                                  0, 2 
			 )
	      )
	{
	case 0: // Yes or Enter

	result = doSave();

	if ( result == KEDIT_USER_CANCEL)
	  return KEDIT_USER_CANCEL;

	if (result != KEDIT_OK){

	  switch(QMessageBox::warning(
			   this,
			   klocale->translate("Sorry:"), 
			   klocale->translate("Could not save the document.\n"\
                                              "Open a new document anyways?"), 
			   klocale->translate("Yes"),
                           klocale->translate("No"),
			       "",
			       0,1
			   )
		 )
	    {

	    case 0:
	      break;
	    case 1:
	      return KEDIT_USER_CANCEL;
	      break;
	    }

	}

        break;

	case 1: // No 

	  break;
	case 2: // cancel
	  return KEDIT_USER_CANCEL;
	  break;

	}
    }
            
    box = getFileDialog(klocale->translate("Select Document to Open"));
    
    box->show();
    
    if (!box->result())   /* cancelled */
      return KEDIT_USER_CANCEL;
    if(box->selectedFile().isEmpty()) {  /* no selection */
      return KEDIT_USER_CANCEL;
    }
    
    fname =  box->selectedFile();
    
    int result2 =  loadFile(fname, mode);
    
    if ( result2 == KEDIT_OK )
      toggleModified(FALSE);
    
    return result2;
	
        
}

int KEdit::newFile(){

  int result;

    if( isModified() ) {           
      switch( QMessageBox::warning( 
			 this,
			 klocale->translate("Warning:"), 	
			 klocale->translate("The current Document has been modified.\n"\
					    "Would you like to save it?"),
			 klocale->translate("Yes"),
			 klocale->translate("No"),
			 klocale->translate("Cancel"),
                                  0, 2 )
	      )
	{
	
	case 0: // Yes or Enter

	  result = doSave();

	  if ( result == KEDIT_USER_CANCEL)
	    return KEDIT_USER_CANCEL;

	  if (result != KEDIT_OK){

	    switch(QMessageBox::warning(this,
			   klocale->translate("Sorry:"), 
			   klocale->translate("Could not save the document.\n"\
                                              "Create a new document anyways?"), 
			   klocale->translate("Yes"),
                           klocale->translate("No"),
			       "",
			       0,1
				      )){

	  case 0:
	    break;
	  case 1:
	    return KEDIT_USER_CANCEL;
	    break;
	  }

	}

        break;

      case 1: // No 

        break;
      case 2: // cancel
	 return KEDIT_USER_CANCEL;
	 break;

      }
    }
    

    this->clear();
    toggleModified(FALSE);

    setFocus();

    filename = klocale->translate("Untitled");
       
    computePosition();
    emit(fileChanged());

    return KEDIT_OK;
            
        
}



void KEdit::computePosition(){

  int line, col, coltemp;

  getCursorPosition(&line,&col);
  QString linetext = textLine(line);

  // O.K here is the deal: The function getCursorPositoin returns the character
  // position of the cursor, not the screenposition. I.e,. assume the line
  // consists of ab\tc then the character c will be on the screen on position 8
  // whereas getCursorPosition will return 3 if the cursors is on the character c.
  // Therefore we need to compute the screen position from the character position.
  // That's what all the following trouble is all about:
  
  coltemp  = 	col;
  int pos  = 	0;
  int find = 	0;
  int mem  = 	0;
  bool found_one = false;

  // if you understand the following algorithm you are worthy to look at the
  // kedit+ sources -- if not, go away ;-)

  while(find >=0 && find <= coltemp- 1 ){
    find = linetext.find('\t', find, TRUE );
    if( find >=0 && find <= coltemp - 1 ){
      found_one = true;
      pos = pos + find - mem;
      pos = pos + 8  - pos % 8;
      mem = find;
      find ++;
    }
  }

  pos = pos + coltemp - mem ;  // add the number of characters behind the
                               // last tab on the line.

  if (found_one){	       
    pos = pos - 1;
  }

  line_pos = line;
  col_pos = pos;

}


void KEdit::keyPressEvent ( QKeyEvent *e){

  QString* pstring;

  if ((e->state() & ControlButton ) && (e->key() == Key_K) ){

    int line = 0;
    int col  = 0;
    QString killstring;

    if(!killing){
      killbufferstring = "";
      killtrue = false;
      lastwasanewline = false;
    }

    getCursorPosition(&line,&col);
    killstring = textLine(line);
    killstring.detach();
    killstring = killstring.mid(col,killstring.length());


    if(!killbufferstring.isEmpty() && !killtrue && !lastwasanewline){
      killbufferstring += "\n";
    }

    if( (killstring.length() == 0) && !killtrue){
      killbufferstring += "\n";
      lastwasanewline = true;
    }

    if(killstring.length() > 0){

      killbufferstring += killstring;
      lastwasanewline = false;
      killtrue = true;

    }else{

      lastwasanewline = false;
      killtrue = !killtrue;

    }

    killing = true;

    QMultiLineEdit::keyPressEvent(e);
    setModified();
    emit CursorPositionChanged();
    return;
  }

  if ((e->state() & ControlButton ) && (e->key() == Key_Y) ){

    int line = 0;
    int col  = 0;

    getCursorPosition(&line,&col);

    if(!killtrue)
      killbufferstring += '\n';

    insertAt(killbufferstring,line,col);

    killing = false;
    setModified();
    emit CursorPositionChanged();
    return;
  }

  killing = false;

  if (e->key() == Key_Tab){
    if (isReadOnly())
      return;
    QMultiLineEdit::insertChar((char)'\t');
    setModified();
    emit CursorPositionChanged();
    return;
  }


  if (e->key() == Key_Insert){
    this->setOverwriteMode(!this->isOverwriteMode());
    emit toggle_overwrite_signal();
    return;
  }
  
  //printf("fill %d word %d value %d\n", fill_column_is_set,
  //   word_wrap_is_set,fill_column_value);

  if(fill_column_is_set && word_wrap_is_set ){

    // word break algorithm
    if(isprint(e->ascii())){
 
      // printf("col_pos %d\n",col_pos);
      if( col_pos +1 > fill_column_value - 1){ 

	if (e->ascii() == 32 ){ // a space we can just break here
	  mynewLine();
	  //  setModified();
	  emit CursorPositionChanged();
	  return;	 
	}

	pstring = getString(line_pos);

	// find a space to break at
	int space_pos = pstring->findRev(" ", -1,TRUE);

	if( space_pos == -1 ){ 
	  
	  // no space to be found on line, just break, what else could we do?
	  mynewLine();
	  computePosition();  

	}
	else{

	  // Can't use insertAt('\n'...) due to a bug in Qt 1.2, and must resort
	  // to the following work around:

	  // go back to the space

	  focusOutEvent(&QFocusEvent(Event_FocusOut));

	  for(uint i = 0; i < (pstring->length() - space_pos -1 ); i++){
	    cursorLeft();
	  }
	  
	  // insert a newline there

	  mynewLine();
	  end(FALSE);

	  focusOutEvent(&QFocusEvent(Event_FocusIn));

	  computePosition();
	}
      }

      QMultiLineEdit::keyPressEvent(e);
      // setModified();
      emit CursorPositionChanged();
      return;
    }
    else{ // not isprint that is some control character or some such
  
      if(e->key() == Key_Return || e->key() == Key_Enter){
    
	mynewLine();
	//setModified();
	emit CursorPositionChanged();
	return;
      }

      QMultiLineEdit::keyPressEvent(e);
      // setModified();
      emit CursorPositionChanged();
      return;

    }
  } // end do_wordbreak && fillcolumn_set
 

  // fillcolumn but no wordbreak

  if (fill_column_is_set){

    if(e->key() == Key_Return || e->key() == Key_Enter){
    
      mynewLine();
      // setModified();
      emit CursorPositionChanged();
      return;

    }

    if(isprint(e->ascii())){
    
      if( col_pos +1> fill_column_value - 1){ 
	  mynewLine();
	  //  setModified();
      }

    }

    QMultiLineEdit::keyPressEvent(e);
    //setModified();
    emit CursorPositionChanged();
    return;

  }

  // default action
  if(e->key() == Key_Return || e->key() == Key_Enter){
    
    mynewLine();
    //setModified();
    emit CursorPositionChanged();
    return;

  }

  QMultiLineEdit::keyPressEvent(e);
  //setModified();
  emit CursorPositionChanged();

}


void KEdit::mynewLine(){

  if (isReadOnly())
    return;

  setModified();

  if(!autoIndentMode){ // if not indent mode
    newLine();
    return;
  }

  int line,col;
  bool found_one = false;

  getCursorPosition(&line,&col);
  
  QString string, string2;

  while(line >= 0){

    string  = textLine(line);
    string2 = string.stripWhiteSpace();

    if(!string2.isEmpty()){
      string = prefixString(string);
      found_one = TRUE;
      break;
    }

    line --;
  }
      
  // string will now contain those whitespace characters that I need to insert
  // on the next line. 

  if(found_one){

    // don't ask my why I programmed it this way. I am quite sick of the Qt 1.2
    // MultiLineWidget -- It is anoyingly buggy. 
    // I have to put in obscure workarounds all over the place. 

    focusOutEvent(&QFocusEvent(Event_FocusOut));
    newLine();
    
    for(uint i = 0; i < string.length();i++){
      insertChar(string.data()[i]);
    }

    // this f***king doesn't work.
    // insertAt(string.data(),line + 1,0);

    focusInEvent(&QFocusEvent(Event_FocusIn));

  }
  else{
    newLine();
  }
}

void KEdit::setAutoIndentMode(bool mode){

  autoIndentMode = mode;

}


QString KEdit::prefixString(QString string){
  
  // This routine return the whitespace before the first non white space
  // character in string. This is  used in mynewLine() for indent mode.
  // It is assumed that string contains at least one non whitespace character
  // ie \n \r \t \v \f and space
  
  //  printf(":%s\n",string.data());

  int size = string.size();
  char* buffer = (char*) malloc(size + 1);
  strncpy (buffer, string.data(),size - 1);
  buffer[size] = '\0';

  int i;
  for (i = 0 ; i < size; i++){
    if(!isspace(buffer[i]))
      break;
  }

  buffer[i] = '\0';

  QString returnstring = buffer;
  
  free(buffer);

  //  printf(":%s:\n",returnstring.data());
  return returnstring;

}

void KEdit::mousePressEvent (QMouseEvent* e){

  
  QMultiLineEdit::mousePressEvent(e);
  emit CursorPositionChanged();

}

void KEdit::mouseMoveEvent (QMouseEvent* e){

  QMultiLineEdit::mouseMoveEvent(e);
  emit CursorPositionChanged();
  

}


void KEdit::installRBPopup(QPopupMenu* p){

  rb_popup = p;

}

void KEdit::mouseReleaseEvent (QMouseEvent* e){

  
  QMultiLineEdit::mouseReleaseEvent(e);
  emit CursorPositionChanged();

}


int KEdit::saveFile(){

    struct stat st;
    int stat_ok = -1;
    bool exists_already;

    if(!modified) {
      emit saving();
      mykapp->processEvents();
      return KEDIT_OK;
    }

    QFile file(filename);
    QString backup_filename;
    exists_already = file.exists();

    if(exists_already){
      stat_ok = stat(filename.data(), &st);
      backup_filename = filename;
      backup_filename.detach();
      backup_filename += '~';

      rename(filename.data(),backup_filename.data());
    }

    if( !file.open( IO_WriteOnly | IO_Truncate )) {
      rename(backup_filename.data(),filename.data());
      QMessageBox::warning(
			   this,
			   klocale->translate("Sorry"),
			   klocale->translate("Could not save the document\n"),
			   klocale->translate("OK"),
			   "",
			   "",
			   0,0
			   );
      return KEDIT_OS_ERROR;
    }

    emit saving();
    mykapp->processEvents();
      
    QTextStream t(&file);
    
    int line_count = numLines();

    for(int i = 0 ; i < line_count ; i++){
	t << textLine(i) << '\n';
    }

    modified = FALSE;    
    file.close();

    if(exists_already)
      chmod(filename.data(),st.st_mode);// preseve filepermissions
    
    return KEDIT_OK;

}

void KEdit::setFileName(char* name){

  filename = name;
  filename.detach();

}

void KEdit::saveasfile(char* name){

  QString filenamebackup;
  filenamebackup = filename;
  filename = name;
  filename.detach();
  saveFile();
  filename = filenamebackup;
  filename.detach();

}

QFileDialog* KEdit::getFileDialog(const char* captiontext){

  if(!file_dialog){

    file_dialog = new QFileDialog(current_directory.data(),"*",this,"file_dialog",TRUE);
  }

  file_dialog->setCaption(captiontext);
  file_dialog->rereadDir();

  return file_dialog;
}

int KEdit::saveAs(){
    
  QFileDialog *box;

  QFileInfo info;
  QString tmpfilename;
  int result;
  
  box = getFileDialog(klocale->translate("Save Document As"));

  QPoint point = this->mapToGlobal (QPoint (0,0));

  QRect pos = this->geometry();
  box->setGeometry(point.x() + pos.width()/2  - box->width()/2,
		   point.y() + pos.height()/2 - box->height()/2, 
		   box->width(),box->height());
try_again:


  box->show();
  
  if (!box->result())
    {
      return KEDIT_USER_CANCEL;
    }
  
  if(box->selectedFile().isEmpty()){
    return KEDIT_USER_CANCEL;
  }
  
  info.setFile(box->selectedFile());
  
  if(info.exists()){

    switch( QMessageBox::warning( 
			   this,
			   klocale->translate("Warning:"), 	
			   klocale->translate("A Document with this Name exists already\n"\
						     "Do you want to overwrite it ?"),
                           klocale->translate("Yes"),
			   klocale->translate("No"),
				  "",
                                  1, 1 )
	    ){
    case 0: // Yes or Enter
        // try again
        break;
    case 1: // No or Escape
        goto try_again;
        break;
    }
  }
  
  
  tmpfilename = filename;
  
  filename = box->selectedFile();
  
  // we need this for saveFile();
  modified = TRUE; 
  
  result =  saveFile();
  
  if( result != KEDIT_OK)
    filename = tmpfilename; // revert filename
  
  return result;
      
}



int KEdit::doSave()
{

    
  int result = 0;
    
    if(filename == "Untitled") {
      result = saveAs();

      if(result == KEDIT_OK)
	setCaption(filename);

      return result;
    }

    QFileInfo info(filename);

    if(info.exists() && !info.isWritable()){

      QMessageBox::warning(
			   this,
			   klocale->translate("Sorry:"), 
			   klocale->translate("You do not have write permission"\
					      "to this file.\n"),
			   klocale->translate("OK"),
			   "",
			   "",
			   0,0
			   );

      return KEDIT_NOPERMISSIONS;
    }
    

    result =  saveFile();
    return result;

}

int KEdit::doSave( const char *_name ){

    QString temp  = filename;
    filename =  _name;
    filename.detach();

    int result = saveFile();

    filename = temp;
    filename.detach();
    return result;
}


void KEdit::setName( const char *_name ){

    filename = _name;
    filename.detach();
}


QString KEdit::getName(){

    return filename;
}


void KEdit::saveBackupCopy(bool par){

  make_backup_copies = par;

}
void KEdit::selectFont(){
 
  QFont font = this->font();
  KFontDialog::getFont(font);
  this->setFont(font);

}

void KEdit::setModified(){

  modified = TRUE;


}

void KEdit::toggleModified( bool _mod ){

    modified = _mod;
}



bool KEdit::isModified(){

    return modified;
}



void KEdit::setContextSens(){

}


bool KEdit::eventFilter(QObject *o, QEvent *ev){

  static QPoint tmp_point;

  (void) o;

  if (ev->type() == Event_Paint)
	{
	if (srchdialog)
		if (srchdialog->isVisible())
			srchdialog->raise();

	if (replace_dialog)
		if (replace_dialog->isVisible())
			replace_dialog->raise();
	}

  

  if(ev->type() != Event_MouseButtonPress) 
    return FALSE;
    
  QMouseEvent *e = (QMouseEvent *)ev;
  
  if(e->button() != RightButton) 
    return FALSE;

  tmp_point = QCursor::pos();
  
  if(rb_popup)
    rb_popup->popup(tmp_point);

  return TRUE;

}


QString KEdit::markedText(){

  return QMultiLineEdit::markedText();

}

void KEdit::doGotoLine() {

	if( !gotodialog )
		gotodialog = new KEdGotoLine( this, "gotodialog" );

	this->clearFocus();

	gotodialog->show();
	// this seems to be not necessary
	// gotodialog->setFocus();
	if( gotodialog->result() ) {
		setCursorPosition( gotodialog->getLineNumber()-1 , 0, FALSE );
		emit CursorPositionChanged();
		setFocus();
	}
}
