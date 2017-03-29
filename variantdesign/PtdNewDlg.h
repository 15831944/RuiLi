#pragma once
#include "DetailInfoClass.h"


// CPtdNewDlg 对话框

class CPtdNewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPtdNewDlg)

public:
	CPtdNewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPtdNewDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PDTNEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPtdmdl();
	CString m_oldsolidname;
	CString m_olddrwname;
	CString m_inscode;
	CString m_mdlnum;
	CString m_insname;
	CString m_designer;
	CString m_group;
	CString m_note;
	CString m_place;
	BOOL FromModel;
	CString m_filename;
	DetailInfoClass *detailinfo;
	BOOL IsError;
	//afx_msg void OnBnClickedButtonSel();
	afx_msg void OnBnClickedButtonGrpfrm();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSelprt();

	BOOL GetAsmModelInfo(CString MdlNum);
	BOOL GetProductInsInfo(CString InsCode);
	
	afx_msg void OnMove(int x, int y);
	CString m_pdmcode;
	afx_msg void OnBnClickedButtonGrpfrm2();
};
