#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "MyDatabase.h"
#include "myproe.h"

// CLoadDlg �Ի���

class CLoadDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadDlg)

public:
	CLoadDlg();
	virtual ~CLoadDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	double m_n;
	double m_dd;
	double m_d;
	double m_f;
	double m_g;
	double m_ff;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CComboBox m_Combo;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
};
