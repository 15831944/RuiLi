#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"

// CLoad0Dlg 对话框

class CLoad0Dlg : public CDialog
{
	DECLARE_DYNAMIC(CLoad0Dlg)

public:
	CLoad0Dlg();
	virtual ~CLoad0Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOAD0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnBnClickedButton2();
	double m_g;
	double m_d;
	double m_a;
	double m_r2;
	double m_r1;
	double m_n;
	double m_ri;
	double m_d1;
	double m_fi;
	afx_msg void OnBnClickedButton1();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
};
