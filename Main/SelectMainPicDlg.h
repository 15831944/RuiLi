#pragma once
#include "afxcmn.h"


// CSelectMainPicDlg �Ի���

class CSelectMainPicDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectMainPicDlg)

public:
	CSelectMainPicDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectMainPicDlg();

// �Ի�������
	enum { IDD = IDD_SELECT_PIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
	ProMdl mdl;
	int SetMdl(ProMdl solid);
	CString select_file;
};
