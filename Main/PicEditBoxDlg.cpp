// PicEditBoxDlg.cpp : 实现文件
//用于对生成的图片的名称进行编辑的对话框

#include "stdafx.h"
#include "Main.h"
#include "PicEditBoxDlg.h"


// CPicEditBoxDlg 对话框

IMPLEMENT_DYNAMIC(CPicEditBoxDlg, CDialog)
CPicEditBoxDlg::CPicEditBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicEditBoxDlg::IDD, pParent)
	, m_Note(_T(""))
{
}

CPicEditBoxDlg::~CPicEditBoxDlg()
{
}

void CPicEditBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Note);
}


BEGIN_MESSAGE_MAP(CPicEditBoxDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CPicEditBoxDlg 消息处理程序

void CPicEditBoxDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
