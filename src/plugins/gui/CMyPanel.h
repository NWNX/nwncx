#include <windows.h>
#include "nwngui/NWNGUI.h"

class CMyPanel : public CenteredPanel<CGuiModalPanel>
{
public:
	CMyPanel();

	void PostAttachmentInitialize();
	void CloseMe();

	CGuiButton btnX;
};
