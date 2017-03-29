#pragma once
#include "afxcmn.h"
#include "afxcview.h"
#include "Part_Temp_Mdl_Info.h"
#include "afxwin.h"
#include "PicViewDlg.h"
#include "PicForDBDlg.h"
// CMenuOpenDlg 对话框

class CMenuOpenDlg : public CDialog
{
	DECLARE_DYNAMIC(CMenuOpenDlg)

public:
	CMenuOpenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMenuOpenDlg();

// 对话框数据
	enum { IDD = IDD_OPEN_MENU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CImageList m_ListImgList;
	CImageList m_TreeImgList;
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	void SetTab(void);
	CTabCtrl m_Tab;
	CTreeCtrl m_Tree[6];
	void ShowTree(void);
	afx_msg void OnTcnSelchangeTab3(NMHDR *pNMHDR, LRESULT *pResult);
	CPart_Temp_Mdl_Info m_pSet;
	CDatabase * p_db;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOpenButton();
	afx_msg void OnBnClickedPicButton();
	afx_msg void OnBnClickedViewLocalFile();
protected:
	virtual void OnOK();
public:
	CStatic m_pic;
	CPicForDBDlg pic_dlg;
	afx_msg void OnPaint();
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL OpenProduct(long pdt_num);
	/*BOOL NewMdl(void);*/
	int m_iNewType;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnMove(int x, int y);
};
