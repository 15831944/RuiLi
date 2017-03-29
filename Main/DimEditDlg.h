#pragma once


// CDimEditDlg 对话框

class CDimEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CDimEditDlg)

public:
	CDimEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDimEditDlg();

// 对话框数据
	enum { IDD = IDD_DIMEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_ID;
	CString m_Symbol;
	double m_Value;
	float m_UpLimit;
	double m_DwLimit;
	afx_msg void OnBnClickedOk();
};
