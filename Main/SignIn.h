#pragma once
#include "afxwin.h"


// CSignIn 对话框

class CSignIn : public CDialog
{
	DECLARE_DYNAMIC(CSignIn)

public:
	CSignIn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSignIn();

// 对话框数据
	enum { IDD = IDD_SignIn };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_staticname;
	CStatic m_staticpwd;
	virtual BOOL OnInitDialog();
	afx_msg void OnLogIn();
	afx_msg void OnBtnNo();
};
