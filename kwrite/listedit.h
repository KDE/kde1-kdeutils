#ifndef _LISTEDIT_H_
#define _LISTEDIT_H_


#include <qwidget.h>
#include <qlistbox.h>
#include <qpushbutton.h>


class ListEdit : public QWidget
{
  Q_OBJECT
  
public:

  ListEdit(QWidget *parent=0, const char *name=0, bool remove_first=true);

  void clear() { listBox->clear(); };
  void insertItem(const char *name) { listBox->insertItem(name); };

  void setCurrent(int index) { listBox->setCurrentItem(index); };
  int current() { return listBox->currentItem(); };
    
protected slots:

  void addButtonPressed();
  void editButtonPressed();
  void removeButtonPressed();
  void itemHighlighted(int index);
  void itemSelected(int index);
  
signals:

  void entryAdded(const char *entry, int index);
  void entryEdited(const char *entry, int index);
  void entryRemoved(const char *entry, int index);  
  void entrySelected(const char *entry, int index);
  void entryHighlighted(const char *entry, int index);
  
private:

  void checkButtons();

  QListBox *listBox;
  bool removeFirst;
  QPushButton *add, *edit, *remove;    
};


#endif