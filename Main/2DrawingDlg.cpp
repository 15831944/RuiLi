// 2DrawingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "2DrawingDlg.h"
#include "LabelDlg.h"

/*
*	全局函数头部声明
*/
//ProError  SgCollectFeat(ProSolid solid,			//(In)当前模型
//						ProFeature **p_data,BOOL nFlag=TRUE);

//ProError  sgCollectParameters( 
//							  ProModelitem    *p_modelitem,   /* In:  The model item */
//							  ProParameter    **p_parameters );
// C2DrawingDlg 对话框

IMPLEMENT_DYNAMIC(C2DrawingDlg, CDialog)
C2DrawingDlg::C2DrawingDlg(CWnd* pParent /*=NULL*/)
: CDialog(C2DrawingDlg::IDD, pParent)
, m_Name(_T(""))
, m_Label(_T(""))
{
	isOpened=FALSE;
	isAssembly=TRUE;
	m_state=0;
	drawflag=0;
}

C2DrawingDlg::~C2DrawingDlg()
{

}

void C2DrawingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MY_TREE_PARAM, m_ModelTree);
	DDX_Control(pDX, IDC_MY_LIST_PARAME, m_List);
	DDX_Control(pDX, IDC_MY_COMBO1, m_combo);
	DDX_Text(pDX, IDC_MY_EDIT_NAME, m_Name);
}


BEGIN_MESSAGE_MAP(C2DrawingDlg, CDialog)
	ON_BN_CLICKED(IDC_MY_BUTTON_LOAD, OnBnClickedButtonLoad)
	ON_NOTIFY(NM_CLICK, IDC_MY_TREE_PARAM, OnNMClickTreeParam)
	ON_NOTIFY(NM_RCLICK, IDC_MY_TREE_PARAM, OnNMRclickTreeParam)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_MY_BUTTON_UPDATE, OnBnClickedButtonUpdate)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_MY_BUTTON_2DCREATE, OnBnClickedButton2dcreate)
	ON_BN_CLICKED(IDC_MY_BUTTON_TABELCELL, OnBnClickedButtonTablecell)
	ON_NOTIFY(NM_DBLCLK, IDC_MY_LIST_PARAME, OnNMDblclkListParame)
	ON_BN_CLICKED(IDC_MY_BUTTON_DESIGNATE, OnBnClickedButtonDesignate)
	ON_BN_CLICKED(IDC_MY_EXPORT, OnBnClickedMyExport)
	ON_BN_CLICKED(IDC_MY_BUTTON_IMPORTFROMTABLE, OnBnClickedMyButtonImportfromtable)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// C2DrawingDlg 消息处理程序

BOOL C2DrawingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	USR_SET_DLG_ICON();
	isOpened=TRUE;
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,"参数名称",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(1,"参数值",LVCFMT_LEFT,150,-1);
	m_List.InsertColumn(2,"值类型",LVCFMT_LEFT,150,-1);
	//m_List.InsertColumn(2,"列",LVCFMT_LEFT,100,-1);
	//m_List.InsertColumn(3,"行",LVCFMT_LEFT,150,-1);
	//m_List.EnableColumnEdit(1);
	///初始化图像列表
	m_ImageList.Create(16, 16, ILC_COLOR8, 0, 4);//
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));

	m_combo.AddString("零件A4纵向");
	m_combo.AddString("零件A4横向");
	m_combo.AddString("零件A3横向");
	m_combo.AddString("零件A2横向");
	m_combo.AddString("零件A1横向");
	m_combo.AddString("装配A4纵向");
	m_combo.AddString("装配A3横向");
	m_combo.AddString("装配A2横向");
	m_combo.AddString("装配A1横向");
	m_combo.SelectString(0,"零件A4纵向");


	m_ModelTree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	this->OnBnClickedButtonLoad();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void C2DrawingDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	isOpened=FALSE;
	CDialog::OnClose();
}
/************************************************************************/
/* 导入模型树                                                           */
/************************************************************************/
void C2DrawingDlg::OnBnClickedButtonLoad()
{
	ProError status;
	ProMdl mdl;
	//	ProDrawing drawing;
	status=ProMdlCurrentGet(&mdl); //获得当前模型
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("无法得到当前模型!");
		this->SendMessage(WM_CLOSE,0,0);
		return ;
	}

	ProMdlType mdltype;
	status=ProMdlTypeGet(mdl,&mdltype);
	int sstate;
	sstate=m_state;
	if(mdltype==PRO_MDL_DRAWING)
	{
		ProMdlCurrentGet((ProMdl *)&drawing);
		ProDrawingCurrentSheetGet(drawing,&sheet);
		status=ProDrawingCurrentsolidGet((ProDrawing)mdl,&rootSolid);
		if(status!=PRO_TK_NO_ERROR) 
		{
			AfxMessageBox("无法得到当前模型的三维模型!");
			this->SendMessage(WM_CLOSE,0,0);
			return ;
		}
		if(m_state==0)
		{
			sstate=2;
		}
		drawflag=1;
	}
	else
	{
		drawflag=0;
		rootSolid=(ProSolid)mdl;
	}

//	this->ShowMyDialog(sstate);
	solid=rootSolid;
	AddParamToList(&solid);
	ProName tempname;
	status=ProMdlNameGet((ProMdl )solid,tempname);
	m_Name=CString(tempname);
	UpdateData(FALSE);
	ProMdlType  mdlType;
	ProMdlTypeGet(solid,&mdlType);
	if(mdlType==PRO_MDL_ASSEMBLY)
	{
		///addTree
		HTREEITEM root;
		ProName featTypeName;
		CString str;
		ProMdlNameGet(solid,featTypeName);
		str=CString(featTypeName);
		m_ModelTree.DeleteAllItems();
		root=m_ModelTree.InsertItem(str,0,0,NULL);//
		AddSolidToTree(&solid,&root);
		m_ModelTree.Expand(root,TVE_EXPAND);
	}
	else
	{
		HTREEITEM root;
		ProName featTypeName;
		CString str;
		ProMdlNameGet(solid,featTypeName);
		str=GetMdlInfo(solid).Name+"("+CString(featTypeName)+")";
		m_ModelTree.DeleteAllItems();
		root=m_ModelTree.InsertItem(str,1,1,NULL);//
		AddSolidToTree(&solid,&root);
		m_ModelTree.Expand(root,TVE_EXPAND);
		isAssembly=FALSE;
	}

}



