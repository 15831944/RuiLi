// MenuOpenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "MenuOpenDlg.h"
#include "AsmTempManageTableSet.h"
#include "PartMdlRecordSet.h"
#include "AsmManageTableSet.h"
#include "TempWorkPlaceDlg.h"
#include "AsmTempOpen.h"
#include "PartMdlOpen.h"
#include "AsmOpen.h"
#include "partPicSet.h"
#include "NPartInsInfoTable.h"
#include "NProductInsInfoTable.h"
//#include "Part_Temp_Pic.h"
//#include "AsmTempPic.h"
#include "CAsmPicSet.h"
#include "NAsmType.h"
#include "asmassembly.h"
#include "NewDlg.h"
#include "MdlPropDefDlg.h"
// CMenuOpenDlg 对话框

IMPLEMENT_DYNAMIC(CMenuOpenDlg, CDialog)
CMenuOpenDlg::CMenuOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuOpenDlg::IDD, pParent)/*,p_Picture(NULL),current_pic(-1),pic_dlg(NULL)*/
	, m_iNewType(0)
{
}

CMenuOpenDlg::~CMenuOpenDlg()
{
}

void CMenuOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_List);
	DDX_Control(pDX, IDC_TAB3, m_Tab);
	DDX_Control(pDX, IDC_TREE1, m_Tree[0]);
	DDX_Control(pDX, IDC_TREE2, m_Tree[1]);
	DDX_Control(pDX, IDC_TREE3, m_Tree[2]);
	DDX_Control(pDX, IDC_TREE4, m_Tree[3]);
	DDX_Control(pDX, IDC_TREE5, m_Tree[4]);
	DDX_Control(pDX, IDC_TREE6, m_Tree[5]);
	DDX_Control(pDX, IDC_PIC, m_pic);
	DDX_Radio(pDX, IDC_RADIO1, m_iNewType);
}


BEGIN_MESSAGE_MAP(CMenuOpenDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB3, OnTcnSelchangeTab3)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, OnTvnSelchangedTree2)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE3, OnTvnSelchangedTree3)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE4, OnTvnSelchangedTree4)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE5, OnTvnSelchangedTree5)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE6, OnTvnSelchangedTree6)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST3, OnNMDblclkList3)
	ON_BN_CLICKED(IDC_OPEN_BUTTON, OnBnClickedOpenButton)
	//ON_BN_CLICKED(IDC_PIC_BUTTON, OnBnClickedPicButton)
	//ON_BN_CLICKED(IDC_VIEW_LOCAL_FILE, OnBnClickedViewLocalFile)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_LIST3, OnNMClickList3)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CMenuOpenDlg 消息处理程序

