#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CProductDelDlg 对话框

class CProductDelDlg : public CDialog
{
	DECLARE_DYNAMIC(CProductDelDlg)

public:
	CProductDelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProductDelDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PRODUCTDEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_product;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	BOOL DeletePrtIns(int SolidID);
	BOOL DeleteProductIns(int SolidID,BOOL IsTheTop);
	BOOL QueryDatabase(CString sql,CString Type);
	BOOL IsUsedByProduct(int SolidID,BOOL *BeUsed,CString Type);
	CString m_inscode;
	CComboBox m_type;
	afx_msg void OnBnClickedButtonQuery();
	int m_all;
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnMove(int x, int y);
};