/************************************************************************/
/* 模型树                                                               */
/************************************************************************/
int C2DrawingDlg::AddSolidToTree(ProSolid * pSolid,HTREEITEM * pItem)
{
	int				i,n;
	CString			str;
	HTREEITEM		hItem;
	ProError		status;
	ProFeature *	feature;
	ProName			Name;
	ProFeattype		featType;
	ProMdlType		mdlType;
	SgCollectFeat(*pSolid,&feature,FALSE);
	status = ProArraySizeGet((ProArray)feature, &n);
	for(i=0;i<n;i++)
	{
		status =ProFeatureTypeGet(feature+i, &featType);
		if(featType==PRO_FEAT_COMPONENT)
		{
			ProSolid newSolid;               
			status=ProAsmcompMdlGet((ProAsmcomp *)(feature+i),(ProMdl *)&newSolid);
			if(status==PRO_TK_NO_ERROR)
			{
				ProMdlNameGet(newSolid,Name);
				ProMdlTypeGet(newSolid,&mdlType);
				str=CString(Name);
				if(mdlType==PRO_MDL_ASSEMBLY)
					hItem=m_ModelTree.InsertItem(str,0,0,*pItem);
				else
					hItem=m_ModelTree.InsertItem(str,1,1,*pItem);
				m_ModelTree.SetItemData(hItem,(DWORD)( (feature+i)->id));
				AddSolidToTree(&newSolid,&hItem);
			}
		}
		else
		{
			status =ProModelitemNameGet (feature+i, Name);
			//str.Format("%d", (feature+i)->id);
			str=CString(Name);
			if(str=="")
			{
				status =ProFeatureTypenameGet(feature+i, Name);
				str=CString(Name);
				str.Format("%s ID %d",str,(feature+i)->id);
			}
			hItem=m_ModelTree.InsertItem(str,1,1,*pItem);
			m_ModelTree.SetItemData(hItem,(DWORD)( (feature+i)->id));
		}
	}
	ProArrayFree((ProArray *)&feature);
	return 1;
}

void C2DrawingDlg::OnNMClickTreeParam(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hItem;
	UINT uFlags=0;
	CPoint point;
	point= GetCurrentMessage()->pt;///
	m_ModelTree.ScreenToClient(&point);///
	hItem =m_ModelTree.HitTest(point, &uFlags);

	if ( hItem&&(uFlags&TVHT_ONITEM) )
	{

		this->ClickTree(hItem);
	}
	else
	{
		return;
	}

	*pResult = 0;
}

void C2DrawingDlg::OnNMRclickTreeParam(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代
	UpdateData(TRUE);
	HTREEITEM hItem;

	CPoint point;
	point= GetCurrentMessage()->pt;///
	////右键菜单
	//CMenu contextMenu;
	//CMenu *pMenu;
	//contextMenu.LoadMenu(IDR_POPUP_MENU);
	//pMenu=contextMenu.GetSubMenu(6);
	//pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTALIGN,point.x,point.y,this);

	m_ModelTree.ScreenToClient(&point);///
	UINT uFlags=0;
	hItem =m_ModelTree.HitTest(point, &uFlags);

	if ( hItem&&(uFlags&TVHT_ONITEM) )
	{
		this->ClickTree(hItem);
	}
	else
	{
		return;
	}
}


void C2DrawingDlg::ClickTree(HTREEITEM hItem)
{
	HTREEITEM root;
	root=m_ModelTree.GetRootItem();
	/*if(root!=hItem)
	{
		if(drawflag==1)
			this->ShowMyDialog(1);

	}
	else
	{
		if(m_state==0&&drawflag==1)
			this->ShowMyDialog(2);
		else
			this->ShowMyDialog(m_state);

	}*/
	ProError status;
	ProSolid newSolid;
	ProIdTable memb_id_tab;
	int table_size;
	CString str;
	int id,i,n;
	int  *table=new int[10];
	i=0;
	do {
		id=(int)m_ModelTree.GetItemData(hItem);
		*(table+i)=id;
		i++;
		hItem=m_ModelTree.GetParentItem(hItem);
	} while(hItem!=NULL);
	n=i-1;
	table_size=n;
	for(i=0;i<n;i++)
	{
		memb_id_tab[i]=*(table+n-1-i);
	}
	status=ProAsmcomppathInit(rootSolid,memb_id_tab,table_size,&comppath);
	if(status==PRO_TK_NO_ERROR)
	{
		status=ProAsmcomppathMdlGet(&comppath,(ProMdl *)&newSolid);
		if(status==PRO_TK_NO_ERROR)
		{
			solid=newSolid;
			ProName name;
			status=ProMdlNameGet((ProMdl )newSolid,name); 
			////辅助操作
			str=CString(name);
			m_Name=str;
			m_Label=str;
			CString str1,str2;
			str1.Format("元件名称:%s",str);
			int postid;
			status=ProSolidToPostfixId(newSolid,&postid);
			//str.Format("元件后缀:%d",postid);
			//str2.Format("%s  %s",str1,str);
			//m_Label=str2;
			//this->LoadParamToList();
			this->AddParamToList(&solid);
			UpdateData(FALSE);
			ProModelitem item;
			ProSelection sec;
			ProMdlToModelitem((ProMdl)solid,&item);
			ProWindowRepaint(PRO_VALUE_UNUSED);
			ProColortype color=PRO_COLOR_SELECTED;
			ProSelectionAlloc(&comppath,&item,&sec);
			ProSelectionHighlight(sec,color);
			return;
		}
		else
		{
			AfxMessageBox("不成功");
			return;
		}
	}
}


