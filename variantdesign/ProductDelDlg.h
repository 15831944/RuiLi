#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CProductDelDlg �Ի���

class CProductDelDlg : public CDialog
{
	DECLARE_DYNAMIC(CProductDelDlg)

public:
	CProductDelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProductDelDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PRODUCTDEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_product;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	BOOL DeletePrtIns(int SolidID);
	BOOL DeleteProductIns(int SolidID,BOOL IsTheTop);
	BOOL QueryDatabase(CString sql,CString Type);
	BOOL IsUsedByProduct(int SolidID,BOOL *BeUsed,CString Type);
	CString m_inscode;
	CComboBox m_type;
	afx_msg void OnBnClickedButtonQuery();
	int m_all;
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnMove(int x, int y);
};
