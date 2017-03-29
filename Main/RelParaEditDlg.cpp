// RelParaEditDlg.cpp : 实现文件
//建立事物特性和关系式以及模型的尺寸，特征，浏览的主窗口

#include "stdafx.h"
#include "Main.h"
#include "RelParaEditDlg.h"
#include <ProSolid.h>
#include <ProModelitem.h>
#include "ProMdl.h"
#include "DimEditDlg.h"
#include "SwtxTableItemEditDlg.h"
#include "UsrSWTXTable.h"
#define DIM_CHECKED 1
#define PARA_CHECKED 2
#define FEAT_CHECKED 3
#define COMP_CHECKED 4

int ParamOperateMenuActFn();
//特征图标
void SetImageListForFeatrure(CImageList * p_imageList);
int GetFeatIconNum(int type);
// CRelParaEditDlg 对话框

IMPLEMENT_DYNAMIC(CRelParaEditDlg, CDialog)
CRelParaEditDlg::CRelParaEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRelParaEditDlg::IDD, pParent)
	,radioFalg(DIM_CHECKED)
	//,XWTXTable_IS_OPEN(0)
	//,GXS_IS_OPEN(0)
	,current_solid(NULL)
	, m_RadioButtonValue(0)
{     
}

CRelParaEditDlg::~CRelParaEditDlg()
{
}

void CRelParaEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//DDX_Control(pDX, IDC_COM1, m_radiobutton1);
	DDX_Control(pDX, IDC_BUTTON2, m_SWTXButtong);
	DDX_Control(pDX, IDC_BUTTON1, m_RELATIONButton);
	DDX_Control(pDX, IDC_CHECK1, m_DimDisplayMode);
	DDX_Control(pDX, IDC_CHECK2, m_FeatDisplayMode);

	//DDX_Control(pDX, IDC_LIST_OF_SWTX, m_List2);
	DDX_Control(pDX, IDC_PART_EDIT1, m_PathEdit);
	DDX_Radio(pDX, IDC_COM1, m_RadioButtonValue);
}

BEGIN_MESSAGE_MAP(CRelParaEditDlg, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)

	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_COM1, OnBnClickedCom1)
	ON_BN_CLICKED(IDC_COM2, OnBnClickedCom2)
	ON_BN_CLICKED(IDC_COM3, OnBnClickedCom3)
	ON_BN_CLICKED(IDC_COM4, OnBnClickedCom4)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_WM_CONTEXTMENU()
	//ON_COMMAND(ID_12045, On12045)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_COMMAND(ID_12058, On12058)
	ON_COMMAND(ID_12059, On12059)
	ON_COMMAND(ID_12076, OnDimEditDlg)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
END_MESSAGE_MAP()

