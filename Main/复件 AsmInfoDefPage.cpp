// AsmInfoDefPage.cpp : 实现文件
//

#include "stdafx.h"
#include "ASM.h"
#include "AsmInfoDefPage.h"


// CAsmInfoDefPage 对话框

IMPLEMENT_DYNAMIC(CAsmInfoDefPage, CPropertyPage)
CAsmInfoDefPage::CAsmInfoDefPage()
	: CPropertyPage(CAsmInfoDefPage::IDD)
{
}

CAsmInfoDefPage::~CAsmInfoDefPage()
{
}

void CAsmInfoDefPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAsmInfoDefPage, CPropertyPage)
END_MESSAGE_MAP()


// CAsmInfoDefPage 消息处理程序
