// AsmSWTXPage.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "AsmSWTXPage.h"
//BOOL CheckAsmSWTX(ProSolid asm_solid,AsmSmlItem * p_asmSwtx);
//extern CArray<AsmSmlItem,AsmSmlItem> p_AsmSmlItemList;

// CAsmSWTXPage 对话框

IMPLEMENT_DYNAMIC(CAsmSWTXPage, CPropertyPage)
CAsmSWTXPage::CAsmSWTXPage()
	: CPropertyPage(CAsmSWTXPage::IDD)
	, m_Name(_T(""))
	, m_Value(_T(""))
	, m_Note(_T(""))
	, m_selListData(-1)
{
}

CAsmSWTXPage::~CAsmSWTXPage()
{
}

void CAsmSWTXPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, 1002, m_List);
	DDX_Text(pDX, 1003, m_Name);
	DDX_Text(pDX, 1014, m_Value);
	DDX_Text(pDX, IDC_EDIT3, m_Note);
}


BEGIN_MESSAGE_MAP(CAsmSWTXPage, CPropertyPage)
	ON_NOTIFY(NM_CLICK, 1002, OnNMClick1002)
END_MESSAGE_MAP()


// CAsmSWTXPage 消息处理程序

int CAsmSWTXPage::SetList(void)
{
	int               num=0;
	int               indexNo=0;
	CString           temp;
	ProError          status;
	CString           csValue;
	ProParamvalue     proval;
	double            value;
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"名称",LVCFMT_LEFT,60,-1);
	m_List.InsertColumn(1,"初始值",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(2,"变型规则",LVCFMT_LEFT,200,-1);
	num=(int)p_AsmSmlItemList.GetCount();
	for(int i=0;i<num;i++)
	{
		//检查及过滤
		if(p_AsmSmlItemList.GetAt(i).owner!=mdl) continue;
		if (!CheckAsmSWTX(mdl,&p_AsmSmlItemList[i]))
		{
			p_AsmSmlItemList.RemoveAt(i,1);
			i--;
			num--;
			continue;
		}
		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,0,p_AsmSmlItemList[i].name,0,0,0,i);
		switch(p_AsmSmlItemList[i].marsk) {
		case TYPE_DIM:

			status=ProDimensionValueGet(&p_AsmSmlItemList[i].dim,&value);
			temp.Format("%f",value);
			m_List.SetItemText(indexNo,1,temp);
			break;
		case TYPE_PARA:
			status=ProParameterValueGet(&p_AsmSmlItemList[i].para,&proval);
			switch(proval.type) {
		case PRO_PARAM_DOUBLE:
			csValue.Format("%f",proval.value.d_val);
			break;
		case PRO_PARAM_STRING:
			csValue=CString(proval.value.s_val);
			break;
		case PRO_PARAM_INTEGER:
			csValue.Format("%d",proval.value.i_val);
			break;
		case PRO_PARAM_BOOLEAN:
			csValue.Format("%d",proval.value.l_val);
			break;
		case PRO_PARAM_NOTE_ID:
			csValue="PRO_PARAM_NOTE_ID";
			break;
		case PRO_PARAM_VOID:
			csValue="PRO_PARAM_VOID";
			break;
		default:
			csValue="默认";
			break;
			}
			m_List.SetItemText(indexNo,1,csValue);
			break;
		case TYPE_FEAT:
			break;
		case TYPE_COMP:
			break;
		default:
			break;
		}
		m_List.SetItemText(indexNo,2,p_AsmSmlItemList[i].note);
	}
	return 0;
}

BOOL CAsmSWTXPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
     SetList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CAsmSWTXPage::OnNMClick1002(NMHDR *pNMHDR, LRESULT *pResult)
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
		m_selListData=(int)m_List.GetItemData(i);
	}
	UpdateData(FALSE);
	*pResult = 0;
}
