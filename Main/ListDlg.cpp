// ListDlg.cpp : 实现文件
//该对话框专门用于列表进行操作

#include "stdafx.h"
#include "Main.h"
#include "ListDlg.h"
#include "AsmMdlManDlg.h"
// CListDlg 对话框

IMPLEMENT_DYNAMIC(CListDlg, CDialog)
CListDlg::CListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListDlg::IDD, pParent)
	, m_strText(_T(""))
{
}
CListDlg::CListDlg(CWnd* pParent /*=NULL*/,CString title)
: CDialog(CListDlg::IDD, pParent)
, m_strText(title)
{
}
CListDlg::~CListDlg()
{
}

void CListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CListDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CListDlg 消息处理程序

BOOL CListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
    if(!m_strText.IsEmpty()) SetWindowText(m_strText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CListDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->GetParent()->SendMessage(WM_LIST_OK,0,0);
	OnOK();
}
