#pragma once
#include "afxcmn.h"


// CMdlRelVisitDlg 对话框

class CMdlRelVisitDlg : public CDialog
{
	DECLARE_DYNAMIC(CMdlRelVisitDlg)

public:
	CMdlRelVisitDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMdlRelVisitDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    
	DECLARE_MESSAGE_MAP()
public:
	bool GetMdlRel(void);
	void ShowMdlRel(void);
	ProMdl mdl;
	void SetMdl(ProMdl owner);
	virtual BOOL OnInitDialog();
	//CArray<ProRelset,ProRelset> p_ProRelSetList;
private:
	CArray<ProRelset,ProRelset> p_ProRelSetList;
	CStringArray rel_stringList;
	
public:
	CListCtrl m_List;
	afx_msg void OnBnClickedOk();
	CStringArray rel_stringList_select;
	
};
