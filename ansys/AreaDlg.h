#pragma once
#include "picshow.h"
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"


// CAreaDlg �Ի���

class CAreaDlg : public CDialog
{
	DECLARE_DYNAMIC(CAreaDlg)

public:
	CAreaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAreaDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_AREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	double m_diam1;
	double m_diam2;
	double m_s;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CPicShow m_pic;
	virtual BOOL OnInitDialog();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton3();
};
