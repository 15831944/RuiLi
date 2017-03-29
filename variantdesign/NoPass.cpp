// NoPass.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "NoPass.h"
#include "ProWindows.h"
#include "NopassDetail.h"

// CNoPass �Ի���

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


// CNoPass ��Ϣ�������

BOOL CNoPass::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CNoPass::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRefresh(PRO_VALUE_UNUSED);

	// TODO: �ڴ������Ϣ����������
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
