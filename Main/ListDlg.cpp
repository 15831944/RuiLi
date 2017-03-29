// ListDlg.cpp : ʵ���ļ�
//�öԻ���ר�������б���в���

#include "stdafx.h"
#include "Main.h"
#include "ListDlg.h"
#include "AsmMdlManDlg.h"
// CListDlg �Ի���

IMPLEMENT_DYNAMIC(CListDlg, CDialog)
CListDlg::CListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListDlg::IDD, pParent)
	, m_strText(_T(""))
{
}
CListDlg::CListDlg(CWnd* pParent /*=NULL*/,CString title)
: CDialog(CListDlg::IDD, pParent)
, m_strText(title)
{
}
CListDlg::~CListDlg()
{
}

void CListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CListDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CListDlg ��Ϣ�������

BOOL CListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    if(!m_strText.IsEmpty()) SetWindowText(m_strText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CListDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->GetParent()->SendMessage(WM_LIST_OK,0,0);
	OnOK();
}
