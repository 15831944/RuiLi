#pragma once


// CAsmInfoDefPage 对话框

class CAsmInfoDefPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmInfoDefPage)

public:
	CAsmInfoDefPage();
	virtual ~CAsmInfoDefPage();

// 对话框数据
	enum { IDD = IDD_ASM_INFODEF_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
