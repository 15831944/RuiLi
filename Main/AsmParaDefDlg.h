#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "AsmSWTXDlg.h"
#include "AsmRelDlg.h"

// CAsmParaDefDlg 对话框

class CAsmParaDefDlg : public CDialog
{
	DECLARE_DYNAMIC(CAsmParaDefDlg)

public:
	CAsmParaDefDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAsmParaDefDlg();

// 对话框数据
	enum { IDD = IDD_ASM1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree;
	void ShowModelTree(ProSolid solid,HTREEITEM hItem);
	virtual BOOL OnInitDialog();
	ProSolid asm_solid;
	CImageList imageList;
	CListCtrl m_List;
	CArray<ProFeature,ProFeature> asm_feat_list;
	CArray<ProDimension,ProDimension> Dimensionlist;
	CArray<ProParameter,ProParameter> Parameterlist;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void ConverthItemToList(HTREEITEM hItem);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CButton m_SWTXButton;
	CButton m_RELButton;
	CAsmSWTXDlg swtx_dlg;
	CAsmRelDlg  rel_dlg;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	int m_RadioFlag;
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	void ShowParaList();
	CEdit m_PathEdit;
	afx_msg void OnBnClickedAsmParadefButton4();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnInsertToRel();
	//CButton m_RadioButton;
	afx_msg void OnEditDim();
	int FreshListByDim(void);
	afx_msg void OnNMClickAsmParadefList(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_DimDisplayModeCheckbox;
	CButton m_FeatDisplayModeCheckbox;
	HTREEITEM current_hItem;
	ProSelection  current_selection;
	int HighLightFeat(HTREEITEM hItem);
	afx_msg void OnClose();
	int m_RadioButtonValue;
};
