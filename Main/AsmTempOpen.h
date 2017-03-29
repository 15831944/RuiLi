#pragma once
#include "AsmTempManageTableSet.h"
#include "AsmInfoFn.h"
class CAsmTempOpen
{
public:
	CAsmTempOpen(void);
	~CAsmTempOpen(void);
	typedef struct asm_mdl_item{
		ProSolid  mdl;
		int       num;
		int       postid;
	}asm_mdl_item;
	long asm_num;
	ProSolid asm_solid;                               //����ģ�͵ľ��
	ProDrawing asm_drw;
	CAsmTempManageTableSet * m_pSet;
	CString                  localDir;
	CArray<asm_mdl_item,asm_mdl_item>       p_temp_AsmMdlItemList;
	/*CArray<UsrDrawingItem,UsrDrawingItem>   p_temp_Drwing;*/
	CStringArray                            p_temp_rel;      //���ݿ��еĹ�ϵ��Ȼ���滻��
	CAsmInfoFn                  asm_inf_fn;  //��������������Ϣ
	BOOL Open(CDatabase * p_db,long  num);
	BOOL CheckCurrentMdl(long num);
	BOOL LoadSolid(long num);                                //���ز���ģ�ͣ�����ĳ�͵ı�ţ��õ���׺
	BOOL AddAsmSolidInfo(long num);                          //������Ļ�����Ϣ��ӽ���  
	//BOOL ReplaceRel(CString db_rel,CString * rel);           //�滻ģ�͵Ĺ�ϵʽ��Ȼ����ӵ�ģ����ȥ
	BOOL SetSWTX();                                          //��ԭ�������Ա�  
	BOOL GetParaPicDrwInfo(long asm_num);
	BOOL AddCompPartInfo(ProSolid mdl,long num);             //����������Ϣ
	/*ProSolid GetComp(int num);*/
	BOOL AddCompInfo(long num);
	BOOL AddCompAsmInfo(ProSolid mdl,long num);
	//�õ�����Ĺ���ͼ
	ProError LoadCompDrawing(int ResourceType,CString filename,CString filepath,ProDrawing * comp_dra);
};
