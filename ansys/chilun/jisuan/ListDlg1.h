#pragma once
#pragma once
#include "ComboGridCtrl.h"
#include "fpt.h"
#include "afxwin.h"

// CListDlg1 对话框

class CListDlg1 : public CDialog
{
	DECLARE_DYNAMIC(CListDlg1)

public:
	CListDlg1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListDlg1();
	CComboGridCtrl m_GridCtrl;
	void InitGridCtrl();
	void SetDB(CDatabase *);
	Cfpt m_Set;
	bool  IsAddMode;
	

		// 对话框数据
		enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAdd();
	CButton m_SaveButton;
	CButton m_AddButton;
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedDelete();
};


	
	
