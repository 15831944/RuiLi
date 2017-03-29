// AsmParaDefDlg.cpp : ʵ���ļ�
//����������Ա�͹�ϵʽ����

#include "stdafx.h"
#include "Main.h"
#include "AsmParaDefDlg.h"
#include "ParaOperaDlg.h"
#include "DimEditDlg.h"
#define  SEL_DIM  0
#define  SEL_PARA 1
#define  SEL_FEAT 2
#define  SEL_PRRT 3
IMPLEMENT_DYNAMIC(CAsmParaDefDlg, CDialog)
CAsmParaDefDlg::CAsmParaDefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmParaDefDlg::IDD, pParent)
	, m_RadioFlag(0)
	,current_hItem(NULL)
	,current_selection(NULL)
	, m_RadioButtonValue(0)
{
}

CAsmParaDefDlg::~CAsmParaDefDlg()
{
}

void CAsmParaDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_ASM_PARADEF_TREE, m_Tree);
	DDX_Control(pDX, IDD_ASM_PARADEF_LIST, m_List);
	DDX_Control(pDX, IDD_ASM_SWTX_BUTTON, m_SWTXButton);
	DDX_Control(pDX, IDD_ASM_REL_BUTTON, m_RELButton);
	DDX_Control(pDX, IDC_ASM_EDIT1, m_PathEdit);
	//DDX_Control(pDX, IDC_RADIO1, m_RadioButton);
	DDX_Control(pDX, 1010, m_DimDisplayModeCheckbox);
	DDX_Control(pDX, 1011, m_FeatDisplayModeCheckbox);
	DDX_Radio(pDX, IDC_RADIO1, m_RadioButtonValue);
}


BEGIN_MESSAGE_MAP(CAsmParaDefDlg, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDD_ASM_PARADEF_TREE, OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDD_ASM_SWTX_BUTTON, OnBnClickedButton3)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON, OnBnClickedButton4)
	ON_BN_CLICKED(IDD_ASM_PARADEF_BUTTON3, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
	ON_BN_CLICKED(IDD_ASM_PARADEF_BUTTON4, OnBnClickedAsmParadefButton4)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_12058, OnInsertToRel)
	ON_COMMAND(ID_12076, OnEditDim)
	ON_NOTIFY(NM_CLICK, IDD_ASM_PARADEF_LIST, OnNMClickAsmParadefList)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAsmParaDefDlg ��Ϣ�������

