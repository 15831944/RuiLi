#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"

// CLoad2Dlg �Ի���

class CLoad2Dlg : public CDialog
{
	DECLARE_DYNAMIC(CLoad2Dlg)

public:
	CLoad2Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoad2Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOAD2 };

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
	double m_ff;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	CComboBox m_Combo;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
};
