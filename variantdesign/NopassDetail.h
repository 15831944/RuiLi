#pragma once
#include "afxcmn.h"


// CNopassDetail �Ի���

class CNopassDetail : public CDialog
{
	DECLARE_DYNAMIC(CNopassDetail)

public:
	CNopassDetail(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNopassDetail();

// �Ի�������
	enum { IDD = IDD_PRT_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