BOOL CMenuOpenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
    if(!m_pSet.Open())
	{
		AfxMessageBox("数据库连接不成功！");
		OnCancel();
		return FALSE;
	}
	p_db=m_pSet.m_pDatabase;
	// TODO:  在此添加额外的初始化	
    SetTab();
	m_Tab.SetCurSel(0);
	ShowTree();
	//设置图片
	RECT   rect1;	
	m_pic.GetWindowRect(&rect1); 
	ScreenToClient(&rect1);
	pic_dlg.Create(IDD_PIC_FORM_DB,this);
	pic_dlg.MoveWindow(&rect1);
	pic_dlg.ShowWindow(SW_SHOW);
	//设置图片控件
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CMenuOpenDlg::SetTab(void)
{	
	m_ListImgList.Create(20,20,ILC_COLOR32,4,4);
	m_ListImgList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ListImgList.Add(AfxGetApp()->LoadIcon(IDI_PRT)); 
	m_ListImgList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_Tab.SetImageList(&m_ListImgList);
	m_Tab.InsertItem(0,"零件工作区",1);
	m_Tab.InsertItem(1,"组件工作区",2);
	m_Tab.InsertItem(2,"零件模型    ",1);
	m_Tab.InsertItem(3,"组件模型    ",2);
	m_Tab.InsertItem(4,"已有零件    ",1);
	m_Tab.InsertItem(5,"已有产品    ",2);
	//
	m_Tab.InsertItem(6,"本地文件    ",2);
 //   //新建
	//m_Tab.InsertItem(7,"新建模型    ",2);

	m_TreeImgList.Create(14,14,ILC_COLOR32,4,4);
	m_TreeImgList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_TreeImgList.Add(AfxGetApp()->LoadIcon(IDI_PRT)); 
	m_TreeImgList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_Tree[0].SetImageList(&m_TreeImgList,TVSIL_NORMAL);
	m_Tree[1].SetImageList(&m_TreeImgList,TVSIL_NORMAL);
	m_Tree[2].SetImageList(&m_TreeImgList,TVSIL_NORMAL);
	m_Tree[3].SetImageList(&m_TreeImgList,TVSIL_NORMAL);
	m_Tree[4].SetImageList(&m_TreeImgList,TVSIL_NORMAL);
	m_Tree[5].SetImageList(&m_TreeImgList,TVSIL_NORMAL);

	ShowTempPartTree(&(m_Tree[0]),p_db);
	ShowTempAsmTree(&(m_Tree[1]),p_db);
	ShowNPartTree(&(m_Tree[2]),p_db);
	ShowNAsmTree(&(m_Tree[3]),p_db);	
	ShowPartTypeTree(&(m_Tree[4]),p_db);
	ShowAsmTree(&m_Tree[5],p_db);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"名称",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(1,"编码",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(2,"文件名",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(3,"备注",LVCFMT_LEFT,100,-1);
}
/************************************************************************/
/* 切换树控件的显示                                                     */
/************************************************************************/
void CMenuOpenDlg::ShowTree(void)
{
	for (int i=0;i<6;i++)
	{
		if(m_Tab.GetCurSel()==i)
		{
			m_Tree[i].ShowWindow(SW_SHOW);
			HTREEITEM hItem=m_Tree[i].GetSelectedItem();
			m_Tree[i].SelectItem(m_Tree[i].GetRootItem());
			m_Tree[i].SelectItem(hItem);
			continue;
		}
		m_Tree[i].ShowWindow(SW_HIDE);
	}
}
/************************************************************************/
/* 浏览本地目录                                                         */
/************************************************************************/
void CMenuOpenDlg::OnTcnSelchangeTab3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Tab.GetCurSel()==6) 
	{
		//浏览本地文件
		OnBnClickedViewLocalFile();
		return;
	}
	//if (m_Tab.GetCurSel()==7)
	//{
	//	NewMdl();
	//	return;
	//}
	ShowTree();
	*pResult = 0;
}
/************************************************************************/
/* 零件模型工作区                                                       */
/************************************************************************/
void CMenuOpenDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	long   FatherNum=0;
	int    indexNo=0;
	HTREEITEM 	hItem=pNMTreeView->itemNew.hItem;
	m_List.DeleteAllItems();
	if(hItem==NULL) return;	
	FatherNum=(long)m_Tree[0].GetItemData(hItem);
	m_pSet.m_strFilter.Format("[FatherNum]=%ld AND [Type]<>'wrk'",FatherNum);
	m_pSet.Requery();
	if (!m_pSet.IsBOF())
	{
		m_pSet.MoveFirst();
		do {
			indexNo=m_List.InsertItem(LVIF_PARAM|LVIF_TEXT ,m_List.GetItemCount(),m_pSet.m_Name,0,0,0,m_pSet.m_Number);
			m_List.SetItemText(indexNo,1,m_pSet.m_MdlCode);
			m_List.SetItemText(indexNo,2,m_pSet.m_FileName);
			m_List.SetItemText(indexNo,3,m_pSet.m_Note);
			m_pSet.MoveNext();
		} while(!m_pSet.IsEOF());
	}
	*pResult = 0;
}
/************************************************************************/
/* 组件模型工作区                                                       */
/************************************************************************/
void CMenuOpenDlg::OnTvnSelchangedTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CAsmTempManageTableSet m_Set(p_db);
	long   FatherNum=0;
	int    indexNo=0;
	HTREEITEM 	hItem=pNMTreeView->itemNew.hItem;
	m_List.DeleteAllItems();
	if(hItem==NULL) return;
	FatherNum=(long)m_Tree[1].GetItemData(hItem);
	m_Set.m_strFilter.Format("[FatherNumber]=%ld AND [Type]<>'wrk'",FatherNum);
	m_Set.Open();
	if (!m_Set.IsBOF())
	{
		m_Set.MoveFirst();
		do {
			indexNo=m_List.InsertItem(LVIF_PARAM|LVIF_TEXT ,m_List.GetItemCount(),m_Set.m_Name,0,0,0,m_Set.m_Number);
			m_List.SetItemText(indexNo,1,m_Set.m_MdlCode );
			m_List.SetItemText(indexNo,2,m_Set.m_FileName );
			m_List.SetItemText(indexNo,3,m_Set.m_Note);
			m_Set.MoveNext();
		} while(!m_Set.IsEOF());
	}
	*pResult = 0;
}
/************************************************************************/
/* 零件模型                                                              */
/************************************************************************/
void CMenuOpenDlg::OnTvnSelchangedTree3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CPartMdlRecordSet m_Set(p_db);
	long			TypeNum=0;
	int				indexNo=0;
	HTREEITEM 			hItem=pNMTreeView->itemNew.hItem;
	m_List.DeleteAllItems();
	if(hItem==NULL) return;
	TypeNum=(long)m_Tree[2].GetItemData(hItem);
	m_Set.m_strFilter.Format("[TypeNum]=%ld",TypeNum);
	m_Set.Open();
	if (!m_Set.IsBOF())
	{
		m_Set.MoveFirst();
		do {
			indexNo=m_List.InsertItem(LVIF_PARAM|LVIF_TEXT ,m_List.GetItemCount(),m_Set.m_Name,0,0,0,m_Set.m_Number);
			m_List.SetItemText(indexNo,1,m_Set.m_Code );
			m_List.SetItemText(indexNo,2,m_Set.m_SolidFileName);
			m_List.SetItemText(indexNo,3,m_Set.m_Note);
			m_Set.MoveNext();
		} while(!m_Set.IsEOF());
	}
	*pResult = 0;
}
/************************************************************************/
/* 组建模型                                                             */
/************************************************************************/
void CMenuOpenDlg::OnTvnSelchangedTree4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CAsmManageTableSet	m_Set(p_db);
	//CNAsmType           m_typeset(p_db);
	long				TypeNum=0;
	int					indexNo=0;
	HTREEITEM 	        hItem=pNMTreeView->itemNew.hItem;
	m_List.DeleteAllItems();
	if(hItem==NULL) return;
	TypeNum=(long)m_Tree[3].GetItemData(hItem);
	m_Set.m_strFilter.Format("[TypeNum]=%ld",TypeNum);
	m_Set.Open();
	if (!m_Set.IsBOF())
	{
		m_Set.MoveFirst();
		do {
			indexNo=m_List.InsertItem(LVIF_PARAM|LVIF_TEXT ,m_List.GetItemCount(),m_Set.m_Name,0,0,0,m_Set.m_Number);
			m_List.SetItemText(indexNo,1,m_Set.m_Code );
			m_List.SetItemText(indexNo,2,m_Set.m_SolidFileName );
			m_List.SetItemText(indexNo,3,m_Set.m_Note);
			m_Set.MoveNext();
		} while(!m_Set.IsEOF());
	}
	*pResult = 0;
}
/************************************************************************/
/* 现有零件                                                             */
/************************************************************************/
void CMenuOpenDlg::OnTvnSelchangedTree5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	long							num=(long)pNMTreeView->itemNew.lParam;
	CNPartInsInfoTable				m_Set1(p_db);
	int								indexNo;
	m_Set1.m_strFilter.Format("[TypeNum]=%ld",num);
	m_List.DeleteAllItems();
	m_Set1.Open();
	if (!m_Set1.IsBOF())
	{
		m_Set1.MoveFirst();
		do {
			indexNo=m_List.InsertItem(LVIF_PARAM|LVIF_TEXT ,m_List.GetItemCount(),m_Set1.m_Name,0,0,0,m_Set1.m_PrtNum );
			m_List.SetItemText(indexNo,1,m_Set1.m_InsCode);
			m_List.SetItemText(indexNo,2,m_Set1.m_SolidFileName);
			m_List.SetItemText(indexNo,3,m_Set1.m_Note);
			m_Set1.MoveNext();
		} while(!m_Set1.IsEOF());
	}
	*pResult = 0;
}
/************************************************************************/
/* 现有产品                                                             */
/************************************************************************/
void CMenuOpenDlg::OnTvnSelchangedTree6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	long							num=(long)pNMTreeView->itemNew.lParam;
	int								indexNo;
	CNProductInsInfoTable  m_set(p_db);
	int        nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	m_set.m_strFilter.Format("[TypeNum]=%ld",num);
	m_List.DeleteAllItems();
	m_set.Open();
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			indexNo=m_List.InsertItem(LVIF_PARAM|LVIF_TEXT ,m_List.GetItemCount(),m_set.m_Name,0,0,0,m_set.m_PdtNum );
			m_List.SetItemText(indexNo,1,m_set.m_InsCode);
			m_List.SetItemText(indexNo,2,m_set.m_SolidFileName);
			m_List.SetItemText(indexNo,3,m_set.m_Note);
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
	*pResult = 0;
}
void CMenuOpenDlg::OnNMDblclkList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
    OnBnClickedOpenButton();
	*pResult = 0;
}
/************************************************************************/
/* 打开选中的模型                                                       */
/************************************************************************/
void CMenuOpenDlg::OnBnClickedOpenButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CTempWorkPlaceDlg	temp;
	CAsmTempOpen		asm_open;
	CPartMdlOpen		part_mdl_open;
	CAsmOpen			asm_mdl_open;
	POSITION			pos = m_List.GetFirstSelectedItemPosition();
	int nItem = m_List.GetNextSelectedItem(pos);
	if(nItem==-1) return;    //必须点中,且不为第一个
	switch(m_Tab.GetCurSel()) {
	case 0:		
		temp.m_pSet.m_pDatabase=p_db;
		if(temp.OpenPartTempMdl((long)m_List.GetItemData(nItem)))
		{
			OnCancel();;
		}
		break;
	case 1:
		if (asm_open.Open(p_db,(long)m_List.GetItemData(nItem)))
		{
			OnCancel();;
		}
		break;
	case 2:
		if (part_mdl_open.Open(p_db,(long)m_List.GetItemData(nItem)))
		{
			OnCancel();;
		}
		break;
	case 3:
		if (asm_mdl_open.Open(p_db,(long)m_List.GetItemData(nItem)))
		{
			OnCancel();;
		}
		break;
	case 4:
		if (part_mdl_open.OpenPart(p_db,(long)m_List.GetItemData(nItem)))
		{
			OnCancel();;
		}
		break;
	case 5:
		if (OpenProduct((long)m_List.GetItemData(nItem)))
		{
			OnCancel();
		}
		break;
	default:
		break;
	}
}
/************************************************************************/
/* 浏览本地的文件                                                       */
/************************************************************************/
void CMenuOpenDlg::OnBnClickedViewLocalFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CString  ext1;
	ProPath  name;
	ProMdl   mdl;
	ProName  w_name;
	ProType  type;
	ProError status;
	int      dot;
	int      w_id;
	CString  path;
	CFileDialog dlg(TRUE,"prt","*.prt*;*.asm*;*.drw*"
		,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST
		,"PRO/ENGINEET 文件(*.prt *.asm *.drw)|*.prt*;*.asm*;*.drw*||");
	if (dlg.DoModal()==IDOK) {
		dot=dlg.GetPathName().Find('.',dlg.GetPathName().GetLength()-2);
		if (dot!=-1) {
			path=dlg.GetPathName().Left(dlg.GetPathName().GetLength()-2);
		}
		else
		{
			dot=dlg.GetPathName().Find('.',dlg.GetPathName().GetLength()-3);
			if (dot!=-1) {
				path=dlg.GetPathName().Left(dlg.GetPathName().GetLength()-3);
			}
			else path=dlg.GetPathName();

		}
		ProStringToWstring(name,_T(path.GetBuffer()+'\0'));
		status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,&mdl); 
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("打开文件失败");
			return;
		}
		status = ProMdlTypeGet(mdl, (ProMdlType*)&type);
		status= ProMdlNameGet(mdl, w_name);
		status= ProObjectwindowCreate(w_name, type, &w_id);
		if(!status== PRO_TK_NO_ERROR)
		{
			return;
		}
		status = ProMdlDisplay(mdl);
		status = ProWindowActivate(w_id);
		OnCancel();
	}
	else
	{
		//if(ProWindowCurrentGet(&w_id)==PRO_TK_NO_ERROR){

		//	ProWindowActivate(w_id);
		//}
		m_Tab.SetCurSel(0);
	    ShowTree();
		return;
	}
}
void CMenuOpenDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
void CMenuOpenDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}
void CMenuOpenDlg::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nItem = m_List.GetNextSelectedItem(pos);
	if(nItem==-1) return;    //必须点中,且不为第一个
	switch(m_Tab.GetCurSel()) {
	case 0:
		pic_dlg.GetMdlPicSet(2,(long)m_List.GetItemData(nItem),m_pSet.m_pDatabase);
		break;
	case 1:
		pic_dlg.GetMdlPicSet(3,(long)m_List.GetItemData(nItem),m_pSet.m_pDatabase);
		break;
	case 2:
		pic_dlg.GetMdlPicSet(0,(long)m_List.GetItemData(nItem),m_pSet.m_pDatabase);
		break;
	case 3:      //查看组建模型的图片
		pic_dlg.GetMdlPicSet(1,(long)m_List.GetItemData(nItem),m_pSet.m_pDatabase);
		break;
	case 4:
		pic_dlg.GetMdlPicSet(-1,(long)m_List.GetItemData(nItem),m_pSet.m_pDatabase);
		break;
	case 5:
		pic_dlg.GetMdlPicSet(-1,(long)m_List.GetItemData(nItem),m_pSet.m_pDatabase);
		break;
	default:
		break;
	}
	*pResult = 0;
}

