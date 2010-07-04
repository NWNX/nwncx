#include <windows.h>

class CResRef
{
public:
	char ResRef[16];

	CResRef();
	CResRef(char *);
};

class CGuiObject
{
public:
	CGuiObject();
	virtual ~CGuiObject(){};

	/*
	offset ?HandleOkButton@CPanelLoadSave@@MAEXXZ ; CPanelLoadSave::HandleOkButton(void)
	offset ?SetId@CGameObject@@UAEXK@Z ; CGameObject::SetId(ulong)
	offset ?SetId@CGameObject@@UAEXK@Z ; CGameObject::SetId(ulong)
	offset ?GetGuiObjType@CGuiObject@@UAEHXZ ; CGuiObject::GetGuiObjType(void)
	offset ?AsNWSObject@CGameObject@@UAEPAVCNWSObject@@XZ ; CGameObject::AsNWSObject(void)
	offset ?AsNWSObject@CGameObject@@UAEPAVCNWSObject@@XZ ; CGameObject::AsNWSObject(void)
	offset ?AsNWSObject@CGameObject@@UAEPAVCNWSObject@@XZ ; CGameObject::AsNWSObject(void)
	offset ?AsCGuiExamineGeneric@CGuiExamineGeneric@@UAEPAV1@XZ ; CGuiExamineGeneric::AsCGuiExamineGeneric(void)
	offset ?LoadGffData@CGuiObject@@UAEXPAVCResGFF@@PAVCGuiTag@@@Z ; CGuiObject::LoadGffData(CResGFF *,CGuiTag *)
	offset ?Update@CGuiObject@@MAEHM@Z ; CGuiObject::Update(float)
	offset ??1CNWSCombatRoundAction@@QAE@XZ ; CNWSCombatRoundAction::~CNWSCombatRoundAction(void)

	offset ?SetId@CGameObject@@UAEXK@Z ; CGameObject::SetId(ulong)
	offset ?HandleOkButton@CPanelLoadSave@@MAEXXZ ; CPanelLoadSave::HandleOkButton(void)
	offset ?HandleUninitializedCallback@?$CGuiCallbackr1@HPAVCTreeviewNode@@@@QAEHPAVCTreeviewNode@@@Z ; CGuiCallbackr1<int,CTreeviewNode *>::HandleUninitializedCallback(CTreeviewNode *)
	offset ?GetIsIDInExcludeList@CNWArea@@UAEHKPAV?$CExoArrayList@PAVCGameObject@@@@@Z ; CNWArea::GetIsIDInExcludeList(ulong,CExoArrayList<CGameObject *> *)
	offset ?LoadGffData@CGuiObject@@UAEXPAVCResGFF@@PAVCGuiTag@@@Z ; CGuiObject::LoadGffData(CResGFF *,CGuiTag *)
	offset ?HandleOkButton@CPanelLoadSave@@MAEXXZ ; CPanelLoadSave::HandleOkButton(void)
	offset ?HandleOkButton@CPanelLoadSave@@MAEXXZ ; CPanelLoadSave::HandleOkButton(void)
	offset ?HandleOkButton@CPanelLoadSave@@MAEXXZ ; CPanelLoadSave::HandleOkButton(void)
	offset ?HandleOkButton@CPanelLoadSave@@MAEXXZ ; CPanelLoadSave::HandleOkButton(void)
	offset ?HandleOkButton@CPanelLoadSave@@MAEXXZ ; CPanelLoadSave::HandleOkButton(void)
	offset ?HandleOkButton@CPanelLoadSave@@MAEXXZ ; CPanelLoadSave::HandleOkButton(void)
	offset ?HandleOkButton@CPanelLoadSave@@MAEXXZ ; CPanelLoadSave::HandleOkButton(void)
	offset ?HandleUninitializedCallback@?$CGuiCallbackr1@HPAVCTreeviewNode@@@@QAEHPAVCTreeviewNode@@@Z ; CGuiCallbackr1<int,CTreeviewNode *>::HandleUninitializedCallback(CTreeviewNode *)
	offset ?HandleTriggerTooltip@CGuiObject@@MAEXXZ ; CGuiObject::HandleTriggerTooltip(void)
	offset ?HandleFinishTooltip@CGuiObject@@MAEXXZ ; CGuiObject::HandleFinishTooltip(void)
	offset ?SetId@CGameObject@@UAEXK@Z ; CGameObject::SetId(ulong)
	offset ?HandleDragDropAsDest@CGuiObject@@MAEHHPAVCGuiDragSource@@PAVCHitInfo@@@Z ; CGuiObject::HandleDragDropAsDest(int,CGuiDragSource *,CHitInfo *)
	offset ?AsNWSObject@CGameObject@@UAEPAVCNWSObject@@XZ ; CGameObject::AsNWSObject(void)
	offset ?AsNWSObject@CGameObject@@UAEPAVCNWSObject@@XZ ; CGameObject::AsNWSObject(void)
	offset ?HandleOkButton@CPanelLoadSave@@MAEXXZ ; CPanelLoadSave::HandleOkButton(void)
	offset ?SetId@CGameObject@@UAEXK@Z ; CGameObject::SetId(ulong)
	dd offset ?GetPanelHeight@CGuiPanel@@UAEMXZ ; CGuiPanel::GetPanelHeight(void)
	dd offset ?GetPanelHeight@CGuiPanel@@UAEMXZ ; CGuiPanel::GetPanelHeight(void)
	dd offset ?HandleOkButton@CPanelLoadSave@@MAEXXZ ; CPanelLoadSave::HandleOkButton(void)
	dd offset ?OnResourceServiced@CRes@@UAEHXZ ; CRes::OnResourceServiced(void)
	dd offset ?OnResourceServiced@CRes@@UAEHXZ ; CRes::OnResourceServiced(void)
	dd offset ?HandleOkButton@CPanelLoadSave@@MAEXXZ ; CPanelLoadSave::HandleOkButton(void)
	dd offset ?AsNWSObject@CGameObject@@UAEPAVCNWSObject@@XZ ; CGameObject::AsNWSObject(void)
	*/

