#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DetailInfoClass.h"


// CPdtModifyDlg �Ի���

class CPdtModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CPdtModifyDlg)

public:
	CPdtModifyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPdtModifyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PTDMODIMY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_key;
	CComboBox m_combokey;
	afx_msg void OnBnClickedButtonQuery();
	BOOL GetPdtFromBase(CString sql,BOOL PDM);
	int m_workspace;
	CTreeCtrl m_treeprt;
	CListCtrl m_listprt;
	DetailInfoClass *info;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel2();
	virtual BOOL OnInitDialog();
	BOOL m_audit;                          //��������Ǹ�ѡ��ť
	afx_msg void OnBnClickedRadioWorkapace();
	afx_msg void OnBnClickedRadioPdm();
	afx_msg void OnNMClickTreePart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMove(int x, int y);
};
