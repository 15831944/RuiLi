// AsmParaDefGlg.cpp : ʵ���ļ�
//�������ģ�͵Ŀɱ������

#include "stdafx.h"
#include "Main.h"
#include "AsmParaDefGlg.h"
#include "DimEditDlg.h"
#include "ParaOperaDlg.h"
#include "SwtxTableItemEditDlg.h"
// CAsmParaDefGlg �Ի���

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


// CAsmParaDefGlg ��Ϣ�������

BOOL CAsmParaDefDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (ProMdlCurrentGet((ProMdl*)&asm_solid)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ǰ������ģ��");
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


	//����ģ���������Ա�
	ShowAsmSWTXList2(asm_solid,m_List2);
	m_Tree.SelectItem(m_Tree.GetRootItem());

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
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
			//����ģ�͵Ļ�����Ϣ	

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
					,"�ɱ������",3,3,0,0,p_treeitemlist.Add(temp_item),tempItem,NULL);
			}
			/*GetAsmcompConstraint(tempItem,(ProAsmcomp *)(&feat_list[i]));*/
			temp_item.type=3;
			m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM
				,"װ��ߴ�",3,3,0,0,p_treeitemlist.Add(temp_item),tempItem,NULL);
	    }
	}
	feat_list.FreeExtra();
}
//�ж�ʵ���Ƿ��ģ�͹�����
bool CAsmParaDefDlg2::IsSolidFromMdl(ProSolid solid)
{
	UsrPartPorp temp;
	CString     name=GetMdlFileName(solid);
	temp=GetMdlInfo(solid);
	//�������ģ�͵�ʵ���Լ���ģ�ͣ��Ż�����������Ա�
	if (temp.MdlType==1 || temp.MdlType==3)
	    return true;

	return false;
}
//���Ԫ��Լ����ϵ
void CAsmParaDefDlg2::GetAsmcompConstraint(HTREEITEM hItem,ProAsmcomp * p_component)
{
	//ProAsmcompconstraint *p_constraints ;
	//ProAsmcompConstraintsGet(p_component,&p_constraints);
}

void CAsmParaDefDlg2::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	case 2:    //�������Ա�
		ShowSolidSWTX(p_treeitemlist.GetAt(index).p_solid);
		break;
	case 3:    //װ��Լ���ߴ�
		ShowFeatDim(p_treeitemlist.GetAt(index).feature);
		break;
	default:
		break;
	}
	*pResult = 0;
}

