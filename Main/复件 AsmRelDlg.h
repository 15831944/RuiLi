#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CAsmRelDlg 对话框

class CAsmRelDlg : public CDialog
{
	DECLARE_DYNAMIC(CAsmRelDlg)

public:
	CAsmRelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAsmRelDlg();

// 对话框数据
	enum { IDD = IDD_ASM_REL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	CEdit m_RelEdit;
	CEdit m_RelInfoEdit;
	afx_msg void OnBnClickedButton13();
	ProSolid asm_solid;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton12();
	void ShowRelSet(ProSolid solid);
};
