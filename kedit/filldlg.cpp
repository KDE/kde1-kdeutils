#include "filldlg.h"


FillDlg::FillDlg(QWidget *parent, const char *name)
     : QDialog(parent, name, TRUE)

{
    frame1 = new QGroupBox("Fill Column", this, "frame1");
    values = new QLineEdit( this, "values");
    this->setFocusPolicy(QWidget::StrongFocus);
    connect(values, SIGNAL(returnPressed()), this, SLOT(checkit()));
    fill_column = new QCheckBox("Set Fill-Column at:", frame1, "fill");
    word_wrap = new QCheckBox("Word Warp", frame1, "word");
    ok = new QPushButton("OK", this, "OK");
    cancel = new QPushButton("Cancel", this, "cancel");
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ok, SIGNAL(clicked()), this, SLOT(checkit()));
    resize(300, 150);
}

struct fill_struct  FillDlg::getFillCol() { 

  QString string;

  fillstr.word_wrap_is_set = word_wrap->isChecked();
  fillstr.fill_column_is_set = fill_column->isChecked();

  string = values->text();  
  fillstr.fill_column_value = string.toInt();

  return fillstr;

}



bool FillDlg::fill(){

  if (fill_column->isChecked())
    return true;
  else
    return false;


};

void FillDlg::setWidgets(struct fill_struct fill){

  QString string;

  fill_column->setChecked(fill.fill_column_is_set);
  word_wrap->setChecked(fill.word_wrap_is_set);
  values->setText(string.setNum(fill.fill_column_value));

}

bool FillDlg::wordwrap(){

  if (word_wrap->isChecked())
    return true;
  else
    return false;

};


void FillDlg::resizeEvent(QResizeEvent *)
{
    frame1->setGeometry(5, 5, width() - 10, height() - 45);
    cancel->setGeometry(width() - 80, height() - 30, 70, 25);
    ok->setGeometry(width() - 160, height() - 30, 70, 25);
    values->setGeometry(170, 35, 70, 25);
    fill_column->setGeometry(20, 30, 140, 25);
    word_wrap->setGeometry(20, 65, 140, 25);
}

void FillDlg::checkit(){

  bool ok;
  QString string;

  string = values->text();
  string.toInt(&ok);

  if (ok){
    accept();
  }
 
  QMessageBox::message("Sorry","You must enter an integer.","OK");

}


#include "filldlg.moc"
