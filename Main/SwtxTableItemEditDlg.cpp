// SwtxTableItemEditDlg.cpp : ʵ���ļ�
//���������Ե����ƺͱ��͹�����б༭�ĶԻ�

#include "stdafx.h"
#include "Main.h"
#include "SwtxTableItemEditDlg.h"


// CSwtxTableItemEditDlg �Ի���

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


// CSwtxTableItemEditDlg ��Ϣ�������

BOOL CSwtxTableItemEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	USR_SET_DLG_ICON()
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
