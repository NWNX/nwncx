#include "Gob.h"

DWORD Gob__GetName = 0x007A9510;
DWORD Gob__GetModelName = 0x007AE6A0;
DWORD Gob__GetPart = 0x007A8940;

__declspec( naked ) char * Gob::GetName(){ __asm{ jmp dword ptr [Gob__GetName] }}
__declspec( naked ) char * Gob::GetModelName(){ __asm{ jmp dword ptr [Gob__GetModelName] }}
__declspec( naked ) void * Gob::GetPart(const char *){ __asm{ jmp dword ptr [Gob__GetPart] }}
