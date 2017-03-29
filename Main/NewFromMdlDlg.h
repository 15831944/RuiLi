#pragma once
#include "afxdlgs.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ComboListCtrl.h"
#include "partmdlrecordset.h"
// CNewFromMdlDlg 对话框

class CNewFromMdlDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewFromMdlDlg)

public:
	CNewFromMdlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewFromMdlDlg();

// 对话框数据
	enum { IDD = IDD_NEW_FROM_MDL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree1;
	CTreeCtrl m_Tree2;
	CTabCtrl m_Tab;
	CComboListCtrl m_List;
	CEdit m_Path;
	//CEdit m_Name;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	int m_selTab;
	CPartMdlRecordSet m_pSet;
	CImageList m_ImageList;
	afx_msg void OnTvnSelchangedTree5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	long mdlSelectNumber;
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	CString m_Name;
	void OkPart(CString new_name);
};
