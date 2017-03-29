#pragma once
#include "AsmTempManageTableSet.h"
class CAsmTempSave
{
public:
	CAsmTempSave(void);
	~CAsmTempSave(void);
	void MarkComp(ProSolid comp, int upLevel);
	typedef struct asm_comp_item {
		ProSolid mdl;       //Ԫ�����
		int      num;       //Ԫ���ı��
		int      upLevel;   //�ϲ���
		int      postId;    //��׺
	}asm_comp_item,*AsmCompItemList;

	//��سߴ硢������
	typedef struct Asm_Para_item {
		int      solid_num;     //����ģ�͵ı�ţ���Ϊ��ӵ����ݿ��еĺ�׺
		CString  type;          //"para" "dim"
		CString  id;            //"����/���� �� ģ�ͺ�"
		CString  DimSymbol;     //�ߴ����
		CString  Name;          //�ߴ���ţ��������� �к�׺
		CString  featName;          //��������Ч
		CString  SubType;       //����
		CString  Value;         //�������ߴ��ֵ  ��ֵ���ܹ�Ϊ�գ�Ҫ�ж�
		CString  UpLimit;
		CString  DwLimit;
		CString  Note;
	}Asm_Para_item,*AsmParaItemList;
	CArray<AsmRelItem,AsmRelItem> tempRelList;               //�滻��׺֮��Ĺ�ϵ
	CArray<asm_comp_item,asm_comp_item> p_AsmCompItem;       //ģ�ͱ�����׺��Ӧ��ϵ
	CArray<Asm_Para_item,Asm_Para_item> p_AsmRelRvItem;      //ģ�͵���سߴ���
	CArray<Asm_Para_item,Asm_Para_item> P_temp_AsmSmlItem;   //ģ�͵��������Ա���
	CAsmTempManageTableSet * m_pSet;
	int                asm_num;                              //��ӵ����ݿ��У�װ����ı�ţ���ʼֵΪ-1
	long               asm_father_num;
	CString            fileDirectory;                        //ģ���ڷ������еĴ��λ��
	CString            fileLodcalDir;                        //ģ���ڱ��صĴ��λ��
	ProSolid   asm_solid;                                      //װ����ľ��,��Ҫ�ڵ����Ի���֮ǰ��ֵ
	//BOOL       ReplaceRelPostIdWithMdlNum(void);
	//BOOL       GetRelRvItem(void);
	void       GetSWTXCStringArray(CStringArray * cs_swtx_item);
	int        GetSolidNum(ProSolid solid);
	BOOL       GetSWTXItem(void);
	BOOL InsertAsmPropInfo(void);
	BOOL InsertAsmMakeupInfo(void);
	BOOL InsertSWTX(void);
	//BOOL InsertRelInfo(void);
	BOOL InsertPerfInfo(void);
	BOOL InsertAsmPicInfo(void);
	BOOL CopyLocalDirToServer(void);
	BOOL InsertDrwInfo(void);
	BOOL Save(ProSolid solid,long fatherNum,CDatabase * p_db);
};
