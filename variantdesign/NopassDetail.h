#pragma once
#include "afxcmn.h"


// CNopassDetail 对话框

class CNopassDetail : public CDialog
{
	DECLARE_DYNAMIC(CNopassDetail)

public:
	CNopassDetail(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNopassDetail();

// 对话框数据
	enum { IDD = IDD_PRT_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_DetailList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnquit();
	void InitListData(void);
	afx_msg void OnRepipei();
	float svalue;
	BOOL ExamTheDatas(void);
	bool Bfirst;
};
