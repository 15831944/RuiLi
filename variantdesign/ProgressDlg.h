#pragma once
#include "afxcmn.h"


// CProgressDlg �Ի���

class CProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProgressDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
		int m_Pos;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	CProgressCtrl m_Progress;
	afx_msg void OnTimer(UINT nIDEvent);
	CString m_Label;
};
