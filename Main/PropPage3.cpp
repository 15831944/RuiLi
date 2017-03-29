// PropPage3.cpp : 实现文件
//关系

#include "stdafx.h"
#include "Main.h"
#include "PropPage3.h"


// CPropPage3 对话框

IMPLEMENT_DYNAMIC(CPropPage3, CPropertyPage)
CPropPage3::CPropPage3()
	: CPropertyPage(CPropPage3::IDD)
	/*,p_temp_UsrRelTableList(NULL)*/
	,mdl(NULL)
{
}

CPropPage3::~CPropPage3()
{
}

void CPropPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CPropPage3, CPropertyPage)
END_MESSAGE_MAP()


// CPropPage3 消息处理程序

int CPropPage3::SetValue(ProMdl solid/*,CArray<UsrRelTable,UsrRelTable>* pp_temp_UsrRelTableList*/)
{
	//p_temp_UsrRelTableList=pp_temp_UsrRelTableList;
	mdl=solid;
	return 0;
}

BOOL CPropPage3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
    SetList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

int CPropPage3::SetList(void)
{
	//int num=0;
	//int indexNo=0;
	//int temp=0;
	//CString No="";

	CArray<CRel,CRel&>	p_RelList;
	CArray<int,int&>	p_IndexList;
	CString				id="";
	CString				rel;
	int					indexNo=0;
	m_List.DeleteAllItems();
	for(int i=0;i<3;i++)
	{
		m_List.DeleteColumn(0);
	}
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"编号",LVCFMT_LEFT,40);
	m_List.InsertColumn(1,"关系式",LVCFMT_LEFT,100);
	m_List.InsertColumn(2,"说明",LVCFMT_LEFT,300);
	//num=(int)p_temp_UsrRelTableList->GetCount();
    //////////////////////////////////////////////////////////////////////////    
	//for(int i=0;i<num;i++)
	//{
	//	if(p_temp_UsrRelTableList->GetAt(i).owner!=mdl) continue;
 //       if(p_temp_UsrRelTableList->GetAt(i).isCheck!=1) continue;
	//	No.Format("%d",++temp);
	//	indexNo=m_List.InsertItem(temp-1,No);
	//	m_List.SetItemText(indexNo,1,p_temp_UsrRelTableList->GetAt(i).rel);
	//	m_List.SetItemText(indexNo,2,p_temp_UsrRelTableList->GetAt(i).note);
	//}
	//////////////////////////////////////////////////////////////////////////
	m_List.DeleteAllItems();
	g_RelCollection.GetSolidAllRel((ProSolid)mdl,false,&p_RelList,&p_IndexList);
	for (int i=0;i<p_RelList.GetCount();i++)
	{
		id.Format(" %d",m_List.GetItemCount()+1);
		p_RelList[i].GetRelString(rel);
		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,(int)m_List.GetItemCount(),id,0,0,0,p_IndexList[i]);
		m_List.SetItemText(indexNo,1,rel);
		m_List.SetItemText(indexNo,2,p_RelList[i].GetRelNote());
	}
	return 0;
}
