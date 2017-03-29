// AsmMdlTreePage.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "AsmMdlTreePage.h"
#include "PartInsTableSet.h"
// CAsmMdlTreePage 对话框

IMPLEMENT_DYNAMIC(CAsmMdlTreePage, CPropertyPage)
CAsmMdlTreePage::CAsmMdlTreePage()
	: CPropertyPage(CAsmMdlTreePage::IDD)
	, m_CompName(_T(""))
	, m_CompCode(_T(""))
	, m_CompFileName(_T(""))
	, m_CompType(_T(""))
	, m_CompNote(_T(""))
	, mdl(NULL)
	, tempPartPorp(NULL)
	, selNum(-1)
	, m_DefInsName(_T(""))
	, m_DefInsCode(_T(""))
	, list_dlg(NULL)
	, p_db(NULL)
{
}

CAsmMdlTreePage::~CAsmMdlTreePage()
{
}

void CAsmMdlTreePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Text(pDX, IDC_EDIT1, m_CompName);
	DDX_Text(pDX, IDC_EDIT2, m_CompCode);
	DDX_Text(pDX, IDC_EDIT3, m_CompFileName);
	DDX_Text(pDX, IDC_EDIT4, m_CompType);
	DDX_Text(pDX, IDC_EDIT5, m_CompNote);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
	DDX_Text(pDX, IDC_EDIT6, m_DefInsName);
	DDX_Text(pDX, IDC_EDIT7, m_DefInsCode);
}


BEGIN_MESSAGE_MAP(CAsmMdlTreePage, CPropertyPage)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CAsmMdlTreePage 消息处理程序

BOOL CAsmMdlTreePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	imageList.Create(12, 12, ILC_COLOR16 , 4, 4);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_CSYS));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_DTM));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_FEAT));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_LINE));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_POT));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_Tree.SetImageList(&imageList,TVSIL_NORMAL);
	if (mdl!=NULL) {
		int   indexNo=0;
		/*IsMdlInfoDefAndDef(mdl);*/
		m_Tree.DeleteAllItems();
		GetMdlProp(mdl,&indexNo);
		HTREEITEM hItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM
			,p_UsrPartPorp[indexNo].Name
			,0,0,0,0,-1,NULL,NULL);
		ShowMdlTree(mdl,hItem);
		m_Tree.Expand(m_Tree.GetRootItem(),TVE_EXPAND );
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 显示装配件的模型树                                                   */
/************************************************************************/
int CAsmMdlTreePage::ShowMdlTree(ProSolid solid,HTREEITEM hItem)
{
	ProError      status;
	HTREEITEM     tempItem;
	ProMdlType    type;
	int           imageNum=0;
	int           indexNo=0;
	CArray<ProFeature,ProFeature> feat_list;
	status=ProSolidFeatVisit(solid,(ProFeatureVisitAction)FeatureAsmVisitAcFn
		,NULL,(CArray<ProFeature,ProFeature>*)&feat_list);
	int num=(int)feat_list.GetCount();
	for (int i=0;i<num;i++) {
		ProFeattype    ftype;
		status = ProFeatureTypeGet (&feat_list[i], &ftype);

		if (ftype == PRO_FEAT_COMPONENT){
			ProSolid              mdl;
			ProAsmcompMdlGet((ProAsmcomp *)(&feat_list[i]),(ProMdl *)&mdl);
			/*IsMdlInfoDefAndDef(mdl);*/
			GetMdlProp(mdl,&indexNo);
			ProMdlTypeGet(mdl,&type);
			switch(type) {
			case PRO_MDL_ASSEMBLY:
				imageNum=0;
				break;
			case PRO_MDL_PART:
				imageNum=6;
				break;
			default:

				break;
			}			
			tempItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM,p_UsrPartPorp[indexNo].Name
				,imageNum,imageNum,0,0,indexNo,hItem,NULL);				//设置模型的基本信息			
			ShowMdlTree(mdl,tempItem);
		}
	}
	return 0;
}
/************************************************************************/
/* 点击模型树的动作                                                     */
/************************************************************************/
void CAsmMdlTreePage::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//// TODO: 在此添加控件通知处理程序代码
	//int     item=-1;
	//HTREEITEM hItem=m_Tree.GetSelectedItem();
	//if(hItem==m_Tree.GetRootItem()) return;
	//selNum=(int)m_Tree.GetItemData(hItem);
	//ConvertUsrPropNumToCompInsItemNum(selNum,&item);               //根据模型号得到模型变形范围的号
	//m_CompName=p_UsrPartPorp[selNum].Name;
	//m_CompCode=p_UsrPartPorp[selNum].Code;
	//m_CompFileName=p_UsrPartPorp[selNum].FileName;
	//m_CompType=p_UsrPartPorp[selNum].FileName.Find(".asm",0)==-1?_T("零件"):_T("组件");
 //   m_CompNote=p_UsrPartPorp[selNum].Note;
	//if(m_Combo.GetCount()!=0) 
	//	m_Combo.ResetContent();
	//if (item!=-1)
	//{
	//	m_DefInsName=p_AsmCompItemList[item].varFeild->GetAt(0).insName;
	//	m_DefInsCode=p_AsmCompItemList[item].varFeild->GetAt(0).insCode;
	//	for (int i=0;i<(int)p_AsmCompItemList[item].varFeild->GetCount();i++)
	//	{
	//		m_Combo.AddString(p_AsmCompItemList[item].varFeild->GetAt(i).insName);
	//		m_Combo.SetCurSel(0);
	//	}
	//}
	//else
	//{
	//	m_DefInsCode="";
	//	m_DefInsName="";
	//}
	//UpdateData(FALSE);
	*pResult = 0;
}

