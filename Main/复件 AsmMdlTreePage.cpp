// AsmMdlTreePage.cpp : 实现文件
//

#include "stdafx.h"
#include "ASM.h"
#include "AsmMdlTreePage.h"


// CAsmMdlTreePage 对话框

IMPLEMENT_DYNAMIC(CAsmMdlTreePage, CPropertyPage)
CAsmMdlTreePage::CAsmMdlTreePage()
	: CPropertyPage(CAsmMdlTreePage::IDD)
{
}

CAsmMdlTreePage::~CAsmMdlTreePage()
{
}

void CAsmMdlTreePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAsmMdlTreePage, CPropertyPage)
END_MESSAGE_MAP()


// CAsmMdlTreePage 消息处理程序
