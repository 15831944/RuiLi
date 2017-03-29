// CheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "CheckDlg.h"
#include "ProWindows.h"
#include "Compute.h"
#include "MyDatabase.h"
#include "afxtempl.h"
#include "ProSolid.h"
#include "ProDimension.h"
#include "ProUtil.h"
#include "CommonFuncClass.h"
#include "DetailDlg.h"
#include <stdlib.h>
#include <stdio.h>
#include "FtpFile.h"
#include "ProFeature.h"
#include "ProDrawing.h"
#include "MyDatabase.h"
#include "DetailInfoClass.h"
#include "PrtInfoDlg.h"

extern CStringArray RelationSet;
extern CArray<JuBing,JuBing> JuBing_struc;
extern CArray <Database,Database> Database_struc_temp;
extern   CArray <Database,Database> Database_struc;
extern CMyDatabase m_db;
extern FtpInfo Ftp_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;

/// 
void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data);
// CCheckDlg 对话框

IMPLEMENT_DYNAMIC(CCheckDlg, CDialog)
CCheckDlg::CCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckDlg::IDD, pParent)
	,s(-1)
{
}

CCheckDlg::~CCheckDlg()
{
}

void CCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DIM, m_dimension);
}


BEGIN_MESSAGE_MAP(CCheckDlg, CDialog)
	//ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_MOVE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_DIM, OnNMClickListDim)
	ON_BN_CLICKED(IDC_BUTTON_OTHER, OnBnClickedButtonOther)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
ON_BN_CLICKED(IDC_BUTTON_NO, OnBnClickedButtonNo)
END_MESSAGE_MAP()


// CCheckDlg 消息处理程序

//void CCheckDlg::OnBnClickedOk()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	INT_PTR status;
//	CPartOpenDlg partopendlg;
//    status=	partopendlg.DoModal();
//	
//}

void CCheckDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::DestroyWindow();
	
}

BOOL CCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	CCommonFuncClass commonfun;
	CString str;					      
	BOOL IsHave=FALSE;
	CCompute compute;
	//去掉进程中没有的基本信息
	commonfun.IsBasicInfoAlreadyHave();
	commonfun.RemoveNotInSession();

	if (!m_db.DatabaseState())
	{
		m_db.ReOpen();
	}
	m_dimension.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_dimension.InsertColumn(0,"可变尺寸",LVCFMT_LEFT,80,-1);
	m_dimension.InsertColumn(1,"数值",LVCFMT_LEFT,80,-1);
	m_dimension.InsertColumn(2,"上偏差",LVCFMT_LEFT,55,-1);
	m_dimension.InsertColumn(3,"下偏差",LVCFMT_LEFT,55,-1);
    
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_NO)->EnableWindow(FALSE);
	ProSolid solid;
	ProError status;
	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowMdlGet(w_id,(ProMdl *)&solid);
	s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("无法得到这个零件的模型信息！");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	partnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum;
	if (partnum=="-1")
	{
		AfxMessageBox("这个是非模型，没有可变尺寸！");
	}
	else
	{
		Database_struc_temp.RemoveAll();
		if(!compute.GetVariantForPart(solid,partnum,TRUE))
		{
            this->SendMessage(WM_CLOSE,0,0);
			return FALSE;//把主变量读到临时结构中
		}
		if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))	 //也需要读入偏差信息
		{
			return FALSE;
		}
		int t=0;
		//显示变量尺寸
		for (int i=0;i<Database_struc_temp.GetSize();i++)	 //有问题
		{
			str=Database_struc_temp[i].name;
			int indexNo=m_dimension.InsertItem(LVIF_TEXT|LVIF_PARAM,t,Database_struc_temp[i].name,
				0,0,0,Database_struc_temp[i].number);
			m_dimension.SetItemText(indexNo,1,Database_struc_temp[i].value);
			m_dimension.SetItemText(indexNo,2,Database_struc_temp[i].up);	 //上偏差
			m_dimension.SetItemText(indexNo,3,Database_struc_temp[i].down);	 //下偏差
			UpdateData(FALSE);
			t++;
		}
	}
	return TRUE;  
	// 异常：OCX 属性页应返回 FALSE
}

void CCheckDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
    ProWindowRepaint(PRO_VALUE_UNUSED);
	// TODO: 在此添加消息处理程序代码
}	

