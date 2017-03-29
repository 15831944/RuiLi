// TempWorkPlaceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "TempWorkPlaceDlg.h"
#include "FtpFile.h"
#include "Part_Temp_Para_Info.h"
#include "Part_Temp_Perf.h"
#include "Part_Temp_Pic.h"
#include "Part_Temp_Rel.h"
#include "MdlDefDlg.h"
#include "AddMdlSeriesDlg.h"
#include "AsmTempManageTableSet.h"
#include "Part_Temp_Drawing.h"
#include "UsrSWTXTable.h"
#include "asmtempsave.h"
#include "asmtempopen.h"
// CTempWorkPlaceDlg �Ի���

IMPLEMENT_DYNAMIC(CTempWorkPlaceDlg, CDialog)
CTempWorkPlaceDlg::CTempWorkPlaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTempWorkPlaceDlg::IDD, pParent)
	, m_iSelTab(0)
	, m_CurrentTvItem(NULL)
	, m_CurrentTvItemForMenu(NULL)
	, dlgMode(-1)
{
}

CTempWorkPlaceDlg::~CTempWorkPlaceDlg()
{
}

void CTempWorkPlaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree1);
	DDX_Control(pDX, IDC_TREE2, m_Tree2);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_EDIT1, m_Path);
}


BEGIN_MESSAGE_MAP(CTempWorkPlaceDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, OnTvnSelchangedTree2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_COMMAND(ID_12117, OnNewWorkPlace)
	ON_COMMAND(ID_12118, OnDelWorkPlace)
	ON_COMMAND(ID_12126, OnRename)
	ON_COMMAND(ID_12120, OnOpenMdl)
	ON_COMMAND(ID_Menu12121, OnDelMdl)
	ON_COMMAND(ID_12122, OnProp)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_Menu12160, OnMenu12160)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE1, OnTvnBegindragTree1)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, OnTvnEndlabeleditTree1)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE2, OnTvnBeginlabeleditTree2)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE2, OnTvnEndlabeleditTree2)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnTcnSelchangingTab1)
	ON_COMMAND(ID_12166, OnOpenWorkPlace)
	ON_COMMAND(ID_New, OnNew)
	ON_COMMAND(ID_Rename, OnRenameWorkPlace)
	
	ON_COMMAND(ID_Menu12167, OnMenu12167)
END_MESSAGE_MAP()


// CTempWorkPlaceDlg ��Ϣ�������

