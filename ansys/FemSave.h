#pragma once
#include "FtpDirTreeCtrl.h"
#include "MyDatabase.h"
#include "afxwin.h"


// CFemSave 对话框

class CFemSave : public CDialog
{
	DECLARE_DYNAMIC(CFemSave)

public:
	CFemSave(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFemSave();

// 对话框数据
	enum { IDD = IDD_FEM_SAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HTREEITEM m_hitem;
	CMyDatabase m_db;
   CFtpDirTreeCtrl m_Tree;
   void Refresh();
   bool CopyFilesToFtpServer(long MdlNumber, CString localDir,CString serDir);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
//	afx_msg void OnNMDblclkTreeSave(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
	CString CName;
	CString CType;
	CString CConnprtMdlNum;
	CString CFatherNum;
	CString CDateTime;
	CString CNote;
	afx_msg void OnNMDblclkTreeSave(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_Path;
	afx_msg void OnNMRclickTreeSave(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNewcat();
	afx_msg void OnDelcat();
	afx_msg void OnRenamefile();
};
