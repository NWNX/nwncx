#include "NWNCommon.h"

DWORD CResRef__CResRef = 0x005CAE80;
DWORD CResRef__CResRef_char = 0x005CAF20;
DWORD CResRef__CResRef_destructor = 0x005CAF20;
__declspec( naked ) CResRef::CResRef(){ __asm{ jmp dword ptr [CResRef__CResRef] }}
__declspec( naked ) CResRef::CResRef(char *){ __asm{ jmp dword ptr [CResRef__CResRef_char] }}