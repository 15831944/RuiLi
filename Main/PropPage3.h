#pragma once
#include "afxcmn.h"


// CPropPage3 对话框

class CPropPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPage3)

public:
	CPropPage3();
	virtual ~CPropPage3();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_LARGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	// 关系
	/*CArray<UsrRelTable,UsrRelTable> * p_temp_UsrRelTableList;*/
	int SetValue(ProMdl solid/*,CArray<UsrRelTable,UsrRelTable>* pp_temp_UsrRelTableList*/);
	ProMdl mdl;
	virtual BOOL OnInitDialog();
	int SetList(void);
};
