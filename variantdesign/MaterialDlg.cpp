// MaterialDlg.cpp : ÊµÏÖÎÄ¼þ
//

#include "stdafx.h"
#include "variantdesign.h"
#include "MaterialDlg.h"


// CMaterialDlg ¶Ô»°¿ò

IMPLEMENT_DYNAMIC(CMaterialDlg, CDialog)
CMaterialDlg::CMaterialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaterialDlg::IDD, pParent)
	, m_Text(_T(""))
{
	m_ID="";
}

CMaterialDlg::~CMaterialDlg()
{
}

void CMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_List);
	DDX_Text(pDX, IDC_Text, m_Text);
}


BEGIN_MESSAGE_MAP(CMaterialDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT, OnNMClickListProduct)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRODUCT, OnNMDblclkListProduct)
END_MESSAGE_MAP()


// CMaterialDlg ÏûÏ¢´¦Àí³ÌÐò

BOOL CMaterialDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,"ÀàÐÍ",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(1,"±àºÅ",LVCFMT_LEFT,200,-1);

    int i=0;
	m_List.InsertItem(i,"ÂÁ²Ä");m_List.SetItemText(i,1,"2A12");i++;
	m_List.InsertItem(i,"ÂÁ²Ä");m_List.SetItemText(i,1,"Y112A");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"35¸Ö");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"45¸Ö");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"Q235");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"Q235AF");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"SPHC");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"ST12");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"ST13");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"ST14");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"ST16");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"Dx3");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"Dx2");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"65Mn");i++;
	m_List.InsertItem(i,"¸Ö²Ä");m_List.SetItemText(i,1,"60Si2Mn");i++;
	m_List.InsertItem(i,"Ïð½º");m_List.SetItemText(i,1,"NBRT001");i++;
	m_List.InsertItem(i,"Ïð½º");m_List.SetItemText(i,1,"NBRT002");i++;
	m_List.InsertItem(i,"Ïð½º");m_List.SetItemText(i,1,"CR5503");i++;
	m_List.InsertItem(i,"Ïð½º");m_List.SetItemText(i,1,"F7504");i++;
	m_List.InsertItem(i,"Ïð½º");m_List.SetItemText(i,1,"BR5505");i++;
	m_List.InsertItem(i,"Ïð½º");m_List.SetItemText(i,1,"NBR5507");i++;
	m_List.InsertItem(i,"Ïð½º");m_List.SetItemText(i,1,"NBR5509");i++;
	m_List.InsertItem(i,"Ïð½º");m_List.SetItemText(i,1,"NBR6506");i++;
	m_List.InsertItem(i,"Ïð½º");m_List.SetItemText(i,1,"NBR8008");i++;
	m_List.InsertItem(i,"Ïð½º");m_List.SetItemText(i,1,"Q7010");i++;
	m_List.InsertItem(i,"Ïð½º");m_List.SetItemText(i,1,"PN7011");i++;
	m_List.InsertItem(i,"Ïð½º");m_List.SetItemText(i,1,"CM580");i++;
	m_List.InsertItem(i,"²»Ðâ¸Ö");m_List.SetItemText(i,1,"1Cr17");i++;
	m_List.InsertItem(i,"²»Ðâ¸Ö");m_List.SetItemText(i,1,"1Cr13");i++;
	m_List.InsertItem(i,"²»Ðâ¸Ö");m_List.SetItemText(i,1,"2Cr17");i++;
	m_List.InsertItem(i,"²»Ðâ¸Ö");m_List.SetItemText(i,1,"1Cr18Ni9");i++;
	m_List.InsertItem(i,"Í­");m_List.SetItemText(i,1,"T2");i++;
	m_List.InsertItem(i,"Í­");m_List.SetItemText(i,1,"T3");i++;
	m_List.InsertItem(i,"Í­");m_List.SetItemText(i,1,"H59");i++;
	m_List.InsertItem(i,"Í­");m_List.SetItemText(i,1,"HPb59-1");i++;


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
		m_Text.Format("ÄãÑ¡ÔñµÄÊÇ %s",m_ID);
		UpdateData(FALSE);
	}
	
   }



	




	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³££ºOCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}

void CMaterialDlg::OnBnClickedOk()
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

	m_ID=m_List.GetItemText(number,1);////////
	OnOK();
}

void CMaterialDlg::OnNMClickListProduct(NMHDR *pNMHDR, LRESULT *pResult)
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
	m_ID=m_List.GetItemText(number,1);////////
	m_Text.Format("ÄãÑ¡ÔñµÄÊÇ %s",m_ID);
	UpdateData(FALSE);
}

void CMaterialDlg::OnNMDblclkListProduct(NMHDR *pNMHDR, LRESULT *pResult)
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

	m_ID=m_List.GetItemText(number,1);////////
	OnOK();
}
