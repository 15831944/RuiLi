#pragma once
#include "MyDatabase.h"
#include <afxtempl.h>
#include "FtpFile.h"
#include "myproe.h"



// CSaveModelDlg 对话框

class CSaveModelDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaveModelDlg)

public:
	CSaveModelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSaveModelDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SAVE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	 CFtpFile ftpfile;
	CArray<ModelStruct,ModelStruct> ModelList;
	CMyDatabase m_db;
	HTREEITEM m_hitem;
	CImageList m_ImageList;
	int m_FatherNum;
	void AddTree();
	void AddTree(HTREEITEM root,CArray<ModelStruct,ModelStruct> * List,int Fathernum);

public:
	CTreeCtrl m_Tree;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRclickTreeSave(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnNewcat();
	afx_msg void OnDelcat();
	afx_msg void OnRefresh();
	afx_msg void OnClose();
	afx_msg void OnNMClickTreeSave(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_Name;
	CString m_Path;
	CString m_Note;
};
