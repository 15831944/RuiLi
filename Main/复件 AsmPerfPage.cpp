// AsmPerfPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ASM.h"
#include "AsmPerfPage.h"


// CAsmPerfPage �Ի���

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


// CAsmPerfPage ��Ϣ�������
