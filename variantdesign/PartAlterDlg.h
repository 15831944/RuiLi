#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ListCtrlEx.h"


// CPartAlterDlg �Ի���

class CPartAlterDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartAlterDlg)

public:
	CPartAlterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPartAlterDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PARTALTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonApply();
	afx_msg LRESULT OnLeachIns(WPARAM wparam,LPARAM lparam);
	afx_msg void OnNMClickListDimen(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL GetPartStandard(CListCtrl *m_list,CString MdlNum);       //��ʾ���������
	void ToggleSize();
private:
	BOOL m_btogglesize;
	CRect m_rectfull;
	CRect m_recthalf;
	CListCtrlEx	 m_dimension;
	BOOL  IsKong;
	BOOL First;
public:
	
	CButton m_button;
	CString partnum;
	CListCtrl m_standard;
	afx_msg void OnNMDblclkListStandard(NMHDR *pNMHDR, LRESULT *pResult);
};
