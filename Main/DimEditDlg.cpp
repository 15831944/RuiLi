// DimEditDlg.cpp : 实现文件
//用于对选择的尺寸进行编辑

#include "stdafx.h"
#include "Main.h"
#include "DimEditDlg.h"


// CDimEditDlg 对话框

IMPLEMENT_DYNAMIC(CDimEditDlg, CDialog)
CDimEditDlg::CDimEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDimEditDlg::IDD, pParent)
	, m_ID(_T(""))
	, m_Symbol(_T(""))
	, m_Value(0)
	, m_UpLimit(0)
	, m_DwLimit(0)
{
}

CDimEditDlg::~CDimEditDlg()
{
}

void CDimEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ID);
	DDX_Text(pDX, IDC_EDIT2, m_Symbol);
	DDX_Text(pDX, IDC_EDIT3, m_Value);
	DDX_Text(pDX, IDC_EDIT4, m_UpLimit);
	DDX_Text(pDX, IDC_EDIT5, m_DwLimit);
}


BEGIN_MESSAGE_MAP(CDimEditDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDimEditDlg 消息处理程序

void CDimEditDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnOK();
}
