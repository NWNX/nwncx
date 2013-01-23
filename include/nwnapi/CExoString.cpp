#include "CExoString.h"

DWORD CExoString__operator_as_c = 0x005BA520;
DWORD CExoString__operator_as_CExoString = 0x005BA450;
DWORD CExoString__operator_pl = 0x005BAA10;
DWORD CExoString__CExoString = 0x004F85A0;
DWORD CExoString__CExoString_c = 0x005BA260;
DWORD CExoString___CExoString = 0x005BA420;

__declspec( naked ) CExoString::CExoString(){ __asm{ jmp dword ptr [CExoString__CExoString] }}
__declspec( naked ) CExoString::CExoString(char *str){ __asm{ jmp dword ptr [CExoString__CExoString_c] }}
__declspec( naked ) CExoString::~CExoString(){ __asm{ jmp dword ptr [CExoString___CExoString] }}

 __declspec( naked ) CExoString & CExoString::operator=(char const *){ __asm{ jmp dword ptr [CExoString__operator_as_c] }}
 __declspec( naked ) CExoString & CExoString::operator=(CExoString const &){ __asm{ jmp dword ptr [CExoString__operator_as_CExoString] }}
 __declspec( naked ) CExoString CExoString::operator+(CExoString const &) const{ __asm{ jmp dword ptr [CExoString__operator_pl] }}