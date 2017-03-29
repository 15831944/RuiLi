#pragma once
#include "DetailInfoClass.h"


// CDownLoadDlg �Ի���

class CDownLoadDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownLoadDlg)

public:
	CDownLoadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownLoadDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL    IsPart;
	int s;
	CString InsCode;
	CString m_oldsolidname;
	CString m_olddrwname;
	CString m_solidplace;
	BOOL    IsCheck;
	CString m_solidname;
	DetailInfoClass *info;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonDrawplace();
	afx_msg void OnBnClickedButtonSolidplace();
	//BOOL OpenFile(CString SolidName,CString SolidPath,CString DrwName,CString DrwPath);
	afx_msg void OnEnChangeEditSolidname1();
	afx_msg void OnMove(int x, int y);
};
