#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DetailInfoClass.h"


// CPrtModifyDlg �Ի���

class CPrtModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CPrtModifyDlg)

public:
	CPrtModifyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPrtModifyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PRTMODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_key;
	CComboBox m_combokey;
	afx_msg void OnBnClickedButtonQuery();
	DetailInfoClass *info;
	CTreeCtrl m_treeprt;
	CListCtrl m_listprt;
	virtual BOOL OnInitDialog();
	BOOL GetPrtFromBase(CString sql,BOOL PDM);
	afx_msg void OnBnClickedOk();
	int m_workspace;
	CString InsCode;
	int TypeNum;
	afx_msg void OnBnClickedRadioPdm();
	afx_msg void OnBnClickedRadioWorkapace();
	afx_msg void OnNMClickTreePart(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL ObtianPartInfByType(int num);
	afx_msg void OnMove(int x, int y);
	BOOL m_audit;
	BOOL OpenPrtFileFromSever();
};
