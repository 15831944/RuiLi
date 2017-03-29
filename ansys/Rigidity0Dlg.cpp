// Rigidity0Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "Rigidity0Dlg.h"


// CRigidity0Dlg 对话框

IMPLEMENT_DYNAMIC(CRigidity0Dlg, CPropertyPage)
CRigidity0Dlg::CRigidity0Dlg()
	: CPropertyPage(CRigidity0Dlg::IDD)
{
}

CRigidity0Dlg::~CRigidity0Dlg()
{
}

void CRigidity0Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRigidity0Dlg, CPropertyPage)
END_MESSAGE_MAP()


// CRigidity0Dlg 消息处理程序
