/* -*- C++ -*-
 * This file implements the toplevel widget for the addressbook.
 * 
 * the KDE addressbook.
 * copyright:  (C) Mirko Sucker, 1998
 * license:    GNU Public License, Version 2
 * mail to:    Mirko Sucker <mirko.sucker@unibw-hamburg.de>
 * requires:   C++-compiler, STL, string class,
 *             Nana for debugging
 * $Revision$
 */

#include "toplevelwidget.h"
#include "AddressBookAbout.h"
#include <qmsgbox.h>
#include <qpopmenu.h>
#include <qkeycode.h>
#include <kapp.h>
#include <klocale.h>
#include <string.h>
#include "debug.h"

ABTLWidget::ABTLWidget(const char* name)
  : KTMainWindow(name)
{
  // ############################################################################
  timer=new QTimer(this);
  connect(timer, SIGNAL(timeout()),
	  SLOT(timeOut()));
  // -----
  widget=new AddressWidget(this);
  if(!constructMenu())
    {
      QMessageBox::critical
	(this, i18n("Initialization failure"),
	 i18n
	 ("The program's menubar could not be constructed."));
      ::exit(-1);
    }
  if(!constructStatusBar())
    {
      QMessageBox::critical
	(this, i18n("Initialization failure"),
	 i18n
	 ("The program's statusbar could not be constructed."));
      ::exit(-1);     
    }
  menu->show();
  setMenu(menu);
  status->show();
  setStatusBar(status);
  setView(widget);
  widget->show();
  updateRects();
  createConnections();
  widget->currentChanged();
  // ----- be friendly:
  setStatus(i18n("KDE addressbook."));
  // ############################################################################
}  
 
bool ABTLWidget::constructMenu()
{
  // ############################################################################
  // menu=new KMenuBar(this);
  menu=menuBar();
  int id;
  // -----
  // the file menu
  QPopupMenu* file=new QPopupMenu;
  CHECK(file!=0);
  id=file->insertItem(i18n("&Save"), widget, SLOT(save()),
		      CTRL+Key_S);
  QPopupMenu* export=new QPopupMenu;
  export->insertItem
    (i18n("HTML table"), widget, SLOT(exportHTML()));
  /*
    export->insertItem
    (i18n("Plain text"), widget, SLOT(exportPlain()));
    export->insertItem
    (i18n("TeX table"), widget, SLOT(exportTeXTable()));
    export->insertItem
    (i18n("TeX labels"), widget, SLOT(exportTeXLabels()));
  */
  file->insertItem(i18n("Export"), export);
  file->insertSeparator();
  file->insertItem(i18n("Search &entries"), widget, 
		   SLOT(search()), CTRL+Key_F);
  file->insertItem(i18n("&Print"), widget, SLOT(print()),
		   CTRL+Key_P);
  file->insertSeparator();
  file->insertItem(i18n("&Quit"), 
		   KApplication::getKApplication(), 
		   SLOT(quit()), CTRL+Key_Q);
  // the edit menu
  QPopupMenu* edit=new QPopupMenu;
  CHECK(edit!=0);
  edit->insertItem(i18n("&Copy"), widget, SLOT(copy()),
		   CTRL+Key_W);
  edit->insertSeparator();
  id=edit->insertItem(i18n("&Add entry"), widget, SLOT(add()), 
		      CTRL+Key_N);
  id=edit->insertItem(i18n("&Edit entry"), widget, SLOT(edit()), 
		      CTRL+Key_E);
  id=edit->insertItem(i18n("&Remove entry"), widget, 
		      SLOT(remove()), CTRL+Key_R);
  edit->insertSeparator();
  /* I had to disable it as it is not implemented and 
   * needs to wait for a little time. On the other hand I did not 
   * want to show entries without functionality.
   int result;
   result=edit->insertItem(i18n("Import &vCard"), widget, 
   SLOT(importVCard()), CTRL+Key_V);
   edit->setItemEnabled(result, false);
   edit->insertSeparator();
  */
  mailItemID=edit->insertItem(i18n("&Mail"), widget, SLOT(mail()),
			      CTRL+Key_M);
  talkItemID=edit->insertItem(i18n("&Talk"), widget, SLOT(talk()),
			      CTRL+Key_T);
  browseItemID=edit->insertItem(i18n("&Browse"), widget, 
				SLOT(browse()),
				CTRL+Key_B);
  // the about menu
  QPopupMenu* about=new QPopupMenu;
  about->insertItem(i18n("Help on Addressbook"), 
		    KApplication::getKApplication(),
		    SLOT(appHelpActivated()), 
		    Key_F1);
  about->insertSeparator();
  about->insertItem(i18n("About Addressbook"), this, 
		    SLOT(about()), CTRL+Key_A);
  about->insertItem(i18n("About &Qt"), this, SLOT(aboutQt()));
  about->insertItem(i18n("About &KDE"), 
		    KApplication::getKApplication(), 
		    SLOT(aboutKDE()));
  // fill the menu
  menu->insertItem(i18n("&File"), file);
  menu->insertItem(i18n("&Edit"), edit);
  menu->insertSeparator();
  menu->insertItem(i18n("&About"), about);
  // ############################################################################
  return true;
}  

