#pragma once


// CAsmPicPage 对话框

class CAsmPicPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmPicPage)

public:
	CAsmPicPage();
	virtual ~CAsmPicPage();

// 对话框数据
	enum { IDD = IDD_ASM_PIC_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
