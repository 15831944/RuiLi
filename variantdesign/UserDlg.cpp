// UserDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "UserDlg.h"
#include "MyDatabase.h"


extern CMyDatabase m_db;
// CUserDlg �Ի���

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


// CUserDlg ��Ϣ�������

void CUserDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	UserName=m_user;
	CString sql,str;
	sql.Format("select * from zd_UserInfo where UserLogName='%s'",m_user);
	int Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		AfxMessageBox("�û�������");
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
				   AfxMessageBox("�������");
                   IsOK=FALSE;
				   return;
			}
		} while(m_db.MoveNext());
	}
	OnOK();
}
