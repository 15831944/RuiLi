// AsmPicPage.cpp : 实现文件
//

#include "stdafx.h"
#include "ASM.h"
#include "AsmPicPage.h"


// CAsmPicPage 对话框

IMPLEMENT_DYNAMIC(CAsmPicPage, CPropertyPage)
CAsmPicPage::CAsmPicPage()
	: CPropertyPage(CAsmPicPage::IDD)
{
}

CAsmPicPage::~CAsmPicPage()
{
}

void CAsmPicPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAsmPicPage, CPropertyPage)
END_MESSAGE_MAP()


// CAsmPicPage 消息处理程序
