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
	CPartMdlRecordSet m_pSet;
	CTreeCtrl m_Tree;
	void ShowPartMdlTree(void);
	void AddSubPartItem(HTREEITEM item);
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
	// 判断信息显示模型，是否是当前，还是零件库中的信息
	//CButton m_RadioFalg;
	//afx_msg void OnBnClickedCom1();
	//afx_msg void OnBnClickedCom2();
	//// 性能参数表
	//CButton m_PerfTableCheckBox;
	//// 事务特性表复选框
	//CButton m_SWTXTableCheckBox;
	//// 关系式复选按钮
	//CButton m_RelTableBox;
	//// 图片显显示复选按钮
	//CButton m_PicBox;
private:
	//// 单选按钮标志
	//int radioFalg;
public:
	// 根据复选框的属性来设置相应需要显示的页面
	void SetSheepPage(void);
	
	// 图片浏览页
	CPropPage5 m_page5;

	afx_msg void OnBnClickedButton2();
	//afx_msg void OnBnClickedCheck4();
	//afx_msg void OnBnClickedCheck3();
	//afx_msg void OnBnClickedCheck2();
	//afx_msg void OnBnClickedCheck1();
	int SetSheetOfCurrentMdl(void);
	//int SetSheetOFDbMdl(void);
	//CButton m_radioFlag2;
	CFtpFile m_ftp;
	afx_msg void OnDelParttype();
	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelectTypeForMdl();
	afx_msg void OnPartRename();
	afx_msg void OnRefreshTree();
	afx_msg void OnDelPartmdl();
	afx_msg void OnL12108();
	afx_msg void OnMenuMdlInfo();
};
