/*
 *   khexdit - a little hex editor
 *   Copyright (C) 1996,97  Stephan Kulow
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef _HEXWIDGET_H
#define _HEXWIDGET_H

#include <qmenubar.h>
#include <qlist.h>
#include <qpopmenu.h>

#include <kfm.h>
#include <kapp.h>
#include <kurl.h>
#include <klocale.h>

class HexFile;

class HexWidget : public KTopLevelWidget {
    Q_OBJECT
    
public:

    enum KIND_OF_OPEN {
	READONLY, READWRITE
    };
    enum action { GET, PUT };
    
    HexWidget();
    HexWidget(const char*);
    ~HexWidget();
    void open(const char*, KIND_OF_OPEN kind);
    void open(const char *fileName, const char *url, KIND_OF_OPEN);
    void openURL(const char *fileName, KIND_OF_OPEN kind);
    
protected slots:
    void menuCallback(int);
    void slotDropEvent( KDNDDropZone * _dropZone );
    virtual void saveProperties(KConfig*);
    virtual void readProperties(KConfig*);
    void unsaved(bool flag);
    /// Gets signals from KFM
    void slotKFMFinished();

private:
    QList<HexFile> files;
    HexFile *CurrentFile;
    int winID;
    QString netFile;
    KToolBar *toolbar; 
    KMenuBar *menu;
    KDNDDropZone * dropZone;
    /**
       Only one KFM connection should be opened at once. Otherwise kedit could get
       confused. If this is 0L, you may create a new connection to kfm.
    */
    KFM * kfm;
    
    /**
       If KEdit is waiting for some internet task to finish, this is the
       file that is involved. Mention that it is a complete URL like
       "file:/tmp/mist.txt".
    */
    QString tmpFile;
    /**
       If this is for example GET, then KFM loads a file from the net
       to the local file system.
    */
    action kfmAction; 
    
protected:
    static QList<HexWidget> windowList;
    int initMenu();
    //void paintEvent(QPaintEvent *pa);
    void closeEvent ( QCloseEvent *e);
    int scrollVWidth,scrollHHeight;
    void initGeometry();
};

#endif