BOOL CAsmParaDefDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//�����б�
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,100,0);
	m_List.InsertColumn(1,"����",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(2,"ֵ",LVCFMT_LEFT,80,0);
	m_List.InsertColumn(3,"��ƫ��",LVCFMT_LEFT,30,0);
	m_List.InsertColumn(4,"��ƫ��",LVCFMT_LEFT,30,0);
	m_List.InsertColumn(5,"����Ԫ��",LVCFMT_LEFT,100,0);
	//���ͼ��
	imageList.Create(12, 12, ILC_COLOR16 , 4, 4);
    SetImageListForFeatrure(&imageList);
    m_Tree.SetImageList(&imageList,TVSIL_NORMAL);
	//���ģ����
	if (ProMdlCurrentGet((ProMdl*)&asm_solid)==PRO_TK_NO_ERROR) {
		ProName name;
		ProMdlNameGet(asm_solid,name);
		//����ϵʽ��ʵ�帶ֵ
		rel_dlg.asm_solid=asm_solid;
		//����ģ�͵Ļ�����Ϣ
		IsMdlInfoDefAndDef(asm_solid);
		m_Tree.DeleteAllItems();
		asm_feat_list.RemoveAll();
		HTREEITEM hItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM,_T(CString(name)+".ASM")
			,0,0,0,0,-1,NULL,NULL);
		ShowModelTree(asm_solid,hItem);
		m_Tree.Expand(m_Tree.GetRootItem(),TVE_EXPAND );
		//m_RadioButton.SetCheck(BST_CHECKED);
		m_RadioButtonValue=0;
		m_RadioFlag=SEL_DIM;
	}
	else
	{
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CAsmParaDefDlg::ShowModelTree(ProSolid solid,HTREEITEM hItem)
{
	ProError       status;
	CString        csName;
	ProName        wName;
	HTREEITEM      tempItem;
	ProMdlType     type;
	ProFeattype    ftype;
	int            imageNum=0;
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
				csName=csName+".ASM";
				break;
			case PRO_MDL_PART:
				imageNum=6;
				csName=csName+".PRT";
				break;
			default:
				
				break;
			}			
			tempItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM,csName
				,imageNum,imageNum,0,0,asm_feat_list.Add(feat_list[i]),hItem,NULL);	
			//����ģ�͵Ļ�����Ϣ
		/*	IsMdlInfoDefAndDef(mdl);*/
			ShowModelTree(mdl,tempItem);
		}
		else
		{
			status =ProModelitemNameGet (&feat_list[i], wName);
			csName=CString(wName);
			if(csName=="")
			{
				status =ProFeatureTypenameGet(&feat_list[i], wName);
				csName=CString(wName);
				csName.Format("%s ��ʶ%d",csName,feat_list[i].id);
			}
			tempItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM,csName
				,GetFeatIconNum(ftype),GetFeatIconNum(ftype),0,0,asm_feat_list.Add(feat_list[i]),hItem,NULL);			
		}
	}
	feat_list.FreeExtra();
}
void CAsmParaDefDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    CString path;
	HTREEITEM pItem;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	current_hItem=hItem;   //���õ�ǰѡ�е���
	path=m_Tree.GetItemText(hItem);
	if (hItem!=m_Tree.GetRootItem()) {
		pItem=hItem;
		do {
			pItem=m_Tree.GetParentItem(pItem);
            path=m_Tree.GetItemText(pItem)+"\\"+path;
		} while(pItem!=m_Tree.GetRootItem());

		if (m_FeatDisplayModeCheckbox.GetState()==1) {
			HighLightFeat(hItem);
		}
		switch(m_RadioFlag) {
		case SEL_DIM:
			ConverthItemToList(hItem);
			break;
		case SEL_PARA:
			//ShowParaList();
			m_RadioFlag=SEL_DIM;
			m_RadioButtonValue=0;
			UpdateData(FALSE);
			ConverthItemToList(hItem);
			break;
		case SEL_FEAT:
			break;
		case SEL_PRRT:
			break;
		default:
			break;
			}
	}
	m_PathEdit.SetWindowText(path);

	
	*pResult = 0;
}
/************************************************************************/
/* ��������                                                             */
/************************************************************************/
int CAsmParaDefDlg::HighLightFeat(HTREEITEM hItem)
{
	ProSelection         selection ;
	CArray<int,int>      memb_id_tab;
	ProIdTable           memb_id_tab_1;
	ProAsmcomppath       m_path;
	ProFeature           feature;
	ProFeattype          ftype;
	ProError             status;
	feature=asm_feat_list[(int)m_Tree.GetItemData(hItem)];	
    //�������·��
	GetSolidIDTable(asm_solid,&asm_feat_list,current_hItem,&m_Tree,&memb_id_tab);
	for (int j=0;j<(int)memb_id_tab.GetCount();j++) {
		memb_id_tab_1[j]=memb_id_tab[j];
	}
	status=ProAsmcomppathInit(asm_solid,memb_id_tab_1,(int)memb_id_tab.GetCount(),&m_path);
	//����������ͣ��������ͽ��в�ͬ����
    status=ProFeatureTypeGet (&feature, &ftype);
	if(current_selection!=NULL) ProSelectionUnhighlight(current_selection);
	if (ftype==PRO_FEAT_COMPONENT) {
		memb_id_tab_1[(int)memb_id_tab.GetCount()-1]=0;
		status=ProAsmcomppathInit(asm_solid,memb_id_tab_1,(int)memb_id_tab.GetCount()-1,&m_path);
		status=ProSelectionAlloc (&m_path,(ProModelitem*)&feature,&selection );
		if(status==PRO_TK_NO_ERROR) status=ProSelectionHighlight(selection,PRO_COLOR_HIGHLITE);
		current_selection=selection;
	}
	else
	{
		status=ProSelectionAlloc (&m_path,(ProModelitem*)&feature,&selection );
		if(status==PRO_TK_NO_ERROR) status=ProSelectionHighlight(selection,PRO_COLOR_HIGHLITE);
		current_selection=selection;
    }
	return 0;
}
void CAsmParaDefDlg::ConverthItemToList(HTREEITEM hItem)
{
	ProError  status;

    Dimensionlist.RemoveAll();
	
    //���������ĳߴ�
	status=ProFeatureDimensionVisit(&asm_feat_list.GetAt((int)m_Tree.GetItemData(hItem)),(ProDimensionVisitAction)partFeatDimVisAc ,
		NULL,(CArray<ProDimension,ProDimension> *)&Dimensionlist);
	FreshListByDim();
}
/************************************************************************/
/* ���³ߴ�                                                             */
/************************************************************************/
int CAsmParaDefDlg::FreshListByDim(void)
{
	int             num=0;
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
	m_List.DeleteAllItems();
	do {		
	} while(m_List.DeleteColumn(0));	

	//m_List.InsertColumn(0,"��ʶ",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,100,0);
	m_List.InsertColumn(1,"����",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(2,"ֵ",LVCFMT_LEFT,80,0);
	m_List.InsertColumn(3,"��ƫ��",LVCFMT_LEFT,30,0);
	m_List.InsertColumn(4,"��ƫ��",LVCFMT_LEFT,30,0);
	m_List.InsertColumn(5,"����Ԫ��",LVCFMT_LEFT,100,0);
	num=(int)Dimensionlist.GetCount();
	for(int i=0;i<num;i++)
	{
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
		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,i,cSymbol,0,0,0,i);
		m_List.SetItemText(indexNo,1,csType);
		m_List.SetItemText(indexNo,2,csvalue);
		CString upper_limit_value;
		CString lower_limit_value;
		upper_limit_value.Format("%f",upper_limit);
		lower_limit_value.Format("%f",lower_limit);
		m_List.SetItemText(indexNo,3,_T(upper_limit_value));
		m_List.SetItemText(indexNo,4,_T(lower_limit_value));
		ProMdlNameGet(Dimensionlist[i].owner,symbol);
		m_List.SetItemText(indexNo,5,CString(symbol));

		for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) {
			if(p_AsmSmlItemList[j].marsk!=TYPE_DIM) continue;
			if(p_AsmSmlItemList[j].owner!=asm_solid) continue;
			if (Dimensionlist[i].id==p_AsmSmlItemList[j].dim.id && Dimensionlist[i].type==p_AsmSmlItemList[j].dim.type 
				&& Dimensionlist[i].owner ==p_AsmSmlItemList[j].dim.owner) 
			{
				ListView_SetCheckState(m_List,indexNo,TRUE);
			}
		}

	}
	return 0;
}
/************************************************************************/
/* �л��������Ա�                                                       */
/************************************************************************/
void CAsmParaDefDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString   text;
	CRect     rect;
	CRect     rect1;
	CRect     rect2;
	m_RELButton.GetWindowText(text);
    if (text==_T("<")) {
		//�ر�
		GetWindowRect(rect);
		rel_dlg.GetWindowRect(&rect2);
		rect.right=rect.right-rect2.Width();
		rel_dlg.DestroyWindow();
		MoveWindow(&rect,TRUE);
		m_RELButton.SetWindowText(_T(">"));
    }
	m_SWTXButton.GetWindowText(text);
	if (text==_T("<")) {
		GetWindowRect(rect);
		swtx_dlg.GetWindowRect(&rect2);
		rect.right=rect.right-rect2.Width();
		swtx_dlg.DestroyWindow();
		MoveWindow(&rect,TRUE);
		m_SWTXButton.SetWindowText(_T(">"));
		return;
	}
	//��

	GetWindowRect(rect);
	GetClientRect(rect1);
	swtx_dlg.Create(IDD_ASM_SWTX,this);
	//swtx_dlg.GetWindowRect(&rect2);	
	swtx_dlg.GetWindowRect(&rect2);
	rect.right=rect.right+rect2.Width();
	MoveWindow(&rect,TRUE);
	swtx_dlg.MoveWindow(rect1.right,0,rect2.Width(),rect.Height(),TRUE);
	swtx_dlg.ShowWindow(SW_SHOW);
	swtx_dlg.ShowSWTXList(asm_solid);
	m_SWTXButton.SetWindowText(_T("<"));
}
/************************************************************************/
/* �л���ϵʽ�༭                                                       */
/************************************************************************/
void CAsmParaDefDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString text;

	CRect rect;
	CRect rect1;
	CRect rect2;
	m_SWTXButton.GetWindowText(text);
	if (text=="<") {
		//�ر�
		GetWindowRect(rect);
		swtx_dlg.GetWindowRect(&rect2);
		rect.right=rect.right-rect2.Width();
		swtx_dlg.DestroyWindow();
		MoveWindow(&rect,TRUE);
		m_SWTXButton.SetWindowText(_T(">"));
	}
	m_RELButton.GetWindowText(text);
	if (text=="<") {
		GetWindowRect(rect);
		rel_dlg.GetWindowRect(&rect2);
		rect.right=rect.right-rect2.Width();
		rel_dlg.DestroyWindow();
		MoveWindow(&rect,TRUE);
		m_RELButton.SetWindowText(_T(">"));
		return;
	}
	//��
	GetWindowRect(rect);
	GetClientRect(rect1);
	rel_dlg.Create(IDD_ASM_REL,this);
	//rel_dlg.GetWindowRect(&rect2);	
	rel_dlg.GetWindowRect(&rect2);
	rect.right=rect.right+rect2.Width();
	MoveWindow(&rect,TRUE);
	rel_dlg.MoveWindow(rect1.right,0,rect2.Width(),rect.Height(),TRUE);
	rel_dlg.asm_solid=asm_solid;
	rel_dlg.ShowWindow(SW_SHOW);
	rel_dlg.ShowRelSet(asm_solid);
	m_RELButton.SetWindowText(_T("<"));
