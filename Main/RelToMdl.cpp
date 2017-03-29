// RelToMdl.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "RelToMdl.h"


// CRelToMdl 对话框

IMPLEMENT_DYNAMIC(CRelToMdl, CDialog)
CRelToMdl::CRelToMdl(CWnd* pParent /*=NULL*/)
	: CDialog(CRelToMdl::IDD, pParent)
	,mdl(NULL)
{
}

CRelToMdl::~CRelToMdl()
{
}

void CRelToMdl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CRelToMdl, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CRelToMdl 消息处理程序
/*确定把关系添加到模型中
 *
 */
void CRelToMdl::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
	//CArray<ProWstring,ProWstring> w_array;    
	int num=0;   
	num=(int)m_List.GetItemCount();

	for(int i=0;i<num;i++)
	{
		if(!ListView_GetCheckState(m_List,i)) continue;
	    AddRelToMdl(mdl,m_List.GetItemText(i,1));
	}   

	OnOK();
}

BOOL CRelToMdl::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_List.InsertColumn(0,"编号",LVCFMT_LEFT,40);
	m_List.InsertColumn(1,"关系式",LVCFMT_LEFT,140);
	ShowRel();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 设置需要显示的关系式                                                 */
/************************************************************************/
int CRelToMdl::SetRel(ProMdl solid/*,CArray<UsrRelTable,UsrRelTable>  *p_temp_UsrRelTalbe*/)
{
	//pp_UsrRelTable.Copy(*p_temp_UsrRelTalbe);
	mdl=solid;
	return 0;
}
/************************************************************************/
/* 设置需要显示的关系式                                                 */
/************************************************************************/
int CRelToMdl::SetRel(ProMdl solid,CArray<UsrRelTable,UsrRelTable>  *p_temp_UsrRelTalbe)
{
	//pp_UsrRelTable.Copy(*p_temp_UsrRelTalbe);
	mdl=solid;
	return 0;
}
/************************************************************************/
/* 显示关系式                                                           */
/************************************************************************/
int CRelToMdl::ShowRel(void)
{
	int indexNo=0;	
	//int num=(int)pp_UsrRelTable.GetCount();
	CString				temp;
	CString				rel;
	m_List.DeleteAllItems();
	CArray<CRel,CRel&>  m_RelList;
	CArray<int,int&>	m_IndexList;
	g_RelCollection.GetSolidAllRel((ProSolid)mdl,true,&m_RelList,&m_IndexList);
	for(int i=0;i<m_RelList.GetCount();i++)
	{
		//////////////////////////////////////////////////////////////////////////		
		//if(pp_UsrRelTable[i].owner!=mdl) 
		//{
		//	//pp_UsrRelTable.RemoveAt(i,1);
		//	//num=num-1;
		//	continue;
		//}
		//if(pp_UsrRelTable[i].isCheck!=1) 
		//{
		//	//pp_UsrRelTable.RemoveAt(i,1);
		//	//num=num-1;
		//	continue;
		//}
		//temp.Format("%d",indexNo+1);
  //      m_List.InsertItem(indexNo++,temp);
		//m_List.SetItemText(indexNo-1,1,pp_UsrRelTable[i].rel);
		//////////////////////////////////////////////////////////////////////////
		temp.Format("%d",indexNo+1);
		GetMdlType(mdl)==PRO_MDL_PART?m_RelList[i].GetRelString(rel):m_RelList[i].GetRelStringAsComp(rel);		
		m_List.InsertItem(indexNo++,temp);
		m_List.SetItemText(indexNo-1,1,rel);
		//////////////////////////////////////////////////////////////////////////		
	}
	return 0;
}

int CRelToMdl::AddRelToMdl(ProMdl solid, CString rel)
{
	ProError status;
	ProLine w_line;
	ProWstring *w_array;
	int n_lines;
	ProRelset relset;
	wchar_t *w_ptr;
    ProCharLine c_w_line;
	ProModelitem modelitem;

	//获得当前模型
	//ProMdlCurrentGet((ProMdl*)&solid);
	//获得当前模型项
	ProMdlToModelitem(solid, &modelitem);
	//指定输入和显示的信息文件名

	//显示输入关系式的提示
    // PRO_LINE_SIZE
	//读入输入的关系式
    //为保存关系式的数组分配内存
		status = ProArrayAlloc(0, sizeof(ProWstring), 1, (ProArray*)&w_array);
		//获得当前模型项关系式集的指针
		status = ProModelitemToRelset(&modelitem, &relset);		
		if (status == PRO_TK_NO_ERROR)
			//如果当前模型的关系式集存在，得到关系式字串数组
			status = ProRelsetRelationsGet(&relset, &w_array);
		else
			//如果当前模型的关系式集不存在创建新的关系式集，并获得其指针
			status = ProRelsetCreate(&modelitem, &relset);
		sprintf(c_w_line,"%s",rel.GetString());
		ProStringToWstring(w_line,c_w_line);
		w_ptr = w_line;
		//将新的关系式添加到关系式字串数组中
		status = ProArrayObjectAdd((ProArray*)&w_array, PRO_VALUE_UNUSED, 1,
			&w_ptr);
		//得到关系式字串数组的元素数目
		status = ProArraySizeGet((ProArray)w_array, &n_lines);
		//为关系式集设置新的关系式
		status = ProRelsetRelationsSet(&relset, w_array, n_lines);
        status=ProRelsetRegenerate(&relset);
		if(status!=PRO_TK_NO_ERROR) AfxMessageBox("无法添加，请转到模型内部的关系式，清楚不正确的关系式！");
		status = ProArrayFree((ProArray*)&w_array);
	
	
	return 0;
}
