#ifndef Gob_h_
#define Gob_h_

#include <windows.h>
#include "CExoString.h"
#include "CResRef.h"

struct Vector
{
  /* 0x0/0 */ float x;
  /* 0x4/4 */ float y;
  /* 0x8/8 */ float z;
};

class Gob
{
	public:
	char *GetName();
	char *GetModelName();
	void *GetPart(const char *);


  /* 0x0/0 */ void *vtable;
  /* 0x4/4 */ unsigned long field_4;
  /* 0x8/8 */ unsigned long field_8;
  /* 0xC/12 */ unsigned long field_C;
  /* 0x10/16 */ char field_10;
  /* 0x11/17 */ char field_11;
  /* 0x12/18 */ char ForceAnimations;
  /* 0x14/20 */ char rsvd1;
  /* 0x14/20 */ char field_14;
  /* 0x15/21 */ char AnimationsEnabled;
  /* 0x18/24 */ char rsvd2[2];
  /* 0x18/24 */ unsigned long field_18;
  /* 0x1C/28 */ unsigned long field_1C;
  /* 0x20/32 */ unsigned long field_20;
  /* 0x24/36 */ char Name[16];
  /* 0x34/52 */ unsigned long field_34;
  /* 0x38/56 */ unsigned long field_38;
  /* 0x3C/60 */ unsigned long field_3C;
  /* 0x40/64 */ unsigned long field_40;
  /* 0x44/68 */ unsigned long field_44;
  /* 0x48/72 */ unsigned long field_48;
  /* 0x4C/76 */ unsigned long field_4C;
  /* 0x50/80 */ unsigned long field_50;
  /* 0x54/84 */ unsigned long field_54;
  /* 0x58/88 */ unsigned long field_58;
  /* 0x5C/92 */ unsigned long field_5C;
  /* 0x60/96 */ unsigned long field_60;
  /* 0x64/100 */ unsigned long Model;
  /* 0x68/104 */ unsigned long field_68;
  /* 0x6C/108 */ unsigned long field_6C;
  /* 0x70/112 */ unsigned long field_70;
  /* 0x74/116 */ void *Scene;
  /* 0x78/120 */ void *RootPart;
  /* 0x7C/124 */ unsigned long field_7C;
  /* 0x80/128 */ unsigned long field_80;
  /* 0x84/132 */ Vector Position;
  /* 0x90/144 */ Vector Orientation;
  /* 0x9C/156 */ unsigned long field_9C;
  /* 0xA0/160 */ unsigned long field_A0;
  /* 0xA4/164 */ unsigned long field_A4;
  /* 0xA8/168 */ unsigned long field_A8;
  /* 0xAC/172 */ unsigned long field_AC;
  /* 0xB0/176 */ unsigned long field_B0;
  /* 0xB4/180 */ unsigned long field_B4;
  /* 0xB8/184 */ unsigned long field_B8;
  /* 0xBC/188 */ unsigned long field_BC;
  /* 0xC0/192 */ unsigned long field_C0;
  /* 0xC4/196 */ unsigned long field_C4;
  /* 0xC8/200 */ unsigned long field_C8;
  /* 0xCC/204 */ unsigned long field_CC;
  /* 0xD0/208 */ unsigned long field_D0;
  /* 0xD4/212 */ unsigned long field_D4;
  /* 0xD8/216 */ unsigned long field_D8;
  /* 0xDC/220 */ unsigned long field_DC;
  /* 0xE0/224 */ unsigned long field_E0;
  /* 0xE4/228 */ unsigned long field_E4;
  /* 0xE8/232 */ unsigned long field_E8;
  /* 0xEC/236 */ unsigned long field_EC;
  /* 0xF0/240 */ unsigned long field_F0;
  /* 0xF4/244 */ unsigned long field_F4;
  /* 0xF8/248 */ unsigned long field_F8;
  /* 0xFC/252 */ unsigned long field_FC;
  /* 0x100/256 */ unsigned long field_100;
  /* 0x104/260 */ unsigned long field_104;
  /* 0x108/264 */ unsigned long field_108;
  /* 0x10C/268 */ unsigned long field_10C;
  /* 0x110/272 */ unsigned long field_110;
  /* 0x114/276 */ unsigned long field_114;
  /* 0x118/280 */ unsigned long PathInfo;
  /* 0x11C/284 */ unsigned long field_11C;
  /* 0x120/288 */ unsigned long field_120;
  /* 0x124/292 */ unsigned long field_124;
  /* 0x128/296 */ unsigned long field_128;
  /* 0x12C/300 */ unsigned long field_12C;
  /* 0x130/304 */ unsigned long field_130;
  /* 0x134/308 */ unsigned long field_134;
  /* 0x138/312 */ unsigned long field_138;
  /* 0x13C/316 */ unsigned long field_13C;
  /* 0x140/320 */ unsigned long field_140;
  /* 0x144/324 */ unsigned long field_144;
  /* 0x148/328 */ unsigned long field_148;
  /* 0x14C/332 */ unsigned long field_14C;
  /* 0x150/336 */ unsigned long field_150;
  /* 0x154/340 */ unsigned long field_154;
  /* 0x158/344 */ unsigned long field_158;
  /* 0x15C/348 */ char field_15C;
  /* 0x160/352 */ char rsvd3[3];
  /* 0x160/352 */ unsigned long field_160;
  /* 0x164/356 */ unsigned long field_164;
  /* 0x168/360 */ unsigned long field_168;
  /* 0x16C/364 */ unsigned long field_16C;
  /* 0x170/368 */ unsigned long field_170;
  /* 0x174/372 */ unsigned long field_174;
  /* 0x178/376 */ unsigned long field_178;
  /* 0x17C/380 */ unsigned long field_17C;
  /* 0x180/384 */ unsigned long field_180;
  /* 0x184/388 */ unsigned long field_184;
  /* 0x188/392 */ unsigned long field_188;
  /* 0x18C/396 */ unsigned long field_18C;
  /* 0x190/400 */ unsigned long field_190;
  /* 0x194/404 */ unsigned long field_194;
  /* 0x198/408 */ unsigned long field_198;
  /* 0x19C/412 */ unsigned long field_19C;
  /* 0x1A0/416 */ unsigned long ExternalData;
  /* 0x1A4/420 */ unsigned long ExternalDataType;
  /* 0x1A8/424 */ unsigned long field_1A8;
  /* 0x1AC/428 */ unsigned long field_1AC;
  /* 0x1B0/432 */ unsigned long field_1B0;
  /* 0x1B4/436 */ unsigned long field_1B4;
  /* 0x1B8/440 */ unsigned long field_1B8;
  /* 0x1BC/444 */ unsigned long field_1BC;
};

#endif