#include <qfiledlg.h>
#include <qchkbox.h>

#include "fileselect.moc"

FileSelect::FileSelect(QWidget *parent, const char *name, const char *filter)
    : QFileDialog(name, filter, parent, name, TRUE)
{
    hide();
    resize(450, 350);
    //    readonly = new QCheckBox("Open read only", this, "ro");
}

FileSelect::~FileSelect()
{
}

