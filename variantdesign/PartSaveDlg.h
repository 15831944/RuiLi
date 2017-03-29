#pragma once
#include "afxcmn.h"
#include "ProSolid.h"
#include "ListCtrlEx.h"
#include "afxwin.h"
#include "DetailInfoClass.h"

// CPartSaveDlg 对话框

class CPartSaveDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartSaveDlg)

public:
	CPartSaveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPartSaveDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PARTSAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int variantnum;
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedOk();
	BOOL BeUsed;
	CString LocalPath;
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
	CComboBox m_designstate;
	CListCtrlEx m_perf;
	DetailInfoClass *info;
	ProSolid solid;
	ProSolid ProductSolid;
	BOOL IsOneOfProduct;
	afx_msg void OnBnClickedButtonGrpfrm();
	afx_msg void OnBnClickedButtonSolidlocal();
	afx_msg void OnBnClickedButtonSolidserver();
	afx_msg void OnBnClickedButtonDrwlocal();
	afx_msg void OnBnClickedButtonDrwserver();

	CString m_pdmcode;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton9();
};
