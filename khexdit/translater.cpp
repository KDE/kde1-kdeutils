#include <config.h>

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

extern "C" {
#include <libintlP.h>
}

#include "translater.h"
#include <kapp.h>

KTranslater::KTranslater()
{

#ifdef HAVE_SETLOCALE
  /* Set locale via LC_ALL.  */
  setlocale (LC_ALL, "");
#endif

  /* Set the text message domain.  */
  bindtextdomain ( kapp->appName().data() , kapp->kdedir() + "/locale");
  textdomain ( kapp->appName().data() );
}

const char *KTranslater::translate(const char *msgid)
{
  return gettext(msgid);
}
