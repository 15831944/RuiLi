// TestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "TestDlg.h"


// CTestDlg �Ի���

IMPLEMENT_DYNAMIC(CTestDlg, CDialog)
CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
	, m_Text1(_T(""))
	, m_Text2(_T(""))
{
}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Text1);
	DDX_Text(pDX, IDC_EDIT2, m_Text2);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
END_MESSAGE_MAP()


// CTestDlg ��Ϣ�������
BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_db.Open("DSN=jisuan ;UID=;PWD=");
	//m_db.Open(globl_connectString);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}


void CTestDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
	CString strSql;
	int num;
	strSql.Format("select * from material where name='%s'",m_Text1);
	num=m_db.Execute(strSql);
	if(num==0)
	{
		AfxMessageBox("����");
		return;
	}
	m_db.MoveBegin();
	m_Text2=m_db.getString("G");
	UpdateData(FALSE);

}

void CTestDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_db.Close();
	OnOK();
}

void CTestDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_db.Close();
	OnCancel();
}


void CTestDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}
