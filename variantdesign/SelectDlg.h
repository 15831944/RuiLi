#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSelectDlg 对话框

class CSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectDlg)

public:
	CSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedOk();
    int s;
	int num;
	CString Name;
	CTreeCtrl m_parttree;
	CString   Type;
	afx_msg void OnNMClickTreePart(NMHDR *pNMHDR, LRESULT *pResult);
};
