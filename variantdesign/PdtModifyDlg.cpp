// PdtModifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PdtModifyDlg.h"
#include "MyDatabase.h"
#include "CommonFuncClass.h"
#include "DownLoadDlg.h"
#include "DetailInfoClass.h"
#include "ProSolid.h"
#include "ProductClass.h"
#include <ProWindows.h>


// CPrtModifyDlg 对话框
extern CMyDatabase m_db;
extern CPtrArray p_classarray;
extern CArray <FilePath,FilePath> FilePath_struc;


// CPdtModifyDlg 对话框

IMPLEMENT_DYNAMIC(CPdtModifyDlg, CDialog)
CPdtModifyDlg::CPdtModifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPdtModifyDlg::IDD, pParent)
	, m_key(_T(""))
	, m_workspace(0)
	, m_audit(FALSE)
{
}

CPdtModifyDlg::~CPdtModifyDlg()
{
}

void CPdtModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KEY, m_key);
	DDX_Control(pDX, IDC_COMBO_KEY, m_combokey);
	DDX_Control(pDX, IDC_TREE_PART, m_treeprt);
	DDX_Control(pDX, IDC_LIST_PART, m_listprt);
	DDX_Radio(pDX, IDC_RADIO_WORKAPACE, m_workspace);
	DDX_Check(pDX, IDC_CHECK_AUDIT, m_audit);
}


BEGIN_MESSAGE_MAP(CPdtModifyDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL2, OnBnClickedCancel2)
	ON_BN_CLICKED(IDC_RADIO_WORKAPACE, OnBnClickedRadioWorkapace)
	ON_BN_CLICKED(IDC_RADIO_PDM, OnBnClickedRadioPdm)
	ON_NOTIFY(NM_CLICK, IDC_TREE_PART, OnNMClickTreePart)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CPdtModifyDlg 消息处理程序
BOOL CPdtModifyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	CCommonFuncClass commonfun;
	//去掉进程中没有的基本信息
	commonfun.IsBasicInfoAlreadyHave();
	commonfun.RemoveNotInSession();
	int index=m_combokey.AddString("图号");
	m_combokey.SetCurSel(index);
	m_listprt.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listprt.InsertColumn(0,"序号",LVCFMT_LEFT,40,-1);
	m_listprt.InsertColumn(1,"图号",LVCFMT_LEFT,100,-1);
	m_listprt.InsertColumn(2,"名称",LVCFMT_LEFT,55,-1);
	m_listprt.InsertColumn(3,"类别",LVCFMT_LEFT,55,-1);
	m_listprt.InsertColumn(4,"设计状态",LVCFMT_LEFT,55,-1);
	m_listprt.InsertColumn(5,"设计者",LVCFMT_LEFT,55,-1);
	//得到产品的图号
	commonfun.ObtainProductModelClass(&m_treeprt);
	return TRUE;  
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 查询按钮                                                             */
/************************************************************************/
void CPdtModifyDlg::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listprt.DeleteAllItems();
	UpdateData(TRUE);
	CString sql;
	if (m_workspace==0 && m_audit)			 //查找工作区，且未审核的
	{
		sql.Format("SELECT * FROM NProductInsInfoTable WHERE InsCode LIKE '%%%s%%' AND FinishState='完成' AND AuditState='审核未通过' AND PDMState='未提交'",m_key);
		if (!GetPdtFromBase(sql,FALSE))        //在工作区查找
		{
			AfxMessageBox("查找失败");
			return;
		}
	}
	else
	{
		if (m_workspace==0 && (!m_audit))
		{
			sql.Format("SELECT * FROM NProductInsInfoTable WHERE InsCode LIKE '%%%s%%' AND PDMState='未提交'",m_key);
			if (!GetPdtFromBase(sql,FALSE))     //在工作区查找
			{
				AfxMessageBox("查找失败");
				return;
			}
		}
		else    //查找PDM数据库
		{
			sql.Format("SELECT * FROM NProductInsInfoTable WHERE PDMNum LIKE '%%%s%%' AND PDMState='提交'",m_key);
			if (!GetPdtFromBase(sql,TRUE))
			{
				AfxMessageBox("查找失败");
				return;
			}
		}
	}
}

void CPdtModifyDlg::OnBnClickedCancel2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

