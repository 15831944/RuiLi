#pragma once
#include "Resource.h"

// CLabelDlg �Ի���

class CLabelDlg : public CDialog
{
	DECLARE_DYNAMIC(CLabelDlg)

public:
	CLabelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLabelDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DIM_LABEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strEdit;
	afx_msg void OnBnClickedOk();
	CString m_oldname;
	virtual BOOL OnInitDialog();
	CString m_Note;
	afx_msg void OnMove(int x, int y);
};
