// ChooseProductDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "ChooseProductDlg.h"
#include "MyDatabase.h"
#include "afxtempl.h"
#include "ProWindows.h"
#include "CheckDlg.h"
#include "SelectDlg.h"
#include "afxtempl.h"
#include "ProDrawing.h"
#include "MyDatabase.h"
#include "CommonFuncClass.h"
#include "Compute.h"
#include "FtpFile.h"
#include "DownLoadDlg.h"
#include "DetailInfoClass.h"


extern CArray<ProDrawing,ProDrawing>  DrawingList;
void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern FtpInfo Ftp_struc;
extern CPtrArray p_classarray;




//extern CMyDatabase m_pdm;
CArray<ItemHandle,ItemHandle> treehandle;
IMPLEMENT_DYNAMIC(CChooseProductDlg, CDialog)
CChooseProductDlg::CChooseProductDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseProductDlg::IDD, pParent)
	, m_show(0)
{
}

CChooseProductDlg::~CChooseProductDlg()
{
}

void CChooseProductDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_listproduct);
	DDX_Radio(pDX, IDC_RADIO_PART, m_show);
}


BEGIN_MESSAGE_MAP(CChooseProductDlg, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_RADIO_PART, OnBnClickedRadioPart)
	ON_BN_CLICKED(IDC_RADIO_ASM, OnBnClickedRadioAsm)
END_MESSAGE_MAP()


// CChooseProductDlg 消息处理程序

void CChooseProductDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRepaint(PRO_VALUE_UNUSED);

	// TODO: 在此添加消息处理程序代码
}

void CChooseProductDlg::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CChooseProductDlg::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL IsSelect=FALSE;

	CCommonFuncClass commonfun;
	CString InsCode,sql;
	for (int i=0;i<m_listproduct.GetItemCount();i++)
	{
		if (m_listproduct.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsSelect=TRUE;
			InsCode=m_listproduct.GetItemText(i,1);
			if (m_show==0)					  //说明是零件
			{
				sql.Format("SELECT * FROM NPartInsInfoTable WHERE InsCode='%s'",InsCode);
				//if (!commonfun.GetInfoFromDatabaseBase(sql,0,FALSE,TRUE,""))
				//{
				//	AfxMessageBox("无法得到基本信息");
				//	return;
				//}
			}
			else
			{
				;//对产品操作
			}
		}
	}
	if (!IsSelect)
	{
		AfxMessageBox("请选择你要打开的模型！");
		return;
	}
	int s=commonfun.IsBasicInfoHave(InsCode);
	if (s==-1)
	{
		AfxMessageBox("无法得到基本信息");
		return;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="本地")
	{
		//直接打开；
		//if(!commonfun.OpenPartFile(((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFileName,
		//	((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwName,
		//	((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFilePath,
		//	((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwFilePath,s))
		//{
		//	AfxMessageBox("打开失败");
		//}
		//else
		//{
		// //   checkdlg.Create(IDD_DIALOG3,NULL);
		//	//checkdlg.ShowWindow(SW_SHOWNORMAL);
		//}
	}
	else
		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="服务器")
		{
			OnOK();
			CDownLoadDlg downloaddlg;
			downloaddlg.s=s;
			downloaddlg.IsCheck=TRUE;
			downloaddlg.DoModal();//跳出对话框来让其选择
		}
		OnOK();
}

BOOL CChooseProductDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	CCommonFuncClass commonfun;
	// TODO:  在此添加额外的初始化
	m_listproduct.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listproduct.InsertColumn(0,"序号",LVCFMT_LEFT,20,-1);
	m_listproduct.InsertColumn(1,"图号",LVCFMT_LEFT,80,-1);
	m_listproduct.InsertColumn(2,"名称",LVCFMT_LEFT,100,-1);
	m_listproduct.InsertColumn(3,"类别",LVCFMT_LEFT,55,-1);
	m_listproduct.InsertColumn(4,"类型",LVCFMT_LEFT,55,-1);
	m_listproduct.InsertColumn(5,"保存位置",LVCFMT_LEFT,55,-1);
    m_listproduct.InsertColumn(6,"设计者",LVCFMT_LEFT,55,-1);
	UpdateData(FALSE);
	CString sql;
	sql.Format("SELECT * FROM NPartInsInfoTable Where FinishState='完成' AND AuditState='审核未通过'");
	SHOW(&m_listproduct,sql);
	return TRUE;  // return TRUE unless you set the focus to a control
}

/************************************************************************/
/* 显示待审核的零件                                                     */
/************************************************************************/
void CChooseProductDlg::OnBnClickedRadioPart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listproduct.DeleteAllItems();
	CString sql;
	sql.Format("SELECT * FROM NPartInsInfoTable Where FinishState='完成' AND AuditState='审核未通过'");
}
/************************************************************************/
/* 显示待审核的产品                                                     */
/************************************************************************/
void CChooseProductDlg::OnBnClickedRadioAsm()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listproduct.DeleteAllItems();
	CString sql;
	sql.Format("SELECT * FROM NProductInsInfoTable Where FinishState='完成' AND AuditState='审核未通过'");
	SHOW(&m_listproduct,sql);
}
/************************************************************************/
/* 显示全部                                                             */
/************************************************************************/
void CChooseProductDlg::OnBnClickedRadioAll()
{
	// TODO: 在此添加控件通知处理程序代码
	
}
/************************************************************************/
/* 显示函数                                                             */
/************************************************************************/
BOOL CChooseProductDlg::SHOW(CListCtrl *m_list,CString sql)
{
	CString str;
	CStringArray GroupNum;
	GroupNum.RemoveAll();
	int i=0;
	 int Z=m_db.Query(sql);
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
		 m_db.MoveBegin();
		 do {
			 str.Format("%d",i+1);
		 	m_list->InsertItem(i,str);
			str=m_db.getString("InsCode");
			m_list->SetItemText(i,1,str);
			str=m_db.getString("Name");
			m_list->SetItemText(i,2,str);
			str.Format("%d",m_db.getInt("TypeNum"));
			GroupNum.Add(str);
			str="零件";
			m_list->SetItemText(i,4,str);
			str=m_db.getString("LocalState");
			m_list->SetItemText(i,5,str);
			str=m_db.getString("Designer");
			m_list->SetItemText(i,6,str);
		 } while(m_db.MoveNext());
		 for (int j=0;j<GroupNum.GetSize();j++)
		 {
			 sql.Format("SELECT * FROM NPartType where Number=%s",GroupNum[j]);
			 Z=m_db.Query(sql);
			 if (Z==-1 || Z==0)
			 {
				 continue;
			 }
			 else
			 {
				 str=m_db.getString("Name");
				 m_list->SetItemText(j,3,str);
			 }
		 }
	 }
	 return TRUE;
}