BOOL C2DrawingDlg::GetParam(ProModelitem * theOwner,CString strName,CString * strValue)
{
	ProError status;
	CString str="";
	ProName name;
	ProParameter param;
	ProParamvalue value;
	ProStringToWstring(name,strName.GetBuffer());
	status = ProParameterInit (theOwner,name,&param);
	if(status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	ProParameterValueGet(&param,&value);
	str=CString(param.id);
	switch(value.type) {
		case PRO_PARAM_DOUBLE:
			str.Format("%0.2f",value.value.d_val);
			break;
		case PRO_PARAM_STRING:
			str=CString(value.value.s_val);
			break;
		case PRO_PARAM_INTEGER:
			str.Format("%d",value.value.i_val);
			break;
		case PRO_PARAM_BOOLEAN:
			str.Format("%d",value.value.l_val);
			break;
		case PRO_PARAM_NOTE_ID:
			break;
		case PRO_PARAM_VOID:
			break;
		default:
			break;
	}
	*strValue=str;
	return TRUE;


}


void C2DrawingDlg::SetParam(ProModelitem * theOwner,CString strName,CString strValue,BOOL isOver,CString strType)
{

	CString str;
	ProError status;
	ProName name;  
	ProParamvalue value; 
	ProParameter  param;
	ProStringToWstring(name,strName.GetBuffer());
	if(strType=="string")
	{
		value.type=PRO_PARAM_STRING;
		ProLine *pLine;
		pLine=&(value.value.s_val);
		ProStringToWstring(*pLine,strValue.GetBuffer());
	}
	if(strType=="double")
	{
		value.type=PRO_PARAM_DOUBLE;
		value.value.d_val=atof(strValue);
	}
	if(strType=="integer")
	{
		value.type=PRO_PARAM_INTEGER;
		value.value.i_val=atoi(strValue);
	}

	status = ProParameterInit (theOwner, name,&param);
	status = ProParameterValueSet (&param, &value);
	return;

}


void C2DrawingDlg::AddParam(ProModelitem * theOwner,CString strName,CString strValue,BOOL isOver,CString strType)
{

	CString str;
	ProError status;
	ProName name;  
	ProParamvalue value; 
	ProParameter  param;
	ProStringToWstring(name,strName.GetBuffer());
	if(strType=="string")
	{
		value.type=PRO_PARAM_STRING;
		ProLine *pLine;
		pLine=&(value.value.s_val);
		ProStringToWstring(*pLine,strValue.GetBuffer());
	}
	if(strType=="double")
	{
		value.type=PRO_PARAM_DOUBLE;
		value.value.d_val=atof(strValue);
	}
	if(strType=="integer")
	{
		value.type=PRO_PARAM_INTEGER;
		value.value.i_val=atoi(strValue);
	}


	status=ProParameterCreate (theOwner,name,&value,&param);
	if(status==PRO_TK_E_FOUND)
	{
		if(isOver==FALSE) return;	
		status = ProParameterInit (theOwner, name,&param);
		status = ProParameterValueSet (&param, &value);
		return;
	}
}

void C2DrawingDlg::AddParamToList(ProSolid * theSolid)
{

	m_List.DeleteAllItems();
	BOOL flag=FALSE;
	ProMdlType  mdlType;
	ProMdlTypeGet(*theSolid,&mdlType);
	if(mdlType==PRO_MDL_ASSEMBLY)
	{
		//flag=TRUE;
		//if(*theSolid!=rootSolid)
			//return;
	}
	ProModelitem theOwner;
	ProMdlToModelitem(*theSolid,&theOwner);
	this->Begin(flag);
	CString strValue;
	BOOL theflag;
	for(int i=0;i<paramArray.GetSize();i++)
	{
		theflag=this->GetParam(&theOwner,paramArray[i].name,&strValue);
		if(theflag==FALSE)
		{
			this->AddParam(&theOwner,paramArray[i].name,paramArray[i].value);
		}
		else
		{
			paramArray[i].value=strValue;
		}

		/*if(drawflag==1&&paramArray[i].text=="比例"&&solid==rootSolid)
		{
			CString strscale="1:1";
			this->GetScaleS(&strscale);
			paramArray[i].value=strscale;
		}*/
		strValue=paramArray[i].value;
		m_List.InsertItem(i,paramArray[i].text);
		m_List.SetItemText(i,1,paramArray[i].value);
		CString tempstr;
		if(paramArray[i].type=="string") tempstr="字符串";
		if(paramArray[i].type=="integer") tempstr="整数";
		if(paramArray[i].type=="double") tempstr="实数";
		m_List.SetItemText(i,2,tempstr);

	}


}

void C2DrawingDlg::Begin(BOOL flag)
{
	paramArray.RemoveAll();
	PARAM2D param;
	param.name="CNAME";
	param.text="零件名称";
	param.value="";
	param.type="string";
    paramArray.Add(param);

	param.name="CMASS";
	param.text="零件质量";
	param.value="";
	param.type="double";
	paramArray.Add(param);

	param.name="CMAT";
	param.text="材料";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="CINDEX";
	param.text="件号";
	param.value="";
	param.type="integer";
	paramArray.Add(param);

	param.name="CTYPE";
	param.text="零部件类型";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="CSL";
	param.text="零部件数量";
	param.value="";
	param.type="integer";
	paramArray.Add(param);

	param.name="CJGLX";
	param.text="加工类型";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="CGG";
	param.text="零件规格";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="CLC";
	param.text="生产流程";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="CZPTH";
	param.text="装配图号";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="CJDBJ";
	param.text="阶段标识";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="SJR";
	param.text="设计人";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="CSHR";
	param.text="审核人";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="CSPR";
	param.text="审批人";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="CLJTH";
	param.text="零部件图号";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="CZCTH";
	param.text="总成图号";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="CGSMC";
	param.text="公司名称";
	param.value="";
	param.type="string";
	paramArray.Add(param);

	param.name="CSJRQ";
	param.text="设计日期";
	param.value="";
	param.type="string";
	paramArray.Add(param);









}

void C2DrawingDlg::OnBnClickedButtonUpdate()
{
	ProError	status;
	ProMdl		mdl;
	status=ProMdlCurrentGet(&mdl); //获得当前模型
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("无法得到当前模型!");
		return;
	}
	ProModelitem theOwner;
	ProMdlToModelitem(solid,&theOwner);
	CString str;
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		str=m_List.GetItemText(i,1);
		paramArray[i].value=str;
		this->SetParam(&theOwner,paramArray[i].name,paramArray[i].value,TRUE,paramArray[i].type);

		//if(drawflag==1)
		//{
		//	str=m_List.GetItemText(i,0);
		//	if(str=="比例")
		//	{
		//		str=m_List.GetItemText(i,1);
		//		//this->SetScaleS(str);
		//	}
		//}
	}
	ProWindowRepaint(PRO_VALUE_UNUSED);
}

