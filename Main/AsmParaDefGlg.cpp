// AsmParaDefGlg.cpp : 实现文件
//定义组件模型的可变参数表

#include "stdafx.h"
#include "Main.h"
#include "AsmParaDefGlg.h"
#include "DimEditDlg.h"
#include "ParaOperaDlg.h"
#include "SwtxTableItemEditDlg.h"
// CAsmParaDefGlg 对话框

IMPLEMENT_DYNAMIC(CAsmParaDefDlg2, CDialog)
CAsmParaDefDlg2::CAsmParaDefDlg2(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmParaDefDlg2::IDD, pParent)
	, m_flag(-1)
{
}

CAsmParaDefDlg2::~CAsmParaDefDlg2()
{
}

void CAsmParaDefDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Control(pDX, IDC_LIST1, m_List1);
	DDX_Control(pDX, IDC_LIST2, m_List2);
	DDX_Control(pDX, IDC_PATH, m_Path);
}


BEGIN_MESSAGE_MAP(CAsmParaDefDlg2, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_12076, OnEditDim)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, OnNMClickList2)
	ON_COMMAND(ID_12059, OnFreshList)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CAsmParaDefGlg 消息处理程序

BOOL CAsmParaDefDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (ProMdlCurrentGet((ProMdl*)&asm_solid)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("当前不存在模型");
		OnClose();
		return FALSE;
	}

	m_List1.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
    m_List2.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ImageList.Create(12, 12, ILC_COLOR16 , 4, 4);
	SetImageListForFeatrure(&m_ImageList);

	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	p_treeitemlist.FreeExtra();

	tree_item temp_item;
	temp_item.type=-1;
	temp_item.p_solid=asm_solid;
	temp_item.f_solid=NULL;

	HTREEITEM hItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM
		,GetMdlInfo(asm_solid).Name+"("+GetMdlFileName(asm_solid)+")",0,0,0,0,p_treeitemlist.Add(temp_item),NULL,NULL);

	ShowModelTree(asm_solid,hItem);

    m_Tree.Expand(hItem,TVE_EXPAND);


	//设置模型事务特性表
	ShowAsmSWTXList2(asm_solid,m_List2);
	m_Tree.SelectItem(m_Tree.GetRootItem());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CAsmParaDefDlg2::ShowModelTree(ProSolid solid,HTREEITEM hItem)
{
	ProError       status;
	CString        csName;
	ProName        wName;
	HTREEITEM      tempItem;
	ProMdlType     type;
	ProFeattype    ftype;
	int            imageNum=0;
	tree_item	   temp_item;

	CArray<ProFeature,ProFeature> feat_list;
	status=ProSolidFeatVisit(solid,(ProFeatureVisitAction)FeatureAsmVisitAcFn
		,NULL,(CArray<ProFeature,ProFeature>*)&feat_list);
	int num=(int)feat_list.GetCount();
	for (int i=0;i<num;i++) {
		status = ProFeatureTypeGet (&feat_list[i], &ftype);
		if (ftype == PRO_FEAT_COMPONENT){
			ProSolid              mdl;
			status=ProAsmcompMdlGet((ProAsmcomp *)(&feat_list[i]),(ProMdl *)&mdl);
			status=ProMdlNameGet(mdl,wName);
			csName=CString(wName);
			status=ProMdlTypeGet(mdl,&type);
			switch(type) {
			case PRO_MDL_ASSEMBLY:
				imageNum=0;
				temp_item.type=0;
				csName=csName+".ASM";
				break;
			case PRO_MDL_PART:
				imageNum=6;
				temp_item.type=1;
				csName=csName+".PRT";
				break;
			default:
				break;
			}
			//设置模型的基本信息	

			temp_item.p_solid=mdl;
			temp_item.f_solid=solid;	
			temp_item.feature=feat_list[i];

			tempItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM
				,GetMdlInfo(mdl).Name+"("+GetMdlFileName(mdl)+")",imageNum,imageNum,0,0,p_treeitemlist.Add(temp_item),hItem,NULL);
		    if (!IsSolidFromMdl(mdl))
		    {
				ShowModelTree(mdl,tempItem);
		    }
			else
			{
				temp_item.type=2;
				m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM
					,"可变参数表",3,3,0,0,p_treeitemlist.Add(temp_item),tempItem,NULL);
			}
			/*GetAsmcompConstraint(tempItem,(ProAsmcomp *)(&feat_list[i]));*/
			temp_item.type=3;
			m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM
				,"装配尺寸",3,3,0,0,p_treeitemlist.Add(temp_item),tempItem,NULL);
	    }
	}
	feat_list.FreeExtra();
}
//判断实体是否从模型过来的
bool CAsmParaDefDlg2::IsSolidFromMdl(ProSolid solid)
{
	UsrPartPorp temp;
	CString     name=GetMdlFileName(solid);
	temp=GetMdlInfo(solid);
	//如果是有模型的实例以及是模型，才会添加事务特性表
	if (temp.MdlType==1 || temp.MdlType==3)
	    return true;

	return false;
}
//获得元件约束关系
void CAsmParaDefDlg2::GetAsmcompConstraint(HTREEITEM hItem,ProAsmcomp * p_component)
{
	//ProAsmcompconstraint *p_constraints ;
	//ProAsmcompConstraintsGet(p_component,&p_constraints);
}

