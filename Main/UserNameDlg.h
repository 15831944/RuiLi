#pragma once
#include "afxcmn.h"


// CUserNameDlg �Ի���

class CUserNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserNameDlg)

public:
	CUserNameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserNameDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_USERNAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_UserName;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_Text;
	CListCtrl m_List;
	afx_msg void OnNMClickListProduct(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListProduct(NMHDR *pNMHDR, LRESULT *pResult);
};
