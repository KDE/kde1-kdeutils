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

#include <qtooltip.h>
#include <qapp.h>

#include "KJotsMainData.h"
#include "KJotsMain.h"
#include "pixloader.h"

extern PixmapLoader *global_pix_loader;

KJotsMainData::KJotsMainData
(
	QWidget* parent
)
{
	f_main = new QFrame( parent, "Frame_0" );
	f_main->move(0, 28);
	f_main->setMinimumSize( 500, 180 );
	f_main->setFrameStyle( 0 );

	bg_bot = new QButtonGroup( f_main, "ButtonGroup_1" );
	bg_bot->setMinimumSize( 452, 28 );
	bg_bot->setFrameStyle( 49 );
	bg_bot->setTitle( "" );
	bg_bot->setAlignment( 1 );
	bg_bot->lower();

	f_text = new QFrame( f_main, "Frame_1" );
	f_text->setGeometry( 8, 72, 452, 45 );
	f_text->setFrameStyle( 50 );
	
	f_labels = new QFrame( f_main, "Frame_2" );
	f_labels->setMinimumSize( 436, 24 );
	f_labels->setFrameStyle( 0 );

	menubar = new QMenuBar( parent, "MenuBar_1" );
	menubar->setFrameStyle( 34 );
	menubar->setLineWidth( 2 );

	b_new = new QPushButton( bg_bot, "PushButton_1" );
	//b_new->setMinimumSize( 55, 24 );
	b_new->setMinimumSize( 24, 20 );
	b_new->setPixmap( global_pix_loader->loadPixmap("filenew.xpm") );
	QToolTip::add( b_new, "New" );

	b_delete = new QPushButton( bg_bot, "PushButton_2" );
	b_delete->setMinimumSize( 24, 20 );
	b_delete->setPixmap( global_pix_loader->loadPixmap("filedel.xpm") );
	QToolTip::add( b_delete, "Delete" );

	b_prev = new QPushButton( bg_bot, "PushButton_3" );
	b_prev->setMinimumSize( 24, 20 );
	b_prev->setPixmap( global_pix_loader->loadPixmap("back.xpm") );
	QToolTip::add( b_prev, "Previous" );

	b_next = new QPushButton( bg_bot, "PushButton_4" );
	b_next->setMinimumSize( 24, 20 );
	b_next->setPixmap( global_pix_loader->loadPixmap("forward.xpm") );
	QToolTip::add( b_next, "Next" );

	b_list = new QPushButton( bg_bot, "b_list" );
	b_list->setToggleButton( TRUE );
	b_list->setMinimumSize( 24, 20 );
	b_list->setPixmap( global_pix_loader->loadPixmap("OpenBook.xpm") );
	QToolTip::add( b_list, "Subject List" );

	b_exit = new QPushButton( bg_bot, "PushButton_5" );
	b_exit->setMinimumSize( 24, 20 );
	b_exit->setPixmap( global_pix_loader->loadPixmap("exit.xpm") );
	QToolTip::add( b_exit, "Exit" );

	s_bar = new QScrollBar( f_main, "ScrollBar_1" );
	s_bar->setMinimumSize( 452, 16 );
	s_bar->setOrientation( QScrollBar::Horizontal );

	me_text = new MyMultiEdit( f_text, "MultiLineEdit_1" );
	me_text->setMinimumSize( 436, 30 );
	me_text->insertLine( "" );

	l_folder = new QLabel( f_labels, "Label_4" );
	l_folder->setMinimumSize( 68, 20 );
	l_folder->setFrameStyle( QFrame::WinPanel | QFrame::Sunken );
	l_folder->setText( "" );

	le_subject = new QLineEdit( f_labels, "le_subject" );
	le_subject->setMinimumSize( 56, 20 );
	le_subject->setText( "" );

	bg_top = new QButtonGroup( f_main, "ButtonGroup_2" );
	bg_top->setMinimumSize( 452, 32 );
	bg_top->setFrameStyle( 49 ); 
	bg_top->setTitle( "" );
	bg_top->setAlignment( 1 );
	bg_top->lower();

	parent->resize(600, 478);
	parent->setMinimumSize(500, 210);
	f_main->setGeometry( 0, 28, parent->size().width(), parent->size().height() - 28 );
}