void C2DrawingDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRepaint(PRO_VALUE_UNUSED);
	// TODO: 在此添加消息处理程序代码
}

void C2DrawingDlg::OnBnClickedButton2dcreate()
{
	//UpdateData(TRUE);
	//CString str;
	//ProName name;
	//ProMdlNameGet(solid,name);
	//CString m_Temp=m_Name;
	//if(m_Temp=="") return;
	//CString m_Format="";
	//if(m_Format=="")  
	//{
	//	int index;
	//	index=m_combo.GetCurSel();
	//	m_combo.GetLBText(index,str);
	//	if(str=="零件A4纵向") m_Format="a4_part_shu.drw";
	//	else if(str=="零件A4横向") m_Format="a4_part_heng.drw";
	//	else if(str=="零件A3横向") m_Format="a3_part_heng.drw";
	//	else if(str=="零件A2横向") m_Format="a2_part_heng.drw";
	//	else if(str=="零件A1横向") m_Format="a1_part_heng.drw";
	//	else if(str=="装配A4纵向") m_Format="a4_asm_shu.drw";
	//	else if(str=="装配A3横向") m_Format="a3_asm_heng.drw";
	//	else if(str=="装配A2横向") m_Format="a2_asm_heng.drw";
	//	else if(str=="装配A1横向") m_Format="a1_asm_heng.drw";
	//	else
	//	{ m_Format="a4_part_shu.drw";}
	//}

	//ProDrawing createdDrawing;
	//int status;
	//status=this->CreateFromTemplate(m_Temp,solid,m_Format,&createdDrawing);
	//if(status==1)
	//{
	//	//AfxMessageBox("成功");

	//	this->ShowNewWindow((ProMdl *)&createdDrawing);
	//	this->SetDtl(&createdDrawing);
	//}
	//else if(status==2)
	//{
	//	AfxMessageBox("存在相同的名字");
	//	return;
	//}
	//else 
	//{
	//	AfxMessageBox("创建不成功");
	//	return;
	//}
	///*this->exit();*/
	//OnCancel();
	//UsrActivateCurrentWindow();
}

void C2DrawingDlg::OnBnClickedButtonTablecell()
{
	//CString str;
	//BOOL flag;
	//int i;
	//for (i=0;i<m_List.GetItemCount();i++) {
	//	if ( m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
	//	{
	//		//flag=this->TablecellGet(&str);
	//		//if(flag==FALSE) return;
	//		str="&"+paramArray[i].name;

	//		flag=this->TablecellSet(str);
	//		return;
	//	}  
	//}

}

BOOL C2DrawingDlg::TablecellSet(CString theText)
{
	//ProError status;
	//CString str="";
	//ProDtlnote  note;
	////ProDtlnotedata  notedata;
	////ProDtlnoteline * line;
	////ProDtlnotetext * text;
	////ProLine string;
	//ProDwgtable  dwgtable;
	//int table_segment,row,column;
	////	ProName name;
	//ProSelection *sel;
	//int n_sel;
	////status = ProSelect ("table_cell", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	//status = ProSelect ("dwg_table", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	//if(status!=PRO_TK_NO_ERROR|| n_sel < 1)
	//{
	//	AfxMessageBox("没有选择");
	//	return FALSE;
	//}
	//status=ProSelectionModelitemGet(sel[0],&dwgtable);
	//if(status!=PRO_TK_NO_ERROR)
	//{
	//	AfxMessageBox("选择的不是表格");
	//	return FALSE;
	//}
	//ProWindowRepaint(PRO_VALUE_UNUSED);
	//status = ProSelect ("table_cell", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	////ProWindowRepaint(PRO_VALUE_UNUSED);
	//status=ProSelectionDwgtblcellGet(sel[0],&table_segment,&row,&column);
	//ProDwgtableCellNoteGet(&dwgtable,column+1,row+1,&note);

	///////////////////////////////////////////////
	////for (int i=0;i<m_List.GetItemCount();i++) {
	////	if ( m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
	////	{
	////		str.Format("%d",column+1);
	////		m_List.SetItemText(i,2,str);
	////		str.Format("%d",row+1);
	////		m_List.SetItemText(i,3,str);

	////		break;
	////	}  
	////}
	/////////////////////////////////////

	//this->SetStringToTableCell(&dwgtable,column+1,row+1,theText);
	//ProDwgtableDisplay(&dwgtable);
	//ProDwgSheetRegenerate(drawing,sheet);
	//ProWindowRepaint(PRO_VALUE_UNUSED);
	//ProArrayFree((ProArray *)&sel);
	//ProDtlnotelineFree(line[0]);
	//ProArrayFree((ProArray *)&text);
	return TRUE;
}


