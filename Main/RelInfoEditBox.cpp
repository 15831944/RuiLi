// RelInfoEditBox.cpp : ʵ���ļ�
//���ڶԹ�ϵʽ����Ϣ���б༭�ĶԻ���

#include "stdafx.h"
#include "Main.h"
#include "RelInfoEditBox.h"


// CRelInfoEditBox �Ի���

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


// CRelInfoEditBox ��Ϣ�������

void CRelInfoEditBox::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	OnOK();
}
