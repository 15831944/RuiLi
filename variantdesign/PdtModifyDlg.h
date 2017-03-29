#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DetailInfoClass.h"


// CPdtModifyDlg 对话框

class CPdtModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CPdtModifyDlg)

public:
	CPdtModifyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPdtModifyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PTDMODIMY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_key;
	CComboBox m_combokey;
	afx_msg void OnBnClickedButtonQuery();
	BOOL GetPdtFromBase(CString sql,BOOL PDM);
	int m_workspace;
	CTreeCtrl m_treeprt;
	CListCtrl m_listprt;
	DetailInfoClass *info;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel2();
	virtual BOOL OnInitDialog();
	BOOL m_audit;                          //这个变量是复选按钮
	afx_msg void OnBnClickedRadioWorkapace();
	afx_msg void OnBnClickedRadioPdm();
	afx_msg void OnNMClickTreePart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMove(int x, int y);
};
