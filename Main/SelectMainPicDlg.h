#pragma once
#include "afxcmn.h"


// CSelectMainPicDlg 对话框

class CSelectMainPicDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectMainPicDlg)

public:
	CSelectMainPicDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectMainPicDlg();

// 对话框数据
	enum { IDD = IDD_SELECT_PIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
	ProMdl mdl;
	int SetMdl(ProMdl solid);
	CString select_file;
};