BOOL C2DrawingDlg::TablecellGet(CString * theText)
{
	//ProError status;
	//ProDtlnote  note;
	//ProDtlnotedata  notedata;
	//ProDtlnoteline * line;
	//ProDtlnotetext * text;
	//ProLine string;
	//ProDwgtable  dwgtable;
	//int table_segment,row,column;
	////	ProName name;
	//ProSelection *sel;
	//int n_sel;
	////status = ProSelect ("table_cell", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	//status = ProSelect ("dwg_table", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	//if(status!=PRO_TK_NO_ERROR|| n_sel < 1)
	//{
	//	AfxMessageBox("没有选择");
	//	return FALSE;
	//}
	//status=ProSelectionModelitemGet(sel[0],&dwgtable);
	//if(status!=PRO_TK_NO_ERROR)
	//{
	//	AfxMessageBox("选择的不是表格");
	//	return FALSE;
	//}
	//ProWindowRepaint(PRO_VALUE_UNUSED);
	//status = ProSelect ("table_cell", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	////ProWindowRepaint(PRO_VALUE_UNUSED);
	//status=ProSelectionDwgtblcellGet(sel[0],&table_segment,&row,&column);
	//ProDwgtableCellNoteGet(&dwgtable,column+1,row+1,&note);
	//ProDtlnoteDataGet(&note,NULL,PRODISPMODE_NUMERIC,&notedata);
	//ProDtlnotedataLinesCollect(notedata,&line);
	//ProDtlnotelineTextsCollect(line[0],&text);
	//ProDtlnotetextStringGet(text[0],string);
	////m_CellText=CString(string);
	////UpdateData(FALSE);
	//CString str;
	//str=CString(string);
	//*theText=str;
	//return TRUE;
	return TRUE;

}



void C2DrawingDlg::exit()
{
	isOpened=FALSE;
	CDialog::DestroyWindow();
}




void C2DrawingDlg::OnNMDblclkListParame(NMHDR *pNMHDR, LRESULT *pResult)
{
	LVHITTESTINFO lvhti;
	CString str;
	CPoint point = GetCurrentMessage()->pt;
	CPoint pt=point;
	m_List.ScreenToClient(&point);
	lvhti.pt = point;
	m_List.SubItemHitTest(&lvhti);
	int i;
	if (lvhti.flags & LVHT_ONITEMLABEL)
	{
		i=lvhti.iItem;
		CLabelDlg dlg;
		str=m_List.GetItemText(i,1);
		dlg.m_strEdit=str;
		str=m_List.GetItemText(i,0);
		dlg.m_oldname=str;
		dlg.m_Note="编辑参数信息";
		if(dlg.DoModal()==IDOK)
		{
			str=dlg.m_strEdit;
			m_List.SetItemText(i,1,str);
			OnBnClickedButtonUpdate();
			str=m_List.GetItemText(i,0);
			if(drawflag==1)
			{
				str=m_List.GetItemText(i,0);
				/*if(str=="比例"&&solid==rootSolid)
				{
					str=m_List.GetItemText(i,1);
					this->SetScaleS(str);
				}*/
			}
			return;
		}

	}
	*pResult = 0;
}




/************************************************************************/
/* 从模版创建二维图形                                                   */
/************************************************************************/
int C2DrawingDlg::CreateFromTemplate(CString strName ,ProMdl solid_mdl,CString templtName,ProDrawing * createdDrawing)
{
	//CString str;
	//ProError      err;
	////	ProMdlType    mdl_type;
	//ProMdldata    data;
	//ProModel      model;
	//ProName       predefined_template;
	//ProName       name;
	//ProStringToWstring(name,strName.GetBuffer());
	//ProDwgcreateOptions options = (ProDwgcreateOptions)0;
	//ProDwgcreateErrs  errors;
	//err = ProMdlDataGet (solid_mdl, &data);
	//str=CString(data.name);
	//ProStringToWstring(model.name,str.GetBuffer());
	//str=CString(data.type);
	//ProStringToWstring(model.type,str.GetBuffer());
	////ProUtilWstrcpy (model.name, data.name);
	////ProUtilWstrcpy (model.type, data.type);
	//ProStringToWstring (predefined_template, templtName.GetBuffer());
	//err = ProDrawingFromTmpltCreate (name, predefined_template,&model, options, createdDrawing, &errors);
	//if (err == PRO_TK_DWGCREATE_ERRORS)
	//{
	//	str.Format("创建%s出现错误",strName);
	//	AfxMessageBox(str);
	//	return 0;
	//}
	//if (err == PRO_TK_E_FOUND)
	//	return 2;
	return 1;
}


void C2DrawingDlg::OnBnClickedButtonDesignate()
{
	//ProDwgtable  dwgtable;
	//ProSelection *sel;
	//int n_sel;
	//ProError status;
	////status = ProSelect ("table_cell", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	//status = ProSelect ("dwg_table", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	//if(status!=PRO_TK_NO_ERROR|| n_sel < 1)
	//{
	//	AfxMessageBox("没有选择");
	//	return ;
	//}
	//status=ProSelectionModelitemGet(sel[0],&dwgtable);
	//if(status!=PRO_TK_NO_ERROR)
	//{
	//	AfxMessageBox("选择的不是表格");
	//	return ;
	//}
	//ProWindowRepaint(PRO_VALUE_UNUSED);
	//CString str;
	//for(int i=0;i<paramArray.GetSize();i++)
	//{
	//	str="&"+paramArray[i].name;
	//	if(paramArray[i].column!=0)
	//	{
	//		SetStringToTableCell(&dwgtable,paramArray[i].column,paramArray[i].row,str);
	//		SetHeightToTableCell(&dwgtable,paramArray[i].column,paramArray[i].row,paramArray[i].height);
	//	}
	//}
	//ProDwgSheetRegenerate(drawing,sheet);
	//ProWindowRepaint(PRO_VALUE_UNUSED);

}

