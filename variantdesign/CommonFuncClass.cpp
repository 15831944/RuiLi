// CommonFuncClass.cpp : ʵ���ļ�
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
CStringArray NumArray;                    //�����ṹ�ı�����飬�ڱ���ʧ�ܵ�ʱ����

CCommonFuncClass::CCommonFuncClass()
{
	
}

CCommonFuncClass::~CCommonFuncClass()
{
}


// CCommonFuncClass ��Ա����
/************************************************************************/
/* �õ����Ĳ�Ʒ���                                                 */
/************************************************************************/
ProSolid CCommonFuncClass::ObtainTheTopSolid(ProSolid solid)		//��ǰ���
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
			AfxMessageBox("������������ϵĲ�Ʒ�����ȹر����е�һ����");
			p_promdl[0]=NULL;			  //����NULL
		}
	}
	return (ProSolid)p_promdl[0];	  //���ز�Ʒ���
}
/************************************************************************/
/* ȥ����������ǰģ�͵�װ����                                           */
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
/* �õ�������ǰ����������������                                       */
/************************************************************************/
BOOL CCommonFuncClass::GetAllAsmContainCurrentMdl(ProSolid ProductSolid,ProSolid CurrentSolid,CArray <ProSolid,ProSolid> *asmarray)
//1.��Ʒ�ľ��   2.��ǰҪ�����������߲���   3.������ǰ������߲����Ĳ������������Ʒ�����в���
{
	 CCompute compute;
	 JuBing_struc.RemoveAll();
	 ProMdlType p_type;
	 ProError status;
	 if (!compute.ObtainJuBing(ProductSolid))
	 {
		 AfxMessageBox("�޷��õ������Ʒ���²��㲿��");
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
					  //�ж�����Ƿ����ԭ�����Ǹ�
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
/* ��Ӳ�Ʒ�Ĳ�����Ϣ��������                                                 */
/************************************************************************/
BOOL CCommonFuncClass::SetProductPara(ProSolid ProductsSolid)  //1. ��Ʒ���
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
				//�½�����
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
/* ���ģ�ͺ���Ϣ��������                                                 */
/************************************************************************/
//BOOL CCommonFuncClass::AddMdlInfo(ProSolid ProductsSolid)  //1. ��Ʒ���
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
//				//�½�����
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
/* ������ģ���Ƿ������ǰ��ģ��                                       */
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
			AfxMessageBox("�޷��õ���Ʒ�����");
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
/* ���ǲ�����ʱ��õ��������²��㲿���ľ�������·��                   */
/************************************************************************/
void CCommonFuncClass::AddPartSolid(ProSolid solid,CArray <ProSolid,ProSolid> *solidarray)		 //�����ľ��
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
/* ȥ��������ǰ�������װ����                                           */
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
/* ����Ҫ��������                                                     */
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
/* ȥ��drawing�ļ����з�DRW���ļ�                                       */
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
		if(!ff.IsDirectory() && !ff.IsDots()) //������Ͳ���ļ�
		{
			//��ʾ��ǰ���ʵ��ļ�
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
/* ȥ�����ص��ļ��к�������ļ�                                         */
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
		if(ff.IsDirectory() && !ff.IsDots())       //Ŀ¼���ļ���
		{
			//�����һ����Ŀ¼���õݹ��������һ����
			CString strPath = ff.GetFilePath();    //�õ�·������Ϊ�ݹ���õĿ�ʼ			         	
			RemoveDireAndFile(strPath);            //�ݹ����
		}
		else if(!ff.IsDirectory() && !ff.IsDots()) //������Ͳ���ļ�
		{
			//��ʾ��ǰ���ʵ��ļ�
			CString strPath="";			
			strPath = ff.GetFilePath();
			DeleteFile(strPath);
		}
	}	
	ff.Close();                                   //�ر�
	RemoveDirectory(_T(localdir+"\\"));	
	return 1;
}
/************************************************************************/
/* �Ѳ��ڽ����е�ģ�͵Ļ�����Ϣȥ��                                     */
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
/* ��������Ϣ�Ƿ����ظ��ģ��������ȥ��                               */
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
/* ȥ����Ʒ���ظ��ľ�����Է��ظ����棨ֻ������ʱ���ã�               */
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
/* �Ѿ�����鱣������ʱ���������                                               */
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
/* ȥ���ظ����㲿��������                                               */
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
/* �ж������Ƿ��ԭ����һ��                                             */
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
/* �����Ʒ                                                             */
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
/* �����ļ�                                                             */
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
	//���ݶ�άͼ
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
/* ɾ��ָ�����ļ�                                                       */
/************************************************************************/
BOOL CCommonFuncClass::DeleteSpecialFile(CString LocalPlace, CString Name)   //1.ָ��Ŀ¼   2.�ļ������������汾��Ϣ
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
/* �ж����ļ��Ƿ���ڣ�������ڷ�������ļ��ľ���·���������ļ��ľ���·��
               �����汾,       ��       1111.prt.*	  ���ָ�ʽ
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
/* �Ƿ���ڵ�ǰ����Ļ�����Ϣ                                           */
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
/* �Ƿ���ڵ�ǰ����Ļ�����Ϣ                                           */
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
/* ������������ֵõ��õ�����ľ��                                     */
/************************************************************************/
BOOL CCommonFuncClass::ObtianHandleFromName(int s)//��Ʒ���
{
	CCompute compute;
	CString Name,str;
	ProName w_name;
	ProMdlType p_type;
	if (!compute.ObtainJuBing(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
	{
		AfxMessageBox("�޷��õ�������飡");
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
			if (((DetailInfoClass *)p_classarray.GetAt(j))->ProductID==((DetailInfoClass *)p_classarray.GetAt(s))->ProductID)  //����ͬһ����Ʒ
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
			str.Format("�����Ϊ%s�޷��õ����",Name);
			AfxMessageBox(str);
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* ���ݾ���õ��õ�����Ļ�����Ϣ                                       */
/************************************************************************/
BOOL CCommonFuncClass::ObtianBasicInfoFromHandle(ProSolid solid)//��Ʒ���
{
	CCompute compute;
	CString Name,str;
	ProName w_name;
	ProMdlType p_type;
	JuBing_struc.RemoveAll();
	if (!compute.ObtainJuBing(solid))
	{
		AfxMessageBox("�޷��õ�������飡");
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
			str.Format("�����Ϊ%s�޷��õ����",Name);
			AfxMessageBox(str);
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* ȥ���ظ��������ţ��Է��ظ�����                                     */
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
/* �жϱ����������ļ��Ƿ����,����Ҫ�к�׺��.prt/.asm/.drw            */
/************************************************************************/
BOOL CCommonFuncClass::IsFileExist(CString SolidPlace,CString DrwPlace,CString SolidName,CString DrwName)
{
	//�ж����ļ����Ƿ����
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
			//���ж����·���Ƿ���ԭ����·��
			//str.Format("%s�������Ѿ����ڣ��Ƿ��滻",SolidName);
			//if(AfxMessageBox(str,MB_YESNO)==IDYES)
			//{
				while(FindFile(Path)!="")					//���Ǳ���ʱ�ж�
				{
					DeleteSpecialFile(SolidPlace,SolidName);//ɾ�����
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
			//���ж����·���Ƿ���ԭ����·��
			//str.Format("%s�������Ѿ����ڣ��Ƿ��滻",DrwName);
			//if(AfxMessageBox(str,MB_YESNO)==IDYES)
			//{
				while(FindFile(Path)!="")					//���Ǳ���ʱ�ж�
				{
					DeleteSpecialFile(DrwPlace,DrwName);//ɾ�����
				}
			//}
			//else
				//return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/* �ж������άͼ�ļ��Ƿ����                                           */
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
		//���ж����·���Ƿ���ԭ����·��
		str.Format("%s����ļ��Ѿ����ڣ��Ƿ��滻",DrwName);
		if(AfxMessageBox(str,MB_YESNO)==IDYES)
		{
			while(FindFile(Path)!="")					//���Ǳ���ʱ�ж�
			{
				DeleteSpecialFile(DrwPlace,DrwName);//ɾ�����
			}
		}
		else
			return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* �ж��±����·����ԭ���Ƿ���һ����                                   */
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
/* ���������                                                           */
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
		//�����������Ա�����ܲ�����]
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
/* ����������ݿ�                                                           */
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
/* ������������ܲ������������Ա�                                       */
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
/* ������������ܲ������������Ա�                                       */
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
/* �������ݿ������                                                     */
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
				return FALSE;	 //��������������ʱ�ṹ��
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
/* ����Ҫ���ص�ģ���Ƿ����ڴ��д���,����ֵ��ture���У�false��û��       */
/************************************************************************/
BOOL CCommonFuncClass::IsAlreadyInTheSession(CString Name,CArray <ProSolid,ProSolid> *p_arraylist)
{
	//����Ҫ�ж϶�άͼ�Ƿ���ڣ�
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
/* �õ��������ڴ��д��ڵľ��                                           */
/************************************************************************/
BOOL CCommonFuncClass::GetAlreadyInTheSession(CArray <ProSolid,ProSolid> *p_arraylist)
{
	//����Ҫ�ж϶�άͼ�Ƿ���ڣ�
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
/*�ó��ڴ溯��                                                          */
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
/* �ж����ʵ���Ƿ�Ͳ��������                                         */
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
		return SolidID;		//�����ڷ���Ϊ-1��
	}
	else
	{
		//�õ�ʵ����
		m_db.MoveBegin();
		do {
			InsNum=m_db.getInt("PrtNum");
			str.Format("%d",InsNum);
			AllNumber.Add(str);
		} while(m_db.MoveNext());
		for (int i=0;i<AllNumber.GetSize();i++)
		{
			sql.Format("SELECT * FROM NPartInsVariantTable WHERE PrtNum=%s",AllNumber[i]);
			//����ߴ�ֵ��ֵ�Ƿ���� �����������ע���ã����������ʾ���ͼ��
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
/* �жϲ�Ʒʵ���Ƿ�Ͳ��������                                         */
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
		return -1;		//�����ڷ���Ϊ������
	}
	else
	{
		//�õ�ʵ����
		m_db.MoveBegin();
		do {
			InsNum=m_db.getInt("PdtNum");
			str.Format("%d",InsNum);
			AllNumber.Add(str);
		} while(m_db.MoveNext());
		for (int i=0;i<AllNumber.GetSize();i++)
		{
			sql.Format("SELECT * FROM NProductInsVariantTable WHERE PdtNum=%s",AllNumber[i]);
			//����ߴ�ֵ��ֵ�Ƿ���� �����������ע���ã����������ʾ���ͼ��
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
/* ��ʵ�����ڵ�����£��Ƚ�ͼ��ʵ���е�ֵ��������ȫ����򷵻���ȷ       */
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
		//�õ�����ֵ
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
/* ���ͼ�ź�ʵ�����Ƿ����,���룬ͼ�ź����ʵ���� ,��ȷ����-1������ȷ�������ʵ����                     */
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
/* ���ݲ�Ʒ�ļ�����������ͼ��Ϊ���ֵ��ļ�����                           */
/************************************************************************/
BOOL CCommonFuncClass::BackUpFile(int s)		//��Ʒ���
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
	//�����ļ�
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
	//����и�Ŀ¼�Ļ������ҷ������Ƿ��Ѿ�������ļ�,Ҫ�ж��Ƕ�ά�Ļ�����ά
	if (!ftp.FindFile(((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathSolid,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->DrwName!="")				//����ж�άͼ
	{
		if (!ftp.FindFile(((DetailInfoClass *)p_classarray.GetAt(s))->DrwName,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathDrw,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
		{
			return FALSE;
		}
	}
	return TRUE;
}
/************************************************************************/
/*���ݵ�ĳ���ط����ڱ��ݵ�ԭ����λ��                                    */
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
/* ���浽����������  �����ڵ�����������                                                   */
/************************************************************************/
BOOL CCommonFuncClass::SavePartToTheServerforPart(int s)
{
	CFtpFile ftp;
	CCommonFuncClass commonfun;
	CString strLocalFilePath;
	//�����ļ�

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
/* ���浽���������������ڲ�Ʒ�����                                                   */
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
	//����и�Ŀ¼�Ļ������ҷ������Ƿ��Ѿ�������ļ�,Ҫ�ж��Ƕ�ά�Ļ�����ά
	if (!ftp.FindFile(((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathSolid,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->DrwName!="")				//����ж�άͼ
	{
		if (!ftp.FindFile(((DetailInfoClass *)p_classarray.GetAt(s))->DrwName,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathDrw,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
		{
			return FALSE;
		}
	}
	//��д���ݱ�
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
	{
		if (!commonfun.InsertNewPart(s,TRUE))
		{
			return FALSE;
		}//Insert���ݿ�
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2)
	{
		if (!commonfun.UpdateNewPart(s,TRUE))
		{
			return FALSE;
		}//Update���ݿ�
	}
	//�ϴ��ļ�
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
/* //�Ѳ�Ʒģ����ӵ����ṹ�ؼ���ȥ                                     */
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
/* //�ѵ���ģ����ӵ����ṹ�ؼ���ȥ                                     */
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
/* �������ӵ����ؼ�                                                   */
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
/* �ж�����ı��淽ʽ                                                   */
/************************************************************************/
BOOL CCommonFuncClass::SaveStyle(ProSolid solid,int s,CString MdlNum)
{
	int SolidID=IsPartExist(solid,MdlNum);
	if(SolidID!=-1)							//˵���������Ѿ�����
	{
		//�ж��������Ƿ��Ѿ���˹�
		if (!IsAuditPass(SolidID,TRUE))
		{	//����ԭ����ID���ж��ò��뻹��
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;		   //���滻
		}
		else
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=2;		   //�Ѿ��������뱣��
			AfxMessageBox("�������Ѿ����ڣ������ύ");
			return FALSE;
		}
	}
	else      //˵�����������µģ��ò���
	{
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidID!=-1)
		{
			//�ж����������Ƿ�һ��
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
/* �ж�����Ƿ����ͨ��                                                 */
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
		if (AuditState=="���δͨ��")
		{
			return FALSE;
		}
		else
			return TRUE;
	}
	return TRUE;
}
/************************************************************************/
/* �õ����ģ�͵����з���                                               */
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
			htop=m_tree->InsertItem("�������",NULL,TVI_LAST);
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
/* �õ����ģ�͵����з���                                               */
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
			htop=m_tree->InsertItem("��Ʒ���",NULL,TVI_LAST);
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
/* �ж�Ψһ�ԣ�ͼ�ţ��ļ���                                             */
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
/* Ҫ�������Ϣ�Ƿ����Ҫ��                                             */
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
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)		 //��ʾ���������
	{
		//�ж�ͼ�� ���ļ�����Ψһ��
		sql.Format("SELECT * FROM NPartInsInfoTable where InsCode='%s'",InsCode);
		if (IsHave(sql))
		{
			AfxMessageBox("���ͼ���Ѿ����ڣ����޸�");
			return FALSE;
		}
		sql.Format("SELECT * FROM NProductInsInfoTable Where InsCode='%s'",InsCode);
		if (IsHave(sql))
		{
			AfxMessageBox("���ͼ���Ѿ����ڣ����޸�");
			return FALSE;
		}
		if (IsPart)					//˵����������ж���ά�ļ����Ƿ����
		{
			sql.Format("SELECT * FROM NPartInsInfoTable where SolidFileName='%s'",SolidName);
			if (IsHave(sql))
			{
				str.Format("�ļ�%s�Ѿ����ڣ����޸�",SolidName);
				AfxMessageBox(str);
				return FALSE;
			}
		}
		else                       //˵���ǲ�Ʒ���ж���ά�ļ����Ƿ����
		{
			sql.Format("SELECT * FROM NProductInsInfoTable where SolidFileName='%s'",SolidName);
			if (IsHave(sql))
			{
				str.Format("�ļ�%s�Ѿ����ڣ����޸�",SolidName);
				AfxMessageBox(str);
				return FALSE;
			}
		}
		//�ж϶�ά�ļ����Ƿ����
		sql.Format("SELECT * FROM NPartInsInfoTable where DrawingName='%s'",DrwName);
		if (IsHave(sql))
		{
			str.Format("�ļ�%s�Ѿ����ڣ����޸�",DrwName);
			AfxMessageBox(str);
			return FALSE;
		}
	}
	else                 //����Ǹ�����������ж������/��Ʒ���Ƿ����
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
						str.Format("�ļ�%s�Ѿ����ڣ����޸�",SolidName);
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
						str.Format("�ļ�%s�Ѿ����ڣ����޸�",SolidName);
						AfxMessageBox(str);
						return FALSE;
					}
				}
			}
			//�ж�ͼ���Ƿ��Ѿ�����
			if (InsCode.MakeLower()!=InsCode1.MakeLower())
			{
				sql.Format("SELECT * FROM NPartInsInfoTable where InsCode='%s'",InsCode);
				if (IsHave(sql))
				{
					AfxMessageBox("���ͼ���Ѿ����ڣ����޸�");
					return FALSE;
				}
				sql.Format("SELECT * FROM NProductInsInfoTable where InsCode='%s'",InsCode);
				if (IsHave(sql))
				{
					AfxMessageBox("���ͼ���Ѿ����ڣ����޸�");
					return FALSE;
				}
			}
			if (DrwName1.MakeLower()!=DrwName.MakeLower())
			{
				sql.Format("SELECT * FROM NPartInsInfoTable where DrawingName='%s'",DrwName);
				if (IsHave(sql))
				{
					str.Format("�ļ�%s�Ѿ����ڣ����޸�",DrwName);
					AfxMessageBox(str);
					return FALSE;
				}
			}
		}
		return TRUE;
}
/************************************************************************/
/* �ϴ��ļ�������ɹ��Ļ������������ı�ż�����                       */
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

	//����s//�Ժ�ʧ�ܵ�ʱ��Ҫ��
	str.Format("%d",s);
	NumArray.Add(str);
	//����и�Ŀ¼�Ļ������ҷ������Ƿ��Ѿ�������ļ�,Ҫ�ж��Ƕ�ά�Ļ�����ά
	if (!ftp.FindFile(((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathSolid,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->DrwName!="")				//����ж�άͼ
	{
		if (!ftp.FindFile(((DetailInfoClass *)p_classarray.GetAt(s))->DrwName,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathDrw,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
		{
			return FALSE;
		}
	}
	//�ϴ��ļ�
	strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->TempPath+"\\"+((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName+".1";
	if (!ftp.UploadFiletoDirectory(strLocalFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort))
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->DrwName!="")				//����ж�άͼ
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
/* ���������滻�Ļ����жϱ���λ���Ƿ�һ��                             */
/************************************************************************/
BOOL CCommonFuncClass::IsTheSavePlaceTheSameIfReplace(BOOL IsLocal)  
{
	//NumArray�Ǵ�commonfun.ObtianBasicInfoFromHandle(solid)�����ģ������������Ҫ����
	int s;
	for (int i=0;i<NumArray.GetSize();i++)
	{
		s=atol(NumArray[i]);
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1)
		{
			if (IsLocal)
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer!="����")
				{
					return FALSE;
				}
			}
			else
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="����")
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* ����ǰ��׼������                                                     */
/************************************************************************/
//1���õ�ͼ�ĵ�ǰ�İ汾��Ϣ
BOOL CCommonFuncClass::TheWorkBeforeSave(ProSolid ProductSolid)
{
	ProError status;
	ProMdldata  mdldata;
	CCompute compute;
	CCommonFuncClass commonfun;
	if(!ObtianBasicInfoFromHandle(ProductSolid))               //�ж��Ƿ��л�����Ϣ
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
			if ((!Success) && ((DetailInfoClass *)p_classarray.GetAt(s))->CurrentSaveFlag)   //����ʧ�ܣ�ȥ���߰汾����ԭ������
			{
                pdtclass.DeleteOldVersion(JuBing_struc[i].solid,Success,((DetailInfoClass *)p_classarray.GetAt(s))->SolidVersion,((DetailInfoClass *)p_classarray.GetAt(s))->DrwVersion);
				((DetailInfoClass *)p_classarray.GetAt(s))->SolidID=((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidID;
				if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="������")
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
/* ������ʧ�ܵ�ʱ��                                                     */
/************************************************************************/
BOOL CCommonFuncClass::RollBackFile(int s)
{
	CFtpFile ftp;
	CString	strSeverFilePath,strLocalFilePath;

	//��ȥ�ϴ����ļ�
	strSeverFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath+"/"+((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName+".1";
	ftp.RemoveFile(strSeverFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	strSeverFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath+"/"+((DetailInfoClass *)p_classarray.GetAt(s))->DrwName+".1";
	ftp.RemoveFile(strSeverFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	//�ϴ����ݵ��ļ�
	strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathSolid+"\\"+((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName+".1";
	ftp.UploadFiletoDirectory(strLocalFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	strLocalFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->BackupPathDrw+"\\"+((DetailInfoClass *)p_classarray.GetAt(s))->DrwName+".1";
	ftp.UploadFiletoDirectory(strLocalFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort);
	return TRUE;
}
/************************************************************************/
/* �ڲ�������������ͼ�ź����֣�ģ�ͺţ�����PDMͼ��                      */
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
	//���ʵ����
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
			//�½�����
			p_value.type=PRO_PARAM_STRING;
			ProStringToWstring(p_value.value.s_val,((DetailInfoClass *)p_classarray.GetAt(s))->InsName.GetBuffer());
			status=ProParameterCreate(&modelitem,w_name,&p_value,&param);
		}
		else
		    return FALSE;
	//�½�����ͼ��
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
			//�½�����
			p_value.type=PRO_PARAM_STRING;
			ProStringToWstring(p_value.value.s_val,((DetailInfoClass *)p_classarray.GetAt(s))->InsCode.GetBuffer());
			status=ProParameterCreate(&modelitem,w_name,&p_value,&param);
		}
		else
			return FALSE;
	//�½�PDMͼ��
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
			//�½�����
			p_value.type=PRO_PARAM_STRING;
			ProStringToWstring(p_value.value.s_val,((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode.GetBuffer());
			status=ProParameterCreate(&modelitem,w_name,&p_value,&param);
		}
		else
			return FALSE;
	//���ģ�ͺ�
	ProStringToWstring(w_name,"RL_Mdl");
	status=ProParameterInit(&modelitem,w_name,&param);
	if (status==PRO_TK_E_NOT_FOUND)
	{
		//�½�����
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
/* �õ�ͼ����Ϣ                                                         */
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
	////�õ�ͼ��
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
/* ���ͼ����Ϣ��������                                                 */
/************************************************************************/
BOOL CCommonFuncClass::SetCodeInfo(ProSolid solid)  //1. ���
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
	ProStringToWstring(w_name,"�ļ���");
	status=ProParameterInit(&modelitem,w_name,&param);
	if (status==PRO_TK_E_NOT_FOUND)
	{
		//�½�����
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