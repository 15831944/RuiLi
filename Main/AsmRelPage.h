#pragma once
#include "afxcmn.h"


// CAsmRelPage �Ի���

class CAsmRelPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmRelPage)

public:
	CAsmRelPage();
	virtual ~CAsmRelPage();

// �Ի�������
	enum { IDD = IDD_ASM_REL_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	ProSolid mdl;
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	int ShowList(void);
	int m_index;
};
