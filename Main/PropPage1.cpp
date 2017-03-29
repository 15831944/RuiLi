// PropPage1.cpp : 实现文件
//// 事物特性表

#include "stdafx.h"
#include "Main.h"
#include "PropPage1.h"
#include "UsrSWTXTable.h"

// CPropPage1 对话框

IMPLEMENT_DYNAMIC(CPropPage1, CPropertyPage)
CPropPage1::CPropPage1()
	: CPropertyPage(CPropPage1::IDD)
	,p_temp_UsrSWTXTableList(NULL)
	,mdl(NULL)
	, m_Name(_T(""))
	, m_Value(_T(""))
	, m_Note(_T(""))
	, m_index(-1)
{
}

CPropPage1::~CPropPage1()
{
}

void CPropPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_EDIT2, m_Value);
	DDX_Text(pDX, IDC_EDIT3, m_Note);
	DDX_Control(pDX, IDC_EDIT2, m_ValueButton);
}


BEGIN_MESSAGE_MAP(CPropPage1, CPropertyPage)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON2, OnModefy)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CPropPage1 消息处理程序

int CPropPage1::SetValue(ProMdl solid, CArray<UsrSWTXTable1,UsrSWTXTable1>* pp_temp_UsrSWTXTableList)
{
    p_temp_UsrSWTXTableList=pp_temp_UsrSWTXTableList;
	mdl=solid;
	return 0;
}

BOOL CPropPage1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 显示和刷新列表框                                                     */
/************************************************************************/
int CPropPage1::SetList(void)
{
	int num=0;
	int indexNo=0;
	m_List.DeleteAllItems();
	for(int i=0;i<3;i++)
	{
		m_List.DeleteColumn(0);
	}
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"名称",LVCFMT_LEFT,60,-1);
	m_List.InsertColumn(1,"初始值",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(2,"变型规则",LVCFMT_LEFT,200,-1);
    num=(int)p_temp_UsrSWTXTableList->GetCount();
	for(int i=0;i<num;i++)
	{
		p_temp_UsrSWTXTableList->GetAt(i).Update();
		if(p_temp_UsrSWTXTableList->GetAt(i).owner!=mdl) continue;
		if(p_temp_UsrSWTXTableList->GetAt(i).isCheck!=1) continue;
		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,0,p_temp_UsrSWTXTableList->GetAt(i).Name,0,0,0,i);
		m_List.SetItemText(indexNo,1,p_temp_UsrSWTXTableList->GetAt(i).value);
		m_List.SetItemText(indexNo,2,p_temp_UsrSWTXTableList->GetAt(i).Note);
	}
	return 0;
}
/************************************************************************/
/* 左键单击列表框                                                       */
/************************************************************************/
void CPropPage1::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int num=0;
	num=(int)m_List.GetItemCount();
	for(int i=0;i<num;i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;
        m_Name=m_List.GetItemText(i,0);
		m_Value=m_List.GetItemText(i,1);
		m_Note=m_List.GetItemText(i,2);
		m_index=(int)m_List.GetItemData(i);
	}
	//m_ValueButton.en
	UpdateData(FALSE);

	*pResult = 0;
}
/************************************************************************/
/* 修改                                                                 */
/************************************************************************/
void CPropPage1::OnModefy()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	if(m_index==-1) 
	{AfxMessageBox("请先选择一项！");return;}
	if(m_Name=="") 
	{
		AfxMessageBox("名称不能为空！");
		return;
	}
	//if(p_temp_UsrSWTXTableList->GetAt(m_index).type=="dim")
	//{

	//}

    p_temp_UsrSWTXTableList->GetAt(m_index).Name=m_Name;
   // p_temp_UsrSWTXTableList->GetAt(m_index).value=m_Value;
	p_temp_UsrSWTXTableList->GetAt(m_index).Note=m_Note;
	SetList();
}
/************************************************************************/
/* 清空编辑框中的内容                                                   */
/************************************************************************/
void CPropPage1::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Name="";
	m_Value="";
	m_Note="";
	m_index=-1;
	UpdateData(FALSE);
}
/************************************************************************/
/* 删除                                                                 */
/************************************************************************/
void CPropPage1::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp;
	if(m_index==-1) return;
    temp.Format("确认删除事物特性表项：“%s”！",p_temp_UsrSWTXTableList->GetAt(m_index).Name);
	if(AfxMessageBox(temp)!=IDOK) return;
    p_temp_UsrSWTXTableList->RemoveAt(m_index,1);
	//清空
	OnBnClickedButton3();
	//刷新
	SetList();
}
