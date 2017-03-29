// ProductClass.cpp : 实现文件
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


// CProductClass 成员函数
//1.判断图号文件名是否符合要求
//2.新的产品文件名是否存在
//3.要下载下来的产品模型或者产品实例名是否存在
/************************************************************************/
/* 判断图号，文件名是否符合要求                                         */
/************************************************************************/
BOOL CProductClass::IsInsCodeOk(CString InsCode,CString SolidName,CString DrwName)
{
	CCommonFuncClass commonfun;
	CString sql,str;
	//判断图号 ，文件名的唯一性
	sql.Format("SELECT * FROM NPartInsInfoTable where InsCode='%s'",InsCode);
	if (commonfun.IsHave(sql))
	{
		AfxMessageBox("你的图号已经存在，请修改");
		return FALSE;
	}
	sql.Format("SELECT * FROM NProductInsInfoTable where InsCode='%s'",InsCode);
	if (commonfun.IsHave(sql))
	{
		AfxMessageBox("你的图号已经存在，请修改");
		return FALSE;
	}
	sql.Format("SELECT * FROM NPartInsInfoTable where SolidFileName='%s'",SolidName);
	if (commonfun.IsHave(sql))
	{
		str.Format("文件%s已经存在，请修改",SolidName);
		AfxMessageBox(str);
		return FALSE;
	}
	sql.Format("SELECT * FROM NPartInsInfoTable where DrawingName='%s'",DrwName);
	if (commonfun.IsHave(sql))
	{
		str.Format("文件%s已经存在，请修改",SolidName);
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 激活窗口                                                             */
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
/* 为产品得到所有的零件的信息											*/
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
			if(!GetInfoFromDatabaseBase(sql,i,TRUE,IsNew))				  //是模型
			{
				return FALSE;
			}
		}
		if (FilePath_struc[i].Type=="asm")
		{
			sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%d",FilePath_struc[i].SolidID);
			if(!GetInfoFromDatabaseBase(sql,i,FALSE,IsNew))				  //是新建
			{
				return FALSE;
			}
		}
	}
	if (!IsNew)		   //如果不是新建产品，则读入产品的所有的信息
	{
		if (FilePath_struc[0].Type=="prt")
		{
			sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%d",FilePath_struc[0].SolidID);
			if(!GetInfoFromDatabaseBase(sql,0,TRUE,IsNew))				  //是模型
			{
				return FALSE;
			}
		}
		if (FilePath_struc[0].Type=="asm")
		{
			sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%d",FilePath_struc[0].SolidID);
			if(!GetInfoFromDatabaseBase(sql,0,FALSE,IsNew))				  //是新建
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 是已有零件实例的话                                                   */
/************************************************************************/
BOOL CProductClass::GetInfoFromDatabaseBase(CString sql,int Num,BOOL IsPart,BOOL IsNew)	  //2.模型的零件编号
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
				info->IsInsert=FilePath_struc[Num].IsInsert;          //得到是否是插入过的
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

			//模型的话是当前的位置
			info->SolidFileName=m_db.getString("SolidFileName");
			info->DrwName=m_db.getString("DrawingName");
			info->SolidFilePath=m_db.getString("SolidPlace");
			info->DrwFilePath=m_db.getString("DrawingPlace");

			info->IsLocalOrServer=m_db.getString("LocalState");
			info->PDMCode=m_db.getString("PDMNum");
			info->Note=m_db.getString("Note");
		} while(m_db.MoveNext());

		//把结构和句柄对应起来
		for (int i=0;i<JuBing_struc.GetSize();i++)
		{
			BOOL IsHave=FALSE;
			status=ProMdlNameGet((ProMdl)JuBing_struc[i].solid,w_name);
			if (status!=PRO_TK_NO_ERROR)
			{
				str.Format("无法得到%s的文件名",info->SolidFileName);
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
					AfxMessageBox("得到的零件类型错误");
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
					str.Format("无法对应%s的句柄",info->SolidFileName);
					AfxMessageBox(str);
					return FALSE;
				}
			}
		}
		//得到性能参数
		if (IsPart)
		{
			//得到零件的模型名
			if (info->MdlNum=="-1")
			{
				info->MdlName="无";
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
			//得到模型名
			if (info->MdlNum=="-1")
			{
				info->MdlName="无";
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
/* 写产品数据表，有事物处理，只保存产品本身，要进行事物处理             */
/************************************************************************/
BOOL CProductClass::WriteProductToDatabase(ProSolid solid,BOOL NeedTrans,BOOL IsLocal)
{
	//需要进行事物处理
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
/* 写产品数据表，无事物处理 ，且只对产品更新，在保存零件的同时用        */
/************************************************************************/
BOOL CProductClass::WriteAsmToDatabase(ProSolid solid,BOOL NeedTrans,BOOL IsLocal)
{
	//需要进行事物处理
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
		((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="本地";
		if (!commonfun.SaveProductFile(solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath))
		{
			AfxMessageBox("产品保存失败");
			return FALSE;
		}
	}
	else
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="服务器";
		if (!commonfun.BackUpFile(s))
		{
			AfxMessageBox("产品备份失败");
			return FALSE;
		}
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
	{
		//保存文件
		if (!InsertNewAsm(s,NeedTrans))
		{
			return FALSE;
		}//;插入组件
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)
	{
		//;更新组件
		if (!UpdateNewAsm(s,NeedTrans))
		{
			return FALSE;
		}
	}
	//再写BOM，组成表和组成零件变量表
	//判断这表是否已经纯在
	if (!WriteAsmExtraToDatabase(solid))
	{
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/*更新部件的BOM表，产品清单表，产品可变零部件的可变参数表               */
/************************************************************************/
BOOL CProductClass::WriteAsmExtraToDatabase(ProSolid solid)			//写组件数据库函数，1.组件句柄 
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
/* 插入新组件                                                           */
/************************************************************************/
BOOL CProductClass::InsertNewAsm(int s,BOOL NeedTrans)			//是否需要上传文件到服务器
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
		//插入事物特性表和性能参数表]
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
/* 更新组件数据库的内容                                          */
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
			return FALSE;	 //把主变量读到临时结构中
		}                   
		////把句柄添加到结构中去
		if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
		{
			return FALSE;
		}
		//错了，应该用组件的更新
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
/* 更新产品的性能参数和事物特性表                                       */
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
/* 插入零件的性能参数和事物特性表                                       */
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
/* 插入零件数据库                                                           */
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
/* 新插入BOM,组成表，和产品零件变量表                                   */
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
/* 更新BOM,组成表，和产品零件变量表                                   */
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
/* 判断零件是否改变                                                     */
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
	//里面尺寸值是值是否相等 ，如果相等则标注借用，不相等则提示其改图号
	if(!commonfun.GetDimInfo(MdlNum,sql,(int)Database_struc_temp.GetSize(),&IsHave))
	{
		return FALSE;	
	}
	else
		if (!IsHave)
		{
			return TRUE;    //	已经改变
		}
	return FALSE;    //没有被改变
}
/************************************************************************/
/* 判断零件是否改变                                                     */
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
	//里面尺寸值是值是否相等 ，如果相等则标注借用，不相等则提示其改图号
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
/* 判断是否是已有零件/组件                                              */
/************************************************************************/
BOOL CProductClass::AffirmIsOld(CString Type,int s,BOOL New)
{
	CString sql,str,str1;
	int solidID;
	CPrtClass prtclass;
	CString FinishState;
	BOOL BeUsed;
	CCommonFuncClass commonfun;
	//如果是零件
	if (Type=="prt")
	{
		solidID=commonfun.IsPartExist(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum);
		if (solidID==-1)          //修改了的零件
		{
			//如果这个零部件ID被别的产品用了，要直接用插入
			if (!IsUsedByProduct(s,&BeUsed,Type))
			{
				return FALSE;
			}
			else
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->PDMState=="")
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="未提交";
				}
				if (BeUsed)
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;	   //从新插入零件
					((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
					((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=FALSE;
				}
				else
				{
					if (((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert=="F")	   //说明没有插入过
					{
						((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;	   //从新插入零件
						((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
						((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=FALSE;
					}
					else
					{
						((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=2;	   //替换原来新插入的零件
						((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
						((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=FALSE;
					}
				}
			}
		}
		else
		{
            //根据原来的ID再判断用插入还是
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;		   //用旧零件替换
			((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="F";
			((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
			((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=solidID;
			//得到这个产品/零件的所有信息
			sql.Format("SELECT * FROM NPartInsInfoTable WHERE PrtNum=%d",solidID);
			if (!prtclass.GetInfoFromDatabaseBase(s,sql))
			{
				AfxMessageBox("无法得到替换零件的信息！");
				return FALSE;
			}	
			//需要改名
			if (!NewRename(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,&(((DetailInfoClass *)p_classarray.GetAt(s))->DrwName)))
			{
				AfxMessageBox("改名失败");
				return FALSE;
			}
			((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
		}
	}
	//如果是产品
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
						((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="未提交";
					}
					if (BeUsed)
					{
                          ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;
						  ((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=FALSE;		  
					}
					else
					{
						if (((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert=="F")	   //说明没有插入过
						{
							((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;	   //从新插入零件
							//((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="未提交";
						}
						else
						{
							((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=2;	   //替换原来新插入的零件
							((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
							//((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="未提交";
						}
						((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=FALSE;
					}
				}
			}
			else
			{
				//判断这个组件是否已经审核过
				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;		   //用旧零件替换
				((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
				((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=solidID;
				//得到这个产品/零件的所有信息
				sql.Format("SELECT * FROM NProductInsInfoTable WHERE PdtNum=%d",solidID);
				if (!prtclass.GetInfoFromDatabaseBase(s,sql))
				{
					AfxMessageBox("无法得到替换部件的信息！");
					return FALSE;
				} 
				//需要改名
				if (!NewRename(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,&(((DetailInfoClass *)p_classarray.GetAt(s))->DrwName)))
				{
					AfxMessageBox("改名失败");
					return FALSE;
				}
				((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
			}
		}
		else
			return FALSE;
	//当保存方式是0和2的时候要在原来的名字后面加一个来表示已经修改，需要重新保存
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
		//为要新插入的随机产生一个图号
		if(((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode+"--";
		}
	}
	//}
		return TRUE;
}
/**********************************************************************************************/
/* 根据是否需要保存改名字，来提醒需要重新保存，在原来的名字后面加一个”*“                    */
/**********************************************************************************************/
BOOL CProductClass::RenameAddChar(ProSolid solid,int s,CString Type)   //是否有二维图
{
	//改名字和替换的一样的名字
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
/* 根据不同的方式来更新产品中的零件/部件                                    */
/************************************************************************/
BOOL CProductClass::UpdateProductMenber(ProSolid ProductSolid,int s)   //1.产品的句柄 2.零件的结构位置
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
		//根据不同的保存位置进行不同的操作
		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="本地")
		{
			IsLocal=TRUE;
			//保存文件,写数据库
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
			if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="服务器")
			{
				IsLocal=FALSE;
				if(!WriteProductMenberToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,Pos,IsLocal))
				{
					m_db.m_pConnection->RollbackTrans();
					//删除上传的文件
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
/* 更新产品的零部件                                                     */
/************************************************************************/
BOOL CProductClass::WriteProductMenberToDatabase(ProSolid solid,int Pos,BOOL IsLocal)		 //1.零件或部件句柄，2.产品结构的位置 3.是否保存在本地
{
	CCommonFuncClass commonfun;
	CPrtClass prtclass;
	int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1)				//用以前设计好的零件去替换
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="F";
		if(!UpdateThreeTable(solid,Pos))                          //产品的句柄
		{
			AfxMessageBox("更新产品的BOM表失败");
			return FALSE;
		}
	}
	else
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)			//替换以前插入的这个零件
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;            //为了和零件的更新通用
			if (IsLocal)
			{
				//如果是零件，写数据
				if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="prt")
				{
					if(!commonfun.UpdateNewPart(s,FALSE))
					{
						AfxMessageBox("更新零件失败");
						return FALSE;
					}
				}
				else
				if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="asm")
				{
					if(!UpdateNewAsm(s,FALSE))//替换以前的部件；
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
				//保存文件
				if(!commonfun.SaveFile(solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				{
					AfxMessageBox("保存本地文件失败");
					return FALSE;
				}
			}
			else	  //服务器的
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="prt")
				{
					if(!commonfun.SavePartToTheServerforProduct(s))		 //要判断是本地还是服务器
					{
						AfxMessageBox("保存服务器失败");
						return FALSE;
					}
				}
				else
				if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="asm")
				{
					if(!UpdateNewAsm(s,TRUE))//替换以前的部件；
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
			if(!UpdateThreeTable(solid,Pos))      //产品的句柄
			{
				AfxMessageBox("更新产品的BOM表失败");
				return FALSE;
			}
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=2;           //在把保存方式改为原来的值
		}
		else			   //重新插入新零件
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
		    if (IsLocal)
		    {
				//从新插入一个零件
				if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="prt")
				{
					if(!commonfun.InsertNewPart(s,FALSE))		 //根据判断是本地还是异地
					{
						AfxMessageBox("新增零件失败");
						return FALSE;
					}
				}
				else     //如果是部件
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
				//保存文件
				if(!commonfun.SaveFile(solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				{
					AfxMessageBox("保存本地文件失败");
					return FALSE;
				}
		    }
			else			 //是服务器
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->Type=="prt")
				{
					if(!commonfun.SavePartToTheServerforProduct(s))		 //要判断是本地还是服务器
					{
						AfxMessageBox("保存服务器失败");
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
			if(!UpdateThreeTable(solid,Pos))      //产品的句柄
			{
				AfxMessageBox("更新产品的BOM表失败");
				return FALSE;
			}
		}
		return TRUE;
}
/************************************************************************/
/*更新这个零件所属的部件的文件                                          */
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
			if (k==-1)			 //说明在服务器上
			{
				//上传文件
				if (!commonfun.UpLoadFile(s))
				{
					return FALSE;
				}
			}
			else
			{
				//直接保存文件
				if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				{
					AfxMessageBox("部件保存失败");
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 打开产品文件                                                         */
/************************************************************************/
BOOL CProductClass::OpenProductFile(CString PdtNum)              //产品编号
{
	CCommonFuncClass commonfun;
	CCompute compute;
	CFtpFile ftp;
	CString Name,str;
	ProSolid solid;
	//下载所有三维模型和二维模型
	if (!GetSavePlaceInfo(PdtNum,"",FALSE,FALSE,&solid))            
	{
		return FALSE;
	}
	if (!GetAllPartInfoForAsm(solid,FALSE))         //不是新建
	{
		AfxMessageBox("无法得到这个产品的下属零件的详细信息");
		return FALSE;
	}
	if (!ActivateAsm(solid))
	{
		AfxMessageBox("显示产品失败");
		return FALSE;
	}
	//commonfun.AddMdlInfo(solid);                 //添加产品模型信息
	int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		return FALSE;
	}
	else
	if (!commonfun.SetProductPara(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
	{
		AfxMessageBox("设置参数错误");
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 新建产品文件                                                         */
/************************************************************************/
BOOL CProductClass::NewProductFile(DetailInfoClass *info,int *s,BOOL IsModel)	  //是否是选模型
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
	//下载所有三维模型和二维模型
	if (IsModel)
	{
		//得到产品所有文件的保存位置
		if (!GetSavePlaceInfo(info->MdlNum,info->SolidFilePath,IsModel,TRUE,&solid))
		{
			AfxMessageBox("打开文件失败");
			return FALSE;
		}
	}
	else
	{   
		//新建产品从实例而来
		str.Format("%d",info->OldSolidID);
		if (!GetSavePlaceInfo(str,info->SolidFilePath,IsModel,TRUE,&solid))
		{
			AfxMessageBox("打开文件失败");
			return FALSE;
		}
	}
	info->solid=solid;
	//还要改名
	if (!NewRename(solid,info->SolidFileName,&(info->DrwName)))
	{
		AfxMessageBox("重命名失败");
		return FALSE;
	}
	p_classarray.Add(info);
	*s=(int)p_classarray.GetSize()-1;			 
	if (!GetAllPartInfoForAsm(info->solid,TRUE))
	{
		AfxMessageBox("无法得到这个产品的下属零件的详细信息");
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 得到产品或者模型的保存位置信息                                       */
/************************************************************************/
BOOL CProductClass::GetSavePlaceInfo(CString AsmNum,CString LocalPlace,BOOL IsModel,BOOL IsNew,ProSolid *solid)
//1.模型的话是模型号或者实例的话是产品号，下载文件的保存位置，是否是选模型，是否是新建
{
	CString sql,str,SolidPath,DrwPath;
	CFtpFile ftp;
	FilePath uu;
	ProPath path;
	ProError status;
	ProMdl DrwSolid;
	CPrtClass prtclass;
	DetailInfoClass *info=NULL;     //过度作用，因为prtclass.GetDocumentsFromPDM调用的函数要用到
	info=new DetailInfoClass;
	CStringArray strServerFilePath;

	strServerFilePath.RemoveAll();
	CCommonFuncClass commonfun;
	FilePath_struc.RemoveAll();
	int Z;
	if (IsModel)       //是模型
	{
		sql.Format("SELECT * FROM NAsmManageTable where Number=%s",AsmNum);         //根据模型号查找
		Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
		{
			AfxMessageBox("你要的这个产品模型不存在！");
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
	else          //是产品实例
	{
		sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%s",AsmNum);     //根据产品好查找
		Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
		{
			AfxMessageBox("你要的这个产品不存在！");
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
				if (m_db.getString("PDMState")=="未提交")
				{
					uu.InsCode=m_db.getString("InsCode");
					uu.AddToPDM=FALSE;             //未提交到PDM中
					info->InsCode=uu.InsCode;
				}
				else
				{
					uu.AddToPDM=TRUE;              //已经提交到PDM中
					uu.InsCode=m_db.getString("PDMNum");
					info->PDMCode=uu.InsCode;
				}
			} while(m_db.MoveNext());
			//得到图号，还看图号是否已经在PDM中有了，如果有了，则下载PDM中已经有了的文件
            if (uu.AddToPDM)        //说明有图号
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
	if (IsModel)        //还是模型
	{
		sql.Format("SELECT * FROM NAsmBomTable WHERE AsmNum=%s",AsmNum);
		Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
		{
			AfxMessageBox("你要的这个产品模型不存在！");
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
	else              //还是产品
	{
		sql.Format("SELECT * FROM NProductBOMTable WHERE PdtNum=%d",uu.SolidID);
		Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
		{
			AfxMessageBox("你要的这个产品不存在！");
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
	//根据零件的ID查找到保存位置的零件的名字,//得到图号，并查找在PDM中是否已经有了，如果有了，则下载PDM中的文件
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
					FilePath_struc[i].InsCode=m_db.getString("PDMNum");                   //得到PDM的图号
					if (m_db.getString("PDMState")=="未提交")
					{
						FilePath_struc[i].AddToPDM=FALSE;             //未提交到PDM中
						FilePath_struc[i].InsCode=m_db.getString("InsCode");
					}
					else
					{
						FilePath_struc[i].AddToPDM=TRUE;              //已经提交到PDM中
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
					if (m_db.getString("PDMState")=="未提交")
					{
						FilePath_struc[i].AddToPDM=FALSE;             //未提交到PDM中
						 FilePath_struc[i].InsCode=m_db.getString("InsCode");
					}
					else
					{
						FilePath_struc[i].AddToPDM=TRUE;              //已经提交到PDM中
					    FilePath_struc[i].InsCode=m_db.getString("PDMNum");
					}
				} while(m_db.MoveNext());
			}
		}
		//得到图号，还看图号是否已经在PDM中有了，如果有了，则下载PDM中已经有了的文件
		//info->InsCode=FilePath_struc[i].InsCode;
		if (FilePath_struc[i].AddToPDM)        //说明有图号
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
	delete info;            //去掉临时的内存
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
		if (k==-1)		   //说明在服务器
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
	//判断当前目录是否以后产品的相关文件（包括零部件）
	if(!IsProductMenberFileAlreadyExist(LocalPlace))
	{
		return FALSE;
	}
	//判断内存中是否有同名文件存在
	if (!IsProductMenberFileAlreadyInSession())
	{
		AfxMessageBox("你要打开的产品，已经有同名的文件在内存中，请请关闭这个文件，再清除内存，再打开");
		return FALSE;
	}
	//下载服务器上的文件
	if (strServerFilePath.GetSize()>0)
	{
		if(!ftp.DownloadFilestoDirectory(&strServerFilePath,LocalPlace,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
		{
			AfxMessageBox("下载文件失败");
			return FALSE;
		}
	}
	//上载文件
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
			str.Format("找不到%s这个文件",FilePath_struc[i].SolidName);
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
				str.Format("找不到%s这个二维图",FilePath_struc[i].DrwName);
				AfxMessageBox(str);
				return FALSE;
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 显示性能参数                                                         */
/************************************************************************/
BOOL CProductClass::ShowPerf(int s,CString Type,CListCtrlEx *list)
{
	CString sql,str;
	//显示性能参数
	list->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	list->InsertColumn(0,"序号",LVCFMT_LEFT,40,-1);
	list->InsertColumn(1,"性能参数名",LVCFMT_LEFT,80,-1);
	list->InsertColumn(2,"性能参数",LVCFMT_LEFT,100,-1);
	list->InsertColumn(3,"备注",LVCFMT_LEFT,100,-1);
	//设置只读行
	list->SetReadOnlyColumns(0,false);
	list->SetReadOnlyColumns(1,false);
	list->SetReadOnlyColumns(3,false);

	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum!="-1")
	{
		//读数据库，显示该模型的性能参数
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
/* 需要修改已经保存过的零部件信息时（性能参数没有改变）                 */
/************************************************************************/
//BOOL CProductClass::UpdateProductMenberToDatabase(int s,CString Type)
//{
//	CCommonFuncClass commonfun;
//	CProductClass pdtclass;
//	CPrtClass prtclass;
//	BOOL IsLocal;
//	if(Type=="prt")
//	{
//		//根据不同的保存位置进行不同的操作
//		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="本地")
//		{
//			IsLocal=TRUE;
//			if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
//			{
//				AfxMessageBox("无法保存！");
//				return FALSE;
//			}
//			if (!commonfun.UpdateNewPart(s,FALSE))
//			{
//				AfxMessageBox("保存失败");
//				return FALSE;
//			}
//			else
//			{
//				AfxMessageBox("保存成功");
//			}
//		}
//		else
//		{
//			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1; //为了和零件通用
//			if (!commonfun.SavePartToTheServerforPart(s))
//			{
//				AfxMessageBox("保存失败！");
//				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=3;
//				return FALSE;
//			}//上传到服务器
//			else
//			{
//				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=3;
//				AfxMessageBox("保存成功");
//			}
//		}
//	}
//	else
//	{
//		//插入产品,插入数据库
//		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="本地")
//		{
//			IsLocal=TRUE;
//			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE,IsLocal))
//			{
//				AfxMessageBox("写数据库失败！");
//				return FALSE;
//			}
//			else
//			{
//				AfxMessageBox("保存成功");
//			}
//		}
//		else
//		{
//			IsLocal=FALSE;
//			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,TRUE,IsLocal))
//			{
//				AfxMessageBox("写数据库失败！");
//				return FALSE;
//			}
//			else
//			{
//				AfxMessageBox("保存成功");
//			}
//		}
//	}
//	return TRUE;
//}
/************************************************************************/
/* 判断这个零件是否已经有审核，如果没有审核                             */
/************************************************************************/
BOOL CProductClass::ListPartWhichIsNotAudit(CString Type,int s,int *i,CListCtrl *m_list)
{
	CString sql,str;
	int Z;
	if (Type=="prt")
	{
		sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%d AND AuditState='审核未通过'",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	}
	if (Type=="asm")
	{
		sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%d AND AuditState='审核未通过'",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
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
		str="已保存";
		m_list->SetItemText(*i,3,str);
		if (Type=="prt")
		{
			str="零件";
		}
		else
			if (Type=="asm")
			{
				str="组件";
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
/* 如果改名，先备份到临时的目录，再改名，再备份过来                     */
/************************************************************************/
BOOL CProductClass::NewRename(ProSolid solid,CString NewSolidName,CString *NewDrwName)
{
	//得到旧路径
	ProError     status;
	ProMdldata   oldsdata,oldddata;	 // 1.原来的三维保存位置，2.二维的原来的 保存位置
	CString      SolidPlace,DrwPlace;
	ProName      w_name;
	CCommonFuncClass commonfun;
	status=ProMdlDataGet((ProMdl)solid,&oldsdata);
	if (status!=PRO_TK_NO_ERROR) 
		return FALSE;
	//先判断有没有改名
	status=ProMdlNameGet((ProMdl)solid,w_name);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	CString Name;
	NewSolidName.MakeLower();
	if (NewSolidName.Right(4)==".prt" || NewSolidName.Right(4)==".asm")
	{
		int k=NewSolidName.ReverseFind('.');					//去掉后缀
		if (k!=-1)
		{
			Name=NewSolidName.Left(k);
		}
		
	}
	else 
		Name=NewSolidName;
	////先判断当前的是否有相同文件名的文件
	SolidPlace=CString(oldsdata.device)+":"+CString(oldsdata.path);
    int	k=SolidPlace.ReverseFind('\\');
	SolidPlace=SolidPlace.Left(k);

	if (Name.MakeLower()!=CString(oldsdata.name).MakeLower())		//没有改名
	{
		if(!commonfun.IsFileExist(SolidPlace,"",NewSolidName,*NewDrwName))
		{
			return FALSE;
		}
	}
	//得到临时路径
	CFtpFile ftp;
	CString FullLocalPath;
	TCHAR TempPath[MAX_PATH];
	GetTempPath(MAX_PATH,TempPath);
	FullLocalPath=CString(TempPath)+"zjueducn";
	ftp.RemoveDireAndFile(FullLocalPath);
	//备份三维
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
	//备份二维
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
			k=(*NewDrwName).ReverseFind('.');					//去掉后缀
			if (k!=-1)
			{
				Name=(*NewDrwName).Left(k);
			}
		}
		else 
			Name=*NewDrwName;
		if (Name.MakeLower()!=CString(oldddata.name).MakeLower())		//没有改名
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
	//改名
	if (!Rename(solid,NewSolidName,*NewDrwName))
	{
		return FALSE;
	}
	//再备份到原来的路径
	status=ProMdlDataGet((ProMdl)solid,&p_mdldata);              //得到备份后的临时路径
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
		status=ProMdlDataGet((ProMdl)DrawingList[i],&p_mdldata);              //得到备份后的临时路径
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
/* 修改文件名                                                           */
/************************************************************************/
BOOL CProductClass::Rename(ProSolid solid,CString SolidName,CString DrwName)
{
	ProName w_name;
	ProError status;
	CString Name;
	CCommonFuncClass commonfun;
	int k=SolidName.Find(".");					//去掉后缀
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
			AfxMessageBox("修改名字不成功");
			return FALSE;
		}
	}
	//得到二维图的句柄
	DrawingList.RemoveAll();
	GetSolidDrawing((ProSolid)solid,&DrawingList);
	for (int i=0;i<DrawingList.GetSize();i++)
	{
		k=(DrwName).Find(".");					//去掉后缀
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
/* 判断要下载的产品是否在当前目录已经有存在了                           */
/************************************************************************/
BOOL CProductClass::IsProductMenberFileAlreadyExist(CString LocalPlace)
{
	//先判断是否在服务器
	CCommonFuncClass commonfun;
	for (int i=0;i<FilePath_struc.GetSize();i++)
	{                   
		if (!FilePath_struc[i].IsLocal)					  //说明在服务器上
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
/* 判断内存中是否有已有产品的文件,FALSE说明有存在                       */
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
		//判断文件名是否有版本后缀，如果有则去掉版本后缀
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
		//判断文件名是否有版本后缀，如果有则去掉版本后缀
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
///* 判断是否有新零件产生,有则插入                                        */
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
		AfxMessageBox("无法得到产品零部件的句柄");
		return FALSE;
	}
	commonfun.IsHandleAlreadyHave();
	commonfun.ChangeJuBingToJuBingTemp();
	for (int i=(int)JuBing_struc_temp.GetSize()-1;i>=1;i--)
	{
		int s=commonfun.IsBasicInfoHave(JuBing_struc_temp[i].solid);
		if (s==-1)
		{
			str.Format("无法找到%s",JuBing_struc_temp[i].Name);
			AfxMessageBox(str);
			return FALSE;
		}
		if(JuBing_struc_temp[i].Type=="prt" && ((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum!="-1")
		{
			//判断零件是否已经修改
			if (pdtclass.IsPartModify(JuBing_struc_temp[i].solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum,((DetailInfoClass *)p_classarray.GetAt(s))->SolidID))
			{
				//跳出对话框让其修改，还要判断是否原来有的
				if (!pdtclass.AffirmIsOld(JuBing_struc_temp[i].Type,s,New))						 //说明是已有零件
				{
					return FALSE;
				}
			}
			else
				((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
		}
		else
			if (JuBing_struc_temp[i].Type=="asm" && ((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum!="-1" && productsolid!=((DetailInfoClass *)p_classarray.GetAt(s))->solid)		  
				//只判断部件，不判断产品
			{
				//判断部件是否已经修改
				if (pdtclass.IsAsmModify(JuBing_struc_temp[i].solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum,((DetailInfoClass *)p_classarray.GetAt(s))->SolidID))
				{
					//跳出对话框让其修改，还要判断是否原来有的
					if (!pdtclass.AffirmIsOld(JuBing_struc_temp[i].Type,s,New))						 //说明是已有零件
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
/* 重新插入部件和零件                                                   */
/************************************************************************/
BOOL CProductClass::InsertProductMenberForProduct(ProSolid productsolid)
{
	ProError     status;
	ProMdldata   oldsdata,oldddata;	 // 1.原来的三维保存位置，2.二维的原来的 保存位置
	CString      SolidPlace,DrwPlace,str;
	CPrtClass    prtclass;
	CProductClass pdtclass;
	CCommonFuncClass commonfun;
	CCompute compute;
	int i;
	JuBing_struc.RemoveAll();
	if (!compute.ObtainJuBing(productsolid))
	{
		AfxMessageBox("无法得到产品零部件的句柄");
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
				////先判断当前的是否有相同文件名的文件
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
				//如果要新插入的则改名字
				if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName.Find("--")==-1)
				{
					if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)
					{
						if (!RenameAddChar(JuBing_struc[i].solid,s,JuBing_struc[i].Type))
						{
							//还原原来的位置
							((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
							((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
							return FALSE;
						}
						((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode+"--";
					}
				}
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="本地";
				((DetailInfoClass *)p_classarray.GetAt(s))->DesignState="未完成";
				((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime="";
				((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="审核未通过";
				((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime="";
				((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="未提交";
				if (JuBing_struc[i].Type=="prt")
				{		//Insert数据库
					if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
					{
						if (!commonfun.InsertNewPart(s,FALSE))
						{
							m_db.m_pConnection->RollbackTrans();
							//还原原来的位置
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
							//还原原来的位置
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
							//还原原来的位置
							((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
							((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
							return FALSE;
						}//插入
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
						//还原原来的位置
						((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
						((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
						return FALSE;
					}
					//还原原来的位置
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
/* 重新插入部件和零件,不在内部进行事物处理                              */
/************************************************************************/
BOOL CProductClass::InsertAsmMenberForProduct(ProSolid productsolid)
{
	ProError     status;
	ProMdldata   oldsdata,oldddata;	 // 1.原来的三维保存位置，2.二维的原来的 保存位置
	CString      SolidPlace,DrwPlace,str;
	CPrtClass    prtclass;
	CProductClass pdtclass;
	CCommonFuncClass commonfun;
	CCompute compute;
	int i;
	JuBing_struc.RemoveAll();
	if (!compute.ObtainJuBing(productsolid))
	{
		AfxMessageBox("无法得到产品零部件的句柄");
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
			////先判断当前的是否有相同文件名的文件
			SolidPlace=CString(oldsdata.device)+":"+CString(oldsdata.path);
			int k=SolidPlace.ReverseFind('\\');
			SolidPlace=SolidPlace.Left(k);
			str=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;        //保存原来的保存位置
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
			//如果要新插入的则改名字
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)
			{
				if (!RenameAddChar(JuBing_struc[i].solid,s,JuBing_struc[i].Type))
				{
					//还原原来的位置
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
					((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
					return FALSE;
				}
				else
				((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode+"--";
			}
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="本地";
			((DetailInfoClass *)p_classarray.GetAt(s))->DesignState="未完成";
			((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime="";
			((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="审核未通过";
			((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime="";
			((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="未提交";
			if (JuBing_struc[i].Type=="prt")
			{		//Insert数据库
				if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
				{
					if (!commonfun.InsertNewPart(s,FALSE))
					{
						//还原原来的位置
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
						//还原原来的位置
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
						//还原原来的位置
						((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
						((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
						return FALSE;
					}//插入
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
					//还原原来的位置
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
					((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
					return FALSE;
				}
		//还原原来的位置
		((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=SolidPlace;
		((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=DrwPlace;
		}
	}
	return TRUE;
}
/************************************************************************/
/* 判断是否审核过                                                       */
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
		if (str=="审核通过")
		{
			*IsAudit=TRUE;
		}
		else
			*IsAudit=FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 判断零件是否被产品所用                                               */
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
		if (ProductSolid==((DetailInfoClass *)p_classarray.GetAt(s))->solid)		 //说明 是产品/单个零件
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
		else			//说明是零部件
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
			else  //再判断多个组件是否被同一个产品用
			{
				//得到所有的组件编号
				m_db.MoveBegin();
				do {
					str.Format("%d",m_db.getInt("PdtNum"));
					PdtArray.Add(str);
				} while(m_db.MoveNext());
				//去掉重复的产品标号
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
/* 判断零部件是否被同一个产品用                                         */
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
/* 更新零部件的同时，把产品也更新一把                                   */
/************************************************************************/
BOOL CProductClass::SaveBothPartAndAsm(int s,CString Type,BOOL New) //1.要更新的零部件，2。类型，是零件还是部件 3。是插入还是替换
{
	CCommonFuncClass commonfun;
	CString SavePlace;
	CProductClass pdtclass;
	ProSolid ProductSolid;
	BOOL IsLocal;
	//得到顶层产品的句柄
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
		if (Type=="prt" && (!(New)))   //零件更新
		{
            ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath.Find(":")!=-1)
			{
				//保存到本地
				IsLocal=TRUE;
				//if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				//{
				//	AfxMessageBox("无法保存！");
				//	return FALSE;
				//}
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="本地";
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
			else   //保存到服务器
			{
				IsLocal=FALSE;
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="服务器";
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
		if (Type=="prt" && New)           //零件新建
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath.Find(":")!=-1)
			{
				IsLocal=TRUE;
				//if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				//{
				//	AfxMessageBox("无法保存！");
				//	return FALSE;
				//}
                ((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="本地";
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
			else    //保存到服务器
			{
				IsLocal=FALSE;
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="服务器";
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
		//部件更新
		if (Type=="asm" && (!(New)))
		{
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath.Find(":")!=-1)
			{
				IsLocal=TRUE;
				//if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				//{
				//	AfxMessageBox("无法保存！");
				//	return FALSE;
				//}
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="本地";
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
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="服务器";
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
				//保存到服务器
			}
		}
		//部件新建
		if (Type=="asm" && New)
		{
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath.Find(":")!=-1)
			{
				IsLocal=TRUE;
				//if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
				//{
				//	AfxMessageBox("无法保存！");
				//	return FALSE;
				//}
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="本地";
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
			else  //保存到服务器
			{
				IsLocal=FALSE;
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="服务器";
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
		//保存包含这个零部件的上层文件
		for (int i=0;i<asmarray.GetSize();i++)
		{
			//提取原来的保存方式
			if (ProductSolid==asmarray[i])          //说明是产品
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
				if (k==-1)   //说明在服务器
				{
					IsLocal=FALSE;
					((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="服务器";
				}
				else
				{
					IsLocal=TRUE;
					((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="本地";
				}
			}
			//得到保存方式
			if(!pdtclass.FindProductMenberForProductWhichIsModify(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE))
			{
				return FALSE;
			}
			//插入还没有插入的下层零件
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
			{   //先保存还没有保存的零部件
				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;
				if(!WriteAsmToDatabase(asmarray[i],(!(IsLocal)),IsLocal))
				{
					m_db.m_pConnection->RollbackTrans();
					((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=SaveStyle;
					((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
					return FALSE;
				}
			}
			if (ProductSolid!=asmarray[i])          //说明不是产品
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
/* 当保存完毕后删掉就版本                                               */
/************************************************************************/
BOOL  CProductClass::DeleteOldVersion(ProSolid Solid,BOOL SaveSuccess,int SolidVersion,int DrwVersion) // 1.句柄  2。 保存是否成功 3。保存前的句柄，保存失败用
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
