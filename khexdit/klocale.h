#ifndef _KTRANSLATER_H
#define _KTRANSLATER_H

class KLocale {

public:
    KLocale( const char *catalogue = 0L );
    ~KLocale();

    const char *translate( const char *index );
    const char *translate( const char *index, const char *text);

private:
    char *catalogue;
};

#endif
