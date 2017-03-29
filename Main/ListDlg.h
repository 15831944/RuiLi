#pragma once
#include "afxcmn.h"


// CListDlg 对话框

class CListDlg : public CDialog
{
	DECLARE_DYNAMIC(CListDlg)

public:
	CListDlg(CWnd* pParent = NULL);   // 标准构造函数
	CListDlg(CWnd* pParent /*=NULL*/,CString title);
	virtual ~CListDlg();

// 对话框数据
	enum { IDD = IDD_LIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	// 对话框的标题
	CString m_strText;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
