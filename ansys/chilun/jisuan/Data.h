#pragma once
#include "msflexgrid1.h"
#include "Fr.h"

// CData 对话框

class CData : public CDialog
{
	DECLARE_DYNAMIC(CData)

public:
	CData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CData();

// 对话框数据
	enum { IDD = IDD_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMsflexgrid1 m_FlexGrid;
	void xiansh();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	CFr  m_pSet;
	void Set(CDatabase *);
};