BOOL CTempWorkPlaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	TCITEM tcItem;
	switch(dlgMode) {
	case -1:
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = _T("�����ģ��");
		m_Tab.InsertItem(0, &tcItem);
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = _T("��Ʒ/�����ģ��");
		m_Tab.InsertItem(1, &tcItem);
		m_iSelTab=0;
		m_Tree1.ShowWindow(SW_SHOW);
		m_Tree2.ShowWindow(SW_HIDE);
		break;
	case PRO_MDL_PART:
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = _T("�����ģ��");
		m_Tab.InsertItem(0, &tcItem);
		m_iSelTab=0;
		m_Tree1.ShowWindow(SW_SHOW);
		m_Tree2.ShowWindow(SW_HIDE);
		break;
	case PRO_MDL_ASSEMBLY:
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = _T("��Ʒ/�����ģ��");
		m_Tab.InsertItem(1, &tcItem);
		m_iSelTab=1;
		m_Tree1.ShowWindow(SW_HIDE);
		m_Tree2.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
	if (m_pSet.Open())
	{
		ShowTempAsmTree(&m_Tree2,m_pSet.m_pDatabase);
		ShowTempPartTree(&m_Tree1,m_pSet.m_pDatabase);
	}
	else
	{
		AfxMessageBox("���ݿ�����ʧ��");
		OnOK();
		return FALSE;
	}
	m_ImgForList.Create(32, 32, ILC_COLOR32, 4, 4);
	m_ImgForList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImgForList.Add(AfxGetApp()->LoadIcon(IDI_PRT)); 
	m_ImgForList.Add(AfxGetApp()->LoadIcon(IDI_ASM));

	m_ImgForList2.Create(14, 14, ILC_COLOR32, 4, 4);
	m_ImgForList2.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImgForList2.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_ImgForList2.Add(AfxGetApp()->LoadIcon(IDI_ASM));

	m_ImgForList3.Create(14, 14, ILC_COLOR32, 4, 4);
	m_ImgForList3.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImgForList3.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_ImgForList3.Add(AfxGetApp()->LoadIcon(IDI_PRT));

	m_List.SetImageList(&m_ImgForList,TVSIL_NORMAL);
	m_Tree1.SetImageList(&m_ImgForList2,TVSIL_NORMAL);
	m_Tree2.SetImageList(&m_ImgForList3,TVSIL_NORMAL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CTempWorkPlaceDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(dlgMode!=-1) return;
	int sel = m_Tab.GetCurSel();
	switch(sel)
	{
	case 0:
		if (m_iSelTab == 0) return;
		if (m_iSelTab == 1)  m_Tree2.ShowWindow(SW_HIDE);
		m_Tree1.ShowWindow(SW_SHOW);
		m_iSelTab = 0;
		SetList(m_Tree1.GetRootItem());
		SetPath(&m_Tree1,m_Tree1.GetRootItem());
		break;
	case 1:
		if (m_iSelTab == 1) return;		
		if (m_iSelTab == 0) m_Tree1.ShowWindow(SW_HIDE);
		m_Tree2.ShowWindow(SW_SHOW);
		m_iSelTab = 1;
		SetList(m_Tree2.GetRootItem());
		SetPath(&m_Tree2,m_Tree2.GetRootItem());
		break;
	default:
		break;
	}
	*pResult = 0;
}
/************************************************************************/
/* ���б������ʾ����                                                   */
/************************************************************************/
void CTempWorkPlaceDlg::SetList(HTREEITEM hItem)
{
	int						iImg=0;
	int						iSelImg=0;
	int						firstItem=0;
	CAsmTempManageTableSet	m_Set(m_pSet.m_pDatabase);
	CTreeCtrl *				pmyTreeCtrl=(m_iSelTab==0?&m_Tree1:&m_Tree2); 
	m_CurrentTvItem	=hItem;
	if(hItem==NULL) 
		return;
	m_List.DeleteAllItems(); 
	if(hItem == pmyTreeCtrl->GetRootItem()) 
		firstItem=-2;	
	else 
		firstItem=-1;

	//�������,�����Ϊ�գ�
	m_List.InsertItem(LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM,m_List.GetItemCount(),".." ,0,0,0,(LPARAM)firstItem);
	switch(m_iSelTab) {
	case 0:
		try
		{ 
			m_pSet.m_strFilter.Format("[FatherNum]=%ld",(long)m_Tree1.GetItemData(hItem));
			m_pSet.Requery();
			if (!m_pSet.IsBOF())
			{
				m_pSet.MoveFirst();
				do {
					m_List.InsertItem(LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM,m_List.GetItemCount()
						,m_pSet.m_Name ,0,0,(m_pSet.m_Type=="mdl"?1:0),(LPARAM)m_pSet.m_Number);
					m_pSet.MoveNext();
				} while(!m_pSet.IsEOF());
			}
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			delete  e;
		}
		break;
	case 1:
		try
		{ 
			m_Set.m_strFilter.Format("[FatherNumber]=%ld",(long)m_Tree1.GetItemData(hItem));
			m_Set.Open();
			if (!m_Set.IsBOF())
			{
				m_Set.MoveFirst();
				do {
					m_List.InsertItem(LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM,m_List.GetItemCount()
						,m_Set.m_Name ,0,0,(m_Set.m_Type=="mdl"?2:0),(LPARAM)m_Set.m_Number);
					m_Set.MoveNext();
				} while(!m_Set.IsEOF());
			}
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			
		}
		break;
	default:
		break;
	}
}
/************************************************************************/
/* �л������ģʽ                                                       */
/************************************************************************/
void CTempWorkPlaceDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetPath(&m_Tree1,m_Tree1.GetSelectedItem());
	SetList(m_Tree1.GetSelectedItem());
	*pResult = 0;   
}
/************************************************************************/
/* �л������ģʽ                                                       */
/************************************************************************/
void CTempWorkPlaceDlg::OnTvnSelchangedTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetPath(&m_Tree2,m_Tree2.GetSelectedItem());
	SetList(m_Tree2.GetSelectedItem());
	*pResult = 0;
}
/************************************************************************/
/* ����б���ͼ��                                                     */
/************************************************************************/
void CTempWorkPlaceDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LPNMLISTVIEW pNMListView=reinterpret_cast<LPNMLISTVIEW>(pNMHDR); 
	HTREEITEM    hItem=NULL;
	int          iImg=0;
	int          iSelImg=0;
	CTreeCtrl *  pmyTreeCtrl=(m_iSelTab==0?&m_Tree1:&m_Tree2);
	//1.û�е���е�ʱ�򣬲�ִ��
	if(pNMListView->iItem==-1 ) return;
	if(m_CurrentTvItem!=NULL && pmyTreeCtrl->ItemHasChildren(m_CurrentTvItem) && (long)m_List.GetItemData(pNMListView->iItem)>=0) 
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = pmyTreeCtrl->GetChildItem(m_CurrentTvItem); 
		while (hChildItem != NULL)
		{
			hNextItem = pmyTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
			if(pmyTreeCtrl->GetItemData(hChildItem)==(long)m_List.GetItemData(pNMListView->iItem))
			{
				hItem=hChildItem;
				pmyTreeCtrl->Select(hItem,TVGN_CARET);  //��ʾ�����ѡ��
				SetList(hItem);
				SetPath(pmyTreeCtrl,hItem);
				//4.չ����Ӧ������
				if(pmyTreeCtrl->ItemHasChildren(hItem)) pmyTreeCtrl->Expand(hItem,TVE_EXPAND);
				pmyTreeCtrl->EnsureVisible(hItem); 
				break;
			}
			hChildItem = hNextItem;
		}
	}
	else if (m_List.GetItemData(pNMListView->iItem)==-1)
	{
		hItem=pmyTreeCtrl->GetParentItem(m_CurrentTvItem);
		if (hItem!=NULL)
		{
			SetList(hItem);
			pmyTreeCtrl->Select(hItem,TVGN_CARET);  //��ʾ�����ѡ��
			SetPath(pmyTreeCtrl,hItem);
		}
		
	}
	*pResult = 0;
}
/************************************************************************/
/* ����·��                                                             */
/************************************************************************/
void CTempWorkPlaceDlg::SetPath(CTreeCtrl * p_Tree, HTREEITEM hItem)
{
	CString   path;
	HTREEITEM pItem;
	path=p_Tree->GetItemText(hItem);
	if (hItem!=p_Tree->GetRootItem()) {
		pItem=hItem;
		do {
			pItem=p_Tree->GetParentItem(pItem);
			path=p_Tree->GetItemText(pItem)+"\\"+path;
		} while(pItem!=p_Tree->GetRootItem());
	}
	m_Path.SetWindowText(path);
	
}
/************************************************************************/
/* �����                                                             */
/************************************************************************/
BOOL CTempWorkPlaceDlg::OpenPart(void)
{
	return 0;
}
/************************************************************************/
/* �½�������                                                           */
/************************************************************************/
void CTempWorkPlaceDlg::OnNewWorkPlace()
{
	// TODO: �ڴ���������������
	if(m_CurrentTvItemForMenu==NULL) return;
	switch(m_iSelTab) {
	case 0:
		NewPartWorkPlace();
		m_Tree1.Select(m_CurrentTvItemForMenu,TVGN_CARET);
		SetList(m_CurrentTvItemForMenu);
		break;
	case 1:
		NewAsmWorkPlace();
		SetList(m_CurrentTvItemForMenu);
		m_Tree2.Select(m_CurrentTvItemForMenu,TVGN_CARET);
		break;
	default:
		break;
	}
}
/************************************************************************/
/* ɾ��������                                                           */
/************************************************************************/
void CTempWorkPlaceDlg::OnDelWorkPlace()
{
	// TODO: �ڴ���������������
	CFtpFile				m_ftp;
	CString					tempDir;
	CAsmTempManageTableSet	m_Set(m_pSet.m_pDatabase);
	HTREEITEM				hParantItem=NULL;
	if(m_CurrentTvItemForMenu==NULL) return;
	switch(m_iSelTab) {
	case 0:
		if(m_Tree1.GetRootItem()==m_CurrentTvItemForMenu) return;
		try
		{
			m_pSet.m_pDatabase->BeginTrans();
			m_pSet.m_strFilter.Format("[FatherNum]=%ld",(long)m_Tree1.GetItemData(m_CurrentTvItemForMenu));
			m_pSet.Requery();
			//��鹤�������Ƿ�������
			if (!m_pSet.IsBOF())
			{
				if (AfxMessageBox("��������Ϊ�գ��Ƿ�ȫ��ɾ����",MB_YESNO)==IDNO)
				{
					m_pSet.m_pDatabase->Rollback();
					return;
				}	
			}
			m_pSet.m_strFilter.Format("[Number]=%ld",(long)m_Tree1.GetItemData(m_CurrentTvItemForMenu));
			m_pSet.Requery();
			if (!m_pSet.IsBOF())
			{
				//��ɾ���ļ�
				m_pSet.MoveFirst();
				if (!m_pSet.m_FileDirectory.IsEmpty())
				{
					if (!m_ftp.DeleteDirAndFile(m_pSet.m_FileDirectory,myFtp))
					{
						AfxMessageBox("ɾ�����ɹ���");
						m_pSet.m_pDatabase->Rollback();
						return;
					}
				}
				m_pSet.Delete();
				m_pSet.Requery();				
				hParantItem=m_Tree1.GetParentItem(m_CurrentTvItemForMenu);
				m_Tree1.DeleteItem(m_CurrentTvItemForMenu);
				SetList(hParantItem);
			}
			m_pSet.m_pDatabase->CommitTrans();
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			
			m_pSet.m_pDatabase->Rollback();
		}
		break;
	case 1:
		if(m_Tree2.GetRootItem()==m_CurrentTvItemForMenu) return;
		try
		{
			m_Set.m_pDatabase->BeginTrans();
			m_Set.m_strFilter.Format("[FatherNumber]=%ld",(long)m_Tree2.GetItemData(m_CurrentTvItemForMenu));
			m_Set.Open();
			if (!m_Set.IsBOF())
			{
				if (AfxMessageBox("��������Ϊ�գ��Ƿ�ȫ��ɾ��",MB_YESNO)==IDNO)
				{
					m_pSet.m_pDatabase->Rollback();
					return;
				}
			}
			m_Set.m_strFilter.Format("[Number]=%ld",(long)m_Tree2.GetItemData(m_CurrentTvItemForMenu));
			m_Set.Requery();
			if (!m_Set.IsBOF())
			{
				m_Set.MoveFirst();
				if (!m_Set.m_FileDirectory.IsEmpty())
				{
					if (!m_ftp.DeleteDirAndFile(m_Set.m_FileDirectory,myFtp ))
					{
						AfxMessageBox("ɾ�����ɹ���");
						m_pSet.m_pDatabase->Rollback();
						return;
					}				
				}
				m_Set.Delete();
				m_Set.Requery();				
				//�Ի�����ɾ��
				hParantItem=m_Tree2.GetParentItem(m_CurrentTvItemForMenu);
				m_Tree2.DeleteItem(m_CurrentTvItemForMenu);
				SetList(hParantItem);
			}	
			m_Set.m_pDatabase->CommitTrans();			
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			delete			e;
			m_Set.m_pDatabase->Rollback();
		}
		break;
	default:
		break;
	}
}
/************************************************************************/
/* ������                                                               */
/************************************************************************/
void CTempWorkPlaceDlg::OnRename()
{
	// TODO: �ڴ���������������
	switch(m_iSelTab) {
	case 0:
		m_Tree1.Expand(m_CurrentTvItemForMenu,TVE_EXPAND);
		m_Tree1.EnsureVisible(m_CurrentTvItemForMenu);
		m_Tree1.EditLabel(m_CurrentTvItemForMenu);
		break;
	case 1:
		m_Tree2.Expand(m_CurrentTvItemForMenu,TVE_EXPAND);
		m_Tree2.EnsureVisible(m_CurrentTvItemForMenu);
		m_Tree2.EditLabel(m_CurrentTvItemForMenu);
		break;
	default:
		break;
	}
}
/************************************************************************/
/* ��ģ��                                                             */
/************************************************************************/
void CTempWorkPlaceDlg::OnOpenMdl()
{
	// TODO: �ڴ���������������
	//�õ�ѡ�е���
	int			type=0;
	POSITION	pos = m_List.GetFirstSelectedItemPosition();
	LVITEM		iLem;
	int nItem = m_List.GetNextSelectedItem(pos);
	if((nItem==-1 || nItem==0 )) return;    //�������,�Ҳ�Ϊ��һ��
	iLem.iItem	=nItem;
	iLem.iSubItem=0;
	iLem.state	=LVIS_SELECTED;
	iLem.mask	=TVIF_IMAGE|LVIF_STATE|TVIF_SELECTEDIMAGE;
	iLem.state	=LVIS_SELECTED;
	m_List.GetItem(&iLem);
	type=iLem.iImage;    //�����0  1
	if (type==0) return;
	switch(m_iSelTab) {
	case 0:
        if(OpenPartTempMdl((long)m_List.GetItemData(nItem)))
		{
			OnOK();
		}
		break;
	case 1:
		if(OpenAsmTempMdl((long)m_List.GetItemData(nItem)))
		{
			OnOK();
		}
		break;
	default:
		break;
	}
}
/************************************************************************/
/* ɾ��ģ��                                                             */
/************************************************************************/
void CTempWorkPlaceDlg::OnDelMdl()
{
	// TODO: �ڴ���������������

	int						type=0;
	CAsmTempManageTableSet	m_Set(m_pSet.m_pDatabase);
	POSITION				pos = m_List.GetFirstSelectedItemPosition();
	LVITEM					iLem;
	int						nItem = m_List.GetNextSelectedItem(pos);
	CFtpFile				m_ftp;
	CString                 tempDir;
	if((nItem==-1 || nItem==0 )) return;    //�������,�Ҳ�Ϊ��һ��
	iLem.iItem		=nItem;
	iLem.iSubItem	=0;
	iLem.state	=LVIS_SELECTED;
	iLem.mask	=TVIF_IMAGE|LVIF_STATE|TVIF_SELECTEDIMAGE;
	iLem.state	=LVIS_SELECTED;
	m_List.GetItem(&iLem);
	type	=iLem.iImage;    //�����0  1
	if (type==0) return;

	switch(m_iSelTab) {
	case 0:
		if (DelTempPartMdl(m_pSet.m_pDatabase,(long)m_List.GetItemData(nItem)))
		{
			SetPath(&m_Tree1,m_Tree1.GetSelectedItem());
			SetList(m_Tree1.GetSelectedItem());
		}	
		break;
	case 1:
		if (DelTempAsmMdl(m_pSet.m_pDatabase,(long)m_List.GetItemData(nItem)))
		{
			SetPath(&m_Tree2,m_Tree2.GetSelectedItem());
			SetList(m_Tree2.GetSelectedItem());
		}
		break;
	default:
		break;
	}
}
/************************************************************************/
/* �鿴ģ�͵�����                                                       */
/************************************************************************/
void CTempWorkPlaceDlg::OnProp()
{
	// TODO: �ڴ���������������
}
/************************************************************************/
/* �Ҽ��˵�                                                             */
/************************************************************************/
void CTempWorkPlaceDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: �ڴ������Ϣ����������
	CMenu mTreeRightMenu;
	VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));	
	CMenu * pPopup;
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nItem = m_List.GetNextSelectedItem(pos);
	CTreeCtrl * pmyTreeCtrl=(m_iSelTab==0?&m_Tree1:&m_Tree2);
    switch(GetStatus()) {
    case 0:                    //�����ؼ��ڵ��
		pPopup=mTreeRightMenu.GetSubMenu(7);
		m_CurrentTvItemForMenu=m_CurrentTvItem;
    	break;
	case 1:                     //�½��������ͱ���ģ�ͣ�������е����
		pPopup=mTreeRightMenu.GetSubMenu(6);
		pPopup->EnableMenuItem(ID_12120,MF_DISABLED|MF_GRAYED);
		pPopup->EnableMenuItem(ID_Menu12121,MF_DISABLED|MF_GRAYED);
		pPopup->EnableMenuItem(ID_Menu12162,MF_DISABLED|MF_GRAYED);
		pPopup->EnableMenuItem(ID_12118,MF_DISABLED|MF_GRAYED);
		pPopup->EnableMenuItem(ID_12119,MF_DISABLED|MF_GRAYED);
		pPopup->EnableMenuItem(ID_12126,MF_DISABLED|MF_GRAYED); 
		m_CurrentTvItemForMenu=m_CurrentTvItem;
		if(!HaveMdlCurrent() && dlgMode!=-1 )	pPopup->EnableMenuItem(ID_Menu12160,MF_DISABLED|MF_GRAYED);  //��ǰû��ģ���Լ�������±�����Ч
        if(m_CurrentTvItemForMenu==pmyTreeCtrl->GetRootItem()) pPopup->EnableMenuItem(ID_Menu12160,MF_DISABLED|MF_GRAYED); //��Ŀ¼�²��ܱ���
		break; 
	case 2:                    //���б����ѡ����Ŀ¼���ļ���ͼ�꣩
		pPopup=mTreeRightMenu.GetSubMenu(6);
		pPopup->EnableMenuItem(ID_12120,MF_DISABLED|MF_GRAYED);
		pPopup->EnableMenuItem(ID_Menu12121,MF_DISABLED|MF_GRAYED);
		pPopup->EnableMenuItem(ID_Menu12162,MF_DISABLED|MF_GRAYED);
		if(!HaveMdlCurrent())	pPopup->EnableMenuItem(ID_Menu12160,MF_DISABLED|MF_GRAYED);  //��ǰû��ģ�ͣ�������Ч
		m_CurrentTvItemForMenu=ConverListItemTohItem((long)m_List.GetItemData(nItem));
		if(m_CurrentTvItemForMenu==pmyTreeCtrl->GetRootItem()) pPopup->EnableMenuItem(ID_Menu12160,MF_DISABLED|MF_GRAYED); //��Ŀ¼�²��ܱ���
		break;
	case 3:                   //���б����ѡ������������ͼ��
		pPopup=mTreeRightMenu.GetSubMenu(6);
		pPopup->EnableMenuItem(ID_12117,MF_DISABLED|MF_GRAYED);
		pPopup->EnableMenuItem(ID_12118,MF_DISABLED|MF_GRAYED);
		pPopup->EnableMenuItem(ID_12119,MF_DISABLED|MF_GRAYED);
		pPopup->EnableMenuItem(ID_12126,MF_DISABLED|MF_GRAYED); 
		/*if(!HaveMdlCurrent() && dlgMode!=-1)	*/pPopup->EnableMenuItem(ID_Menu12160,MF_DISABLED|MF_GRAYED);
		break;
    default:
		return;
		break;
    }
	ASSERT(pPopup!=NULL);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
}
/************************************************************************/
/* ����Ҽ�ʱ��״̬���                                                 */
/************************************************************************/
int CTempWorkPlaceDlg::GetStatus(void)
{
	CTreeCtrl * pmyTreeCtrl=(m_iSelTab==0?&m_Tree1:&m_Tree2);
	CPoint point=GetCurrentMessage()->pt;
	if (WindowFromPoint(point)==pmyTreeCtrl)
	{
		return 0;      //��
	}
	else if (WindowFromPoint(point)==&m_List)
	{
		POSITION pos = m_List.GetFirstSelectedItemPosition();
		int nItem = m_List.GetNextSelectedItem(pos);
		LVITEM   iLem;
		iLem.iItem=nItem;
		iLem.iSubItem=0;
		iLem.state=LVIS_SELECTED;
		iLem.mask=TVIF_IMAGE|LVIF_STATE|TVIF_SELECTEDIMAGE;
		iLem.state=LVIS_SELECTED;
		m_List.GetItem(&iLem);
		//type=iLem.iImage;    //�����0  1
		if (nItem==-1 && m_CurrentTvItem!=NULL) //���б���û��ѡ�У����е�����ؼ�
		{
			return 1;                           //�������Ӳ���
		}
		else if (nItem==0)
		{
			return -1;                          //������е��ǵ�һ����򲻳��ֲ˵�����Ϊ��һ���Ǵ����ϲ�
		}
		else if (iLem.iImage==0)
		{
			return 2;
		}
		else if (iLem.iImage==1 || iLem.iImage==2)
		{
			return 3;
		}
	}
	return -1;
}
/************************************************************************/
/* ���б���ת������                                                     */
/************************************************************************/
HTREEITEM CTempWorkPlaceDlg::ConverListItemTohItem(long lItem)
{
	HTREEITEM hItem=NULL;
	CTreeCtrl *  pmyTreeCtrl=(m_iSelTab==0?&m_Tree1:&m_Tree2);
	if(m_CurrentTvItem!=NULL && pmyTreeCtrl->ItemHasChildren(m_CurrentTvItem) && lItem>=0) 
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = pmyTreeCtrl->GetChildItem(m_CurrentTvItem); 
		while (hChildItem != NULL)
		{
			hNextItem = pmyTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
			if(pmyTreeCtrl->GetItemData(hChildItem)==lItem)
			{
				hItem=hChildItem;
				return hItem;
				break;
			}
			hChildItem = hNextItem;
		}
	}
	return hItem;
}
/************************************************************************/
/* ����ģ��                                                             */
/************************************************************************/
void CTempWorkPlaceDlg::OnMenu12160()
{
	// TODO: �ڴ���������������
	if(m_CurrentTvItemForMenu==NULL) return;
	switch(m_iSelTab) {
	case 0:
		SavePart(m_CurrentTvItemForMenu,(long)m_Tree1.GetItemData(m_CurrentTvItemForMenu));
		break;
	case 1:
		SaveAsm(m_CurrentTvItemForMenu,(long)m_Tree2.GetItemData(m_CurrentTvItemForMenu));
		break;
	default:
		break;
	}
}