// CRelParaEditDlg 消息处理程序
/***************************************************************************************\
函数：pGetFeats()
说明：访问零件里面所有的特征
\***************************************************************************************/
void CRelParaEditDlg::pGetFeats(ProSolid part,CArray<ProFeature,ProFeature> * p_FeatureList)
{
    ProSolidFeatVisit((ProSolid)part,(ProFeatureVisitAction)partFeatVisAc,
		NULL,(CArray<ProFeature,ProFeature> *)p_FeatureList);
	
}
/****************************************************************************************\
函数：OnInitDialog()
说明：初始化对话框，在树控件中显示出特征树
\****************************************************************************************/
BOOL CRelParaEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	//1.设置单选按钮，尺寸项默认被选中
	radioFalg=DIM_CHECKED;
	m_RadioButtonValue=0;
	//m_radiobutton1.SetCheck(BST_CHECKED);

	//2.设置初始列表框显示尺寸，设置列表框的风格
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	for(int i=0;i<6;i++)
	{
		m_List.DeleteColumn(0);
	}
	
	m_List.InsertColumn(0,"符号",LVCFMT_LEFT,100,0);
	m_List.InsertColumn(1,"类型",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(2,"值",LVCFMT_LEFT,80,0);
	m_List.InsertColumn(3,"上偏差",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(4,"下偏差",LVCFMT_LEFT,50,0);

	//3.设置树的图像
	//添加图像
	imageList.Create(12, 12, ILC_COLOR16 , 4, 4);
	SetImageListForFeatrure(&imageList);
	m_Tree.SetImageList(&imageList,TVSIL_NORMAL);
    //3.判断当前模型的特征树
	ProMdlCurrentGet((ProMdl*)&current_solid);    
	prtShowTree(current_solid);  
	
    ////////////////////////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

/****************************************************************************\
函数：prtShowTree(ProSolid solid)
说明：在树中显示零件模型的所有可见的特征
\****************************************************************************/
void CRelParaEditDlg::prtShowTree(ProSolid solid)
{
	ProName      name;
	ProCharName  strName;
	int          numOfFeat=0;	
	ProMdlNameGet(solid,name);
	ProWstringToString(strName,name);
    //先把原先的都删除
	m_Tree.DeleteAllItems();
	HTREEITEM hItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM
		,_T(CString(strName)+".PRT"),6,6,0,0,-1,NULL,NULL);
	pGetFeats(solid,&FeatureList);
	numOfFeat=(int)FeatureList.GetCount();
	if(numOfFeat==0) return;
	for(int i=0;i<numOfFeat;i++)
	{ 
		ProBoolean     visible;
		ProFeattype    ftype;
		ProFeatureVisibilityGet(&FeatureList[i],&visible); 
		ProFeatureTypeGet (&FeatureList[i], &ftype);
		if(visible==PRO_B_FALSE) continue;
		int type=ProModelitemNameGet(&FeatureList[i],name);       
		if(type==PRO_TK_E_NOT_FOUND)
		{
			ProFeatureTypenameGet(&FeatureList[i],name);
			ProWstringToString(strName,name);
 			sprintf(strName,"%s 标识号%d",strName,FeatureList[i].id);
		}
		else if(type==PRO_TK_BAD_INPUTS)
		{
			sprintf(strName,"无效");
		}
		else 
		{
			ProWstringToString(strName,name);
		}
		m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM,_T(strName),
			GetFeatIconNum(ftype),GetFeatIconNum(ftype), 0, 0, i, hItem,NULL);
	}
	m_Tree.Expand(hItem,TVE_EXPAND);
}

/************************************************************************/
/*函数：OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)  
  说明：在树中点击的时候得到树项的句柄，然后调用ConverthItemToList(hItem)*/
/************************************************************************/
void CRelParaEditDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
 {
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM pItem;
	CString   path;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	path=m_Tree.GetItemText(hItem);
	if(m_FeatDisplayMode.GetState()&0x0003)
	{
		ProSelection selection;
		ProError status;
		status=ProFeatureSelectionGet(&FeatureList[m_Tree.GetItemData(hItem)],
			&selection);
		status=ProSelectionHighlight(selection,PRO_COLOR_HIGHLITE);
		if(status!=PRO_TK_NO_ERROR) AfxMessageBox("错误，不能加亮显示！");
	}
	if (hItem!=m_Tree.GetRootItem()) {
		//编辑框，特征的路径
		pItem=hItem;
		do {
			pItem=m_Tree.GetParentItem(pItem);
			path=m_Tree.GetItemText(pItem)+"\\"+path;
		} while(pItem!=m_Tree.GetRootItem());

	switch(radioFalg) {
	case DIM_CHECKED:
		ConverthItemToList(hItem);
		break;
	case PARA_CHECKED:    //如果是参数按钮选中,则切换到尺寸
		radioFalg=DIM_CHECKED;
		m_RadioButtonValue=0;
		UpdateData(FALSE);
		ConverthItemToList(hItem);
		break;
	case FEAT_CHECKED:
		break;
	case COMP_CHECKED:
		break;
	default:
		break;
		}
	}	
	m_PathEdit.SetWindowText(path);
	*pResult = 0;
}


