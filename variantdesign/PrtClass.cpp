// PrtClass.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PrtClass.h"
#include "CommonFuncClass.h"
#include "FtpFile.h"
#include "ProSolid.h"
#include "ProWindows.h"
#include "afxtempl.h"
#include "MyDatabase.h"
#include "ProductClass.h"

void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
extern CArray <Database,Database> Database_struc_temp;
extern CArray <FilePath,FilePath> FilePath_struc;
extern CMyDatabase m_db;
extern FtpInfo Ftp_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;
// CPrtClass

CPrtClass::CPrtClass()
{
}

CPrtClass::~CPrtClass()
{
}


// CPrtClass ��Ա����
/************************************************************************/
/* �������                                                             */
/************************************************************************/
BOOL CPrtClass::DownLoadPart(CString SolidName,CString SolidPlace,CString DrwName,CString DrwPlace,CString LocalPlace)
{
	CCommonFuncClass commonfun;
	CFtpFile ftp;
	CString Name,SolidPath,DrwPath,temppath;
	Name=SolidName;
	Name.MakeLower();
	//���������ж��ڴ����Ƿ��и�ģ��
	//���������Ƿ�����ͬ�����
	if (!IsPartFileAlreadyInSession(SolidName,DrwName))
	{
		AfxMessageBox("���ڴ����Ѿ���ͬ���ļ����ڣ�������ڴ棬�ٴ�������");
		return FALSE;
	}
	//�ж�Ҫ���صģ�����Ŀ¼+ģ�͵����֣��������Ƿ����
	if (!commonfun.IsFileExist(SolidPlace,DrwPlace,SolidName,DrwName))
	{
		return FALSE;
	}
	//������άģ��
	if (SolidName.Right(4)==".prt")
	{
		temppath=SolidPlace+"/"+SolidName+".1";
	}
	else
		temppath=SolidPlace+"/"+SolidName;
	temppath.Replace(" ","#");
	if(!ftp.DownloadFiletoDirectory(temppath,LocalPlace,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
	{
		//AfxMessageBox("�޷�������άģ�ͣ�");
		return FALSE;
	}
	//���ض�άģ��
	if (DrwName!="")
	{
		if (DrwName.Right(4)==".drw")
		{
			temppath=DrwPlace+"/"+DrwName+".1";
		}
		else
			temppath=DrwPlace+"/"+DrwName;
		if(!ftp.DownloadFiletoDirectory(temppath,LocalPlace,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
		{
			AfxMessageBox("�޷����ض�άģ�ͣ�");
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* ������ļ�                                                         */
/************************************************************************/
BOOL CPrtClass::OpenPartFile(CString SolidName,CString SolidPlace,CString DrwName,CString DrwPlace,ProSolid *solid)
{
	CCommonFuncClass commonfun;
	ProPath path;
	ProError status;
	ProMdl DrwSolid;
	int k;
	CString SolidPath,DrwPath;
	if (SolidName.Right(4)!=".prt")
	{
         k=SolidName.ReverseFind('.');
		 if (k!=-1)
		 {
			 SolidName.Delete(k,(SolidName.GetLength()-k));
		 }	 
	}
	SolidPath=SolidPlace+"\\"+SolidName;
	ProStringToWstring(path,SolidPath.GetBuffer());
	status=ProMdlLoad(path,PRO_MDL_PART,PRO_B_FALSE,(ProMdl *)solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("û���ҵ���άģ��"); 
		return FALSE;
	}
	if (DrwName!="")
	{
		if (DrwName.Right(4)=="drw")
		{
			k=DrwName.ReverseFind('.');
			if (k!=-1)
			{
				DrwName.Delete(k,(DrwName.GetLength()-k));
			}
		}
		DrwPath=DrwPlace+"\\"+DrwName;
		ProStringToWstring(path,DrwPath.GetBuffer());
		status=ProMdlLoad(path,PRO_MDL_DRAWING,PRO_B_FALSE,&DrwSolid);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("û���ҵ���άģ��");
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* �����                                                             */
/************************************************************************/
BOOL CPrtClass::ActivatePart(ProSolid solid)
{
	ProError status;
	ProType type;
	ProName w_name;
	int w_id;
	status = ProMdlTypeGet(solid, (ProMdlType*)&type);
	status= ProMdlNameGet(solid, w_name);
	status= ProObjectwindowCreate(w_name, type, &w_id);
	if(!status== PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	ProWindowRepaint(PRO_VALUE_UNUSED);
	status = ProMdlDisplay(solid);
	status = ProWindowActivate(w_id);
	return TRUE;
}
/************************************************************************/
/* �ж����ͼ�ţ��ļ�����Ψһ��                                         */
/************************************************************************/
BOOL  CPrtClass::IsInsCodeOk(CString InsCode,CString SolidName,CString DrwName)
{
	CCommonFuncClass commonfun;
	CString sql,str;
	//�ж�ͼ�� ���ļ�����Ψһ��
	sql.Format("SELECT * FROM NPartInsInfoTable where InsCode='%s'",InsCode);
	if (commonfun.IsHave(sql))
	{
		AfxMessageBox("���ͼ���Ѿ����ڣ����޸�");
		return FALSE;
	}
	sql.Format("SELECT * FROM NProductInsInfoTable where InsCode='%s'",InsCode);
	if (commonfun.IsHave(sql))
	{
		AfxMessageBox("���ͼ���Ѿ����ڣ����޸�");
		return FALSE;
	}
	sql.Format("SELECT * FROM NPartInsInfoTable where SolidFileName='%s'",SolidName);
	if (commonfun.IsHave(sql))
	{
		str.Format("�ļ�%s�Ѿ����ڣ����޸�",SolidName);
		AfxMessageBox(str);
		return FALSE;
	}
	sql.Format("SELECT * FROM NPartInsInfoTable where DrawingName='%s'",DrwName);
	if (commonfun.IsHave(sql))
	{
		str.Format("�ļ�%s�Ѿ����ڣ����޸�",SolidName);
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}	 
BOOL CPrtClass::InsertPart(int s,BOOL NeedTrans)
{
	_variant_t vNULL;
	vNULL.vt = VT_ERROR;
	vNULL.scode = DISP_E_PARAMNOTFOUND;
	CCommonFuncClass commonfun;
	try
	{
		m_db.m_pConnection->Close();
		m_db.ReOpen();
		m_db.m_pConnection->BeginTrans();
		if (!commonfun.InsertNewPart(s,FALSE))
		{
			m_db.m_pConnection->RollbackTrans();
			return FALSE;
		}
		else
		{
			m_db.m_pConnection->CommitTrans();
			return TRUE;
		}
	}
	catch (_com_error e) 
	{
		m_db.m_pConnection->RollbackTrans();
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* �������                                                             */
/************************************************************************/
BOOL CPrtClass::UpdatePart(int s,BOOL NeedTrans)
{
	_variant_t vNULL;
	vNULL.vt = VT_ERROR;
	vNULL.scode = DISP_E_PARAMNOTFOUND;
	CCommonFuncClass commonfun;
	try
	{
		m_db.m_pConnection->Close();
		m_db.ReOpen();
		m_db.m_pConnection->BeginTrans();
		if (!commonfun.UpdateNewPart(s,NeedTrans))
		{
			m_db.m_pConnection->RollbackTrans();
			return FALSE;
		}
		else
		{
			m_db.m_pConnection->CommitTrans();
			return TRUE;
		}
	}
	catch (_com_error e) 
	{
		m_db.m_pConnection->RollbackTrans();
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* ������ļ�                                                         */
/************************************************************************/
BOOL CPrtClass::OpenPartFile(DetailInfoClass *detailinfo,int *s,BOOL IsLocal)
{
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	ProSolid solid;
		//ֱ�Ӵ�
	if (IsLocal)
	{
		if (!OpenPartFile(detailinfo->OldSolidFileName,detailinfo->OldSolidFilePath,detailinfo->OldDrwName,detailinfo->OldDrwFilePath,&solid))
		{
			AfxMessageBox("�����ʧ��");
			return FALSE;
		}
		else
		{
			detailinfo->solid=solid;
			//��Ҫ����
			if (!pdtclass.NewRename(solid,detailinfo->SolidFileName,&(detailinfo->DrwName)))
			{
				AfxMessageBox("������ʧ�ܣ�");
				return FALSE;
			}
		}
	}
	else
	{
		//Ҫ����
		if (!DownLoadPart(detailinfo->OldSolidFileName,detailinfo->OldSolidFilePath,detailinfo->OldDrwName,detailinfo->OldDrwFilePath,detailinfo->SolidFilePath))
		{
			AfxMessageBox("�޷����ظ����");
			return FALSE;
		}
		else
		{
			if (!OpenPartFile(detailinfo->OldSolidFileName,detailinfo->SolidFilePath,detailinfo->OldDrwName,detailinfo->DrwFilePath,&solid))
			{
				AfxMessageBox("�����ʧ��");
				return FALSE;
			}
			else
			{
				detailinfo->solid=solid;
				if (!pdtclass.NewRename(solid,detailinfo->SolidFileName,&(detailinfo->DrwName)))
				{
					AfxMessageBox("������ʧ�ܣ�");
					return FALSE;
				}
			}
		}
	}
	p_classarray.Add(detailinfo);
	*s=(int)p_classarray.GetSize()-1;
	return TRUE;
}
/************************************************************************/
/* �ж��ڴ����Ƿ������в�Ʒ���ļ�,FALSE˵���д���                       */
/************************************************************************/
BOOL CPrtClass::IsPartFileAlreadyInSession(CString SolidName,CString DrwName)
{
	CCommonFuncClass commonfun;
	CArray <ProSolid,ProSolid> p_arraylist;
	if (!commonfun.GetAlreadyInTheSession(&p_arraylist))
	{
		return FALSE;
	}               
	//���û�а汾�ŵĺ�׺
	if (SolidName.Right(4)==".prt" || SolidName.Right(4)==".asm")
	{
		if (commonfun.IsAlreadyInTheSession(SolidName,&p_arraylist))
		{
			return FALSE;
		}
	}
	else
	{
		int k=SolidName.ReverseFind('.');
		if (k!=-1)
		{
			 SolidName.Delete(k,(SolidName.GetLength()-k));
		}
		if (commonfun.IsAlreadyInTheSession(SolidName,&p_arraylist))
		{
			return FALSE;
		}
	}
	if (DrwName.Right(4)==".drw")
	{
		if (commonfun.IsAlreadyInTheSession(DrwName,&p_arraylist))
		{
			return FALSE;
		}
	}
	else
	{
		int k=DrwName.ReverseFind('.');
		if (k!=-1)
		{
			DrwName.Delete(k,(DrwName.GetLength()-k));
		}
		if (commonfun.IsAlreadyInTheSession(DrwName,&p_arraylist))
		{
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* �ж��Ƿ���˹�                                                       */
/************************************************************************/
BOOL CPrtClass::IsAlreadyAudit(int SolidID,BOOL *IsAudit)
{
	CString sql,str;
	sql.Format("SELECT * FROM  NPartInsInfoTable where PrtNum=%d",SolidID);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveEnd();
	    str=m_db.getString("AuditState");
		if (str=="���ͨ��")
		{
			*IsAudit=TRUE;
		}
		else
			*IsAudit=FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* �ڱ�������У��жϱ��淽ʽ�У�����õ��滻����Ҫ�õ�ԭ���Ķ���       */
/************************************************************************/
BOOL CPrtClass::GetInfoFromDatabaseBase(int s,CString sql)	  
{
	CString str,SolidPlace,DrwPlace;
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z=0)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum.Format("%d",m_db.getInt("MdlNum"));
			((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=m_db.getString("InsCode");
			((DetailInfoClass *)p_classarray.GetAt(s))->InsName=m_db.getString("Name");

			((DetailInfoClass *)p_classarray.GetAt(s))->GroupNum=m_db.getInt("TypeNum");
			((DetailInfoClass *)p_classarray.GetAt(s))->DesignState=m_db.getString("FinishState");

			((DetailInfoClass *)p_classarray.GetAt(s))->Unit=m_db.getString("Unit");
			((DetailInfoClass *)p_classarray.GetAt(s))->Designer=m_db.getString("Designer");
			((DetailInfoClass *)p_classarray.GetAt(s))->Auditor=m_db.getString("Auditor");
			((DetailInfoClass *)p_classarray.GetAt(s))->AuditState=m_db.getString("AuditState");

			((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName=m_db.getString("SolidFileName");
			((DetailInfoClass *)p_classarray.GetAt(s))->DrwName=m_db.getString("DrawingName");
			((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=m_db.getString("SolidPlace");
			((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=m_db.getString("DrawingPlace");

			((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFileName=m_db.getString("SolidFileName");
			((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwName=m_db.getString("DrawingName");
			((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFilePath=m_db.getString("SolidPlace");
			((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwFilePath=m_db.getString("DrawingPlace");

			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=m_db.getString("LocalState");
			((DetailInfoClass *)p_classarray.GetAt(s))->PDMState=m_db.getString("PDMState");
			((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode=m_db.getString("PDMNum");
			((DetailInfoClass *)p_classarray.GetAt(s))->Note=m_db.getString("Note");
		} while(m_db.MoveNext());
	}
	BOOL IsPart;
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName.Right(4)==".asm")
	{
		IsPart=FALSE;
		//�õ�ģ����
		if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum=="-1")
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->MdlName="��";
		}
		else
		{
			sql.Format("SELECT * FROM NAsmManageTable WHERE Number=%s",((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum);
			Z=m_db.Query(sql);
			if (Z==-1 || Z==0)
			{
				return FALSE;
			}
			else
			{
				m_db.MoveBegin();
				do {
					((DetailInfoClass *)p_classarray.GetAt(s))->MdlName=m_db.getString("Name");
				} while(m_db.MoveNext());
			}
		}
		//�������ͺţ��õ�������
		sql.Format("SELECT * FROM NAsmType where Number=%d",((DetailInfoClass *)p_classarray.GetAt(s))->GroupNum);
		Z=m_db.Query(sql);
		if (Z==-1)
		{
			return FALSE;
		}
		if (Z!=0)
		{
			m_db.MoveBegin();
			do {
				((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm=m_db.getString("Name");
			}while (m_db.MoveNext());
		}
		//�õ����ܲ�����ֵ
		((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue.RemoveAll();
		sql.Format("SELECT * FROM NProductInsPerfTable WHERE PdtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
		Z=m_db.Query(sql);
		if (Z==-1)
		{
			return FALSE;
		}
		if (Z==0)
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue.RemoveAll();
		}
		else
		{
			m_db.MoveBegin();
			do {
				str=m_db.getString("Value");
				((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue.Add(str);
			} while(m_db.MoveNext());
		}
	}
	else
	{
		IsPart=TRUE;
		//�õ�ģ����
		if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum=="-1")
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->MdlName="��";
		}
		else
		{
			sql.Format("SELECT * FROM NPartManageTable WHERE Number=%s",((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum);
			Z=m_db.Query(sql);
			if (Z==-1 || Z==0)
			{
				return FALSE;
			}
			else
			{
				m_db.MoveBegin();
				do {
					((DetailInfoClass *)p_classarray.GetAt(s))->MdlName=m_db.getString("Name");
				} while(m_db.MoveNext());
			}
		}
		//�������ͺţ��õ�������
		sql.Format("SELECT * FROM NPartType where Number=%d",((DetailInfoClass *)p_classarray.GetAt(s))->GroupNum);
		Z=m_db.Query(sql);
		if (Z==-1)
		{
			return FALSE;
		}
		if (Z!=0)
		{
			m_db.MoveBegin();
			do {
				((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm=m_db.getString("Name");
			}while (m_db.MoveNext());
		}
		//�õ����ܲ�����ֵ
		((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue.RemoveAll();
		sql.Format("SELECT * FROM NPartInsPerfTable WHERE PrtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
		Z=m_db.Query(sql);
		if (Z==-1)
		{
			return FALSE;
		}
		if (Z==0)
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue.RemoveAll();
		}
		else
		{
			m_db.MoveBegin();
			do {
				str=m_db.getString("Value");
				((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue.Add(str);
			} while(m_db.MoveNext());
		}
	}
	return TRUE;
}
/************************************************************************/
/* ��PDM�еõ��ļ����ͱ���λ��                                          */
/************************************************************************/
BOOL CPrtClass::GetDocumentsFromPDM(CString DatabaseDre,DetailInfoClass *info,BOOL *Find)
{
	CMyDatabase m_pdm;
	CString Edition;
	CString sql,str;
	info->OldDrwName="";            //�ȰѶ�άͼ���ļ����ÿգ����ж��Ƿ��ж�άͼ���ж���
	if(!m_pdm.Open(DatabaseDre))
	{
		return FALSE;
	}
	sql.Format("select * from Documents where Code='%s'",info->PDMCode);
	int Z=m_pdm.Query(sql);
	if (Z==-1)
	{
		(*Find)=FALSE;
		m_pdm.Close();
		return FALSE;
	}
	if (Z==0)
	{
		(*Find)=FALSE;
	}
	else
		if(Z!=0)             //�����PDM����������������PDM��������������ع��������ļ�
		{
			m_pdm.MoveBegin();
			do {
				(*Find)=TRUE;
				str=m_pdm.getString("FileName");
				str.MakeLower();
				int k=str.ReverseFind('.');
				if (k!=-1)
				{
					str.Delete(k,(str.GetLength()-k));
				}
				if(str.Right(4)==".prt" || str.Right(4)==".asm")          //����άͼ
				{
					str=m_pdm.getString("FileName");
					str.MakeLower();
					info->OldSolidFileName=str;
				}
				else
					if (str.Right(4)==".drw")                              //�Ƕ�άͼ
					{
						str=m_pdm.getString("FileName");
						str.MakeLower();
						info->OldDrwName=str;
					}
				str=m_pdm.getString("ProductDwgCode");             //��װͼ��
				Edition=m_pdm.getString("RevNumber");  //PDM�İ汾��A��B��C�ȵ�
				CString tempstr;
				tempstr=Ftp_struc.PDMPath+str+"/"+info->PDMCode+"/"+Edition;
				tempstr.Replace(" ","#");
				info->OldSolidFilePath=tempstr;
				tempstr=Ftp_struc.PDMPath+str+"/"+info->PDMCode+"/"+Edition;
				tempstr.Replace(" ","#");
				info->OldDrwFilePath=tempstr;
			} while(m_pdm.MoveNext());
		}
	m_pdm.Close();
	return TRUE;
}






/************************************************************************/
/* ��PDM�еõ��ļ����ͱ���λ��                                          */
/************************************************************************/
BOOL CPrtClass::GetDocumentsFromNoPDM(DetailInfoClass *info,BOOL *Find)
{

	CString Edition;
	CString sql,str;
	info->OldDrwName="";            //�ȰѶ�άͼ���ļ����ÿգ����ж��Ƿ��ж�άͼ���ж���
	

	sql.Format("select * from NPartInsInfoTable where InsCode='%s'",info->InsCode);
	int Z=m_db.Query(sql);
	
	if (Z==0)
	{
		(*Find)=FALSE;
	}
	else
		if(Z!=0)             //���������������������������������ع��������ļ�
		{
			m_db.MoveBegin();
			do {
				(*Find)=TRUE;
		
			                  //����άͼ
					str=m_db.getString("SolidFileName");
					str.MakeLower();
					info->OldSolidFileName=str;
				               //�Ƕ�άͼ
					str=m_db.getString("DrawingName");
					str.MakeLower();
					info->OldDrwName=str;
					
				    CString SolidPlace,DrawingPlace;
					SolidPlace=m_db.getString("SolidPlace");
					DrawingPlace=m_db.getString("DrawingPlace");
				
					info->OldSolidFilePath=SolidPlace;

				
					info->OldDrwFilePath=DrawingPlace;
			} while(m_db.MoveNext());
		}
	
		return TRUE;
}





























//
//
//BOOL GetSavePlaceInfo(int SolidID,CString Type,FilePath *uu);
//BOOL GetDocumentsFromPDM(CString DatabaseDre,FilePath **uu,BOOL *Find);
//
///************************************************************************/
///* ���²��ĵõ�������߲�Ʒ�ĵ�ַ                                       */
///************************************************************************/
////����Ҫ����PDM�����ݿ��
//BOOL GetInfo(int SolidID,CString Type,CArray <FilePath,FilePath>  *FilePath_struc)
//{
//	int Z;
//	FilePath uu;
//	CString sql;
//	if (Type=="prt")
//	{
//        if(!GetSavePlaceInfo(SolidID,Type,&uu))
//		{
//			return FALSE;
//		}
//		FilePath_struc->Add(uu);
//	}
//	else
//		if (Type=="asm")
//		{
//			if(!GetSavePlaceInfo(SolidID,Type,&uu))
//			{
//				return FALSE;
//			}
//			FilePath_struc->Add(uu);
//			//�õ����������ص�ַ���ļ���
//			sql.Format("SELECT * FROM NProductBOMTable WHERE PdtNum=%d",SolidID);
//			Z=m_db.Query(sql);
//			if (Z==-1 || Z==0)
//			{
//				AfxMessageBox("��Ҫ�������Ʒ�����ڣ�");
//				return FALSE;
//			}
//			else
//			{
//				m_db.MoveBegin();
//				do {
//					uu.SolidID=m_db.getInt("PrtNum");
//					uu.Type=m_db.getString("Type");
//					FilePath_struc->Add(uu);
//				} while(m_db.MoveNext());
//			}
//			for(int i=1;i<FilePath_struc->GetSize();i++)
//			{
//                if(!GetSavePlaceInfo((*FilePath_struc)[i].SolidID,(*FilePath_struc)[i].Type,&uu))
//				{
//					return FALSE;
//				}
//				else
//				{
//                    (*FilePath_struc)[i].SolidID=uu.SolidID;
//					(*FilePath_struc)[i].SolidName=uu.SolidName;
//					(*FilePath_struc)[i].SolidFilePath=uu.SolidFilePath;
//					(*FilePath_struc)[i].DrwName=uu.DrwName;
//					(*FilePath_struc)[i].DrwFilePath=uu.DrwFilePath;
//					(*FilePath_struc)[i].MdlNum=uu.MdlNum;
//					(*FilePath_struc)[i].InsCode=uu.InsCode;
//					(*FilePath_struc)[i].AddToPDM=uu.AddToPDM;
//				}
//			}
//		}
//	return TRUE;
//}
///************************************************************************/
///*�õ���������ļ����߲�Ʒ�ļ��ĵ�ַ                                    */
///************************************************************************/
//BOOL GetSavePlaceInfo(int SolidID,CString Type,FilePath *uu)
//{
//	CString sql;
//	if (Type=="prt")
//	{
//        sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%d",SolidID);
//		uu->Type="prt";
//	}
//	else
//		if (Type=="asm")
//		{
//			sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%d",SolidID);     //���ݲ�Ʒ�ò���
//			uu->Type="asm";
//		}
//		int Z=m_db.Query(sql);
//		if (Z==-1 || Z==0)
//		{
//			AfxMessageBox("��Ҫ�����/��Ʒ�����ڣ�");
//			return FALSE;
//		}
//		else
//		{
//			m_db.MoveBegin();
//			do {
//				uu->SolidID=SolidID;
//				uu->SolidName=m_db.getString("SolidFileName");
//				uu->SolidFilePath=m_db.getString("SolidPlace");
//				uu->DrwName=m_db.getString("DrawingName");
//				uu->DrwFilePath=m_db.getString("DrawingPlace");
//				uu->MdlNum.Format("%d",m_db.getInt("MdlNum"));
//				uu->InsCode=m_db.getString("PDMNum");
//				if (m_db.getString("PDMState")=="δ�ύ")
//				{
//					uu->AddToPDM=FALSE;             //δ�ύ��PDM��
//				}
//				else
//					uu->AddToPDM=TRUE;              //�Ѿ��ύ��PDM��
//			} while(m_db.MoveNext());
//		}
//		if (uu->AddToPDM)        //˵����ͼ��
//		{
//			BOOL Find;
//			//PDM�����ݿ����ӵ�ַ
//			if (!GetDocumentsFromPDM(Ftp_struc.Database,&uu,&Find))
//			{
//				return FALSE;
//			}
//			else
//			{
//				if (!Find)
//				{
//					uu->SolidName=uu->SolidName+".1";
//					AfxMessageBox(uu->SolidName);
//					uu->DrwName=uu->DrwName+".1";
//					AfxMessageBox(uu->DrwName);
//				}  
//			}
//		}
//		else
//		{
//			uu->SolidName=uu->SolidName+".1";
//			uu->DrwName=uu->DrwName+".1";
//		}
//		return TRUE;
//}
///************************************************************************/
///*                                                                      */
///************************************************************************/
//BOOL GetDocumentsFromPDM(CString DatabaseDre,FilePath **uu,BOOL *Find)
//{
//	CMyDatabase m_pdm;
//	CString Edition;
//	CString sql,str;
//	(*uu)->DrwName="";            //�ȰѶ�άͼ���ļ����ÿգ����ж��Ƿ��ж�άͼ���ж���
//	if(!m_pdm.Open(DatabaseDre))
//	{
//		return FALSE;
//	}
//	sql.Format("select * from Documents where Code='%s'",(*uu)->InsCode);
//	int Z=m_pdm.Query(sql);
//	if (Z==-1)
//	{
//		(*Find)=FALSE;
//		m_pdm.Close();
//		return FALSE;
//	}
//	if (Z==0)
//	{
//		(*Find)=FALSE;
//	}
//	else
//		if(Z!=0)             //�����PDM����������������PDM��������������ع��������ļ�
//		{
//			m_pdm.MoveBegin();
//			do {
//				(*Find)=TRUE;
//				str=m_pdm.getString("FileName");
//				str.MakeLower();
//				int k=str.ReverseFind('.');
//				if (k!=-1)
//				{
//					str.Delete(k,(str.GetLength()-k));
//				}
//				if(str.Right(4)==".prt" || str.Right(4)==".asm")          //����άͼ
//				{
//					str=m_pdm.getString("FileName");
//					str.MakeLower();
//					(*uu)->SolidName=str;
//				}
//				else
//					if (str.Right(4)==".drw")                              //�Ƕ�άͼ
//					{
//						str=m_pdm.getString("FileName");
//						str.MakeLower();
//						(*uu)->DrwName=str;
//					}
//					str=m_pdm.getString("ProductDwgCode");
//					Edition=m_pdm.getString("RevNumber");
//					(*uu)->SolidFilePath=Ftp_struc.PDMPath+str+"/"+(*uu)->InsCode+"/"+Edition;
//					(*uu)->DrwFilePath=Ftp_struc.PDMPath+str+"/"+(*uu)->InsCode+"/"+Edition;
//			} while(m_pdm.MoveNext());
//		}
//		m_pdm.Close();
//		return TRUE;
//}