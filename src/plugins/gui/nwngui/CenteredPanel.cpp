#include "CenteredPanel.h"

DWORD CenteredPanel__Reposition = 0x00466EB0;

void __declspec( naked ) CenteredPanel<CGuiPanel>::Reposition(){ __asm{ jmp dword ptr [CenteredPanel__Reposition] }}
void __declspec( naked ) CenteredPanel<CGuiModalPanel>::Reposition(){ __asm{ jmp dword ptr [CenteredPanel__Reposition] }}