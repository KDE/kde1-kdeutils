    /*

    $Id$

    Requires the Qt widget libraries, available at no cost at 
    http://www.troll.no
       
    Copyright (C) 1997 Bernd Johannes Wuebben   
                       wuebben@math.cornell.edu

    Parts
    Copyright (C) 1996 Alexander Sanda (alex@darkstar.ping.at)


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

    */
 

#include "kedit+.h"
#include <kcolordlg.h>
#include "KEdit.h"
#include "kedit+.moc"
#include <qfile.h>
#include "filldlg.h"

// 
#include <qpainter.h>
#include <qstrlist.h> 

#define TOOLBAR_HEIGHT 26
#define TOOLBAR_X_OFFSET 10
#define TOOLBAR_Y_OFFSET 5
#define TOOLBAR_BUTTON_HEIGHT 28
#define TOOLBAR_BUTTON_WIDTH 28

#define STATUSBAR_HEIGHT 18

#define BUTTON_HEIGHT		25
#define BUTTON_WIDTH		25
#define BUTTON_SEPARATION	6
#define B_XOFFSET 3
#define B_YOFFSET 2



QList<TopLevel> TopLevel::windowList;

KApplication *a;

int default_open = KEdit::OPEN_READWRITE;

TopLevel::TopLevel (QWidget * parent, const char *name)
    : QWidget (parent, name)
{

  setMinimumSize (100, 100);

  readSettings();
  kfm = 0L;
  windowList.setAutoDelete( FALSE );


  statusbar_timer = new QTimer(this);
  connect(statusbar_timer, SIGNAL(timeout()),this,SLOT(timer_slot()));

  file = 	new QPopupMenu ();
  edit = 	new QPopupMenu ();
  help = 	new QPopupMenu ();
  options = 	new QPopupMenu ();
  colors =  	new QPopupMenu ();

  colors->insertItem("&Foreground Color",this, SLOT(set_foreground_color()));
  colors->insertItem("&Background Color",this, SLOT(set_background_color()));

  help->insertItem ("&About...",this, 	SLOT(about()));
  help->insertItem ("&Help", 	this, 	SLOT(helpselected()));
  
  file->insertItem ("Ne&w...",	this, 	SLOT(file_new()));
  file->insertItem ("&Open...",	this, 	SLOT(file_open()));
  file->insertItem ("&Save", 	this, 	SLOT(file_save()));
  file->insertItem ("S&ave as...",this, SLOT(file_save_as()));
  file->insertItem ("&Close", 	this,	SLOT(file_close()));
  file->insertSeparator (-1);
  file->insertItem ("Open &URL...",this,SLOT(file_open_url()));
  file->insertItem ("Save to U&RL...",this,SLOT(file_save_url()));
  file->insertSeparator (-1);
  file->insertItem ("&Print...",this,SLOT(print()) );
  //  file->insertItem ("&Print...",this,SLOT(fancyprint()) );
  file->insertSeparator (-1);
  file->insertItem ("New &Window",this,SLOT(newTopLevel()) );
  file->insertSeparator (-1);
  file->insertItem ("E&xit", this,	SLOT(quiteditor()));


  edit->insertItem("&Copy", this, SLOT(copy()));

  edit->insertItem("&Paste", this, SLOT(paste()));
  edit->insertItem("C&ut", this, SLOT(cut()));
  edit->insertSeparator(-1);
  edit->insertItem("Insert &File", this, SLOT(insertFile()));
  edit->insertItem("Insert &Date", this, SLOT(insertDate()));
  edit->insertSeparator(-1);
  edit->insertItem("&Search...", this, SLOT(search()));
  edit->insertItem("&Repeat last search",this, SLOT(search_again()));

  options->insertItem("&Font", this,		SLOT(font()));
  options->insertItem("Colors", colors);
  options->insertSeparator(-1);
  options->insertItem("Fill &Column...", this, SLOT(fill_column_slot()));
  options->insertSeparator(-1);
  buttonID = options->insertItem("&Button Bar",this,SLOT(toggleButtonBar()));
  statusID = options->insertItem("&Status Bar",this,SLOT(toggleStatusBar()));	


  menubar = new QMenuBar (this, "menubar");
  menubar->insertItem ("&File", file);

  //  menubar->setLineWidth (1);

  menubar->insertItem ("&Edit", edit);
  menubar->insertItem ("&Options", options);
  //  menubar->insertSeparator();
  menubar->insertItem ("&Help", help);

  statusbar1 = new QLabel(this);
  statusbar1->setFrameStyle(QFrame::Panel|QFrame::Sunken);
  statusbar2 = new QLabel(this);
  statusbar2->setFrameStyle(QFrame::Panel|QFrame::Sunken);
  statusbar3 = new QLabel(this);
  statusbar3->setFrameStyle(QFrame::Panel|QFrame::Sunken);
  statusbar3->setAlignment(AlignCenter);

  right_mouse_button = new QPopupMenu;

  right_mouse_button->insertItem ("Open...",	this, 	SLOT(file_open()));
  right_mouse_button->insertItem ("Save", 	this, 	SLOT(file_save()));
  right_mouse_button->insertItem ("Save as...",this, SLOT(file_save_as()));
  right_mouse_button->insertSeparator(-1);
  right_mouse_button->insertItem("&Copy", this, SLOT(copy()));
  right_mouse_button->insertItem("&Paste", this, SLOT(paste()));
  right_mouse_button->insertItem("C&ut", this, SLOT(cut()));
  right_mouse_button->insertSeparator(-1);
  right_mouse_button->insertItem("&Font", this,	SLOT(font()));


  ///////////////////////////////////////////////

  QString pixpath = kapp->kdedir() + QString("/lib/pics/toolbar/");
  pb1 = new QPushButton(this);
  pb1->setFocusPolicy(QWidget::NoFocus);

  QToolTip::add(pb1,"Open Document");
  connect( pb1, SIGNAL(clicked()),this, SLOT(file_open()));

  if ( !pb1_pixmap.load(pixpath + "filenew.xpm") ) {
    QMessageBox::message( "Sorry", "Could not load filenew.xpm" );
  }
  pb1->setPixmap( pb1_pixmap );

  ///////////////////////////////////////////////

  pb2 = new QPushButton(this);
  pb2->setFocusPolicy(QWidget::NoFocus);

  QToolTip::add(pb2,"Copy");
  connect( pb2, SIGNAL( clicked() ), SLOT( copy() ) );

  if ( !pb2_pixmap.load(pixpath + "editcopy.xpm") ) {
    QMessageBox::message( "Sorry", "Could not load editcopy.xpm" );
  }
  pb2->setPixmap( pb2_pixmap );

  ////////////////////////////////////////////////

  pb3 = new QPushButton( this);
  pb3->setFocusPolicy(QWidget::NoFocus);

  QToolTip::add(pb3,"Paste");

  connect( pb3, SIGNAL( clicked() ), SLOT( paste() ) );

  if ( !pb3_pixmap.load(pixpath + "editpaste.xpm" ) ) {
    QMessageBox::message( "Sorry", "Could not load editpaste.xpm" );
  }
  pb3->setPixmap( pb3_pixmap );

  /////////////////////////////////////////////////

  pb4 = new QPushButton( this);
  pb4->setFocusPolicy(QWidget::NoFocus);
  QToolTip::add(pb4,"Save Document");
  connect( pb4, SIGNAL( clicked() ), SLOT( file_save() ) );

  if ( !pb4_pixmap.load(pixpath + "filefloppy.xpm") ) {
    QMessageBox::message( "Sorry", "Could not load filefloppy.xpm" );
  }
  pb4->setPixmap( pb4_pixmap );

  //////////////////////////////////////////////////

  pb5 = new QPushButton( this);
  pb5->setFocusPolicy(QWidget::NoFocus);
  QToolTip::add(pb5,"Print Document");
  connect( pb5, SIGNAL( clicked() ), SLOT( print() ) );

  if ( !pb5_pixmap.load(pixpath + "fileprint.xpm") ) {
    QMessageBox::message( "Sorry", "Could not load fileprint.xpm" );
  }
  pb5->setPixmap( pb5_pixmap );

  ///////////////////////////////////////////////////

  pb6 = new QPushButton( this);
  pb6->setFocusPolicy(QWidget::NoFocus);
  QToolTip::add(pb6,"Help");
  connect( pb6, SIGNAL( clicked() ), SLOT( helpselected() ) );

  if ( !pb6_pixmap.load(pixpath + "help.xpm") ) {
    QMessageBox::message( "Sorry", "Could not load help.xpm" );
  }
  pb6->setPixmap( pb6_pixmap );

  ////////////////////////////////////////////////////

  pb7 = new QPushButton( this);
  pb7->setFocusPolicy(QWidget::NoFocus);
  QToolTip::add(pb7,"Cut");
  connect( pb7, SIGNAL( clicked() ), SLOT(cut() ) );

  if ( !pb7_pixmap.load(pixpath + "editcut.xpm") ) {
    QMessageBox::message( "Sorry", "Could not load editcut.xpm" );
  }
  pb7->setPixmap( pb7_pixmap );

  ////////////////////////////////////////////////////

  pb8 = new QPushButton( this);
  pb8->setFocusPolicy(QWidget::NoFocus);
  QToolTip::add(pb8,"Open File");
  connect( pb8, SIGNAL( clicked() ), SLOT(file_open() ) );

  if ( !pb8_pixmap.load(pixpath + "fileopen.xpm") ) {
    QMessageBox::message( "Sorry", "Could not load fileopen.xpm" );
  }
  pb8->setPixmap( pb8_pixmap );

  ////////////////////////////////////////////////////

  if(buttonbar_visible)
    options->changeItem("Hide &Button Bar",buttonID);
  else
    options->changeItem("Show &Button Bar",buttonID);

  if(statusbar_visible)
    options->changeItem("Hide &Status Bar",statusID);
  else
    options->changeItem("Show &Status Bar",statusID);

  ////////////////////////////////////////////////

  eframe = new KEdit (this, "eframe", "Untitled", KEdit::HAS_POPUP
                      | KEdit::ALLOW_OPEN | KEdit::ALLOW_SAVE | KEdit::ALLOW_SAVEAS);

  connect(eframe, SIGNAL(update_status_bar()),this,SLOT(statusbar_slot()));
  connect(eframe, SIGNAL(toggle_overwrite_signal()),this,SLOT(toggle_overwrite()));
  connect(eframe, SIGNAL(loading()),this,SLOT(loading_slot()));
  connect(eframe, SIGNAL(saving()),this,SLOT(saving_slot()));

  eframe->fill_column_is_set  = 	fill_column_is_set;
  eframe->word_wrap_is_set    = 	word_wrap_is_set;
  eframe->fill_column_value   = 	fill_column_value;

  set_colors();

  eframe->setFont(generalFont);

  eframe->setLineWidth (1);
  eframe->setEditMode(default_open);
  connect(eframe, SIGNAL(fileChanged()), SLOT(setFileCaption()));
  setSensitivity ();
  adjustSize ();
  eframe->setFocus ();
  setCaption (eframe->getName ());

  // Drag and drop

  KDNDDropZone * dropZone = new KDNDDropZone( this , DndURL);

  connect( dropZone, SIGNAL( dropAction( KDNDDropZone *) ), 
	   this, SLOT( slotDropEvent( KDNDDropZone *) ) );

  this->resize(editor_width,editor_height);

}


