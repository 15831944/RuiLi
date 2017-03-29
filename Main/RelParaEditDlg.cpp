// RelParaEditDlg.cpp : ʵ���ļ�
//�����������Ժ͹�ϵʽ�Լ�ģ�͵ĳߴ磬�����������������

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
//����ͼ��
void SetImageListForFeatrure(CImageList * p_imageList);
int GetFeatIconNum(int type);
// CRelParaEditDlg �Ի���

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

// CRelParaEditDlg ��Ϣ�������
/***************************************************************************************\
������pGetFeats()
˵������������������е�����
\***************************************************************************************/
void CRelParaEditDlg::pGetFeats(ProSolid part,CArray<ProFeature,ProFeature> * p_FeatureList)
{
    ProSolidFeatVisit((ProSolid)part,(ProFeatureVisitAction)partFeatVisAc,
		NULL,(CArray<ProFeature,ProFeature> *)p_FeatureList);
	
}
/****************************************************************************************\
������OnInitDialog()
˵������ʼ���Ի��������ؼ�����ʾ��������
\****************************************************************************************/
BOOL CRelParaEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//1.���õ�ѡ��ť���ߴ���Ĭ�ϱ�ѡ��
	radioFalg=DIM_CHECKED;
	m_RadioButtonValue=0;
	//m_radiobutton1.SetCheck(BST_CHECKED);

	//2.���ó�ʼ�б����ʾ�ߴ磬�����б��ķ��
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	for(int i=0;i<6;i++)
	{
		m_List.DeleteColumn(0);
	}
	
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,100,0);
	m_List.InsertColumn(1,"����",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(2,"ֵ",LVCFMT_LEFT,80,0);
	m_List.InsertColumn(3,"��ƫ��",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(4,"��ƫ��",LVCFMT_LEFT,50,0);

	//3.��������ͼ��
	//���ͼ��
	imageList.Create(12, 12, ILC_COLOR16 , 4, 4);
	SetImageListForFeatrure(&imageList);
	m_Tree.SetImageList(&imageList,TVSIL_NORMAL);
    //3.�жϵ�ǰģ�͵�������
	ProMdlCurrentGet((ProMdl*)&current_solid);    
	prtShowTree(current_solid);  
	
    ////////////////////////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

/****************************************************************************\
������prtShowTree(ProSolid solid)
˵������������ʾ���ģ�͵����пɼ�������
\****************************************************************************/
void CRelParaEditDlg::prtShowTree(ProSolid solid)
{
	ProName      name;
	ProCharName  strName;
	int          numOfFeat=0;	
	ProMdlNameGet(solid,name);
	ProWstringToString(strName,name);
    //�Ȱ�ԭ�ȵĶ�ɾ��
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
 			sprintf(strName,"%s ��ʶ��%d",strName,FeatureList[i].id);
		}
		else if(type==PRO_TK_BAD_INPUTS)
		{
			sprintf(strName,"��Ч");
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
/*������OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)  
  ˵���������е����ʱ��õ�����ľ����Ȼ�����ConverthItemToList(hItem)*/
/************************************************************************/
void CRelParaEditDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
 {
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		if(status!=PRO_TK_NO_ERROR) AfxMessageBox("���󣬲��ܼ�����ʾ��");
	}
	if (hItem!=m_Tree.GetRootItem()) {
		//�༭��������·��
		pItem=hItem;
		do {
			pItem=m_Tree.GetParentItem(pItem);
			path=m_Tree.GetItemText(pItem)+"\\"+path;
		} while(pItem!=m_Tree.GetRootItem());

	switch(radioFalg) {
	case DIM_CHECKED:
		ConverthItemToList(hItem);
		break;
	case PARA_CHECKED:    //����ǲ�����ťѡ��,���л����ߴ�
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
������ConverthItemToList(HTREEITEM hItem)
˵������ѡ�����е�ĳһ�������õ�����ľ�������������ڴ洢�������ĺţ��õ�������
      �����ʸ����������еĳߴ磬�����б�����ʾ����
\******************************************************************************/
void CRelParaEditDlg::ConverthItemToList(HTREEITEM hItem)
{

	ProError status;
	ProFeature termFeat;
    termFeat=FeatureList[(int)m_Tree.GetItemData(hItem)];
    Dimensionlist.RemoveAll();
	status=ProFeatureDimensionVisit(&termFeat,(ProDimensionVisitAction)partFeatDimVisAc ,
		NULL,(CArray<ProDimension,ProDimension> *)&Dimensionlist);
	
	//ˢ��
    FreshDim();

}
/************************************************************************/
/* ������OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
   ˵�������б���ڵ��������ʱ�򣬷���ѡ����ĳߴ磬��ģ������ʾ�ߴ�    */
/************************************************************************/
void CRelParaEditDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		//��ѡ�е�����ߴ�����Ƚϣ���ʾ��Ӧ�ĳߴ�
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
/* ������OnBnClickedButton2()
   ˵��������������Ա�ť֮�󣬴��Ҳ���������Ա�,����ʼ��          */
/************************************************************************/
void CRelParaEditDlg::OnBnClickedButton2()
{
	CString   text;
	CRect     rect;
	CRect     rect1;
	CRect     rect2;
	m_RELATIONButton.GetWindowText(text);
	if (text==_T("<")) {
		//�ر�
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
	//��

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
/* ������OnBnClickedButton3()
   ˵������ѡ�е���Ŀ��ӵ��������Ա��У������֮ǰҪ�����жϣ��Ƿ��Ѿ�
         �������Ա���                                                   */
/************************************************************************/
void CRelParaEditDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�ж����ͣ����ݲ�ͬ���ͣ�����ѡ��
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
/* �����ĸ��ǵ�ѡ��ť��״̬                                             */
/************************************************************************/
void CRelParaEditDlg::OnBnClickedCom1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
/* ������OnBnClickedCom2()
   ˵����ѡ����ѡ��ť֮�󣨲������������еĲ�����ӵ��б����           */
/************************************************************************/
void CRelParaEditDlg::OnBnClickedCom2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    radioFalg=PARA_CHECKED;
	int indexNo=0;
	m_List.DeleteAllItems();
	for(int i=0;i<10;i++)
	{
        m_List.DeleteColumn(0);
	}
	
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	//m_List.InsertColumn(0,"��ʶ",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,100,0);
	m_List.InsertColumn(1,"����",LVCFMT_LEFT,80,0);
	m_List.InsertColumn(2,"ֵ",LVCFMT_LEFT,80,0);

    ConverthItemOfParaToList();//�Ѳ�����ӵ��б��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	radioFalg=FEAT_CHECKED;
}

void CRelParaEditDlg::OnBnClickedCom4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	radioFalg=COMP_CHECKED;
}
/************************************************************************/
/* ������AddDimToSWTXList(void)
   ˵�����ڳߴ���ʾģ��ʽ�£���ѡ�еĳߴ���ӵ��������Ա�
         �����Ҫ�����жϣ������Ƿ��Ѿ�����ߴ���                       */
/************************************************************************/
void CRelParaEditDlg::AddDimToSWTXList(void)
{	
    int         num; 
	CString     id;
	CString     error_dimlist="";
	CString     error_info="";
	//�ߴ�����Ϣ����ı���

	num=(int)p_UsrSWTXTableList.GetCount();

		//�����ݽ��й��ˣ�ȥ�����ǵ�ǰģ�ͣ��Լ��Ѿ���ɾ��������
		//���ǳߴ��

	for(int j=0;j<m_List.GetItemCount();j++)
		{
			//����б��е�J��û�б�ѡ�У�����ѭ��
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
			//��ȡѡ�������Ϣ
			//����һ����ʱUsrSWTXTable
            
			p_UsrSWTXTableList.Add(temp_UsrSWTXTable);        //��ӵ�������

		}
	if (!error_dimlist.IsEmpty())
	{
		error_info.Format("�ߴ�:%s,�ڹ�ϵʽ���Ѿ���Ϊ����������(��ʽ��ϵʽ���)\n,���ܹ���Ϊ�������Ա����!���޸Ĺ�ϵʽ��,����!\0",error_dimlist);
		AfxMessageBox(error_info);
	}
	//ˢ���б��2
	CString text;
	m_SWTXButtong.GetWindowText(text);
	if (text==_T("<")) {
		swtx_dlg.ShowSWTXList(current_solid);
	}	
}
/************************************************************************/
/* ������AddParaToSWTXList(void)
   ˵�����Ѳ�����ӵ��������Ա�                                          */
/************************************************************************/
void CRelParaEditDlg::AddParaToSWTXList(void)
{
	int					num;                     //��¼�Զ���ṹp_UsrSWTXTableList�����ݴ�С
	CString				id;
	CString				csValue;
	CString				csType;
	ProParamvalue		proval;   
	CString				error_paralist="";
	CString				error_info;
	num=(int)p_UsrSWTXTableList.GetCount();

	for(int j=0;j<m_List.GetItemCount();j++)
	{
															//����б��е�J��û�б�ѡ�У�����ѭ��
		if(!ListView_GetCheckState(m_List,j))  continue;
															//��ѡ�е�������еĲ������бȽϣ������ʶ����ͬ����ȷ����ѡ�е���
		id=CString(Parameterlist[(int)m_List.GetItemData(j)].id);

															//�ų��Ѿ����ڵĲ���

		UsrSWTXTable1 temp_UsrSWTXTable(current_solid,&Parameterlist[m_List.GetItemData(j)]);
		int flag=0;
		for(int i=0;i<num;i++)
		{
			if(p_UsrSWTXTableList[i].owner!=(ProMdl)current_solid) continue;   //�ų����ǵ�ǰģ�͵Ĳ���
			if(p_UsrSWTXTableList[i].isCheck!=1) continue;             //�ų��Ѿ���ɾ���Ĳ���
			if(p_UsrSWTXTableList[i].type!="para") continue;
			if(ComparaParameter(&Parameterlist[m_List.GetItemData(j)],&p_UsrSWTXTableList[i].Para))  {flag=1;break;} 
		}
        //�жϲ����Ƿ��Ѿ��ڹ�ϵ�����
		if (!g_RelCollection.IsDimParaInRelSetLeftPart(current_solid,&Parameterlist[m_List.GetItemData(j)],NULL))
		{
			error_paralist+=temp_UsrSWTXTable.Name+" ";
			flag=1;
		}
		if(flag==1) continue;
															//��ȡѡ�������Ϣ
        if(flag==1) continue;
		ProParameterValueGet(&Parameterlist[m_List.GetItemData(j)],&proval);
        if(proval.type==PRO_PARAM_STRING && CString(proval.value.s_val).IsEmpty()) 
		{
			AfxMessageBox(_T("������"+id+"����ֵΪ��,�������"));
			continue;
		}
		p_UsrSWTXTableList.Add(temp_UsrSWTXTable);			 //��ӵ�������

	}	
	if (!error_paralist.IsEmpty())
	{
		error_info.Format("����:%s,�ڹ�ϵʽ���Ѿ���Ϊ����������(��ʽ��ϵʽ���)\n,���ܹ���Ϊ�������Ա����!���޸Ĺ�ϵʽ��,����!",error_paralist);
		AfxMessageBox(error_info);
	}//ˢ���б��2
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
/*����:ConverthItemOfParaToList
  ˵��������ģ�͵Ĳ���                                                  */
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
/* �Բ������б༭                                                       */
/************************************************************************/
void CRelParaEditDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	m_ParaOperaDlg.DoModal();

}
/************************************************************************/
/* ������OnBnClickedCheck1()
   ˵�����ж�CHECK��ť��״̬                                            */
/************************************************************************/
void CRelParaEditDlg::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
/************************************************************************/
/* ������OnContextMenu����
   ˵�����Ҽ��˵�,���������������Ա��еĲ˵�                            */
/************************************************************************/
void CRelParaEditDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: �ڴ������Ϣ����������
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
�رնԻ���֮�󣬼����
\*****************************************************************************/
void CRelParaEditDlg::OnClose()
{	
	CDialog::OnClose();
}
/************************************************************************/
/* ������OnBnClickedButton1()
   ˵�����򿪹�ϵʽ�༭*/
/************************************************************************/
void CRelParaEditDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString		text;
	CRect		rect;
	CRect		rect1;
	CRect		rect2;

	m_SWTXButtong.GetWindowText(text);
	if (text=="<") {
		//�ر�
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

	//��
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
/* �Ҽ��˵����ѳߴ磬��ϵ����ӵ���ϵʽ��ȥ                             */
/************************************************************************/
void CRelParaEditDlg::On12058()
{
	// TODO: �ڴ���������������
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
/* ˢ��m_List,ʹ����ӵĲ�����ɾ���Ĳ����õ�����                        */
/************************************************************************/
void CRelParaEditDlg::On12059()
{
	// TODO: �ڴ���������������
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
/* �Ҽ��˵��жԳߴ����ֵ�����ţ�����ƫ��Ƚ����޸�                     */
/************************************************************************/
void CRelParaEditDlg::OnDimEditDlg()
{
	// TODO: �ڴ���������������
    CDimEditDlg dlg;	
	for( int i=0;i<m_List.GetItemCount();i++)
	{
		//ѡ��ѡ�е���
		if(m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
            ProBoolean      rel_driven; 
			ProDimensionIsReldriven (&Dimensionlist[m_List.GetItemData(i)],&rel_driven);
			if(rel_driven==PRO_B_TRUE) 
			{AfxMessageBox("�ߴ�Ϊ��ϵʽ�����������޸ģ�");
			return;}
            //����ȵĳߴ�����޸�
			dlg.m_ID.Format("d%d",Dimensionlist[m_List.GetItemData(i)].id);
			dlg.m_Symbol=m_List.GetItemText(i,0);
			dlg.m_Value=(float)atof(m_List.GetItemText(i,2).GetBuffer());
			dlg.m_UpLimit=(float)atof(m_List.GetItemText(i,3).GetBuffer());
			dlg.m_DwLimit=(float)atof(m_List.GetItemText(i,4).GetBuffer());

			if(dlg.DoModal()==IDOK)
			{

				ProName symbol;
				ProStringToWstring(symbol,dlg.m_Symbol.GetBuffer());
				if(ProDimensionSymbolSet(&Dimensionlist[m_List.GetItemData(i)],symbol)!=PRO_TK_NO_ERROR) AfxMessageBox("ʧ�ܣ�");
				ProDimensionValueSet(&Dimensionlist[m_List.GetItemData(i)],dlg.m_Value);
				ProDimensionToleranceSet(&Dimensionlist[m_List.GetItemData(i)],dlg.m_UpLimit,dlg.m_DwLimit);
			}			
			break;
		}
	}
    //ˢ��
	ProSolid		solid;
	ProMdlCurrentGet((ProMdl*)&solid);
	ProSolidRegenerate((ProSolid)solid,PRO_REGEN_NO_FLAGS);
    FreshDim();	
}
/************************************************************************/
/* �Ҽ��˵��У����б��ĳߴ����ˢ��                                   */
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

	m_List.InsertColumn(0,"����",LVCFMT_LEFT,100,0);
	m_List.InsertColumn(1,"����",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(2,"ֵ",LVCFMT_LEFT,80,0);
	m_List.InsertColumn(3,"��ƫ��",LVCFMT_LEFT,50,0);
	m_List.InsertColumn(4,"��ƫ��",LVCFMT_LEFT,50,0);
	
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//if (m_FeatDisplayMode.GetState()==1)
	//{
	//	AfxMessageBox("ѡ��");
	//}
	//m_FeatDisplayMode.SetCheck();
}
