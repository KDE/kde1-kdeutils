/***********************************************************

These are a few miscellaneous string editing functions.

************************************************************/

#ifdef C_DEBUG
#include "debug.h"
#endif
#include "text.h"

/* This function removes num_rem characters at point *find, 
   moving the remaining characters back to fill in the spot */
   
void strshort( char *start, int num_rem )
{
	int c=0;
	char *orig = start;
	while( c < num_rem )
	{
		while( *start != '\0' )
		{
			*start = *(start+1);
			start++;
		}
		c++;
		start = orig;
	}
}