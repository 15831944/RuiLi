#pragma once
#include "afxcmn.h"
#include "FtpDirTreeCtrl.h"


// CStyleStandDlg �Ի���

class CStyleStandDlg : public CDialog
{
	DECLARE_DYNAMIC(CStyleStandDlg)

public:
	CStyleStandDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStyleStandDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CFtpDirTreeCtrl m_FtpTree;
    CString path;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnNMClickTreeStylestand(NMHDR *pNMHDR, LRESULT *pResult);
	
};
