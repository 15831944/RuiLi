#pragma once
#include "afxcmn.h"
#include "MyDatabase.h"
#include "FtpFile.h"
#include "UtilDimen.h"


// CAllFemMdlDlg 对话框

class CAllFemMdlDlg : public CDialog
{
	DECLARE_DYNAMIC(CAllFemMdlDlg)

public:
	CAllFemMdlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAllFemMdlDlg();

// 对话框数据
	enum { IDD = IDD_FEM_MDL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CUtilDimen util1,util2;
	ProSolid rootSolid;
	ProModelitem owner;
	CFtpFile ftpfile;
	int m_MdlNum;
	CMyDatabase m_db;
	void AddToList();
	int GetMdlNum();
public:
	afx_msg void OnBnClickedOk();
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CString m_Path;
	afx_msg void OnBnClickedButtonSelect();
	CString m_Number;
	afx_msg void OnBnClickedButtonRefresh();
};
