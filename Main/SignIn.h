#pragma once
#include "afxwin.h"


// CSignIn �Ի���

class CSignIn : public CDialog
{
	DECLARE_DYNAMIC(CSignIn)

public:
	CSignIn(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSignIn();

// �Ի�������
	enum { IDD = IDD_SignIn };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_staticname;
	CStatic m_staticpwd;
	virtual BOOL OnInitDialog();
	afx_msg void OnLogIn();
	afx_msg void OnBtnNo();
};
