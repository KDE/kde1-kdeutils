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
#include <qlist.h>

struct HexCursor {
    QRect fields[16][2];
};

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
    int labelOffset;
    char *filename;
    int horoff;
    QBrush *leftM,*rightM;
    bool UseBig;
    QScrollBar *scrollV;
    QScrollBar *scrollH;
    QList<HexCursor> rects;
    unsigned long int currentByte;
    int cursorHeight;
    int cursorPosition;

protected:
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);
    void keyPressEvent (QKeyEvent*);
    void mousePressEvent (QMouseEvent*);
    void mouseReleaseEvent (QMouseEvent*);
    void focusInEvent ( QFocusEvent *);
    void focusOutEvent ( QFocusEvent *);
    void fillPixmap();
    int fillLine(QPainter& p, int line);
    int paintLabel( QPainter& p, long int label, int y);
    void paintCursor(QPainter& p);
    QColor colorPosition(int field);
    int calcPosition( int field );
    void init();
    void changeSide();
    void calcScrolls();
    void calcCurrentByte();

public slots:
    void scrolled(int);
    void moved(int);

signals:
    void scrolling(int);
    void unsaved(bool);
};

#endif





