#pragma once
#include "afxcmn.h"


// CMaterialDlg 对话框

class CMaterialDlg : public CDialog
{
	DECLARE_DYNAMIC(CMaterialDlg)

public:
	CMaterialDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMaterialDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MATERIAL };

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
	CString m_Text;
	afx_msg void OnNMDblclkListProduct(NMHDR *pNMHDR, LRESULT *pResult);
};