void CCheckDlg::OnNMClickListDim(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	ProSolid solid;
	ProDimension haha;
	CString str;
	BOOL IsHave=FALSE;
	CString type;
	ProError status;
	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowMdlGet(w_id,(ProMdl *)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		return;
	}
	POSITION pos = m_dimension.GetFirstSelectedItemPosition();
	while (pos)
	{
		IsHave=TRUE;
		int nItem = m_dimension.GetNextSelectedItem(pos);
		if (Database_struc_temp[nItem].type!="dim")		   //如果是参数，则不显示尺寸
		{
			ProWindowRepaint(PRO_VALUE_UNUSED);
			ProWindowRefresh(PRO_VALUE_UNUSED);
			return;
		}
		else
		{
			str=Database_struc_temp[nItem].idnum;
			str.Delete(0,1);
			haha.id=atoi(str.GetBuffer());
			haha.type=PRO_DIMENSION;
			haha.owner=((ProMdl)solid);
			ProWindowRepaint(PRO_VALUE_UNUSED);
			ProDimensionShow(&haha,NULL,NULL,NULL);
		}	
	}

	if (!IsHave)
	{
		return;
	}
	*pResult = 0;
}


void CCheckDlg::OnBnClickedButtonOther()
{
	// TODO: 在此添加控件通知处理程序代码
	CPrtInfoDlg infodlg;
	infodlg.s=s;
	if(IDOK==infodlg.DoModal())	
	{
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_NO)->EnableWindow(TRUE);
	}
}
void CCheckDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	//判断图号是否正确
	CString str;
	CCommonFuncClass commonfun;
	//ProMdlCurrentGet((ProMdl*)&solid);
	//int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("找不到这个零件的信息，无法更新");
		return;
	}
	//开始保存
	//得到时间
	CTime time=CTime::GetCurrentTime();
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime=time.Format("%Y-%m-%d %H:%M:%S");
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="审核通过";

	m_db.m_pConnection->Close();
	m_db.ReOpen();
	m_db.m_pConnection->BeginTrans();
	try
	{
		if (!CheckPart(s))
		{
			m_db.m_pConnection->RollbackTrans();
			AfxMessageBox("审核失败");
		}//Update数据库
		else
		{
			m_db.m_pConnection->CommitTrans();
			AfxMessageBox("提交成功！");
		}

	}
	catch (_com_error e) 
	{
		m_db.m_pConnection->RollbackTrans();
		AfxMessageBox("审核失败！");
		//AfxMessageBox(e.ErrorMessage());
	}
	CDialog::DestroyWindow();
}

/************************************************************************/
/* 审核未通过                                                           */
/************************************************************************/
void CCheckDlg::OnBnClickedButtonNo()
{
	// TODO: 在此添加控件通知处理程序代码
	if (s==-1)
	{
		AfxMessageBox("找不到这个零件的信息，无法更新");
		return;
	}
	//开始保存
	//得到时间
	CTime time=CTime::GetCurrentTime();
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime=time.Format("%Y-%m-%d %H:%M:%S");
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="审核未通过";
	((DetailInfoClass *)p_classarray.GetAt(s))->DesignState="未完成";
	((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime="";
	m_db.m_pConnection->Close();
	m_db.ReOpen();
	m_db.m_pConnection->BeginTrans();
	try
	{
		if (!CheckPart(s))
		{
			m_db.m_pConnection->RollbackTrans();
			AfxMessageBox("审核失败");
		}//Update数据库
		else
		{
			m_db.m_pConnection->CommitTrans();
			AfxMessageBox("提交失败！");
		}

	}
	catch (_com_error e) 
	{
		m_db.m_pConnection->RollbackTrans();
		AfxMessageBox("审核失败！");
		AfxMessageBox(e.ErrorMessage());
	}
	CDialog::DestroyWindow();
}
/************************************************************************/
/* 更新数据库的内容                                                     */
/************************************************************************/
BOOL CCheckDlg::CheckPart(int s)
{
	CString sql;
	CCompute compute;
	CCommonFuncClass commonfun;
	/////
	sql.Format("UPDATE NPartInsInfoTable Set Name='%s',InsCode='%s',Material='%s',Designer='%s',FinishTime='%s',Auditor='%s',AuditTime='%s',FinishState='%s',AuditState='%s',Note='%s' where PrtNum=%d",
		((DetailInfoClass *)p_classarray.GetAt(s))->InsName,((DetailInfoClass *)p_classarray.GetAt(s))->InsCode,
		((DetailInfoClass *)p_classarray.GetAt(s))->Material,((DetailInfoClass *)p_classarray.GetAt(s))->Designer,
		((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime,((DetailInfoClass *)p_classarray.GetAt(s))->Auditor,
		((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime,((DetailInfoClass *)p_classarray.GetAt(s))->DesignState,
		((DetailInfoClass *)p_classarray.GetAt(s))->AuditState,((DetailInfoClass *)p_classarray.GetAt(s))->Note,
		((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
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
		if (!commonfun.UpdateVariantAndPerf(s))
		{
			return FALSE;
		}
	}
	return TRUE;
}