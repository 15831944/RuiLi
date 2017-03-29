#pragma once
#include "DetailInfoClass.h"



// CPrtClass ����Ŀ��

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
	BOOL OpenPartFile(DetailInfoClass *detailinfo,int *s,BOOL IsLocal);        //�����

	BOOL IsPartFileAlreadyInSession(CString SolidName,CString DrwName);        //�ж�����ļ��Ƿ��Ѿ����ڴ�����

	BOOL IsAlreadyAudit(int SolidID,BOOL *IsAudit);          //�ж�ʱ���Ѿ������

	BOOL GetInfoFromDatabaseBase(int s,CString sql);         //�õ�����Ͳ�������ϸ��Ϣ

	BOOL GetDocumentsFromPDM(CString DatabaseDre,DetailInfoClass *info,BOOL *); //����ͼ�ţ��õ�PDM�е��ļ�����λ�ã��͵�ַ

    BOOL GetDocumentsFromNoPDM(DetailInfoClass *info,BOOL *Find);//����ͼ�ţ��õ��ļ�����λ�ú͵�ַ

};


