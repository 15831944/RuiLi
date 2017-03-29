#pragma once
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"

// CJointDlg �Ի���

class CJointDlg : public CDialog
{
	DECLARE_DYNCREATE(CJointDlg)

public:
	CJointDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CJointDlg();
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_DIALOG_JOINT, IDH = IDR_HTML_JOINTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	double m_F;
	double m_t;
	double m_l;
	double m_L;
	double m_K;
	double m_G1;
	double m_G2;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton5();
};
