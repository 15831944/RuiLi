#pragma once
#include "afxcmn.h"


// CRelToMdl 对话框

class CRelToMdl : public CDialog
{
	DECLARE_DYNAMIC(CRelToMdl)

public:
	CRelToMdl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRelToMdl();

// 对话框数据
	enum { IDD = IDD_REL_TO_MDL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	//CArray<UsrRelTable,UsrRelTable> pp_UsrRelTable;
	int SetRel(ProMdl solid/*,CArray<UsrRelTable,UsrRelTable> * p_temp_UsrRelTalbe*/);
	int SetRel(ProMdl solid,CArray<UsrRelTable,UsrRelTable> * p_temp_UsrRelTalbe);
	CListCtrl m_List;
	int ShowRel(void);
	ProMdl mdl;
	int AddRelToMdl(ProMdl solid, CString rel);
};
