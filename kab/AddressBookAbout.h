/* -*- C++ -*-
 *********************************************************************

	--- Qt Architect generated file ---

	File: AddressBookAbout.h
	Last generated: Tue Feb 3 14:07:07 1998
	$Id$
 *********************************************************************/

#ifndef AddressBookAbout_included
#define AddressBookAbout_included

#include "AddressBookAboutData.h"

class AddressBookAbout : public AddressBookAboutData
{
    Q_OBJECT

public:

    AddressBookAbout
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~AddressBookAbout();

protected:
  void initializeGeometry();
  int ix, iy; // image width and height
protected slots:
  void mail(const char*);
signals:
  void sendEmail(const char*);
};
#endif // AddressBookAbout_included
