/**********************************************************************

	--- Dlgedit generated file ---

	File: PaperData.cpp
	Last generated: Tue Jun 24 22:20:20 1997

	DO NOT EDIT!!!  This file will be automatically
	regenerated by dlgedit.  All changes will be lost.

 *********************************************************************/

#include "PaperData.h"

#define Inherited QDialog

#include <qbttngrp.h>
#include <qcombo.h>
#include <qframe.h>
#include <qlabel.h>
#include <kapp.h>
#include <klocale.h>



PaperData::PaperData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
  	QFrame* tmpQFrame;
	tmpQFrame = new QFrame( this, "Frame_1" );
	tmpQFrame->setGeometry( 10, 10, 320, 235 );
	tmpQFrame->setFrameStyle( 49 );
	

	FormatComboBox = new QComboBox( FALSE, this, "FormatCombo" );
	FormatComboBox->setGeometry( 25, 60, 100, 25 );
	connect( FormatComboBox, SIGNAL(activated(const char*)), 
		 SLOT(FormatSelected(const char*)) );
	FormatComboBox->setSizeLimit( 10 );
	FormatComboBox->setAutoResize( FALSE );
	FormatComboBox->insertItem( "A4" );
	FormatComboBox->insertItem( "Letter" );
	FormatComboBox->insertItem( "Legal" );
	FormatComboBox->insertItem( "Executive" );
	FormatComboBox->insertItem( "Com10" );
	FormatComboBox->insertItem( "B5" );
	FormatComboBox->insertItem( "C5" );
	FormatComboBox->insertItem( "DL" );
	FormatComboBox->insertItem( "Monarch" );

	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( this, "FormatLabel" );
	tmpQLabel->setGeometry( 25, 35, 100, 20 );
	tmpQLabel->setText( i18n("Format") );
	tmpQLabel->setAlignment( 289 );
	tmpQLabel->setMargin( -1 );

	tmpQLabel = new QLabel( this, "CopiesLabel" );
	tmpQLabel->setGeometry( 25, 100, 100, 20 );
	tmpQLabel->setText(i18n( "Copies") );
	tmpQLabel->setAlignment( 289 );
	tmpQLabel->setMargin( -1 );

	numberofcopies = new QLineEdit( this, "CopiesEdit" );
	numberofcopies->setGeometry( 25, 125, 100, 25 );
	numberofcopies->setText( "1" );
	numberofcopies->setMaxLength( 32767 );
	numberofcopies->setEchoMode( QLineEdit::Normal );
	numberofcopies->setFrame( TRUE );
	connect(numberofcopies,SIGNAL(textChanged(const char*)),this,
		SLOT(copiesChanged(const char*)));

	tmpQLabel = new QLabel( this, "LengthLabel" );
	tmpQLabel->setGeometry( 25, 160, 100, 20 );
	tmpQLabel->setText(i18n( "Lines") );
	tmpQLabel->setAlignment( 289 );
	tmpQLabel->setMargin( -1 );

	LengthEdit = new QLineEdit( this, "LengthEdit" );
	LengthEdit->setGeometry( 25, 185, 100, 25 );
	LengthEdit->setText( "66" );
	LengthEdit->setMaxLength( 32767 );
	LengthEdit->setEchoMode( QLineEdit::Normal );
	LengthEdit->setFrame( TRUE );
	connect(LengthEdit,SIGNAL(textChanged(const char*)),this,
		SLOT(linesChanged(const char*)));

	OrientButtonGroup = new QButtonGroup( this, "OrientationGroup" );
	OrientButtonGroup->setGeometry( 145, 30, 170, 90 );
	OrientButtonGroup->setFrameStyle( 49 );
	OrientButtonGroup->setTitle( "Orientation" );
       	OrientButtonGroup->setAlignment( 1 );

	PortraitCheckBox = new QRadioButton( OrientButtonGroup, "RadioButton_1" );
	PortraitCheckBox->move(15,25);
	connect( PortraitCheckBox, SIGNAL(clicked()), SLOT(ProtraitCheckBoxClicked()) );
	PortraitCheckBox->setText( i18n("Portrait") );
	PortraitCheckBox->setAutoRepeat( FALSE );
	PortraitCheckBox->setAutoResize( TRUE );
	PortraitCheckBox->setChecked( TRUE );
	PortraitCheckBox->raise();

	LandscapeCheckBox = new QRadioButton( OrientButtonGroup, "LandscapeCheckBox" );
	LandscapeCheckBox->move(15,55);
	connect( LandscapeCheckBox, SIGNAL(clicked()), SLOT(LandscapeCheckBoxClicked()) );
	LandscapeCheckBox->setText( i18n("Landscape") );
	LandscapeCheckBox->setAutoRepeat( FALSE );
	LandscapeCheckBox->setAutoResize( TRUE );


	MiscButtonGroup = new QButtonGroup( this, "Miscellaneous" );
	MiscButtonGroup->setGeometry( 145, 135, 170, 80 );
	MiscButtonGroup->setFrameStyle( 49 );
	MiscButtonGroup->setTitle( i18n("Miscellaneous") );
	MiscButtonGroup->setAlignment( 1 );

	ManualFeedCheckBox = new QCheckBox( MiscButtonGroup, "CheckBox_1" );
	ManualFeedCheckBox->move(15,25);
	connect( ManualFeedCheckBox, SIGNAL(clicked()), 
		 SLOT(ManaulFeedCheckBoxClicked()) );

	ManualFeedCheckBox->setText(i18n( "Manual Feed") );
	ManualFeedCheckBox->setAutoRepeat( FALSE );
	ManualFeedCheckBox->setAutoResize( TRUE );

	AutoContinueCheckBox = new QCheckBox( MiscButtonGroup, "CheckBox_2" );
	AutoContinueCheckBox->move(15,55);
	connect( AutoContinueCheckBox, SIGNAL(clicked()), 
		 SLOT(AutoContinueCheckBoxClicked()) );

	AutoContinueCheckBox->setText( i18n("Auto Continue") );
	AutoContinueCheckBox->setAutoRepeat( FALSE );
	AutoContinueCheckBox->setAutoResize( TRUE );


	resize( 330, 255 );
}


