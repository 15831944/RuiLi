#pragma once
#include "afxcmn.h"


// CIntDlg �Ի���

class CIntDlg : public CDialog
{
	DECLARE_DYNAMIC(CIntDlg)

public:
	CIntDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIntDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_INTEGRATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_code;
	int m_part;
	CString Dcode;
	afx_msg void OnBnClickedButtonLook();
	CListCtrl m_pdmlist;
	CListCtrl m_vdslist;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioPart();
	afx_msg void OnBnClickedRadioAsm();
	afx_msg void OnNMClickListPdm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListVds(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListVds(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonApply();
	BOOL Integrate();
	CString m_vdscode;
	afx_msg void OnBnClickedButtonOut();
	BOOL Search(BOOL First);
	BOOL IsDocumentNameTheSave(CString PDMCode,int k,BOOL IsPrt);
	afx_msg void OnMove(int x, int y);
};
