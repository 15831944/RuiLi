#pragma once
#include "afxcmn.h"


// CAsmSWTXPage �Ի���

class CAsmSWTXPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmSWTXPage)

public:
	CAsmSWTXPage();
	virtual ~CAsmSWTXPage();

// �Ի�������
	enum { IDD = IDD_ASM_SWTX_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
