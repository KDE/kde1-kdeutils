/* -*- C++ -*-
 * This file implements the about-dialog.
 * 
 * the KDE addressbook.
 * copyright:  (C) Mirko Sucker, 1998
 * license:    GNU Public License, Version 2
 * mail to:    Mirko Sucker <mirko.sucker@unibw-hamburg.de>
 * requires:   C++-compiler, STL, string class,
 *             Nana for debugging
 * $Revision$
 */

#include "AddressBookAbout.h"
#include <qpixmap.h>
#include <qmsgbox.h>
#include <string>
#include <kapp.h>
#include "debug.h"

#undef Inherited
#define Inherited AddressBookAboutData

AddressBookAbout::AddressBookAbout
(
 QWidget* parent,
 const char* name
 )
  :
  Inherited( parent, name )
{
  // ########################################################  
  const string AddressBookLogo="addressbook_logo.jpg";
  setCaption(i18n("About addressbook"));
  QPixmap pixmap;
  string path;
  path=KApplication::getKApplication()->kde_datadir();
  path+=(string)"/kab/pics/"+AddressBookLogo;
  // this needs the installed JPEG reader:
  if(!pixmap.load(path.c_str()))
    {
      QMessageBox::information
	(parent, // we are invisible here!
	 i18n("Image load failure"),
	 i18n("Could not load addressbook logo image!"));
      labelLogo->setText
	(i18n("Here you would see\nthe addressbook logo."));
    } else {
      labelLogo->setPixmap(pixmap);
      ix=pixmap.width(); iy=pixmap.height();
    }
  initializeGeometry();
  buttonOK->setText(i18n("OK"));
  buttonOK->setFocus();
  // ########################################################  
}


AddressBookAbout::~AddressBookAbout()
{
}

void AddressBookAbout::initializeGeometry()
{
  // ########################################################  
  const int Grid=5;
  int tempx, tempy, cx, cy;
  // ----- first determine size of label containing the logo:
  tempx=6+ix;
  tempy=6+iy;
  // ----- then determine dialog width:
  cx=QMAX(labelVersion->sizeHint().width(),
	  labelAuthor->sizeHint().width());
  cx=QMAX(tempx, cx)+4*Grid;
  cy=2*Grid;
  // ----- now show image, version and author label:
  labelLogo->setGeometry
    ((cx-tempx)/2, cy, ix+6, iy+6);
  cy+=Grid+labelLogo->height();
  labelVersion->setGeometry
    (2*Grid, cy, cx-4*Grid, labelVersion->sizeHint().height());
  cy+=Grid+labelVersion->sizeHint().height();
  labelAuthor->setGeometry
    (2*Grid, cy, cx-4*Grid, labelAuthor->sizeHint().height());
  cy+=Grid+labelAuthor->sizeHint().height();
  // ----- now we know how to resize the outer frame:
  labelFrame->setGeometry
    (Grid, Grid, cx-2*Grid, cy-Grid);
  cy+=Grid;
  // ----- now resize the button:
  buttonOK->setGeometry
    (Grid, cy, cx-2*Grid, buttonOK->sizeHint().height());
  cy+=Grid+buttonOK->sizeHint().height();
  setFixedSize(cx, cy);
  // ########################################################  
}
  

#include "AddressBookAbout.moc"
#include "AddressBookAboutData.moc"

