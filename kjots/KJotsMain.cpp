//
//  kjots
//
//  Copyright (C) 1997 Christoph Neerfeld
//  email:  Christoph.Neerfeld@mail.bonn.netsurf.de
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <qapp.h>
#include <qmenubar.h>
#include <qpopmenu.h>
#include <qkeycode.h>
#include <qfile.h>
#include <qtstream.h>
#include <qdir.h>
#include <qlined.h>
#include <qdialog.h>
#include <qmsgbox.h>
#include <qmlined.h>

#include <kapp.h>

extern "C" {
#include <unistd.h>
};

#include "KJotsMain.h"
#include "SubjList.h"
#include "ReadListConf.h"

const unsigned int HOT_LIST_SIZE = 8;
const unsigned int BUTTON_WIDTH = 56;

//----------------------------------------------------------------------
// ASKFILENAME
//----------------------------------------------------------------------

AskFileName::AskFileName(QWidget* parent=0, const char* name=0)
  : QDialog(parent, name, TRUE)
{
  initMetaObject();
  resize(300, 80);
  QLabel *l_name = new QLabel( "Book name:", this );
  i_name = new QLineEdit( this );
  QPushButton *b_ok = new QPushButton("Ok", this);
  QPushButton *b_cancel = new QPushButton("Cancel", this);

  l_name->setGeometry(10, 10, 80, 24);
  i_name->setGeometry(100, 10, 190, 24);
  b_ok->setGeometry(20, 44, 60, 24);
  b_cancel->setGeometry(220, 44, 60, 24);

  i_name->setFocus();
  connect( b_ok, SIGNAL(clicked()), this, SLOT(accept()) );
  connect( b_cancel, SIGNAL(clicked()), this, SLOT(reject()) );
}

//----------------------------------------------------------------------
// MYMULTIEDIT
//----------------------------------------------------------------------

MyMultiEdit::MyMultiEdit (QWidget* parent=0, const char* name=0)
  : QMultiLineEdit(parent, name)
{
  initMetaObject();
}

void MyMultiEdit::keyPressEvent( QKeyEvent *e )
{
  if( e->key() == Key_Tab )
    {
      insertChar('\t');
      return;
    }
  QMultiLineEdit::keyPressEvent(e);
  return;
}

//----------------------------------------------------------------------
// KJOTSMAIN
//----------------------------------------------------------------------

