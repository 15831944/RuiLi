#pragma once


// AddToPDMDlg 对话框

class AddToPDMDlg : public CDialog
{
	DECLARE_DYNAMIC(AddToPDMDlg)

public:
	AddToPDMDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AddToPDMDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDTOPDM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPar();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedOk();
};
