  /*

  $Id$
 
         kedit+ , a simple text editor for the KDE project

  Requires the Qt widget libraries, available at no cost at 
  http://www.troll.no
  
  Copyright (C) 1996 Bernd Johannes Wuebben   
  wuebben@math.cornell.edu
  
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
  Nov 96, Alexander Sanda <alex@darkstar.ping.at>
 
  */



#include "KEdit.moc"
#include "KEdit.h"
#include "kedit+.h"
#include "kfontdialog.h"



extern KApplication *a;

KEdit::KEdit(QWidget *parent, const char *name, const char *fname, unsigned flags)
    : QMultiLineEdit(parent, name){
    
    p_parent = parent;

    qstrncpy(filename, fname, 1023);
    modified = FALSE;
    k_flags = flags;

    line_pos = col_pos = 0;
    fill_column_is_set = true;
    word_wrap_is_set = true;
    fill_column_value = 79;
    
    installEventFilter( this );     
    srchdialog = 0;
    connect(this, SIGNAL(textChanged()), this, SLOT(setModified()));
    setContextSens();
}



KEdit::~KEdit(){

}


int KEdit::loadFile(const char *name, int mode){

    int fdesc;
    struct stat s;
    char *mb_caption = "Load file:";
    char *addr;
    QFileInfo info(name);

    if(!info.exists()){
      QMessageBox::message("Sorry","The specified File does not exist","OK");
      return KEDIT_RETRY;
    }

    if(!info.isReadable()){
      QMessageBox::message("Sorry","You do not have read permission to this file.","OK");
      return KEDIT_RETRY;
    }


    fdesc = open(name, O_RDONLY);

    if(fdesc == -1) {
        switch(errno) {
        case EACCES:
            QMessageBox::message("Sorry", 
				 "You have do not have Permission to \n"\
				 "read this Document", "Ok");
            return KEDIT_OS_ERROR;

        default:
            QMessageBox::message(mb_caption, 
				 "An Error occured while trying to open this Document", 
				 "Ok");
            return KEDIT_OS_ERROR;
        }
    }
    
    emit loading();
    a->processEvents();

    fstat(fdesc, &s);
    addr = (char *)mmap(0, s.st_size, PROT_READ | PROT_WRITE, 
			MAP_PRIVATE, fdesc, 0);
    

    setAutoUpdate(FALSE);

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
      // in that case I would simply put a zero after the last char in the file.
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
    
    munmap(addr, s.st_size);
        
    modified = mode & OPEN_INSERT;
    setFocus();
    
    if(!(mode & OPEN_INSERT))
        qstrncpy(filename, name, 1023);
    
    setEditMode(edit_mode);
    emit(fileChanged());

    return KEDIT_OK;
}


int KEdit::insertFile(){

    QFileDialog *box;
    QString filename;
      
    box = new QFileDialog( this, "fbox", TRUE);
    
    box->setCaption("Select Document to Insert");

    if(k_flags & ALLOW_OPEN) {
      
      box->show();
      
      if (!box->result()) {
	delete box;
	return KEDIT_USER_CANCEL;
      }

      if(box->selectedFile().isEmpty()) {  /* no selection */
	delete box;
	return KEDIT_USER_CANCEL;
      }

      filename = box->selectedFile();
      filename.detach();

      delete box;

    }
    
    return loadFile(filename.data(), OPEN_INSERT);

}

