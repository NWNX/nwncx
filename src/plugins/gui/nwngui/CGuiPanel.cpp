#include "CGuiPanel.h"

DWORD CGuiPanel__CGuiPanel = 0x005E3740;
DWORD CGuiPanel__SetLayoutFile = 0x00430630;
DWORD CGuiPanel__SetAutoDeleteOnClose = 0x005E3C20;
DWORD CGuiPanel__SetPositionScreenCenter = 0x005E3FB0;
DWORD CGuiPanel__Activate = 0x005E3900;
DWORD CGuiPanel__Deactivate = 0x005E3AA0;

CGuiPanel::CGuiPanel(){ __asm{ pop ecx
leave
jmp dword ptr [CGuiPanel__CGuiPanel] }}
void __declspec( naked ) CGuiPanel::SetLayoutFile(CResRef sFileName){ __asm{ jmp dword ptr [CGuiPanel__SetLayoutFile] }}
void __declspec( naked ) CGuiPanel::SetAutoDeleteOnClose(){ __asm{ jmp dword ptr [CGuiPanel__SetAutoDeleteOnClose] }}
void __declspec( naked ) CGuiPanel::SetPositionScreenCenter(){ __asm{ jmp dword ptr [CGuiPanel__SetPositionScreenCenter] }}
void __declspec( naked ) CGuiPanel::Activate(){ __asm{ jmp dword ptr [CGuiPanel__Activate] }}
void __declspec( naked ) CGuiPanel::Deactivate(int a1){ __asm{ jmp dword ptr [CGuiPanel__Deactivate] }}