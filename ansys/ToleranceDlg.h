#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Tolerance.h"


// CToleranceDlg �Ի���

class CToleranceDlg : public CDialog
{
	DECLARE_DYNAMIC(CToleranceDlg)

public:
	CToleranceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CToleranceDlg();

// �Ի�������
	enum { IDD = IDD_TOLERANCE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTolerance m_Tol;
	int m_Cols;
	int m_Rows;
    int m_PageIndex;
	int m_StartCol,m_EndCol;
	ExcelPage m_OutPage;
	
     
public:
	int LoadAll();
	int LoadPage(int pageIndex);
	int LoadList(int pageIndex,int startcol,int endcol);
	int GetList();
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	CComboBox m_SmallDia;
	CComboBox m_BigDia;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnBnClickedOk();
	CComboBox m_NameCombo;
	afx_msg void OnCbnSelchangeCombo2();
};