BOOL C2DrawingDlg::SetStringToTableCell(ProDwgtable *table,int col,int row,CString text)
{
	/*ProError status;
	ProWstring *lines;
	ProArrayAlloc(1, sizeof(ProWstring), 1, (ProArray*)&lines);
	lines[0] = (wchar_t*)calloc(text.GetLength() + 1, sizeof(wchar_t));
	ProStringToWstring(lines[0], text.GetBuffer());
	status=ProDwgtableTextEnter(table, col, row, lines);
	ProArrayFree((ProArray*)&lines);
	if(status==PRO_TK_NO_ERROR)
		return TRUE;*/
	return FALSE;
}


BOOL C2DrawingDlg::SetHeightToTableCell(ProDwgtable *table,int col,int row,double height)
{
	//ProError status;
	//ProDtlnote  note;
	//ProDtlnotedata  notedata;
	//ProDtlnoteline * line;
	//ProDtlnotetext * text;
	//ProLine string;

	//status=ProDwgtableCellNoteGet(table,col,row,&note);
	//status=ProDtlnoteDataGet(&note,NULL,PRODISPMODE_NUMERIC,&notedata);
	//status=ProDtlnotedataJustifSet(notedata,PRO_HORZJUST_CENTER,PRO_VERTJUST_MIDDLE);
	//status=ProDtlnotedataLinesCollect(notedata,&line);
	//if(status!=PRO_TK_NO_ERROR) return FALSE;
	//status=ProDtlnotelineTextsCollect(line[0],&text);
	//if(status!=PRO_TK_NO_ERROR) return FALSE;
	//status=ProDtlnotetextHeightSet(text[0],height);
	//if(status!=PRO_TK_NO_ERROR) return FALSE;
	//status=ProDtlnotetextStringGet(text[0],string);
	//if(status!=PRO_TK_NO_ERROR) return FALSE;
	//CString str=CString(string);
	////AfxMessageBox(str);
	return TRUE;
}

BOOL C2DrawingDlg::GetTableCellText(ProDwgtable *table,int col,int row,CString * theText)
{
	//ProError status;
	//ProDtlnote  note;
	//ProDtlnotedata  notedata;
	//ProDtlnoteline * line;
	//ProDtlnotetext * text;
	//ProLine string;

	//status=ProDwgtableCellNoteGet(table,col,row,&note);
	//status=ProDtlnoteDataGet(&note,NULL,PRODISPMODE_NUMERIC,&notedata);
	//status=ProDtlnotedataJustifSet(notedata,PRO_HORZJUST_CENTER,PRO_VERTJUST_MIDDLE);
	//status=ProDtlnotedataLinesCollect(notedata,&line);
	//if(status!=PRO_TK_NO_ERROR) return FALSE;
	//status=ProDtlnotelineTextsCollect(line[0],&text);
	//if(status!=PRO_TK_NO_ERROR) return FALSE;
	////status=ProDtlnotetextHeightSet(text[0],height);
	//if(status!=PRO_TK_NO_ERROR) return FALSE;
	//status=ProDtlnotetextStringGet(text[0],string);
	//if(status!=PRO_TK_NO_ERROR) return FALSE;
	//CString str=CString(string);
	////AfxMessageBox(str);
	//*theText=str;
	return TRUE;

}

void C2DrawingDlg::OnBnClickedButtonReplacemoban()
{
//	ProError status;
//	status=ProMdlCurrentGet((ProMdl *)&drawing); //获得当前模型
//	if (status!=PRO_TK_NO_ERROR)
//	{
//		AfxMessageBox("无法得到当前模型!");
//		return ;
//	}
//	ProMdl mdl;
//	CString str;
//	CString m_Format;
//	int index;
//	index=m_combo.GetCurSel();
//	m_combo.GetLBText(index,str);
//	if(str=="零件a4") m_Format="parta4.drw";
//	else if(str=="零件a3") m_Format="parta3.drw";
//	else if(str=="零件a1") m_Format="parta1.drw";
//	else if(str=="组件a3") m_Format="asma3.drw";
//	else if(str=="组件a1") m_Format="asma1.drw";
//	else
//	{ m_Format="parta4.drw";}
//	CString folder="C:\\Program Files\\proeWildfire 2.0\\templates\\";
//	CString path=folder+m_Format;
//	int result;
//	result=this->LoadModel(path,&mdl);
//	if(result==0) 
//	{
//		AfxMessageBox("无法加载模板");
//		return;
//	}
//	int sheet;
//	status=ProDrawingCurrentSheetGet(drawing,&sheet);
//	status=ProDrawingCurrentsolidGet((ProDrawing)mdl,&solid);
//	status=ProDrawingFormatAdd(drawing,sheet,solid,mdl,0);
//	ProDwgSheetRegenerate(drawing,sheet);
//	ProWindowRepaint(PRO_VALUE_UNUSED);
//
}

////根据路径和solid指针打开solid文件
int C2DrawingDlg::LoadModel(CString path,ProMdl * pSolid)
{
	ProPath fullpath;
	ProError status;
	ProStringToWstring(fullpath,path.GetBuffer());
	ProMdlType type=PRO_MDL_UNUSED ;
	status=ProMdlLoad(fullpath,type,PRO_B_FALSE,(ProMdl*)pSolid);
	if(status==PRO_TK_NO_ERROR)
	{
		//AfxMessageBox("加载成功");
		return 1;
	}
	else
	{
		AfxMessageBox("加载失败");
		return 0;
	}
}