int KEdit::openFile(int mode)
{
    char fname[1024];
    QFileDialog *box;
    

    if(modified && !(mode & OPEN_INSERT)) {           
      if((QMessageBox::query("Message", 
			     "The current Document has been modified.\n"\
			     "Would you like to save it?"))) {

	if (doSave() != KEDIT_OK){
	  
	  QMessageBox::message("Sorry", "Could not Save the Document", "OK");
	  return KEDIT_OS_ERROR;     

	}
      }
    }
            
    box = new QFileDialog( this, "fbox", TRUE);
    
    box->setCaption("Select Document to Open");

    if(k_flags & ALLOW_OPEN) {
        box->show();
        if (!box->result())   /* cancelled */
            return KEDIT_USER_CANCEL;
        if(box->selectedFile().isNull()) {  /* no selection */
            return KEDIT_USER_CANCEL;
        }

        strcpy(fname, box->selectedFile().data());

	delete box;

        return loadFile(fname, mode);
    }
    else {
        QMessageBox::message("Sorry", 
			     "You do not have permision to open a Document", 
			     "Yes", this);
        return KEDIT_OS_ERROR;
    }
        
}

int KEdit::newFile(int mode){


    if(modified && !(mode & OPEN_INSERT)) {           
      if((QMessageBox::query("Message", 
			     "The current Document has been modified.\n"\
			     "Would you like to save it?"))) {

	if (doSave() != KEDIT_OK){
	  
	  QMessageBox::message("Sorry", "Could not Save the Document", "OK");
	  return KEDIT_OS_ERROR;     

	}
      }
    }

    this->clear();

    
    modified = FALSE;
    setFocus();

    qstrncpy(filename, "Untitled", 1023);
       
    setEditMode(HAS_POPUP | ALLOW_OPEN | ALLOW_SAVE | ALLOW_SAVEAS | OPEN_READWRITE);

    compute_Position();
    emit(fileChanged());

    return KEDIT_OK;
            
        
}