TopLevel::~TopLevel (){

  delete file;
  delete edit;
  delete help;
  delete options;
}




void TopLevel::copy(){
  
  eframe->copyText();

}

void TopLevel::insertFile(){

  int returncode;

retry_insertFile:

  returncode = eframe->insertFile();
  if(returncode == KEdit::KEDIT_OK)
    setStatusBar1Text("Done");
  if(returncode == KEdit::KEDIT_RETRY)
    goto retry_insertFile;

  statusbar_slot();
}




void TopLevel::insertDate(){
  
  int line, column;

  QString string;
  QDateTime dt = QDateTime::currentDateTime();
  string = dt.toString();

  eframe->getCursorPosition(&line,&column);
  eframe->insertAt(string,line,column);
  eframe->toggleModified(TRUE);

  statusbar_slot();
}

void TopLevel::paste(){

 eframe->paste();
 eframe->toggleModified(TRUE);

 statusbar_slot();
}

void TopLevel::cut(){

 eframe->cut();
 eframe->toggleModified(TRUE);
 statusbar_slot();

}

void TopLevel::file_new(){

  eframe->newFile(0);
  statusbar_slot();

}

void TopLevel::file_open(){
  
  int result;

tryagain_fileopen:

  result = eframe->openFile(0);
  switch (result){
  
  case KEdit::KEDIT_OK :	
    {
      QString string;
      setStatusBar1Text("Done");
      break;
    }

  case KEdit::KEDIT_RETRY :
    {
      goto tryagain_fileopen;
      break;
    }
  case KEdit::KEDIT_OS_ERROR :
    {
      break;
    }
  default:
    break;
  }
  
  statusbar_slot();

}


