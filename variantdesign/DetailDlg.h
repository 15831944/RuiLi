#pragma once
#include "afxcmn.h"
#include "ProSolid.h"
#include "afxdlgs.h"
#include "DetailInfoClass.h"
#include "CommonFuncClass.h"


// CDetailDlg 对话框

class CDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(CDetailDlg)

public:
	CDetailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDetailDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedOk();
	//afx_msg void OnClose();
	CListCtrl m_list;
	ProSolid solid;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	BOOL GetHandleFromName(CString FileName,int i,CListCtrl *m_prtlist);
	BOOL ListThePartAndAsm(int SolidID,CListCtrl *m_prtlist);
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNMDblclkListInfo(NMHDR *pNMHDR, LRESULT *pResult);
};
