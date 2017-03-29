// DetailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "DetailDlg.h"
#include "ProWindows.h"
#include "Compute.h"
#include "ProSolid.h"
#include "ProUtil.h"
#include "afxtempl.h"
#include "MyDatabase.h"
#include <stdlib.h>
#include <stdio.h>
#include "FtpFile.h"
#include "ProFeature.h"
#include "ProDrawing.h"
#include "CommonFuncClass.h"
#include <commctrl.h>
#include <ctype.h>
#include "DetailInfoClass.h"
#include "ProductClass.h"

ProError SolidFeatGet(ProSolid solid,ProFeature **p_data);
extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;
extern CDetailDlg *detaildlg;


// CDetailDlg 对话框

IMPLEMENT_DYNAMIC(CDetailDlg, CDialog)
CDetailDlg::CDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailDlg::IDD, pParent)
{
}

CDetailDlg::~CDetailDlg()
{
}

void CDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_list);
}


BEGIN_MESSAGE_MAP(CDetailDlg, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnBnClickedButtonClose)
	ON_WM_NCLBUTTONDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_INFO, OnNMDblclkListInfo)
END_MESSAGE_MAP()


// CDetailDlg 消息处理程序

BOOL CDetailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	m_list.DeleteAllItems();
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	ProError status;
	ListView_SetExtendedListViewStyle(m_list.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0,"序号",LVCFMT_LEFT,25,-1);
	m_list.InsertColumn(1,"文件名",LVCFMT_LEFT,120,-1);
	m_list.InsertColumn(2,"设计状态",LVCFMT_LEFT,65,-1);
	m_list.InsertColumn(3,"保存状态",LVCFMT_LEFT,65,-1);
	m_list.InsertColumn(4,"保存方式",LVCFMT_LEFT,65,-1);
	m_list.InsertColumn(5,"审核状态",LVCFMT_LEFT,65,-1);
	m_list.InsertColumn(6,"提交状态",LVCFMT_LEFT,65,-1);
	status=ProMdlCurrentGet((ProMdl*)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("对不起,您还没有打开模型!");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("无法得到这个零件的基本信息");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	if(!pdtclass.FindProductMenberForProductWhichIsModify(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE))
	{
		AfxMessageBox("无法得到这个产品的设计信息");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	if (!ListThePartAndAsm(((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,&m_list))
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	return TRUE; 
}

void CDetailDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRepaint(PRO_VALUE_UNUSED);

	// TODO: 在此添加消息处理程序代码
}

void CDetailDlg::OnBnClickedOk()
{
	OnOK();
}


void CDetailDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	m_list.DeleteAllItems();
	int	s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("无法得到这个产品的模型信息！");
		return;
	}
	if(!pdtclass.FindProductMenberForProductWhichIsModify(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE))
	{
		return;
	}
	if (!ListThePartAndAsm(((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,&m_list))
	{
        return;   
	}
	CDialog::OnLButtonDown(nFlags, point);
}
/************************************************************************/
/* 显示零部件                                                           */
/************************************************************************/
BOOL CDetailDlg::ListThePartAndAsm(int SolidID,CListCtrl *m_prtlist)
{
	CString sql,str,str1,FileName;
	int Z,k=0;
	CStringArray SolidIDArray;
	CStringArray TypeArray;
	SolidIDArray.RemoveAll();
	TypeArray.RemoveAll();
	sql.Format("SELECT * FROM NProductBOMTable WHERE PdtNum=%d",SolidID);
	Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			str.Format("%d",m_db.getInt("PrtNum"));
			SolidIDArray.Add(str);
			str=m_db.getString("Type");
			TypeArray.Add(str);
		} while(m_db.MoveNext());
	}
	for (int i=0;i<SolidIDArray.GetSize();i++)
	{
		if (TypeArray[i]=="prt")
		{
			sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%s",SolidIDArray[i]);
			Z=m_db.Query(sql);
			if (Z==0 || Z==-1)
			{
				return FALSE;
			}
			else
			{
				m_db.MoveBegin();
				do {
					str.Format("%d",i+1);
					m_prtlist->InsertItem(i,str);
					str=m_db.getString("AuditState");
					FileName=m_db.getString("SolidFileName");
					m_prtlist->SetItemText(i,1,FileName);
					str=m_db.getString("FinishState");
					m_prtlist->SetItemText(i,2,str);
					if(!GetHandleFromName(FileName,i,m_prtlist))           //根据名字得到句柄
					{
						return FALSE;
					}
					str=m_db.getString("AuditState");
					m_prtlist->SetItemText(i,5,str);
					str=m_db.getString("PDMState");
					m_prtlist->SetItemText(i,6,str);
				} while(m_db.MoveNext());
			}
		}
		else
			if (TypeArray[i]=="asm")
			{
				sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%s",SolidIDArray[i]);
				Z=m_db.Query(sql);
				if (Z==0 || Z==-1)
				{
					return FALSE;
				}
				else
				{
					m_db.MoveBegin();
					do {
						str.Format("%d",i+1);
						m_prtlist->InsertItem(i,str);
						str=m_db.getString("AuditState");
						FileName=m_db.getString("SolidFileName");
						m_prtlist->SetItemText(i,1,FileName);
						str=m_db.getString("FinishState");
						m_prtlist->SetItemText(i,2,str);
						if(!GetHandleFromName(FileName,i,m_prtlist))
						{
							return FALSE;
						}
						str=m_db.getString("AuditState");
						m_prtlist->SetItemText(i,5,str);
						str=m_db.getString("PDMState");
					    m_prtlist->SetItemText(i,6,str);
					} while(m_db.MoveNext());
				}
			}
	}
	return TRUE;
}
/************************************************************************/
/* 根据名字得到句柄得到                                                 */
/************************************************************************/
BOOL CDetailDlg::GetHandleFromName(CString FileName,int i,CListCtrl *m_prtlist)
{
	// TODO: 在此添加控件通知处理程序代码
	ProError status;
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CString str,Name;
	ProMdlType p_type;
	ProName w_name;
	ProSolid solid;
	int k=FileName.Find(".prt");
	if (k!=-1)   //说明是零件
	{
		Name=FileName.Left(k);
		p_type=PRO_MDL_PART;
	}
	k=FileName.Find(".asm");
	if (k!=-1)
	{
		Name=FileName.Left(k);
		p_type=PRO_MDL_ASSEMBLY;
	}
	ProStringToWstring(w_name,Name.GetBuffer());
	status=ProMdlInit(w_name,p_type,(ProMdl *)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		str.Format("无法得到%s模型的句柄",FileName);
		AfxMessageBox(str);
		return FALSE;
	}
	else
	{
		int s=commonfun.IsBasicInfoHave(solid);
		if (s==-1)
		{
			str.Format("无法得到%s模型的基本信息",FileName);
			AfxMessageBox(str);
			return FALSE;
		}
		else
		{
			if (((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved)
			{
				str="已保存";
			}
			else
				str="未保存";
            m_prtlist->SetItemText(i,3,str);
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0 && (!((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved))
			{
				str="另存为";
			}
			else
			if ((((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==2) && (!((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved))
			{
				str="保存";
			}
			else
				str="保存";
			 m_prtlist->SetItemText(i,4,str);
		}
	}
	return TRUE;
}
void CDetailDlg::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::DestroyWindow();
}
/************************************************************************/
/* 更新显示                                                             */
/************************************************************************/
void CDetailDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	m_list.DeleteAllItems();
	int	s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("无法得到这个产品的模型信息！");
		return;
	}
	if(!pdtclass.FindProductMenberForProductWhichIsModify(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE))
	{
		return;
	}
	if (!ListThePartAndAsm(((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,&m_list))
	{
		return;   
	}
	CDialog::OnNcLButtonDown(nHitTest, point);
}

void CDetailDlg::OnNMDblclkListInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	ProError status;
	BOOL IsOk;
	int w_id;
	CString str,Name;
	ProMdlType p_type;
	ProName w_name;
	ProMdl mdl;
	for (int i=0;i<m_list.GetItemCount();i++)
	{
		if (m_list.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsOk=TRUE;
			str=m_list.GetItemText(i,1);
			break;
		}
	}
	int k=str.Find(".prt");
	if (k!=-1)   //说明是零件
	{
		Name=str.Left(k);
		p_type=PRO_MDL_PART;
	}
	k=str.Find(".asm");
	if (k!=-1)
	{
		Name=str.Left(k);
		p_type=PRO_MDL_ASSEMBLY;
	}
	ProStringToWstring(w_name,Name.GetBuffer());
	status=ProMdlInit(w_name,p_type,&mdl);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("无法得到这个模型的居柄，请手工打开审核");
	}
	else
	{
		status=ProObjectwindowCreate(w_name,(ProType)p_type,&w_id);
		status=ProMdlDisplay(mdl);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("无法显示三维图！");
			return;
		}
		status=ProWindowActivate(w_id);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("无法激活当前窗口，请手工激活！");
			return;
		}
	}
	*pResult = 0;
}