void CAsmMdlTreePage::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(selNum==-1)
	{
		AfxMessageBox("没有选中元件，没法修改");
		return;
	}
	UpdateData(TRUE);
	p_UsrPartPorp[selNum].Name=m_CompName;
	p_UsrPartPorp[selNum].Code=m_CompCode;
	p_UsrPartPorp[selNum].Note=m_CompNote;
	if (mdl!=NULL) {
		int   indexNo=0;
		m_Tree.DeleteAllItems();
		GetMdlProp(mdl,&indexNo);
		HTREEITEM hItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM
			,p_UsrPartPorp[indexNo].Name
			,0,0,0,0,-1,NULL,NULL);
		ShowMdlTree(mdl,hItem);
		m_Tree.Expand(m_Tree.GetRootItem(),TVE_EXPAND );
	}
}
/************************************************************************/
/* 确定实例的变形范围                                                   */
/************************************************************************/
void CAsmMdlTreePage::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CPartInsTableSet m_Set(p_db);
	int              item=-1;
	if(list_dlg==NULL) {list_dlg=new CListDlg();list_dlg->m_strText="设置模型的可变范围";list_dlg->Create(IDD_LIST_DLG,this);}	
    else
	{
		delete list_dlg;
		list_dlg=new CListDlg();list_dlg->m_strText="设置模型的可变范围";list_dlg->Create(IDD_LIST_DLG,this);
	}
	list_dlg->m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	list_dlg->m_List.InsertColumn(0,"名称",LVCFMT_LEFT,100,0);
	list_dlg->m_List.InsertColumn(1,"编码",LVCFMT_LEFT,100,0);
    list_dlg->m_List.InsertColumn(2,"注释",LVCFMT_LEFT,100,0);
	if (p_db!=NULL && selNum!=-1 && ConvertUsrPropNumToCompInsItemNum(selNum,&item))
	{	

		list_dlg->ShowWindow(SW_SHOW);
		m_Set.m_strFilter.Format("[PrtNum]=%d",p_AsmCompItemList[item].mdlNum);
		m_Set.Open();
		if (!m_Set.IsBOF())
		{
		    m_Set.MoveFirst();
			do {
				list_dlg->m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,list_dlg->m_List.GetItemCount(),m_Set.m_InsName
					,0,0,0,m_Set.m_InsNum);
				list_dlg->m_List.SetItemText(list_dlg->m_List.GetItemCount()-1,1,m_Set.m_InsCode);
				list_dlg->m_List.SetItemText(list_dlg->m_List.GetItemCount()-1,2,m_Set.m_InsNote);
				//if(m_Set.m_InsNum==p_AsmCompItemList[item].insNum) ListView_SetCheckState(list_dlg->m_List.m_hWnd,list_dlg->m_List.GetItemCount()-1,TRUE);
				   for (int i=0;i<(int)p_AsmCompItemList[item].varFeild->GetCount();i++)
				   {
					   if (m_Set.m_InsNum==p_AsmCompItemList[item].varFeild->GetAt(i).insNum)
					   {
						   ListView_SetCheckState(list_dlg->m_List.m_hWnd,list_dlg->m_List.GetItemCount()-1,TRUE);
						   break;
					   }
				   }
				m_Set.MoveNext();
			} while(!m_Set.IsEOF());
		}
	}
}
/************************************************************************/
/* 消息处理函数，用于选择好系列范围之后的                               */
/************************************************************************/
void CAsmMdlTreePage::OnListDlgOk(WPARAM wParam,LPARAM lParam)
{
	int   item=-1;
	if (selNum!=-1)
	{
		//保留第一个
		ConvertUsrPropNumToCompInsItemNum(selNum,&item);      //根据模型号得到模型变形范围的号
		if (item==-1) return ;
		while ((int)p_AsmCompItemList[item].varFeild->GetCount()>1)
		{
			p_AsmCompItemList[item].varFeild->RemoveAt((int)p_AsmCompItemList[item].varFeild->GetCount()-1);
		}
		p_AsmCompItemList[item].varFeild->FreeExtra();

		for (int i=0;i<(int)list_dlg->m_List.GetItemCount();i++)
		{
			if((int)list_dlg->m_List.GetItemData(i)==p_AsmCompItemList[item].insNum) continue;
			if (ListView_GetCheckState(list_dlg->m_List.m_hWnd,i))
			{
				CompInsItem temp;
				temp.insCode=list_dlg->m_List.GetItemText(i,1);
				temp.insNum=(int)list_dlg->m_List.GetItemData(i);
				temp.insName=list_dlg->m_List.GetItemText(i,0);
				temp.insNote=list_dlg->m_List.GetItemText(i,2);
				p_AsmCompItemList[item].varFeild->Add(temp);
			}
		}
	}
}