/******************************************************************************\
函数：ConverthItemToList(HTREEITEM hItem)
说明：当选择树中的某一特征，得到树项的句柄，根据树项内存储的特征的号，得到特征，
      并访问该特征的所有的尺寸，并在列表内显示出来
\******************************************************************************/
void CRelParaEditDlg::ConverthItemToList(HTREEITEM hItem)
{

	ProError status;
	ProFeature termFeat;
    termFeat=FeatureList[(int)m_Tree.GetItemData(hItem)];
    Dimensionlist.RemoveAll();
	status=ProFeatureDimensionVisit(&termFeat,(ProDimensionVisitAction)partFeatDimVisAc ,
		NULL,(CArray<ProDimension,ProDimension> *)&Dimensionlist);
	
	//刷新
    FreshDim();

}
/************************************************************************/
/* 函数：OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
   说明：在列表框内单击左键的时候，访问选中项的尺寸，在模型中显示尺寸    */
/************************************************************************/
void CRelParaEditDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch(radioFalg) {
	case DIM_CHECKED:
		int i;
		if(m_DimDisplayMode.GetState()==1)
		{
			ProDisplaymodeSet(PRODISPMODE_SYMBOLIC);
		}
		else
		{
			ProDisplaymodeSet(PRODISPMODE_NUMERIC);
		}
		//把选中的项与尺寸数组比较，显示相应的尺寸
		for(i=0;i<m_List.GetItemCount();i++)
		{
			if(m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
			{
				ProDimensionShow(&Dimensionlist[(int)m_List.GetItemData(i)],NULL,NULL,NULL);
                break;
			}
		}
		break;
	case FEAT_CHECKED:
		break;
	case PARA_CHECKED:
		break;
	case COMP_CHECKED:
		break;
	default:
		break;
	}
	

	*pResult = 0;
}
/************************************************************************/
/* 函数：OnBnClickedButton2()
   说明：点击事物特性表按钮之后，打开右侧的事物特性表,并初始化          */
/************************************************************************/
void CRelParaEditDlg::OnBnClickedButton2()
{
	CString   text;
	CRect     rect;
	CRect     rect1;
	CRect     rect2;
	m_RELATIONButton.GetWindowText(text);
	if (text==_T("<")) {
		//关闭
		GetWindowRect(rect);
		rel_dlg.GetWindowRect(&rect2);
		rect.right=rect.right-rect2.Width();
		rel_dlg.DestroyWindow();
		MoveWindow(&rect,TRUE);
		m_RELATIONButton.SetWindowText(_T(">"));
	}
	m_SWTXButtong.GetWindowText(text);
	if (text==_T("<")) {
		GetWindowRect(rect);
		swtx_dlg.GetWindowRect(&rect2);
		rect.right=rect.right-rect2.Width();
		swtx_dlg.DestroyWindow();
		MoveWindow(&rect,TRUE);
		m_SWTXButtong.SetWindowText(_T(">"));
		return;
	}
	//打开

	GetWindowRect(rect);
	GetClientRect(rect1);
	swtx_dlg.Create(IDD_PART_SWTX,this);	
	swtx_dlg.GetWindowRect(&rect2);
	rect.right=rect.right+rect2.Width();
	MoveWindow(&rect,TRUE);
	swtx_dlg.MoveWindow(rect1.right,0,rect2.Width(),rect.Height(),TRUE);
	swtx_dlg.ShowWindow(SW_SHOW);
	swtx_dlg.ShowSWTXList(current_solid);
	m_SWTXButtong.SetWindowText(_T("<"));
}

/************************************************************************/
/* 函数：OnBnClickedButton3()
   说明：把选中的项目添加到事物特性表当中，在添加之前要进行判断，是否已经
         事物特性表内                                                   */
