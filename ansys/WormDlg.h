#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"

// CWormDlg 对话框

class CWormDlg : public CDialog
{
	DECLARE_DYNAMIC(CWormDlg)

public:
	CWormDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWormDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	double m_d1;
	double m_F;
	double m_F1;
	double m_F0;
	double m_d;
	double m_Fs;
	double m_h;
	double m_G1;
	double m_G2;
	double m_G3;
	double m_t;
	double m_G4;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	double m_m;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton11();
	virtual BOOL OnInitDialog();
};