BOOL CTempWorkPlaceDlg::SavePart(HTREEITEM hItem,long fatherNum)
{
	CString                tempPath;
	CString                fileDir;
	int                    num=0;
	int                    flag=0;
	int                    mdlNumber=0;
	UsrPartPorp            temp_porp;
	int                    indexNo=0;
	CFtpFile               m_ftp;
	CString                name;
	ProSolid               solid;
	ProError               status;
	CPart_Temp_Para_Info   m_partParaInfo(m_pSet.m_pDatabase);
	CPart_Temp_Perf        m_partPerf(m_pSet.m_pDatabase);
	CPart_Temp_Pic         m_partPic(m_pSet.m_pDatabase);
	CPart_Temp_Rel         m_partRel(m_pSet.m_pDatabase);
	status=ProMdlCurrentGet((ProMdl*)&solid);
	if(status!=PRO_TK_NO_ERROR) return FALSE;
	//1.�õ�����ģ�͵Ļ�����Ϣ
	temp_porp=GetMdlInfo(solid);
	m_pSet.m_strFilter.Empty();
    if(!m_pSet.IsOpen())
	{
		m_pSet.Open();
		m_pSet.Requery();
	}		
	if (m_pSet.m_pDatabase->CanTransact()) {		
		try
		{
			m_pSet.m_pDatabase->BeginTrans();
			//��ӻ�����Ϣ
			m_pSet.AddNew();
			m_pSet.m_Name=temp_porp.Name;
			m_pSet.m_FatherNum=fatherNum;
			m_pSet.m_Type="mdl";
			m_pSet.m_MdlerName=temp_porp.Person;
			m_pSet.m_MdlCode=temp_porp.Code;
			m_pSet.m_Note=temp_porp.Note;
			m_pSet.m_MdlTypeNum=temp_porp.TypeNum;
			m_pSet.m_MdlTypeName=temp_porp.TypeName;
			m_pSet.m_Material	=temp_porp.Material;
			CTime  time=CTime::GetCurrentTime();
			m_pSet.m_Datatime=time;//.Format("%Y-%A-%d");
			m_pSet.Update();
			m_pSet.m_strFilter.Empty();
			m_pSet.Requery();
			m_pSet.MoveLast();
			mdlNumber=m_pSet.m_Number;
			m_pSet.Edit();
			fileDir.Format("/��ģ������/���/%ld",mdlNumber);
			m_pSet.m_FileName=GetMdlFileName(solid);
			m_pSet.m_FileDirectory=fileDir;
			if (temp_porp.bHaveDraw)
			{
				m_pSet.m_DrawFileName=GetMdlFileName(temp_porp.hDraw);
				m_pSet.m_DrawFileDirectory=fileDir+"/"+"drawing";
			}
			m_pSet.Update();
			//����������Ա�
			num=(int)p_UsrSWTXTableList.GetCount();
			m_partParaInfo.Open();
			for(int i=0;i<num;i++)
			{
				if(p_UsrSWTXTableList[i].owner!=solid) continue;
				if(p_UsrSWTXTableList[i].isCheck!=1)   continue;
				indexNo++;
				m_partParaInfo.AddNew();
				m_partParaInfo.m_PrtNum=mdlNumber;
				m_partParaInfo.m_Number=indexNo;
				m_partParaInfo.m_Name=p_UsrSWTXTableList[i].Name;
				m_partParaInfo.m_IdNum=p_UsrSWTXTableList[i].id;
				m_partParaInfo.m_ParaFeatName=p_UsrSWTXTableList[i].featName;
				m_partParaInfo.m_DimSymbol=p_UsrSWTXTableList[i].dimSymbol;
				m_partParaInfo.m_Value=p_UsrSWTXTableList[i].value;
				m_partParaInfo.m_Type=p_UsrSWTXTableList[i].type;
				m_partParaInfo.m_SubType=p_UsrSWTXTableList[i].subType;
				m_partParaInfo.m_UpLimit=p_UsrSWTXTableList[i].UpLimit;
				m_partParaInfo.m_DwLimit=p_UsrSWTXTableList[i].DwLimit;
				m_partParaInfo.m_Note=p_UsrSWTXTableList[i].Note;
				m_partParaInfo.Update();
			}
			m_partParaInfo.Close();
			indexNo=0;
			num=(int)p_UsrPerfParaTableList.GetCount();
			m_partPerf.Open();
			//������ܲ�����Ϣ
			for(int i=0;i<num;i++)
			{
				if(p_UsrPerfParaTableList[i].owner!=solid) continue;
				if(p_UsrPerfParaTableList[i].isCheck!=1) continue;
				indexNo++;
				m_partPerf.AddNew();
				m_partPerf.m_PrtNum=mdlNumber;
				m_partPerf.m_Number=indexNo;
				m_partPerf.m_Name=p_UsrPerfParaTableList[i].Name;
				m_partPerf.m_Value=p_UsrPerfParaTableList[i].value;
				m_partPerf.m_Note=p_UsrPerfParaTableList[i].Note;
				m_partPerf.m_Type   = p_UsrPerfParaTableList[i].Type;
				m_partPerf.m_MaxVal = p_UsrPerfParaTableList[i].MaxValue;
				m_partPerf.m_MinVal = p_UsrPerfParaTableList[i].MinValue;
				m_partPerf.Update();

			}
			m_partPerf.Close();
			//��ӹ�ϵ��Ϣ
			CArray<CRel,CRel&>	p_RelList;
			CArray<int,int&>	p_IndexList;
			CString				rel;
			g_RelCollection.GetSolidAllRel(solid,false,&p_RelList,&p_IndexList);
			num=(int)p_RelList.GetCount();
			m_partRel.Open();
			indexNo=0;
			for (int i=0;i<num;i++)
			{
				p_RelList[i].GetRelString(rel);
				indexNo++;
				m_partRel.AddNew();
				m_partRel.m_PrtNum=mdlNumber;
				m_partRel.m_Number=indexNo;
				m_partRel.m_Relation=rel;
				m_partRel.m_Note=p_RelList[i].GetRelNote();
				m_partRel.Update();
				m_partRel.Requery();
			}
			m_partRel.Close();



			indexNo=0;
			num=(int)p_UsrPicture.GetCount();
			m_partPic.Open();
			//���ͼƬ��Ϣ
			for(int i=0;i<num;i++)
			{
				if(p_UsrPicture[i].owner!=solid) continue;
				if(p_UsrPicture[i].isCheck!=1) continue;
				indexNo++;
				CString error_info;
				m_partPic.AddNew();
				m_partPic.m_PrtNum=mdlNumber;
				m_partPic.m_Number=indexNo;
				m_partPic.m_Name=p_UsrPicture[i].fileName;
				m_partPic.m_Note=p_UsrPicture[i].Note;
				m_partPic.m_IsMain=p_UsrPicture[i].isMain;
				DBSavePic(&m_partPic.m_File,p_UsrPicture[i].filePath+p_UsrPicture[i].fileName,&m_partPic,error_info);
				m_partPic.Update();
			}
			m_partPic.Close();

			//������ʱ�ļ���,����ģ��
			if(!CreateTempDir(solid,&tempPath))
			{
				m_pSet.m_pDatabase->Rollback();
				m_pSet.Close();
				return FALSE;
			}
			//�ϴ�ģ��
			if (!m_ftp.CreateDirectory(myFtp,fileDir)) {
				RemoveDireAndFile(tempPath);
				m_pSet.m_pDatabase->Rollback();
				m_pSet.Close();
				return FALSE;
			}
			if (!m_ftp.UploadDirectorytoDirectory(tempPath,fileDir,myFtp)) {
				m_ftp.RemoveDirectory(myFtp,fileDir);
				RemoveDireAndFile(tempPath);
				m_pSet.m_pDatabase->Rollback();
				m_pSet.Close();
				return FALSE;
			}
			RemoveDireAndFile(tempPath);			
			SetList(hItem);
			m_Tree1.Expand(hItem,TVE_EXPAND);
			m_Tree1.EnsureVisible(hItem);
			m_pSet.m_pDatabase->CommitTrans();
			AfxMessageBox("����ɹ���");
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			m_pSet.m_pDatabase->Rollback();
			
			RemoveDireAndFile(tempPath);
			return FALSE;

		}
	}
	return TRUE;
}

