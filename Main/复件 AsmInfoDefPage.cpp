// AsmInfoDefPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ASM.h"
#include "AsmInfoDefPage.h"


// CAsmInfoDefPage �Ի���

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


// CAsmInfoDefPage ��Ϣ�������
