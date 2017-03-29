#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CAsmParaDefGlg 对话框

class CAsmParaDefDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CAsmParaDefDlg2)

public:
	CAsmParaDefDlg2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAsmParaDefDlg2();

// 对话框数据
	enum { IDD = IDD_ASM_DEF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    typedef struct tree_item {
		int        type;          //-1(顶层),0(组件) 1(零件) 2(事物特性表) 3(装配约束)
		ProSolid   p_solid;       //元件的句柄
		ProSolid   f_solid;       //上层句柄
		ProFeature feature;       //元件的作为特征的句柄
    };
	CArray<tree_item,tree_item&> p_treeitemlist;       //元件信息列表的列表
	typedef struct list_item{
		int          type;  //TYPE_DIM TYPE_PARA
		CStringA     name;
		CStringA     note;
		ProParameter Para;
		ProDimension Dim;
	};
	CArray<list_item,list_item&> p_listitem;
	CArray<list_item,list_item&> p_listitem2;
	int   m_flag;   //状态标志，0 表示点中事务特性表 1 表示点中装配尺寸 -1没有
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree;
	CListCtrl m_List1;
	CListCtrl m_List2;
	CImageList m_ImageList;
	CStatic m_Path;
	ProSolid asm_solid;
	virtual BOOL OnInitDialog();
	void GetAsmcompConstraint(HTREEITEM hItem,ProAsmcomp * p_component);
	void ShowModelTree(ProSolid solid,HTREEITEM hItem);
	bool IsSolidFromMdl(ProSolid solid);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	//在列表里面显示模型的事务特性表
	void ShowSolidSWTX(ProSolid solid);
	void ShowPartSWTX(ProSolid part);
	//void ShowAsmSWTX(ProSolid asm,CListCtrl & p_List);
	void ShowAsmSWTXList1(ProSolid ,CListCtrl &);
	void ShowAsmSWTXList2(ProSolid ,CListCtrl &);
	//在列表里面显示特征的尺寸
	CArray<ProDimension,ProDimension> Dimensionlist;//特征尺寸列表
	void ShowFeatDim(ProFeature feature);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnEditDim();
	//显示模型顶层的参数
	void ShowAsmSolidPara(ProSolid asm1);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFreshList();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnMove(int x, int y);
};
