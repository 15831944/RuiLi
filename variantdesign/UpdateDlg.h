#pragma once
#include "afxcmn.h"


// CUpdateDlg 对话框

class CUpdateDlg : public CDialog
{
	DECLARE_DYNAMIC(CUpdateDlg)

public:
	CUpdateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpdateDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_UPDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
