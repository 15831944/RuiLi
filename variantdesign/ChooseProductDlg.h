#pragma once
#include <ProWindows.h>
#include "afxcmn.h"
#include "MyDatabase.h"


// CChooseProductDlg 对话框

class CChooseProductDlg : public CDialog
{
	DECLARE_DYNAMIC(CChooseProductDlg)

public:
	CChooseProductDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChooseProductDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHOOSEPRODUCT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonOk();
	CListCtrl m_listproduct;
	virtual BOOL OnInitDialog();
	int m_show;
	afx_msg void OnBnClickedRadioPart();
	afx_msg void OnBnClickedRadioAsm();
	afx_msg void OnBnClickedRadioAll();
	BOOL SHOW(CListCtrl *m_list,CString sql);
};
