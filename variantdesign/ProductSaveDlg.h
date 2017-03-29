#pragma once
#include "afxcmn.h"
#include "ProductMenberBasicInfoDlg.h"
#include "ListCtrlEx.h"
#include "afxwin.h"
#include "DetailInfoClass.h"


// CProductSaveDlg 对话框

class CProductSaveDlg : public CDialog
{
	DECLARE_DYNAMIC(CProductSaveDlg)

public:
	CProductSaveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProductSaveDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PRODUCTSAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_asmmodeltree;
	CProductMenberBasicInfoDlg productmenberdlg;
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnNMClickTreeAsmmodel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSave();

	BOOL IsAsmCanSave(ProSolid solid,int i,CListCtrl *list);
	int variantnum;
	ProMdl m_menmdl;
	ProSolid ProductSolid;
	CString ProductNum;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	CString m_inscode;
	CString m_insname;
	CString m_designer;
	CString m_mdlnum;
	CString m_group;
	CString m_note;
	CString m_place;
	CString m_drwingplace;
	CString m_filename;
	CComboBox m_designstate;
	CListCtrlEx m_perf;
	BOOL BeUsed;
	BOOL IsOneOfProduct;
	CImageList m_ImageList;
	afx_msg void OnBnClickedButtonGrpfrm();
	afx_msg void OnBnClickedButtonSolidlocalplace();
	afx_msg void OnBnClickedButtonDrwlocalplace();
	afx_msg void OnBnClickedButtonSolidserverplace();
	afx_msg void OnBnClickedButtonDwgserverplace();
	//BOOL IsPartUsedByProduct(int s,BOOL *IsUsed);
	//BOOL IsUsedByTheSameProduct(CStringArray *PdtArray,BOOL *Yes);
	CString m_pdmcode;
	afx_msg void OnBnClickedButtonde();
};