KJotsMain::KJotsMain(QWidget* parent, const char* name)
  : QWidget( parent, name ), KJotsMainData( this )
{
  KConfig *config = KApplication::getKApplication()->getConfig();
  config->setGroup("kjots");
  entrylist.setAutoDelete(TRUE);
  button_list.setAutoDelete(TRUE);
  folderOpen = FALSE;
  entrylist.append(new TextEntry);
  subj_list = new SubjList;
  connect( this, SIGNAL(folderChanged(QList<TextEntry> *)), subj_list,
	   SLOT(rebuildList( QList<TextEntry> *)) );
  connect( this, SIGNAL(entryMoved(int)), subj_list, SLOT( select(int)) );
  connect( subj_list, SIGNAL(entryMoved(int)), this, SLOT( barMoved(int)) );
  connect( le_subject, SIGNAL(textChanged(const char *)), subj_list,
	   SLOT(entryChanged(const char*)) );

  me_text->setEnabled(FALSE);
  le_subject->setEnabled(FALSE);
  current = 0;
  connect( b_exit, SIGNAL(clicked()), qApp, SLOT(quit()) );
  connect( b_new, SIGNAL(clicked()), this, SLOT(newEntry()) );
  connect( b_delete, SIGNAL(clicked()), this, SLOT(deleteEntry()) );
  connect( b_next, SIGNAL(clicked()), this, SLOT(nextEntry()) );
  connect( b_prev, SIGNAL(clicked()), this, SLOT(prevEntry()) );
  connect( s_bar, SIGNAL(valueChanged(int)), this, SLOT(barMoved(int)) );
  connect( b_list, SIGNAL(clicked()), this, SLOT(toggleSubjList()) ); 
  connect( subj_list, SIGNAL(hidden()), b_list, SLOT(toggle()) );
  
  top2bottom = new QGridLayout( f_main, 5, 1, 4 );
  top2bottom->addWidget( bg_bot, 0, 0, AlignCenter );
  top2bottom->addWidget( f_text, 1, 0, AlignCenter );
  top2bottom->setRowStretch( 1, 1 );
  top2bottom->addWidget( s_bar, 2, 0, AlignCenter );
  top2bottom->addWidget( bg_top, 3, 0, AlignCenter );
  top2bottom->addWidget( f_labels, 4, 0, AlignCenter );
  top2bottom->activate();
  
  bg_bot_layout = new QBoxLayout( bg_bot, QBoxLayout::LeftToRight, 4 );
  bg_bot_layout->addWidget( b_new, 0, AlignCenter );
  bg_bot_layout->addSpacing(2);
  bg_bot_layout->addWidget( b_delete, 0, AlignCenter );
  bg_bot_layout->addSpacing(2);
  bg_bot_layout->addWidget( b_prev, 0, AlignCenter );
  bg_bot_layout->addSpacing(2);
  bg_bot_layout->addWidget( b_next, 0, AlignCenter );
  bg_bot_layout->addSpacing(6);
  bg_bot_layout->addWidget( b_list, 0, AlignCenter );
  bg_bot_layout->addSpacing(6);
  bg_bot_layout->addWidget( b_exit, 0, AlignCenter );
  bg_bot_layout->addStretch(1);
  bg_bot_layout->activate();

  labels_layout = new QGridLayout( f_labels, 1, 2, 0 );
  labels_layout->addWidget( l_folder, 0, 0, AlignVCenter | AlignLeft );
  labels_layout->addWidget( le_subject, 0, 1, AlignVCenter | AlignLeft );
  labels_layout->setColStretch( 1, 1 );
  labels_layout->activate();

  QFont font_label(l_folder->fontInfo().family());
  font_label.setBold(TRUE);
  l_folder->setFont(font_label);

  f_text_layout = new QGridLayout( f_text, 2, 1, 4 );
  f_text_layout->addWidget( me_text, 0, 0, AlignCenter );
  f_text_layout->setRowStretch( 0, 1 );
  f_text_layout->activate();

  s_bar->setRange(0,0);
  s_bar->setValue(0);
  s_bar->setSteps(1,1);

  bg_top->setExclusive(TRUE);
  me_text->setFocusPolicy(QWidget::ClickFocus);

  // read hotlist
  readListConf( config, "Hotlist", hotlist );
  while( hotlist.count() > HOT_LIST_SIZE )
    hotlist.removeLast();
  // read list of folders
  readListConf( config, "Folders", folder_list );

  QString temp;
  folders = new QPopupMenu;
  int i = 0;
  QPushButton *temp_button;
  for( temp = folder_list.first(); !temp.isEmpty(); temp = folder_list.next(), i++ )
    { 
      folders->insertItem(temp, i); 
      if( hotlist.contains(temp) )
	{
	  temp_button = new QPushButton(temp, f_main);
	  temp_button->setToggleButton(TRUE);
	  temp_button->setFixedSize(BUTTON_WIDTH,24);
	  bg_top->insert(temp_button, i);
	  button_list.append(temp_button);
	}
    }
  unique_id = i+1;
  connect( folders, SIGNAL(activated(int)), this, SLOT(openFolder(int)) );
  connect( bg_top, SIGNAL(clicked(int)), this, SLOT(openFolder(int)) );
  QPopupMenu *file = new QPopupMenu;
  file->insertItem("Open Book", folders );
  file->insertItem("New Book", this, SLOT(createFolder()) );
  file->insertSeparator();
  file->insertItem("Save current book", this, SLOT(saveFolder()), CTRL+Key_S );
  file->insertItem("Save to ascii file");
  file->insertSeparator();
  file->insertItem("Delete current book", this, SLOT(deleteFolder()) );
  file->insertSeparator();
  file->insertItem("Quit", qApp, SLOT(quit()), CTRL+Key_Q);

  QPopupMenu *edit_menu = new QPopupMenu;
  edit_menu->insertItem("Cut", me_text, SLOT(cut()) );
  edit_menu->insertItem("Copy", me_text, SLOT(copyText()) );
  edit_menu->insertItem("Paste", me_text, SLOT(paste()) );

  QPopupMenu *fonds = new QPopupMenu;
  fonds->insertItem("Normal size", this, SLOT(normalSize()) );
  fonds->insertItem("Medium size", this, SLOT(mediumSize()) );
  fonds->insertItem("Large size", this, SLOT(largeSize()) );

  QPopupMenu *hotlist = new QPopupMenu;
  hotlist->insertItem("Add current book to hotlist", this, SLOT(addToHotlist()) );
  hotlist->insertItem("Remove current book from hotlist", this, SLOT(removeFromHotlist()) );

  QPopupMenu *help = new QPopupMenu;
  help->insertItem("Help", this, SLOT(startHelp()), CTRL+Key_H );
  help->insertItem("About", this, SLOT(about()) );

  menubar->insertItem( "&File", file );
  menubar->insertItem( "&Edit", edit_menu, ALT+Key_E );
  menubar->insertItem( "F&onds", fonds, ALT+Key_O );
  menubar->insertItem( "Hot&list", hotlist, ALT+Key_L );
  menubar->insertSeparator();
  menubar->insertItem( "&Help", help, ALT+Key_H );

  QString last_folder = config->readEntry("LastOpenFolder");
  int nr;
  if( (nr = folder_list.find(last_folder)) >= 0 )
    openFolder(nr);
  button_font = b_new->font();
  int width, height;
  width = config->readNumEntry("Width");
  height = config->readNumEntry("Height");
  if( width < minimumSize().width() )
    width = minimumSize().width();
  if( height < minimumSize().height() )
    height = minimumSize().height();
  resize(width, height);
}


