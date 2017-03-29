#pragma once
#include "picshow.h"
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"


// CAreaDlg 对话框

class CAreaDlg : public CDialog
{
	DECLARE_DYNAMIC(CAreaDlg)

public:
	CAreaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAreaDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_AREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	double m_diam1;
	double m_diam2;
	double m_s;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CPicShow m_pic;
	virtual BOOL OnInitDialog();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton3();
};
