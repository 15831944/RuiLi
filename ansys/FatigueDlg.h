#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"

// CFatigueDlg �Ի���

class CFatigueDlg : public CDialog
{
	DECLARE_DYNAMIC(CFatigueDlg)

public:
	CFatigueDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFatigueDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_fatigue };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;

	double m_d1;
	double m_d2;
	double m_t;
	double m_F1;
	double m_F2;
	double m_t1;
	double m_t2;
	
	double m_S;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonCancel1();
	afx_msg void OnBnClickedButtonCancel2();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton4();
	virtual BOOL OnInitDialog();
};
