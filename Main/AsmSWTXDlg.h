#pragma once
#include "afxcmn.h"


// CAsmSWTXDlg �Ի���

class CAsmSWTXDlg : public CDialog
{
	DECLARE_DYNAMIC(CAsmSWTXDlg)

public:
	CAsmSWTXDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAsmSWTXDlg();

// �Ի�������
	enum { IDD = IDD_ASM_SWTX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	void ShowSWTXList(ProSolid solid);
	virtual BOOL OnInitDialog();
	afx_msg void OnDel();
	ProSolid mdl;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnEdit();
	afx_msg void OnRefresh();
	afx_msg void OnNMClickAsmSwtxList(NMHDR *pNMHDR, LRESULT *pResult);
};
