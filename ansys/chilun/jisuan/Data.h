#pragma once
#include "msflexgrid1.h"
#include "Fr.h"

// CData �Ի���

class CData : public CDialog
{
	DECLARE_DYNAMIC(CData)

public:
	CData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CData();

// �Ի�������
	enum { IDD = IDD_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CMsflexgrid1 m_FlexGrid;
	void xiansh();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	CFr  m_pSet;
	void Set(CDatabase *);
};
