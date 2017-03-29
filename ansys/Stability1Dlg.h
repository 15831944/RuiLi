#pragma once


// CStability1Dlg 对话框

class CStability1Dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CStability1Dlg)

public:
	CStability1Dlg();
	virtual ~CStability1Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_STABILITY1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
