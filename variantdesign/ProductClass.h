#pragma once
#include "DetailInfoClass.h"
#include "ListCtrlEx.h"



// CProductClass ����Ŀ��

class CProductClass : public CObject
{
public:
	CProductClass();
	virtual ~CProductClass();
	BOOL IsInsCodeOk(CString InsCode,CString SolidName,CString DrwName);

	BOOL OpenProductFile(CString InsCode);
	BOOL NewProductFile(DetailInfoClass *info,int *s,BOOL IsModel);
	BOOL GetSavePlaceInfo(CString AsmNum,CString LocalPlace,BOOL IsModel,BOOL IsNew,ProSolid *solid);

	BOOL GetAllPartInfoForAsm(ProSolid solid,BOOL IsNew);
	BOOL GetInfoFromDatabaseBase(CString sql,int Num,BOOL IsPart,BOOL IsNew);

	BOOL InsertAsmDatabase(int s);
	BOOL InsertAsmVariantAndPerf(int s,int PdtNum);
	BOOL InsertNewAsm(int s,BOOL NeedTrans);
	BOOL WriteProductToDatabase(ProSolid solid,BOOL NeedTrans,BOOL IsLocal);
	BOOL WriteAsmToDatabase(ProSolid solid,BOOL NeedTrans,BOOL IsLocal);
	BOOL SaveBothPartAndAsm(int s,CString Type,BOOL New);
	BOOL WriteAsmExtraToDatabase(ProSolid solid);
	BOOL UpdateThreeTable(ProSolid solid,int Pos);
	//BOOL ProductMenberUpdateThreeTable(ProSolid solid,int Pos);
	BOOL InsertThreeTable(ProSolid solid,int Pos);

	BOOL ActivateAsm(ProSolid solid);           
	BOOL IsPartModify(ProSolid solid,CString MdlNum,int SolidID);        
	BOOL IsAsmModify(ProSolid solid,CString MdlNum,int SolidID);
	BOOL AffirmIsOld(CString Type,int s,BOOL New);                  //����ȷ������Ƿ���ԭ���е�

	BOOL UpdateProductMenber(ProSolid ProductSolid,int s);		  //���ڲ�Ʒ���㲿�������Ѿ��޸ģ���Ҫ�ı�װ�����BOM�ȵȣ�
	//BOOL UpdateProductMenberToDatabase(int s,CString Type);		  //�����ڲ�Ʒ���㲿������û���޸ģ�������Ҫ�޸�״̬ʲô��
	BOOL WriteProductMenberToDatabase(ProSolid solid,int Pos,BOOL IsLocal); 
	BOOL UpdateTheAsmTheContainTheCurrent(int Pos,ProSolid solid);		  //�滻

	BOOL UpdateAsmVariantAndPerf(int s);
	BOOL UpdateNewAsm(int s,BOOL NeedTrans);

	BOOL ShowPerf(int s,CString Type,CListCtrlEx *list);				 //��ʾ���ܲ���
	BOOL ListPartWhichIsNotAudit(CString Type,int s,int *i,CListCtrl *m_list);		  //��ʾû����˹���


	BOOL Rename(ProSolid solid,CString SolidName,CString DrwName);
	BOOL NewRename(ProSolid solid,CString NewSolidName,CString *NewDrwName);
	BOOL RenameAddChar(ProSolid solid,int s,CString Type);

	BOOL IsProductMenberFileAlreadyExist(CString LocalPlace);			 //�ж�Ҫ�����ļ��Ƿ��ڱ��ش���
	BOOL IsProductMenberFileAlreadyInSession();							 //�жϲ�Ʒ���ļ��Ƿ��Ѿ������ڴ��д���


	BOOL FindProductMenberForProductWhichIsModify(ProSolid productsolid,BOOL New);
	BOOL InsertProductMenberForProduct(ProSolid productsolid);
	BOOL InsertAsmMenberForProduct(ProSolid productsolid);

	BOOL IsAlreadyAudit(int SolidID,BOOL *IsAudit);

	BOOL IsUsedByProduct(int s,BOOL *BeUsed,CString Type);
	BOOL IsUsedByTheSameProduct(CStringArray *PdtArray,BOOL *Yes);

	BOOL  DeleteOldVersion(ProSolid Solid,BOOL SaveSuccess,int SolidVersion,int DrwVersion);
};


