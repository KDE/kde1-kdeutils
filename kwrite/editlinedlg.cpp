#include "editlinedlg.h"

#include <kapp.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qframe.h>
#include <qpushbutton.h>


EditLineDialog::EditLineDialog(QString title, const char *t, QWidget *parent, const char *name, bool modal)
  : QDialog(parent,name,modal)
{
  QGridLayout *grid = new QGridLayout(this,4,4,8);

  QLabel *label = new QLabel(title, this);
  label->setFixedSize(label->sizeHint());
  grid->addWidget(label,0,0);

  lineEdit = new QLineEdit(this);
  lineEdit->setText(t);
  grid->addMultiCellWidget(lineEdit,1,1,0,3);

  QFrame *f = new QFrame(this);
  f->setFrameStyle(QFrame::HLine|QFrame::Sunken);
  grid->addMultiCellWidget(f,2,2,0,3);

  QPushButton *cancel = new QPushButton(i18n("&Cancel"),this);
  cancel->setFixedSize(cancel->sizeHint());
  grid->addWidget(cancel,3,2);

  QPushButton *ok = new QPushButton(i18n("&OK"),this);
  ok->setDefault(true);
  ok->setFixedSize(ok->sizeHint());
  grid->addWidget(ok,3,3);
  
  grid->setColStretch(1,1);
  grid->activate();

  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

  lineEdit->setFocus();

  resize(360,128);
}