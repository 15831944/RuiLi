// ColumnDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "ColumnDlg.h"


// CColumnDlg

IMPLEMENT_DYNAMIC(CColumnDlg, CPropertySheet)
CColumnDlg::CColumnDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CColumnDlg::CColumnDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CColumnDlg::~CColumnDlg()
{
}


BEGIN_MESSAGE_MAP(CColumnDlg, CPropertySheet)
END_MESSAGE_MAP()


// CColumnDlg 消息处理程序
