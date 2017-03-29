#pragma once
#include "afxcmn.h"


// CAsmRelVistDlg 对话框

class CAsmRelVistDlg : public CDialog
{
	DECLARE_DYNAMIC(CAsmRelVistDlg)

public:
	CAsmRelVistDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAsmRelVistDlg();

// 对话框数据
	enum { IDD = IDD_ASM_REL_VISIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	void VistRel(ProSolid mdl);
	ProSolid solid;
	afx_msg void OnBnClickedOk();
	CStringArray * relSet;
	CStringArray * relNote;
};
