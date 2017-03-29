// AsmParaDefDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ASM.h"
#include "AsmParaDefDlg.h"
#define  SEL_DIM  0
#define  SEL_PARA 1
#define  SEL_FEAT 2
#define  SEL_PRRT 3

extern CArray<AsmSmlItem,AsmSmlItem> p_AsmSmlItemList;
ProError FeatureAsmVisitAcFn(ProFeature *p_object,ProError status,
							 CArray<ProFeature,ProFeature> * app_data) ;
ProError FeatureAsmFilterAcFn(ProFeature *p_object,
							  CArray<ProFeature,ProFeature> * app_data) ;
//�����ĳߴ���ʺ�����ͬMAIN, ��������
ProError partFeatDimVisAc(ProDimension *dimension,ProError status,
						  CArray<ProDimension,ProDimension> * app_data);
ProError partAsmParaVisAc(ProParameter* p_parameter,ProError status,
					   CArray<ProParameter,ProParameter> * app_data);
// CAsmParaDefDlg �Ի���
BOOL ComparaParameter(ProParameter * para1,ProParameter * para2);

IMPLEMENT_DYNAMIC(CAsmParaDefDlg, CDialog)
CAsmParaDefDlg::CAsmParaDefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmParaDefDlg::IDD, pParent)
	, m_RadioFlag(0)
{
}

CAsmParaDefDlg::~CAsmParaDefDlg()
{
}

void CAsmParaDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_BUTTON3, m_SWTXButton);
	DDX_Control(pDX, IDC_BUTTON4, m_RELButton);
	DDX_Control(pDX, IDC_EDIT1, m_PathEdit);
}


BEGIN_MESSAGE_MAP(CAsmParaDefDlg, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
END_MESSAGE_MAP()


// CAsmParaDefDlg ��Ϣ�������

BOOL CAsmParaDefDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//�����б�
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	//���ͼ��
	imageList.Create(12, 12, ILC_COLOR16 , 4, 4);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_CSYS));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_DTM));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_FEAT));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_LINE));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_POT));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
    m_Tree.SetImageList(&imageList,TVSIL_NORMAL);
	//���ģ����
	if (ProMdlCurrentGet((ProMdl*)&asm_solid)==PRO_TK_NO_ERROR) {
		ProName name;
		ProMdlNameGet(asm_solid,name);
		//����ϵʽ��ʵ�帶ֵ
		rel_dlg.asm_solid=asm_solid;
		m_Tree.DeleteAllItems();
		asm_feat_list.RemoveAll();
		HTREEITEM hItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM,_T(CString(name)+".ASM")
			,0,0,0,0,-1,NULL,NULL);
		ShowModelTree(asm_solid,hItem);
		m_Tree.Expand(m_Tree.GetRootItem(),TVE_EXPAND );
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
	ProError   status;
	CString    csName;
	ProName    wName;
	HTREEITEM  tempItem;
	ProMdlType    type;
	int        imageNum=0;
	CArray<ProFeature,ProFeature> feat_list;
	status=ProSolidFeatVisit(solid,(ProFeatureVisitAction)FeatureAsmVisitAcFn
		,NULL/*(ProFeatureFilterAction)FeatureAsmFilterAcFn*/,(CArray<ProFeature,ProFeature>*)&feat_list);
	int num=(int)feat_list.GetCount();
	for (int i=0;i<num;i++) {
		ProFeattype    ftype;
		status = ProFeatureTypeGet (&feat_list[i], &ftype);

		if (ftype == PRO_FEAT_COMPONENT){
            ProSolid              mdl;
			ProAsmcompMdlGet((ProAsmcomp *)(&feat_list[i]),(ProMdl *)&mdl);
			ProMdlNameGet(mdl,wName);
			csName=CString(wName);
			ProMdlTypeGet(mdl,&type);
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
			switch(ftype) {
			case PRO_FEAT_DATUM:
				imageNum=2;
				break;
			case PRO_FEAT_DATUM_POINT:
				imageNum=5;
				break;
			case PRO_FEAT_DATUM_AXIS:
				imageNum=4;
				break;
			case PRO_FEAT_CSYS:
				imageNum=1;
				break;
			default:
				imageNum=3;
				break;
			}
			tempItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM,csName
				,imageNum,imageNum,0,0,asm_feat_list.Add(feat_list[i]),hItem,NULL);			
		}
	}
}
void CAsmParaDefDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    CString path;
	HTREEITEM pItem;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	path=m_Tree.GetItemText(hItem);
	if (hItem!=m_Tree.GetRootItem()) {
		pItem=hItem;
		do {
			pItem=m_Tree.GetParentItem(pItem);
            path=m_Tree.GetItemText(pItem)+"\\"+path;
		} while(pItem!=m_Tree.GetRootItem());
	}
	m_PathEdit.SetWindowText(path);
	switch(m_RadioFlag) {
	case SEL_DIM:
		ConverthItemToList(hItem);
		break;
	case SEL_PARA:
		ShowParaList();
		break;
	case SEL_FEAT:
		break;
	case SEL_PRRT:
		break;
	default:
		break;
	}
	*pResult = 0;
}

void CAsmParaDefDlg::ConverthItemToList(HTREEITEM hItem)
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
	ProError        status;
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
	Dimensionlist.RemoveAll();
    //���������ĳߴ�
	status=ProFeatureDimensionVisit(&asm_feat_list[(int)m_Tree.GetItemData(hItem)],(ProDimensionVisitAction)partFeatDimVisAc ,
		NULL,(CArray<ProDimension,ProDimension> *)&Dimensionlist);
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
		//m_List.SetItemData(indexNo,i);		
		//m_List.SetItemText(indexNo,1,cSymbol);
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
	int       num=0;
	ProName   symbol;
	CString   temp;
	int       Flag=0;
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
			AsmSmlItem tempAsm;
			tempAsm.owner=asm_solid;
			tempAsm.marsk=TYPE_DIM;
			ProDimensionSymbolGet(&Dimensionlist[(int)m_List.GetItemData(i)],symbol);
			temp=CString(symbol);
			if (temp=="") temp.Format("d%d",Dimensionlist[(int)m_List.GetItemData(i)].id);
			tempAsm.name=temp;
			tempAsm.dim=Dimensionlist[(int)m_List.GetItemData(i)];
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
			AsmSmlItem tempAsm;
			tempAsm.owner=asm_solid;
			tempAsm.marsk=TYPE_PARA;
			tempAsm.name=CString(Parameterlist[(int)m_List.GetItemData(i)].id);
			tempAsm.para=Parameterlist[(int)m_List.GetItemData(i)];
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
	
	swtx_dlg.ShowSWTXList(asm_solid);
}
/************************************************************************/
/* ��ѡ��ť�ߴ�                                                         */
/************************************************************************/
void CAsmParaDefDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RadioFlag=SEL_DIM;
	ConverthItemToList(m_Tree.GetRootItem());
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
	int          indexNo=0;
	int          num=0;
	int          PostID;
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