/************************************************************************/
void CRelParaEditDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//判断类型，根据不同类型，进行选择
	switch(radioFalg) {
	case DIM_CHECKED:
		AddDimToSWTXList();
		break;
	case PARA_CHECKED:
		AddParaToSWTXList();
		break;
	case FEAT_CHECKED:
		//AddFeatToSWTXList();
		break;
	case COMP_CHECKED:
		//AddCompToSWTXList();
		break;
	default:
		break;
	}
}
/************************************************************************/
/* 下面四个是单选按钮的状态                                             */
/************************************************************************/
void CRelParaEditDlg::OnBnClickedCom1()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem;
	radioFalg=DIM_CHECKED;
    hItem=m_Tree.GetSelectedItem();
	if (hItem!=m_Tree.GetRootItem())
	{
		ConverthItemToList(hItem);
	}
	else m_List.DeleteAllItems();
}
/************************************************************************/
/* 函数：OnBnClickedCom2()
   说明：选定单选按钮之后（参数），把所有的参数添加到列表框中           */
/************************************************************************/
void CRelParaEditDlg::OnBnClickedCom2()
{
	// TODO: 在此添加控件通知处理程序代码
    radioFalg=PARA_CHECKED;
	int indexNo=0;
	m_List.DeleteAllItems();
	for(int i=0;i<10;i++)
	{
        m_List.DeleteColumn(0);
	}
	
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	//m_List.InsertColumn(0,"标识",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(0,"名称",LVCFMT_LEFT,100,0);
	m_List.InsertColumn(1,"类型",LVCFMT_LEFT,80,0);
	m_List.InsertColumn(2,"值",LVCFMT_LEFT,80,0);

    ConverthItemOfParaToList();//把参数添加到列表框
	for(int i=0;i<(int)Parameterlist.GetCount();i++)
	{
		
		ProParamvalue proval;
		ProCharName str_id;
//		ProCharLine  str_line;
		CString csType;
		CString csValue;
		ProWstringToString(str_id,Parameterlist[i].id);
		ProParameterValueGet(&Parameterlist[i],&proval);

		//indexNo=m_List.InsertItem(0,str_id);
		indexNo=m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List.GetItemCount(),str_id,0,0,0,i);
		switch(proval.type) {
		case PRO_PARAM_DOUBLE:
			csType="浮点型";
			csValue.Format("%f",proval.value.d_val);
			break;
		case PRO_PARAM_STRING:
			csType="字符串";
			//ProWstringToString(str_line,proval.value.s_val);
			csValue=CString(proval.value.s_val);//Format("%c",str_line);
			break;
		case PRO_PARAM_INTEGER:
			csValue.Format("%d",proval.value.i_val);
			csType="整型";
			break;
		case PRO_PARAM_BOOLEAN:
			csType="布尔型";
			csValue.Format("%d",proval.value.l_val);
			break;
		case PRO_PARAM_NOTE_ID:
			csType="注释号";
			csValue="PRO_PARAM_NOTE_ID";
			break;
		case PRO_PARAM_VOID:
			csType="无类型";
			csValue="PRO_PARAM_VOID";
			break;
		default:
			csType="默认";
			csValue="默认";
			break;
		}
        m_List.SetItemText(indexNo,1,csType);
        m_List.SetItemText(indexNo,2,csValue);
        for (int j=0;j<(int)p_UsrSWTXTableList.GetCount();j++)
        {
			if(p_UsrSWTXTableList.GetAt(j).type!="para") continue;
			if(p_UsrSWTXTableList.GetAt(j).owner!=current_solid) continue;
			if(ComparaParameter(&Parameterlist[i],&p_UsrSWTXTableList.GetAt(j).Para))
			{
				ListView_SetCheckState(m_List,indexNo,TRUE);
			}
        }
	}
}

void CRelParaEditDlg::OnBnClickedCom3()
{
	// TODO: 在此添加控件通知处理程序代码
	radioFalg=FEAT_CHECKED;
}

