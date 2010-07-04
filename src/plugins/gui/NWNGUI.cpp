#include "NWNGUI.h"
/*
void init(){
	int (CGuiObject::*func)() = reinterpret_cast<int (__thiscall CGuiObject::* )(void)> (0x005E4D70);
}*/

DWORD CResRef__CResRef = 0x005CAE80;
DWORD CResRef__CResRef_char = 0x005CAF20;
DWORD CResRef__CResRef_destructor = 0x005CAF20;
__declspec( naked ) CResRef::CResRef(){ __asm{ jmp dword ptr [CResRef__CResRef] }}
__declspec( naked ) CResRef::CResRef(char *){ __asm{ jmp dword ptr [CResRef__CResRef_char] }}

DWORD CGuiObject__CGuiObject = 0x005E4D40;
DWORD CGuiObject__SetEnabled = 0x005E5020;

CGuiObject::CGuiObject(){ __asm{ pop ecx
leave
jmp dword ptr [CGuiObject__CGuiObject] }}
void __declspec( naked ) CGuiObject::SetEnabled(int a1){ __asm{ jmp dword ptr [CGuiObject__SetEnabled] }}

DWORD CGuiPanel__CGuiPanel = 0x005E3740;
DWORD CGuiPanel__SetLayoutFile = 0x00430630;
DWORD CGuiPanel__SetAutoDeleteOnClose = 0x005E3C20;
CGuiPanel::CGuiPanel(){ __asm{ pop ecx
leave
jmp dword ptr [CGuiPanel__CGuiPanel] }}
void __declspec( naked ) CGuiPanel::SetLayoutFile(CResRef sFileName){ __asm{ jmp dword ptr [CGuiPanel__SetLayoutFile] }}
void __declspec( naked ) CGuiPanel::SetAutoDeleteOnClose(){ __asm{ jmp dword ptr [CGuiPanel__SetAutoDeleteOnClose] }}

DWORD CGuiModalPanel__CGuiModalPanel = 0x004338D0;
DWORD CGuiModalPanel__Activate = 0x005E4620;
void __declspec( naked ) CGuiModalPanel::Activate(){ __asm{ jmp dword ptr [CGuiModalPanel__Activate] }}
//__declspec( naked ) CGuiModalPanel::CGuiModalPanel(int modal){ __asm{ jmp dword ptr [CGuiModalPanel__CGuiModalPanel] }}


CMyPanel::CMyPanel()
{
	//CResRef pRef("EXAMINE_BASIC");
	__asm { mov  dword ptr [eax], 008A72F4h}
	SetLayoutFile("EXAMINE_BASIC");
	SetAutoDeleteOnClose();
}

void init()
{
	CGuiPanel *gui = new CGuiPanel();
	delete gui;
}