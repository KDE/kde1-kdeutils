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
  paint(QPaintEvent *);
  int maxLine();
  int lines();
  const char *Title();
  const char* FileName();
  int NormWidth();
  int HorOffset();
  bool isModified() {
    return modified;
  };
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
  QByteArray hexdata;
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





