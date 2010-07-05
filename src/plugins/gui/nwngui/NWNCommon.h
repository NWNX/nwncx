#ifndef NWNCommon_h_
#define NWNCommon_h_
#include <windows.h>

class CResRef
{
public:
	char ResRef[16];

	CResRef();
	CResRef(char *);
};

class CGuiObject;
class CGuiPanel;
class CGuiModalPanel;


#endif // NWNCommon_h_