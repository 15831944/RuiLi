#pragma once
#include "ComboGridCtrl.h"
#include "Fr.h"
#include "afxwin.h"
// CListDlg 对话框

class CListDlg : public CDialog
{
	DECLARE_DYNAMIC(CListDlg)

public:
	CListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListDlg();
	CComboGridCtrl m_GridCtrl;
	void InitGridCtrl();
	void SetDB(CDatabase *);
    CFr   m_pSet;
	bool  IsAddMode;
// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedSave();
	CButton m_AddButton;
	CButton m_SaveButton;
	afx_msg void OnBnClickedDelete();
	CButton m_DELETEBUTTON;
};
