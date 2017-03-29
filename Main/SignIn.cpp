// SignIn.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "SignIn.h"

extern UserInfo curUserInfo;

// CSignIn 对话框

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


// CSignIn 消息处理程序
BOOL CSignIn::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	// 异常：OCX 属性页应返回 FALSE
}
void CSignIn::OnLogIn()
{
	CString strUserName;
	CString strPwd;

	GetDlgItemText(IDC_UserName,strUserName);
	GetDlgItemText(IDC_Password,strPwd);

	if(strUserName=="")
	{
		MessageBox("请使用用户名登录！","提示信息");
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
		MessageBox("用户名或密码错误，请重新输入！","提示信息");
	}
}

void CSignIn::OnBtnNo()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}