/************************************************************************/
/* 根据图号查找实例,TRUE从PDM查，FALSE从VDS查                           */
/************************************************************************/
BOOL CPdtModifyDlg::GetPdtFromBase(CString sql,BOOL PDM)
{
	CString str;
	CStringArray TypeNum;
	TypeNum.RemoveAll();
	int Z=m_db.Query(sql);

	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		m_listprt.DeleteAllItems();
		return TRUE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			//if (PDM)
			//{
   //            str=m_db.getString("PDMNum");
			//   if (str!="")
			//   {
			//	   str.Format("%d",i+1);
			//	   m_listprt.InsertItem(i,str);
			//	   str=m_db.getString("PDMNum");
			//	   m_listprt.SetItemText(i,1,str);
			//	   str=m_db.getString("Name");
			//	   m_listprt.SetItemText(i,2,str);
			//	   str.Format("%d",m_db.getInt("TypeNum"));
			//	   TypeNum.Add(str);
			//	   str=m_db.getString("FinishState");
			//	   m_listprt.SetItemText(i,4,str);
			//	   str=m_db.getString("Designer");
			//	   m_listprt.SetItemText(i,5,str);
			//	   str.Format("%d",m_db.getInt("PdtNum"));
			//	   m_listprt.SetItemData(i,atol(str));
			//   }
			//}
			//else
			//{
				str.Format("%d",i+1);
				m_listprt.InsertItem(i,str);
				if (PDM)
				{
					str=m_db.getString("PDMNum");
				}
				else
				    str=m_db.getString("InsCode");
				m_listprt.SetItemText(i,1,str);
				str=m_db.getString("Name");
				m_listprt.SetItemText(i,2,str);
				str.Format("%d",m_db.getInt("TypeNum"));
				TypeNum.Add(str);
				str=m_db.getString("FinishState");
				m_listprt.SetItemText(i,4,str);
				str=m_db.getString("Designer");
				m_listprt.SetItemText(i,5,str);
				str.Format("%d",m_db.getInt("PdtNum"));
				m_listprt.SetItemData(i,atol(str));
			//}
			i++;
		} while(m_db.MoveNext());
	}
	for (int i=0;i<TypeNum.GetSize();i++)
	{
		sql.Format("SELECT * FROM NAsmType Where Number=%s",TypeNum[i]);
		Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
		{
			return FALSE;
		}
		else
		{
			m_db.MoveBegin();
			do {
				str=m_db.getString("Name");
				m_listprt.SetItemText(i,3,str);
			} while(m_db.MoveNext());
		}
	}
	return TRUE;
}

void CPdtModifyDlg::OnBnClickedRadioWorkapace()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listprt.DeleteAllItems();
	m_workspace=0;
	GetDlgItem(IDC_CHECK_AUDIT)->EnableWindow(TRUE);
}

void CPdtModifyDlg::OnBnClickedRadioPdm()
{
	// TODO: 在此添加控件通知处理程序代码
	m_audit=FALSE;
	m_workspace=1;
	m_listprt.DeleteAllItems();
	GetDlgItem(IDC_CHECK_AUDIT)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CPdtModifyDlg::OnNMClickTreePart(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	HTREEITEM hItem;
	CString str,sql;
	UINT uFlags=0;
	CPoint point;
	int num;
	m_listprt.DeleteAllItems();
	point= GetCurrentMessage()->pt;
	m_treeprt.ScreenToClient(&point);
	hItem =m_treeprt.HitTest(point, &uFlags);
	int i=m_audit;
	if (hItem!=NULL && (uFlags&TVHT_ONITEM))
	{
		num=(int)m_treeprt.GetItemData(hItem);
		if ((!m_audit) && m_workspace==0)
		{
			sql.Format("SELECT * FROM NProductInsInfoTable WHERE TypeNum=%d",num);
			if (!GetPdtFromBase(sql,FALSE))
			{
				AfxMessageBox("查找失败");
				return;
			}
		}
		if (m_audit && m_workspace==0)
		{
			sql.Format("SELECT * FROM NProductInsInfoTable WHERE TypeNum=%d AND FinishState='完成' AND AuditState='审核未通过'",num);
			if (!GetPdtFromBase(sql,FALSE))
			{
				AfxMessageBox("查找失败");
				return;
			}
		}
		else
			if(m_workspace==1)
			{
				sql.Format("SELECT * FROM NProductInsInfoTable WHERE TypeNum=%d and PDMState='提交'",num);
				if (!GetPdtFromBase(sql,TRUE))
				{
					AfxMessageBox("查找失败");
					return;
				}
			}
		
	}
	*pResult = 0;
}
/************************************************************************/
/* 打开按钮                                                             */
/************************************************************************/
void CPdtModifyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CProductClass pdtclass;
	BOOL IsSelect=FALSE;
	BOOL IsLocal;
	CCommonFuncClass commonfun;
	CString InsCode,sql,SolidPath,DrwPath;
	for (int i=0;i<m_listprt.GetItemCount();i++)
	{
		if (m_listprt.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsSelect=TRUE;
			if(m_listprt.GetItemText(i,4)=="本地")
				IsLocal=TRUE;
			else
				IsLocal=FALSE;
			InsCode.Format("%d",m_listprt.GetItemData(i));           //得到产品编号
			break;
		}
	}
	if (!IsSelect)
	{
		return;
	}
	sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%s",InsCode);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0 )
	{
		AfxMessageBox("这个图号的产品不存在！");
		return;
	}
	else
	{
		m_db.MoveEnd();
	    InsCode.Format("%d",m_db.getInt("PdtNum"));
	}
	if (!pdtclass.OpenProductFile(InsCode))            //输入的是产品编号
	{
		return;
	}
	OnOK();
}
void CPdtModifyDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;
	status=ProWindowRepaint(PRO_VALUE_UNUSED);

	// TODO: 在此添加消息处理程序代码
}
