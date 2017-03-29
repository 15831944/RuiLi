// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "TestDlg.h"


// CTestDlg 对话框

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


// CTestDlg 消息处理程序
BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_db.Open("DSN=jisuan ;UID=;PWD=");
	//m_db.Open(globl_connectString);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}


void CTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	
	CString strSql;
	int num;
	strSql.Format("select * from material where name='%s'",m_Text1);
	num=m_db.Execute(strSql);
	if(num==0)
	{
		AfxMessageBox("错误");
		return;
	}
	m_db.MoveBegin();
	m_Text2=m_db.getString("G");
	UpdateData(FALSE);

}

void CTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_db.Close();
	OnOK();
}

void CTestDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_db.Close();
	OnCancel();
}


void CTestDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	
}
