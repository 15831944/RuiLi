#pragma once
#include "afxwin.h"


// CNewDlg �Ի���

class CNewDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewDlg)

public:
	CNewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewDlg();

// �Ի�������
	enum { IDD = IDD_NEWMDL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Name;
	int m_Template;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	bool mynewfileprt(char  filename[]);
	
	//CButton m_radiobutton;
	virtual BOOL OnInitDialog();
	int m_radioFlag;
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();
	bool mynewfileasm(char  filename[]);
	bool mynewfiledwg(char  filename[]);
	CButton m_RadioDrawing;
	CString m_FileName;
	afx_msg void OnMove(int x, int y);
};
