#pragma once
#include "afxcmn.h"


// CProgressDlg 对话框

class CProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProgressDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
		int m_Pos;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	CProgressCtrl m_Progress;
	afx_msg void OnTimer(UINT nIDEvent);
	CString m_Label;
};
