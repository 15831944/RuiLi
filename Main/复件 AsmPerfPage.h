#pragma once


// CAsmPerfPage 对话框

class CAsmPerfPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmPerfPage)

public:
	CAsmPerfPage();
	virtual ~CAsmPerfPage();

// 对话框数据
	enum { IDD = IDD_ASM_PERF_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
