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

#include "filldlg.h"
#include <klocale.h>
#include <kapp.h>

FillDlg::FillDlg(QWidget *parent, const char *name)
     : QDialog(parent, name, TRUE)
{
    frame1 = new QGroupBox(klocale->translate("KEdit Options"), 
			   this, "frame1");
    values = new QLineEdit( this, "values");
    this->setFocusPolicy(QWidget::StrongFocus);
    connect(values, SIGNAL(returnPressed()), this, SLOT(checkit()));
    fill_column = new QCheckBox(klocale->translate("Set Fill-Column at:"),
				frame1, "fill");
    connect(fill_column, SIGNAL(toggled(bool)),this,SLOT(synchronize(bool)));

    word_wrap = new QCheckBox(klocale->translate("Word Wrap"), 
			      frame1, "word");
    backup_copies = new QCheckBox(klocale->translate("Backup Copies"), 
			      frame1, "backup");
    mailcmd = new QLineEdit(this,"mailcmd");
    mailcmdlabel = new QLabel(this,"mailcmdlable");
    mailcmdlabel->setText(klocale->translate("Mail Command:"));
    ok = new QPushButton(klocale->translate("OK"), this, "OK");
    cancel = new QPushButton(klocale->translate("Cancel"), this, "cancel");
    cancel->setFocus();
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ok, SIGNAL(clicked()), this, SLOT(checkit()));
    resize(300, 225);
}

struct fill_struct  FillDlg::getFillCol() { 

  QString string;

  fillstr.word_wrap_is_set = word_wrap->isChecked();
  fillstr.fill_column_is_set = fill_column->isChecked();
  fillstr.backup_copies_is_set = backup_copies->isChecked();
  fillstr.mailcmd = mailcmd->text();
  fillstr.mailcmd.detach();
  string = values->text();  
  fillstr.fill_column_value = string.toInt();

  return fillstr;

}


void FillDlg::synchronize(bool on){

  // if the fill-column button is not checked we can't do word wrap
  // so we have to turn the word wrap button off.

  if(!on)
    word_wrap->setChecked(FALSE);
  
}

bool FillDlg::fill(){

  if (fill_column->isChecked())
    return true;
  else
    return false;


};

void FillDlg::setWidgets(struct fill_struct fill){

  QString string;

  fill_column->setChecked(fill.fill_column_is_set);
  word_wrap->setChecked(fill.word_wrap_is_set);
  backup_copies->setChecked(fill.backup_copies_is_set);
  values->setText(string.setNum(fill.fill_column_value));
  mailcmd->setText(fill.mailcmd.data());

}

bool FillDlg::wordwrap(){

  if (word_wrap->isChecked())
    return true;
  else
    return false;

};


void FillDlg::resizeEvent(QResizeEvent *)
{
    frame1->setGeometry(5, 5, width() - 10, height() - 45);
    cancel->setGeometry(width() - 80, height() - 30, 70, 25);
    ok->setGeometry(width() - 160, height() - 30, 70, 25);
    values->setGeometry(170, 35, 70, 25);
    mailcmd->setGeometry(120, 140, 160, 25);
    mailcmdlabel->setGeometry(25, 140, 90, 25);
    fill_column->setGeometry(20, 30, 140, 25);
    word_wrap->setGeometry(20, 65, 140, 25);
    backup_copies->setGeometry(20, 100, 140, 25);
}

void FillDlg::checkit(){

  bool ok;
  QString string;

  string = values->text();
  string.toInt(&ok);

  if (ok){
    accept();
  }
  else{
  QMessageBox::message(klocale->translate("Sorry"),
		       klocale->translate("You must enter an integer."),
		       klocale->translate("OK"));
  }

}


#include "filldlg.moc"