//���б�������ʾģ�͵��������Ա�
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
	m_List1.InsertColumn(0,"����",LVCFMT_LEFT,60);                  //���������õ���Ĭ��Ϊ�ߴ���źͲ���������������
	m_List1.InsertColumn(1,"����",LVCFMT_LEFT,60);                  //�ߴ�ķ��ţ��Լ�����������������
	m_List1.InsertColumn(2,"����",LVCFMT_LEFT,60);
	m_List1.InsertColumn(3,"������",LVCFMT_LEFT,60);
	m_List1.InsertColumn(4,"ֵ",LVCFMT_LEFT,60);
	m_List1.InsertColumn(5,"��ƫ��",LVCFMT_LEFT,60);
	m_List1.InsertColumn(6,"��ƫ��",LVCFMT_LEFT,60);
	m_List1.InsertColumn(7,"���͹���",LVCFMT_LEFT,100);

    p_listitem.RemoveAll();

	for(int i=0;i<(int)p_UsrSWTXTableList.GetCount();i++)
	{
		//�����������Ա���,
		p_UsrSWTXTableList[i].Update();
		if(p_UsrSWTXTableList[i].isCheck!=1)	 continue;
		if(p_UsrSWTXTableList[i].owner!=part)		continue;
        ProSolidToPostfixId(part,&postid);
		csPostId.Format(":%d",postid);

		indexNo=m_List1.InsertItem(LVIF_TEXT|LVIF_PARAM
			,m_List1.GetItemCount(),p_UsrSWTXTableList[i].Name,0,0,0,(int)p_listitem.Add(temp_item));

		p_listitem[indexNo].name=p_UsrSWTXTableList[i].Name;
		p_listitem[indexNo].note=p_UsrSWTXTableList[i].Note;
		//���ڷ���Ҫ�������֣�����ǳߴ����÷��ţ�����ǲ�������������������
		if(p_UsrSWTXTableList[i].type=="dim")
		{
			p_listitem[indexNo].type=TYPE_DIM;
			p_listitem[indexNo].Dim=p_UsrSWTXTableList[i].Dim;
			m_List1.SetItemText(indexNo,1,p_UsrSWTXTableList[i].dimSymbol+csPostId);
			m_List1.SetItemText(indexNo,2,"�ߴ�");
		}
		else if (p_UsrSWTXTableList[i].type=="para") {
			p_listitem[indexNo].type=TYPE_PARA;
			p_listitem[indexNo].Para=p_UsrSWTXTableList[i].Para;
			m_List1.SetItemText(indexNo,1,p_UsrSWTXTableList[i].featName+csPostId);
			m_List1.SetItemText(indexNo,2,"����");
		}

		//�ж������ͣ��ߴ�ġ������ġ������ġ�ģ�͵�
		//���¹��ڳߴ�
		if(p_UsrSWTXTableList[i].subType=="PRODIMTYPE_LINEAR")
		{
			m_List1.SetItemText(indexNo,3,"����");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRODIMTYPE_RADIUS") {
			m_List1.SetItemText(indexNo,3,"�뾶");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRODIMTYPE_DIAMETER") {
			m_List1.SetItemText(indexNo,3,"ֱ��");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRODIMTYPE_ANGLE") {
			m_List1.SetItemText(indexNo,3,"�Ƕ�");
		}
		//���¹��ڲ���
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_DOUBLE") {
			m_List1.SetItemText(indexNo,3,"������");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_STRING") {
			m_List1.SetItemText(indexNo,3,"�ַ���");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_INTEGER") {
			m_List1.SetItemText(indexNo,3,"����");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_BOOLEAN") {
			m_List1.SetItemText(indexNo,3,"������");
		}
		//����������Ҫ��һ������
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_NOTE_ID") {
			m_List1.SetItemText(indexNo,3,"ע�ͺ�");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_VOID") {
			m_List1.SetItemText(indexNo,3,"������");
		}
		else{
			//��������ֱ��������������ProFeatureTypenameGet()���
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
/* �б��1���ڲ���ģʽ�µ����                                          */
/************************************************************************/
void CAsmParaDefDlg2::ShowAsmSWTXList1(ProSolid asm1,CListCtrl & p_List)
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
	list_item         temp_item;
	p_List.DeleteAllItems();
	do {		
	} while(p_List.DeleteColumn(0));
	//
	p_List.InsertColumn(0,"����",LVCFMT_LEFT,60);                  //���������õ���Ĭ��Ϊ�ߴ���źͲ���������������
	p_List.InsertColumn(1,"����",LVCFMT_LEFT,60);                  //�ߴ�ķ��ţ��Լ�����������������
	p_List.InsertColumn(2,"����",LVCFMT_LEFT,60);
	p_List.InsertColumn(3,"������",LVCFMT_LEFT,60);
	p_List.InsertColumn(4,"ֵ",LVCFMT_LEFT,60);
	p_List.InsertColumn(5,"��ƫ��",LVCFMT_LEFT,60);
	p_List.InsertColumn(6,"��ƫ��",LVCFMT_LEFT,60);
	p_List.InsertColumn(7,"���͹���",LVCFMT_LEFT,100);


	p_List.DeleteAllItems();
	p_listitem.RemoveAll();

	num=(int)p_AsmSmlItemList.GetCount();

	for (int i=0;i<num;i++) {
		int indexNo=0;
		//��鼰����
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
			p_List.SetItemText(indexNo,2,"�ߴ�");
			switch(type) {
			case PRODIMTYPE_LINEAR:
				p_List.SetItemText(indexNo,3,"����");
				break;
			case PRODIMTYPE_RADIUS:
				p_List.SetItemText(indexNo,3,"�뾶");
				break;
			case PRODIMTYPE_DIAMETER:
				p_List.SetItemText(indexNo,3,"ֱ��");
				break;
			case PRODIMTYPE_ANGLE:
				p_List.SetItemText(indexNo,3,"�Ƕ�");
				break;
			default:
				p_List.SetItemText(indexNo,3,"δ֪");
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
			p_List.SetItemText(indexNo,2,"����");

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
		//�ж��Ƿ��Ѿ���������ϲ�������������Ա���
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
/* �ڵڶ����б����ʾģ�͵��������Ա�                                    */
/************************************************************************/
void CAsmParaDefDlg2::ShowAsmSWTXList2(ProSolid asm1,CListCtrl & p_List)
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
	//list_item         temp_item;

	p_List.DeleteAllItems();
	do {		
	} while(p_List.DeleteColumn(0));

	p_List.InsertColumn(0,"����",LVCFMT_LEFT,60);                  //���������õ���Ĭ��Ϊ�ߴ���źͲ���������������
	p_List.InsertColumn(1,"����",LVCFMT_LEFT,60);                  //�ߴ�ķ��ţ��Լ�����������������
	p_List.InsertColumn(2,"����",LVCFMT_LEFT,60);
	p_List.InsertColumn(3,"������",LVCFMT_LEFT,60);
	p_List.InsertColumn(4,"ֵ",LVCFMT_LEFT,60);
	p_List.InsertColumn(5,"��ƫ��",LVCFMT_LEFT,60);
	p_List.InsertColumn(6,"��ƫ��",LVCFMT_LEFT,60);
	p_List.InsertColumn(7,"���͹���",LVCFMT_LEFT,100);
	p_List.InsertColumn(8,"����Ԫ��",LVCFMT_LEFT,100);


	p_List.DeleteAllItems();

	/*if(p_List==m_List1) p_listitem.RemoveAll();*/
	//p_listitem2.RemoveAll();

	num=(int)p_AsmSmlItemList.GetCount();

	for (int i=0;i<num;i++) {
		int indexNo=0;
		//��鼰����
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
			p_List.SetItemText(indexNo,2,"�ߴ�");
		switch(type) {
		case PRODIMTYPE_LINEAR:
			p_List.SetItemText(indexNo,3,"����");
			break;
		case PRODIMTYPE_RADIUS:
			p_List.SetItemText(indexNo,3,"�뾶");
			break;
		case PRODIMTYPE_DIAMETER:
			p_List.SetItemText(indexNo,3,"ֱ��");
			break;
		case PRODIMTYPE_ANGLE:
			p_List.SetItemText(indexNo,3,"�Ƕ�");
			break;
		default:
			p_List.SetItemText(indexNo,3,"δ֪");
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
			p_List.SetItemText(indexNo,2,"����");

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
		//�ж��Ƿ��Ѿ���������ϲ�������������Ա���
		
	}
}
//���б�������ʾ�����ĳߴ�
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

	//���������ĳߴ�
	status=ProFeatureDimensionVisit(&feature,(ProDimensionVisitAction)partFeatDimVisAc ,
		NULL,(CArray<ProDimension,ProDimension> *)&Dimensionlist);

	m_List1.DeleteAllItems();
	do {		
	} while(m_List1.DeleteColumn(0));	

	//m_List.InsertColumn(0,"��ʶ",LVCFMT_LEFT,50,0);
	m_List1.InsertColumn(0,"����",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(1,"����",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(2,"ֵ",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(3,"��ƫ��",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(4,"��ƫ��",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(5,"����Ԫ��",LVCFMT_LEFT,100,0);

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
			csType="����";
			break;
		case PRODIMTYPE_RADIUS:
			csType="�뾶";
			break;
		case PRODIMTYPE_DIAMETER:
			csType="ֱ��";
			break;
		case PRODIMTYPE_ANGLE:
			csType="�Ƕ�";
			break;
		default:
			csType="δ֪";
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
	// TODO: �ڴ������Ϣ����������
	if((WindowFromPoint(point)==&m_List1))
	{
		CMenu mTreeRightMenu;
		MENUITEMINFO mInfo;
		MENUITEMINFO mInfo1;
		mInfo.cbSize=sizeof(MENUITEMINFO);
		mInfo1.cbSize=sizeof(MENUITEMINFO);
		mInfo.fMask=MIIM_STRING;
		mInfo1.fMask=MIIM_STRING;
		mInfo1.dwTypeData=_T("ˢ��");
		mInfo.dwTypeData=_T("�༭");

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
/* �༭�ߴ�                                                             */
/************************************************************************/
void CAsmParaDefDlg2::OnEditDim()
{
	// TODO: �ڴ���������������
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
			{AfxMessageBox("�ߴ�Ϊ��ϵʽ�����������޸ģ�");
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
				if(ProDimensionSymbolSet(&dim,symbol)!=PRO_TK_NO_ERROR) AfxMessageBox("ʧ�ܣ�");
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
	m_List1.InsertColumn(0,"����",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(1,"����",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(2,"ֵ",LVCFMT_LEFT,60,0);
	m_List1.InsertColumn(3,"����Ԫ��",LVCFMT_LEFT,60,0);

    p_listitem.RemoveAll();
	for(int i=0;i<(int)Parameterlist.GetCount();i++)
	{
		ProParamvalue proval;
		ProCharName   str_id;
		CString       csType;
		CString       csValue;
		//���˵����Ƕ������ģ�͵Ĳ���
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
			csType="������";
			csValue.Format("%f",proval.value.d_val);
			break;
		case PRO_PARAM_STRING:
			csType="�ַ���";
			//ProWstringToString(str_line,proval.value.s_val);
			csValue=CString(proval.value.s_val);//Format("%c",str_line);
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
		m_List1.SetItemText(indexNo,1,csType);
		m_List1.SetItemText(indexNo,2,csValue);		
		ProMdlNameGet(Parameterlist[i].owner.type==PRM_MODEL?Parameterlist[i].owner.who.model:Parameterlist[i].owner.who.item.owner 
			,AsmcompName);
		m_List1.SetItemText(indexNo,3,CString(AsmcompName));
		//�ж��Ƿ��Ѿ����������Ա���
		for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) { 
			if(p_AsmSmlItemList[j].marsk!=TYPE_PARA) continue;
			if(p_AsmSmlItemList[j].owner!=asm_solid) continue;
			//��ѡ�еĲ������������Ա��еĽ��бȽ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
/* ˢ��                                                                 */
/************************************************************************/
void CAsmParaDefDlg2::OnFreshList()
{
	// TODO: �ڴ���������������
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
	case 2:    //�������Ա�
		ShowSolidSWTX(p_treeitemlist.GetAt(index).p_solid);
		break;
	case 3:    //װ��Լ���ߴ�
		ShowFeatDim(p_treeitemlist.GetAt(index).feature);
		break;
	default:
		break;
	}
}
/************************************************************************/
/* ��ӳߴ�������������Ա�                                             */
/************************************************************************/
void CAsmParaDefDlg2::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			//�ж��Ƿ��Ѿ�����
			for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) 
			{
				//���Ͳ�ƥ��
				if(p_AsmSmlItemList[j].marsk!=TYPE_DIM) continue;
				if(p_AsmSmlItemList[j].owner!=asm_solid) continue;
				//�Ѿ����
				if (ComparaDimension(&p_listitem[indexNo].Dim,&p_AsmSmlItemList[j].dim))
				{
					Flag=1;
					break;
				}
			}
			//���������
			if(Flag==0)
			{
				//��ѡ�еĳߴ�����������Ա�
				AsmSmlItem   tempAsm;
				tempAsm.owner=asm_solid;
				tempAsm.marsk=TYPE_DIM;
				//�������
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
				//�ж��Ƿ��ڹ�ϵʽ�����,���Ѿ��Ǳ�������
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
			//�ж��Ƿ��Ѿ�����
			for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) 
			{
				//���Ͳ�ƥ��
				if(p_AsmSmlItemList[j].marsk!=TYPE_PARA) continue;
				if(p_AsmSmlItemList[j].owner!=asm_solid) continue;
				//�Ѿ����
				if (ComparaParameter(&p_listitem[indexNo].Para,&p_AsmSmlItemList[j].para))
				{
					Flag=1;
					break;
				}
			}
			//���������
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
				//���ߴ��ֵ�Ƿ�Ϊ��
				UsrSWTXTable1 temp(UsrGetParameterOwner(&tempAsm.para),&tempAsm.para);
				if(temp.isCheck!=1) 
				{
					AfxMessageBox("����:"+tempAsm.name+"��ֵΪ��,���������");
					break;
				}
				//����Ƿ��Ѿ��������ϵʽ�����,�����,�������,
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
		error_info.Format("�ߴ�(����):%s,�ڹ�ϵʽ���Ѿ���Ϊ����������(��ʽ��ϵʽ���)\n,���ܹ���Ϊ�������Ա����!���޸Ĺ�ϵʽ��,����!",error_list);
		AfxMessageBox(error_info);
	}
	ShowAsmSWTXList2(asm_solid,m_List2);
}
/************************************************************************/
/* ɾ��                                                                 */
/************************************************************************/
void CAsmParaDefDlg2::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
/* ������                                                               */
/************************************************************************/
void CAsmParaDefDlg2::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
/* ���͹���                                                             */
/************************************************************************/
void CAsmParaDefDlg2::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedButton4();
}
/************************************************************************/
/* �ر�                                                                 */
/************************************************************************/
void CAsmParaDefDlg2::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendMessage(WM_CLOSE);
}

void CAsmParaDefDlg2::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: �ڴ������Ϣ����������
	UsrRepaintWindow();
}
