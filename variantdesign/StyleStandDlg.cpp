// StyleStandDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "StyleStandDlg.h"
#include "ProWindows.h"
#include "MyDatabase.h"
#include "afxtempl.h"

// CStyleStandDlg 对话框
extern FtpInfo Ftp_struc;

extern CMyDatabase m_db;

IMPLEMENT_DYNAMIC(CStyleStandDlg, CDialog)
CStyleStandDlg::CStyleStandDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStyleStandDlg::IDD, pParent)
{
}

CStyleStandDlg::~CStyleStandDlg()
{
}

void CStyleStandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStyleStandDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_MOVE()
	ON_NOTIFY(NM_CLICK, IDC_TREE_STYLESTAND, OnNMClickTreeStylestand)
END_MESSAGE_MAP()


// CStyleStandDlg 消息处理程序

void CStyleStandDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem=m_FtpTree.GetSelectedItem();
	path=m_FtpTree.GetFullPathEx(hItem);
	OnOK();
}

void CStyleStandDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

BOOL CStyleStandDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	m_FtpTree.SubclassDlgItem(IDC_TREE_STYLESTAND,this);
	m_FtpTree.SetConnect(Ftp_struc.strFtp,Ftp_struc.iPort,Ftp_struc.FtpName,Ftp_struc.FtpPassword);
	m_FtpTree.DisplayTree("临时目录",TRUE);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CStyleStandDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRepaint(PRO_VALUE_UNUSED);

	// TODO: 在此添加消息处理程序代码
}

void CStyleStandDlg::OnNMClickTreeStylestand(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	
	*pResult = 0;
}