void TopLevel::file_open_url(){

  DlgLocation l( "Open Location:", "ftp://localhost/welcome", this );
  if ( l.exec() )
    {
      QString n = l.getText();
      if ( n.left(5) != "file:" && n.left(4) == "ftp:" )
	openNetFile( l.getText(), KEdit::OPEN_READWRITE );
      else
	openNetFile( l.getText(), KEdit::OPEN_READONLY );
    }
  
  statusbar_slot();
}

void TopLevel::file_save_url(){

  DlgLocation l( "Save to Location:", eframe->getName(), this );
  if ( l.exec() )
    {
      QString n = l.getText();
      if ( n.left(5) != "file:" && n.left(4) == "ftp:" )
	saveNetFile( l.getText() );
      else
	saveNetFile( l.getText() );
    }
  statusbar_slot();
}

void TopLevel::quiteditor(){
  
  bool frame_modified = false;
  bool result;
  bool result1;


  // check the normal case of only 1 editor window
  if(windowList.count() == 1){
    if(!eframe->isModified()){
      writeSettings();
      a->quit();
      return;
    }
    else{ /// modified
      result = QMessageBox::query ("Message", 
				 "This Document has been modified.\n"\
				 "Would you like to save it?");      

      if (result){ //modified we want to save
	if (eframe->doSave () != KEdit::KEDIT_OK){
	  result1 = QMessageBox::query ("Sorry", 
					"Could not save the file.\n"\
					"Exit anyways?");

	  if (!result1)
	    return;         // we don't want to exit.
	}
      }
      writeSettings();
      a->quit();
      return;
	
    }
  }

  // there is more than one window open. We need to ask a bit differently

  TopLevel *ptoplevel;
  for ( ptoplevel=windowList.first(); ptoplevel!= 0; ptoplevel=windowList.next() ){
    if (ptoplevel->eframe->isModified())
      frame_modified = true;
  }
  
  bool returncode;

  returncode = QMessageBox::query ("Message", 
		    "There are windows with modified content open.\n"\
		    "If you exit now, you will loose those changes\n"\
		    "Quit anyways?");

  if (returncode){
    writeSettings();
    a->quit();
  }

  return;

}


