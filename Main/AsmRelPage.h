#pragma once
#include "afxcmn.h"


// CAsmRelPage 对话框

class CAsmRelPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmRelPage)

public:
	CAsmRelPage();
	virtual ~CAsmRelPage();

// 对话框数据
	enum { IDD = IDD_ASM_REL_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ProSolid mdl;
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	int ShowList(void);
	int m_index;
};
