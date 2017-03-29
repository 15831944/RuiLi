#pragma once
#include "MyDatabase.h"
#include "afxwin.h"
#include "FtpFile.h"
#include "myproe.h"
#include "afxcmn.h"

// CFemOpenDlg 对话框

class CFemOpenDlg : public CDialog
{
	DECLARE_DYNAMIC(CFemOpenDlg)

public:
	CFemOpenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFemOpenDlg();

// 对话框数据
	enum { IDD = IDD_FEM_OPEN };

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
	int  ShowNewWindow(ProMdl * pSolid);
	int LoadModel(CString path,ProMdl * pSolid);
	afx_msg void OnBnClickedOk();
	CListCtrl m_List;
	CTreeCtrl m_Tree;
	afx_msg void OnNMClickTreeOpen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CString m_Path;
	afx_msg void OnBnClickedButtonSelect();
};
