#pragma once
#include"picshow.h"
#include "afxwin.h"
#include "MyDatabase.h"
#include "myproe.h"
// CHandSpike1Dlg 对话框

class CHandSpike1Dlg : public CDialog
{
	DECLARE_DYNAMIC(CHandSpike1Dlg)

public:
	CHandSpike1Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHandSpike1Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_HANDSPIKE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double  GetDimValue(CString str,ProSolid solid);
	void ReplaceDim();
	CMyDatabase m_db;
	double m_l1;
	double m_l2;
	double m_l3;
	double m_l;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CPicShow m_handspike1;
	virtual BOOL OnInitDialog();
	CComboBox m_Combo;
	afx_msg void OnBnClickedButton3();
};
