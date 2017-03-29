// AsmRelVistDlg.cpp : 实现文件
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
// CAsmRelVistDlg 对话框

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


// CAsmRelVistDlg 消息处理程序

BOOL CAsmRelVistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
    m_List.InsertColumn(0,"编号",LVCFMT_LEFT,40,-1);
	m_List.InsertColumn(1,"关系式",LVCFMT_LEFT,140,-1);
	m_List.InsertColumn(2,"说明",LVCFMT_LEFT,100,-1);
	if (solid==NULL) 
	{
		
		if(ProMdlCurrentGet((ProMdl*)&solid)!=PRO_TK_NO_ERROR) return FALSE;
	}
	VistRel(solid);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 访问模型中的关系                                                     */
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
		m_List.SetItemText(indexNo,2,_T("所属元件："+CString(name)));
		//检查该项是否已经添加
		if (g_RelCollection.IsRelInCollection(mdl,relSet[i].MakeLower())) {
			ListView_SetCheckState(m_List,indexNo,TRUE);
		}		
	}
	////访问用零件建模工具建的关系式
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
	//	//判断有没有已经存在
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
	//		,p_UsrRelTableList[i].note==_T("")?_T("所属元件："+CString(name)):p_UsrRelTableList[i].note);
	//	//检查该项是否已经添加
	//	for (int j=0;j<(int)p_AsmRelItemList.GetSize();j++) {
	//		if(p_AsmRelItemList[j].owner!=solid) continue;
	//		tempRel1=rel;tempRel2=p_AsmRelItemList[j].relation;
	//		if (tempRel1.MakeLower()==tempRel2.MakeLower()) {
	//			ListView_SetCheckState(m_List,indexNo,TRUE);
	//			break;
	//		}
	//	}

	//}
	////装配件中的组件元件建的关系
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
	//		,p_AsmRelItemList[i].note==_T("")?_T("所属元件："+CString(name)):p_AsmRelItemList[i].note);
	//	//检查该项是否已经添加
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
	//继续访问
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
	// TODO: 在此添加控件通知处理程序代码
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
