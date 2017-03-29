// SelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "SelectDlg.h"

#include "afxtempl.h"
#include "MyDatabase.h"
#include "ProWindows.h"
#include "Compute.h"
#include "CommonFuncClass.h"
#include "DetailInfoClass.h"


extern FtpInfo Ftp_struc;
extern CMyDatabase m_db;
extern CPtrArray p_classarray;
extern CArray<ItemHandle1,ItemHandle1> treehandle1;


// CSelectDlg 对话框

IMPLEMENT_DYNAMIC(CSelectDlg, CDialog)
CSelectDlg::CSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectDlg::IDD, pParent)
{
	s=-1;
	num=-1;
}

CSelectDlg::~CSelectDlg()
{
}

void CSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PART, m_parttree);
}


BEGIN_MESSAGE_MAP(CSelectDlg, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_TREE_PART, OnNMClickTreePart)
END_MESSAGE_MAP()


// CSelectDlg 消息处理程序

BOOL CSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	CCommonFuncClass commonfun;
	if(Type=="prt")
	{
		commonfun.ObtainPartModelClass(&m_parttree);      //读取零件的分类
	}
	else
		if(Type=="asm")
	{
		commonfun.ObtainProductModelClass(&m_parttree);
	}
		else
			return FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control

}

void CSelectDlg::OnMove(int x, int y)
{
   CDialog::OnMove(x, y);
   ProWindowRepaint(PRO_VALUE_UNUSED);
	// TODO: 在此添加消息处理程序代码
}

void CSelectDlg::OnBnClickedOk()
{
	OnOK();
}
void CSelectDlg::OnNMClickTreePart(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem;
	CString str,sql;
	UINT uFlags=0;
	CPoint point;
	point= GetCurrentMessage()->pt;
	m_parttree.ScreenToClient(&point);
	hItem =m_parttree.HitTest(point, &uFlags);
	if (hItem!=NULL && (uFlags&TVHT_ONITEM))
	{
		num=(int)m_parttree.GetItemData(hItem);
		Name=m_parttree.GetItemText(hItem);
	}
	*pResult = 0;
}
