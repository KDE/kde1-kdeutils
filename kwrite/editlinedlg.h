#ifndef _EDITLINEDLG_H_
#define _EDITLINEDLG_H_


#include <qdialog.h>
#include <qlineedit.h>
#include <qstring.h>


class EditLineDialog : public QDialog
{
  Q_OBJECT
  
public:

  EditLineDialog(QString title, const char *t, QWidget *parent=0, const char *name=0, bool modal=false);
  
  void setText(const char *text) { lineEdit->setText(text); };
  const char *getText() { return lineEdit->text(); };
  
private:

  QLineEdit *lineEdit;
      
};


#endif