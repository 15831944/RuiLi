#include "StdAfx.h"
#include "PartMdlRecordSet.h"
#include "partmdlopen.h"
//#include "partDrawingSet.h"
#include "partPerfparaTableSet.h"
#include "partPicSet.h"
#include "NPartInsInfoTable.h"
#include "partRelationSet.h"
#include "partVariantTableSet.h"
#include "UsrSWTXTable.h"
#include "NPartTypeSet.h"
#include "FtpFile.h"
CPartMdlOpen::CPartMdlOpen(void):part_draw(NULL),part_solid(NULL)
{
}

CPartMdlOpen::~CPartMdlOpen(void)
{
}
BOOL CPartMdlOpen::Open(CDatabase * p_db,long part_num)
{
	m_db = p_db;
	CPartMdlRecordSet  m_pSet(m_db);
	m_pSet.m_strFilter.Format("[Number]=%ld",part_num);
	try
	{
		m_pSet.Open();
		if (m_pSet.IsBOF())
		{
			AfxMessageBox("选中的模型已经不存在！");
			return FALSE;
		}
		if (!CheckCurrentPartMdl(part_num))
		{
			return FALSE;
		}
		if (!LoadSolid(part_num))
		{
			return FALSE;
		}
		//清楚模型所有信息
		EraseMdlInfo(part_solid,ERASE_ALL);
		//给模型添加信息
		if (!AddPartMdlInfo(part_num))
		{
			return FALSE;
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		
	}
	return TRUE;
}
BOOL CPartMdlOpen::CheckCurrentPartMdl(long num)
{
	CPartMdlRecordSet   m_pSet(m_db);
	ProMdl				part_solid;
	ProType					type;
	ProName					w_name;
	int						w_id;
	ProError				status;
	CString                 error_info;
	m_pSet.m_strFilter.Format("[Number]=%ld",num);
	m_pSet.Open();
	if (m_pSet.IsBOF())
	{
		AfxMessageBox("模型已经不存在！");
		return FALSE;
	}
	m_pSet.MoveFirst();
	switch(UsrCheckCurrentMdl(PRO_MDL_PART,m_pSet.m_SolidFileName,num,&part_solid)) {
	case CHECK_NAME_NUM_SAME:
		status = ProMdlTypeGet(part_solid, (ProMdlType*)&type);
		status = ProMdlNameGet(part_solid, w_name);
		status = ProObjectwindowCreate(w_name, type, &w_id);
		if(!status== PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
		status = ProMdlDisplay(part_solid);
		status = ProWindowActivate(w_id);		
		return FALSE;
		break;
	case CHECK_SNAME_NNUM:
		error_info.Format("你已经打开具有相同文件名(%s)的零件,该零件与选择的零件模型不同。\n无法继续为你打开，必须先拭除这个零件。",m_pSet.m_SolidFileName);
		AfxMessageBox(error_info);
		return FALSE;
		break;
	default:
		return TRUE;
	}
}
/************************************************************************/
/* 导入模型                                                             */
/************************************************************************/
BOOL CPartMdlOpen::LoadSolid(long num)
{
	CPartMdlRecordSet		m_set(m_db);
	ProError				status;
	//ProPath				wcurrentDir;
	CStringArray			files;
	CFtpFile				m_ftp;
	CString					fullFileName;//包括路径的文件名
	ProPath					name;
	ProType					type;
	ProName					w_name;
	int						w_id;
	CString					newDir;	//用来建立的文件夹
	CString					csError;
	//1.选择存放位置
	if (!SelectAnDirectory(part_dir,""))
	{
		return FALSE;
	}
	//2.在选择的存放位置里面新建一个文件夹
	m_set.m_strFilter.Format("[Number]=%ld",num);
	m_set.Open();
	if(m_set.IsBOF()) return FALSE;
	m_set.MoveFirst();
	if(m_set.m_SolidFileName.IsEmpty()) return FALSE;
	if (!CreateDirectoryGivenPathAndName(part_dir,m_set.m_Name,newDir))
	{
		return FALSE;
	}
	part_dir=part_dir+newDir;				//c:\\dd\\new

	//3.下载文件到新建的文件夹里面
	files.Add(m_set.m_SolidFilePlace +"/"+m_set.m_SolidFileName+".1");
			//如果存在工程图
	if(!m_set.m_DrawingFileName.IsEmpty()) files.Add(m_set.m_DrawingFilePlace +"/"+m_set.m_DrawingFileName+".1");
	if (!m_ftp.DownloadFilestoDirectory(&files,part_dir,myFtp,csError))
	{
		AfxMessageBox(csError);
		return FALSE;
	}

	//4.把模型导入内存并打开
	fullFileName=part_dir+"\\"+m_set.m_SolidFileName+".1";
	ProStringToWstring(name,fullFileName.GetBuffer());
	status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&part_solid); 
	//如果有工程图，则打开工程图
	if(!m_set.m_DrawingFileName.IsEmpty())	
	{
		fullFileName=part_dir+"\\"+m_set.m_DrawingFileName+".1";
		ProStringToWstring(name,fullFileName.GetBuffer());
		status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&part_draw);
	}
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("打开模型失败");
		return FALSE;
	}	
	status = ProMdlTypeGet(part_solid, (ProMdlType*)&type);
	status=  ProMdlNameGet(part_solid, w_name);
	status=  ProObjectwindowCreate(w_name, type, &w_id);
	if(!status== PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	status = ProMdlDisplay(part_solid);
	status = ProWindowActivate(w_id);	
	return TRUE;
}
/************************************************************************/
/* 用来打开零件实例                                                     */
/************************************************************************/
BOOL CPartMdlOpen::OpenPart(CDatabase * p_db,long part_num/*零件的标识号*/)
{
	m_db=p_db;
	CNPartInsInfoTable  m_pSet(m_db);
	m_pSet.m_strFilter.Format("[PrtNum]=%ld",part_num);
	try
	{
		m_pSet.Open();
		if (m_pSet.IsBOF())
		{
			AfxMessageBox("选中的零件不存在！");
			return FALSE;
		}
		if (!CheckCurrentPart(part_num))
		{
			return FALSE;
		}
		if (!LoadPart(part_num))
		{
			return FALSE;
		}
		//清楚模型所有信息
		EraseMdlInfo(part_solid,ERASE_ALL);
		//给零件添加零件模型的信息
		if (!AddPartInfo(part_num))
		{
			AfxMessageBox("SDFSD");
			return FALSE;
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		
	}
	return TRUE;
}
BOOL CPartMdlOpen::CheckCurrentPart(long num)
{
	CNPartInsInfoTable  m_pSet(m_db);
	ProMdl				part_solid;
	ProType					type;
	ProName					w_name;
	int						w_id;
	ProError				status;
	CString                 error_info;
	m_pSet.m_strFilter.Format("[PrtNum]=%ld",num);
	m_pSet.Open();
	if (m_pSet.IsBOF())
	{
		AfxMessageBox("零件已经不存在！");
		return FALSE;
	}
	m_pSet.MoveFirst();
	switch(UsrCheckCurrentMdl(PRO_MDL_PART,m_pSet.m_SolidFileName,num,&part_solid)) {
	case CHECK_NAME_NUM_SAME:
		status = ProMdlTypeGet(part_solid, (ProMdlType*)&type);
		status = ProMdlNameGet(part_solid, w_name);
		status = ProObjectwindowCreate(w_name, type, &w_id);
		if(!status== PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
		status = ProMdlDisplay(part_solid);
		status = ProWindowActivate(w_id);		
		return FALSE;
		break;
	case CHECK_SNAME_NNUM:
		error_info.Format("你已经打开具有相同文件名(%s)的零件,该零件与选择的零件不同。\n无法继续为你打开，必须先拭除这个零件。",m_pSet.m_SolidFileName);
		AfxMessageBox(error_info);
		return FALSE;
		break;
	default:
		return TRUE;
	}
}
/************************************************************************/
/* 打开零件实例                                                         */
/************************************************************************/
BOOL CPartMdlOpen::LoadPart(long num)
{
	CNPartInsInfoTable		m_set(m_db);
	ProError				status;
	ProPath					wcurrentDir;
	CStringArray			files;
	CFtpFile				m_ftp;
	CString					fullFileName;//包括路径的文件名
	ProPath					name;
	ProType					type;
	ProName					w_name;
	int						w_id;
	CString					newDir;	//用来建立的文件夹
	CString					csError;
	CString                 draw_dir;
	part_draw  = NULL;
	part_solid = NULL;

	//选择目录
	status   = ProDirectoryCurrentGet(wcurrentDir);
	part_dir = CString(wcurrentDir);       // "C:\\DFS\\"
	if (!SelectAnDirectory(part_dir,part_dir))
	{
		return FALSE;
	}

	m_set.m_strFilter.Format("[PrtNum]=%ld",num);
	m_set.Open();
	if(m_set.IsBOF()) return FALSE;
	m_set.MoveFirst();	

	//1查找文件
	if (m_set.m_PDMState==_T("提交"))
	{
		//根据PDM来获取模型和工程图的地址
		CString  fileTemp1="",fileTemp2="";
		if (!CreateDirectoryGivenPathAndName(part_dir,m_set.m_Name,newDir))
		{
			return FALSE;
		}
		part_dir=part_dir+newDir;
		draw_dir=part_dir;
		UsrGetPartAndProductFromPdm(m_set.m_PDMNum,0,fileTemp1,fileTemp2);
		if(!m_set.m_SolidFileName.IsEmpty()) 
		{					
			//files.Add(m_set.m_SolidPlace+"/"+m_set.m_SolidFileName+".1");
			files.Add(fileTemp1);
		}
		if(!m_set.m_DrawingName.IsEmpty())
		{
			//files.Add(m_set.m_DrawingPlace+"/"+m_set.m_DrawingName+".1");
			files.Add(fileTemp2);
		}	
		m_ftp.DownloadFilestoDirectory(&files,part_dir,myFtp,csError);
		if (!csError.IsEmpty())
		{
			AfxMessageBox(csError);
			return FALSE;
		}
	}
	else if (m_set.m_LocalState==_T("服务器"))
	{
		//根据服务器来获取模型＆工程图的地址,通过下载，
		//首先要有一个查找过程，看是否有找到制定图纸
		if (!CreateDirectoryGivenPathAndName(part_dir,m_set.m_Name,newDir))
		{
			return FALSE;
		}
		part_dir=part_dir+newDir;
		draw_dir=part_dir;
		if(!m_set.m_SolidFileName.IsEmpty()) 
		{					
			files.Add(m_set.m_SolidPlace+"/"+m_set.m_SolidFileName+".1");
		}
		if(!m_set.m_DrawingName.IsEmpty())
		{
			files.Add(m_set.m_DrawingPlace+"/"+m_set.m_DrawingName+".1");
		}	
		m_ftp.DownloadFilestoDirectory(&files,part_dir,myFtp,csError);
		if (!csError.IsEmpty())
		{
			AfxMessageBox(csError);
			return FALSE;
		}
	}
	else if (m_set.m_LocalState==_T("本地"))
	{
		//根据本地，来判断是否为本机，方法是判断制定的目录是否有这个文件，不需要下载
		//本地文件不许要版本
		//根据零件不同位置来打开零件 
		if(!m_set.m_SolidFileName.IsEmpty()) 
		{
			files.Add(m_set.m_SolidPlace+"\\"+m_set.m_SolidFileName);
			part_dir=m_set.m_SolidPlace;
		}					
		if(!m_set.m_DrawingName.IsEmpty())   
		{
			files.Add(m_set.m_DrawingPlace+"\\"+m_set.m_DrawingName);
			draw_dir=m_set.m_DrawingPlace;
		}	
		if (!IsFilesExists(files,csError))
		{
			AfxMessageBox(csError);
			return FALSE;
		}
	}
	//把模型导入内存
	fullFileName= part_dir+"\\"+m_set.m_SolidFileName;
	ProStringToWstring(name,fullFileName.GetBuffer());
	status		= ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&part_solid); 
	//如果有工程图，则打开工程图
	if(!m_set.m_DrawingName.IsEmpty())	
	{
		fullFileName=draw_dir+"\\"+m_set.m_DrawingName;
		ProStringToWstring(name,fullFileName.GetBuffer());
		status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&part_draw);
	}
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("打开模型失败");
		return FALSE;
	}	
	status = ProMdlTypeGet(part_solid, (ProMdlType*)&type);
	status = ProMdlNameGet(part_solid, w_name);
	status = ProObjectwindowCreate(w_name, type, &w_id);
	if(!status== PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	status = ProMdlDisplay(part_solid);
	status = ProWindowActivate(w_id);	
	return TRUE;
}
/************************************************************************/
/* 添加零件模型的信息                                                   */
/************************************************************************/
BOOL CPartMdlOpen::AddPartMdlInfo(long PrtNum)
{	
	CNPartTypeSet           m_parttypeset(m_db);
	CPartMdlRecordSet       m_partmdlset(m_db);
	UsrPartPorp             temp_porp;
	m_partmdlset.m_strFilter.Format("[Number]=%ld",PrtNum);
	m_partmdlset.Open();
	if(!m_partmdlset.IsBOF())
	{
		m_partmdlset.MoveFirst();
		temp_porp.owner  = part_solid;
		temp_porp.Number = PrtNum;
		//类型为模型
		temp_porp.MdlType=3;
		temp_porp.MdlNum=PrtNum;			
		temp_porp.Name   =m_partmdlset.m_Name;
		temp_porp.Code   =m_partmdlset.m_Code;
		//获得模型的类别以及类别名称
		m_parttypeset.m_strFilter.Format("[Number]=%ld",m_partmdlset.m_TypeNum);
		m_parttypeset.Open();
		if (!m_parttypeset.IsBOF())
		{
			m_parttypeset.MoveFirst();
			temp_porp.TypeNum=m_partmdlset.m_TypeNum;
			temp_porp.TypeName=m_parttypeset.m_Name;
		}
		else
		{
			temp_porp.TypeNum=-1;
			temp_porp.TypeName="";
		}
		temp_porp.Material=m_partmdlset.m_Material;
		temp_porp.Person  ="";
		temp_porp.Note    =m_partmdlset.m_Note;
		temp_porp.IsAudited=false;
        temp_porp.ResourceType=1;
		temp_porp.IsAsmComp=false;
		temp_porp.MdlNum=-1;
		temp_porp.MdlName="无模型";
		//添加工程图
		if (part_draw!=NULL)
		{
			temp_porp.bHaveDraw=true;
			temp_porp.hDraw=part_draw;
		}
		//把零件模型其他信息添加进来
		UsrAddPartMdlOtherInfo(m_db,part_solid,PrtNum);
		SetMdlInfo(part_solid,temp_porp);	
	}
	return TRUE;
}
/************************************************************************/
/* 添加零件的信息                                                       */
/************************************************************************/
BOOL CPartMdlOpen::AddPartInfo(long PrtNum)
{
	CNPartInsInfoTable		m_tempSet(m_db);	
	CNPartTypeSet           m_parttypeset(m_db);
	CPartMdlRecordSet       m_partmdlset(m_db);
	UsrPartPorp             temp_porp;
	m_tempSet.m_strFilter.Format("[PrtNum]=%ld",PrtNum);
	m_tempSet.Open();
	if(!m_tempSet.IsBOF())
	{
		m_tempSet.MoveFirst();
		temp_porp.owner  = part_solid;
		temp_porp.Number = PrtNum;
		//判断这个零件是否来自模型
		switch(m_tempSet.m_MdlNum) {
			case -1:
				temp_porp.MdlType=2;           //无模型的零件或组件
				temp_porp.MdlNum=-1;
				break;
			default:
				temp_porp.MdlType=1;           //有模型的实例
				temp_porp.MdlNum=m_tempSet.m_MdlNum;
				break;
		}			
		temp_porp.Name   =m_tempSet.m_Name;
		temp_porp.Code   =m_tempSet.m_InsCode;
		//获得模型的类别以及类别名称
		m_parttypeset.m_strFilter.Format("[Number]=%ld",m_tempSet.m_TypeNum);
		m_parttypeset.Open();
		if (!m_parttypeset.IsBOF())
		{
			m_parttypeset.MoveFirst();
			temp_porp.TypeNum=m_tempSet.m_TypeNum;
			temp_porp.TypeName=m_parttypeset.m_Name;
		}
		else
		{
			temp_porp.TypeNum=-1;
			temp_porp.TypeName="";
		}
		temp_porp.Material=m_tempSet.m_Material;
		temp_porp.Person  ="";
		temp_porp.Note    =m_tempSet.m_Note;
		temp_porp.IsAudited=(m_tempSet.m_AuditState==_T("审核通过")?true:false);
		//文件的来源
		if (m_tempSet.m_PDMState==_T("提交"))
			temp_porp.ResourceType=2;
		else if(m_tempSet.m_LocalState==_T("本地"))
			temp_porp.ResourceType=0;
		else if(m_tempSet.m_LocalState==_T("服务器"))
			temp_porp.ResourceType=1;
		//装配件的元件
		temp_porp.IsAsmComp=false;
		//获得模型的名称
		if (m_tempSet.m_MdlNum>0)
		{
			m_partmdlset.m_strFilter.Format("[Number]=%ld",m_tempSet.m_MdlNum);
			m_partmdlset.Open();
			m_partmdlset.Requery();
			if (!m_partmdlset.IsBOF())
			{
				m_partmdlset.MoveFirst();
				temp_porp.MdlName=m_partmdlset.m_Name;
			}
		}
		else temp_porp.MdlName="无模型";
        //添加工程图
		if (part_draw!=NULL)
		{
			temp_porp.bHaveDraw=true;
			temp_porp.hDraw=part_draw;
		}

		SetMdlInfo(part_solid,temp_porp);		

		if (temp_porp.MdlType==1)
		{
			//把模型的信息都添加进来
			UsrAddPartMdlOtherInfo(m_db,part_solid,m_tempSet.m_MdlNum);
		}
	}
	return TRUE;
}
BOOL CPartMdlOpen::AddPartMdlInfoForOtherUse(CDatabase * p_db,ProSolid solid,long part_num)
{
	m_db=p_db;
	part_solid=solid;
	EraseMdlInfo(solid,ERASE_ALL);
	return AddPartMdlInfo(part_num);
}