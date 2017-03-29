#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ListDlg.h"
// CAsmMdlTreePage 对话框

class CAsmMdlTreePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmMdlTreePage)

public:
	CAsmMdlTreePage();
	virtual ~CAsmMdlTreePage();

// 对话框数据
	enum { IDD = IDD_ASM_MDL_TREE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	ProSolid mdl;
	CTreeCtrl m_Tree;
	CString m_CompName;
	CString m_CompCode;
	CString m_CompFileName;
	CString m_CompType;
	CString m_CompNote;
	UsrPartPorp * tempPartPorp;
	CImageList imageList;
	virtual BOOL OnInitDialog();
	int ShowMdlTree(ProSolid solid,HTREEITEM hItem);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	int selNum;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CComboBox m_Combo;
	CString m_DefInsName;
	CString m_DefInsCode;
	CListDlg * list_dlg;
	CDatabase * p_db;
	void OnListDlgOk(WPARAM wParam,LPARAM lParam);
};
