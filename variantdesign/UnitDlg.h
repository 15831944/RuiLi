#pragma once
#include "afxcmn.h"


// CUnitDlg �Ի���

class CUnitDlg : public CDialog
{
	DECLARE_DYNAMIC(CUnitDlg)

public:
	CUnitDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUnitDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_UNIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_ID;
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMClickListProduct(NMHDR *pNMHDR, LRESULT *pResult);
};
