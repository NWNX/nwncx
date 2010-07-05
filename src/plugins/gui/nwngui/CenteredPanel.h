#ifndef CenteredPanel_h_
#define CenteredPanel_h_
#include "NWNCommon.h"
#include "CGuiPanel.h"
#include "CGuiModalPanel.h"

template <class T>
class CenteredPanel : public T
{
	void Reposition();

};

/*
class CGuiButton : public CGuiObject
{
public:

	CGuiButton();


	DWORD field_20, field_24;
	void *HandlerParam;
	CGUIHANDLER ClickHandler;
	DWORD field_30, field_34;
	char *ButtonState;
	DWORD field_3C;
};
*/

#endif // CenteredPanel_h_