void CRelParaEditDlg::OnBnClickedCom4()
{
	// TODO: 在此添加控件通知处理程序代码
	radioFalg=COMP_CHECKED;
}
/************************************************************************/
/* 函数：AddDimToSWTXList(void)
   说名：在尺寸显示模型式下，把选中的尺寸添加到事物特性表
         添加中要进行判断，改项是否已经在左边存在                       */
/************************************************************************/
void CRelParaEditDlg::AddDimToSWTXList(void)
{	
    int         num; 
	CString     id;
	CString     error_dimlist="";
	CString     error_info="";
	//尺寸项信息所需的变量

	num=(int)p_UsrSWTXTableList.GetCount();

		//对数据进行过滤，去掉不是单前模型，以及已经被删除的数据
		//不是尺寸的

	for(int j=0;j<m_List.GetItemCount();j++)
		{
			//如果列表中的J项没有被选中，继续循环
			if(!ListView_GetCheckState(m_List,j))  continue;
			id.Format("d%d",Dimensionlist[(int)m_List.GetItemData(j)].id);	
			UsrSWTXTable1 temp_UsrSWTXTable(current_solid,&Dimensionlist[m_List.GetItemData(j)]);
            int flag=0;
			for(int i=0;i<num;i++)
			{
				if(p_UsrSWTXTableList[i].owner!=(ProMdl)current_solid) continue;
				if(p_UsrSWTXTableList[i].isCheck!=1)                   continue;
				if(p_UsrSWTXTableList[i].type!="dim")				continue;
				if(ComparaDimension(&p_UsrSWTXTableList[i].Dim,&Dimensionlist[m_List.GetItemData(j)]))  
					{flag=1;break;}
			}
			if (!g_RelCollection.IsDimParaInRelSetLeftPart(current_solid,NULL,&Dimensionlist[m_List.GetItemData(j)]))
			{
				error_dimlist+=temp_UsrSWTXTable.Name+" ";
				flag=1;
			}
			if(flag==1) continue;
			//获取选中项的信息
			//建立一个临时UsrSWTXTable
            
			p_UsrSWTXTableList.Add(temp_UsrSWTXTable);        //添加到数组中

		}
	if (!error_dimlist.IsEmpty())
	{
		error_info.Format("尺寸:%s,在关系式中已经作为被驱动参数(等式关系式左侧)\n,不能够作为事物特性表添加!请修改关系式后,再试!\0",error_dimlist);
		AfxMessageBox(error_info);
	}
	//刷新列表框2
	CString text;
	m_SWTXButtong.GetWindowText(text);
	if (text==_T("<")) {
		swtx_dlg.ShowSWTXList(current_solid);
	}	
}
/************************************************************************/
/* 函数：AddParaToSWTXList(void)
   说明：把参数添加到事物特性表                                          */
