// -*- C++ -*-

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

#ifndef KJotsMain_included
#define KJotsMain_included

#include <qwidget.h>
#include <qlayout.h>
#include <qdatetm.h>
#include <qstring.h>
#include <qlist.h>
#include <qstrlist.h>
#include <qdialog.h>
#include <qlined.h>
#include <qbttngrp.h>
//#include <qradiobt.h>

#include "KJotsMainData.h"

class KJotsMain;
class SubjList;
class CPopupMenu;

class TextEntry
{
  friend KJotsMain;
  friend SubjList;
public:
  TextEntry() {}
  virtual ~TextEntry() {}

protected:
  QString   text;
  QString   subject;
};

class MyMultiEdit : public QMultiLineEdit
{
  Q_OBJECT;  
public:
  MyMultiEdit (QWidget* parent=0, const char* name=0);
  ~MyMultiEdit () {}

protected slots:
  void openUrl();

protected:
  virtual void keyPressEvent (QKeyEvent* e);
  virtual void mousePressEvent (QMouseEvent *e);

  CPopupMenu *web_menu;
};

class AskFileName : public QDialog
{
  Q_OBJECT;
public:
  AskFileName(QWidget* parent=0, const char* name=0);
  virtual ~AskFileName() {}

  QString getName() { return i_name->text(); }

protected:
  QLineEdit *i_name;
};

class KJotsMain : public QWidget, private KJotsMainData
{
  Q_OBJECT;
public:
  KJotsMain( QWidget* parent = NULL, const char* name = NULL );
  virtual ~KJotsMain();

signals:
  void entryMoved(int);
  void entryChanged(int);
  void folderChanged(QList<TextEntry> *);

public slots:
  void saveFolder();

protected slots:
  void openFolder(int id);
  void createFolder();
  void deleteFolder();
  void nextEntry();
  void prevEntry();
  void newEntry();
  void deleteEntry();
  void barMoved(int);
  void normalSize();
  void mediumSize();
  void largeSize();
  void startHelp();
  void about();
  void addToHotlist();
  void removeFromHotlist();
  void toggleSubjList();
  
protected:
  virtual void resizeEvent ( QResizeEvent *e );
  int readFile(QString name);
  int writeFile( QString name);

  QGridLayout   *top2bottom;
  QBoxLayout    *bg_bot_layout;
  QGridLayout   *bg_top_layout;
  QGridLayout   *labels_layout;
  QGridLayout   *f_text_layout;

  QList<TextEntry> entrylist;
  int              current;
  bool             folderOpen;
  QString          current_folder_name;
  //QString          folder_list_str;
  QStrList         folder_list;
  QStrList         hotlist;
  //QString          hotlist_str;
  QList<QPushButton> button_list;
  QPopupMenu      *folders;
  QFont            button_font;
  int              unique_id;
  SubjList        *subj_list;

};
#endif // KJotsMain_included



