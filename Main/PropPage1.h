#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "UsrSWTXTable.h"

// CPropPage1 �Ի���

class CPropPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPage1)

public:
	CPropPage1();
	virtual ~CPropPage1();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_LARGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	// �������Ա�
	CArray<UsrSWTXTable1,UsrSWTXTable1> * p_temp_UsrSWTXTableList;
	int SetValue(ProMdl solid, CArray<UsrSWTXTable1,UsrSWTXTable1>* pp_temp_UsrSWTXTableList);
	virtual BOOL OnInitDialog();
	int SetList(void);
	ProMdl mdl;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_Name;
	CString m_Value;
	CString m_Note;
	afx_msg void OnModefy();
	int m_index;
	CEdit m_ValueButton;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
};
