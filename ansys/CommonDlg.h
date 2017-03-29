#pragma once
#include "afxcmn.h"
#include "MyDatabase.h"
#include "myproe.h"
#include "afxwin.h"

//typedef struct ModelStruct
//{
//	int Number;
//	int FatherNum;
//	CString Name;
//	CString Type;
//}ModelStruct;

// CCommonDlg 对话框

class CCommonDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommonDlg)

public:
	CCommonDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCommonDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_COMMON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    CArray<ModelStruct,ModelStruct> ModelList;
	CImageList m_ImageList;
	CMyDatabase m_db;
	int m_ModelNum;
	CString m_IdNum;
	CString m_Table;
	void AddTree();
	void AddTree(HTREEITEM root,CArray<ModelStruct,ModelStruct> * List,int Fathernum);
	void AddTree(HTREEITEM root,int Number);
	void CCommonDlg::AddToDimList(int ModelNum);
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_Tree;
	CListCtrl m_List;
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListDim(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_Combo;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnClose();
};
