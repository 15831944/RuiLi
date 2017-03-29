#pragma once
#include "afxcmn.h"


// CQuerySeries 对话框

class CQuerySeries : public CDialog
{
	DECLARE_DYNAMIC(CQuerySeries)

public:
	CQuerySeries(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQuerySeries();

// 对话框数据
	enum { IDD = IDD_PRT_SERIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtnQuit();
	CListCtrl m_SeriesList;
	virtual BOOL OnInitDialog();
	void InitSeriesData(void);
	afx_msg void OnMove(int x, int y);
};
