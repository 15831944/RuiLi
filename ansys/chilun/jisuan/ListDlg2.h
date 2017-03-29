#pragma once
#include "afxwin.h"
#include "ComboGridCtrl.h"
#include "ffa.h"
#include "afxwin.h"


// CListDlg2 对话框

class CListDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CListDlg2)

public:
	CListDlg2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListDlg2();
    CComboGridCtrl m_GridCtrl;
	void InitGridCtrl();
	void SetDB(CDatabase *);
	Cffa m_fSet;
	bool  IsAddMode;

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedAdd();
	virtual BOOL OnInitDialog();
	CButton m_AddButton;
	CButton m_SaveButton;
	CButton m_DeleteButton;
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedDelete();
};
