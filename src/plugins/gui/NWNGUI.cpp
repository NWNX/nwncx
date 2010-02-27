#include "NWNGUI.h"
/*
void init(){
	int (CGuiObject::*func)() = reinterpret_cast<int (__thiscall CGuiObject::* )(void)> (0x005E4D70);
}*/

CGuiObject::CGuiObject(){
	__asm{
		mov ebx, 0x005E4D40
		call ebx
	}
}

/*__declspec( naked ) CGuiObject::~CGuiObject(){__asm{ jmp cs:0x005E4D70 }}*/

void init()
{
	CGuiObject *gui = new CGuiObject();
	delete gui;
}