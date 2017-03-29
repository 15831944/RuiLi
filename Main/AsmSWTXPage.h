#pragma once
#include "afxcmn.h"


// CAsmSWTXPage 对话框

class CAsmSWTXPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmSWTXPage)

public:
	CAsmSWTXPage();
	virtual ~CAsmSWTXPage();

// 对话框数据
	enum { IDD = IDD_ASM_SWTX_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ProSolid mdl;
	CListCtrl m_List;
	int SetList(void);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClick1002(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_Name;
	CString m_Value;
	CString m_Note;
	int m_selListData;
};
