// SelectDlg.cpp : ʵ���ļ�
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


// CSelectDlg �Ի���

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


// CSelectDlg ��Ϣ�������

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
		commonfun.ObtainPartModelClass(&m_parttree);      //��ȡ����ķ���
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
	// TODO: �ڴ������Ϣ����������
}

void CSelectDlg::OnBnClickedOk()
{
	OnOK();
}
void CSelectDlg::OnNMClickTreePart(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
