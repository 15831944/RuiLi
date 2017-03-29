#pragma once
#include "DetailInfoClass.h"



// CPrtClass 命令目标

class CPrtClass : public CObject
{
public:
	CPrtClass();
	virtual ~CPrtClass();
	BOOL DownLoadPart(CString SolidName,CString SolidPlace,CString DrwName,CString DrwPlace,CString LocalPlace);
	BOOL OpenPartFile(CString SolidName,CString SolidPlace,CString DrwName,CString DrwPlace,ProSolid *solid);
	BOOL ActivatePart(ProSolid solid);
	BOOL IsInsCodeOk(CString InsCode,CString SolidName,CString DrwName);
	BOOL InsertPart(int s,BOOL NeedTrans);
	BOOL UpdatePart(int s,BOOL NeedTrans);
	BOOL OpenPartFile(DetailInfoClass *detailinfo,int *s,BOOL IsLocal);        //打开零件

	BOOL IsPartFileAlreadyInSession(CString SolidName,CString DrwName);        //判断这个文件是否已经在内存中了

	BOOL IsAlreadyAudit(int SolidID,BOOL *IsAudit);          //判断时候已经审核了

	BOOL GetInfoFromDatabaseBase(int s,CString sql);         //得到零件和部件的详细信息

	BOOL GetDocumentsFromPDM(CString DatabaseDre,DetailInfoClass *info,BOOL *); //根据图号，得到PDM中的文件保存位置，和地址

    BOOL GetDocumentsFromNoPDM(DetailInfoClass *info,BOOL *Find);//根据图号，得到文件保存位置和地址

};