//	rel_dlg.ShowSWTXList(asm_solid);
}
/************************************************************************/
/* ��ѡ�еĲ����ͳߴ�����������Ա�                                     */
/************************************************************************/
void CAsmParaDefDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int						num=0,Flag=0,postId=0;
	ProName					symbol;
	CString					temp="",error_list="",error_info="";

	num=(int)m_List.GetItemCount();
	switch(m_RadioFlag) {
	case SEL_DIM:
		for (int i=0;i<num;i++) {
					if(!ListView_GetCheckState(m_List,i))  continue;
					//�ж��Ƿ��Ѿ�����
					Flag=0;
					for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) {
						if(p_AsmSmlItemList[j].marsk!=TYPE_DIM) continue;			
						if (Dimensionlist[(int)m_List.GetItemData(i)].id==p_AsmSmlItemList[j].dim.id 
							&& Dimensionlist[(int)m_List.GetItemData(i)].type==p_AsmSmlItemList[j].dim.type 
							&& Dimensionlist[(int)m_List.GetItemData(i)].owner ==p_AsmSmlItemList[j].dim.owner) 
						{
							Flag=1;
							break;
						}
			         }
					if(Flag==1) continue;
					//��ѡ�еĳߴ�����������Ա�
					AsmSmlItem			tempAsm;
					tempAsm.owner=asm_solid;
					tempAsm.marsk=TYPE_DIM;
					ProDimensionSymbolGet(&Dimensionlist[(int)m_List.GetItemData(i)],symbol);
					ProSolidToPostfixId((ProSolid)Dimensionlist[(int)m_List.GetItemData(i)].owner,&postId);
					temp=CString(symbol);
					if (temp=="") temp.Format("d%d:d",Dimensionlist[(int)m_List.GetItemData(i)].id,postId);
					else temp.Format("%s:%d",temp,postId);
					tempAsm.name=temp;
					tempAsm.dim=Dimensionlist[(int)m_List.GetItemData(i)];
					if (!g_RelCollection.IsDimParaInRelSetLeftPart(asm_solid,NULL,&Dimensionlist[(int)m_List.GetItemData(i)]))
					{
						error_list+=tempAsm.name+" ";
						continue;
					}
					p_AsmSmlItemList.Add(tempAsm);
		}
		break;
	case SEL_PARA:
		for (int i=0;i<num;i++) {
					if(!ListView_GetCheckState(m_List,i))  continue;
					//�ж��Ƿ��Ѿ�����
					Flag=0;
					for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) {
						if(p_AsmSmlItemList[j].marsk!=TYPE_PARA) continue;
						//��ѡ�еĲ������������Ա��еĽ��бȽ�
						if (ComparaParameter(&Parameterlist[(int)m_List.GetItemData(i)],&p_AsmSmlItemList[j].para)) 
						{
							Flag=1;
							break;
						}
					}
					if(Flag==1) continue;
					//��ѡ�еĳߴ�����������Ա�
					AsmSmlItem			tempAsm;
					tempAsm.owner=asm_solid;
					tempAsm.marsk=TYPE_PARA;
					ProSolidToPostfixId((ProSolid)(Parameterlist[(int)m_List.GetItemData(i)].owner.type==PRM_MODEL?Parameterlist[(int)m_List.GetItemData(i)].owner.who.model:Parameterlist[(int)m_List.GetItemData(i)].owner.who.item.owner),&postId);
					
					tempAsm.name.Format("%s:%d",CString(Parameterlist[(int)m_List.GetItemData(i)].id),postId);
					tempAsm.para=Parameterlist[(int)m_List.GetItemData(i)];
					//��������ֵ�Ƿ�Ϊ��
					UsrSWTXTable1 temp(UsrGetParameterOwner(&Parameterlist[(int)m_List.GetItemData(i)]),&tempAsm.para);
					if(temp.isCheck!=1) 
					{
						AfxMessageBox("����:"+tempAsm.name+"��ֵΪ��,���������");
						continue;
					}
					if (!g_RelCollection.IsDimParaInRelSetLeftPart(asm_solid,&Parameterlist[(int)m_List.GetItemData(i)],NULL))
					{
						error_list+=tempAsm.name+" ";
						continue;
					}
					p_AsmSmlItemList.Add(tempAsm);
		}
		break;
	case SEL_FEAT:
		break;
	case SEL_PRRT:
		break;
	default:
		break;
	}
	if (!error_list.IsEmpty())
	{
		error_info.Format("�ߴ�(����):%s,�ڹ�ϵʽ���Ѿ���Ϊ����������(��ʽ��ϵʽ���)\n,���ܹ���Ϊ�������Ա����!���޸Ĺ�ϵʽ��,����!",error_list.GetBuffer());
		AfxMessageBox(error_info);
	}
	CString text;
	m_SWTXButton.GetWindowText(text);
	if (text==_T("<")) {
		swtx_dlg.ShowSWTXList(asm_solid);
	}	
}
/************************************************************************/
/* ��ѡ��ť�ߴ�                                                         */
/************************************************************************/
void CAsmParaDefDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioFlag=SEL_DIM;
	if(current_hItem!=m_Tree.GetRootItem())
	{
		ConverthItemToList(current_hItem);
	}
}
/************************************************************************/
/* ��ѡ��ť����                                                         */
/************************************************************************/
void CAsmParaDefDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioFlag=SEL_PARA;
	ShowParaList();

}
/************************************************************************/
/* ��ѡ��ť����                                                         */
/************************************************************************/
void CAsmParaDefDlg::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioFlag=SEL_FEAT;
}
/************************************************************************/
/* ��ѡ��ť���                                                         */
/************************************************************************/
void CAsmParaDefDlg::OnBnClickedRadio4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioFlag=SEL_PRRT;
}
/************************************************************************/
/* ��ʾģ�������еĲ���                                                 */
/************************************************************************/
void CAsmParaDefDlg::ShowParaList()
{
	int          indexNo=0,num=0,PostID=0;
	ProName      AsmcompName;
	ProModelitem modelitem;
	m_List.DeleteAllItems();
	do {		
	} while(m_List.DeleteColumn(0));
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,100,0);
	m_List.InsertColumn(1,"����",LVCFMT_LEFT,80,0);
	m_List.InsertColumn(2,"ֵ",LVCFMT_LEFT,80,0);
	m_List.InsertColumn(3,"����Ԫ��",LVCFMT_LEFT,100,0);
	//����������飬���·���
	Parameterlist.RemoveAll();
	//�������еĲ���
		
	ProMdlToModelitem(asm_solid,&modelitem);
	ProParameterVisit((ProModelitem*)&modelitem,NULL,(ProParameterAction)partAsmParaVisAc,
		(CArray<ProParameter,ProParameter>*)&Parameterlist);
	num=(int)asm_feat_list.GetCount();
	for (int i=0;i<num;i++) {
		ProFeattype    ftype;
		ProFeatureTypeGet(&asm_feat_list[i], &ftype);
		if (ftype == PRO_FEAT_COMPONENT)
		{
		
			ProSolid              mdl;
			ProAsmcompMdlGet((ProAsmcomp *)(&asm_feat_list[i]),(ProMdl *)&mdl);
			ProMdlToModelitem(mdl,&modelitem);
			ProParameterVisit((ProModelitem*)&modelitem,NULL,(ProParameterAction)partAsmParaVisAc,
				(CArray<ProParameter,ProParameter>*)&Parameterlist);
		}
	}
	for(int i=0;i<(int)Parameterlist.GetCount();i++)
	{

		ProParamvalue proval;
		ProCharName str_id;
		CString csType;
		CString csValue;
		ProWstringToString(str_id,Parameterlist[i].id);
		ProParameterValueGet(&Parameterlist[i],&proval);
        ProSolidToPostfixId((ProSolid)(Parameterlist[i].owner.type==PRM_MODEL?Parameterlist[i].owner.who.model:Parameterlist[i].owner.who.item.owner)
			,&PostID);
		sprintf(str_id,"%s:%d",str_id,PostID);
        indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,i,str_id,0,0,0,i);
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
		m_List.SetItemText(indexNo,1,csType);
		m_List.SetItemText(indexNo,2,csValue);		
		ProMdlNameGet(Parameterlist[i].owner.type==PRM_MODEL?Parameterlist[i].owner.who.model:Parameterlist[i].owner.who.item.owner 
			,AsmcompName);
		m_List.SetItemText(indexNo,3,CString(AsmcompName));

		//�ж��Ƿ��Ѿ����������Ա���
		for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) {
			if(p_AsmSmlItemList[j].marsk!=TYPE_PARA) continue;
			if(p_AsmSmlItemList[j].owner!=asm_solid) continue;
			//��ѡ�еĲ������������Ա��еĽ��бȽ�
			if (ComparaParameter(&Parameterlist[i],&p_AsmSmlItemList[j].para)) 
			{ 
				ListView_SetCheckState(m_List,indexNo,TRUE);
				break;
			}
		}
	}
}
/************************************************************************/
/* ��ģ�͵Ĳ������в���                                                 */
/************************************************************************/
void CAsmParaDefDlg::OnBnClickedAsmParadefButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CParaOperaDlg dlg;
	dlg.DoModal();
}

void CAsmParaDefDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: �ڴ������Ϣ����������
	CString   text;
	m_RELButton.GetWindowText(text);
	if((WindowFromPoint(point)==&m_List))
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(4);
		ASSERT(pPopup!=NULL);
		if(text!=_T("<")) pPopup->EnableMenuItem(ID_12058,MF_DISABLED|MF_GRAYED);
		if(m_RadioFlag!=SEL_DIM) pPopup->EnableMenuItem(ID_12076,MF_DISABLED|MF_GRAYED);
		pPopup->DeleteMenu(1,MF_BYPOSITION);;
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);

	}
}
/************************************************************************/
/* ��ѡ�еĳߴ�Ͳ������뵽��ϵʽ��ȥ                                   */
/************************************************************************/
void CAsmParaDefDlg::OnInsertToRel()
{
	// TODO: �ڴ���������������
	int num=0;
	num=(int)m_List.GetItemCount();
	for (int i=0;i<num;i++) {
		if (m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED) {			
			rel_dlg.m_RelEdit.ReplaceSel(m_List.GetItemText(i,0),FALSE);
			rel_dlg.m_RelEdit.SetFocus();
		}
	}
}
/************************************************************************/
/* ��ѡ�еĳߴ���б༭                                                 */
/************************************************************************/
void CAsmParaDefDlg::OnEditDim()
{
	// TODO: �ڴ���������������
	CDimEditDlg dlg;

	for( int i=0;i<m_List.GetItemCount();i++)
	{
		//ѡ��ѡ�е���
		if(m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			
			int     index=(int)m_List.GetItemData(i);
			int     postID=0;
			CString id,cSymbol;
			ProName symbol;
			double  value,upper_limit,lower_limit;

			ProBoolean rel_driven; 
			ProDimensionIsReldriven (&Dimensionlist[index],&rel_driven);
			if(rel_driven==PRO_B_TRUE) 
			{AfxMessageBox("�ߴ�Ϊ��ϵʽ�����������޸ģ�");
			return;}

			ProSolidToPostfixId((ProSolid)Dimensionlist[index].owner,&postID);
			id.Format("d%d:%d",Dimensionlist[index].id,postID);

			ProDimensionSymbolGet(&Dimensionlist[index],symbol);
			cSymbol=CString(symbol);            
			ProDimensionValueGet(&Dimensionlist[index],&value);
            ProDimensionToleranceGet(&Dimensionlist[index],&upper_limit,&lower_limit);
            
			dlg.m_ID=id;
			dlg.m_Symbol=cSymbol;
			dlg.m_Value=(float)value;
			dlg.m_UpLimit=(float)upper_limit;
			dlg.m_DwLimit=(float)lower_limit;

			if(dlg.DoModal()==IDOK)
			{
				ProName symbol;
				ProStringToWstring(symbol,dlg.m_Symbol.GetBuffer());
				if(ProDimensionSymbolSet(&Dimensionlist[index],symbol)!=PRO_TK_NO_ERROR) AfxMessageBox("ʧ�ܣ�");
				ProDimensionValueSet(&Dimensionlist[index],dlg.m_Value);
				ProDimensionToleranceSet(&Dimensionlist[index],dlg.m_UpLimit,dlg.m_DwLimit);
			}
		}
	}
	ProSolidRegenerate(asm_solid,PRO_REGEN_NO_FLAGS);
	FreshListByDim();
}

