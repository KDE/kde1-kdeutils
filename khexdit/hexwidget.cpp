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

#include "hexwidget.moc"

QList<HexWidget> HexWidget::windowList;
KApplication *a;

int HexWidget::initMenu() {
  
  windowList.append(this);
  CurrentFile=new HexFile(this);

  QPopupMenu *file = new QPopupMenu;
  file->insertItem( "New Window", ID_FILE_NEWWIN);
  file->insertSeparator();
  file->insertItem( "Open", ID_FILE_OPEN );
  file->insertItem( "Save", ID_FILE_SAVE);
  file->insertItem( "Save As", ID_FILE_SAVEAS);
  file->insertItem( "Close", ID_FILE_CLOSE);
  file->insertSeparator();
  file->insertItem( "Quit", ID_FILE_QUIT);

  QPopupMenu *view = new QPopupMenu;
  view->insertItem( "Toggle Toolbar", ID_VIEW_TOOLBAR);
  //  view->insertItem( "Toggle Statusbar", ID_VIEW_STATUSBAR);
  
  QPopupMenu *help = new QPopupMenu;
  help->insertItem( "Help on KHexdit", ID_HELP_HELP);
  help->insertItem( "About KHexdit", ID_HELP_ABOUT);
  
  connect (file, SIGNAL (activated (int)), SLOT (menuCallback (int)));
  connect (help, SIGNAL (activated (int)), SLOT (menuCallback (int)));
  connect (view, SIGNAL (activated (int)), SLOT (menuCallback (int)));

  KMenuBar *menu = new KMenuBar( this );
  CHECK_PTR( menu );
  menu->insertItem( "&File", file );
  menu->insertItem( "&View", view );
  menu->insertSeparator();
  menu->insertItem( "&Help", help );
  menu->show();
  setMenu(menu);

  QString PIXDIR=kapp->kdedir();
  PIXDIR += "/lib/pics/toolbar/";
  KPixmap pixmap;
  
  KToolBar *toolbar = new KToolBar(this);
  pixmap.load(PIXDIR + "filenew.xpm");
  toolbar->insertItem(pixmap,ID_FILE_NEWWIN, TRUE, "New Window");
  pixmap.load(PIXDIR + "fileopen.xpm");
  toolbar->insertItem(pixmap,ID_FILE_OPEN, TRUE, "Open a file");
  pixmap.load(PIXDIR + "filefloppy.xpm");
  toolbar->insertItem(pixmap,ID_FILE_SAVE, TRUE, "Save the file");
  toolbar->insertSeparator();
  pixmap.load(PIXDIR + "page.xpm");
  toolbar->insertItem(pixmap,ID_EDIT_CUT, FALSE, "Not Implemented");
  pixmap.load(PIXDIR + "contents.xpm");
  toolbar->insertItem(pixmap,ID_EDIT_COPY, FALSE, "Not Implemented");
  pixmap.load(PIXDIR + "devious.xpm");
  toolbar->insertItem(pixmap,ID_EDIT_PASTE, FALSE, "Not Implemented");
  toolbar->insertSeparator();
  pixmap.load(PIXDIR + "fileprint.xpm");
  toolbar->insertItem(pixmap,ID_FILE_PRINT, FALSE, "Not Implemendted");
  toolbar->insertSeparator();
  pixmap.load(PIXDIR + "help.xpm");
  toolbar->insertItem(pixmap,ID_HELP_ABOUT, TRUE, "About Hexdit");

  addToolBar(toolbar);
  toolbar->setPos(KToolBar::Top);
  toolbar->show();
  connect(toolbar, SIGNAL(clicked(int)), this, SLOT(menuCallback(int)));

  // KStatusBar *stat = new KStatusBar(this);
  // stat->show();
  // setStatusBar(stat);

  setView(CurrentFile);
  KDNDDropZone * dropZone = new KDNDDropZone( this , DndURL);
  connect( dropZone, SIGNAL( dropAction( KDNDDropZone *) ), 
	   this, SLOT( slotDropEvent( KDNDDropZone *) ) );

  resize(640,380);
  show();
  return 0;
};

