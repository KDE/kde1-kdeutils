#include <qimage.h>
#include <kabapi.h>
#include <iostream.h>
#include <kapp.h>
#include "jpeg.h"

int main(int argc, char** argv)
{
  KApplication app(argc, argv);
  KabAPI::ErrorCode ec;
  AddressBook::Entry entry;
  list<AddressBook::Entry> entries;
  string key;
#ifdef HAVE_LIBJPEG
  QImageIO::defineIOHandler("JFIF","^\377\330\377\340", 
			    0, read_jpeg_jfif, NULL);
  cout << "main: registered JPEG reader." << endl;
#else
  cout << 
    "main: the JPEG reader has not been "
    "included in this executable." 
       << endl;   
#endif
  KabAPI api;
  // -----
  cout << "main: starting." << endl;
  if(api.init(false)!=KabAPI::NoError)
    {
      cout << "main: initialization error." << endl;
      return -1;
    }
  if(api.add(entry, key)==KabAPI::NoError)
    {
      cout << "main: entry added, key is " << key 
	   << "." << endl;
    } else {
      cout << "main: no entry added." << endl;
    }
  if(api.remove(key))
    {
      cout << "main: entry " << key << " removed." << endl;
    } else {
      cout << "main: entry " << key << " not removed." << endl;
    }
  if(api.getEntries(entries)==KabAPI::NoError)
    {
      cout << "main: " << entries.size() 
	   << " entries retrieved from database." << endl;
    } else {
      cout << "main: error retrieving all entries at once." 
	<< endl;
    }
  if(api.exec())
    {
      cout << "main: accepted." << endl;
      if((ec=api.getEntry(entry, key))!=KabAPI::NoError)
	{
	  switch(ec)
	    {
	    case KabAPI::NoEntry: 
	      cout << "main: there is no entry." << endl;
	      break;
	    default: 
	      cout << "main: unknown error." << endl;
	    }
	} else {
	  cout << "main: the selected entry is "
	       << entry.firstname << " " << entry.name 
	       << ", its key is " << key << "." << endl;
	}
    } else {
      cout << "main: rejected." << endl;
    }
  return 0;
}

