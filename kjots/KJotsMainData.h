// -*- C++ -*-

//
//  kjots
//
//  Copyright (C) 1997 Christoph Neerfeld
//  email:  Christoph.Neerfeld@mail.bonn.netsurf.de
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef KJotsMainData_included
#define KJotsMainData_included

#include <qbttngrp.h>
#include <qframe.h>
#include <qlabel.h>
#include <qmenubar.h>
#include <qmlined.h>
#include <qlined.h>
#include <qpushbt.h>
#include <qscrbar.h>

class KJotsMainData
{
public:

    KJotsMainData
    (
        QWidget* parent
    );


    QFrame* f_text;
    QFrame* f_labels;
    QFrame* f_main;
    QButtonGroup* bg_top;
    QButtonGroup* bg_bot;
    QMenuBar* menubar;
    QPushButton* b_delete;
    QPushButton* b_prev;
    QPushButton* b_next;
    QPushButton* b_exit;
    QPushButton* b_new;
    QPushButton* b_list;
    QScrollBar* s_bar;
    QMultiLineEdit* me_text;
    QLabel* l_folder;
    QLineEdit* le_subject;
};

#endif // KJotsMainData_included
