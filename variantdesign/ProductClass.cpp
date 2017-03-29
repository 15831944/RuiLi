// ProductClass.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "ProductClass.h"
#include "CommonFuncClass.h"
#include "DetailInfoClass.h"
#include "Compute.h"
#include "FtpFile.h"
#include "MyDatabase.h"
#include "ProWindows.h"
#include "ProUtil.h"
#include "PrtClass.h"
#include "FtpFile.h"
#include "DownLoadDlg.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>




void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data,BOOL nFlag);


extern CArray <FilePath,FilePath> FilePath_struc;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CArray <JuBing,JuBing> JuBing_struc_temp;
extern CPtrArray p_classarray;
extern CMyDatabase m_db;
extern CArray <Database,Database> Database_struc_temp;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern FtpInfo Ftp_struc;

// CProductClass

CProductClass::CProductClass()
{
}

CProductClass::~CProductClass()
{
}


// CProductClass ��Ա����
//1.�ж�ͼ���ļ����Ƿ����Ҫ��
//2.�µĲ�Ʒ�ļ����Ƿ����
//3.Ҫ���������Ĳ�Ʒģ�ͻ��߲�Ʒʵ�����Ƿ����
/************************************************************************/
/* �ж�ͼ�ţ��ļ����Ƿ����Ҫ��                                         */
/************************************************************************/
BOOL CProductClass::IsInsCodeOk(CString InsCode,CString SolidName,CString DrwName)
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
/************************************************************************/
/* �����                                                             */
/************************************************************************/
BOOL CProductClass::ActivateAsm(ProSolid solid)
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
/* Ϊ��Ʒ�õ����е��������Ϣ											*/
/************************************************************************/
BOOL CProductClass::GetAllPartInfoForAsm(ProSolid solid,BOOL IsNew)
{
	CString sql;
	BOOL IsOk=TRUE;
	CCompute compute;
	CCommonFuncClass commonfun;
	JuBing_struc.RemoveAll();
	if (!compute.ObtainJuBing(solid))
	{
		return FALSE;
	}
	for (int i=1;i<FilePath_struc.GetSize();i++)
	{
		if (FilePath_struc[i].Type=="prt")
		{
			sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%d",FilePath_struc[i].SolidID);
			if(!GetInfoFromDatabaseBase(sql,i,TRUE,IsNew))				  //��ģ��
			{
				return FALSE;
			}
		}
		if (FilePath_struc[i].Type=="asm")
		{
			sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%d",FilePath_struc[i].SolidID);
			if(!GetInfoFromDatabaseBase(sql,i,FALSE,IsNew))				  //���½�
			{
				return FALSE;
			}
		}
	}
	if (!IsNew)		   //��������½���Ʒ��������Ʒ�����е���Ϣ
	{
		if (FilePath_struc[0].Type=="prt")
		{
			sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%d",FilePath_struc[0].SolidID);
			if(!GetInfoFromDatabaseBase(sql,0,TRUE,IsNew))				  //��ģ��
			{
				return FALSE;
			}
		}
		if (FilePath_struc[0].Type=="asm")
		{
			sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%d",FilePath_struc[0].SolidID);
			if(!GetInfoFromDatabaseBase(sql,0,FALSE,IsNew))				  //���½�
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* ���������ʵ���Ļ�                                                   */
/************************************************************************/
BOOL CProductClass::GetInfoFromDatabaseBase(CString sql,int Num,BOOL IsPart,BOOL IsNew)	  //2.ģ�͵�������
{
	CString str;
	ProName w_name;
	CString Name;
	ProError status;
	DetailInfoClass *info=NULL;
	info = new DetailInfoClass;
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		delete info;
		return FALSE;
	}
	if (Z=0)
	{
		delete info;
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			if (IsPart)
			{
				info->SolidID=m_db.getInt("PrtNum");
				info->Material=m_db.getString("Material");
				info->Type="prt";
			}
			else
			{
				info->SolidID=m_db.getInt("PdtNum");
				info->Type="asm";
			}
			info->MdlCompNum=FilePath_struc[Num].MdlCompNum;
			info->MdlNum.Format("%d",m_db.getInt("MdlNum"));
			info->InsCode=m_db.getString("InsCode");
			info->InsName=m_db.getString("Name");
			if (IsNew)
			{
				info->IsInsert="F";
			}
			else
			{
				info->IsSaved=TRUE;
				info->IsInsert=FilePath_struc[Num].IsInsert;          //�õ��Ƿ��ǲ������
			}
			info->GroupNum=m_db.getInt("TypeNum");
			info->DesignState=m_db.getString("FinishState");

			info->Unit=m_db.getString("Unit");
			info->Designer=m_db.getString("Designer");
			info->Auditor=m_db.getString("Auditor");
			info->AuditState=m_db.getString("AuditState");

			info->OldSolidFileName=m_db.getString("SolidFileName");
			info->OldDrwName=m_db.getString("DrawingName");
			info->OldSolidFilePath=m_db.getString("SolidPlace");
			info->OldDrwFilePath=m_db.getString("DrawingPlace");

			//ģ�͵Ļ��ǵ�ǰ��λ��
			info->SolidFileName=m_db.getString("SolidFileName");
			info->DrwName=m_db.getString("DrawingName");
			info->SolidFilePath=m_db.getString("SolidPlace");
			info->DrwFilePath=m_db.getString("DrawingPlace");

			info->IsLocalOrServer=m_db.getString("LocalState");
			info->PDMCode=m_db.getString("PDMNum");
			info->Note=m_db.getString("Note");
		} while(m_db.MoveNext());

		//�ѽṹ�;����Ӧ����
		for (int i=0;i<JuBing_struc.GetSize();i++)
		{
			BOOL IsHave=FALSE;
			status=ProMdlNameGet((ProMdl)JuBing_struc[i].solid,w_name);
			if (status!=PRO_TK_NO_ERROR)
			{
				str.Format("�޷��õ�%s���ļ���",info->SolidFileName);
				AfxMessageBox(str);
				return FALSE;
			}
			if (JuBing_struc[i].Type=="prt")
			{
				Name=CString(w_name)+".prt";
			}
			else
				if (JuBing_struc[i].Type=="asm")
				{
					Name=CString(w_name)+".asm";
				}
				else
					AfxMessageBox("�õ���������ʹ���");
			if (Name.MakeLower()==info->SolidFileName.MakeLower())
			{
				IsHave=TRUE;
				info->solid=JuBing_struc[i].solid;
				break;
			}
			if (i>=JuBing_struc.GetSize()-1)
			{
				if (!IsHave)
				{
					str.Format("�޷���Ӧ%s�ľ��",info->SolidFileName);
					AfxMessageBox(str);
					return FALSE;
				}
			}
		}
		//�õ����ܲ���
		if (IsPart)
		{
			//�õ������ģ����
			if (info->MdlNum=="-1")
			{
				info->MdlName="��";
			}
			else
			{
				sql.Format("SELECT * FROM NPartManageTable WHERE Number=%s",info->MdlNum);
				Z=m_db.Query(sql);
				if (Z==-1 || Z==0)
				{
					return FALSE;
				}
				else
				{
					m_db.MoveBegin();
					do {
						info->MdlName=m_db.getString("Name");
					} while(m_db.MoveNext());
				}
			}
			sql.Format("SELECT * FROM NPartInsPerfTable WHERE PrtNum=%d",info->SolidID);
			Z=m_db.Query(sql);
			if (Z==-1)
			{ 
				delete info;
				return FALSE;
			}
			if (Z==0)
			{
				info->PerformanceValue.RemoveAll();
			}
			else
			{
				m_db.MoveBegin();
				do {
					str=m_db.getString("Value");
					info->PerformanceValue.Add(str);
				} while(m_db.MoveNext());
			}
			sql.Format("SELECT * FROM NPartType where Number=%d",info->GroupNum);
			Z=m_db.Query(sql);
			if (Z==-1 || Z==0)
			{
				delete info;
				return FALSE;
			}
			else
			{
				m_db.MoveBegin();
				do {
					info->GrpFrm=m_db.getString("Name");
					break;
				} while(m_db.MoveNext());
			}
		}
		else
		{
			//�õ�ģ����
			if (info->MdlNum=="-1")
			{
				info->MdlName="��";
			}
			else
			{
				sql.Format("SELECT * FROM NAsmManageTable WHERE Number=%s",info->MdlNum);
				Z=m_db.Query(sql);
				if (Z==-1 || Z==0)
				{
					return FALSE;
				}
				else
				{
					m_db.MoveBegin();
					do {
						info->MdlName=m_db.getString("Name");
					} while(m_db.MoveNext());
				}
			}
			sql.Format("SELECT * FROM NProductInsPerfTable WHERE PdtNum=%d",info->SolidID);
			Z=m_db.Query(sql);
			if (Z==-1)
			{ 
				delete info;
				return FALSE;
			}
			if (Z==0)
			{
				info->PerformanceValue.RemoveAll();
			}
			else
			{
				m_db.MoveBegin();
				do {
					str=m_db.getString("Value");
					info->PerformanceValue.Add(str);
				} while(m_db.MoveNext());
			}
			sql.Format("SELECT * FROM NAsmType where Number=%d",info->GroupNum);
			Z=m_db.Query(sql);
			if (Z==-1 || Z==0)
			{
				delete info;
				return FALSE;
			}
			else
			{
				m_db.MoveBegin();
				do {
					info->GrpFrm=m_db.getString("Name");
					break;
				} while(m_db.MoveNext());
			}
		}
	}
	p_classarray.Add(info);
	return TRUE;
}

/************************************************************************/
/* д��Ʒ���ݱ������ﴦ��ֻ�����Ʒ����Ҫ�������ﴦ��             */
/************************************************************************/
BOOL CProductClass::WriteProductToDatabase(ProSolid solid,BOOL NeedTrans,BOOL IsLocal)
{
	//��Ҫ�������ﴦ��
	m_db.m_pConnection->Close();
	m_db.ReOpen();
	try
	{
		m_db.m_pConnection->BeginTrans();	
		if(!WriteAsmToDatabase(solid,NeedTrans,IsLocal))
		{
			m_db.m_pConnection->RollbackTrans();
			return FALSE;
		}
		m_db.m_pConnection->CommitTrans();
	}
	catch (_com_error e)
	{
		m_db.m_pConnection->RollbackTrans();
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* д��Ʒ���ݱ������ﴦ�� ����ֻ�Բ�Ʒ���£��ڱ��������ͬʱ��        */
/************************************************************************/
BOOL CProductClass::WriteAsmToDatabase(ProSolid solid,BOOL NeedTrans,BOOL IsLocal)
{
	//��Ҫ�������ﴦ��
	CString strLocalFilePath;
	CFtpFile ftp;
	CCompute compute;
	CCommonFuncClass commonfun;
	int s=commonfun.IsBasicInfoHave(solid);
	{
		if (s==-1)
		{
			return FALSE;
		}
	}	

	if (IsLocal)
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="����";
		if (!commonfun.SaveProductFile(solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath))
		{
			AfxMessageBox("��Ʒ����ʧ��");
			return FALSE;
		}
	}
	else
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="������";
		if (!commonfun.BackUpFile(s))
		{
			AfxMessageBox("��Ʒ����ʧ��");
			return FALSE;
		}
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
	{
		//�����ļ�
		if (!InsertNewAsm(s,NeedTrans))
		{
			return FALSE;
		}//;�������
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)
	{
		//;�������
		if (!UpdateNewAsm(s,NeedTrans))
		{
			return FALSE;
		}
	}
	//��дBOM����ɱ��������������
	//�ж�����Ƿ��Ѿ�����
	if (!WriteAsmExtraToDatabase(solid))
	{
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/*���²�����BOM����Ʒ�嵥����Ʒ�ɱ��㲿���Ŀɱ������               */
/************************************************************************/
BOOL CProductClass::WriteAsmExtraToDatabase(ProSolid solid)			//д������ݿ⺯����1.������ 
{
	int Pos;
	CString sql;
	CCompute compute;
	CCommonFuncClass commonfun;
	JuBing_struc.RemoveAll();

	Pos=commonfun.IsBasicInfoHave(solid);
	if (Pos==-1)
	{
		return FALSE;
	}
	sql.Format("SELECT * From NProductBOMTable Where PdtNum=%d",((DetailInfoClass *)p_classarray.GetAt(Pos))->SolidID);
	if (m_db.Query(sql)==0)
	{
		if(!InsertThreeTable(solid,Pos))
		{
			return FALSE;
		}
	}
	else
	{
		if (!UpdateThreeTable(solid,Pos))
		{
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* ���������                                                           */
/************************************************************************/
BOOL CProductClass::InsertNewAsm(int s,BOOL NeedTrans)			//�Ƿ���Ҫ�ϴ��ļ���������
{
	CString str;
	CCommonFuncClass commonfun;
	if (!InsertAsmDatabase(s))
	{
		return FALSE;
	}
	else
	{
        str="SELECT * FROM NProductInsInfoTable";
		m_db.Query(str);
		m_db.MoveEnd();
		str.Format("%d",m_db.getInt("PdtNum"));
	   ((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=atoi(str);
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum!="-1")
	{
		//�����������Ա�����ܲ�����]
		if (!InsertAsmVariantAndPerf(s,atoi(str)))
		{
			return FALSE;
		}
	}
	if (NeedTrans)
	{
		if (!commonfun.UpLoadFile(s))
		{
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* ����������ݿ������                                          */
/************************************************************************/
BOOL CProductClass::UpdateNewAsm(int s,BOOL NeedTrans)
{
	CString sql;
	CCompute compute;
	CCommonFuncClass commonfun;
	/////
	sql.Format("UPDATE NProductInsInfoTable Set InsCode='%s',TypeNum=%d,Name='%s',Designer='%s',FinishTime='%s',Auditor='%s',AuditTime='%s',SolidFileName='%s',SolidPlace='%s',DrawingName='%s',DrawingPlace='%s',FinishState='%s',AuditState='%s',LocalState='%s',PDMState='%s',PDMNum='%s',Note='%s' where PdtNum=%d",
		((DetailInfoClass *)p_classarray.GetAt(s))->InsCode,((DetailInfoClass *)p_classarray.GetAt(s))->GroupNum,
		((DetailInfoClass *)p_classarray.GetAt(s))->InsName,((DetailInfoClass *)p_classarray.GetAt(s))->Designer,
		((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime,((DetailInfoClass *)p_classarray.GetAt(s))->Auditor,
		((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,
		((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwName,
		((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DesignState,
		((DetailInfoClass *)p_classarray.GetAt(s))->AuditState,((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer,
		((DetailInfoClass *)p_classarray.GetAt(s))->PDMState,((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode,
		((DetailInfoClass *)p_classarray.GetAt(s))->Note,((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	if(m_db.Execute(sql)==0)
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum=="-1")
	{
		return TRUE;
	}
	else
	{
		Database_struc_temp.RemoveAll();
		if(!compute.ObtainAsmFromBaseTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum))
		{
			return FALSE;	 //��������������ʱ�ṹ��
		}                   
		////�Ѿ����ӵ��ṹ��ȥ
		if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
		{
			return FALSE;
		}
		//���ˣ�Ӧ��������ĸ���
		if (!UpdateAsmVariantAndPerf(s))
		{
			return FALSE;
		}
	}
	if (NeedTrans)
	{
		if (!commonfun.UpLoadFile(s))
		{
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* ���²�Ʒ�����ܲ������������Ա�                                       */
/************************************************************************/
BOOL CProductClass::UpdateAsmVariantAndPerf(int s)
{
	CString sql;
	sql.Format("DELETE FROM NProductInsPerfTable WHERE PdtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	if(m_db.Execute(sql)==0)
	{
		return FALSE;
	}
    for (int i=0;i<(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).GetSize();i++)
	{
		sql.Format("INSERT INTO NProductInsPerfTable VALUES (%d,%d,%d,'%s')",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,i+1,
			atol(((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum),
			(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue)[i]);
		if(m_db.Execute(sql)==0)
		{
			return FALSE;
		}
	}
	for (int i=0;i<Database_struc_temp.GetSize();i++)
	{
		sql.Format("UPDATE NProductInsVariantTable Set Value='%s' where PdtNum=%d AND Num=%d",
			Database_struc_temp[i].value,((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,i+1);
		if(m_db.Execute(sql)==0)
		{
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* ������������ܲ������������Ա�                                       */
/************************************************************************/
BOOL CProductClass::InsertAsmVariantAndPerf(int s,int PdtNum)
{
	CString sql;
	CCompute compute;
	Database_struc_temp.RemoveAll();
	if(!compute.ObtainAsmFromBaseTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum))
	{
		return FALSE;
	}
	if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
	{
		return FALSE;
	}

	for (int i=0;i<(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).GetSize();i++)
	{
		sql.Format("INSERT INTO NProductInsPerfTable VALUES (%d,%d,%d,'%s')",PdtNum,i+1,
			atol(((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum),
			(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue)[i]);
		if(m_db.Execute(sql)==0)
		{
			return FALSE;
		}
	}
	for (int i=0;i<Database_struc_temp.GetSize();i++)
	{
		sql.Format("INSERT INTO NProductInsVariantTable VALUES (%d,%d,%d,'%s')",PdtNum,i+1,
			atol(((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum),
			Database_struc_temp[i].value);
		if(m_db.Execute(sql)==0)
		{
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* ����������ݿ�                                                           */
/************************************************************************/
BOOL CProductClass::InsertAsmDatabase(int s)
{
	CString sql;
	CCompute compute;
	sql.Format("INSERT INTO NProductInsInfoTable VALUES('%s',%d,'%s','%s',%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
		((DetailInfoClass *)p_classarray.GetAt(s))->InsCode,((DetailInfoClass *)p_classarray.GetAt(s))->GroupNum,
		((DetailInfoClass *)p_classarray.GetAt(s))->InsName,((DetailInfoClass *)p_classarray.GetAt(s))->Material,
		atol(((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum),((DetailInfoClass *)p_classarray.GetAt(s))->Designer,
		((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime,((DetailInfoClass *)p_classarray.GetAt(s))->Auditor,
		((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,
		((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwName,
		((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DesignState,
		((DetailInfoClass *)p_classarray.GetAt(s))->AuditState,((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer,
		((DetailInfoClass *)p_classarray.GetAt(s))->PDMState,((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode,
		((DetailInfoClass *)p_classarray.GetAt(s))->Note);
	if(m_db.Execute(sql)==0)
	{
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* �²���BOM,��ɱ��Ͳ�Ʒ���������                                   */
/************************************************************************/
BOOL CProductClass::InsertThreeTable(ProSolid solid,int Pos)
{
	CCompute compute;
	CCommonFuncClass commonfun;
	CString sql,str;
	JuBing_struc.RemoveAll();
	if (!compute.ObtainJuBing(solid))
	{
		return FALSE;
	}
	else
	{

		for (int i=1;i<JuBing_struc.GetSize();i++)
		{
			int s=commonfun.IsBasicInfoHave(JuBing_struc[i].solid);
			if (s==-1)
			{
				return FALSE;
			}
			else
			{
				sql.Format("INSERT INTO NProductBOMTable VALUES(%d,%d,%d,%d,%d,'%s','%s')",
					((DetailInfoClass *)p_classarray.GetAt(Pos))->SolidID,JuBing_struc[i].number,
					((DetailInfoClass *)p_classarray.GetAt(s))->MdlCompNum,JuBing_struc[i].upnum,
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,JuBing_struc[i].Type,
					((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert);

				if(m_db.Execute(sql)==0)
				{
					return FALSE;
				}
			}
		}
	}
	commonfun.IsHandleAlreadyHave();
	for (int j=1;j<JuBing_struc.GetSize();j++)
	{
		int s=commonfun.IsBasicInfoHave(JuBing_struc[j].solid);
		if (s==-1)
		{
			return FALSE;
		}
		else
		{
			sql.Format("INSERT INTO NProductListTable VALUES(%d,%d)",
				((DetailInfoClass *)p_classarray.GetAt(Pos))->SolidID,
				((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);

			if(m_db.Execute(sql)==0)
			{
				return FALSE;
			}
			if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum!="-1")
			{
				CString sql;
				CCompute compute;
				Database_struc_temp.RemoveAll();
				if(!compute.GetVariantForPart(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum,TRUE))
				{
					return FALSE;
				}
				if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
				{
					return FALSE;
				}

				for (int k=0;k<Database_struc_temp.GetSize();k++)
				{
					sql.Format("INSERT INTO NPdtPrtInsVariantTable VALUES (%d,%d,%d,'%s')",
						((DetailInfoClass *)p_classarray.GetAt(Pos))->SolidID,
						((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,
						k+1,Database_struc_temp[k].value);
					if(m_db.Execute(sql)==0)
					{
						return FALSE;
					}
				}
			}


		}
	}
	return TRUE;
}
/************************************************************************/
/* ����BOM,��ɱ��Ͳ�Ʒ���������                                   */
/************************************************************************/
BOOL CProductClass::UpdateThreeTable(ProSolid solid,int Pos)
{
	CCompute compute;
	CCommonFuncClass commonfun;
	CString sql;
	sql.Format("DELETE NProductBOMTable WHERE PdtNum=%d",((DetailInfoClass *)p_classarray.GetAt(Pos))->SolidID);
	if (m_db.Execute(sql)==0)
	{
		return FALSE;
	}
	sql.Format("DELETE NProductListTable WHERE PdtNum=%d",((DetailInfoClass *)p_classarray.GetAt(Pos))->SolidID);
	if (m_db.Execute(sql)==0)
	{
		return FALSE;
	}
	sql.Format("DELETE NPdtPrtInsVariantTable WHERE PdtNum=%d",((DetailInfoClass *)p_classarray.GetAt(Pos))->SolidID);
	if (m_db.Execute(sql)==0)
	{
		return FALSE;
	}
	if (!InsertThreeTable(solid,Pos))
	{
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* �ж�����Ƿ�ı�                                                     */
/************************************************************************/
BOOL CProductClass::IsPartModify(ProSolid solid,CString MdlNum,int SolidID)
{
	CString sql,str;
	BOOL IsHave=FALSE;
	CCompute compute;
	CCommonFuncClass commonfun;
	Database_struc_temp.RemoveAll();
	if(!compute.GetVariantForPart(solid,MdlNum,TRUE))
	{
		return TRUE;
	}
	if(!compute.ObtainValueFromMdlTemp(solid))
	{
		return FALSE;
	}

	sql.Format("SELECT * FROM NPartInsVariantTable WHERE PrtNum=%d",SolidID);
	//����ߴ�ֵ��ֵ�Ƿ���� �����������ע���ã����������ʾ���ͼ��
	if(!commonfun.GetDimInfo(MdlNum,sql,(int)Database_struc_temp.GetSize(),&IsHave))
	{
		return FALSE;	
	}
	else
		if (!IsHave)
		{
			return TRUE;    //	�Ѿ��ı�
		}
	return FALSE;    //û�б��ı�
}
/************************************************************************/
/* �ж�����Ƿ�ı�                                                     */
/************************************************************************/
BOOL CProductClass::IsAsmModify(ProSolid solid,CString MdlNum,int SolidID)
{
	CString sql,str;
	BOOL IsHave=FALSE;
	CCompute compute;
	CCommonFuncClass commonfun;
	Database_struc_temp.RemoveAll();
	if(!compute.ObtainAsmFromBaseTemp(solid,MdlNum))
	{
		return TRUE;
	}
	if(!compute.ObtainValueFromMdlTemp(solid))
	{
		return FALSE;
	}
	sql.Format("SELECT * FROM NProductInsVariantTable WHERE PdtNum=%d",SolidID);
	//����ߴ�ֵ��ֵ�Ƿ���� �����������ע���ã����������ʾ���ͼ��
	if(!commonfun.GetDimInfo(MdlNum,sql,(int)Database_struc_temp.GetSize(),&IsHave))
	{
		return TRUE;
	}
	if (!IsHave)
	{
		return TRUE;
	}
	return FALSE;
}
/************************************************************************/
/* �ж��Ƿ����������/���                                              */
/************************************************************************/
BOOL CProductClass::AffirmIsOld(CString Type,int s,BOOL New)
{
	CString sql,str,str1;
	int solidID;
	CPrtClass prtclass;
	CString FinishState;
	BOOL BeUsed;
	CCommonFuncClass commonfun;
	//��������
	if (Type=="prt")
	{
		solidID=commonfun.IsPartExist(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum);
		if (solidID==-1)          //�޸��˵����
		{
			//�������㲿��ID����Ĳ�Ʒ���ˣ�Ҫֱ���ò���
			if (!IsUsedByProduct(s,&BeUsed,Type))
			{
				return FALSE;
			}
			else
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->PDMState=="")
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="δ�ύ";
				}
				if (BeUsed)
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;	   //���²������
					((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
					((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=FALSE;
				}
				else
				{
					if (((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert=="F")	   //˵��û�в����
					{
						((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;	   //���²������
						((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
						((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=FALSE;
					}
					else
					{
						((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=2;	   //�滻ԭ���²�������
						((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
						((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=FALSE;
					}
				}
			}
		}
		else
		{
            //����ԭ����ID���ж��ò��뻹��
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;		   //�þ�����滻
			((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="F";
			((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
			((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=solidID;
			//�õ������Ʒ/�����������Ϣ
			sql.Format("SELECT * FROM NPartInsInfoTable WHERE PrtNum=%d",solidID);
			if (!prtclass.GetInfoFromDatabaseBase(s,sql))
			{
				AfxMessageBox("�޷��õ��滻�������Ϣ��");
				return FALSE;
			}	
			//��Ҫ����
			if (!NewRename(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,&(((DetailInfoClass *)p_classarray.GetAt(s))->DrwName)))
			{
				AfxMessageBox("����ʧ��");
				return FALSE;
			}
			((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
		}
	}
	//����ǲ�Ʒ
	else
		if (Type=="asm")
		{
			solidID=commonfun.IsAsmExist(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum);
			if (solidID==-1)
			{
				if (!IsUsedByProduct(s,&BeUsed,Type))
				{
					return FALSE;
				}
				else
				{
					if (((DetailInfoClass *)p_classarray.GetAt(s))->PDMState=="")
					{
						((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="δ�ύ";
					}
					if (BeUsed)
					{
                          ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;
						  ((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=FALSE;		  
					}
					else
					{
						if (((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert=="F")	   //˵��û�в����
						{
							((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;	   //���²������
							//((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="δ�ύ";
						}
						else
						{
							((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=2;	   //�滻ԭ���²�������
							((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
							//((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="δ�ύ";
						}
						((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=FALSE;
					}
				}
			}
			else
			{
				//�ж��������Ƿ��Ѿ���˹�
				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;		   //�þ�����滻
				((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
				((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=solidID;
				//�õ������Ʒ/�����������Ϣ
				sql.Format("SELECT * FROM NProductInsInfoTable WHERE PdtNum=%d",solidID);
				if (!prtclass.GetInfoFromDatabaseBase(s,sql))
				{
					AfxMessageBox("�޷��õ��滻��������Ϣ��");
					return FALSE;
				} 
				//��Ҫ����
				if (!NewRename(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,&(((DetailInfoClass *)p_classarray.GetAt(s))->DrwName)))
				{
					AfxMessageBox("����ʧ��");
					return FALSE;
				}
				((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
			}
		}
		else
			return FALSE;
	//�����淽ʽ��0��2��ʱ��Ҫ��ԭ�������ֺ����һ������ʾ�Ѿ��޸ģ���Ҫ���±���
	//if (New)
	//{
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName.Find("--")==-1)
	{
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)
		{
			if(!RenameAddChar(((DetailInfoClass *)p_classarray.GetAt(s))->solid,s,Type))
			{
				return FALSE;
			}
		}
		//ΪҪ�²�����������һ��ͼ��
		if(((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode+"--";
		}
	}
	//}
		return TRUE;
}
/**********************************************************************************************/
/* �����Ƿ���Ҫ��������֣���������Ҫ���±��棬��ԭ�������ֺ����һ����*��                    */
/**********************************************************************************************/
BOOL CProductClass::RenameAddChar(ProSolid solid,int s,CString Type)   //�Ƿ��ж�άͼ
{
	//�����ֺ��滻��һ��������
	CCommonFuncClass commonfun;
	ProError status;
	ProName w_name;
	CString str;
	CString SolidName,DrwName;

	if(((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)
	{
		status=ProMdlNameGet((ProMdl)solid,w_name);
		if (status!=PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
		if(Type=="prt")
		{
		   SolidName=(CString)w_name+"--.prt";
		}
		else
			if (Type=="asm")
			{
				SolidName=(CString)w_name+"--.asm";
			}
			else
				return FALSE;
		DrawingList.RemoveAll();
		GetSolidDrawing(solid,&DrawingList);
		for (int i=0;i<DrawingList.GetSize();i++)
		{
			status=ProMdlNameGet((ProMdl)DrawingList[i],w_name);
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
			DrwName=CString(w_name)+"--.drw";
		}
	    if(NewRename(solid,SolidName,&DrwName))
		{
            ((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFileName=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
			((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwName=((DetailInfoClass *)p_classarray.GetAt(s))->DrwName;
			((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName=SolidName;
			((DetailInfoClass *)p_classarray.GetAt(s))->DrwName=DrwName;
		}
		else
			return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* ���ݲ�ͬ�ķ�ʽ�����²�Ʒ�е����/����                                    */
/************************************************************************/
BOOL CProductClass::UpdateProductMenber(ProSolid ProductSolid,int s)   //1.��Ʒ�ľ�� 2.����Ľṹλ��
{
	CCommonFuncClass commonfun;
	CFtpFile ftp;
	BOOL IsLocal;
	CString strLocalFilePath;
	int Pos=commonfun.IsBasicInfoHave(ProductSolid);
	if (Pos==-1)
	{
		return FALSE;
	}
	m_db.m_pConnection->Close();
	m_db.ReOpen();
	try
	{
		m_db.m_pConnection->BeginTrans();
		//���ݲ�ͬ�ı���λ�ý��в�ͬ�Ĳ���
		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="����")
		{
			IsLocal=TRUE;
			//�����ļ�,д���ݿ�
			if(!WriteProductMenberToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,Pos,IsLocal))
			{
				m_db.m_pConnection->RollbackTrans();
				((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
				if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="F";
				}
				return FALSE;
			}
			m_db.m_pConnection->CommitTrans();
		}
		else
		{
			if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="������")
			{
				IsLocal=FALSE;
				if(!WriteProductMenberToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,Pos,IsLocal))
				{
					m_db.m_pConnection->RollbackTrans();
					//ɾ���ϴ����ļ�
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
					if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
					{
						((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="F";
					}
				    if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle!=1)
				    {
						strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->TempPath+"\\"+((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName+".1";
						ftp.RemoveFile(strLocalFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
						strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathSolid+"\\"+((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName+".1";
						ftp.UploadFiletoDirectory(strLocalFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
				    }
					return FALSE;
				}
				m_db.m_pConnection->CommitTrans();
			}
		}
	}
	catch (_com_error e)
	{
		m_db.m_pConnection->RollbackTrans();
		((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="F";
		}
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle!=1)
		{
			strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->TempPath+"\\"+((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName+".1";
			ftp.RemoveFile(strLocalFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
			strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathSolid+"\\"+((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName+".1";
			ftp.UploadFiletoDirectory(strLocalFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
		}
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* ���²�Ʒ���㲿��                                                     */
/************************************************************************/
BOOL CProductClass::WriteProductMenberToDatabase(ProSolid solid,int Pos,BOOL IsLocal)		 //1.����򲿼������2.��Ʒ�ṹ��λ�� 3.�Ƿ񱣴��ڱ���
{
	CCommonFuncClass commonfun;
	CPrtClass prtclass;
	int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1)				//����ǰ��ƺõ����ȥ�滻
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="F";
		if(!UpdateThreeTable(solid,Pos))                          //��Ʒ�ľ��
		{
			AfxMessageBox("���²�Ʒ��BOM��ʧ��");
			return FALSE;
		}
	}
	else
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)			//�滻��ǰ�����������
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;            //Ϊ�˺�����ĸ���ͨ��
			if (IsLocal)
			{
				//����������д����
				if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="prt")
				{
					if(!commonfun.UpdateNewPart(s,FALSE))
					{
						AfxMessageBox("�������ʧ��");
						return FALSE;
					}
				}
				else
				if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="asm")
				{
					if(!UpdateNewAsm(s,FALSE))//�滻��ǰ�Ĳ�����
					{
						return FALSE;
					}
					if (!WriteAsmExtraToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
					{
						return FALSE;
					}
				}
				else
					return FALSE;
				//�����ļ�
				if(!commonfun.SaveFile(solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				{
					AfxMessageBox("���汾���ļ�ʧ��");
					return FALSE;
				}
			}
			else	  //��������
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="prt")
				{
					if(!commonfun.SavePartToTheServerforProduct(s))		 //Ҫ�ж��Ǳ��ػ��Ƿ�����
					{
						AfxMessageBox("���������ʧ��");
						return FALSE;
					}
				}
				else
				if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="asm")
				{
					if(!UpdateNewAsm(s,TRUE))//�滻��ǰ�Ĳ�����
					{
						return FALSE;
					}
					if (!WriteAsmExtraToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
					{
						return FALSE;
					}
				}
				else
					return FALSE;
			}
			if(!UpdateThreeTable(solid,Pos))      //��Ʒ�ľ��
			{
				AfxMessageBox("���²�Ʒ��BOM��ʧ��");
				return FALSE;
			}
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=2;           //�ڰѱ��淽ʽ��Ϊԭ����ֵ
		}
		else			   //���²��������
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
		    if (IsLocal)
		    {
				//���²���һ�����
				if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="prt")
				{
					if(!commonfun.InsertNewPart(s,FALSE))		 //�����ж��Ǳ��ػ������
					{
						AfxMessageBox("�������ʧ��");
						return FALSE;
					}
				}
				else     //����ǲ���
					if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="asm")
					{
						 if (!InsertNewAsm(s,FALSE))
						 {
							 return FALSE;
						 }
						 if (!WriteAsmExtraToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
						 {
							 return FALSE;
						 }
					}
					else
						return FALSE;
				//�����ļ�
				if(!commonfun.SaveFile(solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				{
					AfxMessageBox("���汾���ļ�ʧ��");
					return FALSE;
				}
		    }
			else			 //�Ƿ�����
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="prt")
				{
					if(!commonfun.SavePartToTheServerforProduct(s))		 //Ҫ�ж��Ǳ��ػ��Ƿ�����
					{
						AfxMessageBox("���������ʧ��");
						return FALSE;
					}
				}
				else
					if(((DetailInfoClass *)p_classarray.GetAt(s))->Type=="asm")
					{
						if (!InsertNewAsm(s,TRUE))
						{
							return FALSE;
						}
						if (!WriteAsmExtraToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
						{
							return FALSE;
						}
					}
					else
						return FALSE;
			}
			if(!UpdateThreeTable(solid,Pos))      //��Ʒ�ľ��
			{
				AfxMessageBox("���²�Ʒ��BOM��ʧ��");
				return FALSE;
			}
		}
		return TRUE;
}
/************************************************************************/
/*���������������Ĳ������ļ�                                          */
/************************************************************************/
BOOL CProductClass::UpdateTheAsmTheContainTheCurrent(int Pos,ProSolid solid)
{
	CCommonFuncClass commonfun;
	CString SavePlace;
	CArray <ProSolid,ProSolid> asmarray;
	if (!commonfun.GetAllAsmContainCurrentMdl(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid,solid,&asmarray))
	{
		return FALSE;
	}
	for (int i=0;i<asmarray.GetSize();i++)
	{
		int s=commonfun.IsBasicInfoHave(asmarray[i]);
		if (s==-1)
		{
			return FALSE;
		}
		else
		{
			SavePlace=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
			int k=SavePlace.Find(":");
			if (k==-1)			 //˵���ڷ�������
			{
				//�ϴ��ļ�
				if (!commonfun.UpLoadFile(s))
				{
					return FALSE;
				}
			}
			else
			{
				//ֱ�ӱ����ļ�
				if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				{
					AfxMessageBox("��������ʧ��");
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* �򿪲�Ʒ�ļ�                                                         */
/************************************************************************/
BOOL CProductClass::OpenProductFile(CString PdtNum)              //��Ʒ���
{
	CCommonFuncClass commonfun;
	CCompute compute;
	CFtpFile ftp;
	CString Name,str;
	ProSolid solid;
	//����������άģ�ͺͶ�άģ��
	if (!GetSavePlaceInfo(PdtNum,"",FALSE,FALSE,&solid))            
	{
		return FALSE;
	}
	if (!GetAllPartInfoForAsm(solid,FALSE))         //�����½�
	{
		AfxMessageBox("�޷��õ������Ʒ�������������ϸ��Ϣ");
		return FALSE;
	}
	if (!ActivateAsm(solid))
	{
		AfxMessageBox("��ʾ��Ʒʧ��");
		return FALSE;
	}
	//commonfun.AddMdlInfo(solid);                 //��Ӳ�Ʒģ����Ϣ
	int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		return FALSE;
	}
	else
	if (!commonfun.SetProductPara(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
	{
		AfxMessageBox("���ò�������");
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* �½���Ʒ�ļ�                                                         */
/************************************************************************/
BOOL CProductClass::NewProductFile(DetailInfoClass *info,int *s,BOOL IsModel)	  //�Ƿ���ѡģ��
{
	CCommonFuncClass commonfun;
	CCompute compute;
	CFtpFile ftp;
	CString Name,str,sql;
	ProSolid solid;
	sql.Format("SELECT * FROM NAsmType where Number=%d",info->GroupNum);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			info->GrpFrm=m_db.getString("Name");
		} while(m_db.MoveNext());
	}
	//����������άģ�ͺͶ�άģ��
	if (IsModel)
	{
		//�õ���Ʒ�����ļ��ı���λ��
		if (!GetSavePlaceInfo(info->MdlNum,info->SolidFilePath,IsModel,TRUE,&solid))
		{
			AfxMessageBox("���ļ�ʧ��");
			return FALSE;
		}
	}
	else
	{   
		//�½���Ʒ��ʵ������
		str.Format("%d",info->OldSolidID);
		if (!GetSavePlaceInfo(str,info->SolidFilePath,IsModel,TRUE,&solid))
		{
			AfxMessageBox("���ļ�ʧ��");
			return FALSE;
		}
	}
	info->solid=solid;
	//��Ҫ����
	if (!NewRename(solid,info->SolidFileName,&(info->DrwName)))
	{
		AfxMessageBox("������ʧ��");
		return FALSE;
	}
	p_classarray.Add(info);
	*s=(int)p_classarray.GetSize()-1;			 
	if (!GetAllPartInfoForAsm(info->solid,TRUE))
	{
		AfxMessageBox("�޷��õ������Ʒ�������������ϸ��Ϣ");
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* �õ���Ʒ����ģ�͵ı���λ����Ϣ                                       */
/************************************************************************/
BOOL CProductClass::GetSavePlaceInfo(CString AsmNum,CString LocalPlace,BOOL IsModel,BOOL IsNew,ProSolid *solid)
//1.ģ�͵Ļ���ģ�ͺŻ���ʵ���Ļ��ǲ�Ʒ�ţ������ļ��ı���λ�ã��Ƿ���ѡģ�ͣ��Ƿ����½�
{
	CString sql,str,SolidPath,DrwPath;
	CFtpFile ftp;
	FilePath uu;
	ProPath path;
	ProError status;
	ProMdl DrwSolid;
	CPrtClass prtclass;
	DetailInfoClass *info=NULL;     //�������ã���Ϊprtclass.GetDocumentsFromPDM���õĺ���Ҫ�õ�
	info=new DetailInfoClass;
	CStringArray strServerFilePath;

	strServerFilePath.RemoveAll();
	CCommonFuncClass commonfun;
	FilePath_struc.RemoveAll();
	int Z;
	if (IsModel)       //��ģ��
	{
		sql.Format("SELECT * FROM NAsmManageTable where Number=%s",AsmNum);         //����ģ�ͺŲ���
		Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
		{
			AfxMessageBox("��Ҫ�������Ʒģ�Ͳ����ڣ�");
			return FALSE;
		}
		else
		{
			m_db.MoveBegin();
			do {
				uu.SolidID=-1;
				uu.Type="asm";
				uu.MdlCompNum=0;
				uu.SolidName=m_db.getString("SolidFileName");
				uu.SolidFilePath=m_db.getString("SolidFilePlace");
				uu.DrwName=m_db.getString("DrawingFileName");
				uu.DrwFilePath=m_db.getString("DrawingFilePlace");
				uu.MdlNum=AsmNum;
				FilePath_struc.Add(uu);
			} while(m_db.MoveNext());
		}
	}
	else          //�ǲ�Ʒʵ��
	{
		sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%s",AsmNum);     //���ݲ�Ʒ�ò���
		Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
		{
			AfxMessageBox("��Ҫ�������Ʒ�����ڣ�");
			return FALSE;
		}
		else
		{
			m_db.MoveBegin();
			do {
				uu.SolidID=m_db.getInt("PdtNum");
				uu.Type="asm";
				uu.MdlCompNum=0;
				uu.SolidName=m_db.getString("SolidFileName");
				uu.SolidFilePath=m_db.getString("SolidPlace");
				uu.DrwName=m_db.getString("DrawingName");
				uu.DrwFilePath=m_db.getString("DrawingPlace");
				uu.MdlNum.Format("%d",m_db.getInt("MdlNum"));
				if (m_db.getString("PDMState")=="δ�ύ")
				{
					uu.InsCode=m_db.getString("InsCode");
					uu.AddToPDM=FALSE;             //δ�ύ��PDM��
					info->InsCode=uu.InsCode;
				}
				else
				{
					uu.AddToPDM=TRUE;              //�Ѿ��ύ��PDM��
					uu.InsCode=m_db.getString("PDMNum");
					info->PDMCode=uu.InsCode;
				}
			} while(m_db.MoveNext());
			//�õ�ͼ�ţ�����ͼ���Ƿ��Ѿ���PDM�����ˣ�������ˣ�������PDM���Ѿ����˵��ļ�
            if (uu.AddToPDM)        //˵����ͼ��
            {
				BOOL Find;
				if (!prtclass.GetDocumentsFromPDM(Ftp_struc.Database,info,&Find))
				{
					return FALSE;
				}
				else
				{
					if (Find)
					{
						uu.SolidName=info->OldSolidFileName;
						uu.DrwName=info->OldDrwName;
						uu.SolidFilePath=info->OldSolidFilePath;
						uu.DrwFilePath=info->OldDrwFilePath;
					}  
				}
            }
			FilePath_struc.Add(uu);
		}
	}
	if (IsModel)        //����ģ��
	{
		sql.Format("SELECT * FROM NAsmBomTable WHERE AsmNum=%s",AsmNum);
		Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
		{
			AfxMessageBox("��Ҫ�������Ʒģ�Ͳ����ڣ�");
			return FALSE;
		}
		else
		{
			m_db.MoveBegin();
			do {
				uu.SolidID=m_db.getInt("PartNum");
				uu.Type=m_db.getString("Type");
				uu.MdlCompNum=m_db.getInt("ComNum");
				if (IsNew)
				{
					uu.IsInsert="F";
				}
				else
					uu.IsInsert=m_db.getString("IsNew");
				FilePath_struc.Add(uu);
			} while(m_db.MoveNext());
		}
	}
	else              //���ǲ�Ʒ
	{
		sql.Format("SELECT * FROM NProductBOMTable WHERE PdtNum=%d",uu.SolidID);
		Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
		{
			AfxMessageBox("��Ҫ�������Ʒ�����ڣ�");
			return FALSE;
		}
		else
		{
			m_db.MoveBegin();
			do {
				uu.SolidID=m_db.getInt("PrtNum");
				uu.Type=m_db.getString("Type");
				uu.MdlCompNum=m_db.getInt("MdlComNum");
				uu.MdlNum.Format("%d",m_db.getInt("PrtNum"));
				if (IsNew)
				{
					uu.IsInsert="F";
				}
				else
					uu.IsInsert=m_db.getString("IsNew");
				FilePath_struc.Add(uu);
			} while(m_db.MoveNext());
		}
	}
	//���������ID���ҵ�����λ�õ����������,//�õ�ͼ�ţ���������PDM���Ƿ��Ѿ����ˣ�������ˣ�������PDM�е��ļ�
	for (int i=1;i<FilePath_struc.GetSize();i++)
	{
		if (FilePath_struc[i].Type=="prt")
		{
			sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%d",FilePath_struc[i].SolidID);
			Z=m_db.Query(sql);
			if (Z==-1 || Z==0)
			{
				return FALSE;
			}
			else
			{
				m_db.MoveBegin();
				do {
					FilePath_struc[i].SolidName=m_db.getString("SolidFileName");
					FilePath_struc[i].SolidFilePath=m_db.getString("SolidPlace");
					FilePath_struc[i].DrwName=m_db.getString("DrawingName");
					FilePath_struc[i].DrwFilePath=m_db.getString("DrawingPlace");
					FilePath_struc[i].MdlNum.Format("%d",m_db.getInt("MdlNum"));
					FilePath_struc[i].InsCode=m_db.getString("PDMNum");                   //�õ�PDM��ͼ��
					if (m_db.getString("PDMState")=="δ�ύ")
					{
						FilePath_struc[i].AddToPDM=FALSE;             //δ�ύ��PDM��
						FilePath_struc[i].InsCode=m_db.getString("InsCode");
					}
					else
					{
						FilePath_struc[i].AddToPDM=TRUE;              //�Ѿ��ύ��PDM��
					    FilePath_struc[i].InsCode=m_db.getString("PDMNum");
					}
				} while(m_db.MoveNext());
			}
		}
		if (FilePath_struc[i].Type=="asm")
		{
			sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%d",FilePath_struc[i].SolidID);
			Z=m_db.Query(sql);
			if (Z==-1 || Z==0)
			{
				return FALSE;
			}
			else
			{
				m_db.MoveBegin();
				do {
					FilePath_struc[i].SolidName=m_db.getString("SolidFileName");
					FilePath_struc[i].SolidFilePath=m_db.getString("SolidPlace");
					FilePath_struc[i].DrwName=m_db.getString("DrawingName");
					FilePath_struc[i].DrwFilePath=m_db.getString("DrawingPlace");
					FilePath_struc[i].MdlNum.Format("%d",m_db.getInt("MdlNum"));
					if (m_db.getString("PDMState")=="δ�ύ")
					{
						FilePath_struc[i].AddToPDM=FALSE;             //δ�ύ��PDM��
						 FilePath_struc[i].InsCode=m_db.getString("InsCode");
					}
					else
					{
						FilePath_struc[i].AddToPDM=TRUE;              //�Ѿ��ύ��PDM��
					    FilePath_struc[i].InsCode=m_db.getString("PDMNum");
					}
				} while(m_db.MoveNext());
			}
		}
		//�õ�ͼ�ţ�����ͼ���Ƿ��Ѿ���PDM�����ˣ�������ˣ�������PDM���Ѿ����˵��ļ�
		//info->InsCode=FilePath_struc[i].InsCode;
		if (FilePath_struc[i].AddToPDM)        //˵����ͼ��
		{
			BOOL Find;
			info->PDMCode=FilePath_struc[i].InsCode;
			if (!prtclass.GetDocumentsFromPDM(Ftp_struc.Database,info,&Find))
			{
				return FALSE;
			}
			else
			{
				if (Find)
				{
					FilePath_struc[i].SolidName=info->OldSolidFileName;
					FilePath_struc[i].DrwName=info->OldDrwName;
					FilePath_struc[i].SolidFilePath=info->OldSolidFilePath;
					FilePath_struc[i].DrwFilePath=info->OldDrwFilePath;
				}
			}
		}
		                    //
	}
	delete info;            //ȥ����ʱ���ڴ�
	if (!IsNew)
	{
		//CDownLoadDlg dlg;
		//dlg.IsPart=FALSE;
		//dlg.InsCode=AsmNum;
		//if(dlg.DoModal()==IDOK)
		//{
		//	LocalPlace=dlg.m_solidplace;
		//}
		//else
		//{
		//	return FALSE;
		//}
		LocalPlace=Ftp_struc.LocalPath;
	}
	for (int i=0;i<FilePath_struc.GetSize();i++)
	{
		int k=FilePath_struc[i].SolidFilePath.Find(":");
		if (k==-1)		   //˵���ڷ�����
		{
			if (FilePath_struc[i].SolidName.Right(4)==".prt" || FilePath_struc[i].SolidName.Right(4)==".asm")
			{
				str=FilePath_struc[i].SolidFilePath+"/"+FilePath_struc[i].SolidName+".1";
			}
			else
				str=FilePath_struc[i].SolidFilePath+"/"+FilePath_struc[i].SolidName;
			strServerFilePath.Add(str);
			if (FilePath_struc[i].DrwName!="")
			{
				if (FilePath_struc[i].DrwName.Right(4)==".drw")
				{
					str=FilePath_struc[i].DrwFilePath+"/"+FilePath_struc[i].DrwName+".1";
				}
				else
					str=FilePath_struc[i].DrwFilePath+"/"+FilePath_struc[i].DrwName;
				strServerFilePath.Add(str);
			}
			FilePath_struc[i].SolidFilePath=LocalPlace;
			FilePath_struc[i].DrwFilePath=LocalPlace;
			FilePath_struc[i].IsLocal=FALSE;
		}
		else
			FilePath_struc[i].IsLocal=TRUE;
	}
	commonfun.RemoveNameFromNameArray(&strServerFilePath);
	//�жϵ�ǰĿ¼�Ƿ��Ժ��Ʒ������ļ��������㲿����
	if(!IsProductMenberFileAlreadyExist(LocalPlace))
	{
		return FALSE;
	}
	//�ж��ڴ����Ƿ���ͬ���ļ�����
	if (!IsProductMenberFileAlreadyInSession())
	{
		AfxMessageBox("��Ҫ�򿪵Ĳ�Ʒ���Ѿ���ͬ�����ļ����ڴ��У�����ر�����ļ���������ڴ棬�ٴ�");
		return FALSE;
	}
	//���ط������ϵ��ļ�
	if (strServerFilePath.GetSize()>0)
	{
		if(!ftp.DownloadFilestoDirectory(&strServerFilePath,LocalPlace,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
		{
			AfxMessageBox("�����ļ�ʧ��");
			return FALSE;
		}
	}
	//�����ļ�
	for (int i=(int)FilePath_struc.GetSize()-1;i>=0;i--)
	{
        int k;
		if (FilePath_struc[i].SolidName.Right(4)==".prt" || FilePath_struc[i].SolidName.Right(4)==".asm")
		{
			;//str=FilePath_struc[i].SolidFilePath+"/"+FilePath_struc[i].SolidName+".1";
		}
		else
		{
			k=FilePath_struc[i].SolidName.ReverseFind('.');
			if (k!=-1)
			{
				FilePath_struc[i].SolidName.Delete(k,(FilePath_struc[i].SolidName.GetLength()-1));
			}	
		}	
		SolidPath=FilePath_struc[i].SolidFilePath+"\\"+FilePath_struc[i].SolidName;
		ProStringToWstring(path,SolidPath.GetBuffer());
		if (FilePath_struc[i].Type=="prt")
		{
			status=ProMdlLoad(path,PRO_MDL_PART,PRO_B_FALSE,(ProMdl *)solid);
		}
		else
		{
			status=ProMdlLoad(path,PRO_MDL_ASSEMBLY,PRO_B_FALSE,(ProMdl *)solid);
		}
		if (status!=PRO_TK_NO_ERROR)
		{
			str.Format("�Ҳ���%s����ļ�",FilePath_struc[i].SolidName);
			AfxMessageBox(str);
			return FALSE;
		}
		if (FilePath_struc[i].DrwName!="")
		{
			if (FilePath_struc[i].DrwFilePath.Right(4)==".drw")
			{
				;//DrwPath=FilePath_struc[i].DrwFilePath+"\\"+FilePath_struc[i].DrwName;
			}
			else
			{
				k=FilePath_struc[i].DrwName.ReverseFind('.');
				if (k!=-1)
				{
					FilePath_struc[i].DrwName.Delete(k,(FilePath_struc[i].DrwName.GetLength()-k));
				}
			}
			DrwPath=FilePath_struc[i].DrwFilePath+"\\"+FilePath_struc[i].DrwName;
			ProStringToWstring(path,DrwPath.GetBuffer());
			status=ProMdlLoad(path,PRO_MDL_DRAWING,PRO_B_FALSE,&DrwSolid);
			if (status!=PRO_TK_NO_ERROR)
			{
				str.Format("�Ҳ���%s�����άͼ",FilePath_struc[i].DrwName);
				AfxMessageBox(str);
				return FALSE;
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* ��ʾ���ܲ���                                                         */
/************************************************************************/
BOOL CProductClass::ShowPerf(int s,CString Type,CListCtrlEx *list)
{
	CString sql,str;
	//��ʾ���ܲ���
	list->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	list->InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
	list->InsertColumn(1,"���ܲ�����",LVCFMT_LEFT,80,-1);
	list->InsertColumn(2,"���ܲ���",LVCFMT_LEFT,100,-1);
	list->InsertColumn(3,"��ע",LVCFMT_LEFT,100,-1);
	//����ֻ����
	list->SetReadOnlyColumns(0,false);
	list->SetReadOnlyColumns(1,false);
	list->SetReadOnlyColumns(3,false);

	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum!="-1")
	{
		//�����ݿ⣬��ʾ��ģ�͵����ܲ���
		int i=1;
		if (Type=="prt")
		{
			sql.Format("select * from NPartPerfparaTable WHERE PrtNum=%s",((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum);
		}
		else
		{
			sql.Format("select * from NAsmPerformanceTable WHERE AsmNum=%s",((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum);
		}
		int Z=m_db.Query(sql);
		if (Z==-1)
		{
			return FALSE;
		}
		else
			if (m_db.Query(sql)!=0)
			{
				m_db.MoveBegin();
				do {
					str.Format("%d",i);
					list->InsertItem(i-1,str);
					str=m_db.getString("Name");
					list->SetItemText(i-1,1,str);
					str=m_db.getString("Note");
					list->SetItemText(i-1,3,str);
					i++;
				} while(m_db.MoveNext());
			}
		for(int i=0;i<(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).GetSize();i++)
		{
			list->SetItemText(i,2,(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue)[i]);
		}
	}
	return TRUE;
}
/************************************************************************/
/* ��Ҫ�޸��Ѿ���������㲿����Ϣʱ�����ܲ���û�иı䣩                 */
/************************************************************************/
//BOOL CProductClass::UpdateProductMenberToDatabase(int s,CString Type)
//{
//	CCommonFuncClass commonfun;
//	CProductClass pdtclass;
//	CPrtClass prtclass;
//	BOOL IsLocal;
//	if(Type=="prt")
//	{
//		//���ݲ�ͬ�ı���λ�ý��в�ͬ�Ĳ���
//		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="����")
//		{
//			IsLocal=TRUE;
//			if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
//			{
//				AfxMessageBox("�޷����棡");
//				return FALSE;
//			}
//			if (!commonfun.UpdateNewPart(s,FALSE))
//			{
//				AfxMessageBox("����ʧ��");
//				return FALSE;
//			}
//			else
//			{
//				AfxMessageBox("����ɹ�");
//			}
//		}
//		else
//		{
//			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1; //Ϊ�˺����ͨ��
//			if (!commonfun.SavePartToTheServerforPart(s))
//			{
//				AfxMessageBox("����ʧ�ܣ�");
//				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=3;
//				return FALSE;
//			}//�ϴ���������
//			else
//			{
//				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=3;
//				AfxMessageBox("����ɹ�");
//			}
//		}
//	}
//	else
//	{
//		//�����Ʒ,�������ݿ�
//		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="����")
//		{
//			IsLocal=TRUE;
//			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE,IsLocal))
//			{
//				AfxMessageBox("д���ݿ�ʧ�ܣ�");
//				return FALSE;
//			}
//			else
//			{
//				AfxMessageBox("����ɹ�");
//			}
//		}
//		else
//		{
//			IsLocal=FALSE;
//			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,TRUE,IsLocal))
//			{
//				AfxMessageBox("д���ݿ�ʧ�ܣ�");
//				return FALSE;
//			}
//			else
//			{
//				AfxMessageBox("����ɹ�");
//			}
//		}
//	}
//	return TRUE;
//}
/************************************************************************/
/* �ж��������Ƿ��Ѿ�����ˣ����û�����                             */
/************************************************************************/
BOOL CProductClass::ListPartWhichIsNotAudit(CString Type,int s,int *i,CListCtrl *m_list)
{
	CString sql,str;
	int Z;
	if (Type=="prt")
	{
		sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%d AND AuditState='���δͨ��'",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	}
	if (Type=="asm")
	{
		sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%d AND AuditState='���δͨ��'",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	}
	Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		return TRUE;
	}
	else
	{
		str.Format("%d",*i+1);
		m_list->InsertItem(*i,str);
		m_list->SetItemText(*i,1,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName);
		m_list->SetItemText(*i,2,((DetailInfoClass *)p_classarray.GetAt(s))->DrwName);
		str="�ѱ���";
		m_list->SetItemText(*i,3,str);
		if (Type=="prt")
		{
			str="���";
		}
		else
			if (Type=="asm")
			{
				str="���";
			}
			m_list->SetItemText(*i,4,str);
			str=((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum;
			m_list->SetItemText(*i,5,str);
			(*i)++;
			return TRUE;
	}
	return TRUE;
}
/************************************************************************/
/* ����������ȱ��ݵ���ʱ��Ŀ¼���ٸ������ٱ��ݹ���                     */
/************************************************************************/
BOOL CProductClass::NewRename(ProSolid solid,CString NewSolidName,CString *NewDrwName)
{
	//�õ���·��
	ProError     status;
	ProMdldata   oldsdata,oldddata;	 // 1.ԭ������ά����λ�ã�2.��ά��ԭ���� ����λ��
	CString      SolidPlace,DrwPlace;
	ProName      w_name;
	CCommonFuncClass commonfun;
	status=ProMdlDataGet((ProMdl)solid,&oldsdata);
	if (status!=PRO_TK_NO_ERROR) 
		return FALSE;
	//���ж���û�и���
	status=ProMdlNameGet((ProMdl)solid,w_name);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	CString Name;
	NewSolidName.MakeLower();
	if (NewSolidName.Right(4)==".prt" || NewSolidName.Right(4)==".asm")
	{
		int k=NewSolidName.ReverseFind('.');					//ȥ����׺
		if (k!=-1)
		{
			Name=NewSolidName.Left(k);
		}
		
	}
	else 
		Name=NewSolidName;
	////���жϵ�ǰ���Ƿ�����ͬ�ļ������ļ�
	SolidPlace=CString(oldsdata.device)+":"+CString(oldsdata.path);
    int	k=SolidPlace.ReverseFind('\\');
	SolidPlace=SolidPlace.Left(k);

	if (Name.MakeLower()!=CString(oldsdata.name).MakeLower())		//û�и���
	{
		if(!commonfun.IsFileExist(SolidPlace,"",NewSolidName,*NewDrwName))
		{
			return FALSE;
		}
	}
	//�õ���ʱ·��
	CFtpFile ftp;
	CString FullLocalPath;
	TCHAR TempPath[MAX_PATH];
	GetTempPath(MAX_PATH,TempPath);
	FullLocalPath=CString(TempPath)+"zjueducn";
	ftp.RemoveDireAndFile(FullLocalPath);
	//������ά
	CString  temppath,path;
	CFileFind ff;
	ProMdldata p_mdldata;
	BOOL a=ff.FindFile(FullLocalPath);
	if (!a)
	{
		if(::CreateDirectory(FullLocalPath,NULL)==0)
			return FALSE;
	}
	temppath=FullLocalPath;
	status=ProMdlDataGet((ProMdl)solid,&p_mdldata);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	path=temppath+"\\";
	ProStringToWstring(p_mdldata.path,path.GetBuffer());
	status=ProMdlBackup((ProMdl)solid,&p_mdldata);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	//���ݶ�ά
	DrawingList.RemoveAll();
	GetSolidDrawing(solid,&DrawingList);
	if (DrawingList.GetSize()==0)
	{
		*NewDrwName="";
	}
	for (int i=0;i<DrawingList.GetSize();i++)
	{
		status=ProMdlDataGet((ProMdl)DrawingList[i],&oldddata);
		if (status!=PRO_TK_NO_ERROR) 
			return FALSE;
		DrwPlace=CString(oldddata.device)+":"+CString(oldddata.path);
		k=DrwPlace.ReverseFind('\\');
		DrwPlace=DrwPlace.Left(k);
        
		if ((*NewDrwName).Right(4)==".drw")
		{
			k=(*NewDrwName).ReverseFind('.');					//ȥ����׺
			if (k!=-1)
			{
				Name=(*NewDrwName).Left(k);
			}
		}
		else 
			Name=*NewDrwName;
		if (Name.MakeLower()!=CString(oldddata.name).MakeLower())		//û�и���
		{
			if(!commonfun.IsFileExist("",DrwPlace,"",*NewDrwName))
			{
				return FALSE;
			}
		}
		status=ProMdlDataGet((ProMdl)DrawingList[i],&p_mdldata);
		if (status!=PRO_TK_NO_ERROR) 
			return FALSE;
		ProStringToWstring(p_mdldata.path,path.GetBuffer());
		status=ProMdlBackup((ProMdl)DrawingList[i],&p_mdldata);
		if (status!=PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
	}
	//����
	if (!Rename(solid,NewSolidName,*NewDrwName))
	{
		return FALSE;
	}
	//�ٱ��ݵ�ԭ����·��
	status=ProMdlDataGet((ProMdl)solid,&p_mdldata);              //�õ����ݺ����ʱ·��
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	temppath.Format("%s:%s",CString(oldsdata.device),temppath=CString(oldsdata.path));
	ProStringToWstring(p_mdldata.path,temppath.GetBuffer());
	status=ProMdlBackup((ProMdl)solid,&p_mdldata);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	DrawingList.RemoveAll();
	GetSolidDrawing(solid,&DrawingList);
	for (int i=0;i<DrawingList.GetSize();i++)
	{
		status=ProMdlDataGet((ProMdl)DrawingList[i],&p_mdldata);              //�õ����ݺ����ʱ·��
		if (status!=PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
		temppath.Format("%s:%s",CString(oldddata.device),temppath=CString(oldddata.path));
		ProStringToWstring(p_mdldata.path,temppath.GetBuffer());
		status=ProMdlBackup((ProMdl)DrawingList[i],&p_mdldata);
		if (status!=PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* �޸��ļ���                                                           */
/************************************************************************/
BOOL CProductClass::Rename(ProSolid solid,CString SolidName,CString DrwName)
{
	ProName w_name;
	ProError status;
	CString Name;
	CCommonFuncClass commonfun;
	int k=SolidName.Find(".");					//ȥ����׺
	if (k!=-1)
	{
		Name=SolidName.Left(k);
	}
	else 
		Name=SolidName;
	ProStringToWstring(w_name,Name.GetBuffer());
	if (commonfun.IsNameNeedAlter(solid,Name))
	{
		status=ProMdlRename((ProMdl)solid,w_name);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("�޸����ֲ��ɹ�");
			return FALSE;
		}
	}
	//�õ���άͼ�ľ��
	DrawingList.RemoveAll();
	GetSolidDrawing((ProSolid)solid,&DrawingList);
	for (int i=0;i<DrawingList.GetSize();i++)
	{
		k=(DrwName).Find(".");					//ȥ����׺
		if (k!=-1)
		{
			Name=(DrwName).Left(k);
		}
		else 
			Name=DrwName;
		ProStringToWstring(w_name,Name.GetBuffer());
		if (commonfun.IsNameNeedAlter((ProSolid)DrawingList[i],Name))
		{
			status=ProMdlRename((ProMdl)DrawingList[i],w_name);
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
		}
		else
		{
			return TRUE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* �ж�Ҫ���صĲ�Ʒ�Ƿ��ڵ�ǰĿ¼�Ѿ��д�����                           */
/************************************************************************/
BOOL CProductClass::IsProductMenberFileAlreadyExist(CString LocalPlace)
{
	//���ж��Ƿ��ڷ�����
	CCommonFuncClass commonfun;
	for (int i=0;i<FilePath_struc.GetSize();i++)
	{                   
		if (!FilePath_struc[i].IsLocal)					  //˵���ڷ�������
		{
            if (!commonfun.IsFileExist(LocalPlace,LocalPlace,FilePath_struc[i].SolidName,FilePath_struc[i].DrwName))
            {
				return FALSE;
            }
		}
	}
	return TRUE;
}
/************************************************************************/
/* �ж��ڴ����Ƿ������в�Ʒ���ļ�,FALSE˵���д���                       */
/************************************************************************/
BOOL CProductClass::IsProductMenberFileAlreadyInSession()
{
	CCommonFuncClass commonfun;
	CArray <ProSolid,ProSolid> p_arraylist;
	CString Name;
	int k;
	if (!commonfun.GetAlreadyInTheSession(&p_arraylist))
	{
		return FALSE;
	}
	for (int i=0;i<FilePath_struc.GetSize();i++)
	{   
		Name=FilePath_struc[i].SolidName;
		//�ж��ļ����Ƿ��а汾��׺���������ȥ���汾��׺
		if (Name.Right(4)==".prt" || Name.Right(4)==".asm")
		{
			;
		}
		else
		{
		    k=Name.ReverseFind('.');
			if (k!=-1)
			{
				Name.Delete(k,(Name.GetLength()-k));
			}	
		}
		if (commonfun.IsAlreadyInTheSession(Name,&p_arraylist))
		{
			return FALSE;
		}
		//�ж��ļ����Ƿ��а汾��׺���������ȥ���汾��׺
		Name=FilePath_struc[i].DrwName;
		if (Name.Right(4)==".drw")
		{
			;
		}
		else
		{
			k=Name.ReverseFind('.');
			if (k!=-1)
			{
				Name.Delete(k,(Name.GetLength()-k));
			}	
		}
		if (commonfun.IsAlreadyInTheSession(Name,&p_arraylist))
		{
			return FALSE;
		}
	}
	return TRUE;
}
///************************************************************************/
///* �ж��Ƿ������������,�������                                        */
///************************************************************************/
BOOL CProductClass::FindProductMenberForProductWhichIsModify(ProSolid productsolid,BOOL New)
{
	CCompute compute;
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CString str;
	JuBing_struc.RemoveAll();
	if (!compute.ObtainJuBing(productsolid))
	{
		AfxMessageBox("�޷��õ���Ʒ�㲿���ľ��");
		return FALSE;
	}
	commonfun.IsHandleAlreadyHave();
	commonfun.ChangeJuBingToJuBingTemp();
	for (int i=(int)JuBing_struc_temp.GetSize()-1;i>=1;i--)
	{
		int s=commonfun.IsBasicInfoHave(JuBing_struc_temp[i].solid);
		if (s==-1)
		{
			str.Format("�޷��ҵ�%s",JuBing_struc_temp[i].Name);
			AfxMessageBox(str);
			return FALSE;
		}
		if(JuBing_struc_temp[i].Type=="prt" && ((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum!="-1")
		{
			//�ж�����Ƿ��Ѿ��޸�
			if (pdtclass.IsPartModify(JuBing_struc_temp[i].solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum,((DetailInfoClass *)p_classarray.GetAt(s))->SolidID))
			{
				//�����Ի��������޸ģ���Ҫ�ж��Ƿ�ԭ���е�
				if (!pdtclass.AffirmIsOld(JuBing_struc_temp[i].Type,s,New))						 //˵�����������
				{
					return FALSE;
				}
			}
			else
				((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
		}
		else
			if (JuBing_struc_temp[i].Type=="asm" && ((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum!="-1" && productsolid!=((DetailInfoClass *)p_classarray.GetAt(s))->solid)		  
				//ֻ�жϲ��������жϲ�Ʒ
			{
				//�жϲ����Ƿ��Ѿ��޸�
				if (pdtclass.IsAsmModify(JuBing_struc_temp[i].solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum,((DetailInfoClass *)p_classarray.GetAt(s))->SolidID))
				{
					//�����Ի��������޸ģ���Ҫ�ж��Ƿ�ԭ���е�
					if (!pdtclass.AffirmIsOld(JuBing_struc_temp[i].Type,s,New))						 //˵�����������
					{
						return FALSE;
					}
				}
			}
			else
				((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
	}
	return TRUE;
}
/************************************************************************/
/* ���²��벿�������                                                   */
/************************************************************************/
BOOL CProductClass::InsertProductMenberForProduct(ProSolid productsolid)
{
	ProError     status;
	ProMdldata   oldsdata,oldddata;	 // 1.ԭ������ά����λ�ã�2.��ά��ԭ���� ����λ��
	CString      SolidPlace,DrwPlace,str;
	CPrtClass    prtclass;
	CProductClass pdtclass;
	CCommonFuncClass commonfun;
	CCompute compute;
	int i;
	JuBing_struc.RemoveAll();
	if (!compute.ObtainJuBing(productsolid))
	{
		AfxMessageBox("�޷��õ���Ʒ�㲿���ľ��");
		return FALSE;
	}
	commonfun.IsHandleAlreadyHave();
	m_db.m_pConnection->Close();
	m_db.ReOpen();
	try
	{
		m_db.m_pConnection->BeginTrans();
		for (i=(int)JuBing_struc.GetSize()-1;i>=1;i--)
		{
			int s=commonfun.IsBasicInfoHave(JuBing_struc[i].solid);
			if (s==-1)
			{
				m_db.m_pConnection->RollbackTrans();
				return FALSE;
			}
			if (!((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved)
			{
				status=ProMdlDataGet((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid,&oldsdata);
				if (status!=PRO_TK_NO_ERROR)
				{
					m_db.m_pConnection->RollbackTrans();
					return FALSE;
				}
				////���жϵ�ǰ���Ƿ�����ͬ�ļ������ļ�
				SolidPlace=CString(oldsdata.device)+":"+CString(oldsdata.path);
				int k=SolidPlace.ReverseFind('\\');
				SolidPlace=SolidPlace.Left(k);
				str=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
				((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
				SolidPlace=str;
				DrawingList.RemoveAll();
				GetSolidDrawing(((DetailInfoClass *)p_classarray.GetAt(s))->solid,&DrawingList);
				if (DrawingList.GetSize()==0)
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->DrwName="";
				}
				for (int j=0;j<DrawingList.GetSize();j++)
				{
					status=ProMdlDataGet((ProMdl)DrawingList[j],&oldddata);
					if (status!=PRO_TK_NO_ERROR) 
						return FALSE;
					DrwPlace=CString(oldddata.device)+":"+CString(oldddata.path);
					k=DrwPlace.ReverseFind('\\');
					DrwPlace=DrwPlace.Left(k);
					str=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
					((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
					DrwPlace=str;
				}
				//���Ҫ�²�����������
				if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName.Find("--")==-1)
				{
					if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)
					{
						if (!RenameAddChar(JuBing_struc[i].solid,s,JuBing_struc[i].Type))
						{
							//��ԭԭ����λ��
							((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
							((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
							return FALSE;
						}
						((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode+"--";
					}
				}
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="����";
				((DetailInfoClass *)p_classarray.GetAt(s))->DesignState="δ���";
				((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime="";
				((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="���δͨ��";
				((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime="";
				((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="δ�ύ";
				if (JuBing_struc[i].Type=="prt")
				{		//Insert���ݿ�
					if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
					{
						if (!commonfun.InsertNewPart(s,FALSE))
						{
							m_db.m_pConnection->RollbackTrans();
							//��ԭԭ����λ��
							((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
							((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
							return FALSE;
						}
						else
						{
							((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
							((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
							((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
						}
					}
					if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)
					{
						if (!commonfun.UpdateNewPart(s,FALSE))
						{
							m_db.m_pConnection->RollbackTrans();
							((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=2;
							//��ԭԭ����λ��
							((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
							((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
							return FALSE;
						}
						else
						{
							((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
							((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
						}
					}
				}
				else
					if (JuBing_struc[i].Type=="asm")
					{
						if (!pdtclass.WriteAsmToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE,TRUE))
						{
							m_db.m_pConnection->RollbackTrans();
							//��ԭԭ����λ��
							((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
							((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
							return FALSE;
						}//����
						else
						{
							((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
							((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
							if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
							{
								((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
							}
						}
					}
					else
					{
						m_db.m_pConnection->RollbackTrans();
						//��ԭԭ����λ��
						((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
						((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
						return FALSE;
					}
					//��ԭԭ����λ��
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
					((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
			}
		}
		m_db.m_pConnection->CommitTrans();
	}
	catch (_com_error e)
	{
		m_db.m_pConnection->RollbackTrans();
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* ���²��벿�������,�����ڲ��������ﴦ��                              */
/************************************************************************/
BOOL CProductClass::InsertAsmMenberForProduct(ProSolid productsolid)
{
	ProError     status;
	ProMdldata   oldsdata,oldddata;	 // 1.ԭ������ά����λ�ã�2.��ά��ԭ���� ����λ��
	CString      SolidPlace,DrwPlace,str;
	CPrtClass    prtclass;
	CProductClass pdtclass;
	CCommonFuncClass commonfun;
	CCompute compute;
	int i;
	JuBing_struc.RemoveAll();
	if (!compute.ObtainJuBing(productsolid))
	{
		AfxMessageBox("�޷��õ���Ʒ�㲿���ľ��");
		return FALSE;
	}
    commonfun.IsHandleAlreadyHave();
	for (i=(int)JuBing_struc.GetSize()-1;i>=1;i--)
	{
		int s=commonfun.IsBasicInfoHave(JuBing_struc[i].solid);
		if (s==-1)
		{
			return FALSE;
		}
		if (!((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved)
		{
			status=ProMdlDataGet((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid,&oldsdata);
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
			////���жϵ�ǰ���Ƿ�����ͬ�ļ������ļ�
			SolidPlace=CString(oldsdata.device)+":"+CString(oldsdata.path);
			int k=SolidPlace.ReverseFind('\\');
			SolidPlace=SolidPlace.Left(k);
			str=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;        //����ԭ���ı���λ��
			((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
			SolidPlace=str;                                                       
			DrawingList.RemoveAll();
			GetSolidDrawing(((DetailInfoClass *)p_classarray.GetAt(s))->solid,&DrawingList);
			if (DrawingList.GetSize()==0)
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->DrwName="";
			}
			for (int j=0;j<DrawingList.GetSize();j++)
			{
				status=ProMdlDataGet((ProMdl)DrawingList[j],&oldddata);
				if (status!=PRO_TK_NO_ERROR) 
					return FALSE;
				DrwPlace=CString(oldddata.device)+":"+CString(oldddata.path);
				k=DrwPlace.ReverseFind('\\');
				DrwPlace=DrwPlace.Left(k);
				str=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
				((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
				DrwPlace=str;
			}
			//���Ҫ�²�����������
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)
			{
				if (!RenameAddChar(JuBing_struc[i].solid,s,JuBing_struc[i].Type))
				{
					//��ԭԭ����λ��
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
					((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
					return FALSE;
				}
				else
				((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode+"--";
			}
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="����";
			((DetailInfoClass *)p_classarray.GetAt(s))->DesignState="δ���";
			((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime="";
			((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="���δͨ��";
			((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime="";
			((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="δ�ύ";
			if (JuBing_struc[i].Type=="prt")
			{		//Insert���ݿ�
				if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
				{
					if (!commonfun.InsertNewPart(s,FALSE))
					{
						//��ԭԭ����λ��
						((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
						((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
						return FALSE;
					}
					else
					{
						((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
						((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
						((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
					}
				}
				if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)
				{
					if (!prtclass.UpdatePart(s,FALSE))
					{
						((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=2;
						//��ԭԭ����λ��
						((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
						((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
						return FALSE;
					}
					else
					{
						((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
						((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
					}
				}
			}
			else
				if (JuBing_struc[i].Type=="asm")
				{
					if (!pdtclass.WriteAsmToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE,TRUE))
					{
						//��ԭԭ����λ��
						((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
						((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
						return FALSE;
					}//����
					else
					{
						((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
						((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
						if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
						{
							((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
						}
					}
				}
				else
				{
					//��ԭԭ����λ��
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
					((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
					return FALSE;
				}
		//��ԭԭ����λ��
		((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
		((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
		}
	}
	return TRUE;
}
/************************************************************************/
/* �ж��Ƿ���˹�                                                       */
/************************************************************************/
BOOL CProductClass::IsAlreadyAudit(int SolidID,BOOL *IsAudit)
{
	CString sql,str;
	sql.Format("SELECT * FROM  NProductInsInfoTable where PdtNum=%d",SolidID);
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
/* �ж�����Ƿ񱻲�Ʒ����                                               */
/************************************************************************/
BOOL CProductClass::IsUsedByProduct(int s,BOOL *BeUsed,CString Type)
{
	CString sql,str;
	CCommonFuncClass commonfun;
	CStringArray PdtArray;
	PdtArray.RemoveAll();
	ProSolid ProductSolid;
	ProductSolid=commonfun.ObtainTheTopSolid(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
	if (ProductSolid==NULL)
	{
		return FALSE;
	}
	else
		if (ProductSolid==((DetailInfoClass *)p_classarray.GetAt(s))->solid)		 //˵�� �ǲ�Ʒ/�������
		{
			sql.Format("SELECT * FROM NProductBOMTable WHERE PrtNum=%d AND Type='%s'",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,Type);
			int Z=m_db.Query(sql);
			if (Z==-1)
			{
				return FALSE;
			}
			if (Z==0)
			{
				*BeUsed=FALSE;
				return TRUE;
			}
			else
				*BeUsed=TRUE;
		}
		else			//˵�����㲿��
		{
			sql.Format("SELECT * FROM NProductBOMTable WHERE PrtNum=%d AND Type='%s'",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,Type);
			int Z=m_db.Query(sql);
			if (Z==-1)
			{
				return FALSE;
			}
			if (Z<=1)
			{
				*BeUsed=FALSE;
				return TRUE;
			}
			else  //���ж϶������Ƿ�ͬһ����Ʒ��
			{
				//�õ����е�������
				m_db.MoveBegin();
				do {
					str.Format("%d",m_db.getInt("PdtNum"));
					PdtArray.Add(str);
				} while(m_db.MoveNext());
				//ȥ���ظ��Ĳ�Ʒ���
				commonfun.RemoveNameFromNameArray(&PdtArray);
				BOOL Yes;
				if (!IsUsedByTheSameProduct(&PdtArray,&Yes))
				{
					return FALSE;
				}
				else
				{
					if (Yes)
					{
						*BeUsed=FALSE;
					}
					else
						*BeUsed=TRUE;
				}
			}
		}
		return TRUE;
}
/************************************************************************/
/* �ж��㲿���Ƿ�ͬһ����Ʒ��                                         */
/************************************************************************/
BOOL CProductClass::IsUsedByTheSameProduct(CStringArray *PdtArray,BOOL *Yes)
{
	CString sql;
	BOOL IsUsed=FALSE;
	for(int i=0;i<PdtArray->GetSize();i++) 
	{
		sql.Format("SELECT * FROM NProductBOMTable WHERE PrtNum=%s AND Type='asm'",(*PdtArray)[i]);
		int Z=m_db.Query(sql);
		if (Z==-1)
		{
			return FALSE;
		}
		if (Z>0)
		{
			IsUsed=TRUE;
			PdtArray->RemoveAt(i);
			break;
		}
	}
	if (IsUsed)
	{
		if(!IsUsedByTheSameProduct(PdtArray,Yes))
		{
			return FALSE;
		}
	}
	else
	{
		if(PdtArray->GetSize()==0)
			return FALSE;
		if (PdtArray->GetSize()==1)
		{
			*Yes=TRUE; 
		}
		else
			*Yes=FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* �����㲿����ͬʱ���Ѳ�ƷҲ����һ��                                   */
/************************************************************************/
BOOL CProductClass::SaveBothPartAndAsm(int s,CString Type,BOOL New) //1.Ҫ���µ��㲿����2�����ͣ���������ǲ��� 3���ǲ��뻹���滻
{
	CCommonFuncClass commonfun;
	CString SavePlace;
	CProductClass pdtclass;
	ProSolid ProductSolid;
	BOOL IsLocal;
	//�õ������Ʒ�ľ��
	ProductSolid=commonfun.ObtainTheTopSolid(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
	if (ProductSolid==NULL)
	{
		return FALSE;
	}
	CArray <ProSolid,ProSolid> asmarray;
	if (!commonfun.GetAllAsmContainCurrentMdl(ProductSolid,((DetailInfoClass *)p_classarray.GetAt(s))->solid,&asmarray))
	{
		return FALSE;
	}
	m_db.m_pConnection->Close();
	m_db.ReOpen();
	try
	{
		m_db.m_pConnection->BeginTrans();
		if (Type=="prt" && (!(New)))   //�������
		{
            ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath.Find(":")!=-1)
			{
				//���浽����
				IsLocal=TRUE;
				//if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				//{
				//	AfxMessageBox("�޷����棡");
				//	return FALSE;
				//}
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="����";
				if (!commonfun.UpdateNewPart(s,FALSE))
				{
					m_db.m_pConnection->RollbackTrans();
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
					return FALSE;
				}
				else
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
                    ((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
				}
			}
			else   //���浽������
			{
				IsLocal=FALSE;
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="������";
				if(!commonfun.SavePartToTheServerforProduct(s))
				{
					m_db.m_pConnection->RollbackTrans();
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
                   return FALSE;
				}
				else
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
					((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
				}
			}
		}
		if (Type=="prt" && New)           //����½�
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath.Find(":")!=-1)
			{
				IsLocal=TRUE;
				//if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				//{
				//	AfxMessageBox("�޷����棡");
				//	return FALSE;
				//}
                ((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="����";
				if (!commonfun.InsertNewPart(s,FALSE))
				{
					m_db.m_pConnection->RollbackTrans();
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
					return FALSE;
				}
				else
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
					 ((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
				}
			}
			else    //���浽������
			{
				IsLocal=FALSE;
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="������";
				if(!commonfun.SavePartToTheServerforProduct(s))
				{
					m_db.m_pConnection->RollbackTrans();
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
					return FALSE;
				}
				else
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
					((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
				}
			}
		}
		//��������
		if (Type=="asm" && (!(New)))
		{
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath.Find(":")!=-1)
			{
				IsLocal=TRUE;
				//if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				//{
				//	AfxMessageBox("�޷����棡");
				//	return FALSE;
				//}
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="����";
				if (!pdtclass.UpdateNewAsm(s,FALSE))
				{
					m_db.m_pConnection->RollbackTrans();
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
					return FALSE;
				}
				else
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
					 ((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
				}
			}
			else
			{
				IsLocal=FALSE;
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="������";
				if (!WriteAsmToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,(!(IsLocal)),IsLocal))
				{
					m_db.m_pConnection->RollbackTrans();
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
					return FALSE;
				}
				else
				{
                    ((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
					 ((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
				}
				//���浽������
			}
		}
		//�����½�
		if (Type=="asm" && New)
		{
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath.Find(":")!=-1)
			{
				IsLocal=TRUE;
				//if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				//{
				//	AfxMessageBox("�޷����棡");
				//	return FALSE;
				//}
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="����";
				if (!pdtclass.InsertNewAsm(s,FALSE))
				{
					m_db.m_pConnection->RollbackTrans();
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
					return FALSE;
				}
				else
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
					 ((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
				}
			}
			else  //���浽������
			{
				IsLocal=FALSE;
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="������";
				if (!WriteAsmToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,(!(IsLocal)),IsLocal))
				{
					m_db.m_pConnection->RollbackTrans();
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
					return FALSE;
				}
				else
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
					 ((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
				}
			}
		}
		//�����������㲿�����ϲ��ļ�
		for (int i=0;i<asmarray.GetSize();i++)
		{
			//��ȡԭ���ı��淽ʽ
			if (ProductSolid==asmarray[i])          //˵���ǲ�Ʒ
			{
                ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;
			}
			s=commonfun.IsBasicInfoHave(asmarray[i]);
			if(s==-1)
			{
				m_db.m_pConnection->RollbackTrans();
				return FALSE;
			}
			else
			{
				int k=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath.Find(":");
				if (k==-1)   //˵���ڷ�����
				{
					IsLocal=FALSE;
					((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="������";
				}
				else
				{
					IsLocal=TRUE;
					((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="����";
				}
			}
			//�õ����淽ʽ
			if(!pdtclass.FindProductMenberForProductWhichIsModify(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE))
			{
				return FALSE;
			}
			//���뻹û�в�����²����
            if (!pdtclass.InsertAsmMenberForProduct(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
            {
				m_db.m_pConnection->RollbackTrans();
				return FALSE;
            }
			int SaveStyle=((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle;

			if ((!((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved))
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
				{
					if (!RenameAddChar(((DetailInfoClass *)p_classarray.GetAt(s))->solid,s,"asm"))
					{
						m_db.m_pConnection->RollbackTrans();
						return FALSE;
					}
					else
						((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode+"--";
				}
				if(!WriteAsmToDatabase(asmarray[i],(!(IsLocal)),IsLocal))
				{
					m_db.m_pConnection->RollbackTrans();
					((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=SaveStyle;
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
					return FALSE;
				}
			}
			else
			{   //�ȱ��滹û�б�����㲿��
				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;
				if(!WriteAsmToDatabase(asmarray[i],(!(IsLocal)),IsLocal))
				{
					m_db.m_pConnection->RollbackTrans();
					((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=SaveStyle;
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
					return FALSE;
				}
			}
			if (ProductSolid!=asmarray[i])          //˵�����ǲ�Ʒ
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=SaveStyle;
			}
			 ((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=TRUE;
		}
		m_db.m_pConnection->CommitTrans();
	}
	catch (_com_error e)
	{
		m_db.m_pConnection->RollbackTrans();
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* ��������Ϻ�ɾ���Ͱ汾                                               */
/************************************************************************/
BOOL  CProductClass::DeleteOldVersion(ProSolid Solid,BOOL SaveSuccess,int SolidVersion,int DrwVersion) // 1.���  2�� �����Ƿ�ɹ� 3������ǰ�ľ��������ʧ����
{
	ProError status;
	ProMdldata  mdldata;
	CFileFind ff;
	status=ProMdlDataGet((ProMdl)Solid,&mdldata);
	if (status=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	CString str,name,path,type,host,device;
	int version;
	name=CString(mdldata.name);
	path=CString(mdldata.path);
	type=CString(mdldata.type);
	host=CString(mdldata.host);
	device=CString(mdldata.device);
	version=mdldata.version;
	CString fullpath,newname;
	if (SaveSuccess)
	{
		for(int i=1;i<version;i++)
		{
			fullpath.Format("%s:%s%s.%s.%d",device,path,name.MakeLower(),type.MakeLower(),i);
			::DeleteFile(fullpath);
		}
		fullpath.Format("%s:%s%s.%s.%d",device,path,name.MakeLower(),type.MakeLower(),version);
		newname.Format("%s:%s%s.%s.%d",device,path,name.MakeLower(),type.MakeLower(),1);
		CFile::Rename(fullpath,newname);
	}
	else
	{
		for(int i=SolidVersion+1;i<version;i++)
		{
			fullpath.Format("%s:%s%s.%s.%d",device,path,name.MakeLower(),type.MakeLower(),i);
			::DeleteFile(fullpath);
		}
	}
	DrawingList.RemoveAll();
	GetSolidDrawing(Solid,&DrawingList);
	for (int i=0;i<DrawingList.GetSize();i++)
	{
		status=ProMdlDataGet(DrawingList[i],&mdldata);
		CString str,name,path,type,host,device;
		int version;
		name=CString(mdldata.name);
		path=CString(mdldata.path);
		type=CString(mdldata.type);
		host=CString(mdldata.host);
		device=CString(mdldata.device);
		version=mdldata.version;
		CString fullpath,newname;
		if (SaveSuccess)
		{
			fullpath.Format("%s:%s%s.%s.%d",device,path,name.MakeLower(),type.MakeLower(),version);
            if(ff.FindFile(fullpath))
			{
				for(int i=1;i<version;i++)
				{
					fullpath.Format("%s:%s%s.%s.%d",device,path,name.MakeLower(),type.MakeLower(),i);
					::DeleteFile(fullpath);
				}
				fullpath.Format("%s:%s%s.%s.%d",device,path,name.MakeLower(),type.MakeLower(),version);
				newname.Format("%s:%s%s.%s.%d",device,path,name.MakeLower(),type.MakeLower(),1);
				CFile::Rename(fullpath,newname);
			}
		}
		else
		{
			for(int i=DrwVersion+1;i<version;i++)
			{
				fullpath.Format("%s:%s%s.%s.%d",device,path,name.MakeLower(),type.MakeLower(),i);
				::DeleteFile(fullpath);
			}
		}
	}
	return TRUE;
}
