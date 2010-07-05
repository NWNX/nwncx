#include "CMyPanel.h"

CMyPanel::CMyPanel()
{
	SetLayoutFile("EXAMINE_BASIC");
	SetAutoDeleteOnClose();
}

void CMyPanel::PostAttachmentInitialize()
{
	btnX.ImportLayoutData(this, "XButton");
	btnX.ClickHandler = static_cast<CGUIHANDLER>(&CMyPanel::CloseMe);
	btnX.HandlerParam = (void *) this;
}

void CMyPanel::CloseMe()
{
	this->Deactivate(0);
}