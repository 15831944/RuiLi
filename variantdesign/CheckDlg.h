#pragma once
#include "afxdlgs.h"
#include "afxcmn.h"
#include "ListCtrlEx.h"
#include "afxwin.h"


// CCheckDlg 对话框

class CCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckDlg)

public:
	CCheckDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	CListCtrl    m_dimension;
	CString partnum;
	int s;
	afx_msg void OnNMClickListDim(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonOther();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonNo();
	BOOL CheckPart(int s);
};
