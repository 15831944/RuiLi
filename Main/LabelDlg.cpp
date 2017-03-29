// LabelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LabelDlg.h"
//#include "myproe.h"


// CLabelDlg �Ի���

IMPLEMENT_DYNAMIC(CLabelDlg, CDialog)
CLabelDlg::CLabelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLabelDlg::IDD, pParent)
	, m_strEdit(_T(""))
	, m_oldname(_T(""))
	, m_Note(_T(""))
{
}

CLabelDlg::~CLabelDlg()
{
}

void CLabelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MY_EDIT_UP_DIM_LABEL, m_strEdit);
	DDX_Text(pDX, IDC_MY_UP_DIM_LABEL, m_oldname);
	DDX_Text(pDX, IDC_MY_NOTE, m_Note);
}


BEGIN_MESSAGE_MAP(CLabelDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CLabelDlg ��Ϣ�������

void CLabelDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	OnOK();
}

BOOL CLabelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CLabelDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: �ڴ������Ϣ����������
	ProWindowRepaint(PRO_VALUE_UNUSED);
}
