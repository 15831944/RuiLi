#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"

// CPoleDlg 对话框

class CPoleDlg : public CDialog
{
	DECLARE_DYNAMIC(CPoleDlg)

public:
	CPoleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPoleDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_POLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	double m_d;
	double m_P;
	double m_n;
	double m_M;
	double m_T;
	double m_t;
	double m_G;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	double m_a;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton7();
	virtual BOOL OnInitDialog();
};
