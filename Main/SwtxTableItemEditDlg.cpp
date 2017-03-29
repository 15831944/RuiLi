// SwtxTableItemEditDlg.cpp : 实现文件
//对事物特性的名称和变型规则进行编辑的对话

#include "stdafx.h"
#include "Main.h"
#include "SwtxTableItemEditDlg.h"


// CSwtxTableItemEditDlg 对话框

IMPLEMENT_DYNAMIC(CSwtxTableItemEditDlg, CDialog)
CSwtxTableItemEditDlg::CSwtxTableItemEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSwtxTableItemEditDlg::IDD, pParent)
	, m_Name(_T(""))
	, m_Note(_T(""))
{
}

CSwtxTableItemEditDlg::~CSwtxTableItemEditDlg()
{
}

void CSwtxTableItemEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_EDIT2, m_Note);
}


BEGIN_MESSAGE_MAP(CSwtxTableItemEditDlg, CDialog)
END_MESSAGE_MAP()


// CSwtxTableItemEditDlg 消息处理程序

BOOL CSwtxTableItemEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	USR_SET_DLG_ICON()
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
