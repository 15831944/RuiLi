#pragma once
#include "afxcmn.h"


// CUnitDlg 对话框

class CUnitDlg : public CDialog
{
	DECLARE_DYNAMIC(CUnitDlg)

public:
	CUnitDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUnitDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_UNIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_ID;
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMClickListProduct(NMHDR *pNMHDR, LRESULT *pResult);
};