void TopLevel::newTopLevel(){

  TopLevel *t = new TopLevel ();
  t->show ();
  windowList.append( t );
}


void TopLevel::file_close(){

  bool result;
  bool result1;

  if (eframe->isModified ()) {
    result = QMessageBox::query ("Message", 
				 "This Document has been modified.\n"\
				 "Would you like to save it?");
    
    if (result){
      if (eframe->doSave () != KEdit::KEDIT_OK){
	result1 = QMessageBox::query ("Sorry", 
				      "Could not save the file.\n"\
				      "Exit anyways?");

	if (!result1){
	    return;         // we don't want to exit.
	  }
      }
    }
  }

  if ( windowList.count() > 1 ){  /* more than one window is open */
    
    windowList.remove( this );
    delete this;
  }	
  else{
    writeSettings();
    a->quit();
  }

}

void TopLevel::file_save(){

  int result =KEdit::KEDIT_OK;

  if ( eframe->isModified() ){
    result =  eframe->doSave(); // error messages are handled by ::KEdit

    if ( result == KEdit::KEDIT_OK ){
      setCaption(eframe->getName());
      QString string;
      string.sprintf("Wrote: %s",eframe->getName());
      setStatusBar1Text(string);
    }
  }
  else{
       setStatusBar1Text("No changes need to be saved");
  }


}

void TopLevel::setStatusBar1Text(QString text){

    statusbar_timer->stop();
    statusbar1->setText(text);
    statusbar_timer->start(10000,TRUE); // single shot

}


void TopLevel::file_save_as(){

  if (eframe->saveAs()== KEdit::KEDIT_OK){
    setCaption(eframe->getName());
    QString string;
    string.sprintf("Saved As: %s",eframe->getName());
    setStatusBar1Text(string);
  }
}


void TopLevel::fancyprint(){

  QPrinter prt;
  char buf[200];
  if ( prt.setup(0) ) {
    
    int y =10;
    QPainter p;
    p.begin( &prt );
    p.setFont( eframe->font() );
    QFontMetrics fm = p.fontMetrics();

    int numlines = eframe->numLines();
    for(int i = 0; i< numlines; i++){
      y += fm.ascent();
      QString line;
      line = eframe->textLine(i);
      line.replace( QRegExp("\t"), "        " );         
      strncpy(buf,line.data(),160);
      for (int j = 0 ; j <150; j++){
	if (!isprint(buf[j]))
	    buf[j] = ' ';
      }
      buf[line.length()] = '\0';
      p.drawText( 10, y, buf );
      y += fm.descent();    
    }

    p.end();
  }
  return ;
}


void TopLevel::about(){

  QMessageBox::message ("About kedit+", "kedit+ Version 0.4.1\n"\
			"Copyright 1997\nBernd Johannes Wuebben\n"\
			"wuebben@math.cornell.edu\n"\
			"                                                      \n"\
			"kedit+ is based in part on kedit\n"\
			"Versions 0.1 - 0.3.1\n"\
			"Copyright 1996\nAlexander Sanda\n"\
			"alex@darkstar.ping.at","Ok");

}

void TopLevel::helpselected(){
  
  if ( fork() == 0 )
    {
      QString path = DOCS_PATH;
      path += "/kedit+.html";
      execlp( "kdehelp", "kdehelp", path.data(), 0 );
      ::exit( 1 );      
      
    }	 

}

void TopLevel::search(){

      eframe->initSearch();
      statusbar_slot();
}

void TopLevel::font(){

  eframe->selectFont();
  generalFont = eframe->font();

}


void TopLevel::toggleStatusBar(){

  statusbar_visible = ! statusbar_visible;
  
  if(statusbar_visible){
    options->changeItem("Hide &Status Bar",statusID);
    statusbar1->show();
    statusbar2->show();
  }
  else{
    options->changeItem("Show &Status Bar",statusID);
    statusbar1->hide();
    statusbar2->hide();
  }

  this->resize(width(),height());

}


