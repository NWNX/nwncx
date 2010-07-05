#include "CGuiModalPanel.h"

DWORD CGuiModalPanel__CGuiModalPanel = 0x004338D0;
DWORD CGuiModalPanel__Activate = 0x005E4620;
DWORD CGuiModalPanel__Deactivate = 0x005E4650;
void __declspec( naked ) CGuiModalPanel::Activate(){ __asm{ jmp dword ptr [CGuiModalPanel__Activate] }}
void __declspec( naked ) CGuiModalPanel::Deactivate(int a1){ __asm{ jmp dword ptr [CGuiModalPanel__Deactivate] }}
//__declspec( naked ) CGuiModalPanel::CGuiModalPanel(int modal){ __asm{ jmp dword ptr [CGuiModalPanel__CGuiModalPanel] }}
