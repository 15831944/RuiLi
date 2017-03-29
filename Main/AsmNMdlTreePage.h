#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CAsmNMdlTreePage 对话框

class CAsmNMdlTreePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmNMdlTreePage)

public:
	CAsmNMdlTreePage();
	virtual ~CAsmNMdlTreePage();

// 对话框数据
	enum { IDD = IDD_ASM_MDL_TREE_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree;
	CString m_Name;
	CString m_Code;
	CString m_TypeName;
	CString m_MdlName;
	CString m_Material;
	CString m_AuditState;
	CString m_Resoure;
	CString m_Note;
	ProSolid mdl;
	virtual BOOL OnInitDialog();
	CButton m_CommitPDM;
	CButton m_AuditBtn;
	CButton m_UpdateBtn;
	CImageList imageList;
	int ShowMdlTree(ProSolid solid,HTREEITEM hItem);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();
};
