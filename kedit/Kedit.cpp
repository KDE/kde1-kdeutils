/*
 *  kedit
 *
 * simple editor (like Win* notepad) for the kde project.
 *
 * created: 96-11-17
 *
 * by Alexander Sanda (alex@darkstar.ping.at)
 *
 * $Id$
 */

#include <qpopmenu.h>
#include <qmenubar.h>
#include <qapp.h>
#include <qkeycode.h>
#include <qaccel.h>
#include <qobject.h>
#include <qmlined.h>
#include <qradiobt.h>
#include <qfiledlg.h>
#include <qchkbox.h>
#include <qmsgbox.h>
#include <qgrpbox.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <string.h>

#include <kfm.h>
#include <kapp.h>
#include <kurl.h>

#include "fileselect.h"
#include "Kedit.h"
#include "Kedit.moc"
#include "constants.h"

QList<TopLevel> TopLevel::windowList;

KApplication *a;

int default_open = KEdit::OPEN_READWRITE;

TopLevel::TopLevel (QWidget * parent, const char *name)
    : QWidget (parent, name)
{
  setMinimumSize (800, 500);

  kfm = 0L;
  windowList.setAutoDelete( FALSE );
  
  /*
   * create the menubar
   */

  file = new QPopupMenu ();
  edit = new QPopupMenu ();
  help = new QPopupMenu ();

  help->insertItem ("About...", ID_HELP_ABOUT, -1);
  help->insertItem ("Help", ID_HELP_HELP,-1);
  
  file->insertItem ("New Window", ID_FILE_NEWWIN, -1);
  file->insertSeparator (-1);
  file->insertItem ("Open...", ID_FILE_OPEN, -1);
  file->setAccel (ALT + Key_O, ID_FILE_OPEN);
  file->insertItem ("Open URL...", ID_FILE_OPEN_URL, -1);
  file->setAccel (ALT + Key_L, ID_FILE_OPEN_URL);
  file->insertItem ("Save", ID_FILE_SAVE, -1);
  file->setAccel (ALT + Key_S, ID_FILE_SAVE);
  file->insertItem ("Save as...", ID_FILE_SAVEAS, -1);
  file->setAccel (ALT + Key_A, ID_FILE_SAVEAS);
  file->insertItem ("Save to URL...", ID_FILE_SAVE_URL, -1);
  file->setAccel (ALT + Key_V, ID_FILE_SAVE_URL );
  file->insertItem ("Close", ID_FILE_CLOSE, -1);
  file->setAccel (ALT + Key_C, ID_FILE_CLOSE);
  file->insertSeparator (-1);
  file->insertItem ("Quit", ID_FILE_QUIT, -1);
  file->setAccel (ALT + Key_Q, ID_FILE_QUIT);

  edit->insertItem("Search...", ID_EDIT_SEARCH, -1);
  edit->insertItem("Repeat last search", ID_EDIT_SEARCHAGAIN, -1);
  edit->setAccel(ALT + Key_F, ID_EDIT_SEARCH);
  edit->setAccel(ALT + Key_G, ID_EDIT_SEARCHAGAIN);
  
  menubar = new QMenuBar (this, "menubar");
  menubar->insertItem ("File", file, 1, -1);
  menubar->setLineWidth (1);
  menubar->insertItem ("Edit", edit, 2, -1);
  menubar->insertSeparator();
  menubar->insertItem ("Help", help, 2, -1);

  connect (file, SIGNAL (activated (int)), this, SLOT (menuCallback (int)));
  connect (help, SIGNAL (activated (int)), this, SLOT (menuCallback (int)));
  connect (edit, SIGNAL (activated (int)), this, SLOT (menuCallback (int)));
  
  eframe = new KEdit (this, "eframe", "Untitled", KEdit::HAS_POPUP
                      | KEdit::ALLOW_OPEN | KEdit::ALLOW_SAVE | KEdit::ALLOW_SAVEAS);
  eframe->setFont (QFont ("fixed", 14, QFont::Bold));
  eframe->setLineWidth (1);
  eframe->setEditMode(default_open);
  connect(eframe, SIGNAL(fileChanged()), SLOT(setFileCaption()));
  setSensitivity ();
  adjustSize ();
  eframe->setFocus ();
  setCaption (eframe->getName ());

  // Drag and drop
  KDNDDropZone * dropZone = new KDNDDropZone( this , DndURL);
  connect( dropZone, SIGNAL( dropAction( KDNDDropZone *) ), this, SLOT( slotDropEvent( KDNDDropZone *) ) );

  setGeometry( x(), y(), 400, 400 );
}

TopLevel::~TopLevel ()
{
  delete file;
  delete edit;
  delete help;
}


/*
 * now, our slot functions
 * menu callback handles _all_ menu entries (via menuentry id#)
 */

