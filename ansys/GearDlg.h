#pragma once


// CGearDlg 对话框

class CGearDlg : public CDialog
{
	DECLARE_DYNAMIC(CGearDlg)

public:
	CGearDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGearDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GEAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_z;
	double m_m;
	double m_a;
	double m_x;
	double m_d;
	double m_ha;
	double m_h;
	double m_Fr;
	double m_Fw;
	
	double m_Wk;
	double m_ffa;
	double m_fha;
	double m_k;
	double m_fpt;
	double m_Fpt;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit9();
	double m_hh;
};