//显示模型
int  C2DrawingDlg::ShowNewWindow(ProMdl * pSolid)
{
	ProError status;

	int window_id ;
	ProName object_name ;
	ProType  object_type ;
	ProMdlNameGet(*pSolid,object_name);
	ProMdlTypeGet(*pSolid,(ProMdlType *)&object_type);
	ProObjectwindowCreate(object_name,object_type,&window_id);
	ProMdlWindowGet(*pSolid,&window_id);
	status=ProMdlDisplay(*pSolid);
	if(status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("显示失败");
		return 0;
	}
	status=ProWindowActivate(window_id);
	if(status==PRO_TK_NO_ERROR)
	{
		//AfxMessageBox("显示成功");
		return 1;
	}
	return 0;
}

BOOL C2DrawingDlg::ShowMyDialog(int state)
{
	if(state==0)
	{
		this->GetDlgItem(IDC_MY_EDIT_NAME)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_MY_COMBO1)->ShowWindow(SW_HIDE);
		//this->GetDlgItem(IDC_MY_BUTTON_2DCREATE)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDC_MY_BUTTON_2DCREATE)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_MY_BUTTON_TABELCELL)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_MY_BUTTON_DESIGNATE)->ShowWindow(SW_HIDE);
		this->GetDlgItem( IDC_MY_EXPORT)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_MY_BUTTON_IMPORTFROMTABLE)->ShowWindow(SW_HIDE);

	}
	else if(state==1)
	{
		this->GetDlgItem(IDC_MY_EDIT_NAME)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_MY_COMBO1)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_MY_BUTTON_2DCREATE)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_MY_BUTTON_TABELCELL)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_MY_BUTTON_DESIGNATE)->ShowWindow(SW_HIDE);
		this->GetDlgItem( IDC_MY_EXPORT)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_MY_BUTTON_IMPORTFROMTABLE)->ShowWindow(SW_HIDE);
	}
	else if(state==2)
	{
		//this->GetDlgItem(IDC_MY_EDIT_NAME)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_MY_EDIT_NAME)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_MY_COMBO1)->ShowWindow(SW_HIDE);
		//this->GetDlgItem(IDC_MY_COMBO1)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_MY_BUTTON_2DCREATE)->ShowWindow(SW_HIDE);
		//this->GetDlgItem(IDC_MY_BUTTON_2DCREATE)->EnableWindow(FALSE);
		//this->GetDlgItem(IDC_MY_BUTTON_TABELCELL)->ShowWindow(SW_SHOW);
		//this->GetDlgItem(IDC_MY_BUTTON_DESIGNATE)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDC_MY_BUTTON_TABELCELL)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_MY_BUTTON_DESIGNATE)->ShowWindow(SW_HIDE);
		//this->GetDlgItem( IDC_MY_EXPORT)->ShowWindow(SW_SHOW);
		this->GetDlgItem( IDC_MY_EXPORT)->ShowWindow(SW_HIDE);
		// this->GetDlgItem(IDC_MY_BUTTON_IMPORTFROMTABLE)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDC_MY_BUTTON_IMPORTFROMTABLE)->ShowWindow(SW_HIDE);
	}
	UpdateData(FALSE);
	return TRUE;

}
void C2DrawingDlg::OnBnClickedMyExport()
{
	CString filePath;
	ProError status;
	static char BASED_CODE szFilter[] = "AUTOCAD文件(*.dwg)|*.dwg|All Files (*.*)|*.*||";
	CFileDialog  fdlg(FALSE,"dwg",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,NULL,0);
	if(fdlg.DoModal()!=IDOK)
	{
		return;
	}
	filePath=fdlg.GetPathName();
	CString     ext=fdlg.GetFileExt();
	ProPath     thepath;
	ProStringToWstring(thepath,filePath.GetBuffer());
	if(ext=="dwg")
	{
		status=Pro2dExport(PRO_DWG_FILE,thepath,drawing);
		if(status==PRO_TK_NO_ERROR)
		{
			AfxMessageBox("输出成功");
		}
	}

}

void C2DrawingDlg::OnBnClickedMyButtonImportfromtable()
{
	//ProError	status;
	//ProMdl		mdl;
	//status=ProMdlCurrentGet(&mdl); //获得当前模型
	//if (status!=PRO_TK_NO_ERROR)
	//{
	//	AfxMessageBox("无法得到当前模型!");
	//	return;
	//}

	//ProDwgtable  dwgtable;
	//ProSelection *sel;
	//int n_sel;
	////ProError status;
	////status = ProSelect ("table_cell", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	//status = ProSelect ("dwg_table", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	//if(status!=PRO_TK_NO_ERROR|| n_sel < 1)
	//{
	//	AfxMessageBox("没有选择");
	//	return ;
	//}
	//status=ProSelectionModelitemGet(sel[0],&dwgtable);
	//if(status!=PRO_TK_NO_ERROR)
	//{
	//	AfxMessageBox("选择的不是表格");
	//	return ;
	//}
	//ProWindowRepaint(PRO_VALUE_UNUSED);
	//ProModelitem theOwner;
	//ProMdlToModelitem(solid,&theOwner);
	//CString str;
	////	int m,n;
	//for(int i=0;i<m_List.GetItemCount();i++)
	//{
	//	str="";
	//	this->GetTableCellText(&dwgtable,paramArray[i].column,paramArray[i].row,&str);
	//	m_List.SetItemText(i,1,str);
	//	paramArray[i].value=str;
	//	this->SetParam(&theOwner,paramArray[i].name,paramArray[i].value);
	//}
	//ProWindowRepaint(PRO_VALUE_UNUSED);
}


