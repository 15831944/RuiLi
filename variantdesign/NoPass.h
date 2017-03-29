#pragma once


// CNoPass 对话框

class CNoPass : public CDialog
{
	DECLARE_DYNAMIC(CNoPass)

public:
	CNoPass(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNoPass();

// 对话框数据
	enum { IDD = IDD_NOPASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnNopassYes();
	afx_msg void OnBnClickedNopassNo();
	afx_msg void OnNopassDetail();
};
