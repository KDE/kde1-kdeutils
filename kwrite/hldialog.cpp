#include "hldialog.h"

#include <kapp.h>
#include <qlayout.h>
#include <qfontinfo.h>
#include <kfontdialog.h>


AttribPage::AttribPage(QList<Attribute> &al, QWidget *parent, const char *name)
  : QWidget(parent,name), attributes(al), current(-1)
{
  QVBoxLayout *vbox = new QVBoxLayout(this,8,8);

  nameList = new ListEdit(this,0,false);
  vbox->addWidget(nameList,1);

  QFrame *f = new QFrame(this);
  f->setFrameStyle(QFrame::HLine|QFrame::Sunken);
  vbox->addWidget(f);

  QGridLayout *bottom = new QGridLayout(4,7);
  vbox->addLayout(bottom/*,0*/);

  font = new QCheckBox(i18n("Font"),this);
  font->setFixedSize(font->sizeHint()+QSize(20,0));
  bottom->addWidget(font,0,0);
  size = new QCheckBox(i18n("Size"),this);
  size->setFixedSize(size->sizeHint()+QSize(20,0));
  bottom->addWidget(size,1,0);
  style = new QCheckBox(i18n("Style"),this);
  style->setFixedSize(style->sizeHint()+QSize(20,0));
  bottom->addWidget(style,2,0);
  weight = new QCheckBox(i18n("Weight"),this);
  weight->setFixedSize(weight->sizeHint()+QSize(20,0));
  bottom->addWidget(weight,3,0);

  fontLabel = new QLabel("Courier",this);
  fontLabel->setFixedSize(font->sizeHint()+QSize(100,0));
  bottom->addWidget(fontLabel,0,1);
  sizeLabel = new QLabel("12",this);
  sizeLabel->setFixedSize(size->sizeHint()+QSize(100,0));
  bottom->addWidget(sizeLabel,1,1);
  styleLabel = new QLabel("Roman",this);
  styleLabel->setFixedSize(style->sizeHint()+QSize(100,0));
  bottom->addWidget(styleLabel,2,1);
  weightLabel = new QLabel("Normal",this);
  weightLabel->setFixedSize(weight->sizeHint()+QSize(100,0));
  bottom->addWidget(weightLabel,3,1);

  QPushButton *fontSelect = new QPushButton("Select...",this);
  fontSelect->setFixedSize(fontSelect->sizeHint());
  bottom->addWidget(fontSelect,0,2);
   
  f = new QFrame(this);
  f->setFrameStyle(QFrame::VLine|QFrame::Sunken);
  bottom->addMultiCellWidget(f,0,3,3,3);

  color = new QCheckBox(i18n("Color"),this);
  color->setFixedSize(color->sizeHint()+QSize(40,0));
  bottom->addWidget(color,0,4);
  selection = new QCheckBox(i18n("Selection"),this);
  selection->setFixedSize(selection->sizeHint()+QSize(40,0));
  bottom->addWidget(selection,1,4);

  colorBtn = new KColorButton(this);
  colorBtn->setFixedSize(colorBtn->sizeHint());
  bottom->addWidget(colorBtn,0,5);
  selectionBtn = new KColorButton(this);
  selectionBtn->setFixedSize(selectionBtn->sizeHint());
  bottom->addWidget(selectionBtn,1,5);
    
  bottom->setColStretch(6,1);

  //  bottom->activate();
  vbox->activate();

  fillList();

  connect(nameList, SIGNAL(entryHighlighted(const char*,int)), this, SLOT(entryHighlighted(const char*,int)));
  connect(colorBtn, SIGNAL(changed(const QColor &)), this, SLOT(colorChanged(const QColor &)));
  connect(selectionBtn, SIGNAL(changed(const QColor &)), this, SLOT(selectionChanged(const QColor &)));
  connect(fontSelect, SIGNAL(clicked()), this, SLOT(selectFont()));
  connect(font, SIGNAL(clicked()), this, SLOT(flagChanged()));
  connect(size, SIGNAL(clicked()), this, SLOT(flagChanged()));
  connect(style, SIGNAL(clicked()), this, SLOT(flagChanged()));
  connect(weight, SIGNAL(clicked()), this, SLOT(flagChanged()));
  connect(color, SIGNAL(clicked()), this, SLOT(flagChanged()));
  connect(selection, SIGNAL(clicked()), this, SLOT(flagChanged()));
  connect(nameList, SIGNAL(entryAdded(const char *,int)), this, SLOT(entryAdded(const char *,int)));
  connect(nameList, SIGNAL(entryRemoved(const char *,int)), this, SLOT(entryDeleted(const char *,int)));
}


void AttribPage::fillList()
{
  nameList->clear();
  for (Attribute *i=attributes.first(); i != 0; i=attributes.next())
    nameList->insertItem(i->getName());
}


