#include <kapp.h>
#include <qmessagebox.h>
#include <unistd.h>
#include <qtimer.h>
#include <kwm.h>
#include <qpainter.h>
#include "docked.h"

#define PROCFILE "/proc/loadavg"

float history[ICONWIDTH];

Sysload::Sysload() : QWidget(0) {
  idx = 0;
  for(int i = 0; i < ICONWIDTH; i++)
    history[i] = -1;

  QTimer *t = new QTimer(this);
  connect(t, SIGNAL(timeout()),
	  this, SLOT(doUpdate()));
  t->start(5000);
  resize(ICONWIDTH, ICONHEIGHT);
  KWM::setDockWindow(winId());
  show();
  doUpdate();
  proc << "kpm";
}

void Sysload::mousePressEvent(QMouseEvent *e) {
  if(e->button() == LeftButton)
    clicked();
}

void Sysload::clicked() {
  if(!proc.isRunning()) {
    proc.clearArguments();
    proc << "kpm";
    proc.start();
  }  else
    proc.kill();
}

void Sysload::doUpdate() {
  if(idx == ICONWIDTH-1) {
    for(int i = 0; i < ICONWIDTH-2; i++)
      history[i] = history[i+1];
    history[idx] = sysload();
  } else
    history[idx++] = sysload();

  // find highest load
  float max_load = history[0];
  for(int i = 1; i < idx; i++)
    if(max_load < history[i])
      max_load = history[i];
  
  // round toward nearest integer
  if(max_load < 1)
    max_load = 1;
  else
    max_load = int(max_load + 0.999999);

  QPixmap pm(ICONWIDTH, ICONHEIGHT);
  pm.fill(backgroundColor());
  QPainter p;
  p.begin(&pm);

  // draw baseline
  p.setPen(QPen(black, 1, DotLine));
  p.drawLine(0, ICONHEIGHT-1, ICONWIDTH, ICONHEIGHT-1);

  // draw graph
  p.setPen(red);
  for(int i = 0; i < idx; i++)
    p.drawLine(i, ICONHEIGHT, i, ICONHEIGHT-(history[i] / max_load) * ICONHEIGHT +1); 

  // draw scale
  p.setPen(black);
  for(int j = 1; j < (int)max_load; j++)
    p.drawLine(0, int(j / max_load * ICONHEIGHT), ICONWIDTH, int((j / max_load) * ICONHEIGHT));

  p.end();
  setBackgroundPixmap(pm);
}

float Sysload::sysload() {
  FILE *f = fopen(PROCFILE, "r");

  if(f) {
    char buf[128];
    float load;

    rewind(f);
    fgets(buf, sizeof(buf), f);
    buf[sizeof(buf)-1] = 0;
    sscanf(buf, "%f", &load);
    fclose(f);
    return load;
  } else
    return 0.0;
}

int main(int argc, char **argv) {
  KApplication a(argc, argv, "kpmdocked");

#ifdef linux
  if(access(PROCFILE, R_OK) != 0) {
    QMessageBox::warning(0,
			 i18n("Error"),
			 i18n("This system lacks /proc filesystem support!\n"
			      "This support is needed for proper operation,\n"
			      "so you should consider recompiling your kernel\n"
			      "with the /proc filesystem option enabled."));
    return 1;
  }

  Sysload sl;
  
  return a.exec();
#else
    QMessageBox::warning(0,
			 i18n("Error"),
			 i18n("This program requires a Linux OS, sorry!"));
    reurn 1;
#endif
}

#include "docked.moc"
