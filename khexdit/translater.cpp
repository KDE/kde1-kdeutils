#include <config.h>

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

extern "C" {
#include <libintlP.h>
}

#include "translater.h"
#include <kapp.h>
#include <Kconfig.h>
#include <iostream.h>

KTranslater::KTranslater( const char *_catalogue )
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
}

KTranslater::~KTranslater()
{
  delete [] catalogue;
}

const char *KTranslater::translate(const char *msgid)
{
  return dcgettext( catalogue, msgid, LC_MESSAGES);
}
