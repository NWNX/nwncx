#ifndef _CCONNECTIONLIB_H_
#define _CCONNECTIONLIB_H_
#include "nwndef.h"
#include "CExoString.h"

class CConnectionLib
{
public:
	void AddServer(void *, char *, char *, int, int, int, int, char *, int, int, char *, int, int, int, char *, char *, int, int, int, int, int, bool);
	void ClearServers();
	void UpdateConnectionPhase(unsigned char,CExoString const &);

	/* 0x0/0 */ unsigned long field_0;
};
#endif
