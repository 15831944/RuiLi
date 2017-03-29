// TowerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "TowerDlg.h"


// CTowerDlg

IMPLEMENT_DYNAMIC(CTowerDlg, CPropertySheet)
CTowerDlg::CTowerDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CTowerDlg::CTowerDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CTowerDlg::~CTowerDlg()
{
}


BEGIN_MESSAGE_MAP(CTowerDlg, CPropertySheet)
END_MESSAGE_MAP()


// CTowerDlg 消息处理程序
