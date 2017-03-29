// SignIn.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "SignIn.h"

extern UserInfo curUserInfo;

// CSignIn �Ի���

IMPLEMENT_DYNAMIC(CSignIn, CDialog)
CSignIn::CSignIn(CWnd* pParent /*=NULL*/)
	: CDialog(CSignIn::IDD, pParent)
{
}

CSignIn::~CSignIn()
{
}

void CSignIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICName, m_staticname);
	DDX_Control(pDX, IDC_STATICPwd, m_staticpwd);
}


BEGIN_MESSAGE_MAP(CSignIn, CDialog)
	ON_BN_CLICKED(IDC_YES, OnLogIn)
	ON_BN_CLICKED(IDC_NO, OnBtnNo)
END_MESSAGE_MAP()


// CSignIn ��Ϣ�������
BOOL CSignIn::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CFont* font=new CFont();

	font->CreateFont(14,0,0,0,1000,0,0,0,

		ANSI_CHARSET,OUT_DEFAULT_PRECIS,

		CLIP_DEFAULT_PRECIS,

		DEFAULT_QUALITY,

		DEFAULT_PITCH|FF_DONTCARE,

		"arial"); 

	// Cause the label to use the new font

	m_staticname.SetFont(font);
	m_staticpwd.SetFont(font);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
void CSignIn::OnLogIn()
{
	CString strUserName;
	CString strPwd;

	GetDlgItemText(IDC_UserName,strUserName);
	GetDlgItemText(IDC_Password,strPwd);

	if(strUserName=="")
	{
		MessageBox("��ʹ���û�����¼��","��ʾ��Ϣ");
	}
	else
	{
		CString strSql;
		strSql.Format("select * from zd_UserInfo where UserLogName='%s'",strUserName);
		if(wshfm_db.Query(strSql)!=0)
		{
			int i=0;
			wshfm_db.MoveBegin();
			if(strPwd==wshfm_db.getString("Password"))
			{
				curUserInfo.name=strUserName;
				curUserInfo.authority=wshfm_db.getString("UserAttribute");
				CDialog::OnOK();
				return;
			}
		}
		MessageBox("�û���������������������룡","��ʾ��Ϣ");
	}
}

void CSignIn::OnBtnNo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}
