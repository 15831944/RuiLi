#pragma once
#include "MyDatabase.h"
//#include "material.h"
#include "afxwin.h"

// CTestDlg �Ի���

class CTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_test };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CMyDatabase m_db;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CString m_Text1;
	CString m_Text2;
	CComboBox m_combo;
	afx_msg void OnBnClickedButton4();
};
