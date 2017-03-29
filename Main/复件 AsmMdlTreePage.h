#pragma once


// CAsmMdlTreePage 对话框

class CAsmMdlTreePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmMdlTreePage)

public:
	CAsmMdlTreePage();
	virtual ~CAsmMdlTreePage();

// 对话框数据
	enum { IDD = IDD_ASM_MDL_TREE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
