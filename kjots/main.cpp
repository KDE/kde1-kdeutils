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

#include "KJotsMain.h"
#include "pixloader.h"

KJotsMain *main_widget;
PixmapLoader *global_pix_loader;

int main( int argc, char **argv )
{
debug ( "[kjots] started-------------------------" );

  KApplication a( argc, argv, "kjots" );
  //a.setStyle(WindowsStyle);

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
  config->setGroup("Icons");
  if( !config->hasKey("Path") )
     {
       temp1 = (QString) getenv("KDEDIR");
       temp1 += "/lib/pics";
       config->writeEntry("Path", temp1);
     }
  config->setGroup("kjots");
  temp2 = config->readEntry("helpfile");
  if( temp2.isEmpty() )
    {
      temp2 = (QString) getenv("KDEDIR");
      temp2 += "/doc/HTML/kjots/kjots.html";
      config->writeEntry("helpfile", temp2);
    }
  global_pix_loader = new PixmapLoader( config, "Icons", "Path" );
  KJotsMain jots;
  main_widget = &jots;
  a.setMainWidget( (QWidget *) &jots );
  a.setRootDropZone( new KDNDDropZone( (QWidget *) &jots, DndNotDnd ) );
  jots.show();
  jots.resize(jots.size());
  return a.exec();
}





