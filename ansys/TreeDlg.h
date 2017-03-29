#pragma once
#include "afxcmn.h"
#include"myproe.h"

// CTreeDlg �Ի���

class CTreeDlg : public CDialog
{
	DECLARE_DYNAMIC(CTreeDlg)

public:
	CTreeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTreeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TREE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int OpenModel();
	ProSolid solid;
};