void HexWidget::menuCallback(int item) {
  switch (item) {
  case ID_FILE_NEWWIN:
    new HexWidget();
    break;
  case ID_FILE_OPEN: {
    if (CurrentFile->isModified()) 
      if((QMessageBox::query("Warning", "The current file has been modified.\n\rDo you want to save it ?"))) 
	CurrentFile->save();
    
    QFileDialog *log=new QFileDialog;
    QString fileName = log->getOpenFileName();

    open((const char*)fileName, READWRITE);
    break;
  };
  case ID_FILE_SAVE:
    if (CurrentFile->isModified())
      CurrentFile->save();
    break;
  case ID_FILE_SAVEAS: {
    QFileDialog *log = new QFileDialog;
    QString fileName = log->getOpenFileName();
    
    CurrentFile->setFileName(fileName.data());
    CurrentFile->save();
    break;
  };
  case ID_FILE_CLOSE: {
    if (CurrentFile->isModified()) {
      if (!QMessageBox::query("File changed",
			     "Discard your changes?","Yes","No")) 
	return;
    };
    windowList.remove(this);
    if (windowList.isEmpty())
      kapp->quit();
    else
      delete this;
    break;
  };
  case ID_FILE_QUIT: {
    kapp->quit();
    break;
  };

  case ID_VIEW_TOOLBAR: 
    enableToolBar();
    break;
    
  case ID_VIEW_STATUSBAR:
    enableStatusBar();
    break;

  case ID_HELP_HELP: {
    kapp->invokeHTMLHelp( "khexdit.html", "" );
    break;
  };

  case ID_HELP_ABOUT: {
     QString str;
     str.sprintf( "KHexdit 0.4 \n\nby Stephan Kulow  (coolo@itm.mu-luebeck.de)");
     KMsgBox::message( 0, "About KHexdit", (const char *)str,
		       KMsgBox::INFORMATION, "Close" );
     //     QMessageBox::message("About KHexdit", str, "OK"); 
     break;
  };
  };
};

void HexWidget::open(const char* fileName, KIND_OF_OPEN kind) {
  if (kind);
    
  if (fileName) {                 // got a file name
    if (!CurrentFile)
      CurrentFile = new HexFile(fileName,this);
    else
      CurrentFile->open(fileName);
    CurrentFile->setFocus();
    char Caption[300];
    sprintf(Caption,"KHexdit: %s",CurrentFile->Title());
    setCaption(Caption);
    update();
  }
  return;
};

void HexWidget::openURL(const char *_url, KIND_OF_OPEN _mode) {
  /* This code is from KEDIT (torben's I guess) */
  netFile = _url;
  netFile.detach();
  KURL u( netFile.data() );
  if ( u.isMalformed())   {
    QMessageBox::message ("Error", "Malformed URL", "Ok");
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
      QMessageBox::message ("Error", "KEdit is already waiting\n\rfor an internet job to finish\n\rWait until this one is finished\n\ror stop the running one.", "Ok");
      return;
    }
  
  kfm = new KFM;

  if ( !kfm->isOK() )
    {
      QMessageBox::message ("Error", "Could not start KFM", "Ok");
      delete kfm;
      kfm = 0L;
      return;
    }
  
  tmpFile.sprintf( "file:/tmp/khexdit%i", time( 0L ) );
  connect( kfm, SIGNAL( finished() ), this, SLOT( slotKFMFinished() ) );
  
  kfm->copy( netFile.data(), tmpFile.data() );
  
  kfmAction = HexWidget::GET;
  //openMode = _mode;
};

HexWidget::HexWidget() {
  initMenu();
  setCaption("KHexdit");
};

HexWidget::HexWidget(const char* file) {
  initMenu();
  openURL(file,READWRITE);
};

HexWidget::~HexWidget() {
};

void HexWidget::closeEvent ( QCloseEvent *e) {
  windowList.remove(this);
  if (windowList.isEmpty())
    kapp->quit();
  e->accept();
};

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
  a = new KApplication(argc,argv);
  
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
    };
  } else
    new HexWidget();
  return a->exec();
};






