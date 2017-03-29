// CommonFuncClass.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "CommonFuncClass.h"
#include "MyDatabase.h"
#include <ctype.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <commctrl.h>
#include "afxtempl.h"
////
#include "ProSolid.h"
#include "ProWindows.h"
#include "ProDimension.h"
#include "ProUtil.h"
#include "ProParameter.h"
#include <ProParamval.h>
#include <ProMdl.h>
#include "ProMessage.h"
////
#include "ProModelitem.h"
#include "ProFeature.h"
#include "ProAsmcomp.h"
#include "ProFeatType.h"
#include <ProAsmcomppath.h>
#include "Compute.h"
#include "DetailInfoClass.h"
#include "FtpFile.h"
#include <ProDrawing.h>
#include "ProductClass.h"


// CCommonFuncClass
void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data,BOOL nFlag);
extern CMyDatabase m_db;
extern CArray <Database,Database> Database_struc_temp;
extern CArray <JuBing,JuBing> JuBing_struc;
CArray <JuBing,JuBing> JuBing_struc_temp;
CArray <FilePath,FilePath> FilePath_struc;
extern FtpInfo Ftp_struc;
CArray<ItemHandle1,ItemHandle1> treehandle1;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;
CStringArray NumArray;                    //基本结构的编号数组，在保存失败的时候用

CCommonFuncClass::CCommonFuncClass()
{
	
}

CCommonFuncClass::~CCommonFuncClass()
{
}


