#include "StdAfx.h"
#include "asmassembly.h"
#include "NProductInsInfoTable.h"
#include "NProductBOMTable.h"
#include "NPartInsInfoTable.h"
#include "NAsmType.h"
#include "FtpFile.h"
#include "AsmManageTableSet.h"
#include "CAsmPerfSet.h"
#include "CAsmPicSet.h"
#include "CAsmRelSet.h"
#include "CAsmVarSet.h"
#include "NAsmBomTable.h"
#include "AsmInfoFn.h"
#include "PartMdlRecordSet.h"
#include "NPartTypeSet.h"

CAsmAssembly::CAsmAssembly(void):asm_draw(NULL),m_db(NULL),m_asm_num(0),error_info(_T(""))
{
}

CAsmAssembly::~CAsmAssembly(void)
{
}
/************************************************************************/
/* 打开产品实例															*/
/************************************************************************/
BOOL  CAsmAssembly::OpenProduct(CDatabase * p_db,long asm_num,ProSolid * asm_com)
{
	m_db=p_db;
	m_asm_num=asm_num;
	error_info.Empty();
	try
	{
		if (!GetCompInfo(asm_num))
		{
			return FALSE;
		}
		if (!CheckProduct(asm_num))
		{
			return FALSE;
		}
		if (!LoadSolid(asm_num))
		{
			return FALSE;
		}
		if (!GetCompMdl())
		{
			return FALSE;
		}
	}
	catch (CDBException * e) {
	     AfxMessageBox(e->m_strError);
		 return FALSE;
	}
	(*asm_com)=asm_solid;
	return TRUE;
}
/************************************************************************/
/* 这个函数必须在调用OpenProduct之后，如果装配成功才进行                */
/************************************************************************/
BOOL CAsmAssembly::AddInfo()
{
	if (m_db==NULL)
	{
		AfxMessageBox("数据库连接信息没有");
	}
	if (m_asm_num==0)
	{
		AfxMessageBox("没有设置组件实例");
	}
	try{
		if (!AddCompAsmInfo(asm_solid,asm_draw,m_asm_num))
		{
			return FALSE;
		}
		if (!AddComInfo(m_asm_num))
		{
			return FALSE;
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 装配不成功，则把模型清除内存                                         */
/************************************************************************/
BOOL CAsmAssembly::CancelAssembly()
{
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		if (!p_compitemlist[i].status && p_compitemlist[i].type=="asm")
		{
			ProMdlErase(p_compitemlist[i].mdl);
			if (p_compitemlist[i].drw!=NULL)
			{
				ProMdlErase(p_compitemlist[i].drw);
			}
		}
	}
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		if (!p_compitemlist[i].status && p_compitemlist[i].type=="prt")
		{
			ProMdlErase(p_compitemlist[i].mdl);
			if (p_compitemlist[i].drw!=NULL)
			{
				ProMdlErase(p_compitemlist[i].drw);
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 获得组件的所有零部件文件名，以及所属模型的信息，然后与现在的所有模型进行比较
																		*/
/************************************************************************/
BOOL  CAsmAssembly::CheckProduct(long asm_num)
{
	ProMdl       tempmdl;
	ProName      name;
	CString      csName;
	UsrPartPorp  temp_prop;
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		if (p_compitemlist[i].type == "asm")
		{
			csName=p_compitemlist[i].filename.Left(p_compitemlist[i].filename.GetLength()-4);
			ProStringToWstring(name,csName.GetBuffer());
			if (ProMdlInit(name,PRO_MDL_ASSEMBLY,&tempmdl)==PRO_TK_NO_ERROR)
			{
				//如果已经存在
				temp_prop=GetMdlInfo(tempmdl);
				if (temp_prop.Number!= p_compitemlist[i].num)
				{
					//如果同名的模型具有不同的标识号，
					AfxMessageBox(p_compitemlist[i].filename+"已经存在，但是不属于同一个组件");
					return FALSE;
				}
				else
				{
					p_compitemlist[i].status=true;
				}
			}
			else
			{
				p_compitemlist[i].status=false;
			}
		}
		if (p_compitemlist[i].type == "prt")
		{
			csName=p_compitemlist[i].filename.Left(p_compitemlist[i].filename.GetLength()-4);
			ProStringToWstring(name,csName.GetBuffer());
			//检查是否已经存在
			if (ProMdlInit(name,PRO_MDL_PART,&tempmdl)==PRO_TK_NO_ERROR)
			{
				//如果已经存在
				temp_prop=GetMdlInfo(tempmdl);
				if (temp_prop.Number!= p_compitemlist[i].num)
				{
					//如果同名的模型具有不同的标识号，
					AfxMessageBox(p_compitemlist[i].filename+"已经存在，但是不属于同一个零件");
					return FALSE;
				}
				else
				{
					p_compitemlist[i].status=true;  //存在了，而且是同一个
				}
			}
			else  //如果是不存在
			{
				p_compitemlist[i].status=false;
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 找到组件的零部件编号与零件实例、产品实例中的对应关系                 */
/************************************************************************/
BOOL  CAsmAssembly::GetCompInfo(long asm_num)
{
	CNProductInsInfoTable   asm_ins_set(m_db);
	CNProductBOMTable       asm_ins_bom(m_db);
	CNPartInsInfoTable	    prt_ins_set(m_db);	
	asm_ins_bom.m_strFilter.Format("[PdtNum]=%ld",asm_num);
	asm_ins_bom.Open();
	asm_ins_set.Open();
	prt_ins_set.Open();
	if(!asm_ins_bom.IsBOF())
	{
		asm_ins_bom.MoveFirst();
		while (!asm_ins_bom.IsEOF())
		{
			comp_item				temp_item;
			if (asm_ins_bom.m_Type.MakeLower() == "asm")
			{
				temp_item.type="asm";
				temp_item.num=asm_ins_bom.m_PrtNum;
				temp_item.mdlcomnum=asm_ins_bom.m_MdlComNum;
				temp_item.mdlnum=-1;
				//获得该部件所属的模型
				asm_ins_set.m_strFilter.Format("[PdtNum]=%ld",asm_ins_bom.m_PrtNum);
				asm_ins_set.Requery();
				if (!asm_ins_set.IsBOF())
				{
					asm_ins_set.MoveFirst();
					temp_item.mdlnum=asm_ins_set.m_MdlNum;
					temp_item.pdmnum=asm_ins_set.m_PDMNum;
					temp_item.filename=asm_ins_set.m_SolidFileName;
					temp_item.filefullpath=asm_ins_set.m_SolidFilePlace;
					temp_item.drawfilename=asm_ins_set.m_DrawingFileName;
					temp_item.drawfilefullpath=asm_ins_set.m_DrawingFilePlace;

					if(asm_ins_set.m_PDMState=="提交")
					{
						temp_item.fileresource=2;
					}
					else if (asm_ins_set.m_LocalState=="本地")
					{
						temp_item.fileresource=0;
					}
					else if (asm_ins_set.m_LocalState=="服务器")
					{
						temp_item.fileresource=1;
					}
					p_compitemlist.Add(temp_item);
				}
				else 
				{
					return FALSE;//如果组装的部件在产品实例表中不存在了，则取消
				}
			}
			if (asm_ins_bom.m_Type.MakeLower() == "prt")
			{
				temp_item.type="prt";
				temp_item.num=asm_ins_bom.m_PrtNum;   //零件号
				temp_item.mdlcomnum=asm_ins_bom.m_MdlComNum;
				temp_item.mdlnum=-1;                 //预设所属模型为-1
				//获得该部件所属的模型
				prt_ins_set.m_strFilter.Format("[PrtNum]=%ld",asm_ins_bom.m_PrtNum);
				prt_ins_set.Requery();
				if (!prt_ins_set.IsBOF())
				{
					prt_ins_set.MoveFirst();
					temp_item.mdlnum=prt_ins_set.m_MdlNum;
					temp_item.pdmnum=prt_ins_set.m_PDMNum;
					temp_item.filename=prt_ins_set.m_SolidFileName;
					temp_item.filefullpath=prt_ins_set.m_SolidPlace;
					temp_item.drawfilename=prt_ins_set.m_DrawingName;
					temp_item.drawfilefullpath=prt_ins_set.m_DrawingPlace;
					if(prt_ins_set.m_PDMState=="提交")
					{
						temp_item.fileresource=2;
					}
					else if (prt_ins_set.m_LocalState=="本地")
					{
						temp_item.fileresource=0;
					}
					else if (prt_ins_set.m_LocalState=="服务器")
					{
						temp_item.fileresource=1;
					}
					p_compitemlist.Add(temp_item);
				}
				else
				{
					return FALSE;//如果在零件实例中不存在，则取消
				}
			}
			asm_ins_bom.MoveNext();
		}
	}
	return TRUE;
}
/************************************************************************/
/* 下载，打开模型														*/
/************************************************************************/
BOOL  CAsmAssembly::LoadSolid(long asm_num)
{
	CStringArray				files;   //在服务器上的文件（PDM与设计系统服务器共用）
	CStringArray                localfiles;//在本地上的文件
	CNProductInsInfoTable		asm_set(m_db);
	CString                     fileTemp1="",fileTemp2="";
	CString						csError;
	//1.获得文件的存放位置
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		if(p_compitemlist[i].status) continue;//如果已经在当前存在了，不下载
		switch(p_compitemlist[i].fileresource) {
		case 0:					//本地
			localfiles.Add( p_compitemlist[i].filefullpath+"\\"+p_compitemlist[i].filename);
			if (!p_compitemlist[i].drawfilename.IsEmpty())
			{
				localfiles.Add( p_compitemlist[i].drawfilefullpath+"\\"+ p_compitemlist[i].drawfilename);
			}
			break;
		case 1:					//设计系统
			files.Add( p_compitemlist[i].filefullpath+ "/"+p_compitemlist[i].filename+".1");
			if (!p_compitemlist[i].drawfilename.IsEmpty())
			{
				files.Add( p_compitemlist[i].drawfilefullpath+ "/"+p_compitemlist[i].drawfilename+".1");
			}
			break;
		default:				 //PDM系统
			//去PDM找
		    UsrGetPartAndProductFromPdm(p_compitemlist[i].pdmnum,0,fileTemp1,fileTemp2);
			files.Add(fileTemp1);
			if (!p_compitemlist[i].drawfilename.IsEmpty()) files.Add(fileTemp2);
			break;
		}
	}
	

	//2.1获得组件本身的存放位置
	asm_set.m_strFilter.Format("[PdtNum]=%ld",asm_num);
	asm_set.Open();
	if (!asm_set.IsBOF())
	{
		asm_set.MoveFirst();
		if(asm_set.m_PDMState=="提交")
		{
			//去服务PDM找
			UsrGetPartAndProductFromPdm(asm_set.m_PDMNum,0,fileTemp1,fileTemp2);
			files.Add(fileTemp1);
			if (!asm_set.m_DrawingFileName.IsEmpty()) files.Add(fileTemp2);
		}
		else if (asm_set.m_LocalState=="本地")
		{
			//从本地找
			localfiles.Add(asm_set.m_SolidFilePlace+"\\"+asm_set.m_SolidFileName);
			if (!asm_set.m_DrawingFileName.IsEmpty())
			{
				localfiles.Add(asm_set.m_DrawingFilePlace+"\\"+asm_set.m_DrawingFileName);
			}			
		}
		else if (asm_set.m_LocalState=="服务器")
		{
			files.Add(asm_set.m_SolidFilePlace+"/"+asm_set.m_SolidFileName+".1");
			if (!asm_set.m_DrawingFileName.IsEmpty())
			{
				files.Add(asm_set.m_DrawingFilePlace+"/"+asm_set.m_DrawingFileName +".1");
			}	
		}
	}
	else return FALSE;//组件本省不存在

	//1.1如果是本地文件，则要检查本地是否存在
	if(localfiles.GetCount()>0)
	{
		if (!IsFilesExists(localfiles/*文件名称*/,csError))
		{
			AfxMessageBox(csError);
			return FALSE;
		}
	}
	//3.选择保存位置,建立文件夹
	CString  newDir;	
	if (!SelectAnDirectory(localDir,""))
	{
		return FALSE;
	}	
	if (!CreateDirectoryGivenPathAndName(localDir,asm_set.m_Name,newDir))
	{
		return FALSE;
	}
	localDir=localDir+newDir;   //c:\\dd\\new
	//4.1下载到本地文件夹
	CFtpFile                m_ftp;
	if(files.IsEmpty()) return FALSE;
	if (!m_ftp.DownloadFilestoDirectory(&files,localDir,myFtp,csError))
	{
		AfxMessageBox(csError);
		return FALSE;
	}
	//5.把模型导入内存
    //5.1先把本地的导入内存(本地的组件)  这里面可能会不太合适，以后需要改进
	ProError status;
	ProPath  name;
	CString  fullFileName;
	ProMdl   tempmdl;
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		if(p_compitemlist[i].fileresource==0 && p_compitemlist[i].type=="asm")
		{
			fullFileName= p_compitemlist[i].filefullpath+"\\"+p_compitemlist[i].filename;
			ProStringToWstring(name,fullFileName.GetBuffer());
			status=ProMdlLoad(name,PRO_MDL_ASSEMBLY,PRO_B_FALSE,(ProMdl*)&tempmdl);
			if (!p_compitemlist[i].drawfilename.IsEmpty() )
			{
				fullFileName= p_compitemlist[i].drawfilefullpath+"\\"+p_compitemlist[i].drawfilename;
				ProStringToWstring(name,fullFileName.GetBuffer());
				status=ProMdlLoad(name,PRO_MDL_DRAWING,PRO_B_FALSE,(ProMdl*)&tempmdl);
			}
			if (status!=PRO_TK_NO_ERROR)
			{
				AfxMessageBox("打开模型本地模型"+fullFileName+"失败");
				return FALSE;
			}
		}
	}
	//5.2导入组件
	fullFileName=localDir+"\\"+asm_set.m_SolidFileName;
	ProStringToWstring(name,fullFileName.GetBuffer());
	status=ProMdlLoad(name,PRO_MDL_ASSEMBLY,PRO_B_FALSE,(ProMdl*)&asm_solid); 
	if (!asm_set.m_DrawingFileName.IsEmpty())
	{
		fullFileName=localDir+"\\"+asm_set.m_DrawingFileName;
		ProStringToWstring(name,fullFileName.GetBuffer());
		status=ProMdlLoad(name,PRO_MDL_DRAWING,PRO_B_FALSE,(ProMdl*)&asm_draw); 
	}
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("打开模型失败");
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 获得元件的句柄                                                       */
/************************************************************************/
BOOL  CAsmAssembly::GetCompMdl()
{
	ProName        name;
	ProSolid       tempsolid;
	ProDrawing     tempdrw;
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		if (p_compitemlist[i].type=="asm")
		{
			ProStringToWstring(name,p_compitemlist[i].filename.Left(p_compitemlist[i].filename.GetLength()-4).GetBuffer());  
			if (ProMdlInit(name,PRO_MDL_ASSEMBLY,(ProMdl*)&tempsolid)==PRO_TK_NO_ERROR)
			{
				p_compitemlist[i].mdl=tempsolid;
				ProSolidToPostfixId(tempsolid,&(p_compitemlist[i].postid));
				if(!p_compitemlist[i].drawfilename.IsEmpty())
				{
					ProStringToWstring(name,p_compitemlist[i].drawfilename.Left(p_compitemlist[i].drawfilename.GetLength()-4).GetBuffer()); 
					if (ProMdlInit(name,PRO_MDL_DRAWING,(ProMdl*)&tempdrw))
					{
						p_compitemlist[i].drw=tempdrw;
					}
					else p_compitemlist[i].drw=NULL;
				}
			}
			{
				p_compitemlist[i].mdl=NULL;
			}
		}
		if (p_compitemlist[i].type=="prt")
		{
			ProStringToWstring(name,p_compitemlist[i].filename.Left(p_compitemlist[i].filename.GetLength()-4).GetBuffer());  
			if (ProMdlInit(name,PRO_MDL_PART,(ProMdl*)&tempsolid)==PRO_TK_NO_ERROR)
			{
				p_compitemlist[i].mdl=tempsolid;
				ProSolidToPostfixId(tempsolid,&(p_compitemlist[i].postid));
				if(!p_compitemlist[i].drawfilename.IsEmpty())
				{
					ProStringToWstring(name,p_compitemlist[i].drawfilename.Left(p_compitemlist[i].drawfilename.GetLength()-4).GetBuffer()); 
					if (ProMdlInit(name,PRO_MDL_DRAWING,(ProMdl*)&tempdrw))
					{
						p_compitemlist[i].drw=tempdrw;
					}
					else p_compitemlist[i].drw=NULL;
				}
			}
			else 
			{
				p_compitemlist[i].mdl=NULL;
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 从数据库中把元件的关系式、图片、可变参数等信息添加到各自的模型中     */
/************************************************************************/
BOOL  CAsmAssembly::AddComInfo(long asm_num)
{
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		comp_item temp=p_compitemlist[i];
		if (p_compitemlist[i].type=="asm" /*&& p_compitemlist[i].mdlnum!=-1*/ && !p_compitemlist[i].status)
				AddCompAsmInfo(p_compitemlist[i].mdl,p_compitemlist[i].drw,p_compitemlist[i].num);
		if (p_compitemlist[i].type=="prt" /*&& p_compitemlist[i].mdlnum!=-1*/ && !p_compitemlist[i].status) 
				AddCompPartInfo(p_compitemlist[i].mdl,p_compitemlist[i].drw,p_compitemlist[i].num);
	}
	return TRUE;
}
/************************************************************************/
/* 添加零件的信息                                                       */
/************************************************************************/
BOOL CAsmAssembly::AddCompPartInfo(ProSolid part_solid,ProDrawing part_draw=NULL,long PrtNum=0)
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
				//把模型的信息都添加进来				
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
		//添加其他信息 如果是有模型的实例，把模型的信息添加到实例中，
		//注意，这个函数必须在设置完模型的基本信息之后调用
		//因为，在内部要修改模型基本信息
		if (temp_porp.MdlType==1)
		{
			UsrAddPartMdlOtherInfo(m_db,part_solid,temp_porp.MdlNum);
		}
	}
	return TRUE;
}
/************************************************************************/
/*添加组件的信息                                                        */
/************************************************************************/
BOOL CAsmAssembly::AddCompAsmInfo(ProSolid mdl,ProDrawing part_draw=NULL,long asm_num=0 /*组件实例号*/)
{
	CNAsmType				asm_type(m_db);
	CNProductInsInfoTable		asm_set(m_db);
	CAsmManageTableSet          asm_mdl_set(m_db);
	UsrPartPorp					temp_porp;
	CAsmInfoFn                  asm_info_fn;
	//先清楚原来的
	EraseMdlInfo(asm_solid,ERASE_ALL);
	try
	{

		asm_set.m_strFilter.Format("[PdtNum]=%ld",asm_num);		
		asm_set.Open();
		if (!asm_set.IsBOF())
		{
			asm_set.MoveFirst();
			temp_porp.owner  = mdl;
			temp_porp.Number = asm_num;
			//判断这个零件是否来自模型
			switch(asm_set.m_MdlNum) {
		case -1:
			temp_porp.MdlType=2;           //无模型的零件或组件
			temp_porp.MdlNum=-1;
			break;
		default:
			temp_porp.MdlType=1;           //有模型的实例
			temp_porp.MdlNum=asm_set.m_MdlNum;
			break;
			}			
			temp_porp.Name   =asm_set.m_Name;
			temp_porp.Code   =asm_set.m_InsCode;
			//获得模型的类别以及类别名称
			asm_type.m_strFilter.Format("[Number]=%ld",asm_set.m_TypeNum);
			asm_type.Open();
			if (!asm_type.IsBOF())
			{
				asm_type.MoveFirst();
				temp_porp.TypeNum=asm_set.m_TypeNum;
				temp_porp.TypeName=asm_type.m_Name;
			}
			else
			{
				temp_porp.TypeNum=-1;
				temp_porp.TypeName="";
			}
			temp_porp.Material="";
			temp_porp.Person  ="";
			temp_porp.Note    =asm_set.m_Note;
			temp_porp.IsAudited=(asm_set.m_AuditState==_T("审核通过")?true:false);
			//文件的来源
			if (asm_set.m_PDMState==_T("提交"))
				temp_porp.ResourceType=2;
			else if(asm_set.m_LocalState==_T("本地"))
				temp_porp.ResourceType=0;
			else if(asm_set.m_LocalState==_T("服务器"))
				temp_porp.ResourceType=1;
			//装配件的元件
			temp_porp.IsAsmComp=true;
			//获得模型的名称
			if (asm_set.m_MdlNum>0)
			{
				asm_mdl_set.m_strFilter.Format("[Number]=%ld",asm_set.m_MdlNum);
				asm_mdl_set.Open();
				asm_mdl_set.Requery();
				if (!asm_mdl_set.IsBOF())
				{
					asm_mdl_set.MoveFirst();
					temp_porp.MdlName=asm_mdl_set.m_Name;
				}
			}
			else temp_porp.MdlName="无模型";
			//添加工程图
			if (part_draw!=NULL)
			{
				temp_porp.bHaveDraw=true;
				temp_porp.hDraw=part_draw;
			}
			SetMdlInfo(mdl,temp_porp);
		}

		//如果组件是模型的实例，则添加模型的信息到组件实例中
        if (temp_porp.MdlType==1)
        {
			asm_info_fn.AddAsmInsInfo(m_db,mdl,asm_num);
			return TRUE;
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
	}
	return TRUE;
}