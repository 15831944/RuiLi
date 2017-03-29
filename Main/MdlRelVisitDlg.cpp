// MdlRelVisitDlg.cpp : 实现文件
//用于访问单前模型的所有关系式
//便于从当前关系中选择所需要的关系添加到数据库

#include "stdafx.h"
#include "Main.h"
#include "MdlRelVisitDlg.h"
//ProError UsrSolidRelsetVisAc(ProRelset *p_relset,CArray<ProRelset,ProRelset>* caller_data);
//extern int DlgIcon;
// CMdlRelVisitDlg 对话框

IMPLEMENT_DYNAMIC(CMdlRelVisitDlg, CDialog)
CMdlRelVisitDlg::CMdlRelVisitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMdlRelVisitDlg::IDD, pParent)
	,mdl(NULL)

{
}

CMdlRelVisitDlg::~CMdlRelVisitDlg()
{
}

void CMdlRelVisitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CMdlRelVisitDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CMdlRelVisitDlg 消息处理程序
/************************************************************************/
/* 获得模型的关系式                                                     */
/************************************************************************/
bool CMdlRelVisitDlg::GetMdlRel(void)
{
    int num=0;
    //访问模型的关系式
	if(ProSolidRelsetVisit(mdl,(ProSolidRelsetVisitAction)UsrSolidRelsetVisAc,
		(CArray<ProRelset,ProRelset>*)&p_ProRelSetList)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("访问关系式失败！");
		return false;
	}
		

    num=(int)p_ProRelSetList.GetCount();
	for(int i=0;i<num;i++)
	{
        ProWstring *w_array;                        //字符串数组
		int n_lines;
	  	ProArrayAlloc(0,sizeof(ProWstring),1,(ProArray*)&w_array);
        ProRelsetRelationsGet(&p_ProRelSetList[i],&w_array);
		ProArraySizeGet((ProArray)w_array,&n_lines);
		for(int j=0;j<n_lines;j++)
		{
			CString temp;
			temp=CString(w_array[j]);
			//把空行给排除
			if(temp=="") continue;
			rel_stringList.Add(temp);
		}
        ProWstringproarrayFree(w_array);

	}
	return true;
}

void CMdlRelVisitDlg::ShowMdlRel(void)
{
	int indexNo=0;
	int num=0;
	int no=0;
	CString temp;
	m_List.DeleteAllItems();
	num=(int)rel_stringList.GetCount();
	for(int i=0;i<num;i++)
	{
		temp.Format("%d",++no);
		indexNo=m_List.InsertItem(no-1,temp);
		m_List.SetItemText(indexNo,1,rel_stringList[i]);
		//如果在模型中已经存在，则选中
		//3
		if(g_RelCollection.IsRelInCollection((ProSolid)mdl,rel_stringList[i]))
		{
			ListView_SetCheckState(m_List,i,TRUE);
		}
	}
}

void CMdlRelVisitDlg::SetMdl(ProMdl owner)
{
	mdl=owner;
}

BOOL CMdlRelVisitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
    m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_List.InsertColumn(0,"编号",LVCFMT_LEFT,40,-1);
	m_List.InsertColumn(1,"关系式",LVCFMT_LEFT,140,-1);
	if(!GetMdlRel()) return FALSE;

	ShowMdlRel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
//ProError UsrSolidRelsetVisAc(ProRelset *p_relset,CArray<ProRelset,ProRelset>* caller_data)
//{
//	
//	caller_data->Add(*p_relset);
//	return PRO_TK_NO_ERROR;
//	
//}

void CMdlRelVisitDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<(int)m_List.GetItemCount();i++)
	{
		if(!ListView_GetCheckState(m_List,i)) continue;
		rel_stringList_select.Add(m_List.GetItemText(i,1));
	}
	OnOK();
}

