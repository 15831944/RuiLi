#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"

// CStabilityDlg 对话框

class CStabilityDlg : public CDialog
{
	DECLARE_DYNAMIC(CStabilityDlg)

public:
	CStabilityDlg();
	virtual ~CStabilityDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_STABILITY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	double m_h0;
	double m_d;
	double m_u;
	double m_fc;
	double m_fcfc;
	double m_g1;
	double m_d1;
	double m_n;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton4();
};
