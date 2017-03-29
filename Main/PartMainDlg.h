#pragma once
#include "afxcmn.h"
#include "afxdlgs.h"
#include "proppage1.h"
#include "proppage2.h"
#include "proppage3.h"
#include "afxwin.h"
#include "proppage4.h"
#include "proppage5.h"
#include "ftpfile.h"
#include "NPartTypeSet.h"


// CPartMainDlg 对话框

class CPartMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartMainDlg)

public:
	CPartMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPartMainDlg();

// 对话框数据
	enum { IDD = IDD_MAINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CDatabase db;
	ProSolid         part_solid;
	CNPartTypeSet    m_pSet;
	CTreeCtrl m_Tree;
	//void ShowPartMdlTree(void);
	//void AddSubPartItem(HTREEITEM item);
	virtual BOOL OnInitDialog();
	CImageList m_ImageList;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnNewParttype();
	afx_msg void OnClose();
	
	// 属性页
	CPropertySheet m_sheet;
	CPropPage1 m_page1;
	CPropPage2 m_page2;
	CPropPage3 m_page3;
	CPropPage4 m_page4;
	afx_msg void OnMove(int x, int y);
private:
public:
	// 根据复选框的属性来设置相应需要显示的页面
	void SetSheepPage(void);
	
	// 图片浏览页
	CPropPage5 m_page5;

	afx_msg void OnBnClickedButton2();
	int SetSheetOfCurrentMdl(void);
	CFtpFile m_ftp;
	afx_msg void OnDelParttype();
	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelectTypeForMdl();
	afx_msg void OnPartRename();
	afx_msg void OnRefreshTree();
	afx_msg void OnDelPartmdl();
	afx_msg void OnL12108();
	afx_msg void OnMenuMdlInfo();
	afx_msg void OnUpdateMdl();
	int getRights(CString strAuthor);
	BOOL IsHasRight(CString theRight);
};