bool ABTLWidget::constructStatusBar()
{
  // ############################################################################
  // status=new KStatusBar(this);
  status=statusBar();
  status->setInsertOrder(KStatusBar::RightToLeft);
  status->insertItem("20000/20000", Number);
  status->insertItem("", Text);
  // ############################################################################
  return true;
}

void ABTLWidget::setNumber(int which, int count)
{
  ID(bool GUARD=false);
  // ############################################################################
  LG(GUARD, "ABTLWidget::setNumber: updating statusbar.\n");
  char buffer[256];
  sprintf(buffer, "%i/%i", which, count);
  status->changeItem(buffer, Number);
  // ############################################################################
}

void ABTLWidget::createConnections()
{
  // ########################################################
  connect(widget, SIGNAL(entrySelected(int, int)),
	  SLOT(setNumber(int, int)));
  connect(KApplication::getKApplication(),
	  SIGNAL(appearanceChanged()), 
	  SLOT(updateRects()));
  connect(widget, SIGNAL(sizeChanged()),
	  SLOT(updateRects()));
  // ----- to set status messages
  connect(widget, SIGNAL(setStatus(const char*)),
	  SLOT(setStatus(const char*)));
  // ----- to clean up before closing:
  connect(KApplication::getKApplication(),
	  SIGNAL(saveYourself()),
	  SLOT(saveOurselfes()));
  // ############################################################################
}

void ABTLWidget::aboutQt()
{
  // ############################################################################
  QMessageBox::aboutQt(this, i18n("About Qt"));
  // ############################################################################
}

void ABTLWidget::about()
{
  // ############################################################################
  AddressBookAbout dialog(this);
  // -----
  connect(&dialog, SIGNAL(sendEmail(const char*)),
	  widget, SLOT(mail(const char*)));
  dialog.exec();
  // ############################################################################
}

void ABTLWidget::setStatus(const char* text)
{
  // ############################################################################
  status->changeItem(text, Text);
  timer->start(8000, false);
  // ############################################################################
}

void ABTLWidget::timeOut()
{
  // ############################################################################
  status->changeItem("", Text);
  // ############################################################################
}

void ABTLWidget::saveOurselfes()
{
  ID(bool GUARD=true);
  LG(GUARD, "ABTLWidget::saveOurselfes: called.\n");
  REQUIRE(widget!=0);
  // ############################################################################
  if(widget->AddressBook::save())
    {
      LG(GUARD, "ABTLWidget::saveOurselfes: "
	 "database saved.\n");
    } else {
      LG(GUARD, "ABTLWidget::saveOurselfes: could not save "
	 "database.\n");
    }
  LG(GUARD, "ABTLWidget::saveOurselfes: switching to "
     "readonly mode.\n");
  widget->setFileName(widget->fileName(), true, true);
  // this app will not be closed calling the destructors:
  LG(GUARD, "ABTLWidget::saveOurselfes: looking for stale "
     "lockfiles.\n");
  ConfigDB::CleanLockFiles(0);
  LG(GUARD, "ABTLWidget::saveOurselfes: done.\n");
  // ############################################################################
}


#include "toplevelwidget.moc"
