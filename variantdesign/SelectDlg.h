#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSelectDlg �Ի���

class CSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectDlg)

public:
	CSelectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedOk();
    int s;
	int num;
	CString Name;
	CTreeCtrl m_parttree;
	CString   Type;
	afx_msg void OnNMClickTreePart(NMHDR *pNMHDR, LRESULT *pResult);
};
