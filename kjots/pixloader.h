// -*- C++ -*-

//
//  ktaskbar
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

#ifndef PIXLOADER_H
#define PIXLOADER_H

#include <qapp.h>
#include <qlist.h>
#include <qstrlist.h>
#include <qstring.h>
#include <qtablevw.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qlined.h>
#include <qpushbt.h>

#include <kapp.h>


class PixmapCanvas : public QTableView
{
  Q_OBJECT;
public:
  PixmapCanvas (QWidget *parent=0, const char *name=0);
  ~PixmapCanvas ();

  void loadDir(QString dir_name, QString filter);
  QString getCurrent() { return name_list.at(sel_id); }

signals:
  void nameChanged( const char * );
  void doubleClicked();

protected:
  virtual void resizeEvent( QResizeEvent *e );

  void paintCell( QPainter *p, int r, int c );
  void enterEvent( QEvent *e ) { setMouseTracking(TRUE); }
  void leaveEvent( QEvent *e ) { setMouseTracking(FALSE); }
  void mouseMoveEvent( QMouseEvent *e );
  void mousePressEvent( QMouseEvent *e );
  void mouseDoubleClickEvent( QMouseEvent *e );

  int            sel_id;
  int            max_width;
  int            max_height;
  QList<QPixmap> pixmap_list;
  QStrList       name_list;
};

class PixLoaderDialog : public QDialog
{
  Q_OBJECT;
public:
  PixLoaderDialog ( QWidget *parent=0, const char *name=0 );
  ~PixLoaderDialog ();

  QString getCurrent() { return canvas->getCurrent(); }
  void setDir( QString n ) { dir_name = n; }
  int exec(QString filter);

protected slots:
  void filterChanged();

protected:
  virtual void resizeEvent( QResizeEvent *e );

  PixmapCanvas *canvas;
  QString       dir_name;
  QLabel       *l_name;
  QLineEdit    *i_filter;
  QLabel       *l_filter;
  QPushButton  *ok;
  QPushButton  *cancel;
  QLabel       *text;
};

class PixmapLoader : public QObject
{
  Q_OBJECT;
public:
  PixmapLoader ( KConfig *conf, QString app_name, QString var_name );
  ~PixmapLoader ();

  QPixmap loadPixmap( QString name );
  QPixmap selectPixmap(QString &name, QString filter);
  void setCaching( bool b );

protected:
  KConfig         *config;
  QString          pixmap_path;
  QStrList         name_list;
  QList<QPixmap>   pixmap_list;
  PixLoaderDialog *pix_dialog;
  bool             caching;
};

#endif // PIXLOADER_H


