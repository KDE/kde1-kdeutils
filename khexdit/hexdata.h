#include <qstring.h>

class HexData : protected QByteArray {

public:    
    HexData(const char *filename);
    HexData();

    load(const char *filename);
    save(const char *filename);

    uchar operator[] (unsigned long i) { return hexdata[i]; }
    uchar byteAt(unsigned long i) { return hexdata[i]; }

    ulong size() { return data_size; }
    void changeByte(ulong i, uchar value) { hexdata[i] = value; }
		    
		    
private:
    uchar *hexdata;
    ulong data_size;
};
