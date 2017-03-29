// AsmRelVistDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "AsmRelVistDlg.h"
//extern int DlgIcon;
//BOOL VisitSolidRel(ProSolid solid,CStringArray * relSet);
//ProError FeatureAsmVisitAcFn(ProFeature *p_object,ProError status,
//							 CArray<ProFeature,ProFeature> * app_data) ;
//void  ReplaceDimWithPostID(CString * str,CString postid);
//extern CArray<AsmRelItem,AsmRelItem> p_AsmRelItemList;
//extern CArray<UsrRelTable,UsrRelTable>    p_UsrRelTableList; 
// CAsmRelVistDlg �Ի���

IMPLEMENT_DYNAMIC(CAsmRelVistDlg, CDialog)
CAsmRelVistDlg::CAsmRelVistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmRelVistDlg::IDD, pParent)
	,solid(NULL)
	, relSet(NULL)
	,relNote(NULL)
{
}

CAsmRelVistDlg::~CAsmRelVistDlg()
{
}

void CAsmRelVistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_ASM_REL_VISIT_LIST, m_List);
}


BEGIN_MESSAGE_MAP(CAsmRelVistDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CAsmRelVistDlg ��Ϣ�������

BOOL CAsmRelVistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
    m_List.InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
	m_List.InsertColumn(1,"��ϵʽ",LVCFMT_LEFT,140,-1);
	m_List.InsertColumn(2,"˵��",LVCFMT_LEFT,100,-1);
	if (solid==NULL) 
	{
		
		if(ProMdlCurrentGet((ProMdl*)&solid)!=PRO_TK_NO_ERROR) return FALSE;
	}
	VistRel(solid);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ����ģ���еĹ�ϵ                                                     */
/************************************************************************/
void CAsmRelVistDlg::VistRel(ProSolid mdl)
{

	CArray<ProFeature,ProFeature> p_FeatList;
	CStringArray                  relSet;
	int                           num=0;
	ProName                       name;
	int                           indexNo=0;
	ProFeattype                   ftype;
	CString                       tempRel1;
	CString                       tempRel2;
	ProMdlNameGet(mdl,name);
	VisitSolidRel(mdl,&relSet);
	num=(int)relSet.GetCount();
	for (int i=0;i<num;i++) {
		CString     cIndexNo;
		int         Falg=0;
		if (relSet[i].Find(':',0)==-1) {
			int id;
			CString csId;
			ProSolidToPostfixId(mdl,&id);
			csId.Format("%d",id);
			ReplaceDimWithPostID(&relSet[i],csId);
		}
		for (int m=0;m<m_List.GetItemCount();m++) {
			if(relSet[i]==m_List.GetItemText(m,1)) 
			{
				Falg=1;
				break;
			}
		}
		if(Falg==1) continue;

		cIndexNo.Format("%d",m_List.GetItemCount()+1);
        indexNo=m_List.InsertItem(m_List.GetItemCount(),cIndexNo);	
		m_List.SetItemText(indexNo,1,relSet[i]);
		m_List.SetItemText(indexNo,2,_T("����Ԫ����"+CString(name)));
		//�������Ƿ��Ѿ����
		if (g_RelCollection.IsRelInCollection(mdl,relSet[i].MakeLower())) {
			ListView_SetCheckState(m_List,indexNo,TRUE);
		}		
	}
	////�����������ģ���߽��Ĺ�ϵʽ
	//num=(int)p_UsrRelTableList.GetCount();
	//for (int i=0;i<num;i++) {
	//	if(p_UsrRelTableList[i].owner!=(ProMdl)mdl) continue;
	//	if(p_UsrRelTableList[i].isCheck!=1) continue;

	//	CString     cIndexNo;
	//	CString     rel=p_UsrRelTableList[i].rel;
	//	
	//	if (rel.Find(":",0)==-1) {
	//		int id;
	//		CString csId;
	//		ProSolidToPostfixId(mdl,&id);
	//		csId.Format("%d",id);
	//		ReplaceDimWithPostID(&rel,csId);
	//	}
	//	//�ж���û���Ѿ�����
	//	int         Falg=0;
	//	for (int m=0;m<m_List.GetItemCount();m++) {
	//		if(rel==m_List.GetItemText(m,1)) 
	//		{
	//			Falg=1;
	//			break;
	//		}
	//	}
	//	if(Falg==1) continue;

	//	cIndexNo.Format("%d",m_List.GetItemCount()+1);
	//	indexNo=m_List.InsertItem(m_List.GetItemCount(),cIndexNo);	
	//	m_List.SetItemText(indexNo,1,rel);
	//	m_List.SetItemText(indexNo,2
	//		,p_UsrRelTableList[i].note==_T("")?_T("����Ԫ����"+CString(name)):p_UsrRelTableList[i].note);
	//	//�������Ƿ��Ѿ����
	//	for (int j=0;j<(int)p_AsmRelItemList.GetSize();j++) {
	//		if(p_AsmRelItemList[j].owner!=solid) continue;
	//		tempRel1=rel;tempRel2=p_AsmRelItemList[j].relation;
	//		if (tempRel1.MakeLower()==tempRel2.MakeLower()) {
	//			ListView_SetCheckState(m_List,indexNo,TRUE);
	//			break;
	//		}
	//	}

	//}
	////װ����е����Ԫ�����Ĺ�ϵ
	//num=(int)p_AsmRelItemList.GetCount();
	//for (int i=0;i<num;i++) {
	//	if(p_AsmRelItemList[i].owner!=mdl) continue;
	//	CString     cIndexNo;

	//	int         Falg=0;
	//	for (int m=0;m<m_List.GetItemCount();m++) {
	//		if(p_AsmRelItemList[i].relation==m_List.GetItemText(m,1)) 
	//		{
	//			Falg=1;
	//			break;
	//		}
	//	}
	//	if(Falg==1) continue;

	//	cIndexNo.Format("%d",m_List.GetItemCount()+1);
	//	indexNo=m_List.InsertItem(m_List.GetItemCount(),cIndexNo);
	//	m_List.SetItemText(indexNo,1,p_AsmRelItemList[i].relation);
	//	m_List.SetItemText(indexNo,2
	//		,p_AsmRelItemList[i].note==_T("")?_T("����Ԫ����"+CString(name)):p_AsmRelItemList[i].note);
	//	//�������Ƿ��Ѿ����
	//	for (int j=0;j<(int)p_AsmRelItemList.GetSize();j++) {
	//		if(p_AsmRelItemList[j].owner!=solid) continue;
	//		tempRel1=p_AsmRelItemList[i].relation;
	//		tempRel2=p_AsmRelItemList[j].relation;
	//		if (tempRel1.MakeLower()==tempRel2.MakeLower()) {
	//			ListView_SetCheckState(m_List,indexNo,TRUE);
	//			break;
	//		}
	//	}
	//}
	//��������
	ProSolidFeatVisit(mdl,(ProFeatureVisitAction)FeatureAsmVisitAcFn
		,NULL,(CArray<ProFeature,ProFeature>*)&p_FeatList);
    num=(int)p_FeatList.GetCount();
	for (int i=0;i<num;i++) {
		ProFeatureTypeGet (&p_FeatList[i], &ftype);
		if (ftype==PRO_FEAT_COMPONENT) {
			ProAsmcompMdlGet((ProAsmcomp *)(&p_FeatList[i]),(ProMdl *)&mdl);
			VistRel((ProSolid)mdl);
		}
	}
}

void CAsmRelVistDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString rel;
	CString note;
	if (relSet==NULL) return;
	for (int i=0;i<m_List.GetItemCount();i++) {
		if(!ListView_GetCheckState(m_List,i)) continue;
		rel=m_List.GetItemText(i,1);
		note=m_List.GetItemText(i,2);
		relSet->Add(rel);
		relNote->Add(note);
	}
	OnOK();
}
