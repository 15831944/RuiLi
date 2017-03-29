#pragma once
#include "afxcmn.h"
#include "FtpDirTreeCtrl.h"


// CStyleStandDlg 对话框

class CStyleStandDlg : public CDialog
{
	DECLARE_DYNAMIC(CStyleStandDlg)

public:
	CStyleStandDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStyleStandDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFtpDirTreeCtrl m_FtpTree;
    CString path;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnNMClickTreeStylestand(NMHDR *pNMHDR, LRESULT *pResult);
	
};
