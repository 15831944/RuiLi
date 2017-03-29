// AsmSearchPage.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "AsmSearchPage.h"


// CAsmSearchPage 对话框

IMPLEMENT_DYNAMIC(CAsmSearchPage, CDialog)
CAsmSearchPage::CAsmSearchPage(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmSearchPage::IDD, pParent)
{
}

CAsmSearchPage::~CAsmSearchPage()
{
}

void CAsmSearchPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAsmSearchPage, CDialog)
END_MESSAGE_MAP()


// CAsmSearchPage 消息处理程序
