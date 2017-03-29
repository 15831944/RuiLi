#pragma once
#include "FtpDirTreeCtrl.h"
#include "MyDatabase.h"
#include "afxwin.h"


// CFemSave �Ի���

class CFemSave : public CDialog
{
	DECLARE_DYNAMIC(CFemSave)

public:
	CFemSave(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFemSave();

// �Ի�������
	enum { IDD = IDD_FEM_SAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
