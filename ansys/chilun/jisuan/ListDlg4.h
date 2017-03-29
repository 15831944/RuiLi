#pragma once
#include "afxwin.h"
#include "ComboGridCtrl.h"
#include "fha.h"
#include "afxwin.h"


// CListDlg4 �Ի���

class CListDlg4 : public CDialog
{
	DECLARE_DYNAMIC(CListDlg4)

public:
	CListDlg4(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CListDlg4();
	CComboGridCtrl m_GridCtrl;
	void InitGridCtrl();
	void SetDB(CDatabase *);
	Cfha m_hSet;
	bool  IsAddMode;

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
	CButton m_AddButton;
	CButton m_SaveButton;
	CButton m_DeleteButton;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedDelete();
};





