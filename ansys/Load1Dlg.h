#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"

// CLoad1Dlg 对话框

class CLoad1Dlg : public CDialog
{
	DECLARE_DYNAMIC(CLoad1Dlg)

public:
	CLoad1Dlg();
	virtual ~CLoad1Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOAD1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	double m_g;
	double m_d;
	double m_r2;
	double m_r1;
	double m_t;
	double m_ri;
	double m_fi;
	double m_n;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonload1();
	CComboBox m_Combo;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
};
