#pragma once
#include "afxcmn.h"


// CPartSWTXDlg �Ի���

class CPartSWTXDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartSWTXDlg)

public:
	CPartSWTXDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPartSWTXDlg();

// �Ի�������
	enum { IDD = IDD_PART_SWTX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	ProSolid  mdl;
	virtual BOOL OnInitDialog();
	void ShowSWTXList(ProSolid solid);
	afx_msg void OnDel();
	afx_msg void OnEdit();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnFresh();
	afx_msg void OnNMClickAsmSwtxList(NMHDR *pNMHDR, LRESULT *pResult);
};
