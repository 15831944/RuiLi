#include "StdAfx.h"
#include "asmopen.h"
#include "CAsmCompVariantTableSet.h"
#include "CAsmDrwTableSet.h"
#include "CAsmMakeupSet.h"
#include "CAsmPerfSet.h"
#include "CAsmPicSet.h"
#include "CAsmRelSet.h"
#include "CAsmVarSet.h"
#include "UsrSWTXTable.h"
#include "FtpFile.h"
#include "PartInsTableSet.h"
#include "AsmInsInfoTable.h"

#include "NAsmBomTable.h"
#include "PartMdlRecordSet.h"
#include "NPartInsInfoTable.h"
#include "NProductInsInfoTable.h"
#include "NAsmType.h"

#include "NPartTypeSet.h"
#include "partPerfparaTableSet.h"
#include "partVariantTableSet.h"
#include "partPicSet.h"
#include "partRelationSet.h"
CAsmOpen::CAsmOpen(void):m_pSet(NULL)
,asm_solid(NULL)
,localDir(_T("")) 
,asm_drw(NULL)
{
}

CAsmOpen::~CAsmOpen(void)
{
}

/************************************************************************/
/* 打开模型                                                             */
/************************************************************************/
BOOL CAsmOpen::Open(CDatabase * p_db,long num)
{
	m_pSet=new CAsmManageTableSet(p_db);	
	asm_num=num;
	m_pSet->m_strFilter.Format("[Number]=%ld",asm_num);
	m_pSet->Open();
	if (m_pSet->IsBOF())
	{
		AfxMessageBox("当前模型已经不存在！");
		delete m_pSet;
		return FALSE;
	} 
	if (!CheckCurrentMdl(asm_num))
	{
		delete m_pSet;
		return FALSE;
	}
	if (!LoadSolid(num)) 
	{
		delete m_pSet;
		return FALSE;
	}
	if(!AddAsmSolidInfo(num))
	{
		delete m_pSet;
		return FALSE;
	}
	if (!AddCompInfo(num))
	{
		delete m_pSet;
		return FALSE;
	}
	if(!asm_inf_fn.AddAsmMdlInfo(m_pSet->m_pDatabase,asm_solid,num)) 
	{
		delete m_pSet;
		return FALSE;
	}
	delete m_pSet;
	return TRUE;
}
BOOL CAsmOpen::CheckCurrentMdl(long num)
{
	CNProductInsInfoTable   asm_set(m_pSet->m_pDatabase);
	CNAsmBomTable           bom_set(m_pSet->m_pDatabase);
	CAsmManageTableSet      asm_m_set(m_pSet->m_pDatabase);
	CNPartInsInfoTable      prt_ins_set(m_pSet->m_pDatabase);
	CString                 error_info;
	//得到所有元件的存放位置
	bom_set.m_strFilter.Format("[AsmNum]=%ld",num);
	bom_set.Open();
	prt_ins_set.Open();
	asm_set.Open();
	if (!bom_set.IsBOF())
	{		
		bom_set.MoveFirst();
		do {
			error_info.Empty();
			//获得零部件的存放位置跟地址
			if (bom_set.m_Type.MakeLower()=="asm" && bom_set.m_PartNum>0 )
			{
				asm_set.m_strFilter.Format("[PdtNum]=%ld",bom_set.m_PartNum);
				asm_set.Requery();
				if (!asm_set.IsBOF())
				{
					asm_set.MoveFirst();
					if (UsrCheckCurrentMdl(PRO_MDL_ASSEMBLY,asm_set.m_SolidFileName,asm_set.m_PdtNum)==CHECK_SNAME_NNUM)
					{
						error_info.Format("系统中已经有相同的文件名(%s)存在",asm_set.m_SolidFileName);
						error_info.Format("%s是否继续打开!",error_info);
						if (AfxMessageBox(error_info,MB_YESNO|MB_ICONQUESTION)!=IDYES)
						{
							return FALSE;
						}
					}
				}
			}
			else if(bom_set.m_Type.MakeLower()=="prt" && bom_set.m_PartNum>0)
			{
				prt_ins_set.m_strFilter.Format("[PrtNum]=%ld",bom_set.m_PartNum);
				prt_ins_set.Requery();
				if (!prt_ins_set.IsBOF())
				{
					prt_ins_set.MoveFirst();
					if (UsrCheckCurrentMdl(PRO_MDL_PART,prt_ins_set.m_SolidFileName,prt_ins_set.m_PrtNum)==CHECK_SNAME_NNUM)
					{
						error_info.Format("系统中已经有相同的文件名(%s)存在",prt_ins_set.m_SolidFileName);
						error_info.Format("%s是否继续打开!",error_info);
						if (AfxMessageBox(error_info,MB_YESNO|MB_ICONQUESTION)!=IDYES)
						{
							return FALSE;
						}
					}
				}
			}
			bom_set.MoveNext();
		} while(!bom_set.IsEOF());
	}
	return TRUE;
}
/************************************************************************/
/* 把模型导进来                                                         */
/************************************************************************/
BOOL CAsmOpen::LoadSolid(long num)
{
	CFtpFile                m_ftp;
	ProError                status;
	CString                 fullFileName,fileTemp,csError;//包括路径的文件名
	ProPath                 name;
	ProType                 type;
	ProName                 w_name;
	CStringArray			files,localfiles;       //需要下载的文件,记录文件在服务器上的存放位置数组
	int                     w_id;

	CNProductInsInfoTable   asm_set(m_pSet->m_pDatabase);
	CNAsmBomTable           bom_set(m_pSet->m_pDatabase);
	CAsmManageTableSet      asm_m_set(m_pSet->m_pDatabase);
	CNPartInsInfoTable      prt_ins_set(m_pSet->m_pDatabase);
	//得到所有元件的存放位置
	bom_set.m_strFilter.Format("[AsmNum]=%ld",num);
	bom_set.Open();
	prt_ins_set.Open();
	asm_set.Open();
	if (!bom_set.IsBOF())
	{
		bom_set.MoveFirst();
		do {
			//获得零部件的存放位置跟地址
			if (bom_set.m_Type.MakeLower()=="asm" && bom_set.m_PartNum>0 )
			{
					asm_set.m_strFilter.Format("[PdtNum]=%ld",bom_set.m_PartNum);
					asm_set.Requery();
					if (!asm_set.IsBOF())
					{
						asm_set.MoveFirst();
						if(asm_set.m_PDMState=="提交")
						{
							CString  fileTemp1="",fileTemp2="";
							if (!UsrGetPartAndProductFromPdm(asm_set.m_PDMNum,0,fileTemp1,fileTemp2))
							{
								fileTemp2.Format("组件%s在PDM系统中没法找到",asm_set.m_SolidFileName );
								AfxMessageBox(fileTemp2);
								return FALSE;
							}
							if(!fileTemp1.IsEmpty()) files.Add(fileTemp1);
							if(!fileTemp2.IsEmpty()) files.Add(fileTemp2);
							//从PDM去查找
						}/////////////////////////////////////////////////////////////////////////
						else if (asm_set.m_LocalState=="本地")
						{
									//从本地电脑，注意可能不是本机电脑，
									//如果是本机电脑，则不用下载
									fileTemp=asm_set.m_SolidFilePlace+"\\"+asm_set.m_SolidFileName;
									int Flag=0;
									for (int i=0;i<localfiles.GetCount();i++)
									{
										if(localfiles[i]==fileTemp) Flag=1;
									}
									if(Flag==0) localfiles.Add(fileTemp);
									//判断有无工程图
									if (!asm_set.m_DrawingFileName.IsEmpty())
									{
										fileTemp=asm_set.m_DrawingFilePlace +"\\"+asm_set.m_DrawingFileName;
										int Flag=0;
										for (int i=0;i<localfiles.GetCount();i++)
										{
											if(localfiles[i]==fileTemp) Flag=1;
										}
										if(Flag==0) localfiles.Add(fileTemp);
									}
						}////////////////////////////////////////////////////////////////////////
						else if (asm_set.m_LocalState=="服务器")
						{
										fileTemp=asm_set.m_SolidFilePlace+"/"+asm_set.m_SolidFileName+".1";
										int Flag=0;
										for (int i=0;i<files.GetCount();i++)
										{
											if(files[i]==fileTemp) Flag=1;
										}
										if(Flag==0) files.Add(fileTemp);
										//工程图
										if (!asm_set.m_DrawingFileName.IsEmpty())
										{
											fileTemp=asm_set.m_DrawingFilePlace +"/"+asm_set.m_DrawingFileName+".1";
											int Flag=0;
											for (int i=0;i<files.GetCount();i++)
											{
												if(files[i]==fileTemp) Flag=1;
											}
											if(Flag==0) files.Add(fileTemp);
										}
						}
					}
					else
					{
						CString tempstr;
						tempstr.Format("组件%s,在组件库中已经不存在，打开失败！",bom_set.m_FileName );
						AfxMessageBox(tempstr);
						return  FALSE;
					}
					/////////////////////////////////////////////////////////////////////////
			}
			else if(bom_set.m_Type.MakeLower()=="prt" && bom_set.m_PartNum>0)
			{
					prt_ins_set.m_strFilter.Format("[PrtNum]=%ld",bom_set.m_PartNum);
					prt_ins_set.Requery();
					if (!prt_ins_set.IsBOF())
					{
						prt_ins_set.MoveFirst();
						if(prt_ins_set.m_PDMState=="提交")
						{
							CString  fileTemp1="",fileTemp2="";
							if (!UsrGetPartAndProductFromPdm(prt_ins_set.m_PDMNum,0,fileTemp1,fileTemp2))
							{
								fileTemp2.Format("组件%s在PDM系统中没法找到",prt_ins_set.m_SolidFileName );
								AfxMessageBox(fileTemp2);
								return FALSE;
							}
							if(!fileTemp1.IsEmpty()) files.Add(fileTemp1);
							if(!fileTemp2.IsEmpty()) files.Add(fileTemp2);
							//从PDM去查找
						}///////////////////////////////////////////////////////////////////////
						else if (prt_ins_set.m_LocalState=="本地")
						{
							//从本地电脑，注意可能不是本机电脑，
							//如果是本机电脑，则不用下载
							fileTemp=prt_ins_set.m_SolidPlace+"\\"+prt_ins_set.m_SolidFileName;
							int Flag=0;
							for (int i=0;i<localfiles.GetCount();i++)
							{
								if(localfiles[i]==fileTemp) Flag=1;
							}
							if(Flag==0) localfiles.Add(fileTemp);
							//判断有无工程图
							if (!prt_ins_set.m_DrawingName.IsEmpty())
							{
								fileTemp=prt_ins_set.m_DrawingPlace +"\\"+prt_ins_set.m_DrawingName ;
								int Flag=0;
								for (int i=0;i<localfiles.GetCount();i++)
								{
									if(localfiles[i]==fileTemp) Flag=1;
								}
								if(Flag==0) localfiles.Add(fileTemp);
							}
						}/////////////////////////////////////////////////////////////////////////
						else if (prt_ins_set.m_LocalState=="服务器")
						{
							fileTemp=prt_ins_set.m_SolidPlace +"/"+prt_ins_set.m_SolidFileName+".1";
							int Flag=0;
							for(int i=0;i<files.GetCount();i++)
							{
								if(files[i]==fileTemp) Flag=1;
							}
							if(Flag==0) files.Add(fileTemp);
							//添加工程图
							if (!prt_ins_set.m_DrawingName.IsEmpty())
							{
								fileTemp=prt_ins_set.m_DrawingPlace +"/"+prt_ins_set.m_DrawingName+".1";
								int Flag=0;
								for(int i=0;i<files.GetCount();i++)
								{
									if(files[i]==fileTemp) Flag=1;
								}
								if(Flag==0) files.Add(fileTemp);
							}
						}
					}
					else
					{
						CString tempstr;
						tempstr.Format("零件%s,在组件库中已经不存在，打开失败！",bom_set.m_FileName  );
						AfxMessageBox(tempstr);
						return  FALSE;
					}
					////////////////////////////////////////////////////////////////////////////////////
			}
			bom_set.MoveNext();
		} while(!bom_set.IsEOF());
	}

    
	//1.选择保存位置,建立文件夹
	CString   newDir;
	if (!SelectAnDirectory(localDir,""))
	{
		return FALSE;
	}	

	asm_m_set.m_strFilter.Format("[Number]=%ld",num);
	asm_m_set.Open();
	if(asm_m_set.IsBOF()) return FALSE;
	asm_m_set.MoveFirst();
	if (!CreateDirectoryGivenPathAndName(localDir,asm_m_set.m_Name,newDir))
	{
		return FALSE;
	}
	localDir=localDir+newDir;//c:\\dd\\new


	//2.下载文件到文件夹/*
	files.Add(asm_m_set.m_SolidFilePlace +"/"+asm_m_set.m_SolidFileName+".1");
	if (!asm_m_set.m_DrawingFileName.IsEmpty())
	{
		files.Add(asm_m_set.m_DrawingFilePlace +"/"+asm_m_set.m_DrawingFileName+".1");
	}
    if (!m_ftp.DownloadFilestoDirectory(&files,localDir,myFtp,csError))
    {
		AfxMessageBox(csError);
		return FALSE;
    }
    //2.2检查本地文件是否存在
	if (!IsFilesExists(localfiles,csError))
	{
		AfxMessageBox(csError);
		return FALSE;
	}


    //3.把模型导入内存（包括工程图）
	fullFileName=localDir+"\\"+asm_m_set.m_SolidFileName;
	ProStringToWstring(name,fullFileName.GetBuffer());
	status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&asm_solid); 
	if (!asm_m_set.m_DrawingFileName.IsEmpty() )
	{
		fullFileName=localDir+"\\"+asm_m_set.m_DrawingFileName;
		ProStringToWstring(name,fullFileName.GetBuffer());
		status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&asm_drw); 
	}

	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("打开模型失败");
		m_pSet->Close();
		return FALSE;
	}

	status = ProMdlTypeGet(asm_solid, (ProMdlType*)&type);
	status=  ProMdlNameGet(asm_solid, w_name);
	status=  ProObjectwindowCreate(w_name, type, &w_id);
	if(!status== PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	status = ProMdlDisplay(asm_solid);
	status = ProWindowActivate(w_id);	
	return TRUE;
}
/************************************************************************/
/* 把组件的基本信息添加进来                                             */
/************************************************************************/
BOOL CAsmOpen::AddAsmSolidInfo(long num)
{
	
	CNAsmType      asm_type(m_pSet->m_pDatabase);
	m_pSet->m_strFilter.Format("[Number]=%ld",num);
	m_pSet->Requery() ;
	if (!m_pSet->IsBOF())
	{
		m_pSet->MoveFirst();
		UsrPartPorp tempProp;
		tempProp.owner=asm_solid;
		tempProp.IsAsmComp=false;
		tempProp.Number=num;
		tempProp.Name=m_pSet->m_Name;
		tempProp.Person=m_pSet->m_Designer;
		tempProp.Code=m_pSet->m_Code;
		tempProp.IsAudited=true;
		tempProp.Material="";
		tempProp.MdlName="";
		tempProp.MdlNum=-1;
		tempProp.TypeNum=m_pSet->m_TypeNum;
		tempProp.MdlType=3;  //表明是模型
		asm_type.m_strFilter.Format("[Number]=%ld",m_pSet->m_TypeNum);
		asm_type.Open();
		if(!asm_type.IsBOF())
		{
			asm_type.MoveFirst();
			tempProp.TypeName=asm_type.m_Name;
		}
		if (asm_drw!=NULL)
		{
			tempProp.bHaveDraw=true;
			tempProp.hDraw=asm_drw;
		}
		tempProp.ResourceType=1;
		tempProp.Note=m_pSet->m_Note;
		//把模型信息添加到全局变量
		SetMdlInfo(asm_solid,tempProp);
		return TRUE;
	}
	return FALSE;
}