KJotsMain::~KJotsMain()
{
  KConfig *config = KApplication::getKApplication()->getConfig();
  button_list.clear();
  if( folderOpen )
    {
      QFileInfo fi(current_folder_name);
      config->writeEntry("LastOpenFolder", fi.fileName());
    }
  saveFolder();
  config->writeEntry("Width", width());
  config->writeEntry("Height", height());
  config->sync();
}

void KJotsMain::resizeEvent( QResizeEvent *e )
{
  f_main->setGeometry( 0, 28, size().width(), size().height() - 28 );
  int i;
  QPushButton *item;
  int x = bg_top->x()+bg_top->width()-2;
  int y = bg_top->y()+4;
  for( item = button_list.first(); item != NULL; item = button_list.next() )
    {
      x -= (BUTTON_WIDTH+4);
      item->move(x,y);
    }
  QWidget::resizeEvent( e );
}

int KJotsMain::readFile( QString name )
{
  QString buf, subj;
  TextEntry *entry;
  entrylist.clear();
  QFile folder(name);
  if( !folder.open(IO_ReadWrite) )
    return -1;
  QTextStream st( (QIODevice *) &folder);
  buf = st.readLine();
  if( buf.left(9) != "\\NewEntry" )
    return -1;
  entry = new TextEntry;
  entrylist.append( entry );
  subj = buf.mid(10, buf.length() );
  entry->subject = subj.isNull() ? (QString) "" : (QString) subj;
  while( !st.eof() )
    {
      buf = st.readLine();
      if( buf.left(9) == "\\NewEntry" )
	{
	  entry = new TextEntry;
	  entrylist.append(entry);
	  subj = buf.mid(10, buf.length() );
	  entry->subject = subj.isNull() ? (QString) "" : (QString) subj;
	  buf = st.readLine();
	}
      entry->text.append( buf );
      entry->text.append("\n");
    }
  folder.close();
  while( entry->text.right(1) == "\n" )
    entry->text.truncate(entry->text.length() - 1);
  entry->text.append("\n");
  return 0;
}

int KJotsMain::writeFile( QString name )
{
  TextEntry *entry;
  QString buf;
  QFile folder(name);
  if( !folder.open(IO_WriteOnly | IO_Truncate) )
    return -1;
  QTextStream st( (QIODevice *) &folder);
  for( entry = entrylist.first(); entry != NULL; entry = entrylist.next() )
    {
      st << "\\NewEntry ";
      st << entry->subject;
      st << "\n";
      buf = entry->text;
      st << buf;
      if( buf.right(1) != "\n" )
	st << '\n';
    }
  folder.close();
  return 0;
}

void KJotsMain::openFolder(int id)
{
  QPushButton *but;
  for( but = button_list.first(); but != NULL; but = button_list.next() )
    but->setOn(FALSE);
  but = (QPushButton *) bg_top->find(id);
  if( but )
    but->setOn(TRUE);
  QDir dir = QDir::home();
  dir.cd(".kjots");
  QString file_name = dir.absPath();
  file_name += '/';
  file_name += folder_list.at( folders->indexOf(id) );
  if( current_folder_name == file_name )
    return;
  if( folderOpen )
    saveFolder();
  current_folder_name = file_name;
  if( readFile(current_folder_name) < 0)
    {
      folderOpen = FALSE;
      debug("Kjots: Unable to open folder");
      return;
    }
  current = 0;
  me_text->setText(entrylist.first()->text);
  emit folderChanged(&entrylist);
  emit entryMoved(current);
  le_subject->setText(entrylist.first()->subject);
  folderOpen = TRUE;
  l_folder->setText( folder_list.at(folders->indexOf(id)) );
  me_text->setEnabled(TRUE);
  le_subject->setEnabled(TRUE);
  me_text->setFocus();
  s_bar->setRange(0,entrylist.count()-1);
  s_bar->setValue(0);
}