/************************************************************************/
/* 打开现有产品															*/
/************************************************************************/
BOOL CMenuOpenDlg::OpenProduct(long pdt_num)
{
	CAsmAssembly	pdt_open;
	ProSolid		asm_solid;
	ProName			w_name;
	int				w_id;
	ProType			type;
	ProError        status;
	if (!pdt_open.OpenProduct(p_db,pdt_num,&asm_solid))
	{
		return FALSE;
	}
	if (!pdt_open.AddInfo())
	{
		return FALSE;
	}
	status = ProMdlTypeGet(asm_solid, (ProMdlType*)&type);
	status=  ProMdlNameGet(asm_solid, w_name);
	status=  ProObjectwindowCreate(w_name, type, &w_id);
	if(!status== PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	status = ProMdlDisplay(asm_solid);
	status = ProWindowActivate(w_id);	
	return TRUE;
}
///************************************************************************/
///* 新建模型                                                                     */
///************************************************************************/
//BOOL CMenuOpenDlg::NewMdl(void)
//{
//	
//
//	return 0;
//}
/************************************************************************/
/* 新建模型，分为空模型和继承											*/
/************************************************************************/
void CMenuOpenDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CNewDlg				dlg;
	CTempWorkPlaceDlg	temp;
	CAsmTempOpen		asm_open;
	CPartMdlOpen		part_mdl_open;
	CAsmOpen			asm_mdl_open;
	MdlPropDefDlg       dlg2;
	POSITION			pos = m_List.GetFirstSelectedItemPosition();
	int nItem = m_List.GetNextSelectedItem(pos);
	UpdateData(TRUE);
	switch(m_iNewType) {
	case 0:
		if (dlg.DoModal()==IDOK)
		{
			OnCancel();
		}
		break;
	default:
					if(nItem==-1) return;    //必须点中,且不为第一个
					switch(m_Tab.GetCurSel()) {
				case 0:
					AfxMessageBox("不能从工作区模型中继承");
					break;
				case 1:
					AfxMessageBox("不能从工作区模型中继承");
					break;
				case 2:
					if (part_mdl_open.Open(p_db,(long)m_List.GetItemData(nItem)))
					{
						dlg2.DoModal();
						OnCancel();						
					}
					break;
				case 3:
					if (asm_mdl_open.Open(p_db,(long)m_List.GetItemData(nItem)))
					{
						dlg2.DoModal();
						OnCancel();
					}
					break;
				case 4:
					if (part_mdl_open.OpenPart(p_db,(long)m_List.GetItemData(nItem)))
					{
						dlg2.DoModal();
						OnCancel();
					}
					break;
				case 5:
					if (OpenProduct((long)m_List.GetItemData(nItem)))
					{
						dlg2.DoModal();
						OnCancel();
					}
					break;
				default:
					break;
	              }
	}
}
/************************************************************************/
/* 删除模型                                                             */
/************************************************************************/
void CMenuOpenDlg::OnBnClickedBtnDel()
{
	// TODO: 在此添加控件通知处理程序代码
	CTempWorkPlaceDlg	temp;
	POSITION			pos = m_List.GetFirstSelectedItemPosition();
	int nItem = m_List.GetNextSelectedItem(pos);
	if (nItem<0)
	{
		return;
	}
	switch(m_Tab.GetCurSel()) {
	case 0:
		if (!temp.DelTempPartMdl(p_db,(long)m_List.GetItemData(nItem)))
		{
			AfxMessageBox("删除不成功!");
		}
		else
		{
			m_List.DeleteItem(nItem);
		}
		break;
	case 1:
		if (!temp.DelTempAsmMdl(p_db,(long)m_List.GetItemData(nItem)))
		{
			AfxMessageBox("删除不成功!");
		}
		else
		{
			m_List.DeleteItem(nItem);
		}
		break;
	case 2:
		if (!UsrDelPartMdl(p_db,(long)m_List.GetItemData(nItem)))
		{
			AfxMessageBox("删除不成功!");	
		}
		else
		{
			m_List.DeleteItem(nItem);
		}
		break;
	case 3:
		if (!UsrDelAsmMdl(p_db,(long)m_List.GetItemData(nItem)))
		{
			AfxMessageBox("删除不成功!");	
		}
		else
		{
			m_List.DeleteItem(nItem);
		}
		break;
	case 4:
		AfxMessageBox("建模模式下不能删除零件实例!");
		break;
	case 5:
		AfxMessageBox("建模模式下不能删除组件实例!");
		break;
	default:
		break;
	}
}
/************************************************************************/
/* 关闭对话框                                                           */
/************************************************************************/
void CMenuOpenDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessage(WM_CLOSE);
}

void CMenuOpenDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此添加消息处理程序代码
	UsrRepaintWindow();
}
