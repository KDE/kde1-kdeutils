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

#include <klocale.h>
#include <kiconloader.h>

extern KApplication *mykapp;


About::About(QWidget *parent) : QDialog(parent,
					klocale->translate("About KEdit"), TRUE) {
  resize(450, 320);
  setFixedSize(size());
  setCaption(klocale->translate("About KEdit"));

  QPixmap pm = kapp->getIconLoader()->loadIcon("keditlogo.xpm");
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
  "\n(c) 1997 Bernd Johannes Wuebben\n\nwuebben@kde.org\nwuebben@math.cornell.edu\n\n" \
  "This program is free software; you can redistribute it and/or modify "\
  "it under the terms of the GNU General Public License. See the help documentation for "\
  "details.";


  l = new QLabel(s.data(), this);
  l->setGeometry(170, 70, 240, 220);
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
