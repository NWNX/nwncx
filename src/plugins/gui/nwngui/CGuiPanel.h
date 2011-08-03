#ifndef CGuiPanel_h_
#define CGuiPanel_h_
#include "NWNCommon.h"
#include "CGuiObject.h"

class CGuiPanel : public CGuiObject
{
public:
	CGuiPanel();
	virtual ~CGuiPanel(){};

	/*
	00000000 GuiObject CGuiObject ?
	00000020 field_20 dd ?
	00000024 field_24 dd ?
	00000028 field_28 dd ?
	0000002C field_2C dd ?
	00000030 field_30 dd ?
	00000034 field_34 dd ?
	00000038 field_38 dd ?
	0000003C AutoDeleteOnClose dd ?
	00000040 field_40 dd ?
	00000044 field_44 dd ?
	00000048 field_48 db ?
	00000049 LayoutFile CResRef ?
	00000059 db ? ; undefined
	0000005A db ? ; undefined
	0000005B db ? ; undefined
	0000005C LayoutFileGFF dd ?
	00000060 field_60 dd ?
	00000064 NumTags dd ?
	*/

	virtual void VFunc31(){};
	virtual void VFunc32(int){};
	virtual float VFunc33(){return 0.0;};
	virtual float VFunc34(){return 0.0;};
	virtual void Reposition(){};
	virtual int VFunc36(){return 1;};
	virtual int VFunc37(){return 1;};
	virtual void VFunc38(){};
	virtual void* VFunc39(){return NULL;};

	void Activate();
	void Deactivate(int);
	void SetLayoutFile(CResRef sFileName);
	void SetAutoDeleteOnClose();
	void SetPositionScreenCenter();

	DWORD field_20, field_24, field_28, field_2C, field_30,
		field_34, field_38, AutoDeleteOnClose, field_40, field_44;
	BYTE field_48;
	CResRef LayoutFile;
	void *LayoutFileGFF;
	DWORD field_60;
	DWORD NumTags;
};


#endif // CGuiPanel_h_