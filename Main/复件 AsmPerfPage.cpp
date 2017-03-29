// AsmPerfPage.cpp : 实现文件
//

#include "stdafx.h"
#include "ASM.h"
#include "AsmPerfPage.h"


// CAsmPerfPage 对话框

IMPLEMENT_DYNAMIC(CAsmPerfPage, CPropertyPage)
CAsmPerfPage::CAsmPerfPage()
	: CPropertyPage(CAsmPerfPage::IDD)
{
}

CAsmPerfPage::~CAsmPerfPage()
{
}

void CAsmPerfPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAsmPerfPage, CPropertyPage)
END_MESSAGE_MAP()


// CAsmPerfPage 消息处理程序
