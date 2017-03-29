#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxtempl.h"
#include "listctrlex.h"


// CSearchDlg 对话框

class CSearchDlg : public CDialog
{
	DECLARE_DYNAMIC(CSearchDlg)

public:
	CSearchDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSearchDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SEARCHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnNMClickListQuery(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListQuery(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickTreeAsm(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnMove(int x, int y);
	//void SearchFromResult(CStringArray *stringarray);
	BOOL GetProduct(CString AsmNum,CString ProductMap,CString LocalPath,CString *path,CStringArray *DwgNameArray);
	CTreeCtrl m_asmtree;
	//CComboBox m_combotype;
	CListCtrl m_listquery;
	CComboBox m_performance;
	BOOL m_all;
	int modelnum;
	CString m_dimenname;

	int picNum;
	CString InsCode;
	BOOL IsSelect;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonAlter();

	afx_msg void OnNMClickTreeAsm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListQuery(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPreview2d();
	afx_msg void OnPreview3d();
};
