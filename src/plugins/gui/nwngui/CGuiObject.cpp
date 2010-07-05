#include "CGuiObject.h"

DWORD CGuiObject__CGuiObject = 0x005E4D40;
DWORD CGuiObject__SetEnabled = 0x005E5020;

DWORD CGuiObject__destructor = 0x005E4D70;
DWORD CGuiObject__HandleTriggerTooltip = 0x005E52E0;
DWORD CGuiObject__HandleFinishTooltip = 0x005E5350;
DWORD CGuiObject__HandleDragDropAsDest = 0x0040C9C0;
DWORD CGuiObject__ImportLayoutData = 0x005E72E0;

CGuiObject::CGuiObject(){ __asm{ pop ecx
leave
jmp dword ptr [CGuiObject__CGuiObject] }}
void __declspec( naked ) CGuiObject::SetEnabled(int a1){ __asm{ jmp dword ptr [CGuiObject__SetEnabled] }}
void __declspec( naked ) CGuiObject::HandleTriggerTooltip(){ __asm{ jmp dword ptr [CGuiObject__HandleTriggerTooltip] }}
void __declspec( naked ) CGuiObject::HandleFinishTooltip(){ __asm{ jmp dword ptr [CGuiObject__HandleFinishTooltip] }}
int __declspec( naked ) CGuiObject::HandleDragDropAsDest(int a1, int a2, int a3){ __asm{ jmp dword ptr [CGuiObject__HandleDragDropAsDest] }}
void __declspec( naked ) CGuiObject::ImportLayoutData(CGuiPanel *a1,char *a2){ __asm{ jmp dword ptr [CGuiObject__ImportLayoutData] }}

//__declspec(naked) CGuiObject::~CGuiObject(){ __asm{ jmp dword ptr [CGuiObject__destructor] }}