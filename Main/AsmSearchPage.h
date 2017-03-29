#pragma once


// CAsmSearchPage 对话框

class CAsmSearchPage : public CDialog
{
	DECLARE_DYNAMIC(CAsmSearchPage)

public:
	CAsmSearchPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAsmSearchPage();

// 对话框数据
	enum { IDD = IDD_ASM_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