void TopLevel::toggleButtonBar(){

  buttonbar_visible = ! buttonbar_visible;
  if(buttonbar_visible){
    options->changeItem("Hide &Button Bar",buttonID);
    pb1->show();
    pb2->show();
    pb3->show();
    pb4->show();
    pb5->show();
    pb6->show();
    pb7->show();
  }
  else{
    options->changeItem("Show &Button Bar",buttonID);
    pb1->hide();
    pb2->hide();
    pb3->hide();
    pb4->hide();
    pb5->hide();
    pb6->hide();
    pb7->hide();
    
  }
  this->resize(width(),height());
}	


void TopLevel::closeEvent( QCloseEvent *e ){

  (void) e;

  file_close();

};

void TopLevel::search_again(){

      eframe->repeatSearch();
      statusbar_slot();
}


void TopLevel::setFileCaption(){

    setCaption(eframe->getName());
}


void TopLevel::resizeEvent (QResizeEvent *){

  int a,b,c;
  
  int line;
  int col;
  eframe->getCursorPosition(&line,&col);
  QString linenumber;
  linenumber.sprintf("Line: %d Col: %d",line,col);
  statusbar2->setText(linenumber);

  pb1->setGeometry( B_XOFFSET, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb8->setGeometry( 2*B_XOFFSET + BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb2->setGeometry( 3*B_XOFFSET + 2*BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb3->setGeometry( 4*B_XOFFSET + 3*BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb7->setGeometry( 5*B_XOFFSET + 4*BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb4->setGeometry( 6*B_XOFFSET + 5*BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb5->setGeometry( 7*B_XOFFSET + 6*BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );
  pb6->setGeometry( 8*B_XOFFSET + 7*BUTTON_WIDTH, menubar->height() + B_YOFFSET, 
		    BUTTON_WIDTH, BUTTON_HEIGHT );

  if(statusbar_visible)
    a = 1;
  else
    a = 0;

  if(buttonbar_visible)
    b = c = 1;
  else
    b = c = 0;


  statusbar1->setGeometry(0,height()-STATUSBAR_HEIGHT ,(width()*12)/16,STATUSBAR_HEIGHT);
  statusbar2->setGeometry((width()*3)/4,height()-STATUSBAR_HEIGHT ,
			  width()/4,STATUSBAR_HEIGHT);
  //  statusbar3->setGeometry((width()*10)/16,height()-STATUSBAR_HEIGHT ,
  //		  (width()*2)/16,STATUSBAR_HEIGHT);

  eframe->setGeometry (0, menubar->height () + b*TOOLBAR_HEIGHT + c*2*B_YOFFSET, width (), 
		       height () - menubar->height () - a*STATUSBAR_HEIGHT - 
		      c*2* B_YOFFSET - b*TOOLBAR_HEIGHT);

}

void TopLevel::statusbar_slot(){

  QString linenumber;
  linenumber.sprintf("Line: %d Col: %d",eframe->line_pos,eframe->col_pos);
  statusbar2->setText(linenumber);
  
}




void TopLevel::fill_column_slot(){



  FillDlg *dlg;
  dlg = new FillDlg(this,"print");

  struct fill_struct fillstr;
    
  fillstr.fill_column_is_set  = fill_column_is_set;
  fillstr.word_wrap_is_set    = word_wrap_is_set;;
  fillstr.fill_column_value   = fill_column_value;;
  
  dlg->setWidgets(fillstr);


  if(dlg->exec() == QDialog::Accepted){

    struct fill_struct fillstr = dlg->getFillCol();
    
    fill_column_is_set  = eframe->fill_column_is_set = fillstr.fill_column_is_set;
    word_wrap_is_set    = eframe->word_wrap_is_set   = fillstr.word_wrap_is_set;;
    fill_column_value   = eframe->fill_column_value =  fillstr.fill_column_value;;

  }

}


void TopLevel::print(){


  QString command;
  QString com;
  QString file;


  // TODO much better error checking. Does the system call succeed?
  // set the Statusbar accordingly

  if(strcmp(eframe->getName(), "Untitled")!= 0){

    if(eframe->isModified()) {           /* save old file */
      if((QMessageBox::query("Message", 
			     "The current document has been modified.\n"\
			     "Would you like to save the changes before \n"\
			     "printing this Document?"))) {

	if (eframe->doSave() != KEdit::KEDIT_OK){
	
	  QMessageBox::message("Sorry", "Could not Save the Changes\n", "OK");
	}
	else{
	  QString string;
	  string.sprintf("Saved: %s",eframe->getName());
	  setStatusBar1Text(string);
	}
      }
    }
  }

  PrintDialog *printing;
  printing = new PrintDialog(this,"print",true);
  printing->setWidgets(pi);


  if(printing->exec() == QDialog::Accepted){

    int result = 0;

    pi = printing->getCommand();

    pi.command.detach();
  if(strcmp(eframe->getName(), "Untitled")== 0){
      // we go through all of this so that we can print an "Untiteled" document
      // quickly without going through the hassle saving it. This will 
      // however result in a temporary filename and your printout will
      // usually show that temp name, such as /tmp/00432aaa
      // for a non "untiteled" documen we don't want that to happen so 
      // we asked the user to save before we print the document.

      QString tmpname = tmpnam(NULL);

      QFile file(tmpname);
      file.open(IO_WriteOnly);


      if(pi.selection){
	if(file.writeBlock(eframe->markedText().data(), 
			 eframe->markedText().length()) == -1) { 
	  result = KEdit::KEDIT_OS_ERROR;
	}
	else {
	  result = KEdit::KEDIT_OK;
	}
      }
      else{
	if(file.writeBlock(eframe->text().data(), eframe->text().length()) == -1) { 
	  result = KEdit::KEDIT_OS_ERROR;
	}
	else {
	  result = KEdit::KEDIT_OK;
	}	
      }

      file.close();
      // TODO error handling
    
      if (pi.raw){
	command = "lpr";
      }	
      else{
	command = pi.command;
	command.detach();
      }
  
      com.sprintf("%s %s ; rm %s &",command.data(),tmpname.data(),tmpname.data());
      system(com.data());
      QString string;
      if(pi.selection)
	string.sprintf("Printing: %s Untitled (Selection)", command.data());
      else
	string.sprintf("Printing: %s Untitled", command.data());
      setStatusBar1Text(string);

    }
    else{ // document name != Untiteled


      QString tmpname = tmpnam(NULL);
      if (pi.selection){ // print only the selection


	QFile file(tmpname);
	file.open(IO_WriteOnly);


	if(file.writeBlock(eframe->markedText().data(), 
			 eframe->markedText().length()) == -1) { 
	  result = KEdit::KEDIT_OS_ERROR;
	}
	file.close();
      
	// TODO error handling
      }

      if (pi.raw){
	command = "lpr";
      }	
      else{
	command = pi.command;
	command.detach();
      }
      
      if(!pi.selection){ // print the whole file
	
	com.sprintf("%s %s &",command.data(), eframe->getName());
	system(com.data());
	QString string;	
	string.sprintf("Printing: %s",com.data());
	setStatusBar1Text(string);
      
      }
      else{ // print only the selection
	
	com.sprintf("%s %s ; rm %s &",command.data(), 
		  tmpname.data(),tmpname.data());
	system(com.data());
	QString string;	
	string.sprintf("Printing: %s %s (Selection)",
		       command.data(), eframe->getName());
	setStatusBar1Text(string);
      
      }
      printf("%s\n",com.data());
    }

  }
}


void TopLevel::setSensitivity (){

}


void TopLevel::saving_slot(){

  setStatusBar1Text("Saving ...");

}


void TopLevel::loading_slot(){

  setStatusBar1Text("Loading ...");

}


void TopLevel::saveNetFile( const char *_url )
{
    
    netFile = _url;
    netFile.detach();
    KURL u( netFile.data() );
    if ( u.isMalformed() )
    {
	QMessageBox::message ("Sorry", "Malformed URL", "Ok");
	return;
    }

    
    // Just a usual file ?
    if ( strcmp( u.protocol(), "file" ) == 0 )
   {
     printf("usual file:%s\n",u.path());
      QString string;
      //      statusbar1->setText(string);
      eframe->doSave( u.path() );
      setStatusBar1Text("Saved");
      return;
    }
    
    if ( kfm != 0L )
    {
	QMessageBox::message ("Sorry", 
			      "KEdit is already waiting\n"\
			      "for an internet job to finish\n"\
			      "Please wait until has finished\n"\
			      "Alternatively stop the running one.", "Ok");
	return;
    }

    kfm = new KFM;
    if ( !kfm->isOK() )
    {
	QMessageBox::message ("Sorry", "Could not start or find KFM", "Ok");
	delete kfm;
	kfm = 0L;
	return;
    }
    
    tmpFile.sprintf( "file:/tmp/kedit%i", time( 0L ) );

    eframe->toggleModified( TRUE );
    eframe->doSave( tmpFile.data() + 5 );
    eframe->toggleModified( TRUE );
    
    connect( kfm, SIGNAL( finished() ), this, SLOT( slotKFMFinished() ) );
    kfm->copy( tmpFile.data(), netFile.data() );
    kfmAction = TopLevel::PUT;
}

void TopLevel::openNetFile( const char *_url, int _mode )
{
    
  QString string;
  netFile = _url;
  netFile.detach();
  KURL u( netFile.data() );
  if ( u.isMalformed() )
    {
	QMessageBox::message ("Sorry", "Malformed URL", "Ok");
	return;
    }

    // Just a usual file ?
    if ( strcmp( u.protocol(), "file" ) == 0 )
    {
      QString string;
      string.sprintf("Loading '%s'",u.path() );
      statusbar1->setText(string);
      eframe->loadFile( u.path(), _mode );
      setStatusBar1Text("Done");
      return;
    }
    
    if ( kfm != 0L )
    {
	QMessageBox::message ("Sorry", 
			      "KEdit is already waiting\n"\
			      "for an internet job to finish\n"\
			      "Please wait until has finished\n"\
			      "Alternatively stop the running one.", "Ok");
	return;
    }
    statusbar1->setText("Calling KFM");
    
    kfm = new KFM;
    setStatusBar1Text("Done");
    if ( !kfm->isOK() )
    {
	QMessageBox::message ("Sorry", "Could not start or find KFM", "Ok");
	delete kfm;
	kfm = 0L;
	return;
    }
    
    statusbar1->setText("Starting Job");
    tmpFile.sprintf( "file:/tmp/kedit%i", time( 0L ) );
    connect( kfm, SIGNAL( finished() ), this, SLOT( slotKFMFinished() ) );
    statusbar1->setText("Connected");
    kfm->copy( netFile.data(), tmpFile.data() );
    statusbar1->setText("Waiting...");
    kfmAction = TopLevel::GET;
    openMode = _mode;
}

void TopLevel::slotKFMFinished()
{
  QString string;
  string.sprintf("Finished '%s'",tmpFile.data());
  setStatusBar1Text(string);

    if ( kfmAction == TopLevel::GET )
    {
	KURL u( tmpFile.data() );
	eframe->loadFile( u.path(), openMode );
	eframe->setName( netFile.data() );
	setCaption( netFile.data() );
	
	// Clean up
	unlink( tmpFile.data() );
	delete kfm;
	kfm = 0L;
    }
    if ( kfmAction == TopLevel::PUT )
    {
	eframe->toggleModified( FALSE );
	unlink( tmpFile.data() );
	delete kfm;
	kfm = 0L;
    }
}

void TopLevel::slotDropEvent( KDNDDropZone * _dropZone )
{
    QStrList & list = _dropZone->getURLList();
    
    char *s;

    for ( s = list.first(); s != 0L; s = list.next() )
    {
	// Load the first file in this window
	if ( s == list.getFirst() && !eframe->isModified() )
	{
	    QString n = s;
	    if ( n.left(5) != "file:" && n.left(4) == "ftp:" )
		openNetFile( n.data(), KEdit::OPEN_READWRITE );
	    else
		openNetFile( n.data(), KEdit::OPEN_READONLY );
	}
	else
	{
	    setStatusBar1Text("New Window");
	    TopLevel *t = new TopLevel ();
	    t->show ();
	    windowList.append( t );
	    setStatusBar1Text("New Window Created");
	    QString n = s;
	    if ( n.left(5) != "file:" && n.left(4) == "ftp:" )
		t->openNetFile( n.data(), KEdit::OPEN_READWRITE );
	    else
		t->openNetFile( n.data(), KEdit::OPEN_READONLY );
	    setStatusBar1Text("Load Command Done");
	}
    }
}

void TopLevel::timer_slot(){

  statusbar1->setText("");

}


void TopLevel::set_foreground_color(){

  QColor color;
  
  color = forecolor;
  
  if(KColorDialog::getColor(color) != QDialog::Accepted)
    return;
  
  forecolor = color;
  set_colors();



}

void TopLevel::set_background_color(){

  QColor color;

  color = backcolor;

  if( KColorDialog::getColor(color) != QDialog::Accepted)
    return;
  
  backcolor = color;
  set_colors();
    
}


void TopLevel::set_colors(){


  QPalette mypalette = (eframe->palette()).copy();

  QColorGroup cgrp = mypalette.normal();
  QColorGroup ncgrp(forecolor,cgrp.background(),
		    cgrp.light(),cgrp.dark(),cgrp.mid(),forecolor,backcolor);

  mypalette.setNormal(ncgrp);
  mypalette.setDisabled(ncgrp);
  mypalette.setActive(ncgrp);

  eframe->setPalette(mypalette);
  eframe->setBackgroundColor(backcolor);

}


void TopLevel::readSettings(){

	// let's set the defaults 

	generalFont = QFont ("Fixed", 11, QFont::Bold);
	editor_width = 550;
	editor_height = 400;
	statusbar_visible = 1;
	buttonbar_visible = 1;
	forecolor = QColor(black);
	backcolor = QColor(white);
	fill_column_is_set = true;
	word_wrap_is_set = true;
	fill_column_value = 79;

	pi.command = "enscript -2rG";
	pi.raw = 1;
	pi.selection = 0;

	//////////////////////////////////////////////////

	QString str;
	
	config = a->getConfig();

	config->setGroup( "Text Font" );

	str = config->readEntry( "Family" );
	if ( !str.isNull() )
		generalFont.setFamily(str.data());
		
		
	str = config->readEntry( "Point Size" );
		if ( !str.isNull() )
		generalFont.setPointSize(atoi(str.data()));
		
	
	str = config->readEntry( "Weight" );
		if ( !str.isNull() )
	generalFont.setWeight(atoi(str.data()));
		
	
	str = config->readEntry( "Italic" );
		if ( !str.isNull() )
			if ( atoi(str.data()) != 0 )
				generalFont.setItalic(true);
	
	str = config->readEntry("Width");
		if ( !str.isNull() )
		  editor_width = atoi(str.data());

	str = config->readEntry("Height");
		if ( !str.isNull() )
		  editor_height = atoi(str.data());

	str = config->readEntry("StatusBar");
		if ( !str.isNull() )
		  statusbar_visible = atoi(str.data());

	str = config->readEntry("ButtonBar");
		if ( !str.isNull() )
		  buttonbar_visible = atoi(str.data());

	str = config->readEntry("PrntCmd1");
		if ( !str.isNull() )
		  pi.command = str;

	str = config->readEntry("PrintSelection");
		if ( !str.isNull() )
		  pi.selection = atoi(str.data());

	str = config->readEntry("PrintRaw");
		if ( !str.isNull() )
		  pi.raw = atoi(str.data());

	str = config->readEntry("UseFillColumn");
		if ( !str.isNull() )
		  fill_column_is_set = (bool) atoi(str.data());

	str = config->readEntry("WordWrap");
		if ( !str.isNull() )
		  word_wrap_is_set = (bool) atoi(str.data());

	str = config->readEntry("FillColumn");
		if ( !str.isNull() )
		  fill_column_value = atoi(str.data());


        str = config->readEntry( "ForeColor" );
		if ( !str.isNull() )
			forecolor.setNamedColor( str );

        str = config->readEntry( "BackColor" );
		if ( !str.isNull() )
			backcolor.setNamedColor( str );
			         

}

void TopLevel::writeSettings(){
		
	config = a->getConfig();
	
	config->setGroup( "Text Font" );

	config->writeEntry("Family", generalFont.family());
	
	QString pointSizeStr;
	pointSizeStr.sprintf("%d", generalFont.pointSize() );
	config->writeEntry("Point Size", pointSizeStr);
	
	QString weightStr;
	weightStr.sprintf("%d", generalFont.weight() );
	config->writeEntry("Weight", weightStr);
	
	QString italicStr;
	italicStr.sprintf("%d", (int)generalFont.italic() );
	config->writeEntry("Italic", italicStr);
	
	QString widthstring;
	widthstring.sprintf("%d", this->width() );
	config->writeEntry("Width",  widthstring);

	QString heightstring;
	heightstring.sprintf("%d", this->height() );

	config->writeEntry("Height",  heightstring);

	QString statusstring;
	statusstring.sprintf("%d", statusbar_visible );
	config->writeEntry("StatusBar", statusstring);

	QString buttonstring;
	buttonstring.sprintf("%d", buttonbar_visible );
	config->writeEntry("ButtonBar", buttonstring);


	config->writeEntry("PrntCmd1", pi.command);

	QString string;
	string.sprintf("%d", pi.selection );
	config->writeEntry("PrintSelection", string);

	string="";
	string.sprintf("%d", pi.raw );
	config->writeEntry("PrintRaw", string);

	string="";
	string.sprintf("%d", fill_column_is_set );
	config->writeEntry("UseFillColumn", string);

	string="";
	string.sprintf("%d", word_wrap_is_set );
	config->writeEntry("WordWrap", string);

	string="";
	string.sprintf("%d", fill_column_value );
	config->writeEntry("FillColumn", string);

	string="";
	string.sprintf("#%02x%02x%02x", forecolor.red(), 
		       forecolor.green(), forecolor.blue());
	config->writeEntry( "ForeColor", string );

	string="";
	string.sprintf("#%02x%02x%02x", backcolor.red(), 
		       backcolor.green(), backcolor.blue());
	config->writeEntry( "BackColor", string );


	config->sync();

	
}

void TopLevel::toggle_overwrite(){
  /*
  if(eframe->isOverwriteMode()){
        statusbar3->setText("OWR");
  }
  else{
    statusbar3->setText("INS");
  }
  */
}

int main (int argc, char **argv)
{
    if (!strcmp (argv[0], "kless"))
       default_open = KEdit::OPEN_READONLY;
    else
       default_open = KEdit::OPEN_READWRITE;

    a = new KApplication (argc, argv, "kedit");

    a->setFont (QFont ("Helvetica", 12));

    /*
     * check cmdline args
     * FIXME: not completely done yet
     */

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (*argv[i] == '-')	/* ignore options */
                continue;

	    TopLevel *t = new TopLevel ();
	    t->show ();
	    TopLevel::windowList.append( t );

	    QString f = argv[i];
	    
	    if ( f.find( ":/" ) == -1 && f.left(1) != "/" )
	    {
		char buffer[ 1024 ];
		getcwd( buffer, 1023 );
		f.sprintf( "%s/%s", buffer, argv[i] );
	    }
		
            t->openNetFile( f.data(), default_open );
            t->setCaption( f.data() );
        }
    }
    else
    {
	TopLevel *t = new TopLevel ();
	t->show ();
	TopLevel::windowList.append( t );
    }
    
    return a->exec ();
}
