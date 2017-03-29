// UnitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "UnitDlg.h"


// CUnitDlg 对话框

IMPLEMENT_DYNAMIC(CUnitDlg, CDialog)
CUnitDlg::CUnitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUnitDlg::IDD, pParent)
{
	m_ID="";
}

CUnitDlg::~CUnitDlg()
{
}

void CUnitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_List);
}


BEGIN_MESSAGE_MAP(CUnitDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT, OnNMClickListProduct)
END_MESSAGE_MAP()


// CUnitDlg 消息处理程序

BOOL CUnitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,"单位",LVCFMT_LEFT,150,-1);
	

	int i=0;
	m_List.InsertItem(i,"mm");i++;
	m_List.InsertItem(i,"inch");i++;



	if(m_ID!="")
	{
		int number=-1;
		for(int j=0;j<m_List.GetItemCount();j++)
		{
			if (m_List.GetItemText(j,1)==m_ID)
			{
				number=j;
			}
		}
		if(number!=-1)
		{ 
			m_List.SetItemState(number,LVIS_SELECTED,LVIS_SELECTED); 
			UpdateData(FALSE);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

	void CUnitDlg::OnBnClickedOk()
	{
		int number=-1;
		for(int j=0;j<m_List.GetItemCount();j++)
		{
			if ( m_List.GetItemState(j,LVIS_SELECTED)==LVIS_SELECTED)
			{
				number=j;
			}
		}
		if(number==-1) return;

		m_ID=m_List.GetItemText(number,0);////////
		OnOK();
	}

	void CUnitDlg::OnNMClickListProduct(NMHDR *pNMHDR, LRESULT *pResult)
	{
		int number=-1;
		for(int j=0;j<m_List.GetItemCount();j++)
		{
			if ( m_List.GetItemState(j,LVIS_SELECTED)==LVIS_SELECTED)
			{
				number=j;
			}
		}
		if(number==-1) return;
		m_ID=m_List.GetItemText(number,0);////////
		UpdateData(FALSE);
	}

