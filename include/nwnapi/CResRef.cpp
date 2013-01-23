#include "CResRef.h"

DWORD CResRef__CResRef_c = 0x005CAF20;

__declspec( naked ) CResRef::CResRef(char *){ __asm{ jmp dword ptr [CResRef__CResRef_c] } }