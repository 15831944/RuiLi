#pragma once
#include "afxcmn.h"


// CUpdateDlg �Ի���

class CUpdateDlg : public CDialog
{
	DECLARE_DYNAMIC(CUpdateDlg)

public:
	CUpdateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUpdateDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_UPDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_select;
	CString Type;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_inscode;
	CString m_insname;
	CString m_mdlnum;
	CString m_designer;
	CString m_note;
	CString m_group;
	//CString m_material;
	//CString m_unit;
	CString m_place;
	CString m_filename;
	CString m_drwingplace;
	CString m_drwname;
	int s;
	afx_msg void OnBnClickedCancel();
	CString m_auditor;
	afx_msg void OnBnClickedButtonNo();
	
};