/************************************************************************/
/* 添加元件信息                                                         */
/************************************************************************/
BOOL CAsmOpen::AddCompInfo(long num)
{
	CNProductInsInfoTable   asm_set(m_pSet->m_pDatabase);
	CNAsmBomTable           bom_set(m_pSet->m_pDatabase);
	CAsmManageTableSet      asm_m_set(m_pSet->m_pDatabase);
	CNPartInsInfoTable      prt_ins_set(m_pSet->m_pDatabase);
	ProMdl					mdl;
	ProName					name;
	CString					csName;
	//得到所有元件的存放位置
	bom_set.m_strFilter.Format("[AsmNum]=%ld",num);
	bom_set.Open();
	prt_ins_set.Open();
	asm_set.Open();
	if (!bom_set.IsBOF())
	{
		bom_set.MoveFirst();
		do {
			//获得零部件的存放位置跟地址
			if (bom_set.m_Type.MakeLower()=="asm" && bom_set.m_PartNum>0 )
			{
				asm_set.m_strFilter.Format("[PdtNum]=%ld",bom_set.m_PartNum);
				asm_set.Requery();
				if (!asm_set.IsBOF())
				{
					asm_set.MoveFirst();
					csName=asm_set.m_SolidFileName.Left(asm_set.m_SolidFileName.GetLength()-4);
					ProStringToWstring(name,csName.GetBuffer());
					if (ProMdlInit(name,PRO_MDL_ASSEMBLY,&mdl)==PRO_TK_NO_ERROR)
					{
						//添加部件的信息
						AddCompAsmInfo((ProSolid)mdl,bom_set.m_PartNum);
					}
				}
			}
			else if(bom_set.m_Type.MakeLower()=="prt" && bom_set.m_PartNum>0)
			{
				prt_ins_set.m_strFilter.Format("[PrtNum]=%ld",bom_set.m_PartNum);
				prt_ins_set.Requery();
				if (!prt_ins_set.IsBOF())
				{
					prt_ins_set.MoveFirst();
					csName=prt_ins_set.m_SolidFileName.Left(prt_ins_set.m_SolidFileName.GetLength()-4);
					ProStringToWstring(name,csName.GetBuffer());
					if (ProMdlInit(name,PRO_MDL_PART,&mdl)==PRO_TK_NO_ERROR)
					{
						AddCompPartInfo((ProSolid)mdl,bom_set.m_PartNum);
					}
				}
			}
			bom_set.MoveNext();
		} while(!bom_set.IsEOF());
	}
	return TRUE;
}
BOOL CAsmOpen::AddCompPartInfo(ProSolid mdl,long PrtNum)             //给零件添加信息
{
	CNPartInsInfoTable		m_tempSet(m_pSet->m_pDatabase);	
	CNPartTypeSet           m_parttypeset(m_pSet->m_pDatabase);
	CPartMdlRecordSet       m_partmdlset(m_pSet->m_pDatabase);
	UsrPartPorp             temp_porp;
	ProDrawing              mdl_drw;
	/*CString					csName;*/
	m_tempSet.m_strFilter.Format("[PrtNum]=%ld",PrtNum);
	m_tempSet.Open();
	if(!m_tempSet.IsBOF())
	{
		m_tempSet.MoveFirst();
		temp_porp.owner  = mdl;
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
		temp_porp.IsAsmComp=true;
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
		//设置工程图
		if (!m_tempSet.m_DrawingName.IsEmpty() 
			&& LoadCompDrawing(temp_porp.ResourceType,m_tempSet.m_DrawingName,m_tempSet.m_DrawingPlace,&mdl_drw)==PRO_TK_NO_ERROR)
		{
			temp_porp.bHaveDraw=true;
			temp_porp.hDraw=mdl_drw;
		}
		else
		{
			temp_porp.bHaveDraw=false;
			temp_porp.hDraw=NULL;
		}

	    SetMdlInfo(mdl,temp_porp);		

		if (temp_porp.MdlType==1)
		{
			//把模型的信息都添加进来
			UsrAddPartMdlOtherInfo(m_pSet->m_pDatabase,mdl,m_tempSet.m_MdlNum);
		}
	}
	return TRUE;
}
BOOL CAsmOpen::AddCompAsmInfo(ProSolid mdl,long num)              //给组件添加信息
{
	CNProductInsInfoTable asm_set(m_pSet->m_pDatabase);
	CNAsmType			  asm_type(m_pSet->m_pDatabase);
	CAsmManageTableSet    asmmdlset(m_pSet->m_pDatabase);
	ProDrawing            mdl_drw=NULL;
	/*CString				  csName;*/
	asm_set.m_strFilter.Format("[PdtNum]=%ld",num);
	asm_set.Open();
	if (!asm_set.IsBOF())
	{
		asm_set.MoveFirst();
		UsrPartPorp temp_porp;
		temp_porp.owner=mdl;
		temp_porp.Number=num;
		switch(asm_set.m_MdlNum) {
		case -1:
			temp_porp.MdlType=2;
			temp_porp.MdlNum=-1;
			break;
		default:
			temp_porp.MdlType=1;
			temp_porp.MdlNum=asm_set.m_MdlNum;
			break;
		}
		temp_porp.Name=asm_set.m_Name;
		temp_porp.Code=asm_set.m_InsCode;
		//获得模型的类别
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
		temp_porp.Person="";
		temp_porp.Note=asm_set.m_Note;
		temp_porp.IsAudited=(asm_set.m_AuditState==_T("审核通过")?true:false);
		//文件的来源
		if (asm_set.m_PDMState==_T("提交"))
			temp_porp.ResourceType=2;
		else if(asm_set.m_LocalState==_T("本地"))
			temp_porp.ResourceType=0;
		else if(asm_set.m_LocalState==_T("服务器"))
			temp_porp.ResourceType=1;
		//获得模型的名称
		if (asm_set.m_MdlNum>0)
		{
			asmmdlset.m_strFilter.Format("[Number]=%ld",asm_set.m_MdlNum);
			asmmdlset.Open();
			asmmdlset.Requery();
			if (!asmmdlset.IsBOF())
			{
				asmmdlset.MoveFirst();
				temp_porp.MdlName=asmmdlset.m_Name;
			}
		}
		else temp_porp.MdlName="无模型";

		//设置工程图
		if (!asm_set.m_DrawingFileName.IsEmpty() 
			&& LoadCompDrawing(temp_porp.ResourceType,asm_set.m_DrawingFileName,asm_set.m_DrawingFilePlace,&mdl_drw)==PRO_TK_NO_ERROR)
		{
			temp_porp.bHaveDraw=true;
			temp_porp.hDraw=mdl_drw;
		}
		else
		{
			temp_porp.bHaveDraw=false;
			temp_porp.hDraw=NULL;
		}
		SetMdlInfo(mdl,temp_porp);

		if (temp_porp.MdlType==1)
		{			//给模型添加信息
			asm_inf_fn.AddAsmInsInfo(m_pSet->m_pDatabase,mdl,num);
		}
	}
	return TRUE;
}
ProError CAsmOpen::LoadCompDrawing(int ResourceType,CString filename,CString filepath,ProDrawing * comp_dra)
{
	ProPath  name;
	CString  csName;
	ProError status;
	if (filepath.Right(1)!="\\")
	{
		filepath+="\\";
	}	
	switch(ResourceType) {
	case 0:
		csName=filepath+filename;
		break;
	case 1:
		csName=localDir+"\\"+filename;
		break;
	case 2:
		csName=localDir+"\\"+filename;
		break;
	default:
		return PRO_TK_NOT_VALID;
		break;
	}
	ProStringToWstring(name,csName.GetBuffer());
	status=ProMdlLoad(name,PRO_MDL_DRAWING,PRO_B_FALSE,(ProMdl*)comp_dra);
	return status;
}