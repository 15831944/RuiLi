#pragma once


// CStability0Dlg 对话框

class CStability0Dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CStability0Dlg)

public:
	CStability0Dlg();
	virtual ~CStability0Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_STABILITY0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
