// LabelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LabelDlg.h"
//#include "myproe.h"


// CLabelDlg 对话框

IMPLEMENT_DYNAMIC(CLabelDlg, CDialog)
CLabelDlg::CLabelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLabelDlg::IDD, pParent)
	, m_strEdit(_T(""))
	, m_oldname(_T(""))
	, m_Note(_T(""))
{
}

CLabelDlg::~CLabelDlg()
{
}

void CLabelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MY_EDIT_UP_DIM_LABEL, m_strEdit);
	DDX_Text(pDX, IDC_MY_UP_DIM_LABEL, m_oldname);
	DDX_Text(pDX, IDC_MY_NOTE, m_Note);
}


BEGIN_MESSAGE_MAP(CLabelDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CLabelDlg 消息处理程序

void CLabelDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnOK();
}

BOOL CLabelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CLabelDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此添加消息处理程序代码
	ProWindowRepaint(PRO_VALUE_UNUSED);
}
