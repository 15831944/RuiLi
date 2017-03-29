#pragma once
#include "picshow.h"
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"



// CHandSpikeDlg �Ի���

class CHandSpikeDlg : public CDialog
{
	DECLARE_DYNAMIC(CHandSpikeDlg)

public:
	CHandSpikeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHandSpikeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_HANDSPIKE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	CString m_l1;
	CString m_l2;
	CString m_l3;
	CString m_l;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	//CPicShow m_handspike;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	CComboBox m_Combo;
};
