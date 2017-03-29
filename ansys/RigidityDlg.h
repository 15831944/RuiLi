#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"

// CRigidityDlg �Ի���

class CRigidityDlg : public CDialog
{
	DECLARE_DYNAMIC(CRigidityDlg)

public:
	CRigidityDlg();
	virtual ~CRigidityDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RIGIDITY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	double m_d;
	double m_dd;
	double m_g;
	double m_n;
	double m_f1;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
};
