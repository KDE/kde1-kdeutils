#include "listedit.h"


#include <kapp.h>
#include <qlayout.h>
#include <qpushbutton.h>

#include "editlinedlg.h"


ListEdit::ListEdit(QWidget *parent, const char *name, bool remove_first)
  : QWidget(parent,name), removeFirst(remove_first)
{
  QGridLayout *top = new QGridLayout(this,4,2,0,8);

  listBox = new QListBox(this);
  top->addMultiCellWidget(listBox,0,3,0,0);
  top->setColStretch(0,1);

  add = new QPushButton(i18n("&Add"),this);
  add->setFixedSize(add->sizeHint());
  top->addWidget(add,0,1);
  edit = new QPushButton(i18n("&Edit"),this);
  edit->setFixedSize(edit->sizeHint());
  top->addWidget(edit,1,1);
  remove = new QPushButton(i18n("&Delete"),this);
  remove->setFixedSize(remove->sizeHint());
  top->addWidget(remove,2,1);

  checkButtons();

  top->activate();

  connect(add, SIGNAL(clicked()), this, SLOT(addButtonPressed()));
  connect(edit, SIGNAL(clicked()), this, SLOT(editButtonPressed()));
  connect(remove, SIGNAL(clicked()), this, SLOT(removeButtonPressed()));
  connect(listBox, SIGNAL(highlighted(int)), this, SLOT(itemHighlighted(int)));
  connect(listBox, SIGNAL(selected(int)), this, SLOT(itemSelected(int)));
}


void ListEdit::addButtonPressed()
{
  EditLineDialog dlg(i18n("Name of the attribute:"), "", this, 0, true);

  if (dlg.exec() == QDialog::Accepted)
  {
    listBox->insertItem(dlg.getText());
    emit entryAdded(dlg.getText(), listBox->count()-1);
  }

  checkButtons();
}


void ListEdit::editButtonPressed()
{
  EditLineDialog dlg(i18n("Name of the attribute:"),
                     listBox->text(listBox->currentItem()), this, 0, true);

  if (dlg.exec() == QDialog::Accepted)
  {
    listBox->changeItem(dlg.getText(),listBox->currentItem());
    emit entryEdited(dlg.getText(), listBox->count()-1);
  }

  checkButtons();
}


void ListEdit::removeButtonPressed()
{
  int i = listBox->currentItem();
  QString t = listBox->text(i);

  listBox->removeItem(listBox->currentItem());
  emit entryRemoved(t,i);

  checkButtons();
}


void ListEdit::itemHighlighted(int index)
{
  int i = listBox->currentItem();

  emit entryHighlighted(listBox->text(i),i);

  checkButtons();
}  


void ListEdit::itemSelected(int index)
{
  int i = listBox->currentItem();

  emit entrySelected(listBox->text(i),i);

  checkButtons();
}


void ListEdit::checkButtons()
{
  if (listBox->currentItem() < 0)
  {
    edit->setEnabled(false);
    remove->setEnabled(false);
  }
  else
  {
    edit->setEnabled(true);
    if (listBox->currentItem() > 0 || removeFirst)
      remove->setEnabled(true);
    else
      remove->setEnabled(false);
  }

  // FIXME: as long as there are no user defined highlightings, 
  // disable all edit buttons.
  add->setEnabled(false);
  edit->setEnabled(false);
  remove->setEnabled(false);
}