#pragma once
#include "AsmManageTableSet.h"
#include "AsmInfoFn.h"
class CAsmOpen
{
public:
	CAsmOpen(void);
	~CAsmOpen(void);
	typedef struct asm_mdl_item{
		ProSolid  mdl;
		int       num;
		int       postid;
	}asm_mdl_item;
	long asm_num;
	ProSolid                    asm_solid;                               //����ģ�͵ľ��
	ProSolid                    asm_drw;
	CAsmManageTableSet *        m_pSet;
	CAsmInfoFn                  asm_inf_fn;  //��������������Ϣ
	CString                     localDir;
	CArray<asm_mdl_item,asm_mdl_item>       p_temp_AsmMdlItemList;
	CStringArray                            p_temp_rel;      //���ݿ��еĹ�ϵ��Ȼ���滻��
	BOOL Open(CDatabase * p_db,long  num);
	//��鵱ǰ���㲿�����Ƿ�����ͬ�Ĵ��ڣ�
	//�Լ��Ƿ�ͬ��
	BOOL CheckCurrentMdl(long num);

	BOOL LoadSolid(long num);                                //���ز���ģ�ͣ�����ĳ�͵ı�ţ��õ���׺
	BOOL AddAsmSolidInfo(long num);                          //������Ļ�����Ϣ��ӽ���  
	//BOOL ReplaceRel(CString db_rel,CString * rel);           //�滻ģ�͵Ĺ�ϵʽ��Ȼ����ӵ�ģ����ȥ
/*	BOOL SetSWTX(); */                                         //��ԭ�������Ա�  
	//ProSolid GetComp(int num);
	BOOL AddCompInfo(long num);
	BOOL AddCompPartInfo(ProSolid mdl,long num);             //����������Ϣ
	BOOL AddCompAsmInfo(ProSolid mdl,long num);              //����������Ϣ
	//�õ�����Ĺ���ͼ
	ProError LoadCompDrawing(int ResourceType,CString filename,CString filepath,ProDrawing * comp_dra);
};
