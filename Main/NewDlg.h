#pragma once
#include "afxwin.h"


// CNewDlg 对话框

class CNewDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewDlg)

public:
	CNewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewDlg();

// 对话框数据
	enum { IDD = IDD_NEWMDL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Name;
	int m_Template;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	bool mynewfileprt(char  filename[]);
	
	//CButton m_radiobutton;
	virtual BOOL OnInitDialog();
	int m_radioFlag;
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();
	bool mynewfileasm(char  filename[]);
	bool mynewfiledwg(char  filename[]);
	CButton m_RadioDrawing;
	CString m_FileName;
	afx_msg void OnMove(int x, int y);
};
