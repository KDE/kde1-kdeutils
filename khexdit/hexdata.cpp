#include "hexdata.h"
#include <qfile.h>
#include <unistd.h>
#include <sys/mman.h>
#include <kapp.h>
#include <qmsgbox.h>

HexData::HexData() {
    hexdata = 0;
    data_size = 0;
}

int HexData::save(const char *filename) {
    QFile file(filename);
    file.open(IO_Truncate | IO_WriteOnly | IO_Raw);
    file.writeBlock((char*)hexdata, data_size);
    file.close();
    return 0;
}

int HexData::load(const char *Filename) {
    QString fileString(Filename);
    QFile file(fileString);
    if (!file.open(IO_ReadOnly | IO_Raw)) {
	QString txt;
	txt.sprintf(i18n("Error opening %s"),fileString.data());
	QMessageBox::message(i18n("Error"),txt,
			     i18n("Close"));
	return -1;
    }
    hexdata = (unsigned char*)mmap(0, file.size(),  PROT_READ | PROT_WRITE, MAP_PRIVATE,
				   file.handle(), 0);
    data_size = file.size();
    return 0;
}

