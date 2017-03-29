#pragma once
#include "afxcmn.h"


// CPrtAsmAuditDlg 对话框

class CPrtAsmAuditDlg : public CDialog
{
	DECLARE_DYNAMIC(CPrtAsmAuditDlg)

public:
	CPrtAsmAuditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPrtAsmAuditDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PRTNDAUDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_list;
	ProSolid ProductSolid;
	CImageList m_ImageList;
	BOOL ListPart(int *i,CString Name,CListCtrl *list);
	BOOL AffirmWhichPartIsNotAlreadyAudit(int SolidID,CListCtrl *m_list);
	int s;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNMDblclkListPrtndaudit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
};