int CTempWorkPlaceDlg::NewPartWorkPlace(void)
{
	CString     name;
	//HTREEITEM   hItem=m_Tree1.GetSelectedItem();
	HTREEITEM   hItem=m_CurrentTvItemForMenu;
	CString     fatherDir="";
	CFtpFile    m_ftp;
	int         newNum=0;
	//�����¹�������
	try
	{
		for(int i=0;i<10000;i++)
		{
			name.Format("�¹�������(%d)",i);
			m_pSet.m_strFilter.Format("[FatherNum]=%d AND [Name]='%s'",(long)m_Tree1.GetItemData(hItem),name);
			m_pSet.Requery();
			if (m_pSet.IsBOF()) {
				break;
			}
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		return 0;
	}
	try
	{
		m_pSet.m_strFilter.Empty();
		m_pSet.Requery();
		m_pSet.AddNew();
		m_pSet.m_FatherNum=(long)m_Tree1.GetItemData(hItem);
		m_pSet.m_Name=name;
		m_pSet.m_Type="wrk";
		m_pSet.m_FileDirectory="";
		m_pSet.m_Datatime=CTime::GetCurrentTime();
		m_pSet.Update();
		m_pSet.Requery();
		m_pSet.MoveLast();
		HTREEITEM newItem=m_Tree1.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T(name), 
			0, 0, 0, 0, m_pSet.m_Number, hItem, NULL);
		newNum=m_pSet.m_Number;
		m_Tree1.Expand(hItem,TVE_EXPAND);
		//����Ŀ¼

	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		return 0;
	} 
	return 0;
}

