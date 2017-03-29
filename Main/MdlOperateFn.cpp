#include "stdafx.h"
#include "resource.h"
#include "partPerfparaTableSet.h"
#include "partVariantTableSet.h"
#include "partPicSet.h"
#include "partRelationSet.h"
#include "NPartInsInfoTable.h"
#include "AsmManageTableSet.h"
#include "CAsmPerfSet.h"
#include "CAsmPicSet.h"
#include "CAsmRelSet.h"
#include "CAsmVarSet.h"
#include "NAsmBomTable.h"
#include "NProductInsInfoTable.h"
#include "FtpFile.h"
//判断一个模型是否被删除，如果返回true则是被删除
bool IsMdlDeleted(ProMdl mdl=NULL)
{
	ProError    status;
	ProMdl		*p_model_array;
	int			p_count=0;
	if(mdl==NULL) return true;
	status=ProSessionMdlList(PRO_MDL_PART,&p_model_array,&p_count);
	for (int i=0;i<p_count;i++)
	{
		if(mdl==p_model_array[i]) 
		{
			ProArrayFree((ProArray*)&p_model_array);
			return false;
		}
	}
	ProArrayFree((ProArray*)&p_model_array);
	status=ProSessionMdlList(PRO_MDL_ASSEMBLY,&p_model_array,&p_count);
	for (int i=0;i<p_count;i++)
	{
		if(mdl==p_model_array[i])
		{
			ProArrayFree((ProArray*)&p_model_array);
			return false;
		}
	}
	ProArrayFree((ProArray*)&p_model_array);
	status=ProSessionMdlList(PRO_MDL_DRAWING,&p_model_array,&p_count);
	for (int i=0;i<p_count;i++)
	{
		if(mdl==p_model_array[i])
		{
			ProArrayFree((ProArray*)&p_model_array);
			return false;
		}
	}
	return true;
}
CString GetMdlFullFileName(ProMdl mdl)
{
	return GetMdlPath(mdl)+GetMdlFileName(mdl);
}
CString GetMdlFileName(ProMdl mdl)
{
	ProMdldata	data;
	CString		reValue;
	CString     csType;
	ProMdlType  type;
	if (ProMdlDataGet(mdl,&data)==PRO_TK_NO_ERROR)
	{
		ProMdlTypeGet(mdl,&type);
		if (type==PRO_MDL_PART) {

			csType=_T(".prt");
		}
		else if (type==PRO_MDL_ASSEMBLY) {
			csType=_T(".asm");
		}
		else if(type==PRO_MDL_DRAWING)
		{
			csType=_T(".drw");
		}
		reValue = CString(data.name)+csType;
		return reValue;
	}
	return _T("");
}
CString GetMdlPath(ProMdl mdl)
{
	ProMdldata	data;
	CString		reValue;
	if (ProMdlDataGet(mdl,&data)==PRO_TK_NO_ERROR)
	{
		reValue.Format("%s:%s",CString(data.device),CString(data.path));
		return reValue;
	}
	return _T("");
}
/************************************************************************/
/* 获得当前某新的路径                                                   */
/************************************************************************/
CString GetCurrentMdlPath(void)
{
	ProMdl		mdl;
	ProMdldata	data;
	CString		reValue;
	if (ProMdlCurrentGet(&mdl)==PRO_TK_NO_ERROR)
	{
		if (ProMdlDataGet(mdl,&data)==PRO_TK_NO_ERROR)
		{
			reValue.Format("%s:%s",CString(data.device),CString(data.path));
			return reValue;
		}
	}
	return _T("");
}
/************************************************************************/
/* 判断一个模型是否为另一个模型的元件(可以用于零件)                     */
/************************************************************************/
BOOL IsSolid1InSolid2(ProSolid comp,ProSolid asm_solid)
{
	CArray<ProSolid,ProSolid> solid_list;
	solid_list.Add(asm_solid);
	GetAsmCompHand(asm_solid,&solid_list);
	for (int i=0;i<solid_list.GetCount();i++)
	{
		if(comp==solid_list[i]) return TRUE;
	}
	return FALSE;
}
//判断一个模型是否已经在内存在，要指定类型
bool IsMdlInSession(CString FileName="",ProMdlType type=PRO_MDL_PART,ProMdl * mdl=NULL)
{
	ProMdl *                mdllist;
	int						p_count;
	ProName					tempCheckMdlName;
	CString                 csCheckMdlName;
	CString                 postType;
	ProError                status;
	switch(type) {
	case PRO_MDL_PART:
		postType=".prt";
		break;
	case PRO_MDL_ASSEMBLY:
		postType=".asm";
		break;
	case PRO_MDL_DRAWING:
		postType=".drw";
		break;
	default:
		return false;
	}
	status=ProSessionMdlList(type,&mdllist,&p_count);
	for (int i=0;i<p_count;i++)
	{
		ProMdlNameGet(mdllist[i],tempCheckMdlName);
		csCheckMdlName=CString(tempCheckMdlName);
		
		csCheckMdlName.Format("%s%s",CString(tempCheckMdlName),postType);
		if(csCheckMdlName.MakeLower() == FileName.MakeLower())
		{
			if(mdl!=NULL)
			{
				(*mdl)=mdllist[i];
			}
			ProArrayFree((ProArray*)&mdllist);
			return true;
		}
	}
	ProArrayFree((ProArray*)&mdllist);
	return false;
}
/************************************************************************/
/* 判断当前有没有模型                                                   */
/************************************************************************/
BOOL HaveMdlCurrent()
{
	ProError status;
	ProMdl   mdl;
	status=ProMdlCurrentGet((ProMdl*)&mdl);
	if (status==PRO_TK_NO_ERROR) return TRUE;
	return FALSE;
}
/************************************************************************/
/* 模型的备份工作                                                       */
/************************************************************************/
ProError UsrMdlBackUp(ProMdl mdl,ProMdldata * data)
{
	ProError     status;
	ProMdldata   olddata;
	ProPath      path;
	CString      temp;
	swprintf(path,(data->path));
	status=ProMdlDataGet(mdl,&olddata);
	if (status!=PRO_TK_NO_ERROR) return status;
	(*data)=olddata;
	swprintf(data->path,path);
	status=ProMdlBackup(mdl,data);
	if (status!=PRO_TK_NO_ERROR) return status;
	temp.Format("%s:%s",CString(olddata.device),CString(olddata.path));
	ProStringToWstring(olddata.path,temp.GetBuffer());
	status=ProMdlBackup(mdl,&olddata);
	return status;
}
/************************************************************************/
/* 把文件都导入内存,文件必须是全路径的   
mdllist是动态分配的，所以它的大小必须＆files大小一致*/
/************************************************************************/
bool UsrMdlLoad(CStringArray & files,ProMdl ** mdllist)
{
	ProError status;
	ProPath  name;
	ProMdl   mdl=NULL;
	int      nMdlList=0;
	for (int i=0;i<files.GetCount();i++)
	{
		name[0]='\0';
		ProStringToWstring(name,_T(files[i].GetBuffer()+'\0'));
		status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,&mdl); 
		if (status!=PRO_TK_NO_ERROR)
		{

			for (int j=(int)files.GetCount()-1;j>=0;--j)
			{
				status=ProMdlErase((*mdllist)[j]);
			}
			return false;
		}
		else
		{
			(*mdllist)[i]=mdl;
		}
	}	
	return true;
}
ProMdl GetCurrentMdl()
{
	ProMdl     mdl;
	ProError   status;
	status=ProMdlCurrentGet(&mdl);
	if (status==PRO_TK_NO_ERROR)
		return mdl;
	return NULL;
}
/************************************************************************/
/* 把零件模型的可变参数、性能参数、图片信息、关系式等添加
由这个模型参数的变型零件中												*/
/************************************************************************/
BOOL UsrAddPartMdlOtherInfo(CDatabase * p_db,ProMdl mdl,long PrtNum)
{
	//添加模型的信息，图片信息没有包括
	CpartVariantTableSet    m_partParaInfo(p_db);
	CpartPerfparaTableSet   m_partPerf(p_db);
	CpartRelationSet        m_partRel(p_db);
	CpartPicSet             m_partPic(p_db);
	ProMdlType  mdltype;
	ProMdlTypeGet(mdl,&mdltype);
	//先清楚原来的
	EraseMdlInfo(mdl,ERASE_ALL);
	try
	{
		//添加参数信息
		m_partParaInfo.m_strFilter.Format("[PrtNum]=%ld",PrtNum);
		m_partParaInfo.Open();
		if (!m_partParaInfo.IsBOF())
		{
			m_partParaInfo.MoveFirst();
			while (!m_partParaInfo.IsEOF())
			{
				UsrSWTXTable1 temp;
				temp.id=m_partParaInfo.m_IdNum;
				temp.Name=m_partParaInfo.m_Name;
				temp.isCheck=1;
				temp.Note=m_partParaInfo.m_Note;
				temp.featName=m_partParaInfo.m_ParaFeatName;
				temp.dimSymbol=m_partParaInfo.m_DimSymbol;
				temp.value=m_partParaInfo.m_Value;
				temp.owner=mdl;
				temp.type=m_partParaInfo.m_Type;
				temp.subType=m_partParaInfo.m_SubType;
				temp.DwLimit=m_partParaInfo.m_DwLimit;
				temp.UpLimit=m_partParaInfo.m_UpLimit;
				temp.SetDimPara((ProSolid)mdl);
				p_UsrSWTXTableList.Add(temp);
				m_partParaInfo.MoveNext();
			}
		}
		//添加性能参数信息
		m_partPerf.m_strFilter.Format("[PrtNum]=%ld",PrtNum);
		m_partPerf.Open();
		if (!m_partPerf.IsBOF())
		{
			m_partPerf.MoveFirst();
			while (!m_partPerf.IsEOF())
			{
				UsrPerfParaTable temp_perf;
				temp_perf.isCheck=1;temp_perf.Name=m_partPerf.m_Name;
				temp_perf.owner=mdl;temp_perf.value=m_partPerf.m_Value;
				temp_perf.Note=m_partPerf.m_Note;
				temp_perf.Type=m_partPerf.m_Type;
				temp_perf.MaxValue=m_partPerf.m_MaxVal;
				temp_perf.MinValue=m_partPerf.m_MinVal;
				p_UsrPerfParaTableList.Add(temp_perf);
				m_partPerf.MoveNext();
			}
		}
		//添加关系式信息
		m_partRel.m_strFilter.Format("[PrtNum]=%ld",PrtNum);
		m_partRel.Open();
		if (!m_partRel.IsBOF())
		{
			m_partRel.MoveFirst();
			while (!m_partRel.IsEOF())
			{
				g_RelCollection.AddRel((ProSolid)mdl,m_partRel.m_Relation,m_partRel.m_Note);
				m_partRel.MoveNext();
			}
		}

		m_partPic.m_strFilter.Format("[PrtNum]=%ld",PrtNum);
		m_partPic.Open();
		if (!m_partPic.IsBOF())
		{
			m_partPic.MoveFirst();
			while (!m_partPic.IsEOF())
			{
				UsrPicture temp_pic;
				CString	   error_info;
				CString    temp_full_filename;
				int        tempnum=0;
				CString    mdlName=GetMdlInfo(mdl).Name;
				temp_pic.owner=mdl;
				temp_pic.isCheck=1;
				temp_pic.isMain=m_partPic.m_IsMain;
				temp_pic.fileName=mdlName+".jpg";
				temp_pic.filePath=GetMdlPath(mdl);			
				temp_full_filename=temp_pic.filePath+temp_pic.fileName;
				while (PathFileExistsA(temp_full_filename.GetBuffer()))
				{
					tempnum++;
					temp_pic.fileName.Format("%s_%d.jpg",mdlName,tempnum);
					temp_full_filename=temp_pic.filePath+temp_pic.fileName;
				}
				if (temp_pic.isMain==1)
				{
					SetMdlInfo(mdl)->pic_path=temp_pic.filePath+temp_pic.fileName;
				}
				temp_pic.Note=m_partPic.m_Note;		
				DBOpenPicSaveAsFile(&m_partPic.m_File,temp_pic.filePath+temp_pic.fileName);
				p_UsrPicture.Add(temp_pic);
				m_partPic.MoveNext();
			}
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
	}
	return TRUE;
}
/************************************************************************/
/* 检查当前模型
   reValue*/
/************************************************************************/
UsrCheckCurrentMdlState  UsrCheckCurrentMdl(ProMdlType mdltype,CString filename/*包括扩展符号*/,long num,ProMdl* mdl)
{
	ProName    name;
	ProMdl     tempmdl;
	if(filename.IsEmpty() || filename.GetLength()<=4) return CHECK_NO_MDL;
	ProStringToWstring(name,filename.Left(filename.GetLength()-4).GetBuffer());
	switch(mdltype) {
	case PRO_MDL_PART:
		if(ProMdlInit(name,PRO_MDL_PART,&tempmdl)==PRO_TK_NO_ERROR)
		{
		    if (GetMdlInfo(tempmdl).Number==num)
		    {
				if(mdl!=NULL) (*mdl=tempmdl);
				return CHECK_NAME_NUM_SAME;
		    }
			else
			{
				if(mdl!=NULL) (*mdl=tempmdl);
				return CHECK_SNAME_NNUM;
			}
		}
		else
		{
			return CHECK_NO_MDL;
		}
		break;
	case PRO_MDL_ASSEMBLY:
		if(ProMdlInit(name,PRO_MDL_ASSEMBLY,&tempmdl)==PRO_TK_NO_ERROR)
		{
			if (GetMdlInfo(tempmdl).Number==num)
			{
				if(mdl!=NULL) (*mdl=tempmdl);
				return CHECK_NAME_NUM_SAME;
			}
			else
			{
				if(mdl!=NULL) (*mdl=tempmdl);
				return CHECK_SNAME_NNUM;
			}
		}
		else
		{
			return CHECK_NO_MDL;
		}
		break;
	default:
		break;
	}
    return CHECK_NO_MDL;
}
BOOL  UsrDelPartMdl(CDatabase * p_db,long num)
{
	CPartMdlRecordSet					m_set(p_db);
	CNPartInsInfoTable					m_set2(p_db);
	CString								message;
	CString								strSolidFileDir;
	CFtpFile							m_ftp;
	//1.判断是否有实例
	m_set2.m_strFilter.Format("[MdlNum]=%ld",num);
	m_set2.Open();
	if (!m_set2.IsBOF())
	{
		AfxMessageBox("该模型下有零件实例，不能删除");
		return FALSE;
	}

	//2.判断选种的模型是否存在
	m_set.m_strFilter.Format("[Number]=%ld",num);
	m_set.Open();
	if (!m_set.IsBOF())
	{
		message.Format("确定要删除模型[%s]吗？",_T(m_set.m_Name));
		if(AfxMessageBox(message,MB_OK)!=IDOK)
		{
			return FALSE;
		}			
		try
		{
			m_set.m_pDatabase->BeginTrans();
			strSolidFileDir=m_set.m_SolidFilePlace;
			if (strSolidFileDir.Find("/零件模型/",0)==-1)
			{
				AfxMessageBox("原有模型信息被破坏");
				m_set.m_pDatabase->Rollback();
				return FALSE;
			}
			m_set.Delete();            //删除其下所有相关信息，通过数据库的触发器进行
			m_set.Requery();
			if (!m_ftp.DeleteDirAndFile(strSolidFileDir,myFtp))
			{
				AfxMessageBox("从文件服务器删除文件失败！");
				m_set.m_pDatabase->Rollback();
				return FALSE;
			}
			m_set.m_pDatabase->CommitTrans();			
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			m_set.m_pDatabase->Rollback();
			return FALSE;
		}
	}
	return TRUE;
}
BOOL  UsrDelAsmMdl(CDatabase * p_db,long num)
{
	CAsmManageTableSet					m_set(p_db);
	CNProductInsInfoTable				m_set2(p_db);
	CString								message;
	CString								strSolidFileDir;
	CFtpFile							m_ftp;
	//1.判断是否有实例
	m_set2.m_strFilter.Format("[MdlNum]=%ld",num);
	m_set2.Open();
	if (!m_set2.IsBOF())
	{
		AfxMessageBox("该模型下有零件实例，不能删除");
		return FALSE;
	}
	//2.判断选种的模型是否存在
	m_set.m_strFilter.Format("[Number]=%ld",num);
	m_set.Open();
	if (!m_set.IsBOF())
	{
		message.Format("确定要删除模型[%s]吗？",_T(m_set.m_Name));
		if(AfxMessageBox(message,MB_OK)!=IDOK)
		{
			return FALSE;
		}		
		try
		{
			m_set.m_pDatabase->BeginTrans();
			strSolidFileDir=m_set.m_SolidFilePlace;
			if(strSolidFileDir.Find("/装配模型/",0)==-1)
			{
				AfxMessageBox("原有模型信息被破坏！");
				m_set.m_pDatabase->Rollback();
				return FALSE;
			}
			m_set.Delete();            //删除其下所有相关信息，通过数据库的触发器进行
			m_set.Requery();
			if (!m_ftp.DeleteDirAndFile(strSolidFileDir,myFtp))
			{
				AfxMessageBox("从文件服务器删除文件失败！");
				m_set.m_pDatabase->Rollback();
				return FALSE;
			}
			m_set.m_pDatabase->CommitTrans();			
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			m_set.m_pDatabase->Rollback();
			return FALSE;
		}
	}
	return TRUE;
}