#pragma once
#include "afxcmn.h"


// CMaterialDlg �Ի���

class CMaterialDlg : public CDialog
{
	DECLARE_DYNAMIC(CMaterialDlg)

public:
	CMaterialDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMaterialDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MATERIAL };

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
	CString m_Text;
	afx_msg void OnNMDblclkListProduct(NMHDR *pNMHDR, LRESULT *pResult);
};
