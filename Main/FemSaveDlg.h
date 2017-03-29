#pragma once
#include "afxcmn.h"
#include "FemPrt.h"

// CFemSaveDlg 对话框

class CFemSaveDlg : public CDialog
{
	DECLARE_DYNAMIC(CFemSaveDlg)

public:
	CFemSaveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFemSaveDlg();

// 对话框数据
	enum { IDD = IDD_FEM_SAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedEdit();
	afx_msg void OnBnClickedCancel();
	CTreeCtrl m_Tree;
	CListCtrl m_List;
	CFemPrt   m_pSet;
	CImageList m_ImageList;
	virtual BOOL OnInitDialog();
	CString m_Name;
	CString m_Note;
	CString m_Type;
	long    m_SelTypeNum;
	long    m_SelMdlNum;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void    ShowFemMdlInList(long typnum);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnMove(int x, int y);
};
