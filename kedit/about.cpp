/*

    $Id$
       
    Copyright (C) 1997 Bernd Johannes Wuebben   
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

*/

#include "about.h"
#include "version.h"
#include <qpushbt.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <kapp.h>

extern KApplication *mykapp;

About::About(QWidget *parent) : QDialog(parent, "About KEdit", TRUE) {
  resize(350, 250);
  setFixedSize(size());
  setCaption("About KEdit");

  QString pixdir = mykapp->kdedir() + QString("/share/apps/kedit/pics/");  
  QPixmap pm((pixdir + "keditlogo.xpm").data());
  QLabel *logo = new QLabel(this);
  logo->setPixmap(pm);
  logo->setGeometry(20, (height()-pm.height())/2 - 20, pm.width(), pm.height());

  QLabel *l;
  l = new QLabel("KEdit", this);
  l->setFont(QFont("Utopia", 26, QFont::Normal));
  l->setGeometry((width() - l->sizeHint().width())/2, 20,
		 l->sizeHint().width(), 
		 l->sizeHint().height());

  QString s;
  s = "Version " KEDITVERSION \
  "\n\nCopyright (c) 1997\nBernd Johannes Wuebben\n"\
  "wuebben@math.cornell.edu";


  l = new QLabel(s.data(), this);
  l->setGeometry(145, 70, 230, 200);
  l->setAlignment(AlignLeft|WordBreak|ExpandTabs);

  QPushButton *b_ok = new QPushButton("Ok", this);
  b_ok->setGeometry(width()/2-40, height() - 48, 80, 32);
  b_ok->setDefault(TRUE);
  b_ok->setAutoDefault(TRUE);
  connect(b_ok, SIGNAL(released()),
	  this, SLOT(accept()));
  b_ok->setFocus();
}

#include "about.moc"
