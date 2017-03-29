#pragma once
#include "afxcmn.h"
#include "MyDatabase.h"
#include "stdafx.h"
#include "afxwin.h"
#include "ProSolid.h"
#include "ListCtrlEx.h"

// CModelAlterDlg 对话框

class CModelAlterDlg : public CDialog
{
	DECLARE_DYNAMIC(CModelAlterDlg)

public:
	CModelAlterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModelAlterDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ALTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
public:
	CListCtrlEx m_showdimen;
	//CListCtrl m_showdimengroup;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnNMClickListDimen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnNMRclickListDimen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnHotTrackListDimen(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CToolTipCtrl m_tooltip;
	CString mdlnum;
	int n_bian1;
	BOOL IsGen;
	BOOL InRelation;
private:
	//BOOL m_btogglesize;
	//CRect m_rectfull;
	//CRect m_recthalf;
public:
	//CButton m_button;
	//BOOL IsKong;
	afx_msg void OnMove(int x, int y);
};
