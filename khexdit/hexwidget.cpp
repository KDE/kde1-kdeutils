#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <unistd.h>
#include <time.h>
#include <qpopmenu.h>
#include <qkeycode.h>
#include <qaccel.h> 
#include <qfiledlg.h> 
#include <string.h>
#include <stdlib.h>
#include <kmsgbox.h> 
#include <qmsgbox.h> 
#include <ktopwidget.h>
#include <ktoolbar.h>
#include "hexwidget.h"
#include "klocale.h"
#include <kiconloader.h>
#include <kwm.h>

QList<HexWidget> HexWidget::windowList;

int HexWidget::initMenu() {

  windowList.append(this);
  KWM::setIcon(winId(), kapp->getIcon());
  KWM::setMiniIcon(winId(), kapp->getMiniIcon());
  
  CurrentFile=new HexFile(this);
  
  QPopupMenu *file = new QPopupMenu;
  file->insertItem( klocale->translate("New Window"), ID_FILE_NEWWIN);
  file->insertSeparator();
  file->insertItem(  klocale->translate("Open"), ID_FILE_OPEN );
  file->insertItem(  klocale->translate("Save"), ID_FILE_SAVE);
  file->insertItem(  klocale->translate("Save As"), ID_FILE_SAVEAS);
  file->insertItem(  klocale->translate("Close"), ID_FILE_CLOSE);
  file->insertSeparator();
  file->insertItem(  klocale->translate("Quit"), ID_FILE_QUIT);

  QPopupMenu *view = new QPopupMenu;
  view->insertItem( klocale->translate("Toggle Toolbar"), ID_VIEW_TOOLBAR);
  //  view->insertItem( "Toggle Statusbar", ID_VIEW_STATUSBAR);
  
  QPopupMenu *help = new QPopupMenu;
  help->insertItem( klocale->translate("Contents"), ID_HELP_HELP);
  help->insertItem( klocale->translate("About"), ID_HELP_ABOUT);
  
  connect (file, SIGNAL (activated (int)), SLOT (menuCallback (int)));
  connect (help, SIGNAL (activated (int)), SLOT (menuCallback (int)));
  connect (view, SIGNAL (activated (int)), SLOT (menuCallback (int)));

  menu = new KMenuBar( this );
  CHECK_PTR( menu );
  menu->insertItem( klocale->translate("&File"), file );
  menu->insertItem( klocale->translate("&View"), view );
  menu->insertSeparator();
  menu->insertItem( klocale->translate("&Help"), help );
  menu->show();
  setMenu(menu);

  KIconLoader *loader = kapp->getIconLoader();
  toolbar = new KToolBar(this);

  toolbar->insertButton(loader->loadIcon("filenew.xpm"),
			ID_FILE_NEWWIN, TRUE, klocale->translate("New Window"));
  toolbar->insertButton(loader->loadIcon("fileopen.xpm"),ID_FILE_OPEN, TRUE, 
			klocale->translate("Open a file"));
  toolbar->insertButton(loader->loadIcon("filefloppy.xpm"), ID_FILE_SAVE, TRUE, 
			klocale->translate("Save the file"));
  toolbar->insertSeparator();
  toolbar->insertButton(loader->loadIcon("page.xpm"),ID_EDIT_CUT, FALSE, 
			klocale->translate("Not implemented"));
  toolbar->insertButton(loader->loadIcon("contents.xpm"),ID_EDIT_COPY, FALSE, 
			klocale->translate("Not implemented"));
  toolbar->insertButton(loader->loadIcon("devious.xpm"),ID_EDIT_PASTE, FALSE, 
			klocale->translate("Not implemented"));
  toolbar->insertSeparator();
  toolbar->insertButton(loader->loadIcon("fileprint.xpm"),ID_FILE_PRINT, FALSE, 
			klocale->translate("Not implemented"));
  toolbar->insertSeparator();
  toolbar->insertButton(loader->loadIcon("help.xpm"),ID_HELP_ABOUT, TRUE, 
			klocale->translate("About Hex Editor"));

  addToolBar(toolbar);
  toolbar->setBarPos(KToolBar::Top);
  toolbar->show();
  connect(toolbar, SIGNAL(clicked(int)), this, SLOT(menuCallback(int)));

  // KStatusBar *stat = new KStatusBar(this);
  // stat->show();
  // setStatusBar(stat);

  kfm = 0L;
  setView(CurrentFile);
  KDNDDropZone * dropZone = new KDNDDropZone( this , DndURL);
  connect( dropZone, SIGNAL( dropAction( KDNDDropZone *) ), 
	   this, SLOT( slotDropEvent( KDNDDropZone *) ) );

  resize(640,380);
  show();
  return 0;
}

