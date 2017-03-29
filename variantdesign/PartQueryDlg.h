#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "listctrlex.h"


// CPartQueryDlg �Ի���

class CPartQueryDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartQueryDlg)

public:
	CPartQueryDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPartQueryDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnMove(int x, int y);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnNMClickTreeModelgroup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListPart(NMHDR *pNMHDR, LRESULT *pResult);
	//void SearchFromResult(CStringArray *stringarray);
public:
	CString InsCode;
	BOOL IsSelect;
	int num;	  //ģ�ͱ��
	CComboBox m_dimenname;
	CString m_dimenvalue;
	CListCtrl m_listpart;
	CTreeCtrl m_modeltree;
	CString m_performance;
	CComboBox m_comboperf;
	CButton m_check;
	BOOL m_isselect;
	CString TypeName;
	int picNum;
	afx_msg void OnPaint();
	afx_msg void OnNMRclickListPart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPreview2d();
	afx_msg void OnPreview3d();
};
