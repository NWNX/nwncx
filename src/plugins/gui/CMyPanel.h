#include <windows.h>
#include "nwngui/NWNGUI.h"

class CMyPanel : public CenteredPanel<CGuiPanel>
{
public:
	CMyPanel();

	void PostAttachmentInitialize();
	void CloseMe();

	CGuiButton btnX;
};
