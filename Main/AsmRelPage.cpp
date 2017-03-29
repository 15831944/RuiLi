// AsmRelPage.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "AsmRelPage.h"
//extern CArray<AsmRelItem,AsmRelItem> p_AsmRelItemList;

// CAsmRelPage 对话框

IMPLEMENT_DYNAMIC(CAsmRelPage, CPropertyPage)
CAsmRelPage::CAsmRelPage()
	: CPropertyPage(CAsmRelPage::IDD)
	, m_index(-1)
	,mdl(NULL)
{
}

CAsmRelPage::~CAsmRelPage()
{
}

void CAsmRelPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, 1002, m_List);
}


BEGIN_MESSAGE_MAP(CAsmRelPage, CPropertyPage)
END_MESSAGE_MAP()


// CAsmRelPage 消息处理程序

BOOL CAsmRelPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
    ShowList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

int CAsmRelPage::ShowList(void)
{
	//int       num=0;
	//int       indexNo=0;
	//int       temp=0;
	//CString   No="";
	//m_List.DeleteAllItems();
	//do {		
	//} while(m_List.DeleteColumn(0)==TRUE);
	//m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	//m_List.InsertColumn(0,"编号",LVCFMT_LEFT,40);
	//m_List.InsertColumn(1,"关系式",LVCFMT_LEFT,100);
	//m_List.InsertColumn(2,"说明",LVCFMT_LEFT,200);
	//num=(int)p_AsmRelItemList.GetCount();

	//for(int i=0;i<num;i++)
	//{
	//	if(p_AsmRelItemList[i].owner!=mdl) continue;
	//	No.Format("%d",++temp);
	//	indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,temp-1,No,0,0,0,i);
	//	m_List.SetItemText(indexNo,1,p_AsmRelItemList[i].relation);
	//	m_List.SetItemText(indexNo,2,p_AsmRelItemList[i].note);
	//}
	CArray<CRel,CRel&>	p_RelList;
	CArray<int,int&>	p_IndexList;
	CString				id="";
	CString				rel;
	int					indexNo=0;
	m_List.DeleteAllItems();
	do {		
	} while(m_List.DeleteColumn(0)==TRUE);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"编号",LVCFMT_LEFT,40);
	m_List.InsertColumn(1,"关系式",LVCFMT_LEFT,100);
	m_List.InsertColumn(2,"说明",LVCFMT_LEFT,300);
	g_RelCollection.GetSolidAllRel(mdl,true,&p_RelList,&p_IndexList);
	for (int i=0;i<p_RelList.GetCount();i++)
	{
		id.Format(" %d",m_List.GetItemCount()+1);
		p_RelList[i].GetRelStringAsComp(rel);
		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,(int)m_List.GetItemCount(),id,0,0,0,p_IndexList[i]);
		m_List.SetItemText(indexNo,1,rel);
		m_List.SetItemText(indexNo,2,p_RelList[i].GetRelNote());
	}
	return 0;
}