void HexWidget::menuCallback(int item) {
  switch (item) {
  case ID_FILE_NEWWIN:
    new HexWidget();
    break;
  case ID_FILE_OPEN: {
    if (CurrentFile->isModified()) 
      if (QMessageBox::query(klocale->translate("Warning"), 
			     klocale->translate("The current file has been modified.\n\nDo you want to save it ?"),
			     klocale->translate("Yes"),
			     klocale->translate("No")))
	  CurrentFile->save();
    
    QFileDialog *log=new QFileDialog;
    QString fileName = log->getOpenFileName();

    open((const char*)fileName, READWRITE);
    break;
  }
  case ID_FILE_SAVE:
    if (CurrentFile->isModified())
      CurrentFile->save();
    break;
  case ID_FILE_SAVEAS: {
    QFileDialog *log = new QFileDialog;
    QString fileName = log->getSaveFileName();
    
    CurrentFile->setFileName(fileName.data());
    CurrentFile->save();
    break;
  }
  case ID_FILE_CLOSE: {
    if (CurrentFile->isModified()) {
      if (!QMessageBox::query(klocale->translate("File changed"),
			      klocale->translate("Discard your changes?"),
			      klocale->translate("Yes"),
			      klocale->translate("No"))) 
	return;
    }
    windowList.remove(this);
    if (windowList.isEmpty())
      kapp->quit();
    else
      delete this;
    break;
  }
  case ID_FILE_QUIT: {
    kapp->quit();
    break;
  }

  case ID_VIEW_TOOLBAR: 
    enableToolBar();
    break;
    
  case ID_VIEW_STATUSBAR:
    enableStatusBar();
    break;

  case ID_HELP_HELP: {
    kapp->invokeHTMLHelp( "khexdit/khexdit.html", "" );
    break;
  }

  case ID_HELP_ABOUT: {
     QString str;
     str.sprintf( klocale->translate("Hex Editor 0.4 \n\nby Stephan Kulow  (coolo@itm.mu-luebeck.de)") );
     KMsgBox::message( 0, klocale->translate("About Hex Editor"), 
		       (const char *)str,
		       KMsgBox::INFORMATION, klocale->translate("Close") );
     break;
  }
  }
}

void HexWidget::open(const char* fileName, KIND_OF_OPEN kind) {
  if (kind);
    
  if (fileName) {                 // got a file name
    if (!CurrentFile)
      CurrentFile = new HexFile(fileName,this);
    else
      CurrentFile->open(fileName);
    CurrentFile->setFocus();
    char Caption[300];
    sprintf(Caption,"%s: %s",kapp->getCaption(),CurrentFile->Title());
    setCaption(Caption);
    update();
  }
  return;
}

void HexWidget::openURL(const char *_url, KIND_OF_OPEN _mode) {
  /* This code is from KEDIT (torben's I guess) */
  netFile = _url;
  netFile.detach();
  KURL u( netFile.data() );
  if ( u.isMalformed())   {
    QMessageBox::message (klocale->translate("Error"), klocale->translate("Malformed URL"), klocale->translate("Ok"));
    return;
  }
  
  // Just a usual file ?
  if ( strcmp( u.protocol(), "file" ) == 0 )
    {
      open( u.path(), _mode );
      return;
    }
  
  
  if ( kfm != 0L )
    {
	QMessageBox::message (klocale->translate("Error"), 
			      klocale->translate("KHexdit is already waiting\nfor an internet job to finish\n\nWait until this one is finished\nor stop the running one."),
			      klocale->translate("OK"));
	return;
    }
  
  kfm = new KFM;

  if ( !kfm->isOK() )
    {
      QMessageBox::message (klocale->translate("Error"), 
			    klocale->translate("Could not start KFM"), 
			    klocale->translate("OK"));
      delete kfm;
      kfm = 0L;
      return;
    }
  
  tmpFile.sprintf( "file:/tmp/khexdit%i", time( 0L ) );
  connect( kfm, SIGNAL( finished() ), this, SLOT( slotKFMFinished() ) );
  
  kfm->copy( netFile.data(), tmpFile.data() );
  
  kfmAction = HexWidget::GET;
  //openMode = _mode;
}

HexWidget::HexWidget() {
  initMenu();
  setCaption(kapp->getCaption());
}

HexWidget::HexWidget(const char* file) {
  initMenu();
  openURL(file,READWRITE);
}

HexWidget::~HexWidget() {
  delete toolbar;
  delete CurrentFile;
  delete kfm;
  delete menu;
}

void HexWidget::closeEvent ( QCloseEvent *e) {
  windowList.remove(this);
  if (windowList.isEmpty())
    kapp->quit();
  e->accept();
}

void HexWidget::slotDropEvent( KDNDDropZone * _dropZone ) {
  QStrList & list = _dropZone->getURLList();
  
  char *s;
  
  for ( s = list.first(); s != 0L; s = list.next() )
    {
      // Load the first file in this window
      
      if ( s == list.getFirst() && !CurrentFile->isModified() ) 
	{
	  QString n = s;
	  if ( n.left(5) != "file:" && n.left(4) == "ftp:" )
	    openURL( n.data(), READWRITE );
	  else
	    openURL( n.data(), READONLY );
	}
      else 
	{
	  HexWidget *h = new HexWidget();
	  QString n = s;
	  if ( n.left(5) != "file:" && n.left(4) == "ftp:" )
	    h->openURL( n.data(), READWRITE );
	  else
	    h->openURL( n.data(), READONLY );
	}
    }    
}

int main(int argc, char **argv) {
  KApplication a(argc,argv,"khexdit");  

  if (argc>1) {
    for (int i=1; i < argc; i++) {
      if (*argv[i] == '-')	/* ignore options */
	continue;
      
      QString f = argv[i];
      
      if ( f.find( ":/" ) == -1 && f.left(1) != "/" )
	{
	  char buffer[ 1024 ];
	  getcwd( buffer, 1023 );
	  f.sprintf( "file:%s/%s", buffer, argv[i] );
	}
      
      new HexWidget(f.data());
    }
  } else {
    HexWidget *hw = new HexWidget();
    a.setMainWidget(hw);
  }
  
  return a.exec();
}

#include "hexwidget.moc"



