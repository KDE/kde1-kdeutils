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

#ifndef _HEXFILE_H
#define _HEXFILE_H

#include <qapp.h>
#include <qscrbar.h> 
#include <klocale.h>

class HexFile : public QWidget {
    Q_OBJECT

public:
    HexFile(const char *filename,QWidget *parent=0, const char* name=0);
    HexFile(QWidget *parent=0);
    ~HexFile();
    paint(QPaintEvent *);
    int maxLine();
    int lines();
    const char *Title();
    const char* FileName();
    int NormWidth();
    int HorOffset();
    bool isModified() { return modified; }
    bool open(const char *filename);
    int save();
    void setFileName(const char *filename);
    
private:
    QFontMetrics *metrics;
    int maxWidth;
    enum Side { LEFT, RIGHT };
    bool modified;
    Side sideEdit;
    int curx, cury, relcur;
    char *hexdata;
    unsigned long int data_size;
    int rows,cols;
    QPixmap *pixmap;
    QFont *dispFont;
    long int lineoffset;
    int LineOffset;
    char *filename;
    int horoff;
    QBrush *leftM,*rightM;
    bool UseBig;
    QScrollBar *scrollV;
    QScrollBar *scrollH;
    
protected:
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);
    void keyPressEvent (QKeyEvent*);
    void mousePressEvent (QMouseEvent*);
    void mouseReleaseEvent (QMouseEvent*);
    void focusInEvent ( QFocusEvent *);
    void focusOutEvent ( QFocusEvent *);
    void fillPixmap();
    void init();
    void changeSide();
    void calcScrolls();
    
public slots:
    void scrolled(int);
    void moved(int);

signals:
    void scrolling(int);
    void unsaved(bool);
};

#endif





