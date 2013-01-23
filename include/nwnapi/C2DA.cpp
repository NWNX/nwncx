#include "C2DA.h"

DWORD C2DA__C2DA_CResRef_i = 0x005CE640;
DWORD C2DA__Load2DArray = 0x005CEFA0;
DWORD C2DA__GetCExoStringEntry = 0x005CE980;

//__declspec( naked ) C2DA::C2DA(CResRef, int){ __asm{ jmp dword ptr [C2DA__C2DA_CResRef_i] }}
__declspec( naked ) int C2DA::Load2DArray(){ __asm{ jmp dword ptr [C2DA__Load2DArray] }}
__declspec( naked ) int C2DA::GetCExoStringEntry(int, CExoString const &, CExoString *){ __asm{ jmp dword ptr [C2DA__GetCExoStringEntry] }}