void KEdit::compute_Position(){

  int line, col, coltemp;

  getCursorPosition(&line,&col);
  QString linetext = textLine(line);

  // O.K here is the deal: The function getCursorPositoin returs the character
  // position of the cursor, not the screenposition. i.e. assume the line
  // consists of ab\tc then the character c will be on the screen on position 8
  // whereas getCursorPosition would return 3 if the cursors is on the character c.
  // Therefore we need to compute the screen position from the character position.
  // that's what all the following trouble is all about:
  
  coltemp  = 	col;
  int pos  = 	0;
  int find = 	0;
  int mem  = 	0;
  bool found_one = false;

  // if you understand the following alogrigthm you are worthy to look at the
  // kedit+ sources, if not, go away ;-)

  while(find >=0 && find <= coltemp- 1){
    find = linetext.find('\t',find,TRUE);
    if( find >=0 && find <= coltemp - 1){
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

  if (e->key() == Key_Tab){
    QMultiLineEdit::insertChar((char)'\t');
    compute_Position();
    emit update_status_bar();
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
      if( col_pos > fill_column_value - 1){ 

	if (e->ascii() == 32 ){ // a space we can just break here
	  newLine();
	  compute_Position();
	  emit update_status_bar();
	  return;	 
	}

	pstring = getString(line_pos);

	// find a space to break at
	int space_pos = pstring->findRev(" ", -1,TRUE);

	if( space_pos == -1 ){ 
	  
	  // no space to be found on line, just break, what else could we do?
	  newLine();
	  compute_Position();  

	}
	else{

	  // Can't use insertAt('\n'...) due to a bug in Qt 1.2, and must resort
	  // to the following work around:

	  // go back to the space

	  for(uint i = 0; i < (pstring->length() - space_pos -1 ); i++){
	    cursorLeft();
	  }
	  
	  // insert a newline there

	  newLine();
	  end(FALSE);
	  compute_Position();
	}
      }

      QMultiLineEdit::keyPressEvent(e);
      compute_Position();
      emit update_status_bar();
      return;
    }
    else{ // not isprint that is some control character or some such
  
      QMultiLineEdit::keyPressEvent(e);
      compute_Position();
      emit update_status_bar();
      return;

    }
  } // end do_wordbreak && fillcolumn_set
 

  // fillcolumn but no wordbreak

  if (fill_column_is_set){

    if(isprint(e->ascii())){
    
      //      printf("col_pos %d\n",col_pos);
      if( col_pos > fill_column_value - 1){ 

	  newLine();
	  compute_Position();  
      }

    }

    QMultiLineEdit::keyPressEvent(e);
    compute_Position();
    emit update_status_bar();
    return;

  }

  // default action

  QMultiLineEdit::keyPressEvent(e);
  compute_Position();
  emit update_status_bar();

}

void KEdit::mousePressEvent (QMouseEvent* e){

  
  QMultiLineEdit::mousePressEvent(e);
  compute_Position();
  emit update_status_bar();

}

void KEdit::mouseMoveEvent (QMouseEvent* e){

  QMultiLineEdit::mouseMoveEvent(e);
  compute_Position();
  emit update_status_bar();
  

}


void KEdit::mouseReleaseEvent (QMouseEvent* e){

  
  QMultiLineEdit::mouseReleaseEvent(e);
  compute_Position();
  emit update_status_bar();

}


int KEdit::saveFile(){

    if(!modified) {
      emit saving();
      a->processEvents();
      return KEDIT_OK;
    }

    QString backup_filename;
    backup_filename = filename;
    backup_filename += '~';

    rename(filename,backup_filename.data());

    QFile file(filename);

    if( !file.open( IO_WriteOnly | IO_Truncate )) {
      rename(backup_filename.data(),filename);
      QMessageBox::message("Sorry","Could not save the file\n","OK");
      return KEDIT_OS_ERROR;
    }

    emit saving();
    a->processEvents();
      
    QTextStream t(&file);
    
    int line_count = numLines();

    for(int i = 0 ; i < line_count ; i++){
      t << textLine(i) << '\n';
    }

    modified = FALSE;    
    file.close();
    
    return KEDIT_OK;

}

int KEdit::saveAs()
{

    QFileDialog *box;
    QFileInfo info;
    QString tmpfilename;
    int result;
    
    box = new QFileDialog( this, "box", TRUE);
    box->setCaption("Save Document As");

try_again:

    box->show();

    if (!box->result()){
      delete box;
      return KEDIT_USER_CANCEL;
    }

    if(box->selectedFile().isNull()){
      delete box;
      return KEDIT_USER_CANCEL;
    }

    info.setFile(box->selectedFile());

    if(info.exists()){
        if(!(QMessageBox::query("Warning:", 
				"A Document with this Name exists already\n"\
				"Do you want to overwrite it ?")))
	  goto try_again;  

    }


    tmpfilename = filename;

    qstrncpy(filename, box->selectedFile().data(), 1023);

    // we need this for saveFile();
    modified = TRUE; 
    
    delete box;

    result =  saveFile();
    
    if( result != KEDIT_OK)
      qstrncpy(filename,tmpfilename.data(),1023); // revert filename
	
    return result;
      
}



int KEdit::doSave()
{

    
  int result = 0;
    
    if(strcmp(filename, "Untitled") == 0) {
      result = saveAs();

      if(result == KEDIT_OK)
	setCaption(filename);

      return result;
    }

    QFileInfo info(filename);
    if(!info.isWritable()){
      QMessageBox::message("Sorry:", 
			   "You do not have write permission to this file.\n","OK");
      return KEDIT_NOPERMISSIONS;
    }
    

    result =  saveFile();
    return result;

}

int KEdit::doSave( const char *_name ){

    QString n = filename;
    strcpy( filename, _name );

    int erg = saveFile();

    strcpy( filename, n.data() );
    return erg;
}


void KEdit::setName( const char *_name ){

    strcpy( filename, _name );
}


const char *KEdit::getName(){

    return filename;
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



int KEdit::setEditMode(int mode){

    int oldmode = edit_mode;
    edit_mode = mode;
    //    setReadOnly(mode & OPEN_READONLY);
    setContextSens();
    return oldmode;
}


void KEdit::setContextSens(){
  /*
    context->setItemEnabled(MENU_ID_OPEN, k_flags & ALLOW_OPEN);
    context->setItemEnabled(MENU_ID_INSERT, (k_flags & ALLOW_SAVE) 
			    && (edit_mode & OPEN_READWRITE));

    context->setItemEnabled(MENU_ID_SAVE, (k_flags & ALLOW_SAVE) 
			    && (edit_mode & OPEN_READWRITE));

    context->setItemEnabled(MENU_ID_SAVEAS, k_flags & ALLOW_SAVEAS);
    */
}


bool KEdit::eventFilter(QObject *o, QEvent *ev){

  static QPoint tmp_point;

  (void) o;

  if(ev->type() != Event_MouseButtonPress) 
    return FALSE;
    
  QMouseEvent *e = (QMouseEvent *)ev;
  
  if(e->button() != RightButton) 
    return FALSE;

  tmp_point = QCursor::pos();
  
  if(p_parent)
    ((TopLevel*)p_parent)->right_mouse_button->popup(tmp_point);   


  return TRUE;

}


QString KEdit::markedText(){

  return QMultiLineEdit::markedText();

}


 
void KEdit::initSearch(){
  
  int result = 1;

  if(!srchdialog)
    srchdialog = new KEdSrch(this, "searchdialog");
  this->clearFocus();
  srchdialog->show();
  srchdialog->setFocus();
  if(srchdialog->result()) {
    result = doSearch(srchdialog->getText(), TRUE);
  }
  
  if(result == 0)
    QMessageBox::message("Search", "No  matches found", "Ok");
  
  this->setFocus();
}



int KEdit::doSearch(const char *s_pattern, int mode)
{
    int line, col, i, length;
    QRegExp re;
    
    if(mode) {
        re = QRegExp(s_pattern, FALSE, FALSE);
        *pattern = '\0';
	getCursorPosition(&line, &col);
    }
    else {
      
      if(strcmp(pattern,"") == 0)
	return 1;
	
      re = QRegExp(pattern, FALSE, FALSE);
	getCursorPosition(&line, &col);
	col +=1;
    }

    for(i = line; i < numLines(); i++) {
        if((col = re.match(textLine(i), i == line ? col : 0, &length)) != -1) {

	    setCursorPosition(i,col,FALSE);
	    cursorRight(TRUE);
	    setCursorPosition(i ,col + length,TRUE);

            qstrncpy(pattern, s_pattern, 255);  /* accept the pattern */
            return 1;
        }
    }
    return 0;
}



int KEdit::repeatSearch() {
  
  int result;

  if(!srchdialog)
      return 0;
  
  result = doSearch(srchdialog->getText(), FALSE);
  
  if(result == 0)
    QMessageBox::message("Search", "No  more matches found", "Ok");

  this->setFocus();
  
  return result;
}


void KEdSrch::selected(int)
{
    accept();
}


KEdSrch::KEdSrch(QWidget *parent, const char *name)
     : QDialog(parent, name, TRUE)

{
    frame1 = new QGroupBox("Search for", this, "frame1");
    values = new QLineEdit( this, "values");
    this->setFocusPolicy(QWidget::StrongFocus);
    connect(values, SIGNAL(returnPressed()), this, SLOT(accept()));
    sensitive = new QCheckBox("Case sensitive", this, "sens");
    ok = new QPushButton("Find", this, "find");
    cancel = new QPushButton("Cancel", this, "cancel");
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    resize(300, 120);
}

QString KEdSrch::getText() { return values->text(); }


void KEdSrch::resizeEvent(QResizeEvent *)
{
    frame1->setGeometry(5, 5, width() - 10, 80);
    cancel->setGeometry(width() - 80, height() - 30, 70, 25);
    ok->setGeometry(10, height() - 30, 70, 25);
    values->setGeometry(20, 25, width() - 40, 25);
    sensitive->setGeometry(20, 55, 100, 25);
}