void TopLevel::menuCallback (int item)
{ 

  switch (item) {

  case ID_FILE_OPEN:
      eframe->openFile(0);
      break;

  case ID_FILE_OPEN_URL:
      {
	  DlgLocation l( "Open Location:", "ftp://localhost/welcome", this );
	  if ( l.exec() )
	  {
	      QString n = l.getText();
	      if ( n.left(5) != "file:" && n.left(4) == "ftp:" )
		  openNetFile( l.getText(), KEdit::OPEN_READWRITE );
	      else
		  openNetFile( l.getText(), KEdit::OPEN_READONLY );
	  }
      }
      break;

  case ID_FILE_SAVE_URL:
      {
	  DlgLocation l( "Save to Location:", eframe->getName(), this );
	  if ( l.exec() )
	  {
	      QString n = l.getText();
	      if ( n.left(5) != "file:" && n.left(4) == "ftp:" )
		  saveNetFile( l.getText() );
	      else
		  saveNetFile( l.getText() );
	  }
      }
      break;

  case ID_FILE_QUIT:
  quit:
    a->quit();
    break;

  case ID_FILE_NEWWIN:
      {
	  TopLevel *t = new TopLevel ();
	  t->show ();
	  windowList.append( t );
      }
    break;

  case ID_FILE_CLOSE:
    if (eframe->isModified ()) {
        if (QMessageBox::query ("Warning", "The file has been modified.\n\rDo you want to save ?")) {
            if (eframe->doSave () != KEdit::KEDIT_OK)
                break;
        }
    }
    if ( windowList.count() > 1 )  /* more than one window is open */
    {
	windowList.remove( this );
	delete this;
    }
    else
      goto quit;
    break;

  case ID_FILE_SAVE:
      // eframe->doSave();
      if ( eframe->isModified() )
	  saveNetFile( eframe->getName() );
    break;

  case ID_FILE_SAVEAS:
    if (eframe->saveAs())
      setCaption (eframe->getName ());
    break;

  case ID_HELP_ABOUT:
  QMessageBox::message ("About Kedit", "Kedit Version 0.3.1\n\rWritten by A. Sanda.\n\r\n\rThis program is free software, you may distribute it\n\runder the terms of the GNU General Public License V2", "Ok");
  break;

  case ID_HELP_HELP:
    if ( fork() == 0 )
    {
      char helpurl_s[100];
      helpurl_s[0]='\0';
      strcat(helpurl_s, DOCS_PATH);
      strcat(helpurl_s,"kedit.html");
      
      execlp( "kdehelp", "kdehelp", helpurl_s, 0 );
      exit( 1 );
    } 
    
    break;
    
  case ID_EDIT_SEARCH:
      eframe->initSearch();
      break;

  case ID_EDIT_SEARCHAGAIN:
      eframe->repeatSearch();
      break;
      
  default:
    break;
  }
}

void TopLevel::setFileCaption()
{
    setCaption(eframe->getName());
}

void TopLevel::resizeEvent (QResizeEvent *)
{
    eframe->setGeometry (0, menubar->height (), width (), height () - menubar->height ());
}

void TopLevel::setSensitivity ()
{
}

void TopLevel::saveNetFile( const char *_url )
{
    printf("NETFILE '%s'\n",_url );
    
    netFile = _url;
    netFile.detach();
    KURL u( netFile.data() );
    if ( u.isMalformed() )
    {
	QMessageBox::message ("Error", "Malformed URL", "Ok");
	return;
    }

    printf("URL is ok\n");
    
    // Just a usual file ?
    if ( strcmp( u.protocol(), "file" ) == 0 )
    {
	printf("Loading '%s'\n",u.path() );
	eframe->doSave( u.path() );
	printf("Done\n");
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
	QMessageBox::message ("Error", "Could not start or find KFM", "Ok");
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
    printf("NETFILE '%s'\n",_url );
    
    netFile = _url;
    netFile.detach();
    KURL u( netFile.data() );
    if ( u.isMalformed() )
    {
	QMessageBox::message ("Error", "Malformed URL", "Ok");
	return;
    }

    printf("URL is ok\n");
    
    // Just a usual file ?
    if ( strcmp( u.protocol(), "file" ) == 0 )
    {
	printf("Loading '%s'\n",u.path() );
	eframe->loadFile( u.path(), _mode );
	printf("Done\n");
	return;
    }
    
    printf("Is internet\n");
    
    if ( kfm != 0L )
    {
	QMessageBox::message ("Error", "KEdit is already waiting\n\rfor an internet job to finish\n\rWait until this one is finished\n\ror stop the running one.", "Ok");
	return;
    }

    printf("Creating KFM\n");
    kfm = new KFM;
    printf("Done\n");
    if ( !kfm->isOK() )
    {
	QMessageBox::message ("Error", "Could not start or find KFM", "Ok");
	delete kfm;
	kfm = 0L;
	return;
    }
    
    printf("Starting Job\n");
    tmpFile.sprintf( "file:/tmp/kedit%i", time( 0L ) );
    connect( kfm, SIGNAL( finished() ), this, SLOT( slotKFMFinished() ) );
    printf("Connected\n");
    kfm->copy( netFile.data(), tmpFile.data() );
    printf("Waiting...\n");
    kfmAction = TopLevel::GET;
    openMode = _mode;
}

void TopLevel::slotKFMFinished()
{
    printf("Finished '%s'\n",tmpFile.data());
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
	    printf("New Window\n");
	    TopLevel *t = new TopLevel ();
	    t->show ();
	    windowList.append( t );
	    printf("Created\n");
	    QString n = s;
	    if ( n.left(5) != "file:" && n.left(4) == "ftp:" )
		t->openNetFile( n.data(), KEdit::OPEN_READWRITE );
	    else
		t->openNetFile( n.data(), KEdit::OPEN_READONLY );
	    printf("Load Command done\n");
	}
    }
}

int main (int argc, char **argv)
{
    if (!strcmp (argv[0], "kless"))
       default_open = KEdit::OPEN_READONLY;
    else
       default_open = KEdit::OPEN_READWRITE;

    a = new KApplication (argc, argv);

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
