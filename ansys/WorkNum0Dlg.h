#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"

// CWorkNum0Dlg �Ի���

class CWorkNum0Dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CWorkNum0Dlg)

public:
	CWorkNum0Dlg();
	virtual ~CWorkNum0Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WORKNUM0 };

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
	double m_r2;
	double m_r1;
	double m_n;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
	
};
