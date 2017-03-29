// Stability0Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "Stability0Dlg.h"


// CStability0Dlg 对话框

IMPLEMENT_DYNAMIC(CStability0Dlg, CPropertyPage)
CStability0Dlg::CStability0Dlg()
	: CPropertyPage(CStability0Dlg::IDD)
{
}

CStability0Dlg::~CStability0Dlg()
{
}

void CStability0Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStability0Dlg, CPropertyPage)
END_MESSAGE_MAP()


// CStability0Dlg 消息处理程序
