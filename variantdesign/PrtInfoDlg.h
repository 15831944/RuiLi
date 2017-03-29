#pragma once
#include "listctrlex.h"


// CPrtInfoDlg �Ի���

class CPrtInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CPrtInfoDlg)

public:
	CPrtInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPrtInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PRTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int s;
	CString m_inscode;
	CString m_insname;
	CString m_mdlnum;
	CString m_designer;
	CString m_group;
	CString m_material;
	CString m_unit;
	CString m_note;
	CString m_place;
	CString m_drwingplace;
	CString m_filename;
	CString m_drwname;
	virtual BOOL OnInitDialog();
	CListCtrlEx m_perf;
	afx_msg void OnBnClickedOk();
	CString m_audit;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonGrpfrm2();
	afx_msg void OnBnClickedButtonGrpfrm3();
	afx_msg void OnBnClickedButtonde();
};
