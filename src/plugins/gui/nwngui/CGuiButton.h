#ifndef CGuiButton_h_
#define CGuiButton_h_
#include "NWNCommon.h"
#include "CGuiObject.h"

class CGuiButton : public CGuiObject
{
public:

	CGuiButton();

	/*
	00000000 CGuiButton struc ; (sizeof=0x40)
	00000000 GuiObject CGuiObject ?
	00000020 field_20 dd ?
	00000024 field_24 dd ?
	00000028 field_28 dd ?
	0000002C field_2C dd ?
	00000030 field_30 dd ?
	00000034 field_34 dd ?
	00000038 ButtonState dd ?                        ; offset
	0000003C field_3C dd ?
	00000040 CGuiButton ends
	*/

	DWORD field_20, field_24;
	void *HandlerParam;
	CGUIHANDLER ClickHandler;
	DWORD field_30, field_34;
	char *ButtonState;
	DWORD field_3C;
};


#endif // CGuiButton_h_