#pragma once
#include "afxcmn.h"


// CListDlg �Ի���

class CListDlg : public CDialog
{
	DECLARE_DYNAMIC(CListDlg)

public:
	CListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CListDlg(CWnd* pParent /*=NULL*/,CString title);
	virtual ~CListDlg();

// �Ի�������
	enum { IDD = IDD_LIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	// �Ի���ı���
	CString m_strText;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
