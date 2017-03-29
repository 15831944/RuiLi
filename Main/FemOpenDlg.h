#pragma once
#include "afxcmn.h"
#include "FemPrt.h"

// CFemOpenDlg 对话框
BOOL ShowFemMdlTree(CTreeCtrl & m_Tree,CDatabase * m_db);
class CFemOpenDlg : public CDialog
{
	DECLARE_DYNAMIC(CFemOpenDlg)

public:
	CFemOpenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFemOpenDlg();

// 对话框数据
	enum { IDD = IDD_FEM_OPEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree;
	CListCtrl m_List;
	CFemPrt   m_pSet;
	CImageList m_ImageList;
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
//	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnMove(int x, int y);
};
