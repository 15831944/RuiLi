#pragma once
#include "afxcmn.h"


// CQuerySeries �Ի���

class CQuerySeries : public CDialog
{
	DECLARE_DYNAMIC(CQuerySeries)

public:
	CQuerySeries(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQuerySeries();

// �Ի�������
	enum { IDD = IDD_PRT_SERIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtnQuit();
	CListCtrl m_SeriesList;
	virtual BOOL OnInitDialog();
	void InitSeriesData(void);
	afx_msg void OnMove(int x, int y);
};