void CAsmParaDefDlg2::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM	hItem=m_Tree.GetSelectedItem();
	int			index=(int)m_Tree.GetItemData(hItem);
	switch(p_treeitemlist.GetAt(index).type) {
	case -1:
		ShowAsmSolidPara(asm_solid);
		break;
	case 0:
		break;
	case 1:
		break;
	case 2:    //事务特性表
		ShowSolidSWTX(p_treeitemlist.GetAt(index).p_solid);
		break;
	case 3:    //装配约束尺寸
		ShowFeatDim(p_treeitemlist.GetAt(index).feature);
		break;
	default:
		break;
	}
	*pResult = 0;
}

//在列表里面显示模型的事务特性表
void CAsmParaDefDlg2::ShowSolidSWTX(ProSolid solid)
{
	switch(GetMdlType(solid)) {
	case PRO_MDL_PART:
		ShowPartSWTX(solid);
		break;
	case PRO_MDL_ASSEMBLY:
		ShowAsmSWTXList1(solid,m_List1);
		break;
	default:
		break;
	}
}

void CAsmParaDefDlg2::ShowPartSWTX(ProSolid part)
{
	int			indexNo=0,postid=0;
    list_item	temp_item;
	CString		csPostId;

	m_List1.DeleteAllItems();
	do {		
	} while(m_List1.DeleteColumn(0));	
	m_List1.InsertColumn(0,"名称",LVCFMT_LEFT,60);                  //重新命名得到，默认为尺寸符号和参数和特征的名称
	m_List1.InsertColumn(1,"符号",LVCFMT_LEFT,60);                  //尺寸的符号，以及参数和特征的名称
	m_List1.InsertColumn(2,"类型",LVCFMT_LEFT,60);
	m_List1.InsertColumn(3,"子类型",LVCFMT_LEFT,60);
	m_List1.InsertColumn(4,"值",LVCFMT_LEFT,60);
	m_List1.InsertColumn(5,"上偏差",LVCFMT_LEFT,60);
	m_List1.InsertColumn(6,"下偏差",LVCFMT_LEFT,60);
	m_List1.InsertColumn(7,"变型规则",LVCFMT_LEFT,100);

    p_listitem.RemoveAll();

	for(int i=0;i<(int)p_UsrSWTXTableList.GetCount();i++)
	{
		//更新事物特性表项,
		p_UsrSWTXTableList[i].Update();
		if(p_UsrSWTXTableList[i].isCheck!=1)	 continue;
		if(p_UsrSWTXTableList[i].owner!=part)		continue;
        ProSolidToPostfixId(part,&postid);
		csPostId.Format(":%d",postid);

		indexNo=m_List1.InsertItem(LVIF_TEXT|LVIF_PARAM
			,m_List1.GetItemCount(),p_UsrSWTXTableList[i].Name,0,0,0,(int)p_listitem.Add(temp_item));

		p_listitem[indexNo].name=p_UsrSWTXTableList[i].Name;
		p_listitem[indexNo].note=p_UsrSWTXTableList[i].Note;
		//对于符号要进行区分，如果是尺寸则用符号，如果是参数和特征，则用名称
		if(p_UsrSWTXTableList[i].type=="dim")
		{
			p_listitem[indexNo].type=TYPE_DIM;
			p_listitem[indexNo].Dim=p_UsrSWTXTableList[i].Dim;
			m_List1.SetItemText(indexNo,1,p_UsrSWTXTableList[i].dimSymbol+csPostId);
			m_List1.SetItemText(indexNo,2,"尺寸");
		}
		else if (p_UsrSWTXTableList[i].type=="para") {
			p_listitem[indexNo].type=TYPE_PARA;
			p_listitem[indexNo].Para=p_UsrSWTXTableList[i].Para;
			m_List1.SetItemText(indexNo,1,p_UsrSWTXTableList[i].featName+csPostId);
			m_List1.SetItemText(indexNo,2,"参数");
		}

		//判断子类型，尺寸的、参数的、特征的、模型的
		//以下关于尺寸
		if(p_UsrSWTXTableList[i].subType=="PRODIMTYPE_LINEAR")
		{
			m_List1.SetItemText(indexNo,3,"线性");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRODIMTYPE_RADIUS") {
			m_List1.SetItemText(indexNo,3,"半径");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRODIMTYPE_DIAMETER") {
			m_List1.SetItemText(indexNo,3,"直径");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRODIMTYPE_ANGLE") {
			m_List1.SetItemText(indexNo,3,"角度");
		}
		//以下关于参数
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_DOUBLE") {
			m_List1.SetItemText(indexNo,3,"浮点型");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_STRING") {
			m_List1.SetItemText(indexNo,3,"字符串");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_INTEGER") {
			m_List1.SetItemText(indexNo,3,"整数");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_BOOLEAN") {
			m_List1.SetItemText(indexNo,3,"布尔型");
		}
		//下面两种需要进一步考虑
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_NOTE_ID") {
			m_List1.SetItemText(indexNo,3,"注释号");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_VOID") {
			m_List1.SetItemText(indexNo,3,"无类型");
		}
		else{
			//对于特征直接用特征类型名ProFeatureTypenameGet()获得
			m_List1.SetItemText(indexNo,3,p_UsrSWTXTableList[i].subType);
		}
		m_List1.SetItemText(indexNo,4,p_UsrSWTXTableList[i].value);
		m_List1.SetItemText(indexNo,5,p_UsrSWTXTableList[i].UpLimit);
		m_List1.SetItemText(indexNo,6,p_UsrSWTXTableList[i].DwLimit);
		m_List1.SetItemText(indexNo,7,p_UsrSWTXTableList[i].Note);
		for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) 
		{
			if(p_AsmSmlItemList[j].owner!=asm_solid) continue;
			if(p_AsmSmlItemList[j].marsk!=p_listitem[indexNo].type /*p_UsrSWTXTableList[i].type*/) continue;
			switch(p_AsmSmlItemList[j].marsk) {
			case TYPE_DIM:
				if(ComparaDimension(&p_AsmSmlItemList[j].dim,&p_UsrSWTXTableList[i].Dim)) ListView_SetCheckState(m_List1,indexNo,TRUE);
				break;
			case TYPE_PARA:
				if(ComparaParameter(&p_AsmSmlItemList[j].para,&p_UsrSWTXTableList[i].Para)) ListView_SetCheckState(m_List1,indexNo,TRUE);
				break;
			default:
				break;
			}
		}
	}
}
/************************************************************************/
/* 列表框1是在部件模式下的情况                                          */
/************************************************************************/
void CAsmParaDefDlg2::ShowAsmSWTXList1(ProSolid asm1,CListCtrl & p_List)
{
	int               num=0;
	CString           temp;
	ProName           symbol;            //尺寸符号
	double            value;           //尺寸值
	ProDimensiontype  type;            //尺寸类型
	double            upper_limit;     //尺寸的上偏差  
	double            lower_limit;     //尺寸的下偏差 
	int               ii=0;
	int               postId=0;          //零件的后缀
	ProError          status;
	ProParamvalue     proval;
	CString           csType;
	CString           csValue;
	list_item         temp_item;
	p_List.DeleteAllItems();
	do {		
	} while(p_List.DeleteColumn(0));
	//
	p_List.InsertColumn(0,"名称",LVCFMT_LEFT,60);                  //重新命名得到，默认为尺寸符号和参数和特征的名称
	p_List.InsertColumn(1,"符号",LVCFMT_LEFT,60);                  //尺寸的符号，以及参数和特征的名称
	p_List.InsertColumn(2,"类型",LVCFMT_LEFT,60);
	p_List.InsertColumn(3,"子类型",LVCFMT_LEFT,60);
	p_List.InsertColumn(4,"值",LVCFMT_LEFT,60);
	p_List.InsertColumn(5,"上偏差",LVCFMT_LEFT,60);
	p_List.InsertColumn(6,"下偏差",LVCFMT_LEFT,60);
	p_List.InsertColumn(7,"变型规则",LVCFMT_LEFT,100);


	p_List.DeleteAllItems();
	p_listitem.RemoveAll();

	num=(int)p_AsmSmlItemList.GetCount();

	for (int i=0;i<num;i++) {
		int indexNo=0;
		//检查及过滤
		if(p_AsmSmlItemList[i].owner!=asm1) continue;

		if (!CheckAsmSWTX(asm1,&p_AsmSmlItemList[i]))
		{
			p_AsmSmlItemList.RemoveAt(i,1);
			i--;
			num--;
			continue;
		}
		switch(p_AsmSmlItemList[i].marsk) {
		case TYPE_DIM:
			ProSolidToPostfixId((ProSolid)p_AsmSmlItemList[i].dim.owner,&postId);
			status=ProDimensionValueGet(&p_AsmSmlItemList[i].dim,&value);
			status=ProDimensionTypeGet(&p_AsmSmlItemList[i].dim,&type);
			status=ProDimensionSymbolGet(&p_AsmSmlItemList[i].dim,symbol);
			status=ProDimensionToleranceGet(&p_AsmSmlItemList[i].dim,&upper_limit,&lower_limit);

            temp_item.type=TYPE_DIM;
			temp_item.Dim=p_AsmSmlItemList[i].dim;
			indexNo=p_List.InsertItem(LVIF_TEXT|LVIF_PARAM,ii++,p_AsmSmlItemList[i].name,0,0,0,(int)p_listitem.Add(temp_item));
			p_listitem[indexNo].name=p_AsmSmlItemList[i].name;
			p_listitem[indexNo].note=p_AsmSmlItemList[i].note;


			temp.Format("%s:%d",CString(symbol),postId);
			p_List.SetItemText(indexNo,1,temp);
			p_List.SetItemText(indexNo,2,"尺寸");
			switch(type) {
			case PRODIMTYPE_LINEAR:
				p_List.SetItemText(indexNo,3,"线性");
				break;
			case PRODIMTYPE_RADIUS:
				p_List.SetItemText(indexNo,3,"半径");
				break;
			case PRODIMTYPE_DIAMETER:
				p_List.SetItemText(indexNo,3,"直径");
				break;
			case PRODIMTYPE_ANGLE:
				p_List.SetItemText(indexNo,3,"角度");
				break;
			default:
				p_List.SetItemText(indexNo,3,"未知");
				break;
			}
			temp.Format("%f",value);
			p_List.SetItemText(indexNo,4,temp);
			temp.Format("%f",upper_limit);
			p_List.SetItemText(indexNo,5,temp);
			temp.Format("%f",lower_limit);
			p_List.SetItemText(indexNo,6,temp);
			p_List.SetItemText(indexNo,7,p_AsmSmlItemList[i].note);
			ProMdlNameGet(p_AsmSmlItemList[i].dim.owner,symbol);
			p_List.SetItemText(indexNo,8,CString(symbol));
			break;
		case TYPE_PARA:
			ProSolidToPostfixId((ProSolid)UsrGetParameterOwner(&p_AsmSmlItemList[i].para),&postId);
			status=ProParameterValueGet(&p_AsmSmlItemList[i].para,&proval);

			temp_item.type=TYPE_PARA;
			temp_item.Para=p_AsmSmlItemList[i].para;
			indexNo=p_List.InsertItem(LVIF_TEXT|LVIF_PARAM,ii++,p_AsmSmlItemList[i].name,0,0,0,p_listitem.Add(temp_item));
			//m_List.SetItemText(indexNo,1,p_AsmSmlItemList[i].name);
			temp.Format("%s:%d",CString(p_AsmSmlItemList[i].para.id),postId);
			p_List.SetItemText(indexNo,1,temp);
			p_List.SetItemText(indexNo,2,"参数");

			switch(proval.type) {
		case PRO_PARAM_DOUBLE:
			csType="浮点型";
			csValue.Format("%f",proval.value.d_val);
			break;
		case PRO_PARAM_STRING:
			csType="字符串";
			csValue=CString(proval.value.s_val);
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
			p_List.SetItemText(indexNo,3,csType);
			p_List.SetItemText(indexNo,4,csValue);
			p_List.SetItemText(indexNo,7,p_AsmSmlItemList[i].note);
			ProMdlNameGet((p_AsmSmlItemList[i].para.owner.type==PRM_MODEL?p_AsmSmlItemList[i].para.owner.who.model:p_AsmSmlItemList[i].para.owner.who.item.owner),symbol);
			p_List.SetItemText(indexNo,8,CString(symbol));
			break;
		case TYPE_FEAT:
			//if(p_AsmSmlItemList[i].dim.owner!=(ProMdl)solid) continue;
			break;
		case TYPE_COMP:
			//if(p_AsmSmlItemList[i].dim.owner!=(ProMdl)solid) continue;
			break;
		default:
			break;
		}
		//判断是否已经添加在最上层组件的事务特性表中
		for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) 
		{
			if(p_AsmSmlItemList[j].owner!=asm_solid) continue;
			if(p_AsmSmlItemList[j].marsk != p_AsmSmlItemList[i].marsk) continue;

			switch(p_AsmSmlItemList[j].marsk) {
			case TYPE_DIM:
				if(ComparaDimension(&p_AsmSmlItemList[j].dim,&p_AsmSmlItemList[i].dim)) 
				{
					ListView_SetCheckState(p_List,indexNo,TRUE);
				}
				break;
			case TYPE_PARA:
				if(ComparaParameter(&p_AsmSmlItemList[j].para,&p_AsmSmlItemList[i].para))
				{
					ListView_SetCheckState(p_List,indexNo,TRUE);
				}
				break;
			default:
				break;
			}
		}
	}
}
/************************************************************************/
/* 在第二个列表框显示模型的事务特性表                                    */
/************************************************************************/
void CAsmParaDefDlg2::ShowAsmSWTXList2(ProSolid asm1,CListCtrl & p_List)
{
	int               num=0;
	CString           temp;
	ProName           symbol;            //尺寸符号
	double            value;           //尺寸值
	ProDimensiontype  type;            //尺寸类型
	double            upper_limit;     //尺寸的上偏差  
	double            lower_limit;     //尺寸的下偏差 
	int               ii=0;
	int               postId=0;          //零件的后缀
	ProError          status;
	ProParamvalue     proval;
	CString           csType;
	CString           csValue;
	//list_item         temp_item;

	p_List.DeleteAllItems();
	do {		
	} while(p_List.DeleteColumn(0));

	p_List.InsertColumn(0,"名称",LVCFMT_LEFT,60);                  //重新命名得到，默认为尺寸符号和参数和特征的名称
	p_List.InsertColumn(1,"符号",LVCFMT_LEFT,60);                  //尺寸的符号，以及参数和特征的名称
	p_List.InsertColumn(2,"类型",LVCFMT_LEFT,60);
	p_List.InsertColumn(3,"子类型",LVCFMT_LEFT,60);
	p_List.InsertColumn(4,"值",LVCFMT_LEFT,60);
	p_List.InsertColumn(5,"上偏差",LVCFMT_LEFT,60);
	p_List.InsertColumn(6,"下偏差",LVCFMT_LEFT,60);
	p_List.InsertColumn(7,"变型规则",LVCFMT_LEFT,100);
	p_List.InsertColumn(8,"所属元件",LVCFMT_LEFT,100);


	p_List.DeleteAllItems();

	/*if(p_List==m_List1) p_listitem.RemoveAll();*/
	//p_listitem2.RemoveAll();

	num=(int)p_AsmSmlItemList.GetCount();

	for (int i=0;i<num;i++) {
		int indexNo=0;
		//检查及过滤
		if(p_AsmSmlItemList[i].owner!=asm1) continue;
		if (!CheckAsmSWTX(asm1,&p_AsmSmlItemList[i]))
		{
			p_AsmSmlItemList.RemoveAt(i,1);
			i--;
			num--;
			continue;
		}
		switch(p_AsmSmlItemList[i].marsk) {
		case TYPE_DIM:
			ProSolidToPostfixId((ProSolid)p_AsmSmlItemList[i].dim.owner,&postId);
			status=ProDimensionValueGet(&p_AsmSmlItemList[i].dim,&value);
			status=ProDimensionTypeGet(&p_AsmSmlItemList[i].dim,&type);
			status=ProDimensionSymbolGet(&p_AsmSmlItemList[i].dim,symbol);
			status=ProDimensionToleranceGet(&p_AsmSmlItemList[i].dim,&upper_limit,&lower_limit);

			//temp_item.type=TYPE_DIM;
			//temp_item.Dim=p_AsmSmlItemList[i].dim;
			indexNo=p_List.InsertItem(LVIF_TEXT|LVIF_PARAM,ii++,p_AsmSmlItemList[i].name,0,0,0,i);

			temp.Format("%s:%d",CString(symbol),postId);
			p_List.SetItemText(indexNo,1,temp);
			p_List.SetItemText(indexNo,2,"尺寸");
		switch(type) {
		case PRODIMTYPE_LINEAR:
			p_List.SetItemText(indexNo,3,"线性");
			break;
		case PRODIMTYPE_RADIUS:
			p_List.SetItemText(indexNo,3,"半径");
			break;
		case PRODIMTYPE_DIAMETER:
			p_List.SetItemText(indexNo,3,"直径");
			break;
		case PRODIMTYPE_ANGLE:
			p_List.SetItemText(indexNo,3,"角度");
			break;
		default:
			p_List.SetItemText(indexNo,3,"未知");
			break;
			}
			temp.Format("%f",value);
			p_List.SetItemText(indexNo,4,temp);
			temp.Format("%f",upper_limit);
			p_List.SetItemText(indexNo,5,temp);
			temp.Format("%f",lower_limit);
			p_List.SetItemText(indexNo,6,temp);
			p_List.SetItemText(indexNo,7,p_AsmSmlItemList[i].note);
			ProMdlNameGet(p_AsmSmlItemList[i].dim.owner,symbol);
			p_List.SetItemText(indexNo,8,CString(symbol));
			break;
		case TYPE_PARA:
			ProSolidToPostfixId((ProSolid)UsrGetParameterOwner(&p_AsmSmlItemList[i].para),&postId);
			status=ProParameterValueGet(&p_AsmSmlItemList[i].para,&proval);

			//temp_item.type=TYPE_PARA;
			//temp_item.Para=p_AsmSmlItemList[i].para;
			indexNo=p_List.InsertItem(LVIF_TEXT|LVIF_PARAM,ii++,p_AsmSmlItemList[i].name,0,0,0,i);

			temp.Format("%s:%d",CString(p_AsmSmlItemList[i].para.id),postId);
			p_List.SetItemText(indexNo,1,temp);
			p_List.SetItemText(indexNo,2,"参数");

			switch(proval.type) {
		case PRO_PARAM_DOUBLE:
			csType="浮点型";
			csValue.Format("%f",proval.value.d_val);
			break;
		case PRO_PARAM_STRING:
			csType="字符串";
			csValue=CString(proval.value.s_val);
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
			p_List.SetItemText(indexNo,3,csType);
			p_List.SetItemText(indexNo,4,csValue);
			p_List.SetItemText(indexNo,7,p_AsmSmlItemList[i].note);
			ProMdlNameGet((p_AsmSmlItemList[i].para.owner.type==PRM_MODEL?p_AsmSmlItemList[i].para.owner.who.model:p_AsmSmlItemList[i].para.owner.who.item.owner),symbol);
			p_List.SetItemText(indexNo,8,CString(symbol));
			break;
		case TYPE_FEAT:
			//if(p_AsmSmlItemList[i].dim.owner!=(ProMdl)solid) continue;
			break;
		case TYPE_COMP:
			//if(p_AsmSmlItemList[i].dim.owner!=(ProMdl)solid) continue;
			break;
		default:
			break;
		}
		//判断是否已经添加在最上层组件的事务特性表中
		
	}
}
//在列表里面显示特征的尺寸
void CAsmParaDefDlg2::ShowFeatDim(ProFeature feature)
{
	int             num=0;
	ProError        status;
	CString         id;
	ProName         symbol;
	ProCharName     cSymbol;
	double          value;
	CString         csvalue;
	ProDimensiontype type;
	CString          csType;
	int             indexNo=0;
	double          upper_limit,lower_limit;
	int             postID;
	list_item       temp_item;
	Dimensionlist.RemoveAll();

	//访问特征的尺寸
	status=ProFeatureDimensionVisit(&feature,(ProDimensionVisitAction)partFeatDimVisAc ,
		NULL,(CArray<ProDimension,ProDimension> *)&Dimensionlist);

	m_List1.DeleteAllItems();
	do {		
	} while(m_List1.DeleteColumn(0));	

	//m_List.InsertColumn(0,"标识",LVCFMT_LEFT,50,0);
	m_List1.InsertColumn(0,"符号",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(1,"类型",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(2,"值",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(3,"上偏差",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(4,"下偏差",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(5,"所属元件",LVCFMT_LEFT,100,0);

	p_listitem.RemoveAll();
	temp_item.type=TYPE_DIM;
	num=(int)Dimensionlist.GetCount();
	for(int i=0;i<num;i++)
	{
		temp_item.Dim=Dimensionlist[i];
		temp_item.name="";
		p_listitem.Add(temp_item);

		ProSolidToPostfixId((ProSolid)Dimensionlist[i].owner,&postID);
		id.Format("d%d:%d",Dimensionlist[i].id,postID);

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
		sprintf(cSymbol,"%s:%d",cSymbol,postID);
		indexNo=m_List1.InsertItem(LVIF_TEXT|LVIF_PARAM,i,cSymbol,0,0,0,i);
		m_List1.SetItemText(indexNo,1,csType);
		m_List1.SetItemText(indexNo,2,csvalue);
		CString upper_limit_value;
		CString lower_limit_value;
		upper_limit_value.Format("%f",upper_limit);
		lower_limit_value.Format("%f",lower_limit);
		m_List1.SetItemText(indexNo,3,_T(upper_limit_value));
		m_List1.SetItemText(indexNo,4,_T(lower_limit_value));
		ProMdlNameGet(Dimensionlist[i].owner,symbol);
		m_List1.SetItemText(indexNo,5,CString(symbol));

		for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) 
		{
			if(p_AsmSmlItemList[j].marsk!=TYPE_DIM) continue;
			if(p_AsmSmlItemList[j].owner!=asm_solid) continue;
			if(ComparaDimension(&p_AsmSmlItemList[j].dim,&Dimensionlist[i])) ListView_SetCheckState(m_List1,indexNo,TRUE);
		}

	}
}

void CAsmParaDefDlg2::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此添加消息处理程序代码
	if((WindowFromPoint(point)==&m_List1))
	{
		CMenu mTreeRightMenu;
		MENUITEMINFO mInfo;
		MENUITEMINFO mInfo1;
		mInfo.cbSize=sizeof(MENUITEMINFO);
		mInfo1.cbSize=sizeof(MENUITEMINFO);
		mInfo.fMask=MIIM_STRING;
		mInfo1.fMask=MIIM_STRING;
		mInfo1.dwTypeData=_T("刷新");
		mInfo.dwTypeData=_T("编辑");

		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(4);
		ASSERT(pPopup!=NULL);
		pPopup->DeleteMenu(0,MF_BYPOSITION);
		pPopup->SetMenuItemInfo(0,&mInfo1,TRUE);
		pPopup->SetMenuItemInfo(1,&mInfo,TRUE);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);

	}
}
/************************************************************************/
/* 编辑尺寸                                                             */
/************************************************************************/
void CAsmParaDefDlg2::OnEditDim()
{
	// TODO: 在此添加命令处理程序代码
	CDimEditDlg  dlg;
	CParaOperaDlg dlg_para;
	ProDimension dim;
	POSITION     pos = m_List1.GetFirstSelectedItemPosition();
	int          nItem = m_List1.GetNextSelectedItem(pos); 
	if (nItem>=0)
	{
		int     postID=0;
		CString id,cSymbol;
		ProName symbol;
		double  value,upper_limit,lower_limit;
		switch(p_listitem.GetAt(m_List1.GetItemData(nItem)).type) {
		case TYPE_DIM:
			dim=p_listitem.GetAt(m_List1.GetItemData(nItem)).Dim;

			ProBoolean     rel_driven; 
			ProDimensionIsReldriven (&dim,&rel_driven);
			if(rel_driven==PRO_B_TRUE) 
			{AfxMessageBox("尺寸为关系式驱动，不能修改！");
			return;}

			ProSolidToPostfixId((ProSolid)dim.owner,&postID);
			id.Format("d%d:%d",dim.id,postID);

			ProDimensionSymbolGet(&dim,symbol);
			cSymbol=CString(symbol);            
			ProDimensionValueGet(&dim,&value);
			ProDimensionToleranceGet(&dim,&upper_limit,&lower_limit);

			dlg.m_ID=id;
			dlg.m_Symbol=cSymbol;
			dlg.m_Value=(float)value;
			dlg.m_UpLimit=(float)upper_limit;
			dlg.m_DwLimit=(float)lower_limit;

			if(dlg.DoModal()==IDOK)
			{
				ProName symbol;
				ProStringToWstring(symbol,dlg.m_Symbol.GetBuffer());
				if(ProDimensionSymbolSet(&dim,symbol)!=PRO_TK_NO_ERROR) AfxMessageBox("失败！");
				ProDimensionValueSet(&dim,dlg.m_Value);
				ProDimensionToleranceSet(&dim,dlg.m_UpLimit,dlg.m_DwLimit);
			}
			break;
		case TYPE_PARA:
			dlg_para.DoModal();
			break;
		default:
			break;
		}
		ProSolidRegenerate(asm_solid,PRO_REGEN_NO_FLAGS);
		OnFreshList();
	}	
}

void CAsmParaDefDlg2::ShowAsmSolidPara(ProSolid asm1)
{
	ProModelitem modelitem;
	int          PostID;
	int          indexNo=0;
	ProName      AsmcompName;
	list_item    temp_item;
	CArray<ProParameter,ProParameter> Parameterlist;
	ProMdlToModelitem(asm1,&modelitem);

	ProParameterVisit((ProModelitem*)&modelitem,NULL,(ProParameterAction)partAsmParaVisAc,
		(CArray<ProParameter,ProParameter>*)&Parameterlist);

	m_List1.DeleteAllItems();
	do {		
	} while(m_List1.DeleteColumn(0));
	m_List1.InsertColumn(0,"名称",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(1,"类型",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(2,"值",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(3,"所属元件",LVCFMT_LEFT,60,0);

    p_listitem.RemoveAll();
	for(int i=0;i<(int)Parameterlist.GetCount();i++)
	{
		ProParamvalue proval;
		ProCharName   str_id;
		CString       csType;
		CString       csValue;
		//过滤调不是顶层组件模型的参数
		if(UsrGetParameterOwner(&Parameterlist[i])!=asm1) continue;

		ProWstringToString(str_id,Parameterlist[i].id);
		ProParameterValueGet(&Parameterlist[i],&proval);
		ProSolidToPostfixId((ProSolid)(Parameterlist[i].owner.type==PRM_MODEL?Parameterlist[i].owner.who.model:Parameterlist[i].owner.who.item.owner)
			,&PostID);
		sprintf(str_id,"%s:%d",str_id,PostID);

		temp_item.type=TYPE_PARA;
		temp_item.Para=Parameterlist[i];

		indexNo=m_List1.InsertItem(LVIF_TEXT|LVIF_PARAM,i,str_id,0,0,0,(int)p_listitem.Add(temp_item));
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
		m_List1.SetItemText(indexNo,1,csType);
		m_List1.SetItemText(indexNo,2,csValue);		
		ProMdlNameGet(Parameterlist[i].owner.type==PRM_MODEL?Parameterlist[i].owner.who.model:Parameterlist[i].owner.who.item.owner 
			,AsmcompName);
		m_List1.SetItemText(indexNo,3,CString(AsmcompName));
		//判断是否已经在事物特性表中
		for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) { 
			if(p_AsmSmlItemList[j].marsk!=TYPE_PARA) continue;
			if(p_AsmSmlItemList[j].owner!=asm_solid) continue;
			//把选中的参数与事物特性表中的进行比较
			if (ComparaParameter(&Parameterlist[i],&p_AsmSmlItemList[j].para)) 
			{ 
				ListView_SetCheckState(m_List1,indexNo,TRUE);
				break;
			}
		}
	}
}
void CAsmParaDefDlg2::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION   pos = m_List1.GetFirstSelectedItemPosition();
	int        nItem= m_List1.GetNextSelectedItem(pos); 
	if (nItem>=0)
	{
		if (p_listitem.GetAt(m_List1.GetItemData(nItem)).type==TYPE_DIM)
		{
			UsrShowDim(asm_solid,&p_listitem.GetAt(m_List1.GetItemData(nItem)).Dim);
		}
	}
	*pResult = 0;
}

void CAsmParaDefDlg2::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_List2.GetFirstSelectedItemPosition();
	int nItem = m_List2.GetNextSelectedItem(pos); 
	if (nItem>=0)
	{
		if (p_AsmSmlItemList.GetAt(m_List2.GetItemData(nItem)).marsk ==TYPE_DIM)
		{
			UsrShowDim(asm_solid,&p_AsmSmlItemList.GetAt(m_List2.GetItemData(nItem)).dim) ;
		}
	}
	*pResult = 0;
}
/************************************************************************/
/* 刷新                                                                 */
/************************************************************************/
void CAsmParaDefDlg2::OnFreshList()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	int       index=(int)m_Tree.GetItemData(hItem);
	if(index<0) return;
	switch(p_treeitemlist.GetAt(index).type) {
	case -1:
		ShowAsmSolidPara(asm_solid);
		break;
	case 0:
		break;
	case 1:
		break;
	case 2:    //事务特性表
		ShowSolidSWTX(p_treeitemlist.GetAt(index).p_solid);
		break;
	case 3:    //装配约束尺寸
		ShowFeatDim(p_treeitemlist.GetAt(index).feature);
		break;
	default:
		break;
	}
}
/************************************************************************/
/* 添加尺寸参数到事务特性表                                             */
/************************************************************************/
void CAsmParaDefDlg2::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	int				num=0;
	ProName			symbol;
	CString			temp;
	int				Flag=0;
	int				postId=0;
	CString         error_list="";
	CString			error_info="";
	int				indexNo=0;

	num=(int)m_List1.GetItemCount();
	for (int i=0;i<num;i++)
	{
		if (!ListView_GetCheckState(m_List1,i)) continue;
		indexNo=(int)m_List1.GetItemData(i);
		switch(p_listitem[indexNo].type) {
		case TYPE_DIM:
			Flag=0;
			//判断是否已经存在
			for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) 
			{
				//类型不匹配
				if(p_AsmSmlItemList[j].marsk!=TYPE_DIM) continue;
				if(p_AsmSmlItemList[j].owner!=asm_solid) continue;
				//已经添加
				if (ComparaDimension(&p_listitem[indexNo].Dim,&p_AsmSmlItemList[j].dim))
				{
					Flag=1;
					break;
				}
			}
			//如果不存在
			if(Flag==0)
			{
				//被选中的尺寸加入事物特性表
				AsmSmlItem   tempAsm;
				tempAsm.owner=asm_solid;
				tempAsm.marsk=TYPE_DIM;
				//获得名称
				if (!p_listitem[indexNo].name.IsEmpty())
				{
					tempAsm.name=p_listitem[indexNo].name;
					tempAsm.note=p_listitem[indexNo].note;
				}
				else
				{
					ProDimensionSymbolGet(&p_listitem[indexNo].Dim,symbol);
					ProSolidToPostfixId((ProSolid)p_listitem[indexNo].Dim.owner,&postId);
					temp=CString(symbol);
					if (temp=="") 
						temp.Format("d%d:d",p_listitem[indexNo].Dim.id,postId);
					else 
						temp.Format("%s:%d",temp,postId);
					tempAsm.name=temp;
				}
				tempAsm.dim=p_listitem[indexNo].Dim;
				//判断是否在关系式的左侧,即已经是被驱动了
				if (!g_RelCollection.IsDimParaInRelSetLeftPart(asm_solid,NULL,&p_listitem[indexNo].Dim))
				{
					error_list+=tempAsm.name+" ";
				}
				else
				{
                    p_AsmSmlItemList.Add(tempAsm);
				}				
			}	   
			break;
		case TYPE_PARA:
			Flag=0;
			//判断是否已经存在
			for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) 
			{
				//类型不匹配
				if(p_AsmSmlItemList[j].marsk!=TYPE_PARA) continue;
				if(p_AsmSmlItemList[j].owner!=asm_solid) continue;
				//已经添加
				if (ComparaParameter(&p_listitem[indexNo].Para,&p_AsmSmlItemList[j].para))
				{
					Flag=1;
					break;
				}
			}
			//如果不存在
			if(Flag==0)
			{
				AsmSmlItem    tempAsm;
				tempAsm.owner=asm_solid;
				tempAsm.marsk=TYPE_PARA;
				if (p_listitem[indexNo].name!="")
				{
					tempAsm.name=p_listitem[indexNo].name;
					tempAsm.note=p_listitem[indexNo].note;
				}
				else
				{
					ProSolidToPostfixId((ProSolid)UsrGetParameterOwner(&p_listitem[indexNo].Para),&postId);
					tempAsm.name.Format("%s:%d",CString(p_listitem[indexNo].Para.id),postId);
				}
				tempAsm.para=p_listitem[indexNo].Para;
				//检查尺寸的值是否为空
				UsrSWTXTable1 temp(UsrGetParameterOwner(&tempAsm.para),&tempAsm.para);
				if(temp.isCheck!=1) 
				{
					AfxMessageBox("参数:"+tempAsm.name+"的值为空,将不被添加");
					break;
				}
				//检查是否已经存在与关系式的左侧,如果是,则不能添加,
				if (!g_RelCollection.IsDimParaInRelSetLeftPart(asm_solid,&p_listitem[indexNo].Para,NULL))
				{
					error_list+=tempAsm.name+" ";
				}
				else
				{
					p_AsmSmlItemList.Add(tempAsm);
				}				
			}
			break;
		default:
			break;
		}
	}
	if (!error_list.IsEmpty())
	{
		error_info.Format("尺寸(参数):%s,在关系式中已经作为被驱动参数(等式关系式左侧)\n,不能够作为事物特性表添加!请修改关系式后,再试!",error_list);
		AfxMessageBox(error_info);
	}
	ShowAsmSWTXList2(asm_solid,m_List2);
}
/************************************************************************/
/* 删除                                                                 */
/************************************************************************/
void CAsmParaDefDlg2::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION	pos = m_List2.GetFirstSelectedItemPosition();
	int			nItem = m_List2.GetNextSelectedItem(pos);  
	if (nItem>=0)
	{
		p_AsmSmlItemList.RemoveAt((int)m_List2.GetItemData(nItem));
		p_AsmSmlItemList.FreeExtra();
	}
	ShowAsmSWTXList2(asm_solid,m_List2);
}
/************************************************************************/
/* 重命名                                                               */
/************************************************************************/
void CAsmParaDefDlg2::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	int						 num=0;
	int                      indexNo=0;
	CSwtxTableItemEditDlg    dlg_edit;
	num=(int)m_List2.GetItemCount();
	for (int i=0;i<num;i++) {
		if (m_List2.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED) {	
			indexNo=(int)m_List2.GetItemData(i);
			switch(p_AsmSmlItemList[indexNo].marsk) {
			case TYPE_DIM:
				dlg_edit.m_Name=p_AsmSmlItemList[indexNo].name ;
				dlg_edit.m_Note=p_AsmSmlItemList[indexNo].note;
				if(dlg_edit.DoModal()==IDOK)
				{ 
					p_AsmSmlItemList[indexNo].name=dlg_edit.m_Name;
					p_AsmSmlItemList[indexNo].note=dlg_edit.m_Note;
				}
				break;
			case TYPE_PARA:
				dlg_edit.m_Name=p_AsmSmlItemList[indexNo].name ;
				dlg_edit.m_Note=p_AsmSmlItemList[indexNo].note;
				if(dlg_edit.DoModal()==IDOK)
				{ 
					p_AsmSmlItemList[indexNo].name=dlg_edit.m_Name;
					p_AsmSmlItemList[indexNo].note=dlg_edit.m_Note;
				}
				break;
			default:
				break;
			}
		}
	}
	ShowAsmSWTXList2(asm_solid,m_List2);
}
/************************************************************************/
/* 变型规则                                                             */
/************************************************************************/
void CAsmParaDefDlg2::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton4();
}
/************************************************************************/
/* 关闭                                                                 */
/************************************************************************/
void CAsmParaDefDlg2::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessage(WM_CLOSE);
}

void CAsmParaDefDlg2::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此添加消息处理程序代码
	UsrRepaintWindow();
}
