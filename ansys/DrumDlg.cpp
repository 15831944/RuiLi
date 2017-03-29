// DrumDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "DrumDlg.h"


// CDrumDlg

IMPLEMENT_DYNAMIC(CDrumDlg, CPropertySheet)
CDrumDlg::CDrumDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CDrumDlg::CDrumDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CDrumDlg::~CDrumDlg()
{
}


BEGIN_MESSAGE_MAP(CDrumDlg, CPropertySheet)
END_MESSAGE_MAP()


// CDrumDlg 消息处理程序
