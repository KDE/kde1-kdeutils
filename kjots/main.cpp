// -*- C++ -*-

//
//  kjots
//
//  Copyright (C) 1997 Christoph Neerfeld
//  email:  Christoph.Neerfeld@mail.bonn.netsurf.de
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <qapp.h>
#include <qdir.h>
#include <qfileinf.h>

#include <kapp.h>
#include <drag.h>
#include <kiconloader.h>

#include "KJotsMain.h"

KJotsMain *main_widget;
KIconLoader *global_pix_loader;

QString exec_http;
QString exec_ftp;

int main( int argc, char **argv )
{
  //debug ( "[kjots] started-------------------------" );

  KApplication a( argc, argv, "kjots" );

  QString name = QDir::homeDirPath();
  name += "/.kjots";
  QFileInfo fi(name);
  if( !(fi.exists() && fi.isDir()) )
    {
      QDir dir = QDir::home();
      if( !dir.mkdir(".kjots") )
	{
	  debug("Kjots: can't create folder directory '.kjots' !");
	  debug("Kjots: giving up.");
	  KApplication::exit(1);
	}
    }
  QString temp1, temp2;
  KConfig *config = a.getConfig();
  config->setGroup("KDE Setup");
  if( !config->hasKey("IconPath") )
     {
       temp1 = KApplication::kdedir();
       temp1 += "/lib/pics:";
       temp1 += KApplication::kdedir();
       temp1 += "/lib/pics/toolbar";
       config->writeEntry("IconPath", temp1);
       config->sync();
     }
  config->setGroup("kjots");
  if( !config->hasKey("execHttp") )
    config->writeEntry("execHttp", "kfmclient openURL %u");
  if( !config->hasKey("execFtp") )
    config->writeEntry("execFtp", "kfmclient openURL %u");
  if( !config->hasKey("EFontFamily") )
    config->writeEntry("EFontFamily", "helvetica");
  if( !config->hasKey("EFontSize") )
    config->writeEntry("EFontSize", 12);
  if( !config->hasKey("EFontWeight") )
    config->writeEntry("EFontWeight", 0);
  if( !config->hasKey("EFontItalic") )
    config->writeEntry("EFontItalic", 0);
  global_pix_loader = new KIconLoader();
  KJotsMain jots;
  main_widget = &jots;
  a.setMainWidget( (QWidget *) &jots );
  a.setRootDropZone( new KDNDDropZone( (QWidget *) &jots, DndNotDnd ) );
  jots.show();
  jots.resize(jots.size());
  return a.exec();
}