// CCommonFuncClass 成员函数
/************************************************************************/
/* 得到最顶层的产品句柄                                                 */
/************************************************************************/
ProSolid CCommonFuncClass::ObtainTheTopSolid(ProSolid solid)		//当前句柄
{
	ProMdl *p_mdlarray;
	CArray <ProMdl,ProMdl> p_promdl;
	int     p_count=0;
	int num=0;
	ProSessionMdlList(PRO_MDL_ASSEMBLY,&p_mdlarray,&p_count);
	if (p_count==0)
	{
		return solid;
	}
	else
	{
		for (int i=0;i<p_count;i++)
		{
			p_promdl.Add(p_mdlarray[i]);
		}
		RemoveTheSubAamNotContainCurrent(&p_promdl,solid);
		if (p_promdl.GetSize()==0)
		{
			return solid;
		}
	}
	if (p_promdl.GetSize()>1)
	{
		if(!RemoveTheSubAam(&p_promdl))
		{
			AfxMessageBox("你打开了两个以上的产品，请先关闭其中的一个！");
			p_promdl[0]=NULL;			  //返回NULL
		}
	}
	return (ProSolid)p_promdl[0];	  //返回产品句柄
}
/************************************************************************/
/* 去掉不包括当前模型的装配体                                           */
/************************************************************************/
BOOL CCommonFuncClass::RemoveTheSubAamNotContainCurrent(CArray <ProMdl,ProMdl> *asmarray,ProSolid solid)
{
       BOOL IsHave=FALSE;
	   for (int i=0;i<asmarray->GetSize();i++)
	   {
			   if (!IsHaveCurrentSolid((ProSolid)(*asmarray)[i],solid))
			   {
				   IsHave=TRUE;	  
				   asmarray->RemoveAt(i);
			   }
	   }
	   if (IsHave)
	   {
		   RemoveTheSubAamNotContainCurrent(asmarray,solid);
	   }
	   return TRUE;
}
/************************************************************************/
/* 得到包含当前零件部件的所有组件                                       */
/************************************************************************/
BOOL CCommonFuncClass::GetAllAsmContainCurrentMdl(ProSolid ProductSolid,ProSolid CurrentSolid,CArray <ProSolid,ProSolid> *asmarray)
//1.产品的句柄   2.当前要保存的零件或者部件   3.包含当前零件或者部件的并且属于这个产品的所有部件
{
	 CCompute compute;
	 JuBing_struc.RemoveAll();
	 ProMdlType p_type;
	 ProError status;
	 if (!compute.ObtainJuBing(ProductSolid))
	 {
		 AfxMessageBox("无法得到这个产品的下层零部件");
		 return FALSE;
	 }
	 IsHandleAlreadyHave();
	 for (int i=0;i<JuBing_struc.GetSize();i++)
	 {
		  status=ProMdlTypeGet((ProMdl)JuBing_struc[i].solid,&p_type);
		  if (status!=PRO_TK_NO_ERROR)
		  {
			  return FALSE;
		  }
		  else
		  {
			  if (p_type==PRO_MDL_PART)
			  {
				  continue;
			  }
			  else
				  if (p_type==PRO_MDL_ASSEMBLY)
				  {
					  //判断这个是否就是原来的那个
					  if (JuBing_struc[i].solid==CurrentSolid)
					  {
						  continue;
					  }
					  if (!IsHaveCurrentSolid(JuBing_struc[i].solid,CurrentSolid))
					  {
						  continue;
					  }
					  else
					  {
						  asmarray->Add(JuBing_struc[i].solid);
					  }
				  }
		  }
	 }
	 return TRUE;
}
/************************************************************************/
/* 添加产品的参数信息到参数中                                                 */
/************************************************************************/
BOOL CCommonFuncClass::SetProductPara(ProSolid ProductsSolid)  //1. 产品句柄
{
	CCompute compute;
	CString str;
	//ProLine line;
	ProModelitem modelitem;
	ProParameter param;
	ProParamvalue p_value;
	ProError status;
	ProName w_name;
	JuBing_struc.RemoveAll();
	if(!compute.ObtainJuBing(ProductsSolid))
	{
		return FALSE;
	}
	else
	{
		for (int i=0;i<JuBing_struc.GetSize();i++)
		{
			status=ProMdlToModelitem((ProMdl)JuBing_struc[i].solid,&modelitem);
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
			ProStringToWstring(w_name,"RL_Mdl");
			status=ProParameterInit(&modelitem,w_name,&param);
			if (status==PRO_TK_E_NOT_FOUND)
			{
				//新建参数
				p_value.type=PRO_PARAM_STRING;
				ProStringToWstring(p_value.value.s_val,str.GetBuffer());
				status=ProParameterCreate(&modelitem,w_name,&p_value,&param);
			}
			if (status==PRO_TK_NO_ERROR)
			{
				int s=IsBasicInfoHave(JuBing_struc[i].solid);
				if(s==-1)
				{
					return FALSE;
				}
				else
					if (!SetParam(s))
					{
						return FALSE;
					}
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 添加模型号信息到参数中                                                 */
/************************************************************************/
//BOOL CCommonFuncClass::AddMdlInfo(ProSolid ProductsSolid)  //1. 产品句柄
//{
//	CCompute compute;
//	CString str;
//	ProLine line;
//	ProModelitem modelitem;
//	ProParameter param;
//	ProParamvalue p_value;
//	ProError status;
//	ProName w_name;
//	JuBing_struc.RemoveAll();
//	if(!compute.ObtainJuBing(ProductsSolid))
//	{
//		return FALSE;
//	}
//	else
//	{
//		for (int i=0;i<JuBing_struc.GetSize();i++)
//		{
//			status=ProMdlToModelitem((ProMdl)JuBing_struc[i].solid,&modelitem);
//			if (status!=PRO_TK_NO_ERROR)
//			{
//				return FALSE;
//			}
//			ProStringToWstring(w_name,"RL_Mdl");
//			status=ProParameterInit(&modelitem,w_name,&param);
//			if (status==PRO_TK_E_NOT_FOUND)
//			{
//				//新建参数
//				p_value.type=PRO_PARAM_STRING;
//				ProStringToWstring(p_value.value.s_val,str.GetBuffer());
//				status=ProParameterCreate(&modelitem,w_name,&p_value,&param);
//			}
//			if (status==PRO_TK_NO_ERROR)
//			{
//				int s=IsBasicInfoHave(JuBing_struc[i].solid);
//				if(s==-1)
//				{
//					str="";
//				}
//				else
//				{
//					str=((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum;
//				}
//				ProStringToWstring(line,str.GetBuffer());
//				status=ProParamvalueSet(&p_value,(void*)line,PRO_PARAM_STRING);
//				status=ProParameterValueSet (&param,&p_value);
//			}
//			else
//				continue;
//		}
//	}
//	return TRUE;
//}
/************************************************************************/
/* 这个组件模型是否包含当前的模型                                       */
/************************************************************************/
BOOL CCommonFuncClass::IsHaveCurrentSolid(ProSolid Asm,ProSolid Currentsolid)
{
	ProFeature *p_feature;
	ProMdl mdl;
	ProMdlType p_type;
	ProError status;
	CArray <ProSolid,ProSolid> solidarray;
	solidarray.Add(Asm);
	SolidFeatGet(Asm,&p_feature,FALSE);
	int n;
	ProArraySizeGet((ProArray)p_feature,&n);
	for (int i=0;i<n;i++)
	{
		status=ProAsmcompMdlGet(( ProAsmcomp*)(p_feature+i),&mdl);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("无法得到产品的零件");
			return FALSE;
		}
		status=ProMdlTypeGet(mdl,&p_type);
		if (p_type==PRO_ASSEMBLY)
		{
			AddPartSolid((ProSolid)mdl,&solidarray);
		}
		else
			if (p_type==PRO_PART)
			{
				 solidarray.Add((ProSolid)mdl);
			}
	}
	for(int i=0;i<solidarray.GetSize();i++)
	{
		if (solidarray[i]==Currentsolid)
		{
			return TRUE;
		}
	}
	return FALSE;
}
/************************************************************************/
/* 当是部件的时候得到部件的下层零部件的句柄和零件路径                   */
/************************************************************************/
void CCommonFuncClass::AddPartSolid(ProSolid solid,CArray <ProSolid,ProSolid> *solidarray)		 //部件的句柄
{
	ProError status;
	ProFeature *p_feature;
	ProMdl   mdl;
	ProMdlType p_type;
	solidarray->Add(solid);
	SolidFeatGet(solid,&p_feature,FALSE);
	int n;
	ProArraySizeGet((ProArray)p_feature,&n);
	for (int i=0;i<n;i++)
	{
		status=ProAsmcompMdlGet(( ProAsmcomp*)(p_feature+i),&mdl);
		status=ProMdlTypeGet(mdl,&p_type);
		if (p_type==PRO_ASSEMBLY)
		{
			AddPartSolid((ProSolid)mdl,solidarray);
		}
		else
			if (p_type==PRO_PART)
			{
				solidarray->Add((ProSolid)mdl);
			}
	}
}
/************************************************************************/
/* 去掉包含当前句柄的子装配体                                           */
/************************************************************************/
BOOL CCommonFuncClass::RemoveTheSubAam(CArray <ProMdl,ProMdl> *asmarray)
{
	BOOL IsHave=FALSE;
	for (int i=0;i<asmarray->GetSize();i++)
   {
	   for (int j=i+1;j<asmarray->GetSize();j++)
	   {
		   if (IsHaveCurrentSolid((ProSolid)(*asmarray)[i],(ProSolid)(*asmarray)[j]))
		   {
			   IsHave=TRUE;	  
			   asmarray->RemoveAt(j);
		   }
	   }
   }
   if (IsHave)
   {
	   RemoveTheSubAam(asmarray);
   }
   else
	   if(asmarray->GetSize()>1)
       {
	        return FALSE;
       }
	   else
		   return TRUE;
   return TRUE;
}
/************************************************************************/
/* 备份要保存的零件                                                     */
/************************************************************************/
BOOL CCommonFuncClass::RemoveTempFile(CString localpath)
{

	CFileFind ff;

	BOOL a=ff.FindFile(localpath);
	if (a)
	{
		RemoveDireAndFile(localpath);
	}
	if(::CreateDirectory(localpath,NULL)==0)
		return FALSE;
	return TRUE;
}
/************************************************************************/
/* 去掉drawing文件夹中非DRW的文件                                       */
/************************************************************************/
BOOL CCommonFuncClass::RemoveNotDrw(CString DrawingPath)
{
	CFileFind ff;
	if (DrawingPath=="")
	{
		return FALSE;
	}
	CString szDir = DrawingPath;
	if(szDir.Right(1) != "\\")
		szDir += "\\";
	szDir += "*.*";
	BOOL res = ff.FindFile(szDir);
	while(res)
	{
		res = ff.FindNextFile();
		if(!ff.IsDirectory() && !ff.IsDots()) //到达最低层的文件
		{
			//显示当前访问的文件
			CString strPath="";			
			strPath = ff.GetFilePath();
			strPath.MakeLower();
			int k=strPath.Find("drw");
			if (k==-1)
			{
				DeleteFile(strPath);
			}
		}
	}	
	ff.Close();
	return TRUE;
}
/************************************************************************/
/* 去掉本地的文件夹和里面的文件                                         */
/************************************************************************/
BOOL CCommonFuncClass::RemoveDireAndFile(CString localdir)
{
	CFileFind ff;
	CString szDir = localdir;
	if(szDir.Right(1) != "\\")
		szDir += "\\";
	szDir += "*.*";

	BOOL res = ff.FindFile(szDir);
	while(res)
	{
		res = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots())       //目录是文件夹
		{
			//如果是一个子目录，用递归继续往深一层找
			CString strPath = ff.GetFilePath();    //得到路径，做为递归调用的开始			         	
			RemoveDireAndFile(strPath);            //递归调用
		}
		else if(!ff.IsDirectory() && !ff.IsDots()) //到达最低层的文件
		{
			//显示当前访问的文件
			CString strPath="";			
			strPath = ff.GetFilePath();
			DeleteFile(strPath);
		}
	}	
	ff.Close();                                   //关闭
	RemoveDirectory(_T(localdir+"\\"));	
	return 1;
}
/************************************************************************/
/* 把不在进程中的模型的基本信息去掉                                     */
/************************************************************************/
void CCommonFuncClass::RemoveNotInSession()
{
	ProMdl *p_mdlarray;
	CArray <ProMdl,ProMdl> p_promdl;
	int     p_count=0;
	int num=0;
	ProSessionMdlList(PRO_MDL_ASSEMBLY,&p_mdlarray,&p_count);
	for (int i=0;i<p_count;i++)
	{
		p_promdl.Add(p_mdlarray[i]);
	}
	ProSessionMdlList(PRO_MDL_PART,&p_mdlarray,&p_count);
	for (int i=0;i<p_count;i++)
	{
		p_promdl.Add(p_mdlarray[i]);
	}
	for (int i=0;i<p_classarray.GetSize();i++)
	{
		if (!IsInSession(((DetailInfoClass *)p_classarray.GetAt(i))->solid,&p_promdl))
		{
			((DetailInfoClass *)p_classarray.GetAt(i))->PerformanceValue.RemoveAll();
			((DetailInfoClass *)p_classarray.GetAt(i))->InsValueStr.RemoveAll();
			delete (DetailInfoClass *)p_classarray.GetAt(i);
			p_classarray.RemoveAt(i);
		}
	}
}
BOOL CCommonFuncClass::IsInSession(ProSolid solid,CArray <ProMdl,ProMdl> *p_promdl)
{
	for (int i=0;i<p_promdl->GetSize();i++)
	{
		if (solid==(ProSolid)((*p_promdl)[i]))
		{
			return TRUE;
			break;
		}
	}
	return FALSE;
}
/************************************************************************/
/* 检查基本信息是否有重复的，如果有则去掉                               */
/************************************************************************/
BOOL CCommonFuncClass::IsBasicInfoAlreadyHave()
{
	BOOL IsOk=TRUE;  
	for (int i=0;i<p_classarray.GetSize();i++)
	{
		for (int j=i+1;j<p_classarray.GetSize();j++)
		{
			if(((DetailInfoClass *)p_classarray.GetAt(j))->solid==((DetailInfoClass *)p_classarray.GetAt(i))->solid && ((DetailInfoClass *)p_classarray.GetAt(i))->solid!=NULL && ((DetailInfoClass *)p_classarray.GetAt(j))->solid)
			{
				IsOk=FALSE;
				((DetailInfoClass *)p_classarray.GetAt(i))->PerformanceValue.RemoveAll();
				((DetailInfoClass *)p_classarray.GetAt(i))->InsValueStr.RemoveAll();
				delete (DetailInfoClass *)p_classarray.GetAt(i);
				p_classarray.RemoveAt(i);
				break;
			}
		}
		if (!IsOk)
		{
			break;
		}  
	}
	if (!IsOk)
	{
		IsBasicInfoAlreadyHave();
	}
	else
	{
		return TRUE;
	}
	return TRUE;
}
/************************************************************************/
/* 去掉产品中重复的句柄，以防重复保存（只在入库的时候用）               */
/************************************************************************/
BOOL CCommonFuncClass::IsHandleAlreadyHave()
{
	BOOL IsOk=TRUE;  
	for (int i=0;i<JuBing_struc.GetSize();i++)
	{
		for (int j=i+1;j<JuBing_struc.GetSize();j++)
		{
			if(JuBing_struc[j].solid==JuBing_struc[i].solid)
			{
				IsOk=FALSE;
				JuBing_struc.RemoveAt(i);
				break;
			}
		}
		if (!IsOk)
		{
			break;
		}  
	}
	if (!IsOk)
	{
		IsHandleAlreadyHave();
	}
	else
	{
		return TRUE;
	}
	return TRUE;
}
/************************************************************************/
/* 把句柄数组保存在临时句柄数组中                                               */
/************************************************************************/
void CCommonFuncClass::ChangeJuBingToJuBingTemp()
{
	JuBing_struc_temp.RemoveAll();
	for (int i=0;i<JuBing_struc.GetSize();i++)
	{
		JuBing uu;
		uu.comppath=JuBing_struc[i].comppath;
		uu.Name=JuBing_struc[i].Name;
		uu.number=JuBing_struc[i].number;
		uu.Productsolid=JuBing_struc[i].Productsolid;
		uu.solid=JuBing_struc[i].solid;
		uu.upnum=JuBing_struc[i].upnum;
		uu.Type=JuBing_struc[i].Type;
		JuBing_struc_temp.Add(uu);
	}
}

/************************************************************************/
/* 去掉重复的零部件件名字                                               */
/************************************************************************/
BOOL CCommonFuncClass::RemoveNameFromNameArray(CStringArray *NameArray)
{
	BOOL IsHave=FALSE;
	for (int i=0;i<NameArray->GetSize();i++)
	{
		for (int j=i+1;j<NameArray->GetSize();j++)
		{
			if ((*NameArray)[i]==(*NameArray)[j])
			{
				IsHave=TRUE;
				NameArray->RemoveAt(i);
			}
		}
	}
	if (IsHave)
	{
	   RemoveNameFromNameArray(NameArray);
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* 判断名字是否和原来的一样                                             */
/************************************************************************/
BOOL CCommonFuncClass::IsNameNeedAlter(ProSolid solid,CString Name)
{
	ProError status;
	ProName w_name;
	CString WName;
	status=ProMdlNameGet((ProMdl)solid,w_name);
	Name.MakeLower();
	WName=((CString)w_name);
	if (Name==WName.MakeLower())
	{
		return FALSE;
	}
	else
		return TRUE;
}
/************************************************************************/
/* 保存产品                                                             */
/************************************************************************/
BOOL CCommonFuncClass::SaveProductFile(ProSolid solid,CString SolidPlace,CString DrwPlace)
{
	CCompute compute;
	JuBing_struc.RemoveAll();
	if (!compute.ObtainJuBing(solid))
	{
		return FALSE;
	}
	IsHandleAlreadyHave();
	for (int i=0;i<JuBing_struc.GetSize();i++)
	{
		 if (!SaveFile(JuBing_struc[i].solid,SolidPlace,DrwPlace,DrwPlace))
		 {
			 return FALSE;
		 }
	}
	return TRUE;
}
/************************************************************************/
/* 保存文件                                                             */
/************************************************************************/
BOOL CCommonFuncClass::SaveFile(ProSolid solid,CString SolidPlace,CString DrwPlace,CString SolidName)
{
	CString temppath,path,temp;
	ProError status;
	ProMdldata p_mdldata;
	temppath=SolidPlace;
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
	//备份二维图
	DrawingList.RemoveAll();
	GetSolidDrawing(solid,&DrawingList);
	for (int i=0;i<DrawingList.GetSize();i++)
	{
		status=ProMdlDataGet((ProMdl)DrawingList[i],&p_mdldata);
		if (status!=PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
		temppath=DrwPlace;
		path=temppath+"\\";

		ProStringToWstring(p_mdldata.path,path.GetBuffer());
		status=ProMdlBackup((ProMdl)DrawingList[i],&p_mdldata);
		if (status!=PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* 删除指定的文件                                                       */
/************************************************************************/
BOOL CCommonFuncClass::DeleteSpecialFile(CString LocalPlace, CString Name)   //1.指定目录   2.文件名，不包括版本信息
{
	CString path;
	path=FindFile(LocalPlace+"\\"+Name+".*");
	while (path!="")
	{
		DeleteFile(path);
		path=FindFile(LocalPlace+"\\"+Name+".*");
	}
	return TRUE;
}
/************************************************************************/
/* 判断新文件是否存在，如果存在返回这个文件的绝对路径，输入文件的绝对路径
               包括版本,       如       1111.prt.*	  这种格式
/************************************************************************/
CString CCommonFuncClass::FindFile(CString FilePath)   
{
	CFileFind finder;
	CString strpath="";
	BOOL bWorking = finder.FindFile(FilePath);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strpath=finder.GetFilePath();
	}
	finder.Close();
	return strpath;
}
/************************************************************************/
/* 是否存在当前句柄的基本信息                                           */
/************************************************************************/
int CCommonFuncClass::IsBasicInfoHave(CString InsCode)
{
	int i;
	InsCode.MakeLower();
	for (i=0;i<p_classarray.GetSize();i++)
	{
		((DetailInfoClass *)p_classarray.GetAt(i))->InsCode.MakeLower();
		if (((DetailInfoClass *)p_classarray.GetAt(i))->InsCode==InsCode)
		{
			return i;
		}
	}
	return -1;
}
/************************************************************************/
/* 是否存在当前句柄的基本信息                                           */
/************************************************************************/
int CCommonFuncClass::IsBasicInfoHave(ProSolid solid)
{
	int i;
	for (i=0;i<p_classarray.GetSize();i++)
	{
		if (((DetailInfoClass *)p_classarray.GetAt(i))->solid==solid)
		{
			return i;
		}
	}
	return -1;
}
/************************************************************************/
/* 根据零件的名字得到得到零件的句柄                                     */
/************************************************************************/
BOOL CCommonFuncClass::ObtianHandleFromName(int s)//产品句柄
{
	CCompute compute;
	CString Name,str;
	ProName w_name;
	ProMdlType p_type;
	if (!compute.ObtainJuBing(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
	{
		AfxMessageBox("无法得到句柄数组！");
		return FALSE;
	}
	IsHandleAlreadyHave();
	
	for (int i=1;i<JuBing_struc.GetSize();i++)
	{
		ProMdlTypeGet(JuBing_struc[i].solid,&p_type);
		if (p_type==PRO_MDL_PART)
		{
			ProMdlNameGet((ProMdl)JuBing_struc[i].solid,w_name);
			Name=CString(w_name)+".prt";
			Name.MakeLower();
		}
		else
			if (p_type=PRO_MDL_ASSEMBLY)
			{
				ProMdlNameGet((ProMdl)JuBing_struc[i].solid,w_name);
				Name=CString(w_name)+".asm";
				Name.MakeLower();
			}
		BOOL IsHave=FALSE;
		for (int j=0;j<p_classarray.GetSize();j++)
		{
			if (((DetailInfoClass *)p_classarray.GetAt(j))->ProductID==((DetailInfoClass *)p_classarray.GetAt(s))->ProductID)  //属于同一个产品
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName.MakeLower()==Name)
				{
					IsHave=TRUE;
					((DetailInfoClass *)p_classarray.GetAt(s))->solid=JuBing_struc[i].solid;
					break;
				}
			}
		}
		if (!IsHave)
		{
			str.Format("零件名为%s无法得到句柄",Name);
			AfxMessageBox(str);
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* 根据句柄得到得到零件的基本信息                                       */
/************************************************************************/
BOOL CCommonFuncClass::ObtianBasicInfoFromHandle(ProSolid solid)//产品句柄
{
	CCompute compute;
	CString Name,str;
	ProName w_name;
	ProMdlType p_type;
	JuBing_struc.RemoveAll();
	if (!compute.ObtainJuBing(solid))
	{
		AfxMessageBox("无法得到句柄数组！");
		return FALSE;
	}
	IsHandleAlreadyHave();
	NumArray.RemoveAll();
	for (int i=0;i<JuBing_struc.GetSize();i++)
	{
		BOOL IsHave=FALSE;
		for (int j=0;j<p_classarray.GetSize();j++)
		{
			
			if (((DetailInfoClass *)p_classarray.GetAt(j))->solid==JuBing_struc[i].solid)
			{
				IsHave=TRUE;
				str.Format("%d",j);
				NumArray.Add(str);
				break;
			}
	    }
		if (!IsHave)
		{
			ProMdlTypeGet(JuBing_struc[i].solid,&p_type);
			if (p_type==PRO_MDL_PART)
			{
				ProMdlNameGet((ProMdl)JuBing_struc[i].solid,w_name);
				Name=CString(w_name)+".prt";
				Name.MakeLower();
			}
			else
				if (p_type=PRO_MDL_ASSEMBLY)
				{
					ProMdlNameGet((ProMdl)JuBing_struc[i].solid,w_name);
					Name=CString(w_name)+".asm";
					Name.MakeLower();
				}
			str.Format("零件名为%s无法得到句柄",Name);
			AfxMessageBox(str);
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* 去掉重复的零件编号，以防重复下载                                     */
/************************************************************************/
BOOL CCommonFuncClass::IsSolidIDAlreadyHave()
{
	BOOL IsOk=TRUE;  
	for (int i=0;i<FilePath_struc.GetSize();i++)
	{
		for (int j=i+1;j<FilePath_struc.GetSize();j++)
		{
			if(FilePath_struc[j].SolidID==FilePath_struc[i].SolidID)
			{
				IsOk=FALSE;
				FilePath_struc.RemoveAt(i);
				break;
			}
		}
		if (!IsOk)
		{
			break;
		}  
	}
	if (!IsOk)
	{
		IsSolidIDAlreadyHave();
	}
	else
	{
		return TRUE;
	}
	return TRUE;
}
/************************************************************************/
/* 判断本地这个零件文件是否存在,都需要有后缀，.prt/.asm/.drw            */
/************************************************************************/
BOOL CCommonFuncClass::IsFileExist(CString SolidPlace,CString DrwPlace,CString SolidName,CString DrwName)
{
	//判断新文件名是否存在
	CString Path,temppath,str;
	if (SolidPlace!="" && SolidName!="")
	{
		if (((SolidName.Right(4))==".prt") || (SolidName.Right(4)==".asm"))
		{
			;//Path=SolidPlace+"\\"+SolidName+".*";	
		}
		else
		{
			int k=SolidName.ReverseFind('.');
			SolidName.Delete(k,(SolidName.GetLength()-k));
		}
		Path=SolidPlace+"\\"+SolidName+".*";
		temppath=FindFile(Path);
		if (temppath!="")
		{
			//在判断这个路径是否是原来的路径
			//str.Format("%s这个零件已经存在，是否替换",SolidName);
			//if(AfxMessageBox(str,MB_YESNO)==IDYES)
			//{
				while(FindFile(Path)!="")					//当是本地时判断
				{
					DeleteSpecialFile(SolidPlace,SolidName);//删除零件
				}
			//}
			//else
				//return FALSE;
		}
	}
	if (DrwPlace!="" && DrwName!="")
	{
		if (DrwName.Right(4)!=".drw")
		{
			int k=DrwName.ReverseFind('.');
			DrwName.Delete(k,(SolidName.GetLength()-k));
		}
		Path=DrwPlace+"\\"+DrwName+".*";
		temppath=FindFile(Path);
		if (temppath!="")
		{
			//在判断这个路径是否是原来的路径
			//str.Format("%s这个零件已经存在，是否替换",DrwName);
			//if(AfxMessageBox(str,MB_YESNO)==IDYES)
			//{
				while(FindFile(Path)!="")					//当是本地时判断
				{
					DeleteSpecialFile(DrwPlace,DrwName);//删除零件
				}
			//}
			//else
				//return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* 判断这个二维图文件是否存在                                           */
/************************************************************************/
BOOL CCommonFuncClass::IsDrwFileExist(CString DrwFile,CString DrwPlace)
{
	CString Path,DrwName,temppath,str;
	int k=DrwFile.Find(".");
	if (k!=-1)
	{
		Path=DrwPlace+"\\"+DrwFile+".*";
		DrwName=DrwFile;
	}
	else
	{
		Path=DrwPlace+"\\"+DrwFile+".drw.*";
		DrwName=DrwFile+".drw";
	}
	temppath=FindFile(Path);
	if (temppath!="")
	{
		//在判断这个路径是否是原来的路径
		str.Format("%s这个文件已经存在，是否替换",DrwName);
		if(AfxMessageBox(str,MB_YESNO)==IDYES)
		{
			while(FindFile(Path)!="")					//当是本地时判断
			{
				DeleteSpecialFile(DrwPlace,DrwName);//删除零件
			}
		}
		else
			return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 判断新保存的路径和原来是否是一样的                                   */
/************************************************************************/
BOOL CCommonFuncClass::IsPathTheSame(ProSolid solid,CString Path,CString Name)
{
	ProMdldata p_data;
	ProError  status;
	CString Currentpath,post,NewPath;
	NewPath=Path+"\\"+Name;
	status=ProMdlDataGet((ProMdl)solid,&p_data);
	if(status==PRO_TK_NO_ERROR)
	{
	    if(((CString)p_data.type).MakeLower()=="prt")
		{
			post=".prt";
		}
		else
			if (((CString)p_data.type).MakeLower()=="asm")
			{
				post=".asm";
			}
			else
				if (((CString)p_data.type).MakeLower()=="drw")
				{
					post=".drw";
				}
		Currentpath=(CString)p_data.device+":"+(CString)p_data.path+(CString)p_data.name+post;
		if (Currentpath.MakeLower()==NewPath.MakeLower())
		{
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
	return TRUE;
}
/************************************************************************/
/* 插入新零件                                                           */
/************************************************************************/
BOOL CCommonFuncClass::InsertNewPart(int s,BOOL NeedTrans)
{
	CString str;
	if (!InsertDatabase(s))
	{
		return FALSE;
	}
	str="SELECT * FROM NPartInsInfoTable";
	m_db.Query(str);
	m_db.MoveEnd();
	str.Format("%d",m_db.getInt("PrtNum"));
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum=="-1")
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=atoi(str);
	}
	else
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=atoi(str);
		//插入事物特性表和性能参数表]
		if (InsertVariantAndPerf(s,atoi(str)))
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=atoi(str);
		}
		else
		{
			return FALSE;
		}
	}  
	if (NeedTrans)
	{
		if (!UpLoadFile(s))
		{
			return FALSE;
		}		
	}
	return TRUE;
}
/************************************************************************/
/* 插入零件数据库                                                           */
/************************************************************************/
BOOL CCommonFuncClass::InsertDatabase(int s)
{
	CString sql;
	CCompute compute;
	sql.Format("INSERT INTO NPartInsInfoTable VALUES('%s','%s',%d,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
		((DetailInfoClass *)p_classarray.GetAt(s))->InsName,((DetailInfoClass *)p_classarray.GetAt(s))->InsCode,
		((DetailInfoClass *)p_classarray.GetAt(s))->GroupNum,atol(((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum),
		((DetailInfoClass *)p_classarray.GetAt(s))->Material,((DetailInfoClass *)p_classarray.GetAt(s))->Unit,
		((DetailInfoClass *)p_classarray.GetAt(s))->Designer,((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime,
		((DetailInfoClass *)p_classarray.GetAt(s))->Auditor,((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime,
		((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,
		((DetailInfoClass *)p_classarray.GetAt(s))->DrwName,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,
		((DetailInfoClass *)p_classarray.GetAt(s))->DesignState,((DetailInfoClass *)p_classarray.GetAt(s))->AuditState,
		((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer,((DetailInfoClass *)p_classarray.GetAt(s))->PDMState,
		((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode,((DetailInfoClass *)p_classarray.GetAt(s))->Note);
	if(m_db.Execute(sql)==0)
	{
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 插入零件的性能参数和事物特性表                                       */
/************************************************************************/
BOOL CCommonFuncClass::InsertVariantAndPerf(int s,int PrtNum)
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

	for (int i=0;i<(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).GetSize();i++)
	{
		sql.Format("INSERT INTO NPartInsPerfTable VALUES (%d,%d,%d,'%s')",PrtNum,i+1,
			atol(((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum),
			(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue)[i]);
		if(m_db.Execute(sql)==0)
		{
			return FALSE;
		}
	}
	for (int i=0;i<Database_struc_temp.GetSize();i++)
	{
		sql.Format("INSERT INTO NPartInsVariantTable VALUES (%d,%d,%d,'%s')",PrtNum,i+1,
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
/* 更新零件的性能参数和事物特性表                                       */
/************************************************************************/
BOOL CCommonFuncClass::UpdateVariantAndPerf(int s)
{
	CString sql;
	for (int i=0;i<(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).GetSize();i++)
	{
		sql.Format("UPDATE NPartInsPerfTable Set Value='%s' where PrtNum=%d AND Num=%d",
			(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue)[i],
			((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,i+1);
		if(m_db.Execute(sql)==0)
		{
			return FALSE;
		}			
	}
	for (int i=0;i<Database_struc_temp.GetSize();i++)
	{
		sql.Format("UPDATE NPartInsVariantTable Set Value='%s' where PrtNum=%d AND Num=%d",
			Database_struc_temp[i].value,((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,i+1);
		if(m_db.Execute(sql)==0)
		{
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* 更新数据库的内容                                                     */
/************************************************************************/
BOOL CCommonFuncClass::UpdateNewPart(int s,BOOL NeedTrans)
{
	CString sql;
	CCompute compute;
	/////
	sql.Format("UPDATE NPartInsInfoTable Set Name='%s',InsCode='%s',TypeNum=%d,Material='%s',Designer='%s',FinishTime='%s',Auditor='%s',AuditTime='%s',SolidFileName='%s',SolidPlace='%s',DrawingName='%s',DrawingPlace='%s',FinishState='%s',AuditState='%s',LocalState='%s',PDMState='%s',PDMNum='%s',Note='%s' where PrtNum=%d",
		((DetailInfoClass *)p_classarray.GetAt(s))->InsName,((DetailInfoClass *)p_classarray.GetAt(s))->InsCode,((DetailInfoClass *)p_classarray.GetAt(s))->GroupNum,
		((DetailInfoClass *)p_classarray.GetAt(s))->Material,((DetailInfoClass *)p_classarray.GetAt(s))->Designer,
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
			if(!compute.GetVariantForPart(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum,TRUE))
			{
				return FALSE;	 //把主变量读到临时结构中
			}                   
			if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
			{
				return FALSE;
			}
			if (!UpdateVariantAndPerf(s))
			{
				return FALSE;
			}
		}
		if (NeedTrans)
		{
			if (!UpLoadFile(s))
			{
				return FALSE;
			}
		}
	return TRUE;
}
/************************************************************************/
/* 查找要下载的模型是否在内存中存在,返回值，ture是有，false是没有       */
/************************************************************************/
BOOL CCommonFuncClass::IsAlreadyInTheSession(CString Name,CArray <ProSolid,ProSolid> *p_arraylist)
{
	//还需要判断二维图是否存在；
	CString FileName,Post;
	ProMdlType p_type;
	ProName w_name;
	ProError status;
	CCommonFuncClass commonfun;
	for (int i=0;i<(*p_arraylist).GetSize();i++)
	{
		status=ProMdlNameGet((ProMdl)((*p_arraylist)[i]),w_name);
		if (status!=PRO_TK_NO_ERROR)
		{
			return TRUE;
		}
		status=ProMdlTypeGet((ProMdl)((*p_arraylist)[i]),&p_type);
		if (status!=PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
		if (p_type==PRO_MDL_PART)
		{
			Post=".prt";
		}
		else
			if (p_type==PRO_MDL_ASSEMBLY)
			{
				Post=".asm";
			}
			else
				if (p_type==PRO_MDL_DRAWING)
				{
					Post=".drw";
				}
		FileName=CString(w_name)+Post;
		if (Name.MakeLower()==FileName.MakeLower())
		{
			return TRUE;
		}
	}
	return FALSE;
}
/************************************************************************/
/* 得到所有在内存中存在的句柄                                           */
/************************************************************************/
BOOL CCommonFuncClass::GetAlreadyInTheSession(CArray <ProSolid,ProSolid> *p_arraylist)
{
	//还需要判断二维图是否存在；
	ProMdl *p_mdlarray;
	int     p_count=0;
	int i;
	ProSessionMdlList(PRO_MDL_ASSEMBLY,&p_mdlarray,&p_count);
	for (i=0;i<p_count;i++)
	{
		p_arraylist->Add((ProSolid)p_mdlarray[i]);
	}
	ProSessionMdlList(PRO_MDL_PART,&p_mdlarray,&p_count);
	for (i=0;i<p_count;i++)
	{
		p_arraylist->Add((ProSolid)p_mdlarray[i]);
	}
	ProSessionMdlList(PRO_MDL_DRAWING,&p_mdlarray,&p_count);
	for (i=0;i<p_count;i++)
	{
		p_arraylist->Add((ProSolid)p_mdlarray[i]);
	}
	return TRUE;
}
/************************************************************************/
/*拭除内存函数                                                          */
/************************************************************************/
BOOL CCommonFuncClass::RemoveMemory(ProSolid Productsolid)
{
	CCompute compute;
	ProError status;
	ProMdlType p_type;
	DrawingList.RemoveAll();
	status=ProMdlTypeGet(Productsolid,&p_type);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	else
	{
		if (p_type==PRO_MDL_PART)
		{
			GetSolidDrawing(Productsolid,&DrawingList);
		}
		else
			if (p_type==PRO_MDL_ASSEMBLY)
			{
				JuBing_struc.RemoveAll();
				compute.ObtainJuBing(Productsolid);
				IsHandleAlreadyHave();
				for (int i=0;i<JuBing_struc.GetSize();i++)
				{
					GetSolidDrawing(JuBing_struc[i].solid,&DrawingList);
				}
			}
	}
	if (DrawingList.GetSize()==0)
	{
		status=ProMdlEraseAll(Productsolid);
		if (status!=PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
	}  
	else
	{
		for (int i=0;i<DrawingList.GetSize();i++)
		{
			status=ProMdlEraseAll(DrawingList[i]);
			if (status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 判断零件实例是否和参数相符合                                         */
/************************************************************************/
int CCommonFuncClass::IsPartExist(ProSolid solid,CString MdlNum)
{
	CString sql,str;
	int InsNum,SolidID;
	CStringArray AllNumber;
	BOOL IsHave=FALSE;
	CCompute compute;
	Database_struc_temp.RemoveAll();
	if(!compute.GetVariantForPart(solid,MdlNum,TRUE))
	{
		return -1;
	}
	if(!compute.ObtainValueFromMdlTemp(solid))
	{
		return -1;
	}
	sql.Format("SELECT * FROM NPartInsInfoTable WHERE MdlNum=%s",MdlNum);
	if (m_db.Query(sql)==0)
	{
		SolidID=-1;
		return SolidID;		//不存在返回为-1；
	}
	else
	{
		//得到实例号
		m_db.MoveBegin();
		do {
			InsNum=m_db.getInt("PrtNum");
			str.Format("%d",InsNum);
			AllNumber.Add(str);
		} while(m_db.MoveNext());
		for (int i=0;i<AllNumber.GetSize();i++)
		{
			sql.Format("SELECT * FROM NPartInsVariantTable WHERE PrtNum=%s",AllNumber[i]);
			//里面尺寸值是值是否相等 ，如果相等则标注借用，不相等则提示其改图号
			if(!GetDimInfo(MdlNum,sql,(int)Database_struc_temp.GetSize(),&IsHave))
			{
				return -1;
			}
			else
			{
				if(IsHave)
				{
					SolidID=atol(AllNumber[i]);
					break;
				}
			}
		}
		if (!IsHave)
		{
			SolidID=-1;
		}
	}
	return SolidID;
}
/************************************************************************/
/* 判断产品实例是否和参数相符合                                         */
/************************************************************************/
int CCommonFuncClass::IsAsmExist(ProSolid solid,CString MdlNum)
{
	CString sql,str;
	int InsNum,SolidID,Z;
	CStringArray AllNumber;
	JuBing uu;
	CCompute compute;
	BOOL IsHave=FALSE;
	Database_struc_temp.RemoveAll();
	if(!compute.ObtainAsmFromBaseTemp(solid,MdlNum))
	{
		return -1;
	}
	if(!compute.ObtainValueFromMdlTemp(solid))
	{
		return -1;
	}
	sql.Format("SELECT * FROM NProductInsInfoTable WHERE MdlNum=%s",MdlNum);
	Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		return -1;		//不存在返回为“”；
	}
	else
	{
		//得到实例号
		m_db.MoveBegin();
		do {
			InsNum=m_db.getInt("PdtNum");
			str.Format("%d",InsNum);
			AllNumber.Add(str);
		} while(m_db.MoveNext());
		for (int i=0;i<AllNumber.GetSize();i++)
		{
			sql.Format("SELECT * FROM NProductInsVariantTable WHERE PdtNum=%s",AllNumber[i]);
			//里面尺寸值是值是否相等 ，如果相等则标注借用，不相等则提示其改图号
			if(!GetDimInfo(MdlNum,sql,(int)Database_struc_temp.GetSize(),&IsHave))
			{
				return -1;
			}
			else
			{  
				if (IsHave)
				{
					SolidID=atol(AllNumber[i]);
					break;
				}
			}
		}
	}
	if (!IsHave)
	{
		SolidID=-1;
	}
	return SolidID;
}
/************************************************************************/
/* 在实例存在的情况下，比较图和实例中的值，如两者全相等则返回正确       */
/************************************************************************/
BOOL CCommonFuncClass::GetDimInfo(CString AsmNum,CString strSql,int variantnum,BOOL *IsHave)
{
	int j=0;
	CCompute compute;
	float test;
	int Z=m_db.Query(strSql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z!=variantnum)
	{
		*IsHave=FALSE;
		return TRUE;
	}
	if(Z==0)
	{ 
        *IsHave=TRUE;
		return TRUE;
	}
	else
	{
		//得到变量值
		m_db.MoveBegin();
		CString str;
		do {
			int k=m_db.getInt("Num");
			str=m_db.getString("Value");
			for (int i=0;i<variantnum;i++)
			{
				if (Database_struc_temp[i].number==k)
				{
					test=(float)atof(str);
					test=(float)atof(Database_struc_temp[i].value);
					if (atof(str)!=atof(Database_struc_temp[i].value))
					{
						*IsHave=FALSE;
						return TRUE;
					}
					break;
				}
			}
		} while(m_db.MoveNext());
	}
	*IsHave=TRUE;
	return TRUE;
}
/************************************************************************/
/* 检查图号和实例号是否符合,输入，图号和零件实例号 ,正确返回-1，不正确返回零件实例号                     */
/************************************************************************/
int CCommonFuncClass::IsPrtInsCodeAndPrtNumOk(CString InsCode,int SolidID)
{
	 CString sql;
	 int SolidIDtemp=-2;
	 sql.Format("SELECT * FROM NPartInsInfoTable WHERE InsCode='%s'",InsCode);
	 int Z=m_db.Query(sql);
	 if (Z==-1)
	 {
		 return -1;
	 }
	 if (Z==0)
	 {
		 return -1;
	 }
	 else
	 {
		 m_db.MoveBegin();
		 do {
		 	SolidIDtemp=m_db.getInt("PrtNum");
			break;
		 } while(m_db.MoveNext());
	 }
	 if (SolidID==SolidIDtemp)
	 {
		 return -1;
	 }
	 else
		 return SolidIDtemp;
}
/************************************************************************/
/* 备份产品文件，备份在以图号为名字的文件夹中                           */
/************************************************************************/
BOOL CCommonFuncClass::BackUpFile(int s)		//产品句柄
{
	CCompute compute;
	ProError status;
	CFileFind ff;
	CString  temppath,path,device;
	ProMdldata p_mdldata;

	CFtpFile ftp;
	CCommonFuncClass commonfun;
	CString FullLocalPath,strLocalFilePath;
	TCHAR TempPath[MAX_PATH];
	GetTempPath(MAX_PATH,TempPath);

	FullLocalPath=CString(TempPath)+((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
	((DetailInfoClass *)p_classarray.GetAt(s))->TempPath=FullLocalPath;
	((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathSolid=FullLocalPath+"Solid";
	((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathDrw=FullLocalPath+"Drw";
	//备份文件
	ftp.RemoveDireAndFile(FullLocalPath);
	BOOL a=ff.FindFile(FullLocalPath);
	if (!a)
	{
		if(::CreateDirectory(FullLocalPath,NULL)==0)
			return FALSE;
	}
	temppath=FullLocalPath;
	status=ProMdlDataGet((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid,&p_mdldata);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	path=temppath+"\\";
	ProStringToWstring(p_mdldata.path,path.GetBuffer());
	if (!UsrMdlBackUp((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid,&p_mdldata))
	{
		return FALSE;
	}
	temppath=FullLocalPath+"\\drawing";
	a=ff.FindFile(temppath);
	if (!a)
	{
		if(::CreateDirectory(temppath,NULL)==0)
			return FALSE;
	}
	DrawingList.RemoveAll();
	GetSolidDrawing(((DetailInfoClass *)p_classarray.GetAt(s))->solid,&DrawingList);
	for (int i=0;i<DrawingList.GetSize();i++)
	{
		a=ff.FindFile(temppath);
		if (!a)
		{
			if(::CreateDirectory(temppath,NULL)==0)
				return FALSE;
		}
		ProMdlDataGet((ProMdl)DrawingList[i],&p_mdldata);
		path=path+"drawing\\";
		ProStringToWstring(p_mdldata.path,path.GetBuffer());
		if (!UsrMdlBackUp((ProMdl)DrawingList[i],&p_mdldata))
		{
			return FALSE;
		}
	}
	//如果有改目录的话，查找服务器是否已经有这个文件,要判断是二维的还是三维
	if (!ftp.FindFile(((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathSolid,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->DrwName!="")				//如果有二维图
	{
		if (!ftp.FindFile(((DetailInfoClass *)p_classarray.GetAt(s))->DrwName,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathDrw,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
		{
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/*备份到某个地方，在备份到原来的位置                                    */
/************************************************************************/
BOOL CCommonFuncClass::UsrMdlBackUp(ProMdl mdl,ProMdldata * data)
{
	ProError     status;
	ProMdldata   olddata;
	ProPath      path;
	CString      temp;
	swprintf(path,(data->path));
	status=ProMdlDataGet(mdl,&olddata);
	if (status!=PRO_TK_NO_ERROR) return FALSE;
	(*data)=olddata;
	swprintf(data->path,path);
	status=ProMdlBackup(mdl,data);
	if (status!=PRO_TK_NO_ERROR) return FALSE;
	temp.Format("%s:%s",CString(olddata.device),CString(olddata.path));
	ProStringToWstring(olddata.path,temp.GetBuffer());
	status=ProMdlBackup(mdl,&olddata);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 保存到服务器函数  ，用在单个零件设计中                                                   */
/************************************************************************/
BOOL CCommonFuncClass::SavePartToTheServerforPart(int s)
{
	CFtpFile ftp;
	CCommonFuncClass commonfun;
	CString strLocalFilePath;
	//备份文件

	m_db.m_pConnection->Close();
	m_db.ReOpen();
	try
	{
		m_db.m_pConnection->BeginTrans();
		if (!SavePartToTheServerforProduct(s))
		{
             m_db.m_pConnection->RollbackTrans();
			 return FALSE;
		}
		m_db.m_pConnection->CommitTrans();
		return TRUE;
	}
	catch (_com_error e)
	{
		m_db.m_pConnection->RollbackTrans();
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 保存到服务器函数，用在产品设计中                                                   */
/************************************************************************/
BOOL CCommonFuncClass::SavePartToTheServerforProduct(int s)
{
	CFtpFile ftp;
	CCommonFuncClass commonfun;
	CString strLocalFilePath;
	if (!commonfun.BackUpFile(s))
	{
		return FALSE;
	}
	//如果有改目录的话，查找服务器是否已经有这个文件,要判断是二维的还是三维
	if (!ftp.FindFile(((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathSolid,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->DrwName!="")				//如果有二维图
	{
		if (!ftp.FindFile(((DetailInfoClass *)p_classarray.GetAt(s))->DrwName,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathDrw,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
		{
			return FALSE;
		}
	}
	//先写数据表；
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
	{
		if (!commonfun.InsertNewPart(s,TRUE))
		{
			return FALSE;
		}//Insert数据库
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)
	{
		if (!commonfun.UpdateNewPart(s,TRUE))
		{
			return FALSE;
		}//Update数据库
	}
	//上传文件
	strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->TempPath+"\\"+((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName+".1";
	if (!ftp.UploadFiletoDirectory(strLocalFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
	{
		return FALSE;
	}
	strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->TempPath+"\\drawing\\"+((DetailInfoClass *)p_classarray.GetAt(s))->DrwName+".1";
	if (!ftp.UploadFiletoDirectory(strLocalFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
	{
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* //把产品模型添加到树结构控件中去                                     */
/************************************************************************/
BOOL CCommonFuncClass::AddProductToTree(ProSolid solid,CTreeCtrl *m_tree)
{
	ProError status;
	ProMdlType p_type;
	status=ProMdlTypeGet((ProMdl)solid,&p_type);
	if (p_type==PRO_ASSEMBLY)
	{
		AddAssemblyToTree(NULL,solid,m_tree);
	}
	else
		if (p_type==PRO_PART)
		{
			AddConponentToTree(NULL,(ProMdl)solid,m_tree);
		}
		return TRUE;
}
/************************************************************************/
/* //把单个模型添加到树结构控件中去                                     */
/************************************************************************/
HTREEITEM CCommonFuncClass::AddConponentToTree(HTREEITEM hParent,ProMdl mdl,CTreeCtrl *m_tree)
{
	HTREEITEM hItem;
	ProMdlType p_type;
	int image;
	ProCharName modelname;
	ProName name;
	CString Name;
	ProMdlNameGet(mdl,name);
	ProWstringToString(modelname,name);
	ProMdlTypeGet(mdl,&p_type);
	if (p_type==PRO_MDL_PART)
	{
		Name=CString(modelname)+".prt";
		image=1;
	}
	else
		if (p_type==PRO_MDL_ASSEMBLY)
		{
			Name=CString(modelname)+".asm";
			image=0;
		}
		Name.MakeLower();
		hItem=m_tree->InsertItem(Name,image,image,hParent);
		m_tree->EnsureVisible(hItem);
		return hItem;
}
/************************************************************************/
/* 把组件添加到树控件                                                   */
/************************************************************************/
void CCommonFuncClass::AddAssemblyToTree(HTREEITEM hParent,ProSolid solid,CTreeCtrl *m_tree)
{
	ProMdlType p_type;
	ProFeature *p_feature;
	HTREEITEM hItem;
	ProError status;
	ProMdl mdl;
	hItem=AddConponentToTree(hParent,(ProMdl)solid,m_tree);
	//m_tree->Expand(hItem,TVE_EXPAND);
	SolidFeatGet(solid,&p_feature,FALSE);
	int n;
	ProArraySizeGet((ProArray)p_feature,&n);
	for (int i=0;i<n;i++)
	{
		status=ProAsmcompMdlGet(( ProAsmcomp*)(p_feature+i),&mdl);
		status=ProMdlTypeGet(mdl,&p_type);
		if (p_type==PRO_ASSEMBLY)
		{
			AddAssemblyToTree(hItem,(ProSolid)mdl,m_tree);
		}
		else
			if (p_type==PRO_PART)
			{
				AddConponentToTree(hItem,mdl,m_tree);
			}
	}
	return;
}
/************************************************************************/
/* 判断零件的保存方式                                                   */
/************************************************************************/
BOOL CCommonFuncClass::SaveStyle(ProSolid solid,int s,CString MdlNum)
{
	int SolidID=IsPartExist(solid,MdlNum);
	if(SolidID!=-1)							//说明这个零件已经存在
	{
		//判断这个零件是否已经审核过
		if (!IsAuditPass(SolidID,TRUE))
		{	//根据原来的ID再判断用插入还是
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;		   //用替换
		}
		else
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=2;		   //已经存在无须保存
			AfxMessageBox("这个零件已经存在，无须提交");
			return FALSE;
		}
	}
	else      //说明这个零件是新的，用插入
	{
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidID!=-1)
		{
			//判断下面两个是否一样
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;
		}
		else
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;
		}
	}
	return TRUE;
}
/************************************************************************/
/* 判断零件是否审核通过                                                 */
/************************************************************************/
BOOL CCommonFuncClass::IsAuditPass(int SolidID,BOOL IsPart)
{
	CString sql,AuditState;
	int Z;
	if (IsPart)
	{
		sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%d",SolidID);
	}
	else
		sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%d",SolidID);
	Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			AuditState=m_db.getString("AuditState");
			break;
		} while(m_db.MoveNext());
		if (AuditState=="审核未通过")
		{
			return FALSE;
		}
		else
			return TRUE;
	}
	return TRUE;
}
/************************************************************************/
/* 得到零件模型的所有分类                                               */
/************************************************************************/
BOOL CCommonFuncClass::ObtainPartModelClass(CTreeCtrl *m_tree)
{
	m_tree->DeleteAllItems();
	CString strSql,str,str1;
	int num;
	treehandle1.RemoveAll();
	HTREEITEM h_Item;
	HTREEITEM htop;
	ItemHandle1 handle;
	strSql="select * from NPartType";
	int Z =m_db.Query(strSql);
	if(Z==-1)
	{    
		return FALSE;
	}
	else
		if (Z!=0)
		{
			htop=m_tree->InsertItem("零件分类",NULL,TVI_LAST);
			strSql.Format("select * from NPartType WHERE FatherNum=0");
			Z=m_db.Query(strSql);
			if(Z==-1)
			{
				return FALSE;
			}
			else
				if (Z!=0)
				{
					m_db.MoveBegin();
					do {
						num=m_db.getInt("Number");
						handle.FatherNumber=num;
						str=m_db.getString("Name");
						h_Item=m_tree->InsertItem(str,htop,TVI_LAST);
						m_tree->EnsureVisible(h_Item);
						if (h_Item != NULL)
						{
							m_tree->SetItemData(h_Item, (DWORD)num);
							m_tree->EnsureVisible(h_Item);
						}
						handle.hItem=h_Item;
						treehandle1.Add(handle);
					} while(m_db.MoveNext());
				}
		}
		for(int i=0;i<treehandle1.GetSize();i++)
		{
			num=treehandle1[i].FatherNumber;
			strSql.Format("select * from NPartType WHERE FatherNum=%d",num);
			Z=m_db.Query(strSql);
			if (Z==-1)
			{
				return FALSE;
			}
			if (m_db.Query(strSql)==0)
			{
				continue;
			}
			else
			{
				m_db.MoveBegin();
				do 
				{
					num=m_db.getInt("Number");
					str=m_db.getString("Name");
					h_Item=m_tree->InsertItem(str,treehandle1[i].hItem,TVI_LAST);
					if (h_Item != NULL)
					{
						m_tree->SetItemData(h_Item, (DWORD)num);
					}
					handle.hItem=h_Item;
					handle.FatherNumber=num;
					treehandle1.Add(handle);
				} while(m_db.MoveNext());
			}
		}
		return TRUE;
}
/************************************************************************/
/* 得到零件模型的所有分类                                               */
/************************************************************************/
BOOL CCommonFuncClass::ObtainProductModelClass(CTreeCtrl *m_tree)
{
	m_tree->DeleteAllItems();
	CString strSql,str,str1;
	int num;
	treehandle1.RemoveAll();
	HTREEITEM h_Item;
	HTREEITEM htop;
	ItemHandle1 handle;
	strSql="select * from NAsmType";
	int Z =m_db.Query(strSql);
	if(Z==-1)
	{    
		return FALSE;
	}
	else
		if (Z!=0)
		{
			htop=m_tree->InsertItem("产品类别",NULL,TVI_LAST);
			strSql.Format("select * from NAsmType WHERE FatherNum=0");
			Z=m_db.Query(strSql);
			if(Z==-1)
			{
				return FALSE;
			}
			else
				if (Z!=0)
				{
					m_db.MoveBegin();
					do {
						num=m_db.getInt("Number");
						handle.FatherNumber=num;
						str=m_db.getString("Name");
						h_Item=m_tree->InsertItem(str,htop,TVI_LAST);
						m_tree->EnsureVisible(h_Item);
						if (h_Item != NULL)
						{
							m_tree->SetItemData(h_Item, (DWORD)num);
							m_tree->EnsureVisible(h_Item);
						}
						handle.hItem=h_Item;
						treehandle1.Add(handle);
					} while(m_db.MoveNext());
				}
		}
		for(int i=0;i<treehandle1.GetSize();i++)
		{
			num=treehandle1[i].FatherNumber;
			strSql.Format("select * from NAsmType WHERE FatherNum=%d",num);
			Z=m_db.Query(strSql);
			if (Z==-1)
			{
				return FALSE;
			}
			if (m_db.Query(strSql)==0)
			{
				continue;
			}
			else
			{
				m_db.MoveBegin();
				do 
				{
					num=m_db.getInt("Number");
					str=m_db.getString("Name");
					h_Item=m_tree->InsertItem(str,treehandle1[i].hItem,TVI_LAST);
					if (h_Item != NULL)
					{
						m_tree->SetItemData(h_Item, (DWORD)num);
					}
					handle.hItem=h_Item;
					handle.FatherNumber=num;
					treehandle1.Add(handle);
				} while(m_db.MoveNext());
			}
		}
		return TRUE;
}
/************************************************************************/
/* 判断唯一性，图号，文件名                                             */
/************************************************************************/
BOOL CCommonFuncClass::IsHave(CString sql)
{
	CString str;
	int Z=m_db.Query(sql);
	if (Z==0)
	{
		return FALSE;
	}
		return TRUE;
}
/************************************************************************/
/* 要保存的信息是否符合要求                                             */
/************************************************************************/
BOOL CCommonFuncClass::IsInfoOk(int s,CString InsCode,CString SolidName,CString DrwName)
{
	CString sql,str,InsCode1,SolidName1,DrwName1;
	BOOL IsPart;
	if (SolidName.Right(4)==".prt")
	{
        IsPart=TRUE;
	}
	else
		IsPart=FALSE;
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)		 //表示插入新零件
	{
		//判断图号 ，文件名的唯一性
		sql.Format("SELECT * FROM NPartInsInfoTable where InsCode='%s'",InsCode);
		if (IsHave(sql))
		{
			AfxMessageBox("你的图号已经存在，请修改");
			return FALSE;
		}
		sql.Format("SELECT * FROM NProductInsInfoTable Where InsCode='%s'",InsCode);
		if (IsHave(sql))
		{
			AfxMessageBox("你的图号已经存在，请修改");
			return FALSE;
		}
		if (IsPart)					//说明是零件，判断三维文件名是否存在
		{
			sql.Format("SELECT * FROM NPartInsInfoTable where SolidFileName='%s'",SolidName);
			if (IsHave(sql))
			{
				str.Format("文件%s已经存在，请修改",SolidName);
				AfxMessageBox(str);
				return FALSE;
			}
		}
		else                       //说明是产品，判断三维文件名是否存在
		{
			sql.Format("SELECT * FROM NProductInsInfoTable where SolidFileName='%s'",SolidName);
			if (IsHave(sql))
			{
				str.Format("文件%s已经存在，请修改",SolidName);
				AfxMessageBox(str);
				return FALSE;
			}
		}
		//判断二维文件名是否存在
		sql.Format("SELECT * FROM NPartInsInfoTable where DrawingName='%s'",DrwName);
		if (IsHave(sql))
		{
			str.Format("文件%s已经存在，请修改",DrwName);
			AfxMessageBox(str);
			return FALSE;
		}
	}
	else                 //如果是更新零件，则判断新零件/产品名是否存在
		{
			if (IsPart)
			{
				sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
				int Z=m_db.Query(sql);
				if (Z==0 || Z==-1)
				{
					return FALSE;
				}
				else
				{
					m_db.MoveEnd();
                    InsCode1=m_db.getString("InsCode");
					SolidName1=m_db.getString("SolidFileName");
					DrwName1=m_db.getString("DrawingName");
				}
				if (SolidName1.MakeLower()!=SolidName.MakeLower())
				{
					sql.Format("SELECT * FROM NPartInsInfoTable where SolidFileName='%s'",SolidName);
					if (IsHave(sql))
					{
						str.Format("文件%s已经存在，请修改",SolidName);
						AfxMessageBox(str);
						return FALSE;
					}
				}
			}
			else
			{
				sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
				int Z=m_db.Query(sql);
				if (Z==0 || Z==-1)
				{
					return FALSE;
				}
				else
				{
					m_db.MoveEnd();
					InsCode1=m_db.getString("InsCode");
					SolidName1=m_db.getString("SolidFileName");
					DrwName1=m_db.getString("DrawingName");
				}
				if (SolidName1.MakeLower()!=SolidName.MakeLower())
				{
					sql.Format("SELECT * FROM NProductInsInfoTable where SolidFileName='%s'",SolidName);
					if (IsHave(sql))
					{
						str.Format("文件%s已经存在，请修改",SolidName);
						AfxMessageBox(str);
						return FALSE;
					}
				}
			}
			//判断图号是否已经存在
			if (InsCode.MakeLower()!=InsCode1.MakeLower())
			{
				sql.Format("SELECT * FROM NPartInsInfoTable where InsCode='%s'",InsCode);
				if (IsHave(sql))
				{
					AfxMessageBox("你的图号已经存在，请修改");
					return FALSE;
				}
				sql.Format("SELECT * FROM NProductInsInfoTable where InsCode='%s'",InsCode);
				if (IsHave(sql))
				{
					AfxMessageBox("你的图号已经存在，请修改");
					return FALSE;
				}
			}
			if (DrwName1.MakeLower()!=DrwName.MakeLower())
			{
				sql.Format("SELECT * FROM NPartInsInfoTable where DrawingName='%s'",DrwName);
				if (IsHave(sql))
				{
					str.Format("文件%s已经存在，请修改",DrwName);
					AfxMessageBox(str);
					return FALSE;
				}
			}
		}
		return TRUE;
}
/************************************************************************/
/* 上传文件，如果成功的话，把这个零件的编号记下来                       */
/************************************************************************/
BOOL CCommonFuncClass::UpLoadFile(int s)
{
	CFtpFile ftp;
	CCommonFuncClass commonfun;
	CString FullLocalPath,strLocalFilePath,str;
	TCHAR TempPath[MAX_PATH];
	GetTempPath(MAX_PATH,TempPath);
	FullLocalPath=CString(TempPath)+((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
	((DetailInfoClass *)p_classarray.GetAt(s))->TempPath=FullLocalPath;
	((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathSolid=FullLocalPath+"Solid";
	((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathDrw=FullLocalPath+"Drw";

	//记下s//以后失败的时候要用
	str.Format("%d",s);
	NumArray.Add(str);
	//如果有改目录的话，查找服务器是否已经有这个文件,要判断是二维的还是三维
	if (!ftp.FindFile(((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathSolid,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->DrwName!="")				//如果有二维图
	{
		if (!ftp.FindFile(((DetailInfoClass *)p_classarray.GetAt(s))->DrwName,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathDrw,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
		{
			return FALSE;
		}
	}
	//上传文件
	strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->TempPath+"\\"+((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName+".1";
	if (!ftp.UploadFiletoDirectory(strLocalFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->DrwName!="")				//如果有二维图
	{
		strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->TempPath+"\\drawing\\"+((DetailInfoClass *)p_classarray.GetAt(s))->DrwName+".1";
		if (!ftp.UploadFiletoDirectory(strLocalFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
		{
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* 如果有零件替换的话，判断保存位置是否一致                             */
/************************************************************************/
BOOL CCommonFuncClass::IsTheSavePlaceTheSameIfReplace(BOOL IsLocal)  
{
	//NumArray是从commonfun.ObtianBasicInfoFromHandle(solid)的来的，所以这个函数要先用
	int s;
	for (int i=0;i<NumArray.GetSize();i++)
	{
		s=atol(NumArray[i]);
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1)
		{
			if (IsLocal)
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer!="本地")
				{
					return FALSE;
				}
			}
			else
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="本地")
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 保存前的准备工作                                                     */
/************************************************************************/
//1。得到图的当前的版本信息
BOOL CCommonFuncClass::TheWorkBeforeSave(ProSolid ProductSolid)
{
	ProError status;
	ProMdldata  mdldata;
	CCompute compute;
	CCommonFuncClass commonfun;
	if(!ObtianBasicInfoFromHandle(ProductSolid))               //判断是否都有基本信息
		return FALSE;
	for (int i=0;i<JuBing_struc.GetSize();i++)
	{
		int s=commonfun.IsBasicInfoHave(JuBing_struc[i].solid);
		if (s==-1)
		{
             return FALSE;
		}
		else
		{
			status=ProMdlDataGet((ProMdl)JuBing_struc[i].solid,&mdldata);
			if (status=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
			((DetailInfoClass *)p_classarray.GetAt(s))->SolidVersion=mdldata.version;
			DrawingList.RemoveAll();
			GetSolidDrawing(JuBing_struc[i].solid,&DrawingList);
			for (int i=0;i<DrawingList.GetSize();i++)
			{
				status=ProMdlDataGet(DrawingList[i],&mdldata);
				if(status!=PRO_TK_NO_ERROR)
				{
					return FALSE;
				}
				((DetailInfoClass *)p_classarray.GetAt(s))->DrwVersion=mdldata.version;
			}
			((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag=FALSE;
            ((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID=((DetailInfoClass *)p_classarray.GetAt(s))->SolidID;
		}
	}
	return TRUE;
}
BOOL CCommonFuncClass::TheWorkAferSave(ProSolid ProductSolid,BOOL Success)
{
	CCompute compute;
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	JuBing_struc.RemoveAll();
	if(!compute.ObtainJuBing(ProductSolid))
	{
		return FALSE;
	}
	commonfun.IsHandleAlreadyHave();
	for (int i=0;i<JuBing_struc.GetSize();i++)
	{
		int s=commonfun.IsBasicInfoHave(JuBing_struc[i].solid);
		if (s==-1)
		{
			continue;
		}
		else
		{
			if ((!Success) && ((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag)   //保存失败，去掉高版本，还原服务器
			{
                pdtclass.DeleteOldVersion(JuBing_struc[i].solid,Success,((DetailInfoClass *)p_classarray.GetAt(s))->SolidVersion,((DetailInfoClass *)p_classarray.GetAt(s))->DrwVersion);
				((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
				if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="服务器")
				{
					RollBackFile(s);
				}
			}
			if ((!Success && (!((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag)))
			{
                pdtclass.DeleteOldVersion(JuBing_struc[i].solid,Success,((DetailInfoClass *)p_classarray.GetAt(s))->SolidVersion,((DetailInfoClass *)p_classarray.GetAt(s))->DrwVersion);
				((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
			}
			if (Success)
			{
				pdtclass.DeleteOldVersion(JuBing_struc[i].solid,Success,((DetailInfoClass *)p_classarray.GetAt(s))->SolidVersion,((DetailInfoClass *)p_classarray.GetAt(s))->DrwVersion);
			}
		}	
	}
	return TRUE;
}
/************************************************************************/
/* 当保存失败的时候                                                     */
/************************************************************************/
BOOL CCommonFuncClass::RollBackFile(int s)
{
	CFtpFile ftp;
	CString	strSeverFilePath,strLocalFilePath;

	//除去上传的文件
	strSeverFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath+"/"+((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName+".1";
	ftp.RemoveFile(strSeverFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	strSeverFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath+"/"+((DetailInfoClass *)p_classarray.GetAt(s))->DrwName+".1";
	ftp.RemoveFile(strSeverFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	//上传备份的文件
	strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathSolid+"\\"+((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName+".1";
	ftp.UploadFiletoDirectory(strLocalFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathDrw+"\\"+((DetailInfoClass *)p_classarray.GetAt(s))->DrwName+".1";
	ftp.UploadFiletoDirectory(strLocalFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	return TRUE;
}
/************************************************************************/
/* 在参数中设置瑞立图号和名字，模型号，还有PDM图号                      */
/************************************************************************/
BOOL CCommonFuncClass::SetParam(int s)
{
	ProError status;
	ProModelitem modelitem;
	CString str,Name;
	ProLine line;
	ProName w_name;
	ProParameter param;
	ProParamvalue p_value;

	status=ProMdlToModelitem((ProMdl)((DetailInfoClass *)p_classarray.GetAt(s))->solid,&modelitem);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	//添加实例名
	ProStringToWstring(w_name,"rl_name");
	status=ProParameterInit(&modelitem,w_name,&param);
	if (status==PRO_TK_NO_ERROR)
	{
		ProStringToWstring(line,((DetailInfoClass *)p_classarray.GetAt(s))->InsName.GetBuffer());
		status=ProParamvalueSet(&p_value,(void*)line,PRO_PARAM_STRING);
		status=ProParameterValueSet (&param,&p_value);
	}
	else
		if (status==PRO_TK_E_NOT_FOUND)
		{
			//新建参数
			p_value.type=PRO_PARAM_STRING;
			ProStringToWstring(p_value.value.s_val,((DetailInfoClass *)p_classarray.GetAt(s))->InsName.GetBuffer());
			status=ProParameterCreate(&modelitem,w_name,&p_value,&param);
		}
		else
		    return FALSE;
	//新建瑞立图号
	ProStringToWstring(w_name,"rl_code");
	status=ProParameterInit(&modelitem,w_name,&param);
	if (status==PRO_TK_NO_ERROR)
	{
		ProStringToWstring(line,((DetailInfoClass *)p_classarray.GetAt(s))->InsCode.GetBuffer());
		status=ProParamvalueSet(&p_value,(void*)line,PRO_PARAM_STRING);
		status=ProParameterValueSet (&param,&p_value);
	}
	else
		if (status==PRO_TK_E_NOT_FOUND)
		{
			//新建参数
			p_value.type=PRO_PARAM_STRING;
			ProStringToWstring(p_value.value.s_val,((DetailInfoClass *)p_classarray.GetAt(s))->InsCode.GetBuffer());
			status=ProParameterCreate(&modelitem,w_name,&p_value,&param);
		}
		else
			return FALSE;
	//新建PDM图号
	ProStringToWstring(w_name,"pdm_code");
	status=ProParameterInit(&modelitem,w_name,&param);
	if (status==PRO_TK_NO_ERROR)
	{
		ProStringToWstring(line,((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode.GetBuffer());
		status=ProParamvalueSet(&p_value,(void*)line,PRO_PARAM_STRING);
		status=ProParameterValueSet (&param,&p_value);
	}
	else
		if (status==PRO_TK_E_NOT_FOUND)
		{
			//新建参数
			p_value.type=PRO_PARAM_STRING;
			ProStringToWstring(p_value.value.s_val,((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode.GetBuffer());
			status=ProParameterCreate(&modelitem,w_name,&p_value,&param);
		}
		else
			return FALSE;
	//添加模型号
	ProStringToWstring(w_name,"RL_Mdl");
	status=ProParameterInit(&modelitem,w_name,&param);
	if (status==PRO_TK_E_NOT_FOUND)
	{
		//新建参数
		p_value.type=PRO_PARAM_STRING;
		ProStringToWstring(p_value.value.s_val,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum.GetBuffer());
		status=ProParameterCreate(&modelitem,w_name,&p_value,&param);
	}
	else
	if (status==PRO_TK_NO_ERROR)
	{
	    str=((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum;
		ProStringToWstring(line,str.GetBuffer());
		status=ProParamvalueSet(&p_value,(void*)line,PRO_PARAM_STRING);
		status=ProParameterValueSet (&param,&p_value);
	}
	else 
		return FALSE;
	return TRUE;
}
/************************************************************************/
/* 得到图号信息                                                         */
/************************************************************************/
CString CCommonFuncClass::ObtainPartMap(ProSolid solid)
{
	ProError status;
	ProModelitem modelitem;
	ProCharName cname;
	ProName w_name;
	ProParameter param;
	ProParamvalue p_value;
	ProLine wstr;
	CString partmap;
	////得到图号
	status=ProMdlToModelitem((ProMdl)solid,&modelitem);
	sprintf(cname,"rl_code");
	ProStringToWstring(w_name,cname);
	status=ProParameterInit(&modelitem,w_name,&param);
	if (status!=PRO_TK_NO_ERROR)
	{
		partmap="";
		return partmap;
	}
	status=ProParameterValueGet(&param,&p_value);
	if (status!=PRO_TK_NO_ERROR)
	{
		partmap="";
		return partmap;
	}
	swprintf(wstr,L"%s",p_value.value.s_val);
	partmap=CString(wstr);
	return partmap;
}
/************************************************************************/
/* 添加图号信息到参数中                                                 */
/************************************************************************/
BOOL CCommonFuncClass::SetCodeInfo(ProSolid solid)  //1. 句柄
{
	CString str;
	ProLine line;
	ProModelitem modelitem;
	ProParameter param;
	ProParamvalue p_value;
	ProError status;
	ProName w_name;

	status=ProMdlToModelitem((ProMdl)solid,&modelitem);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	ProStringToWstring(w_name,"文件名");
	status=ProParameterInit(&modelitem,w_name,&param);
	if (status==PRO_TK_E_NOT_FOUND)
	{
		//新建参数
		p_value.type=PRO_PARAM_STRING;
		ProStringToWstring(p_value.value.s_val,str.GetBuffer());
		status=ProParameterCreate(&modelitem,w_name,&p_value,&param);
	}
	if (status==PRO_TK_NO_ERROR)
	{
		int s=IsBasicInfoHave(solid);
		if(s==-1)
		{
			str="";
		}
		else
		{
			str=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
		}
		ProStringToWstring(line,str.GetBuffer());
		status=ProParamvalueSet(&p_value,(void*)line,PRO_PARAM_STRING);
		status=ProParameterValueSet (&param,&p_value);
	}
	return TRUE;
}