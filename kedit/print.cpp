/*
    $Id$

    Requires the Qt widget libraries, available at no cost at 
    http://www.troll.no
       
    Copyright (C) 1996 Bernd Johannes Wuebben   
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
  
    $Log$
    Revision 1.1  1997/04/20 00:18:15  wuebben
    Initial revision

    Revision 1.1  1997/01/04 17:36:44  wuebben
    Initial revision


*/


#include "print.h"
#include "stdio.h"

#define YOFFSET  5
#define XOFFSET  5
#define LABLE_LENGTH  40
#define LABLE_HEIGHT 20
#define SIZE_X 400
#define SIZE_Y 280
#define FONTLABLE_LENGTH 60
#define COMBO_BOX_HEIGHT 28
#define COMBO_ADJUST 3
#define OKBUTTONY 260
#define BUTTONHEIGHT 25

#include "print.moc"


PrintDialog::PrintDialog( QWidget *parent, const char *name,  bool modal)
    : QDialog( parent, name, modal )
{



  
  setCaption("Print Dialog");
  /*  box1 = new QGroupBox(this, "Box1");
  box1->setGeometry(XOFFSET,YOFFSET,SIZE_X -  XOFFSET
		   ,145);
  box1->setTitle("Print");
  */


  bg = new QButtonGroup(this,"bg");
  
  commandbox = new QLineEdit(bg,"command");
  commandbox->setGeometry(165,50,205,25);

  commandbutton = new QRadioButton("Print using Command:",bg,"commandbutton");
  commandbutton->setGeometry(15,50,140,25);

  rawbutton = new QRadioButton("Print directly using lpr",bg,"rawbutton");
  rawbutton->setGeometry(15,20,200,25);
  rawbutton->setChecked(TRUE);

  bg->setGeometry(10,10,385,90);
  

  bg1 = new QButtonGroup(this,"bg1");

  allbutton = new QRadioButton("Print Document",bg1,"documentbutton");
  allbutton->setGeometry(15,10,130,25);
  allbutton->setChecked(TRUE);

  selectionbutton = new QRadioButton("Print Selection",bg1,"selectionbutton");
  selectionbutton->setGeometry(15,40,130,25);
  
  bg1->setGeometry(10,115,385,80);
  
  cancel_button = new QPushButton("Cancel",this);

  cancel_button->setGeometry( 3*XOFFSET +100, 210, 80, BUTTONHEIGHT );
  connect( cancel_button, SIGNAL( clicked() ), SLOT( reject() ) );

  ok_button = new QPushButton( "Ok", this );
  ok_button->setGeometry( 3*XOFFSET, 210, 80, BUTTONHEIGHT );
  connect( ok_button, SIGNAL( clicked() ), SLOT( accept() ) );	


  this->setFixedSize(405,240);

}


struct printinfo PrintDialog::getCommand(){

  struct printinfo pi;

  QString string;
  string = commandbox->text();

  pi.command = string;
  pi.raw = rawbutton->isChecked();
  pi.selection = selectionbutton->isChecked();

  return pi;

}

void PrintDialog::setWidgets(struct printinfo pi){

  if (pi.raw == 1){
    rawbutton->setChecked(TRUE);
    commandbutton->setChecked(FALSE);
  }
  else{
    commandbutton->setChecked(TRUE);
    rawbutton->setChecked(FALSE);
  }
  
  commandbox->setText(pi.command);
  

}
