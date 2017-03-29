#pragma once
#include "afxcmn.h"
#include "FemPrtSet.h"
#include "FtpFile.h"
#include "myproe.h"


// CFemModelSave 对话框

class CFemModelSave : public CDialog
{
	DECLARE_DYNAMIC(CFemModelSave)

public:
	CFemModelSave(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFemModelSave();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ProSolid part_solid;
	CImageList m_ImageList;
	CFtpFile m_ftp;
	CFemPrtSet m_pSet;
	void AddSubPartItem(HTREEITEM item);
	void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	void ShowPartMdlTree(void);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CTreeCtrl m_Tree;
	afx_msg void OnNewcat();
	afx_msg void OnDelcat();
	afx_msg void OnRefresh();
};