	virtual void VFunc1(){};
	virtual void VFunc2(int a1){};
	virtual void VFunc3(int a1){};
	virtual int GetGuiObjType(){return NULL;};
	virtual CGuiObject *As1(){return NULL;};
	virtual CGuiObject *As2(){return NULL;};
	virtual CGuiObject *As3(){return NULL;};
	virtual CGuiObject *AsCGuiExamineGeneric(){return NULL;};
	virtual void LoadGffData(int a1, int a2){};
	virtual int Update(float a1){return NULL;};
	virtual void PostAttachmentInitialize(){};
	virtual void VFunc4(int a1){};
	virtual void HandleLoseModality(){};
	virtual int VFunc5(int a1){return NULL;};
	virtual int HandleKeyEvent(DWORD a1, DWORD a2){return NULL;};
	virtual void VFunc6(int a1, int a2){};
	virtual void VFunc7(){};
	virtual void VFunc8(){};
	virtual void VFunc9(){};
	virtual void VFunc10(){};
	virtual void VFunc11(){};
	virtual void VFunc12(){};
	virtual void VFunc13(){};
	virtual int VFunc14(int a1){return NULL;};
	virtual void HandleTriggerTooltip(){};
	virtual void HandleFinishTooltip(){};
	virtual void VFunc15(int a1){};
	virtual int HandleDragDropAsDest(int, int, int){return NULL;};
	virtual void* VFunc16(){return NULL;};
	virtual void* VFunc17(){return NULL;};
	/*virtual void OnActivate();
	virtual void OnDeactivate();
	virtual float GetFloat1();
	virtual float GetFloat2();*/

	void SetEnabled(int a1);

	/*
	00000000 vftable         dd ?
	00000004 field_4         dd ?
	00000008 AurObject       dd ?
	0000000C field_C         dd ?
	00000010 field_10        dd ?
	00000014 field_14        dd ?
	00000018 field_18        dd ?
*/

	DWORD field_4;
	void *AurObject;
	DWORD field_C, field_10, field_14, field_18, field_1C;
};

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

	void SetLayoutFile(CResRef sFileName);
	void SetAutoDeleteOnClose();


	DWORD field_20, field_24, field_28, field_2C, field_30,
		field_34, field_38, AutoDeleteOnClose, field_40, field_44;
	BYTE field_48;
	CResRef LayoutFile;
	void *LayoutFileGFF;
	DWORD field_60;
	DWORD NumTags;
};

class CGuiModalPanel : public CGuiPanel
{
public:
	//CGuiModalPanel(int modal);
	virtual ~CGuiModalPanel(){};

	void Activate();

	DWORD bModal;
};

class CMyPanel : public CGuiModalPanel
{
public:
	CMyPanel();
};

void init();