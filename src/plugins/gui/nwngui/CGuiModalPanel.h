#ifndef CGuiModalPanel_h_
#define CGuiModalPanel_h_
#include "NWNCommon.h"
#include "CGuiPanel.h"

class CGuiModalPanel : public CGuiPanel
{
public:
	//CGuiModalPanel(int modal);
	virtual ~CGuiModalPanel(){};

	void Activate();
	void Deactivate(int);

	DWORD bModal;
};


#endif // CGuiModalPanel_h_