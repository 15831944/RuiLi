// UserDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "UserDlg.h"
#include "MyDatabase.h"


extern CMyDatabase m_db;
// CUserDlg 对话框

IMPLEMENT_DYNAMIC(CUserDlg, CDialog)
CUserDlg::CUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserDlg::IDD, pParent)
	, m_user(_T(""))
	, m_password(_T(""))
{
}

CUserDlg::~CUserDlg()
{
}

void CUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USER, m_user);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
}


BEGIN_MESSAGE_MAP(CUserDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CUserDlg 消息处理程序

void CUserDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UserName=m_user;
	CString sql,str;
	sql.Format("select * from zd_UserInfo where UserLogName='%s'",m_user);
	int Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		AfxMessageBox("用户名错误");
        IsOK=FALSE;
		return;
	}
	else
	{
		m_db.MoveBegin();
		do {
			if (m_db.getString("Password")==m_password)
			{
				GL_USERNAME=m_user;
                IsOK=TRUE;
			}
			else
			{
				   AfxMessageBox("密码错误");
                   IsOK=FALSE;
				   return;
			}
		} while(m_db.MoveNext());
	}
	OnOK();
}