void CTempWorkPlaceDlg::OnTvnBegindragTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CTempWorkPlaceDlg::OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFtpFile m_ftp;
	CString strItem=pTVDispInfo->item.pszText;

	if (strItem.GetLength()>15) {
		AfxMessageBox("���������ֳ��ȳ���Ҫ��");
		*pResult = 0;
		m_Tree1.EditLabel(m_Tree1.GetSelectedItem());
		return;
	}
	if (strItem.GetLength()!=0) {
		try
		{
			m_pSet.m_strFilter.Format("[Number]=%d",(long)m_Tree1.GetItemData(pTVDispInfo->item.hItem));
			m_pSet.Requery();
			if(m_pSet.IsBOF())
			{
				*pResult = 0;
				return;
			}
			m_pSet.MoveFirst();
			m_pSet.Edit();
			m_pSet.m_Name=strItem;
			m_pSet.m_FileDirectory="";
			m_pSet.Update();
			m_Tree1.SetItemText(pTVDispInfo->item.hItem,pTVDispInfo->item.pszText);
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);		
		}

	}
	SetList(m_Tree1.GetParentItem(pTVDispInfo->item.hItem));
	m_Tree1.Select(pTVDispInfo->item.hItem,TVGN_CARET);
	*pResult = 0;
}
/************************************************************************/
/* ��������½���������                                                 */
/************************************************************************/
int CTempWorkPlaceDlg::NewAsmWorkPlace(void)
{
	CAsmTempManageTableSet		m_Set(m_pSet.m_pDatabase);
	CString						name;
	//HTREEITEM   hItem=m_Tree2.GetSelectedItem();
	HTREEITEM					hItem=m_CurrentTvItemForMenu;
	CString						fatherDir="";
	CFtpFile					m_ftp;
	int							newNum=0;
	//�����¹�������
	try
	{
		if (!m_Set.Open())
		{
			AfxMessageBox("���ݿ�����ʧ�ܣ�");
			return 0;
		}
		for(int i=0;i<10000;i++)
		{
			name.Format("�¹�������(%d)",i);
			m_Set.m_strFilter.Format("[FatherNumber]=%d AND [Name]='%s'",(long)m_Tree2.GetItemData(hItem),name);
			m_Set.Requery();
			if (m_Set.IsBOF()) {
				break;
			}
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		
		return 0;
	}

	try
	{
		m_Set.m_strFilter.Empty();
		m_Set.Requery();
		m_Set.AddNew();
		m_Set.m_FatherNumber=(long)m_Tree1.GetItemData(hItem);
		m_Set.m_Name=name;
		m_Set.m_Type="wrk";
		m_Set.m_FileDirectory="";
		m_Set.m_Datatime=CTime::GetCurrentTime();
		m_Set.Update();
		m_Set.Requery();
		m_Set.MoveLast();
		HTREEITEM newItem=m_Tree2.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T(name), 
			0, 0, 0, 0, m_Set.m_Number, hItem, NULL);
		newNum=m_Set.m_Number;
		m_Tree2.Expand(hItem,TVE_EXPAND);
		//����Ŀ¼

	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		m_ftp.DeleteDirAndFile(fatherDir,myFtp);
		
		return 0;
	}
	return 0;
}
/************************************************************************/
/* �޸�����                                                             */
/************************************************************************/
void CTempWorkPlaceDlg::OnTvnBeginlabeleditTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CTempWorkPlaceDlg::OnTvnEndlabeleditTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAsmTempManageTableSet	m_Set(m_pSet.m_pDatabase);
	CFtpFile				m_ftp;
	CString					strItem=pTVDispInfo->item.pszText;
	CString					oldName="",oldDir="",newDir="";
	int						_location=0,location=0;
	//HTREEITEM hParent=m_Tree.GetParentItem(pTVDispInfo->item);
	//CString strItem
    //���������в��������е�����һ��
	if (strItem.GetLength()>15) {
		AfxMessageBox("���������ֳ��ȳ���Ҫ��");
		*pResult = 0;
		m_Tree2.EditLabel(m_Tree2.GetSelectedItem());
		return;
	}
	if (strItem.GetLength()!=0) {
		try
		{
			m_Set.m_strFilter.Format("[Number]=%d",(long)m_Tree2.GetItemData(pTVDispInfo->item.hItem));
			m_Set.Open();
			if(m_Set.IsBOF())
			{
				*pResult = 0;
				return;
			}
			m_Set.MoveFirst();
			m_Set.Edit();
			m_Set.m_Name=strItem;
			m_Set.m_FileDirectory="";
			m_Set.Update();
			m_Set.m_strFilter.Empty();
			m_Set.Requery();
			m_Tree2.SetItemText(pTVDispInfo->item.hItem,pTVDispInfo->item.pszText);
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);		
		}
	}
	SetList(m_Tree2.GetParentItem(pTVDispInfo->item.hItem));
	m_Tree2.Select(pTVDispInfo->item.hItem,TVGN_CARET);
	*pResult = 0;
}
/************************************************************************/
/* �������ģ��                                                         */
/************************************************************************/
BOOL CTempWorkPlaceDlg::SaveAsm(HTREEITEM hItem, long  fatherNum)
{
    CAsmTempSave save;
	ProSolid     asm_solid;
	if(ProMdlCurrentGet((ProMdl*)&asm_solid)!=PRO_TK_NO_ERROR) return FALSE;
	if(save.Save(asm_solid,fatherNum,m_pSet.m_pDatabase)) 
	{
		AfxMessageBox("����ɹ�");
		SetList(hItem);
		m_Tree2.Expand(hItem,TVE_EXPAND);
		m_Tree2.EnsureVisible(hItem);
		return TRUE;
	}
	return FALSE;
}
/************************************************************************/
/* �л�ģʽ��ȷ��Tabҳ��Ҫ��ӵ�ҳ��                                    */
/************************************************************************/
int CTempWorkPlaceDlg::SetMode(int mode)
{
	dlgMode=mode;//0��Ϊ���ģʽ��1Ϊ���ģʽ��-1Ϊ�����ã���Ϊ��ģʽ
	return 0;
}

