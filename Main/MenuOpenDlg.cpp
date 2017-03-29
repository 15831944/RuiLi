// MenuOpenDlg.cpp : ʵ���ļ�
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
// CMenuOpenDlg �Ի���

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


// CMenuOpenDlg ��Ϣ�������

BOOL CMenuOpenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
    if(!m_pSet.Open())
	{
		AfxMessageBox("���ݿ����Ӳ��ɹ���");
		OnCancel();
		return FALSE;
	}
	p_db=m_pSet.m_pDatabase;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��	
    SetTab();
	m_Tab.SetCurSel(0);
	ShowTree();
	//����ͼƬ
	RECT   rect1;	
	m_pic.GetWindowRect(&rect1); 
	ScreenToClient(&rect1);
	pic_dlg.Create(IDD_PIC_FORM_DB,this);
	pic_dlg.MoveWindow(&rect1);
	pic_dlg.ShowWindow(SW_SHOW);
	//����ͼƬ�ؼ�
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CMenuOpenDlg::SetTab(void)
{	
	m_ListImgList.Create(20,20,ILC_COLOR32,4,4);
	m_ListImgList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ListImgList.Add(AfxGetApp()->LoadIcon(IDI_PRT)); 
	m_ListImgList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_Tab.SetImageList(&m_ListImgList);
	m_Tab.InsertItem(0,"���������",1);
	m_Tab.InsertItem(1,"���������",2);
	m_Tab.InsertItem(2,"���ģ��    ",1);
	m_Tab.InsertItem(3,"���ģ��    ",2);
	m_Tab.InsertItem(4,"�������    ",1);
	m_Tab.InsertItem(5,"���в�Ʒ    ",2);
	//
	m_Tab.InsertItem(6,"�����ļ�    ",2);
 //   //�½�
	//m_Tab.InsertItem(7,"�½�ģ��    ",2);

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
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(1,"����",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(2,"�ļ���",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(3,"��ע",LVCFMT_LEFT,100,-1);
}
/************************************************************************/
/* �л����ؼ�����ʾ                                                     */
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
/* �������Ŀ¼                                                         */
/************************************************************************/
void CMenuOpenDlg::OnTcnSelchangeTab3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_Tab.GetCurSel()==6) 
	{
		//��������ļ�
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
/* ���ģ�͹�����                                                       */
/************************************************************************/
void CMenuOpenDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
/* ���ģ�͹�����                                                       */
/************************************************************************/
void CMenuOpenDlg::OnTvnSelchangedTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
/* ���ģ��                                                              */
/************************************************************************/
void CMenuOpenDlg::OnTvnSelchangedTree3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
/* �齨ģ��                                                             */
/************************************************************************/
void CMenuOpenDlg::OnTvnSelchangedTree4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
/* �������                                                             */
/************************************************************************/
void CMenuOpenDlg::OnTvnSelchangedTree5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
/* ���в�Ʒ                                                             */
/************************************************************************/
void CMenuOpenDlg::OnTvnSelchangedTree6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    OnBnClickedOpenButton();
	*pResult = 0;
}
/************************************************************************/
/* ��ѡ�е�ģ��                                                       */
/************************************************************************/
void CMenuOpenDlg::OnBnClickedOpenButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CTempWorkPlaceDlg	temp;
	CAsmTempOpen		asm_open;
	CPartMdlOpen		part_mdl_open;
	CAsmOpen			asm_mdl_open;
	POSITION			pos = m_List.GetFirstSelectedItemPosition();
	int nItem = m_List.GetNextSelectedItem(pos);
	if(nItem==-1) return;    //�������,�Ҳ�Ϊ��һ��
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
/* ������ص��ļ�                                                       */
/************************************************************************/
void CMenuOpenDlg::OnBnClickedViewLocalFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		,"PRO/ENGINEET �ļ�(*.prt *.asm *.drw)|*.prt*;*.asm*;*.drw*||");
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
			AfxMessageBox("���ļ�ʧ��");
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
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}
void CMenuOpenDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}
void CMenuOpenDlg::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nItem = m_List.GetNextSelectedItem(pos);
	if(nItem==-1) return;    //�������,�Ҳ�Ϊ��һ��
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
	case 3:      //�鿴�齨ģ�͵�ͼƬ
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
/* �����в�Ʒ															*/
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
///* �½�ģ��                                                                     */
///************************************************************************/
//BOOL CMenuOpenDlg::NewMdl(void)
//{
//	
//
//	return 0;
//}
/************************************************************************/
/* �½�ģ�ͣ���Ϊ��ģ�ͺͼ̳�											*/
/************************************************************************/
void CMenuOpenDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
					if(nItem==-1) return;    //�������,�Ҳ�Ϊ��һ��
					switch(m_Tab.GetCurSel()) {
				case 0:
					AfxMessageBox("���ܴӹ�����ģ���м̳�");
					break;
				case 1:
					AfxMessageBox("���ܴӹ�����ģ���м̳�");
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
/* ɾ��ģ��                                                             */
/************************************************************************/
void CMenuOpenDlg::OnBnClickedBtnDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			AfxMessageBox("ɾ�����ɹ�!");
		}
		else
		{
			m_List.DeleteItem(nItem);
		}
		break;
	case 1:
		if (!temp.DelTempAsmMdl(p_db,(long)m_List.GetItemData(nItem)))
		{
			AfxMessageBox("ɾ�����ɹ�!");
		}
		else
		{
			m_List.DeleteItem(nItem);
		}
		break;
	case 2:
		if (!UsrDelPartMdl(p_db,(long)m_List.GetItemData(nItem)))
		{
			AfxMessageBox("ɾ�����ɹ�!");	
		}
		else
		{
			m_List.DeleteItem(nItem);
		}
		break;
	case 3:
		if (!UsrDelAsmMdl(p_db,(long)m_List.GetItemData(nItem)))
		{
			AfxMessageBox("ɾ�����ɹ�!");	
		}
		else
		{
			m_List.DeleteItem(nItem);
		}
		break;
	case 4:
		AfxMessageBox("��ģģʽ�²���ɾ�����ʵ��!");
		break;
	case 5:
		AfxMessageBox("��ģģʽ�²���ɾ�����ʵ��!");
		break;
	default:
		break;
	}
}
/************************************************************************/
/* �رնԻ���                                                           */
/************************************************************************/
void CMenuOpenDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendMessage(WM_CLOSE);
}

void CMenuOpenDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: �ڴ������Ϣ����������
	UsrRepaintWindow();
}
