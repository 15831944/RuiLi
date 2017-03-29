// AsmSWTXDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "AsmSWTXDlg.h"
#include "SwtxTableItemEditDlg.h"
//BOOL CheckAsmSWTX(ProSolid asm_solid,AsmSmlItem * p_asmSwtx);
//extern CArray<AsmSmlItem,AsmSmlItem> p_AsmSmlItemList;

// CAsmSWTXDlg �Ի���

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


// CAsmSWTXDlg ��Ϣ�������

void CAsmSWTXDlg::ShowSWTXList(ProSolid solid)
{
	int               num=0;
	CString           temp;
	ProName           symbol;            //�ߴ����
	double            value;           //�ߴ�ֵ
	ProDimensiontype  type;            //�ߴ�����
	double            upper_limit;     //�ߴ����ƫ��  
    double            lower_limit;     //�ߴ����ƫ�� 
    int               ii=0;
	int               postId=0;          //����ĺ�׺
    ProError          status;
	ProParamvalue     proval;
	CString           csType;
	CString           csValue;
	mdl=solid;
	m_List.DeleteAllItems();	
    num=(int)p_AsmSmlItemList.GetCount();
	for (int i=0;i<num;i++) {
		int indexNo=0;
		//��鼰����
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
			m_List.SetItemText(indexNo,2,"�ߴ�");
			switch(type) {
			case PRODIMTYPE_LINEAR:
				m_List.SetItemText(indexNo,3,"����");
				break;
			case PRODIMTYPE_RADIUS:
				m_List.SetItemText(indexNo,3,"�뾶");
				break;
			case PRODIMTYPE_DIAMETER:
				m_List.SetItemText(indexNo,3,"ֱ��");
				break;
			case PRODIMTYPE_ANGLE:
				m_List.SetItemText(indexNo,3,"�Ƕ�");
				break;
			default:
				m_List.SetItemText(indexNo,3,"δ֪");
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
			m_List.SetItemText(indexNo,2,"����");
		
			switch(proval.type) {
			case PRO_PARAM_DOUBLE:
				csType="������";
				csValue.Format("%f",proval.value.d_val);
				break;
			case PRO_PARAM_STRING:
				csType="�ַ���";
				csValue=CString(proval.value.s_val);
				break;
			case PRO_PARAM_INTEGER:
				csValue.Format("%d",proval.value.i_val);
				csType="����";
				break;
			case PRO_PARAM_BOOLEAN:
				csType="������";
				csValue.Format("%d",proval.value.l_val);
				break;
			case PRO_PARAM_NOTE_ID:
				csType="ע�ͺ�";
				csValue="PRO_PARAM_NOTE_ID";
				break;
			case PRO_PARAM_VOID:
				csType="������";
				csValue="PRO_PARAM_VOID";
				break;
			default:
				csType="Ĭ��";
				csValue="Ĭ��";
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
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	//m_List.InsertColumn(0,"��ʶ",LVCFMT_LEFT,50);                  //
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,100);                  //���������õ���Ĭ��Ϊ�ߴ���źͲ���������������
	m_List.InsertColumn(1,"����",LVCFMT_LEFT,100);                  //�ߴ�ķ��ţ��Լ�����������������
	m_List.InsertColumn(2,"����",LVCFMT_LEFT,50);
	m_List.InsertColumn(3,"������",LVCFMT_LEFT,50);
	m_List.InsertColumn(4,"ֵ",LVCFMT_LEFT,80);
	m_List.InsertColumn(5,"��ƫ��",LVCFMT_LEFT,30);
	m_List.InsertColumn(6,"��ƫ��",LVCFMT_LEFT,30);
	m_List.InsertColumn(7,"���͹���",LVCFMT_LEFT,100);
	m_List.InsertColumn(8,"����Ԫ��",LVCFMT_LEFT,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/*ɾ��    delItemΪ����ͬʱɾ�������                                   */
/************************************************************************/
void CAsmSWTXDlg::OnDel()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ������Ϣ����������
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
/* ���������Ա�����б༭                                               */
/************************************************************************/
void CAsmSWTXDlg::OnEdit()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	ShowSWTXList(mdl);
}

void CAsmSWTXDlg::OnNMClickAsmSwtxList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
