#include <KStatusbar.h>
#include "KStatusbar.moc"
#include <qpainter.h> 

KStatusBar::KStatusBar(QWidget *parent, const char *name) :
  QFrame(parent, name, 0, FALSE) {
    resize(100,19);
};

void KStatusBar::drawContents(QPainter *p) {
  p->drawText(10,15,"TODO: Implementation");
};
