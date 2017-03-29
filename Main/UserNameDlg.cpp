// UserNameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "UserNameDlg.h"
#include "MyDatabase.h"

extern CString         dbLinkInfo;
// CUserNameDlg 对话框

IMPLEMENT_DYNAMIC(CUserNameDlg, CDialog)
CUserNameDlg::CUserNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserNameDlg::IDD, pParent)
	, m_Text(_T(""))
{
	m_UserName="";
}

CUserNameDlg::~CUserNameDlg()
{
}

void CUserNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Text, m_Text);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_List);
}


BEGIN_MESSAGE_MAP(CUserNameDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT, OnNMClickListProduct)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRODUCT, OnNMDblclkListProduct)
END_MESSAGE_MAP()


// CUserNameDlg 消息处理程序

BOOL CUserNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,"用户名",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(1,"姓名",LVCFMT_LEFT,200,-1);

	CMyDatabase db;
	if(db.Open(dbLinkInfo)==0) return TRUE;
	CString strSql;
	strSql.Format("select * from zd_UserInfo");
	int status,rows;
	status=db.Query(strSql);
	rows=db.getRows();
	CString str;
	if(rows>0)
	{
		db.MoveBegin();
		for(int i=0;i<rows;i++)
		{
			str=db.getString("UserLogName");
			m_List.InsertItem(i,str);
			str=db.getString("UserName");
			m_List.SetItemText(i,1,str);
			db.MoveNext();
		}
	}
  db.Close();
	if(m_UserName!="")
	{
		int number=-1;
		for(int j=0;j<m_List.GetItemCount();j++)
		{
			if (m_List.GetItemText(j,1)==m_UserName)
			{
				number=j;
			}
		}
		if(number!=-1)
		{ 
			m_List.SetItemState(number,LVIS_SELECTED,LVIS_SELECTED);
			m_Text.Format("你选择的是 %s",m_UserName);
			UpdateData(FALSE);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CUserNameDlg::OnBnClickedOk()
{
	int number=-1;
	for(int j=0;j<m_List.GetItemCount();j++)
	{
		if ( m_List.GetItemState(j,LVIS_SELECTED)==LVIS_SELECTED)
		{
			number=j;
		}
	}
	if(number==-1) return;

	m_UserName=m_List.GetItemText(number,1);////////
	OnOK();
}

void CUserNameDlg::OnNMClickListProduct(NMHDR *pNMHDR, LRESULT *pResult)
{
	int number=-1;
	for(int j=0;j<m_List.GetItemCount();j++)
	{
		if ( m_List.GetItemState(j,LVIS_SELECTED)==LVIS_SELECTED)
		{
			number=j;
		}
	}
	if(number==-1) return;
	m_UserName=m_List.GetItemText(number,1);////////
	m_Text.Format("你选择的是 %s",m_UserName);
	UpdateData(FALSE);
}

void CUserNameDlg::OnNMDblclkListProduct(NMHDR *pNMHDR, LRESULT *pResult)
{
	int number=-1;
	for(int j=0;j<m_List.GetItemCount();j++)
	{
		if ( m_List.GetItemState(j,LVIS_SELECTED)==LVIS_SELECTED)
		{
			number=j;
		}
	}
	if(number==-1) return;

	m_UserName=m_List.GetItemText(number,1);////////
	OnOK();
}
