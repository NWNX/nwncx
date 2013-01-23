#include <windows.h>
#ifndef CExoString_h_
#define CExoString_h_

class CExoString
{
public:
	char *Text;
	ULONG Length;

	CExoString();
	CExoString(char const *);
	~CExoString();
	CExoString & operator=(char const *);
	CExoString & operator=(CExoString const &);
	CExoString operator+(CExoString const &) const;

};

#endif