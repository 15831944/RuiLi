#pragma once
#include "afxdlgs.h"
#include "afxcmn.h"
#include "ListCtrlEx.h"
#include "afxwin.h"


// CCheckDlg �Ի���

class CCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckDlg)

public:
	CCheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCheckDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	CListCtrl    m_dimension;
	CString partnum;
	int s;
	afx_msg void OnNMClickListDim(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonOther();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonNo();
	BOOL CheckPart(int s);
};
