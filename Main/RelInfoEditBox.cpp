// RelInfoEditBox.cpp : 实现文件
//用于对关系式的信息进行编辑的对话框

#include "stdafx.h"
#include "Main.h"
#include "RelInfoEditBox.h"


// CRelInfoEditBox 对话框

IMPLEMENT_DYNAMIC(CRelInfoEditBox, CDialog)
CRelInfoEditBox::CRelInfoEditBox(CWnd* pParent /*=NULL*/)
	: CDialog(CRelInfoEditBox::IDD, pParent)
	, m_note(_T(""))
{
}

CRelInfoEditBox::~CRelInfoEditBox()
{
}

void CRelInfoEditBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT1, m_note);
	DDX_Text(pDX, IDC_EDIT1, m_note);
}


BEGIN_MESSAGE_MAP(CRelInfoEditBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CRelInfoEditBox 消息处理程序

void CRelInfoEditBox::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnOK();
}
