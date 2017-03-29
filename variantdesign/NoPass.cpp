// NoPass.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "NoPass.h"
#include "ProWindows.h"
#include "NopassDetail.h"

// CNoPass 对话框

IMPLEMENT_DYNAMIC(CNoPass, CDialog)
CNoPass::CNoPass(CWnd* pParent /*=NULL*/)
	: CDialog(CNoPass::IDD, pParent)
{
}

CNoPass::~CNoPass()
{
}

void CNoPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNoPass, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_NOPASS_YES, OnNopassYes)
	ON_BN_CLICKED(IDC_NOPASS_NO, OnBnClickedNopassNo)
	ON_BN_CLICKED(IDC_NOPASS_DETAIL, OnNopassDetail)
END_MESSAGE_MAP()


// CNoPass 消息处理程序

BOOL CNoPass::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CNoPass::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRefresh(PRO_VALUE_UNUSED);

	// TODO: 在此添加消息处理程序代码
}

void CNoPass::OnNopassYes()
{
	CDialog::OnOK();
}

void CNoPass::OnBnClickedNopassNo()
{
	CDialog::OnCancel();
}

void CNoPass::OnNopassDetail()
{
	CNopassDetail m_dlg;
	m_dlg.DoModal();
}
