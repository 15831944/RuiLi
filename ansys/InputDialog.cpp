// InputDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "InputDialog.h"


// CInputDialog �Ի���

IMPLEMENT_DYNAMIC(CInputDialog, CDialog)
CInputDialog::CInputDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDialog::IDD, pParent)
	, m_Text(_T(""))
{
}

CInputDialog::~CInputDialog()
{
}

void CInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Text);
}


BEGIN_MESSAGE_MAP(CInputDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CInputDialog ��Ϣ�������

void CInputDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	OnOK();
}