/************************************************************************/
void CRelParaEditDlg::AddParaToSWTXList(void)
{
	int					num;                     //记录自定义结构p_UsrSWTXTableList的数据大小
	CString				id;
	CString				csValue;
	CString				csType;
	ProParamvalue		proval;   
	CString				error_paralist="";
	CString				error_info;
	num=(int)p_UsrSWTXTableList.GetCount();

	for(int j=0;j<m_List.GetItemCount();j++)
	{
															//如果列表中的J项没有被选中，继续循环
		if(!ListView_GetCheckState(m_List,j))  continue;
															//把选中的项，与所有的参数进行比较，如果标识号相同，则确定被选中的项
		id=CString(Parameterlist[(int)m_List.GetItemData(j)].id);

															//排除已经存在的参数

		UsrSWTXTable1 temp_UsrSWTXTable(current_solid,&Parameterlist[m_List.GetItemData(j)]);
		int flag=0;
		for(int i=0;i<num;i++)
		{
			if(p_UsrSWTXTableList[i].owner!=(ProMdl)current_solid) continue;   //排除不是当前模型的参数
			if(p_UsrSWTXTableList[i].isCheck!=1) continue;             //排除已经被删除的参数
			if(p_UsrSWTXTableList[i].type!="para") continue;
			if(ComparaParameter(&Parameterlist[m_List.GetItemData(j)],&p_UsrSWTXTableList[i].Para))  {flag=1;break;} 
		}
        //判断参数是否已经在关系的左侧
		if (!g_RelCollection.IsDimParaInRelSetLeftPart(current_solid,&Parameterlist[m_List.GetItemData(j)],NULL))
		{
			error_paralist+=temp_UsrSWTXTable.Name+" ";
			flag=1;
		}
		if(flag==1) continue;
															//获取选中项的信息
        if(flag==1) continue;
		ProParameterValueGet(&Parameterlist[m_List.GetItemData(j)],&proval);
        if(proval.type==PRO_PARAM_STRING && CString(proval.value.s_val).IsEmpty()) 
		{
			AfxMessageBox(_T("参数“"+id+"”的值为空,不能添加"));
			continue;
		}
		p_UsrSWTXTableList.Add(temp_UsrSWTXTable);			 //添加到数组中

	}	
	if (!error_paralist.IsEmpty())
	{
		error_info.Format("参数:%s,在关系式中已经作为被驱动参数(等式关系式左侧)\n,不能够作为事物特性表添加!请修改关系式后,再试!",error_paralist);
		AfxMessageBox(error_info);
	}//刷新列表框2
	//ShowUsrSWTXTableList(current_solid);
	CString			text;
	m_SWTXButtong.GetWindowText(text);
	if (text==_T("<")) {
		swtx_dlg.ShowSWTXList(current_solid);
	}	
}

void CRelParaEditDlg::AddFeatToSWTXList(void)
{
}

void CRelParaEditDlg::AddCompToSWTXList(void)
{
}
/************************************************************************/
/*函数:ConverthItemOfParaToList
  说明：访问模型的参数                                                  */
/************************************************************************/
void CRelParaEditDlg::ConverthItemOfParaToList(void)
{
    Parameterlist.RemoveAll();
	ProModelitem modelitem;
    ProMdlToModelitem(current_solid,&modelitem);
	ProParameterVisit(&modelitem,NULL,(ProParameterAction)partParaVisAc,
		(CArray<ProParameter,ProParameter>*)&Parameterlist);
}


/************************************************************************/
/* 对参数进行编辑                                                       */
/************************************************************************/
void CRelParaEditDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码	
	m_ParaOperaDlg.DoModal();

}
/************************************************************************/
/* 函数：OnBnClickedCheck1()
   说明：判断CHECK按钮的状态                                            */
/************************************************************************/
void CRelParaEditDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
}
/************************************************************************/
/* 函数：OnContextMenu（）
   说明：右键菜单,首先是在事物特性表中的菜单                            */
/************************************************************************/
void CRelParaEditDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此添加消息处理程序代码
	CString   text;
	m_RELATIONButton.GetWindowText(text);
	if((WindowFromPoint(point)==&m_List))
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(4);
		ASSERT(pPopup!=NULL);
		if(text!=_T("<"))			pPopup->EnableMenuItem(ID_12058,MF_DISABLED|MF_GRAYED);
		if(radioFalg!=DIM_CHECKED)	pPopup->EnableMenuItem(ID_12076,MF_DISABLED|MF_GRAYED);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	}
}

/*****************************************************************************\
关闭对话框之后，激活窗口
\*****************************************************************************/
void CRelParaEditDlg::OnClose()
{	
	CDialog::OnClose();
}
/************************************************************************/
/* 函数：OnBnClickedButton1()
   说明：打开关系式编辑*/
