// ttystr.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997

#ifndef TTYSTR_H
#define TTYSTR_H

#include <qstring.h>

class Ttystr {
public:
    static QString name(int uid);

private:
    static void read_devs();
    static int minor(int dev) { return dev & 0xff; };
    static int major(int dev) { return dev >> 8; };
    static int major_index(int major);
    static char *tabstr(int devnr);

    static char *tab;
    
    const int DEVNAMELEN = 8;
    static dev_t major_numbers[6];	// {2, 3, 4, 5, 19, 20}
    const int NMAJORS = sizeof(major_numbers) / sizeof(dev_t);
    const int NMINORS = 256;
    const int NDEVS = NMAJORS * NMINORS;
    const int DEVTABSIZE = NDEVS * DEVNAMELEN;
};

#endif	// TTYSTR_H
