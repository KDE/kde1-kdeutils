#ifndef _HLDIALOG_H_
#define _HLDIALOG_H_


#include <qtabdialog.h>
#include <qwidget.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <kcolorbtn.h>
#include <qlist.h>

#include "listedit.h"
#include "highlight.h"


class AttribPage : public QWidget
{
  Q_OBJECT
  
public:

  AttribPage(QList<Attribute> &al, QWidget *parent=0, const char *name=0);

  void fillList();
  
  QList<Attribute> &getAttrList() { return attributes; };

protected slots:

  void entryHighlighted(const char *name, int index);
  
  void colorChanged(const QColor &newColor);
  void selectionChanged(const QColor &newColor);
  void selectFont();
  void flagChanged();

  void entryAdded(const char *entry, int index);
  void entryDeleted(const char *entry, int index);
             
private:

  void update();
  
  ListEdit *nameList;  
  QCheckBox *font, *size, *style, *weight;
  QLabel *fontLabel, *sizeLabel, *styleLabel, *weightLabel;
  QCheckBox *color, *selection;
  KColorButton *colorBtn, *selectionBtn;

  QList<Attribute> attributes;

  int current;    
};


class LanguagePage : public QWidget
{
  Q_OBJECT
  
public:

  LanguagePage(QStrList langs, int hl, QWidget *parent=0, const char *name=0);

  int getLanguage() { if (langList) return langList->current(); else return 0; };

private:

  ListEdit *langList;
    
};


class HlDialog : public QTabDialog
{
  Q_OBJECT

public:

  HlDialog(QList<Attribute> &al, QStrList langs, int hl, QWidget *parent=0, const char *name=0, bool modal=false);
    
  int getLanguage() { return langPage->getLanguage(); };

  QList<Attribute> &getAttrList() { return attribPage->getAttrList(); };
    
private:

  AttribPage *attribPage;
  LanguagePage *langPage;

};


#endif