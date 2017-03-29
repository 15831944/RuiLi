#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"

// CWorkNum1Dlg 对话框

class CWorkNum1Dlg : public CDialog
{
	DECLARE_DYNAMIC(CWorkNum1Dlg)

public:
	CWorkNum1Dlg();
	virtual ~CWorkNum1Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WORKNUM1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	double m_g;
	double m_d;
	double m_f;
	double m_r2;
	double m_r1;
	double m_ff;
	double m_n;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonwork();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
