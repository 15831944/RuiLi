#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CAsmPerfPage 对话框

class CAsmPerfPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmPerfPage)

public:
	CAsmPerfPage();
	virtual ~CAsmPerfPage();

// 对话框数据
	enum { IDD = IDD_ASM_PERF_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ProSolid mdl;
	CString m_PerfParaName;
	CString m_PerfParaValue;
	CString m_PerfParaNote;
	int m_index;
	afx_msg void OnBnClicked1004();
	afx_msg void OnBnClicked1012();
	afx_msg void OnBnClicked1005();
	afx_msg void OnBnClicked1013();
	CListCtrl m_List;
	afx_msg void OnNMClick1002(NMHDR *pNMHDR, LRESULT *pResult);
	void SetList(void);
	virtual BOOL OnInitDialog();
	int m_iRadio;
	CEdit m_MaxValEdit;
	CEdit m_MinValEdit;
	BOOL CheckInput(int iEditItem);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
