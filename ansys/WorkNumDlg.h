#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"

// CWorkNumDlg �Ի���

class CWorkNumDlg : public CDialog
{
	DECLARE_DYNAMIC(CWorkNumDlg)

public:
	CWorkNumDlg();
	virtual ~CWorkNumDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WORKNUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	double m_g;
	double m_d;
	double m_f;
	double m_ff;
	double m_dd;
	double m_n;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
};
