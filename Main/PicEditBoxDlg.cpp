// PicEditBoxDlg.cpp : ʵ���ļ�
//���ڶ����ɵ�ͼƬ�����ƽ��б༭�ĶԻ���

#include "stdafx.h"
#include "Main.h"
#include "PicEditBoxDlg.h"


// CPicEditBoxDlg �Ի���

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


// CPicEditBoxDlg ��Ϣ�������

void CPicEditBoxDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