void KJotsMain::createFolder()
{
  AskFileName *ask = new AskFileName(this);
  if( ask->exec() == QDialog::Rejected )
    return;
  QString name = ask->getName();
  delete ask;
  if( folder_list.contains(name) )
    {
      QMessageBox::message("Warning", "A book with this name already exists.", "Ok", this);
      return;
    }
  saveFolder();
  entrylist.clear();
  folderOpen = TRUE;
  me_text->setEnabled(TRUE);
  le_subject->setEnabled(TRUE);
  me_text->setFocus();
  me_text->clear();
  TextEntry *new_entry = new TextEntry;
  entrylist.append(new_entry);
  new_entry->subject = "";
  current = 0;
  s_bar->setRange(0,0);
  s_bar->setValue(0);
  emit folderChanged(&entrylist);
  emit entryMoved(current);
  le_subject->setText(entrylist.first()->subject);

  folder_list.append(name);
  if( folders->text(folders->idAt(0)) == NULL )
    folders->removeItemAt(0);
  folders->insertItem(name, unique_id++);
  QDir dir = QDir::home();
  dir.cd(".kjots");
  current_folder_name = dir.absPath();
  current_folder_name += '/';
  current_folder_name += name;
  KConfig *config = KApplication::getKApplication()->getConfig();
  config->setGroup("kjots");
  writeListConf(config, "Folders", folder_list );
  l_folder->setText(name);
  QPushButton *but;
  for( but = button_list.first(); but != NULL; but = button_list.next() )
    but->setOn(FALSE);
}

void KJotsMain::deleteFolder()
{
  if( !folderOpen )
    return;
  if( !QMessageBox::query("Delete current book", \
			  "Are you sure you want to delete the current book ?",\
			  "Yes", "No", this) )
    return;
  QFileInfo fi(current_folder_name);
  QDir dir = fi.dir(TRUE);
  QString name = fi.fileName();
  int index = folder_list.find(name);
  if( index < 0 )
    return;
  dir.remove(current_folder_name);
  folder_list.remove(index);
  int id = folders->idAt(index);
  folders->removeItemAt(index);
  //int pos;
  if( hotlist.contains(name) )
    {
      hotlist.remove(name);
      QButton *but = bg_top->find(id);
      bg_top->remove(but);
      button_list.remove( (QPushButton *) but );
      resize(size());
    }
  KConfig *config = KApplication::getKApplication()->getConfig();
  config->setGroup("kjots");
  writeListConf( config, "Folders", folder_list );
  writeListConf( config, "Hotlist", hotlist );
  entrylist.clear();
  current_folder_name = "";
  folderOpen = FALSE;
  me_text->setEnabled(FALSE);
  me_text->clear();
  le_subject->setEnabled(FALSE);
  le_subject->setText("");
  emit folderChanged(&entrylist);
  s_bar->setRange(0,0);
  s_bar->setValue(0);
  l_folder->setText("");
  subj_list->repaint(TRUE);
}

void KJotsMain::saveFolder()
{
  if( !folderOpen )
    return;
  entrylist.at(current)->text = me_text->text();  
  entrylist.at(current)->subject = le_subject->text();  
  writeFile(current_folder_name);
}

void KJotsMain::nextEntry()
{
  if( !folderOpen )
    return;
  if( current+1 >= (int) entrylist.count() )
    return;
  entrylist.at(current)->text = me_text->text();
  entrylist.at(current)->subject = le_subject->text();
  me_text->setText( entrylist.at(++current)->text );
  me_text->deselect();
  me_text->repaint();
  emit entryMoved(current);
  le_subject->setText( entrylist.at(current)->subject );
  s_bar->setValue(current);
}

void KJotsMain::prevEntry()
{
  if( !folderOpen )
    return;
  if( current-1 < 0 )
    return;
  entrylist.at(current)->text = me_text->text();
  entrylist.at(current)->subject = le_subject->text();
  me_text->setText( entrylist.at(--current)->text );
  me_text->deselect();
  me_text->repaint();
  emit entryMoved(current);
  le_subject->setText( entrylist.at(current)->subject );
  s_bar->setValue(current);
}