void AttribPage::entryHighlighted(const char *, int index)
{
  current = index;

  update();
}


void AttribPage::update()
{
  Attribute *att = attributes.at(current);

  // set the overriding flags

  if (current == 0)
  {
    // set all flags, as this is the parent
    font->setChecked(true);
    size->setChecked(true);
    style->setChecked(true);
    weight->setChecked(true);
    color->setChecked(true);
    selection->setChecked(true);
  }
  else
  {
    OverrideFlags flags = att->getOverrideFlags();

    font->setChecked(flags & FontFamily);
    size->setChecked(flags & FontSize);
    style->setChecked(flags & FontStyle);
    weight->setChecked(flags & FontWeight);
    color->setChecked(flags & Color);
    selection->setChecked(flags & SelColor);
  }


  // set the font attributes
 
  QFontInfo fi(att->getFont());

  fontLabel->setText(fi.family());
  QString s; s.setNum(fi.pointSize());
  sizeLabel->setText(s);
  if (fi.italic())
    styleLabel->setText("Italic");    
  else
    styleLabel->setText("Roman");
  if (fi.bold())
    weightLabel->setText("Bold");
  else
   weightLabel->setText("Normal");

  
  // set the color attributes

  colorBtn->setColor(att->getColor());
  selectionBtn->setColor(att->getSelColor()); 
}


void AttribPage::colorChanged(const QColor &newColor)
{
  Attribute *att = attributes.at(current);

  att->setColor(newColor);
  att->setOverrideFlags((OverrideFlags)(att->getOverrideFlags() | Color));

  if (current == 0)
    Attribute::DefaultColor = newColor;

  update();
}


void AttribPage::selectionChanged(const QColor &newColor)
{
  Attribute *att = attributes.at(current);

  att->setSelColor(newColor);
  att->setOverrideFlags((OverrideFlags)(att->getOverrideFlags() | SelColor));

  if (current == 0)
    Attribute::DefaultSelColor = newColor;

  update();
}


void AttribPage::selectFont()
{
  //CT 28Oct1998 - fix crash at first startup of font selection 
  //  (item not selected)

  if (current < 0 /*CT no way to do this, no needed too, maybe*  || current > nameList->count() -1*/) {
    current = 0;
    nameList->setCurrent(current);
  }
  //CT

  Attribute *att = attributes.at(current);
  QFont font = att->getFont();
  
  if (KFontDialog::getFont(font) == QDialog::Accepted)
  {
    att->setFont(font);

     if (current == 0)
       Attribute::DefaultFont = font;

    update();
  }
}


void AttribPage::flagChanged()
{
  Attribute *att = attributes.at(current);

  int flags = NoOverride;

  if (font->isChecked())
    flags += FontFamily; 
  if (size->isChecked())
    flags += FontSize; 
  if (style->isChecked())
    flags += FontStyle; 
  if (weight->isChecked())
    flags += FontWeight; 
  if (color->isChecked())
    flags += Color; 
  if (selection->isChecked())
    flags += SelColor; 

  if (current != 0)
    att->setOverrideFlags((OverrideFlags)flags);

  update();
}


void AttribPage::entryAdded(const char *entry, int)
{
  Attribute *att = new Attribute(entry, Attribute::DefaultColor, 
                     Attribute::DefaultSelColor, Attribute::DefaultFont, NoOverride);
 
  attributes.append(att);
 
  fillList();
}


void AttribPage::entryDeleted(const char *, int index)
{
  // do not delete the attributes for the compiled in highlightings
  if (index<13)
    return;

  attributes.remove(index);

  fillList();
}



LanguagePage::LanguagePage(QStrList langs, int hl, QWidget *parent, const char *name)
  : QWidget(parent,name)
{
  QVBoxLayout *vbox = new QVBoxLayout(this,8,8);

  langList = new ListEdit(this,0,false);
  vbox->addWidget(langList,1);

  QFrame *f = new QFrame(this);
  f->setFrameStyle(QFrame::HLine|QFrame::Sunken);
  vbox->addWidget(f);

  vbox->addStretch(1);

  vbox->activate();

  for (char *item = langs.first(); item != 0; item = langs.next())
    langList->insertItem(item);
  langList->setCurrent(hl);  
}


HlDialog::HlDialog(QList<Attribute> &al, QStrList langs, int hl, QWidget *parent, const char *name, bool modal)
  : QTabDialog(parent,name,modal)
{
  setCancelButton(i18n("Cancel"));
  setCaption(i18n("Configure Highlighting"));

  langPage = new LanguagePage(langs, hl, this);
  addTab(langPage, i18n("&Language"));

  attribPage = new AttribPage(al,this);
  addTab(attribPage, i18n("A&ttributes"));
}
