#ifndef _KTRANSLATER_H
#define _KTRANSLATER_H

class KTranslater {

public:
    KTranslater( const char *catalogue = 0L );
    ~KTranslater();

    const char *translate( const char *msgid );
    
private:
    char *catalogue;
};

#endif