BOOL C2DrawingDlg::GetScaleD(double * dscale)
{
	//ProError status;
	//ProMdl mdl;
	//status=ProMdlCurrentGet(&mdl); //获得当前模型
	//if (status!=PRO_TK_NO_ERROR)
	//{
	//	AfxMessageBox("无法得到当前模型!");
	//	this->SendMessage(WM_CLOSE,0,0);
	//	return FALSE;
	//}

	//ProMdlType mdltype;
	//status=ProMdlTypeGet(mdl,&mdltype);
	//int sstate;
	//sstate=m_state;
	//if(mdltype==PRO_MDL_DRAWING)
	//{
	//	ProMdlCurrentGet((ProMdl *)&drawing);
	//	ProDrawingCurrentSheetGet(drawing,&sheet);
	//	status=ProDrawingCurrentsolidGet((ProDrawing)mdl,&rootSolid);
	//	if(status!=PRO_TK_NO_ERROR) 
	//	{
	//		AfxMessageBox("无法得到当前模型的三维模型!");
	//		this->SendMessage(WM_CLOSE,0,0);
	//		return FALSE;
	//	}
	//	solid=rootSolid;
	//}
	//status=ProDrawingScaleGet(drawing,solid,sheet,dscale);
	//if(status!=PRO_TK_NO_ERROR) return FALSE;
	return TRUE;

}

BOOL C2DrawingDlg::GetScaleS(CString * strscale)
{
	/*double dvalue;
	BOOL flag=GetScaleD(&dvalue);
	if(flag==FALSE) return FALSE;
	CString str;
	double d1;
	int num;
	if(dvalue<1)
	{
		d1=1/dvalue;
		num=(int)d1;
		str.Format("1:%d",num);
	}
	else
	{
		num=(int)dvalue;
		str.Format("%d:1",num);
	}
	*strscale=str;*/
	return TRUE;
}
BOOL C2DrawingDlg::SetScaleD(double dscale)
{
	//ProError status;
	//ProMdl mdl;
	//status=ProMdlCurrentGet(&mdl); //获得当前模型
	//if (status!=PRO_TK_NO_ERROR)
	//{
	//	AfxMessageBox("无法得到当前模型!");
	//	this->SendMessage(WM_CLOSE,0,0);
	//	return FALSE;
	//}

	//ProMdlType mdltype;
	//status=ProMdlTypeGet(mdl,&mdltype);
	//int sstate;
	//sstate=m_state;
	//if(mdltype==PRO_MDL_DRAWING)
	//{
	//	ProMdlCurrentGet((ProMdl *)&drawing);
	//	ProDrawingCurrentSheetGet(drawing,&sheet);
	//	status=ProDrawingCurrentsolidGet((ProDrawing)mdl,&rootSolid);
	//	if(status!=PRO_TK_NO_ERROR) 
	//	{
	//		AfxMessageBox("无法得到当前模型的三维模型!");
	//		this->SendMessage(WM_CLOSE,0,0);
	//		return FALSE;
	//	}
	//	solid=rootSolid;
	//}
	//status=ProDrawingScaleSet(drawing,solid,sheet,dscale);
	//if(status!=PRO_TK_NO_ERROR) return FALSE;
	//status=ProDwgSheetRegenerate(drawing,sheet);
	//ProWindowRepaint(PRO_VALUE_UNUSED);
	return TRUE;
}

BOOL C2DrawingDlg::SetScaleS(CString strscale)
{
	/*int index;
	index=strscale.Find(":");
	if(index==-1) return FALSE;
	CString pre,post;
	pre=strscale.Mid(0,index);
	post=strscale.Mid(index+1);
	double d1,d2,d;
	d1=atof(pre);
	d2=atof(post);
	d=d1/d2;
	return SetScaleD(d);*/
	return TRUE;
}

BOOL C2DrawingDlg::SetDtl(ProDrawing * drawing)
{
	////HKEY software;
	//LPTSTR  temp=new TCHAR[256];
	//ULONG num;
	//CString str;
	////str="C:\\Program Files\\proeWildfire 2.0\\apps\\ptcvconf\\text\\ptcvconf.ptc";
	//CRegKey reg;
	//reg.Open(HKEY_LOCAL_MACHINE,"SOFTWARE",KEY_READ);
	////software=reg.Detach();
	////if(reg.Open(software,"PTC",KEY_READ)!=ERROR_SUCCESS) return FALSE;
	////if(reg.QueryStringValue("vconf_Wildfire 2.0_2005010",temp,&num)!=ERROR_SUCCESS) return FALSE;
	////str=CString(temp,num);
	////delete [] temp;
	////if(str=="") return FALSE;
	////reg.Close();
	////str.Replace("apps\\ptcvconf\\text\\ptcvconf.ptc","templates\\standard.dtl");
	//if(reg.Open(reg.Detach(),"product design")!=ERROR_SUCCESS)
	//	return 0;
	//if(reg.Open(reg.Detach(),"proe")!=ERROR_SUCCESS) return 0;
	//reg.QueryStringValue("exec_path",temp,&num);
	//str=CString(temp,num);
	//if(str=="") return FALSE;
	//reg.Close();
	//str.Replace("bin\\proe.exe","templates\\standard.dtl");
	////str="C:\\Program Files\\proeWildfire 2.0\\templates\\standard.dtl";
	//ProError status;
	//ProPath filename,currentPath;
	//ProDirectoryCurrentGet(currentPath);
	//CString path=CString(currentPath);
	//CString strname="p314259265358.dtl";
	//path=path+strname;
	//::CopyFile(str,path,FALSE);
	//ProStringToWstring(filename,strname.GetBuffer());
	//status=ProInputFileRead(*drawing,filename,PRO_DWG_SETUP_FILE,NULL,NULL,NULL,NULL);
	//::DeleteFile(path);

	//str.Replace("standard.dtl","standard.pro");
	//strname="p314259265358.pro";
	//path=CString(currentPath);
	//path=path+strname;

	//::CopyFile(str,path,FALSE);
	//ProStringToWstring(filename,strname.GetBuffer());
	//status=ProInputFileRead(*drawing,filename,PRO_CONFIG_FILE,NULL,NULL,NULL,NULL);
	//::DeleteFile(path);

	//if(status!=PRO_TK_NO_ERROR) return FALSE;
	return TRUE;

}
void C2DrawingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