/************************************************************************/
void CRelParaEditDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		text;
	CRect		rect;
	CRect		rect1;
	CRect		rect2;

	m_SWTXButtong.GetWindowText(text);
	if (text=="<") {
		//关闭
		GetWindowRect(rect);
		swtx_dlg.GetWindowRect(&rect2);
		rect.right=rect.right-rect2.Width();
		swtx_dlg.DestroyWindow();
		MoveWindow(&rect,TRUE);
		m_SWTXButtong.SetWindowText(_T(">"));
	}

	m_RELATIONButton.GetWindowText(text);
	if (text=="<") {
		GetWindowRect(rect);
		rel_dlg.GetWindowRect(&rect2);
		rect.right=rect.right-rect2.Width();
		rel_dlg.DestroyWindow();
		MoveWindow(&rect,TRUE);
		m_RELATIONButton.SetWindowText(_T(">"));
		return;
	}

	//打开
	GetWindowRect(rect);
	GetClientRect(rect1);
	rel_dlg.SetMdl(current_solid);
	rel_dlg.Create(IDD_REL,this);	
	rel_dlg.GetWindowRect(&rect2);
	rect.right=rect.right+rect2.Width();
	MoveWindow(&rect,TRUE);
	rel_dlg.MoveWindow(rect1.right,0,rect2.Width(),rect.Height(),TRUE);
	rel_dlg.ShowWindow(SW_SHOW);
	m_RELATIONButton.SetWindowText(_T("<"));
}
/************************************************************************/
/* 右键菜单，把尺寸，关系，添加到关系式中去                             */
/************************************************************************/
void CRelParaEditDlg::On12058()
{
	// TODO: 在此添加命令处理程序代码
    switch(radioFalg) {
    case DIM_CHECKED:
		for(int i=0;i<m_List.GetItemCount();i++)
		{
			if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;
			rel_dlg.m_RelEditCtrl.ReplaceSel(m_List.GetItemText(i,0),FALSE);
		}
    	break;
    case PARA_CHECKED:
		for(int i=0;i<m_List.GetItemCount();i++)
		{
			if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;
			rel_dlg.m_RelEditCtrl.ReplaceSel(m_List.GetItemText(i,0),FALSE);
		}
    	break;
    default:
		break;
    }	
}

