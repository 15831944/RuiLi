// AsmNMdlTreePage.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "AsmNMdlTreePage.h"


// CAsmNMdlTreePage 对话框

IMPLEMENT_DYNAMIC(CAsmNMdlTreePage, CPropertyPage)
CAsmNMdlTreePage::CAsmNMdlTreePage()
	: CPropertyPage(CAsmNMdlTreePage::IDD)
	, m_Name(_T(""))
	, m_Code(_T(""))
	, m_TypeName(_T(""))
	, m_MdlName(_T(""))
	, m_Material(_T(""))
	, m_AuditState(_T(""))
	, m_Resoure(_T(""))
	, m_Note(_T(""))
	, mdl(NULL)
{
}

CAsmNMdlTreePage::~CAsmNMdlTreePage()
{
}

void CAsmNMdlTreePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_EDIT2, m_Code);
	DDX_Text(pDX, IDC_EDIT3, m_TypeName);
	DDX_Text(pDX, IDC_EDIT4, m_MdlName);
	DDX_Text(pDX, IDC_EDIT5, m_Material);
	DDX_Text(pDX, IDC_EDIT6, m_AuditState);
	DDX_Text(pDX, IDC_EDIT7, m_Resoure);
	DDX_Text(pDX, IDC_EDIT8, m_Note);
	DDX_Control(pDX, IDC_BUTTON2, m_CommitPDM);
	DDX_Control(pDX, IDC_BUTTON1, m_AuditBtn);
	DDX_Control(pDX, IDC_BUTTON3, m_UpdateBtn);
}


BEGIN_MESSAGE_MAP(CAsmNMdlTreePage, CPropertyPage)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CAsmNMdlTreePage 消息处理程序

BOOL CAsmNMdlTreePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	imageList.Create(12, 12, ILC_COLOR16 , 4, 4);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_CSYS));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_DTM));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_FEAT));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_LINE));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_POT));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_Tree.SetImageList(&imageList,TVSIL_NORMAL);
	if (mdl!=NULL) {
		int   indexNo=0;
		m_Tree.DeleteAllItems();
		GetMdlProp(mdl,&indexNo);
		HTREEITEM hItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM
			,p_UsrPartPorp[indexNo].Name
			,0,0,0,0,-1,NULL,NULL);
		ShowMdlTree(mdl,hItem);
		m_Tree.Expand(m_Tree.GetRootItem(),TVE_EXPAND );
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
int CAsmNMdlTreePage::ShowMdlTree(ProSolid solid,HTREEITEM hItem)
{
	ProError      status;
	HTREEITEM     tempItem;
	ProMdlType    type;
	int           imageNum=0;
	int           indexNo=0;
	CArray<ProFeature,ProFeature> feat_list;
	status=ProSolidFeatVisit(solid,(ProFeatureVisitAction)FeatureAsmVisitAcFn
		,NULL,(CArray<ProFeature,ProFeature>*)&feat_list);
	int num=(int)feat_list.GetCount();
	for (int i=0;i<num;i++) {
		ProFeattype    ftype;
		status = ProFeatureTypeGet (&feat_list[i], &ftype);

		if (ftype == PRO_FEAT_COMPONENT){
			ProSolid              mdl;
			ProAsmcompMdlGet((ProAsmcomp *)(&feat_list[i]),(ProMdl *)&mdl);
			GetMdlProp(mdl,&indexNo);
			ProMdlTypeGet(mdl,&type);
			switch(type) {
			case PRO_MDL_ASSEMBLY:
				imageNum=0;
				break;
			case PRO_MDL_PART:
				imageNum=6;
				break;
			default:

				break;
			}			
			tempItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM,p_UsrPartPorp[indexNo].Name
				,imageNum,imageNum,0,0,indexNo,hItem,NULL);				//设置模型的基本信息			
			ShowMdlTree(mdl,tempItem);
		}
	}
	return 0;
}
void CAsmNMdlTreePage::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int       indexNo=0;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem()) return;
	indexNo=(int)m_Tree.GetItemData(hItem);
	UsrPartPorp temp_porp=p_UsrPartPorp[indexNo];
	m_Name=temp_porp.Name;
	m_Code=temp_porp.Code;
	m_TypeName=temp_porp.TypeName;
	m_MdlName=temp_porp.MdlName;
	m_Material=temp_porp.Material;
	m_AuditState=(temp_porp.IsAudited?"已审核":"未审核");
	switch(temp_porp.ResourceType) {
	case 1:
		m_Resoure="设计系统文件";
		break;
	case 2:
		m_Resoure="PDM系统文件";
		break;
	default:
		m_Resoure="本地系统文件";
		break;
	}	
	m_Note=temp_porp.Note;
	UpdateData(FALSE);
	*pResult = 0;
}
/************************************************************************/
/* 提交PDM按钮                                                          */
/************************************************************************/
void CAsmNMdlTreePage::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
}
