#include <config.h>

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

extern "C" {
#include <libintlP.h>
}

#include "klocale.h"
#include <kapp.h>
#include <Kconfig.h>

#define SYSTEM_MESSAGES "kde"

KLocale::KLocale( const char *_catalogue )
{
    char *buffer;

#ifdef HAVE_SETLOCALE
    /* Set locale via LC_ALL.  */
    setlocale (LC_ALL, "");
#endif
    
    if ( ! _catalogue )
	_catalogue = kapp->appName().data();
    
    catalogue = new char[ strlen(_catalogue) + 1 ];
    strcpy(catalogue, _catalogue);
    
    if (! getenv("LANG") ) {
	KConfig config;
	QString languages = "C";
	config.setGroup("NLS");
	languages = config.readEntry("Language", &languages);
	// putenv needs an extra malloc!
	buffer = new char[languages.length() + 6];
	sprintf(buffer, "LANG=%s",languages.data());
	putenv(buffer);
    }
    
    /* Set the text message domain.  */
    bindtextdomain ( catalogue , kapp->kdedir() + "/locale");
    bindtextdomain ( SYSTEM_MESSAGES,  kapp->kdedir() + "/locale");
}

KLocale::~KLocale()
{
    delete [] catalogue;
}

const char *KLocale::translate(const char *msgid)
{
    char *text = dcgettext( catalogue, msgid, LC_MESSAGES);
    if (text == msgid) // just compare the pointers
	return dcgettext( SYSTEM_MESSAGES, msgid, LC_MESSAGES);
    else
	return text;
}

const char *KLocale::translate(const char *index, const char *d_text)
{
    char *text = dcgettext( catalogue, index, LC_MESSAGES);

    if (text == index) { // just compare the pointers
	text =  dcgettext( SYSTEM_MESSAGES, index, LC_MESSAGES);
	if (text == index)
	    return d_text;
    }
    return text;
    
}