PaperData::~PaperData()
{
}
void PaperData::FormatSelected(const char* format)
{
  if(!data)
    return;

  data->Format = format;
  data->Format.detach();

}
void PaperData::ProtraitCheckBoxClicked()
{
  if(!data)
    return;

  if(PortraitCheckBox->isChecked()){
    data->Orientation = "Portrait";
    data->Orientation.detach();
  }
}


void PaperData::LandscapeCheckBoxClicked()
{

  if(!data)
    return;

  if(LandscapeCheckBox->isChecked()){
    data->Orientation = "Landscape";
    data->Orientation.detach();
  }

}
void PaperData::ManaulFeedCheckBoxClicked()
{
  if(!data)
    return;

  if(ManualFeedCheckBox->isChecked()){
    data->Manualfeed = "on";
    data->Manualfeed.detach();
  }
  else{
    data->Manualfeed = "off";
    data->Manualfeed.detach();
  }
}

void PaperData::AutoContinueCheckBoxClicked()
{
  if(!data)
    return;

  if(AutoContinueCheckBox->isChecked()){
    data->Autocont = "on";
    data->Autocont.detach();
  }
  else{
    data->Autocont = "off";
    data->Autocont.detach();
  }
}


void PaperData::copiesChanged(const char *ncopies){

  if(!data)
    return;
  
  data->Copies = ncopies;
  data->Copies.detach();
  

}

void PaperData::linesChanged(const char *nlines){

  if(!data)
    return;
  
  data->Formlines = nlines;
  data->Formlines.detach();
  

}