void KJotsMain::newEntry()
{
  if( !folderOpen )
    return;
  entrylist.at(current)->text = me_text->text();  
  entrylist.at(current)->subject = le_subject->text();
  me_text->clear();
  le_subject->setText("");
  TextEntry *new_entry = new TextEntry;
  entrylist.append(new_entry);
  new_entry->subject = "";
  current = entrylist.count()-1;
  s_bar->setRange(0,current);
  s_bar->setValue(current);
  emit folderChanged(&entrylist);
  emit entryMoved(current);
}

void KJotsMain::deleteEntry()
{
  if( !folderOpen )
    return;
  if( entrylist.count() == 0 )
    return;
  else if( entrylist.count() == 1 )
    {
      entrylist.at(0)->text = "";
      entrylist.at(0)->subject = "";
      s_bar->setValue(0);
      me_text->clear();
      le_subject->setText("");
      return;
    }
  entrylist.remove(current);
  if( current >= (int) entrylist.count() - 1 )
    {
      current--;
      s_bar->setValue(current);
      s_bar->setRange(0, entrylist.count()-1 );
    }
  me_text->setText( entrylist.at(current)->text );
  le_subject->setText( entrylist.at(current)->subject );
  s_bar->setRange(0, entrylist.count()-1 );
  emit folderChanged(&entrylist);
  emit entryMoved(current);
}

void KJotsMain::barMoved( int new_value )
{
  if( !folderOpen )
    return;
  if( current == new_value )
    return;
  entrylist.at(current)->text = me_text->text();
  entrylist.at(current)->subject = le_subject->text();
  current = new_value;
  me_text->setText( entrylist.at(current)->text );
  me_text->deselect();
  me_text->repaint();
  emit entryMoved(current);
  le_subject->setText( entrylist.at(current)->subject );
  s_bar->setValue(new_value);
}

void KJotsMain::normalSize()
{
  QFont font(me_text->fontInfo().family(), 10);
  me_text->setFont(font);
}

void KJotsMain::mediumSize()
{
  QFont font(me_text->fontInfo().family(), 12);
  me_text->setFont(font);
}

void KJotsMain::largeSize()
{
  QFont font(me_text->fontInfo().family(), 14);
  me_text->setFont(font);
}

void KJotsMain::startHelp()
{
  KConfig *config = KApplication::getKApplication()->getConfig();
  config->setGroup("kjots");
  QString helpfile = "file:";
  helpfile += config->readEntry("helpfile");
  if ( fork() == 0 )
    {
      execlp( "kdehelp", "kdehelp", (const char *) helpfile, 0 );
      exit( 1 );
    }
}

void KJotsMain::about()
{
  QMessageBox::message( "About", \
                        "Kjots 0.2.2\n\r(c) by Christoph Neerfeld\n\rChristoph.Neerfeld@mail.bonn.netsurf.de", "Ok" );
}

void KJotsMain::addToHotlist()
{
  if( hotlist.count() == HOT_LIST_SIZE )
    return;
  QFileInfo fi(current_folder_name);
  QString name = fi.fileName();
  if( hotlist.contains(name) )
    return;
  hotlist.append(name);
  int index = folder_list.find(name);
  if( index < 0 )
    return;
  int id = folders->idAt(index);
  QPushButton *but = new QPushButton(name, f_main);
  button_list.append(but);
  bg_top->insert(but, id);
  KConfig *config = KApplication::getKApplication()->getConfig();
  config->setGroup("kjots");
  writeListConf( config, "Hotlist", hotlist );
  but->setToggleButton(TRUE);
  but->setFixedSize(BUTTON_WIDTH,24);
  but->show();
  but->setOn(TRUE);
  resize(size());
}

void KJotsMain::removeFromHotlist()
{
  QFileInfo fi(current_folder_name);
  QString name = fi.fileName();
  if( !hotlist.contains(name) )
    return;
  hotlist.remove(name);
  int index = folder_list.find(name);
  if( index < 0 )
    return;
  int id = folders->idAt(index);
  QButton *but = bg_top->find(id);
  bg_top->remove(but);
  button_list.remove( (QPushButton *) but );
  KConfig *config = KApplication::getKApplication()->getConfig();
  config->setGroup("kjots");
  writeListConf( config, "Hotlist", hotlist );
  resize(size());
}

void KJotsMain::toggleSubjList()
{
  if( subj_list->isVisible() )
    {
      subj_list->hide();
      b_list->setOn(FALSE);
    }
  else
    {
      subj_list->resize(width() / 2, height() );
      subj_list->show();
      b_list->setOn(TRUE);
    }
}