/************************************************************************/
/* 刷新m_List,使新添加的参数和删除的参数得到更新                        */
/************************************************************************/
void CRelParaEditDlg::On12059()
{
	// TODO: 在此添加命令处理程序代码
	switch(radioFalg) {
	case DIM_CHECKED:
		//ConverthItemToList(hItem);
		FreshDim();
		break;
	case PARA_CHECKED:
        OnBnClickedCom2();
		break;
	case FEAT_CHECKED:
		break;
	case COMP_CHECKED:
		break;
	default:
		break;
	}
}
/************************************************************************/
/* 右键菜单中对尺寸的数值，符号，上下偏差等进行修改                     */
/************************************************************************/
void CRelParaEditDlg::OnDimEditDlg()
{
	// TODO: 在此添加命令处理程序代码
    CDimEditDlg dlg;	
	for( int i=0;i<m_List.GetItemCount();i++)
	{
		//选择被选中的项
		if(m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
            ProBoolean      rel_driven; 
			ProDimensionIsReldriven (&Dimensionlist[m_List.GetItemData(i)],&rel_driven);
			if(rel_driven==PRO_B_TRUE) 
			{AfxMessageBox("尺寸为关系式驱动，不能修改！");
			return;}
            //把相等的尺寸进行修改
			dlg.m_ID.Format("d%d",Dimensionlist[m_List.GetItemData(i)].id);
			dlg.m_Symbol=m_List.GetItemText(i,0);
			dlg.m_Value=(float)atof(m_List.GetItemText(i,2).GetBuffer());
			dlg.m_UpLimit=(float)atof(m_List.GetItemText(i,3).GetBuffer());
			dlg.m_DwLimit=(float)atof(m_List.GetItemText(i,4).GetBuffer());

			if(dlg.DoModal()==IDOK)
			{

				ProName symbol;
				ProStringToWstring(symbol,dlg.m_Symbol.GetBuffer());
				if(ProDimensionSymbolSet(&Dimensionlist[m_List.GetItemData(i)],symbol)!=PRO_TK_NO_ERROR) AfxMessageBox("失败！");
				ProDimensionValueSet(&Dimensionlist[m_List.GetItemData(i)],dlg.m_Value);
				ProDimensionToleranceSet(&Dimensionlist[m_List.GetItemData(i)],dlg.m_UpLimit,dlg.m_DwLimit);
			}			
			break;
		}
	}
    //刷新
	ProSolid		solid;
	ProMdlCurrentGet((ProMdl*)&solid);
	ProSolidRegenerate((ProSolid)solid,PRO_REGEN_NO_FLAGS);
    FreshDim();	
}
/************************************************************************/
/* 右键菜单中，对列表框的尺寸进行刷新                                   */
/************************************************************************/
void CRelParaEditDlg::FreshDim(void)
{
	int					num=0;
	ProName				symbol;
	ProCharName			cSymbol;
	double				value;
	CString				csvalue;
	ProDimensiontype	type;
	CString				csType;
	int					indexNo=0;
	double				upper_limit,lower_limit;	
	m_List.DeleteAllItems();

	for(int i=0;i<6;i++)
	{
		m_List.DeleteColumn(0);
	}

	m_List.InsertColumn(0,"符号",LVCFMT_LEFT,100,0);
	m_List.InsertColumn(1,"类型",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(2,"值",LVCFMT_LEFT,80,0);
	m_List.InsertColumn(3,"上偏差",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(4,"下偏差",LVCFMT_LEFT,50,0);
	
	num=(int)Dimensionlist.GetCount();
	for(int i=0;i<num;i++)
	{
		//id.Format("d%d",Dimensionlist[i].id);

		ProDimensionSymbolGet(&Dimensionlist[i],symbol);
		ProWstringToString(cSymbol,symbol);

		ProDimensionValueGet(&Dimensionlist[i],&value);
		csvalue.Format("%f",value);

		ProDimensionTypeGet(&Dimensionlist[i],&type);
		switch(type) {
		case PRODIMTYPE_LINEAR:
			csType="线性";
			break;
		case PRODIMTYPE_RADIUS:
			csType="半径";
			break;
		case PRODIMTYPE_DIAMETER:
			csType="直径";
			break;
		case PRODIMTYPE_ANGLE:
			csType="角度";
			break;
		default:
			csType="未知";
			break;
		}
		ProDimensionToleranceGet(&Dimensionlist[i],&upper_limit,&lower_limit);

		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,m_List.GetItemCount(),cSymbol,0,0,0,i);
		m_List.SetItemText(indexNo,1,csType);
		m_List.SetItemText(indexNo,2,csvalue);
		CString upper_limit_value;
		CString lower_limit_value;
		upper_limit_value.Format("%f",upper_limit);
		lower_limit_value.Format("%f",lower_limit);
		m_List.SetItemText(indexNo,3,_T(upper_limit_value));
		m_List.SetItemText(indexNo,4,_T(lower_limit_value));
		for (int j=0;j<(int)p_UsrSWTXTableList.GetCount();j++)
		{
			if(p_UsrSWTXTableList[j].type!="dim") continue;
			if(p_UsrSWTXTableList[j].owner !=current_solid)  continue;
			if(ComparaDimension(&Dimensionlist[i],&p_UsrSWTXTableList[j].Dim))
			{
				ListView_SetCheckState(m_List,indexNo,TRUE);
			}
		}
	}
}

void CRelParaEditDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (m_FeatDisplayMode.GetState()==1)
	//{
	//	AfxMessageBox("选中");
	//}
	//m_FeatDisplayMode.SetCheck();
}
