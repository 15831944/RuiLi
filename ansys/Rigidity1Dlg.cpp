// Rigidity1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "Rigidity1Dlg.h"


// CRigidity1Dlg 对话框

IMPLEMENT_DYNAMIC(CRigidity1Dlg, CPropertyPage)
CRigidity1Dlg::CRigidity1Dlg()
	: CPropertyPage(CRigidity1Dlg::IDD)
{
}

CRigidity1Dlg::~CRigidity1Dlg()
{
}

void CRigidity1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRigidity1Dlg, CPropertyPage)
END_MESSAGE_MAP()


// CRigidity1Dlg 消息处理程序
