#include "CExoString.h"

CExoString::CExoString(char *str)
{
	int length = strlen(str)+1;
	this->Text = new char[length];
	this->Length = length;
	strncpy(this->Text, str, length);
}