// AsmSWTXDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "AsmSWTXDlg.h"
#include "SwtxTableItemEditDlg.h"
//BOOL CheckAsmSWTX(ProSolid asm_solid,AsmSmlItem * p_asmSwtx);
//extern CArray<AsmSmlItem,AsmSmlItem> p_AsmSmlItemList;

// CAsmSWTXDlg 对话框

IMPLEMENT_DYNAMIC(CAsmSWTXDlg, CDialog)
CAsmSWTXDlg::CAsmSWTXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmSWTXDlg::IDD, pParent)
	,mdl(NULL)
{
}

CAsmSWTXDlg::~CAsmSWTXDlg()
{
}

void CAsmSWTXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_ASM_SWTX_LIST, m_List);
}


BEGIN_MESSAGE_MAP(CAsmSWTXDlg, CDialog)
	ON_COMMAND(ID_12045, OnDel)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_12081, OnEdit)
	ON_COMMAND(ID_12080, OnRefresh)
	ON_NOTIFY(NM_CLICK, IDD_ASM_SWTX_LIST, OnNMClickAsmSwtxList)
END_MESSAGE_MAP()


// CAsmSWTXDlg 消息处理程序

void CAsmSWTXDlg::ShowSWTXList(ProSolid solid)
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
	mdl=solid;
	m_List.DeleteAllItems();	
    num=(int)p_AsmSmlItemList.GetCount();
	for (int i=0;i<num;i++) {
		int indexNo=0;
		//检查及过滤
		if(p_AsmSmlItemList[i].owner!=solid) continue;
		if (!CheckAsmSWTX(solid,&p_AsmSmlItemList[i]))
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
			indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,ii++,p_AsmSmlItemList[i].name,0,0,0,i);
			temp.Format("%s:%d",CString(symbol),postId);
            m_List.SetItemText(indexNo,1,temp);
			m_List.SetItemText(indexNo,2,"尺寸");
			switch(type) {
			case PRODIMTYPE_LINEAR:
				m_List.SetItemText(indexNo,3,"线性");
				break;
			case PRODIMTYPE_RADIUS:
				m_List.SetItemText(indexNo,3,"半径");
				break;
			case PRODIMTYPE_DIAMETER:
				m_List.SetItemText(indexNo,3,"直径");
				break;
			case PRODIMTYPE_ANGLE:
				m_List.SetItemText(indexNo,3,"角度");
				break;
			default:
				m_List.SetItemText(indexNo,3,"未知");
				break;
			}
			temp.Format("%f",value);
			m_List.SetItemText(indexNo,4,temp);
			temp.Format("%f",upper_limit);
			m_List.SetItemText(indexNo,5,temp);
			temp.Format("%f",lower_limit);
			m_List.SetItemText(indexNo,6,temp);
			m_List.SetItemText(indexNo,7,p_AsmSmlItemList[i].note);
			ProMdlNameGet(p_AsmSmlItemList[i].dim.owner,symbol);
			m_List.SetItemText(indexNo,8,CString(symbol));
			break;
		case TYPE_PARA:
			ProSolidToPostfixId((ProSolid)(p_AsmSmlItemList[i].para.owner.type==PRM_MODEL?p_AsmSmlItemList[i].para.owner.who.model:p_AsmSmlItemList[i].para.owner.who.item.owner),&postId);
			status=ProParameterValueGet(&p_AsmSmlItemList[i].para,&proval);
			indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,ii++,p_AsmSmlItemList[i].name,0,0,0,i);
			//m_List.SetItemText(indexNo,1,p_AsmSmlItemList[i].name);
			temp.Format("%s:%d",CString(p_AsmSmlItemList[i].para.id),postId);
			m_List.SetItemText(indexNo,1,temp);
			m_List.SetItemText(indexNo,2,"参数");
		
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
            m_List.SetItemText(indexNo,3,csType);
			m_List.SetItemText(indexNo,4,csValue);
			m_List.SetItemText(indexNo,7,p_AsmSmlItemList[i].note);
			ProMdlNameGet((p_AsmSmlItemList[i].para.owner.type==PRM_MODEL?p_AsmSmlItemList[i].para.owner.who.model:p_AsmSmlItemList[i].para.owner.who.item.owner),symbol);
			m_List.SetItemText(indexNo,8,CString(symbol));
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
	}
}

BOOL CAsmSWTXDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	//m_List.InsertColumn(0,"标识",LVCFMT_LEFT,50);                  //
	m_List.InsertColumn(0,"名称",LVCFMT_LEFT,100);                  //重新命名得到，默认为尺寸符号和参数和特征的名称
	m_List.InsertColumn(1,"符号",LVCFMT_LEFT,100);                  //尺寸的符号，以及参数和特征的名称
	m_List.InsertColumn(2,"类型",LVCFMT_LEFT,50);
	m_List.InsertColumn(3,"子类型",LVCFMT_LEFT,50);
	m_List.InsertColumn(4,"值",LVCFMT_LEFT,80);
	m_List.InsertColumn(5,"上偏差",LVCFMT_LEFT,30);
	m_List.InsertColumn(6,"下偏差",LVCFMT_LEFT,30);
	m_List.InsertColumn(7,"变型规则",LVCFMT_LEFT,100);
	m_List.InsertColumn(8,"所属元件",LVCFMT_LEFT,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/*删除    delItem为多项同时删除的情况                                   */
/************************************************************************/
void CAsmSWTXDlg::OnDel()
{
	// TODO: 在此添加命令处理程序代码
	int num=0;
	CArray<int,int> delItem;
	num=(int)m_List.GetItemCount();
	for (int i=0;i<num;i++) {
		if (m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED) {	
			 delItem.Add((int)m_List.GetItemData(i));
		}
	}
	num=(int)delItem.GetCount();
	for (int i=0;i<num;i++) {
		p_AsmSmlItemList.RemoveAt(delItem[i]-i,1);		
	}
	p_AsmSmlItemList.FreeExtra();
	ShowSWTXList(mdl);
}

void CAsmSWTXDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此添加消息处理程序代码
	if((WindowFromPoint(point)==&m_List))
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(2);
		ASSERT(pPopup!=NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);

	}
}
/************************************************************************/
/* 对事物特性表项进行编辑                                               */
/************************************************************************/
void CAsmSWTXDlg::OnEdit()
{
	// TODO: 在此添加命令处理程序代码
	int num=0;
	num=(int)m_List.GetItemCount();
	for (int i=0;i<num;i++) {
		if (m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED) {	
            CSwtxTableItemEditDlg dlg;
            dlg.m_Name=m_List.GetItemText(i,0);
			dlg.m_Note=m_List.GetItemText(i,7);
			if(dlg.DoModal()==IDOK)
			{ 
				p_AsmSmlItemList[(int)m_List.GetItemData(i)].name=dlg.m_Name;
				p_AsmSmlItemList[(int)m_List.GetItemData(i)].note=dlg.m_Note;
			}
		}
	}
	ShowSWTXList(mdl);
}

void CAsmSWTXDlg::OnRefresh()
{
	// TODO: 在此添加命令处理程序代码
	ShowSWTXList(mdl);
}

void CAsmSWTXDlg::OnNMClickAsmSwtxList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	int      nItem=m_List.GetNextSelectedItem(pos);
	if (nItem>=0)
	{
		int index=(int)m_List.GetItemData(nItem);
		if (p_AsmSmlItemList[index].marsk==TYPE_DIM )
		{
			UsrShowDim(mdl,&p_AsmSmlItemList[index].dim);
		}
	}
	*pResult = 0;
}
