#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ListCtrlEx.h"
#include "DetailInfoClass.h"

// CPdtSaveAsDlg 对话框

class CPdtSaveAsDlg : public CDialog
{
	DECLARE_DYNAMIC(CPdtSaveAsDlg)

public:
	CPdtSaveAsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPdtSaveAsDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PDTSAVEAS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CImageList m_ImageList;
	CTreeCtrl m_treeproduct;
	ProSolid ProductSolid;
	afx_msg void OnMove(int x, int y);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_inscode;
	CString m_insname;
	CString m_mdlnum;
	CString m_designer;
	CString m_note;
	CString m_place;
	CString m_drwingplace;
	CString m_filename;
	CComboBox m_designstate;

	DetailInfoClass *info;
	CListCtrlEx m_perf;
	BOOL IsOneOfProduct;
	BOOL BeUsed;
	CString m_group;
	
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSolidlocalplace();
	afx_msg void OnBnClickedButtonDrwlocalplace();
	afx_msg void OnBnClickedButtonSolidserverplace();
	afx_msg void OnBnClickedButtonDwgserverplace();
	afx_msg void OnBnClickedButtonGrpfrm();
	CString m_pdmcode;
	afx_msg void OnBnClickedButtonGrpfrm2();
};