/************************************************************************/
/* ����������ʾ��Ӧ�ĳߴ�                                           */
/************************************************************************/
void CAsmParaDefDlg::OnNMClickAsmParadefList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch(m_RadioFlag) {
	case SEL_DIM:
		//
		if(m_DimDisplayModeCheckbox.GetState()==1)
		{
			ProDisplaymodeSet(PRODISPMODE_SYMBOLIC);
		}
		else
		{
			ProDisplaymodeSet(PRODISPMODE_NUMERIC);
		}
		for( int i=0;i<m_List.GetItemCount();i++)
		{
			//ѡ��ѡ�е���
			if(m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
			{

				CArray<int,int> memb_id_tab;
				ProIdTable		memb_id_tab_1;
				ProAsmcomppath path;
				GetSolidIDTable(asm_solid,&asm_feat_list,current_hItem,&m_Tree,&memb_id_tab);
				for (int j=0;j<(int)memb_id_tab.GetCount();j++) {
					memb_id_tab_1[j]=memb_id_tab[j];
				}
				ProAsmcomppathInit(asm_solid,memb_id_tab_1,(int)memb_id_tab.GetCount(),&path);
				if(ProDimensionShow(&Dimensionlist[(int)m_List.GetItemData(i)],NULL,NULL,&path)!=PRO_TK_NO_ERROR)
				{
					//��������ʾʧ��,������ߴ������װ�伶�ߴ�,���Գ���ֱ����ʾ
					//AfxMessageBox("��ʾ�ߴ�ʧ�ܣ�");
					if(ProDimensionShow(&Dimensionlist[(int)m_List.GetItemData(i)],NULL,NULL,NULL)!=PRO_TK_NO_ERROR) AfxMessageBox("��ʾ�ߴ�ʧ�ܣ�");
				}
			}
		}
		break;
	default:
		break;
	}
	*pResult = 0;
}
void CAsmParaDefDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnClose();
}
