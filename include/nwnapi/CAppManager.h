#ifndef _CAPPMANAGER_H_
#define _CAPPMANAGER_H_
#include "nwndef.h"
#include "CClientExoApp.h"

class CAppManager
{
public:
  /* 0x0/0 */ CClientExoApp *ClientExoApp;
  /* 0x4/4 */ unsigned long field_4;
  /* 0x8/8 */ unsigned long TileSetManager;
  /* 0xC/12 */ unsigned long ObjectTableManager;
  /* 0x10/16 */ unsigned long ObjectTableManager2;
  /* 0x14/20 */ unsigned long field_14;
  /* 0x18/24 */ unsigned long field_18;
  /* 0x1C/28 */ unsigned long field_1C;
  /* 0x20/32 */ unsigned long field_20;
  /* 0x24/36 */ unsigned long field_24;
  /* 0x28/40 */ unsigned long field_28;
  /* 0x2C/44 */ unsigned long field_2C;
  /* 0x30/48 */ unsigned long field_30;
  /* 0x34/52 */ unsigned long field_34;
  /* 0x38/56 */ unsigned long field_38;
};
#endif
