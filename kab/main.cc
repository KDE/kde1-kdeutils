/* The main-function for the addressbook application 
 *
 * the Configuration Database library
 * copyright:  (C) Mirko Sucker, 1998
 * license:    GNU Public License, Version 2
 * mail to:    Mirko Sucker <mirko.sucker@unibw-hamburg.de>
 * requires:   C++-compiler, STL, string class, Qt > 1.33,
 *             NANA (for debugging)
 * $Revision$
 */

#include <qimage.h>
#include "toplevelwidget.h"
#include <kapp.h>
#include "jpeg.h"
#include "debug.h"

extern "C" {
#include <signal.h>
	   }

void exit_handler()
{
  ID(bool GUARD=true);
  LG(GUARD, "addressbook exit_handler: called.\n");
  // ########################################################
  LG(GUARD, "addressbook exit_handler: clearing ConfigDB "
     "lockfiles.\n");
  ConfigDB::CleanLockFiles(0);
  LG(GUARD, "addressbook exit_handler: done.\n");
  // ########################################################  
}

void signal_handler(int sig)
{
  ID(bool GUARD=true);
  LG(GUARD, "addressbook signal_handler: called.\n");
  // ########################################################  
  exit(sig);
  // ########################################################  
}
  
int main(int argc, char** argv)
{
  ID(bool GUARD=true);
  LG(GUARD, "addressbook main: starting.\n");
  // ########################################################  
  KApplication app(argc, argv, "kab");
  AuthorEmailAddress="mirko@kde.org"; // static, public
#ifdef HAVE_LIBJPEG
  QImageIO::defineIOHandler("JFIF","^\377\330\377\340", 
			    0, read_jpeg_jfif, NULL);
  LG(GUARD, "addressbook main: registered JPEG reader.\n");
#else
  L("addressbook main: the JPEG reader has not been "
    "included in this executable.\n");   
  CHECK(false);
#endif
  LG(GUARD, "addressbook main: installing signal "
     "handler.\n");
  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGKILL, signal_handler);
  signal(SIGTERM, signal_handler);
  LG(GUARD, "addressbook main: installing exit handler.\n");
  atexit(exit_handler);
  LG(GUARD, "addressbook main: creating addressbook.\n");
  ABTLWidget* db=new ABTLWidget;
  app.setMainWidget(db);
  db->show();
  LG(GUARD, "addressbook main: executing X application.\n");
  return app.exec();
  // ########################################################
}