void CTempWorkPlaceDlg::OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
/************************************************************************/
/* ��ѡ�е����                                                       */
/************************************************************************/
BOOL CTempWorkPlaceDlg::OpenPartTempMdl(long part_num)
{
	ProError		status;
	ProPath			/*wcurrentDir,*/name;
	CString			currentDit,newDir,error_info,fullFileName;
	CFtpFile		m_ftp;
	ProType			type;
	ProName			w_name;
	int				w_id;
	ProMdl			mdl;
	ProDrawing		draw_mdl=NULL;
	CPart_Temp_Mdl_Info     m_partPropInfo(m_pSet.m_pDatabase); 

	if (!SelectAnDirectory(currentDit,""))
	{
		return FALSE;
	}
	//status=ProDirectoryCurrentGet(wcurrentDir);
	//currentDit=CString(wcurrentDir);
	try
	{
		m_partPropInfo.m_strFilter.Format("[Number]=%ld",part_num);
		m_partPropInfo.Open();
		if (m_partPropInfo.IsBOF()) 
		{
			AfxMessageBox("ģ���Ѿ������ڣ����ܴ򿪣�");
			return FALSE;
		}
		m_partPropInfo.MoveFirst();
		//��鵱ǰ�Ƿ����Ѿ�������ͬ�ļ�����ģ�ʹ�
		//////////////////////////////////////////////////////////////////////////
		if (!CreateDirectoryGivenPathAndName(currentDit,m_partPropInfo.m_Name,newDir))
		{
			AfxMessageBox("�ڱ��ع���Ŀ¼������ʱĿ¼ʧ�ܣ�");
			return FALSE;
		}
		currentDit=currentDit+newDir;
		if (!m_ftp.DownloadDirectorytoDirectory(m_partPropInfo.m_FileDirectory,currentDit,myFtp))
		{
			AfxMessageBox("�ӷ�������ȡ�ļ������в��ɹ�����������");
			return FALSE;
		}
		fullFileName=currentDit+"\\"+m_partPropInfo.m_FileName;
		ProStringToWstring(name,fullFileName.GetBuffer());
		status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,&mdl); 
		//������ڹ���ͼ
		if (!m_partPropInfo.m_DrawFileName.IsEmpty())
		{
			fullFileName=currentDit+"\\drawing\\"+m_partPropInfo.m_DrawFileName;
			ProStringToWstring(name,fullFileName.GetBuffer());
			status=ProMdlLoad(name,PRO_MDL_DRAWING,PRO_B_FALSE,(ProMdl*)&draw_mdl); 
		}
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("û�ܴ����ص����ص��ļ�������汾��");
			return FALSE;
		}
		status = ProMdlTypeGet(mdl, (ProMdlType*)&type);
		status= ProMdlNameGet(mdl, w_name);
		status= ProObjectwindowCreate(w_name, type, &w_id);
		if(!status== PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
		status = ProMdlDisplay(mdl);

	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		return FALSE;
	}
	status = ProWindowActivate(w_id);
	AddPartParaRelPic(_T(currentDit+"\\"),mdl,draw_mdl,part_num);
	return TRUE;
}
/************************************************************************/
/* ���������Ϣ��ӽ���                                                 */
/************************************************************************/
void CTempWorkPlaceDlg::AddPartParaRelPic(CString localDir, ProMdl mdl,ProMdl drawing=NULL,long part_num=0)
{
	CPart_Temp_Mdl_Info     m_partPropInfo(m_pSet.m_pDatabase); 
	CPart_Temp_Para_Info	m_partParaInfo(m_pSet.m_pDatabase);
	CPart_Temp_Perf			m_partPerf(m_pSet.m_pDatabase);
	CPart_Temp_Pic			m_partPic(m_pSet.m_pDatabase);
	CPart_Temp_Rel			m_partRel(m_pSet.m_pDatabase);
	int						propNum=0;
	int						num=0;
	EraseMdlInfo(mdl,ERASE_ALL);
	try
	{
		m_partPropInfo.m_strFilter.Format("[Number]=%d",part_num);
		m_partPropInfo.Open();
		if (m_partPropInfo.IsBOF()) 
		{
			return;
		}
		//���������Ϣ
		UsrPartPorp * tempProp=GetMdlPInfo(mdl);
		tempProp->owner	=mdl;
		tempProp->Name	=m_partPropInfo.m_Name;
		tempProp->Person=m_partPropInfo.m_MdlerName;
		if (!m_partPropInfo.m_DrawFileName.IsEmpty())
		{
			tempProp->hDraw=drawing;
			tempProp->bHaveDraw=true;
		}
		else
			tempProp->bHaveDraw=false;
		tempProp->Code		=m_partPropInfo.m_MdlCode;
		tempProp->TypeNum	=m_partPropInfo.m_MdlTypeNum;
		tempProp->TypeName	=m_partPropInfo.m_MdlTypeName;
        tempProp->Note		=m_partPropInfo.m_Note;
		tempProp->Material	=m_partPropInfo.m_Material;

		//��Ӳ�����Ϣ
		m_partParaInfo.m_strFilter.Format("[PrtNum]=%d",part_num);
		m_partParaInfo.Open();
		if (!m_partParaInfo.IsBOF())
		{
			m_partParaInfo.MoveFirst();
			while (!m_partParaInfo.IsEOF())
			{
				UsrSWTXTable1 temp;
				temp.id=m_partParaInfo.m_IdNum;
				temp.Name=m_partParaInfo.m_Name;
				temp.isCheck=1;
				temp.Note=m_partParaInfo.m_Note;
				temp.featName=m_partParaInfo.m_ParaFeatName;
				temp.dimSymbol=m_partParaInfo.m_DimSymbol;
				temp.value=m_partParaInfo.m_Value;
				temp.owner=mdl;
				temp.type=m_partParaInfo.m_Type;
				temp.subType=m_partParaInfo.m_SubType;
				temp.DwLimit=m_partParaInfo.m_DwLimit;
				temp.UpLimit=m_partParaInfo.m_UpLimit;
				temp.SetDimPara((ProSolid)mdl);
				p_UsrSWTXTableList.Add(temp);
				m_partParaInfo.MoveNext();
			}
		}

		//������ܲ�����Ϣ
		m_partPerf.m_strFilter.Format("[PrtNum]=%d",part_num);
		m_partPerf.Open();
		if (!m_partPerf.IsBOF())
		{
			m_partPerf.MoveFirst();
			while (!m_partPerf.IsEOF())
			{
				UsrPerfParaTable temp_perf;
				temp_perf.isCheck=1;temp_perf.Name=m_partPerf.m_Name;
				temp_perf.owner=mdl;temp_perf.value=m_partPerf.m_Value;
				temp_perf.Note=m_partPerf.m_Note;
				temp_perf.Type=m_partPerf.m_Type;
				temp_perf.MaxValue=m_partPerf.m_MaxVal;
				temp_perf.MinValue=m_partPerf.m_MinVal;
				p_UsrPerfParaTableList.Add(temp_perf);
				m_partPerf.MoveNext();
			}
		}


		////��ӹ�ϵʽ��Ϣ
		m_partRel.m_strFilter.Format("[PrtNum]=%d",part_num);
		m_partRel.Open();
		if (!m_partRel.IsBOF())
		{
			m_partRel.MoveFirst();
			while (!m_partRel.IsEOF())
			{
				g_RelCollection.AddRel((ProSolid)mdl
					,m_partRel.m_Relation,m_partRel.m_Note);							
				m_partRel.MoveNext();
			}
		}


		//���ͼƬ��Ϣ
		m_partPic.m_strFilter.Format("[PrtNum]=%d",part_num);
		m_partPic.Open();
		if (!m_partPic.IsBOF())
		{
			m_partPic.MoveFirst();
			while (!m_partPic.IsEOF())
			{
				UsrPicture temp_pic;
				CString	   error_info;
				CString    temp_full_filename;
				int        tempnum=0;
				CString    mdlName=GetMdlInfo(mdl).Name;
				temp_pic.owner=mdl;
				temp_pic.isCheck=1;
				temp_pic.isMain=m_partPic.m_IsMain;
				temp_pic.Note=m_partPic.m_Note;				
				temp_pic.fileName=mdlName+".jpg";
				temp_pic.filePath=GetMdlPath(mdl);
				temp_full_filename=temp_pic.filePath+temp_pic.fileName;
				while (PathFileExistsA(temp_full_filename.GetBuffer()))
				{
					tempnum++;
					temp_pic.fileName.Format("%s_%d.jpg",mdlName,tempnum);
					temp_full_filename=temp_pic.filePath+temp_pic.fileName;
				}
				if (temp_pic.isMain==1)
				{
					SetMdlInfo(mdl)->pic_path=temp_pic.filePath+temp_pic.fileName;
				}
				DBOpenPicSaveAsFile(&m_partPic.m_File,temp_pic.filePath+temp_pic.fileName);
				p_UsrPicture.Add(temp_pic);
				m_partPic.MoveNext();
			}
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		
	}
}

/************************************************************************/
/*  ��ѡ�е����                                                      */
/************************************************************************/
BOOL CTempWorkPlaceDlg::OpenAsmTempMdl(long asm_num)
{
	CAsmTempOpen asm_open;
	if (asm_open.Open(m_pSet.m_pDatabase,asm_num))
	{
		return TRUE;
	}
	return FALSE;
}
void CTempWorkPlaceDlg::AddAsmParaRelPic(CString localDir, ProMdl mdl,long asm_num)
{
}
/************************************************************************/
/* ���������Ҽ��˵�,������                                            */
/************************************************************************/
void CTempWorkPlaceDlg::OnOpenWorkPlace()
{
	// TODO: �ڴ���������������
	CTreeCtrl * pmyTreeCtrl=(m_iSelTab==0?&m_Tree1:&m_Tree2);
	if(m_CurrentTvItem==NULL) return;
	pmyTreeCtrl->Expand(m_CurrentTvItem,TVE_EXPAND);
	pmyTreeCtrl->EnsureVisible(m_CurrentTvItem);
	SetList(m_CurrentTvItem);
}
/************************************************************************/
/* �½�������                                                           */
/************************************************************************/
void CTempWorkPlaceDlg::OnNew()
{
	// TODO: �ڴ���������������
	OnNewWorkPlace();
}
/************************************************************************/
/* ɾ��                                                                 */
/************************************************************************/
void CTempWorkPlaceDlg::OnMenu12167()
{
	// TODO: �ڴ���������������
	OnDelWorkPlace();
}

/************************************************************************/
/* ������                                                               */
/************************************************************************/
void CTempWorkPlaceDlg::OnRenameWorkPlace()
{
	// TODO: �ڴ���������������
	OnRename();
}
BOOL  CTempWorkPlaceDlg::DelTempPartMdl(CDatabase * p_db,long num)
{
	CPart_Temp_Mdl_Info m_set(p_db);
	CString				tempDir="";
	CFtpFile			m_ftp;

	try
	{
		if (!m_set.Open())
		{
			AfxMessageBox("�������ݿ�ʧ��!");
			return FALSE;
		}
		m_set.m_strFilter.Format("[Number]=%ld",num);
		m_set.Requery();
		m_set.m_pDatabase->BeginTrans();
		if (!m_set.IsBOF())
		{
			m_set.MoveFirst();	
			tempDir=m_set.m_FileDirectory;
			m_set.Delete();
			if(!m_ftp.DeleteDirAndFile(tempDir,myFtp))
			{
				m_set.m_pDatabase->Rollback();
				AfxMessageBox("ɾ�����ɹ���");
				return FALSE;
			}	
			m_set.Requery();
		}
		m_set.m_pDatabase->CommitTrans();
	}
	catch (CDBException * e){
		AfxMessageBox(e->m_strError);
		m_set.m_pDatabase->Rollback();
		return FALSE;
	}
	return TRUE;
}
BOOL  CTempWorkPlaceDlg::DelTempAsmMdl(CDatabase * p_db,long num)
{
	CAsmTempManageTableSet	m_set(p_db);
	CString					tempDir="";
	CFtpFile				m_ftp;
	try
	{
		if (!m_set.Open())
		{
			AfxMessageBox("�������ݿ�ʧ��!");
			return FALSE;
		}
		m_set.m_strFilter.Format("[Number]=%ld",num);
		m_set.Requery();
		m_set.m_pDatabase->BeginTrans();
		if (!m_set.IsBOF())
		{
			m_set.MoveFirst();	
			tempDir=m_set.m_FileDirectory;
			m_set.Delete();
			if(!m_ftp.DeleteDirAndFile(tempDir,myFtp))
			{
				m_set.m_pDatabase->Rollback();
				AfxMessageBox("ɾ�����ɹ���");
				return FALSE;
			}
			m_set.Requery();			
		}
		m_set.m_pDatabase->CommitTrans();
	}
	catch (CDBException * e){
		AfxMessageBox(e->m_strError);
		m_set.m_pDatabase->Rollback();
		return FALSE;
	}
	return TRUE;
}

