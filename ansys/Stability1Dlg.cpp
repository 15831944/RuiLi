// Stability1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "Stability1Dlg.h"


// CStability1Dlg 对话框

IMPLEMENT_DYNAMIC(CStability1Dlg, CPropertyPage)
CStability1Dlg::CStability1Dlg()
	: CPropertyPage(CStability1Dlg::IDD)
{
}

CStability1Dlg::~CStability1Dlg()
{
}

void CStability1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStability1Dlg, CPropertyPage)
END_MESSAGE_MAP()


// CStability1Dlg 消息处理程序
