#pragma once
#include "DetailInfoClass.h"
#include "ListCtrlEx.h"



// CProductClass 命令目标

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
	BOOL AffirmIsOld(CString Type,int s,BOOL New);                  //用来确定零件是否是原来有的

	BOOL UpdateProductMenber(ProSolid ProductSolid,int s);		  //用于产品的零部件参数已经修改，需要改变装配表，如BOM等等；
	//BOOL UpdateProductMenberToDatabase(int s,CString Type);		  //用于在产品的零部件参数没有修改，但是需要修改状态什么的
	BOOL WriteProductMenberToDatabase(ProSolid solid,int Pos,BOOL IsLocal); 
	BOOL UpdateTheAsmTheContainTheCurrent(int Pos,ProSolid solid);		  //替换

	BOOL UpdateAsmVariantAndPerf(int s);
	BOOL UpdateNewAsm(int s,BOOL NeedTrans);

	BOOL ShowPerf(int s,CString Type,CListCtrlEx *list);				 //显示性能参数
	BOOL ListPartWhichIsNotAudit(CString Type,int s,int *i,CListCtrl *m_list);		  //显示没有审核过的


	BOOL Rename(ProSolid solid,CString SolidName,CString DrwName);
	BOOL NewRename(ProSolid solid,CString NewSolidName,CString *NewDrwName);
	BOOL RenameAddChar(ProSolid solid,int s,CString Type);

	BOOL IsProductMenberFileAlreadyExist(CString LocalPlace);			 //判断要下载文件是否在本地存在
	BOOL IsProductMenberFileAlreadyInSession();							 //判断产品的文件是否已经存在内存中存在


	BOOL FindProductMenberForProductWhichIsModify(ProSolid productsolid,BOOL New);
	BOOL InsertProductMenberForProduct(ProSolid productsolid);
	BOOL InsertAsmMenberForProduct(ProSolid productsolid);

	BOOL IsAlreadyAudit(int SolidID,BOOL *IsAudit);

	BOOL IsUsedByProduct(int s,BOOL *BeUsed,CString Type);
	BOOL IsUsedByTheSameProduct(CStringArray *PdtArray,BOOL *Yes);

	BOOL  DeleteOldVersion(ProSolid Solid,BOOL SaveSuccess,int SolidVersion,int DrwVersion);
};


