#ifndef __DOCKED__H__
#define __DOCKED__H__

#include <qwidget.h>
#include <qpixmap.h>
#include <stdio.h>
#include <kprocess.h>

#define ICONWIDTH 24
#define ICONHEIGHT 24

class Sysload : public QWidget {
  Q_OBJECT
public:
  Sysload();
  virtual void mousePressEvent(QMouseEvent *);

private slots:
  void doUpdate();

private:
  int idx;
  KProcess proc;

  float history[ICONWIDTH];
  float sysload();
  void clicked();
};

#endif
