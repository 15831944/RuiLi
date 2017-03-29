// PartMainDlg.cpp : ʵ���ļ�
//���Ŀ¼�����Լ������ģ��������
//�����Ŀ¼�������ԡ���ϵʽ���������Ա����ܲ�����ͼƬ����ȹ���

#include "stdafx.h"
#include "Main.h"
#include "PartMainDlg.h"
#include "PartMdlRecordSet.h"
#include "NewPartTypeDlg.h"
#include "MdlOkDlg.h"
#include "partVariantTableSet.h"
#include "partPerfparaTableSet.h"
#include "partRelationSet.h"
#include "partRelativeDimParaTableSet.h"
#include "partPicSet.h"
#include "PartSeriesDlg.h"
#include "AddMdlSeriesDlg.h"
#include "UsrSWTXTable.h"
//#define	HAVE_BOOLEAN
//#define		BMP_FILE			0x00
//#define		JPG_FILE			0x01

//// CPartMainDlg �Ի���
//extern int DlgIcon;
//void  IsMdlInfoDefAndDef(ProSolid mdl);
//extern CArray<UsrSWTXTable1,UsrSWTXTable1>  p_UsrSWTXTableList;  //����ȫ�ֵ��������Ա����
////�ڳ�������ʱ��Ҫ����һ����ʼֵ(��Ч)
//extern CArray<UsrRelTable,UsrRelTable>    p_UsrRelTableList;   //����ȫ�ֵĹ�ϵ�б�������
//extern CArray<UsrPicture,UsrPicture>   p_UsrPicture;           //ͼƬ��ȫ�ֱ���    
//extern CArray<UsrPerfParaTable,UsrPerfParaTable> p_UsrPerfParaTableList;
//extern CArray<UsrPartPorp,UsrPartPorp> p_UsrPartPorp; 
//
//extern FtpConnectInfo myFtp;
IMPLEMENT_DYNAMIC(CPartMainDlg, CDialog)
CPartMainDlg::CPartMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartMainDlg::IDD, pParent)
	, part_solid(NULL)
{
	
}

CPartMainDlg::~CPartMainDlg()
{
}

void CPartMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_Tree);
}


BEGIN_MESSAGE_MAP(CPartMainDlg, CDialog)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_NEW_PARTTYPE, OnNewParttype)
	ON_WM_CLOSE()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_COMMAND(ID_DEL_PARTTYPE, OnDelParttype)
	ON_NOTIFY(NM_CLICK, IDC_TREE, OnNMClickTree)
	ON_COMMAND(ID_L_, OnSelectTypeForMdl)
	ON_COMMAND(ID_L_12105, OnRefreshTree)
	ON_COMMAND(ID_DEL_PARTMDL, OnDelPartmdl)
	ON_COMMAND(ID_L_12108, OnL12108)
	ON_COMMAND(ID_Menu12107, OnMenuMdlInfo)
END_MESSAGE_MAP()


// CPartMainDlg ��Ϣ�������

/*******************************************************************************************\
������ShowPartMdlTree(void)
˵������ʾ��Ŀ¼
\*******************************************************************************************/
void CPartMainDlg::ShowPartMdlTree(void)
{
	//��Ӹ��ڵ�
    m_Tree.DeleteAllItems();
	HTREEITEM hItemRoot=m_Tree.InsertItem(_T("���ģ��"));
	if(m_pSet.IsOpen()) m_pSet.Close();
	m_pSet.m_strFilter="";
	if(!m_pSet.Open(CRecordset::dynaset,_T("SELECT * FROM [dbo].[PartManageTable]"),CRecordset::readOnly))
	{
		AfxMessageBox("���ݿ����Ӳ��ɹ���");
		return;
	}
	if(m_pSet.IsBOF())
	{
		m_pSet.Close();
		return;
	}
	m_pSet.MoveFirst();    
    while (!m_pSet.IsEOF()) {
		if(m_pSet.m_FatherNumber==0)
		{

			HTREEITEM hItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T(m_pSet.m_Name), 
				0, 0, 0, 0, m_pSet.m_Number, hItemRoot, NULL);
		
			int num=m_pSet.m_Number;
            AddSubPartItem(hItem);
			m_pSet.MoveFirst();
			while (m_pSet.m_Number!=num) {
				m_pSet.MoveNext();
			}
		}
		m_pSet.MoveNext();
	}
    m_pSet.Close();
    m_Tree.Expand(hItemRoot,TVE_EXPAND);
}

/*******************************************************************************************\
������AddSubPartItem(HTREEITEM item)
˵������ʾ��������
\*******************************************************************************************/
void CPartMainDlg::AddSubPartItem(HTREEITEM item)
{
	//CString sql_lang;
	HTREEITEM tempItem;
	if(m_pSet.IsEOF()) return;
	m_pSet.MoveFirst();
	do {
		if(m_pSet.m_FatherNumber==(long)m_Tree.GetItemData(item))
		{

			int i=0,j=0;
			int nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			if(m_pSet.m_Type=="mdl") i=2,j=2;
			else  i=j=0;
			tempItem= m_Tree.InsertItem(nMarsk,_T(m_pSet.m_Name), 
				i, j, 0, 0, m_pSet.m_Number, item, NULL);
			int num=m_pSet.m_Number;
			AddSubPartItem(tempItem);
			m_pSet.MoveFirst();
			while (m_pSet.m_Number!=num) {
				m_pSet.MoveNext();
			}
		}
		m_pSet.MoveNext();
	} while(!m_pSet.IsEOF());	
}


BOOL CPartMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	
	HICON hIcon = AfxGetApp()->LoadIcon (DlgIcon);
	SetIcon (hIcon, TRUE);     // Set small icon  ����Сͼ��
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(ProMdlCurrentGet((ProMdl*)&part_solid)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ǰû��ģ��");
		OnClose();
		return FALSE;
	}
	//����ͼƬ�б�
	m_ImageList.Create(14, 14, ILC_COLOR32, 4, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	//��������ҳ
	SetSheepPage();
	//��ʾĿ¼��
	ShowPartMdlTree();
	return TRUE;                                     // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ���е��Ҽ��˵�����                                                   */
/************************************************************************/
void CPartMainDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �ڴ������Ϣ����������
	CPoint point=GetCurrentMessage()->pt;
	if(WindowFromPoint(point)==&m_Tree)
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(0);
		//����ͼƬ���ͣ�ѡ��˵��Ƿ���Ч
		HTREEITEM hItem=m_Tree.GetSelectedItem();
		TVITEM iTem;
		iTem.hItem=hItem;
		iTem.mask=TVIF_IMAGE;
		m_Tree.GetItem(&iTem);
		//�½����ID_NEW_PARTTYPE ɾ����� ID_DEL_PARTTYPE
		//ɾ��ģ��ID_DEL_PARTMDL  ģ��ϵ�л� ID_L_12108
		if (iTem.iImage!=2) {
			pPopup->EnableMenuItem(ID_DEL_PARTMDL,MF_DISABLED|MF_GRAYED);
			pPopup->EnableMenuItem(ID_L_12108,MF_DISABLED|MF_GRAYED);
		}
		else 
		{
		    pPopup->EnableMenuItem(ID_DEL_PARTMDL,MF_ENABLED);
		    pPopup->EnableMenuItem(ID_L_12108,MF_ENABLED);
			pPopup->EnableMenuItem(ID_NEW_PARTTYPE,MF_DISABLED|MF_GRAYED);
			pPopup->EnableMenuItem(ID_DEL_PARTTYPE,MF_DISABLED|MF_GRAYED);
			pPopup->EnableMenuItem(ID_L_,MF_DISABLED|MF_GRAYED);

		}
		ASSERT(pPopup!=NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);

	}
}

/***************************************************************************************\
������OnNewParttype����
˵���������ؼ��ϵ���Ҽ�����½����,ͬʱ�ڷ������ϴ���Ŀ¼
\***************************************************************************************/
void CPartMainDlg::OnNewParttype()
{
	// TODO: �ڴ���������������

	CNewPartTypeDlg dlg;
	long i=0;
	CString sql_lang;
	CString directory;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	
	//���ѡ�е��Ǹ�Ŀ¼
    if(hItem==m_Tree.GetRootItem())
	{
		dlg.SetFatherName("��Ŀ¼","");
		directory="/���ģ��";
		i=0;
	}
	else   //������Ǹ�Ŀ¼
	{
		i=(long)m_Tree.GetItemData(hItem);
		if(m_pSet.IsOpen()) m_pSet.Close();
		sql_lang.Format("SELECT * FROM [dbo].[PartManageTable] WHERE [Number] = %d",i);
		m_pSet.Open(AFX_DB_USE_DEFAULT_TYPE,_T(sql_lang),CRecordset::none);
		if(m_pSet.IsBOF()) 
		{			
			dlg.SetFatherName("��Ŀ¼","");
			i=0;
			directory="/���ģ��";
		}
		else
		{           
			m_pSet.MoveFirst();
			if (m_pSet.m_Type=="mdl") {
				m_pSet.Close();
				return;
			}
			dlg.SetFatherName(m_pSet.m_Name,m_pSet.m_MdlCode);
			directory=m_pSet.m_FileDirectory;
		}
		m_pSet.Close();
	}
	//�򿪶Ի���

	if(dlg.DoModal()==IDCANCEL)
	{
		return;
	}
	else if (dlg.m_TypeName=="") {
		return;
	}
	else if(dlg.m_TypeName!="")
	{
       try
	   {
		   if (!m_pSet.m_pDatabase->CanTransact())  return;
		    m_pSet.m_pDatabase->BeginTrans();
		    directory=directory+"/"+dlg.m_TypeName;//ȷ����Ҫ�ڷ�������Ҫ����Ŀ¼
		    m_pSet.m_strFilter.Empty();
		    m_pSet.Open();
			m_pSet.AddNew();	  
			m_pSet.m_FatherNumber=i;                //���� i
			m_pSet.m_Name=dlg.m_TypeName;           //�����
			m_pSet.m_Type="typ";                    //����Ϊ���
			m_pSet.m_Note=dlg.m_TypeNote;           //��ע
			m_pSet.m_MdlCode=dlg.m_Code;            //����
			m_pSet.m_FileDirectory=directory;       //Ŀ¼
			m_pSet.m_Datatime=CTime::GetCurrentTime();
			m_pSet.Update();
			if(!m_ftp.CreateDirectory(myFtp,directory))
			{
				AfxMessageBox("�������Ŀ¼���ɹ������������������û�Ŀ¼�Ƿ���ڣ�\n���Ŀ¼�Ѿ����ڣ�");
				m_pSet.m_pDatabase->Rollback();
				if(m_pSet.IsOpen()) m_pSet.Close();
				ShowPartMdlTree();
				return;		  
			}
			m_pSet.m_pDatabase->CommitTrans();
			m_pSet.Requery();
			m_pSet.MoveLast();
			m_Tree.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE
				,_T(dlg.m_TypeName),0, 0, 0, 0, m_pSet.m_Number, hItem, NULL);
			m_Tree.Expand(hItem,TVE_EXPAND);
		    m_pSet.Close();
	   }
	   catch (...) {
		   AfxMessageBox("�½�ʧ�ܣ�");
		   m_pSet.m_pDatabase->Rollback();
		   if(m_pSet.IsOpen()) m_pSet.Close();
		   return;
	   }	   
	}
}
/************************************************************************/
/* �Ҽ��˵�ɾ�����                                                     */
/************************************************************************/
void CPartMainDlg::OnDelParttype()
{
	// TODO: �ڴ���������������
	long i=0;
    CString sql_lang;
	CString directory;
    HTREEITEM hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem()) return;
    i=(long)m_Tree.GetItemData(hItem);
	if(m_pSet.IsOpen()) m_pSet.Close();
	//�����ж���û���¼�Ŀ¼���ڣ�����д�������ɾ��
	m_pSet.m_strFilter.Format("[FatherNumber] = %d",i);	
    m_pSet.Open();
	if(!m_pSet.IsBOF())
	{
		AfxMessageBox("����ɾ��������µ�ģ�ͻ������");
		m_pSet.Close();
		return;
	}
	m_pSet.Close();

	m_pSet.m_strFilter.Format("[Number] = %d",i);		
	m_pSet.Open();
	if(m_pSet.IsBOF()) 
	{
		AfxMessageBox("��ǰ��𲻴��ڣ�");
		ShowPartMdlTree();
		return;
	}
	m_pSet.MoveFirst();
	if(m_pSet.m_Type=="mdl")
	{
		m_pSet.Close();
		return;
	}
	directory=m_pSet.m_FileDirectory;	
	if(!m_ftp.DeleteDirAndFile(_T(directory),myFtp))
	{
		AfxMessageBox("��������ڱ�ʹ�ã�����ɾ����");
		m_pSet.Close();
		return;
	}

	try
	{
		m_pSet.Delete();
		m_Tree.DeleteItem(hItem);
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		e->Delete();
		m_pSet.MoveFirst();
		m_pSet.Close();
		return;
	}	
	m_pSet.Requery();
	m_pSet.Close();

}
void CPartMainDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int window_id;
	ProWindowCurrentGet(&window_id);
	ProWindowActivate(window_id);
	CDialog::OnClose();
}

void CPartMainDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	// TODO: �ڴ������Ϣ����������
    UsrRepaintWindow();
}
/************************************************************************/
/* // ���ݸ�ѡ���������������Ӧ��Ҫ��ʾ��ҳ��                          */
/************************************************************************/
void CPartMainDlg::SetSheepPage(void)
{	
    //�����ѡ��Ϊָ����ǰģ��
	SetSheetOfCurrentMdl();
}

/************************************************************************/
/* �����ɽ�ģ֮��Ķ���                                               */
/************************************************************************/
void CPartMainDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMdlOkDlg dlg;
	dlg.part_solid=part_solid;
	dlg.SetDbConnet(m_pSet.m_pDatabase);
	if (dlg.DoModal())
	{
		ShowPartMdlTree();
	}
	
}
/************************************************************************/
/* ��ǰģ��ģʽ�£���ʾ��ǰģ�͵��������Ա����ܲ�������ϵʽ��ͼƬ����Ϣ*/
/************************************************************************/
int CPartMainDlg::SetSheetOfCurrentMdl(void)
{

	ProSolid solid;                  //���õ�ǰģ�͵�����
	ProMdlCurrentGet((ProMdl*)&solid);	
    IsMdlInfoDefAndDef(solid);       //�ж�ģ�͵Ļ�����Ϣ�Ƿ��Ѿ����壬���û������
    m_page4.SetValue(solid,&p_UsrPartPorp);
	m_sheet.AddPage(&m_page4); 
	m_page1.SetValue(solid,&p_UsrSWTXTableList);
	m_sheet.AddPage(&m_page1);         //�������Ա�	
	m_page2.SetValue(solid,&p_UsrPerfParaTableList);	
	m_sheet.AddPage(&m_page2);         //���ܲ�����	
	m_page3.SetValue(solid/*,&p_UsrRelTableList*/);		
	m_sheet.AddPage(&m_page3);               //��ϵʽ
	m_page5.SetValue(solid,&p_UsrPicture);		
	m_sheet.AddPage(&m_page5);                    //ͼƬ���ҳ
	//��Create������һ������ҳ
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE|WS_GROUP, WS_EX_CONTROLPARENT);
	//��������ҳ�Ĵ�С��λ��		
	m_sheet.SetWindowPos(NULL, 0, 0,0,0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	return 0;
}
/************************************************************************/
/* �����οؼ��У������������Ķ���                                     */
/************************************************************************/
void CPartMainDlg::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}
/************************************************************************/
/* ѡ�����                                                             */
/************************************************************************/
void CPartMainDlg::OnSelectTypeForMdl()
{
	// TODO: �ڴ���������������
		long i=0;
		int num=0;
		ProSolid solid;		
		ProMdlCurrentGet((ProMdl*)&solid);	
		CString sql_lang;

		HTREEITEM hItem=m_Tree.GetSelectedItem();
		if(hItem==m_Tree.GetRootItem()) return;
		i=(long)m_Tree.GetItemData(hItem);
		try
		{
			if(m_pSet.IsOpen()) m_pSet.Close();
			//�����ѡ��������û�б��޸�
			//sql_lang.Format("SELECT * FROM [dbo].[PartManageTable] WHERE [Number] = %d",i);
			m_pSet.m_strFilter.Format("[Number] = %d",i);
			m_pSet.Open();
			if(m_pSet.IsBOF()) 
			{
				m_pSet.Close();
				AfxMessageBox("��ǰ����Ѿ������ڣ�");
				ShowPartMdlTree();
				return;
			}
			m_pSet.MoveFirst();
			if(m_pSet.m_Type=="mdl") 
			{
				m_pSet.Close();
				return;
			}
			m_page4.m_TypeEdit.SetWindowText(m_pSet.m_Name);
			m_page4.m_CodeEdit.SetWindowText(m_pSet.m_MdlCode+".(������)");

			num=(int)p_UsrPartPorp.GetCount();
			for(int j=0;j<num;j++)
			{
				if(p_UsrPartPorp[j].owner!=solid) continue;
				if(p_UsrPartPorp[j].isDef!=1) continue;

				p_UsrPartPorp[j].FatherNumber=i;                           //m_pSet.m_Number;
				p_UsrPartPorp[j].Code=m_pSet.m_MdlCode+".(������)";        // ģ�͵Ĵ�����ʱΪ���Ĵ���
				p_UsrPartPorp[j].Type=m_pSet.m_Name;           
				break;
			}
			m_pSet.Close();

		}
		catch (CDBException * e) {
			
			AfxMessageBox(e->m_strError);		
		}	
	return;
}


void CPartMainDlg::OnRefreshTree()
{
	// TODO: �ڴ���������������
	ShowPartMdlTree();
}

void CPartMainDlg::OnDelPartmdl()
{
	// TODO: �ڴ���������������
	long       indexNo=0;
	CString    sql_lang;
	CString    message;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem()) return;
    indexNo=(int)m_Tree.GetItemData(hItem);
	if(m_pSet.IsOpen()) m_pSet.Close();
	m_pSet.m_strFilter.Format("[Number]=%d",indexNo);	
	//�������ݿ����Ӻͷ���������
	if(!m_pSet.Open())
	{
		AfxMessageBox("���ݲ��ܴ򿪣�");
		return;
	}
    if(!m_ftp.TestConnect(myFtp))
	{
		AfxMessageBox("����������ʧ�ܣ�");
		return;
	}

	if(m_pSet.IsBOF()) 
	{
		m_pSet.Close();
		return;
	}
	m_pSet.MoveFirst();
	if(m_pSet.m_Type!="mdl") 
	{
		m_pSet.Close();
		return;
	}
	message.Format("ȷ��Ҫɾ����ģ��[%s]����ϵ����",_T(m_pSet.m_Name));
	if(AfxMessageBox(message,MB_OK)!=IDOK)
	{
		m_pSet.Close();
		return;
	}
    m_ftp.DeleteDirAndFile(_T(m_pSet.m_FileDirectory),myFtp);	
	try
	{
        m_pSet.Delete();            //ɾ���������������Ϣ��ͨ�����ݿ�Ĵ���������
		m_Tree.DeleteItem(hItem);
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		delete e;
	}
	m_pSet.Close();
	//ShowPartMdlTree();
}
/************************************************************************/
/* �Ҽ��˵���ģ��ϵ�л�                                                 */
/************************************************************************/
void CPartMainDlg::OnL12108()
{
	// TODO: �ڴ���������������
	CPartSeriesDlg dlg;
	long          Number=0;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem()) return;
    Number=(int)m_Tree.GetItemData(hItem);
	if(m_pSet.IsOpen()) m_pSet.Close();
	m_pSet.m_strFilter.Format("[Number]=%d",Number);
	//�������ݿ����Ӻͷ���������
	if(!m_pSet.Open())
	{
		AfxMessageBox("���ݲ��ܴ򿪣�");
		return;
	}
	if(m_pSet.IsBOF()) 
	{
		m_pSet.Close();
		return;
	}
	m_pSet.MoveFirst();
	if(m_pSet.m_Type!="mdl") 
	{
		m_pSet.Close();
		return;
	}
	dlg.SetMdlNumberAndDB(Number,m_pSet.m_pDatabase);
	dlg.DoModal();

}
/************************************************************************/
/* �鿴ģ�͵�����                                                       */
/************************************************************************/
void CPartMainDlg::OnMenuMdlInfo()
{
	// TODO: �ڴ���������������
	CAddMdlSeriesDlg dlg;
	MdlInsInfo       mdlInfo;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if (m_Tree.GetRootItem()==hItem) return;
	long number=(long)m_Tree.GetItemData(hItem);
	try
	{
		if(m_pSet.IsOpen()) m_pSet.Close();
		m_pSet.m_strFilter.Format("[Number]=%d",number);
		m_pSet.Open();
		if (m_pSet.IsBOF()) {
			m_pSet.Close();
			m_pSet.m_strFilter.Empty();
		}
		m_pSet.MoveFirst();
        mdlInfo.Name=m_pSet.m_Name;
		mdlInfo.Code=m_pSet.m_MdlCode;
		mdlInfo.Person=m_pSet.m_MdlerName;
		mdlInfo.Note=m_pSet.m_Note;
		mdlInfo.m_time=m_pSet.m_Datatime;
		mdlInfo.mdlNum=number;
		mdlInfo.Dept=m_pSet.m_MdlerDepartment;
		dlg.newMdl=mdlInfo;
		dlg.strCaption="�鿴������Ϣ";
		dlg.SetMdlNumberAndCode(-1,"",NULL);
		dlg.DoModal();
		m_pSet.m_strFilter.Empty();
		m_pSet.Close();
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		if(m_pSet.IsOpen()) m_pSet.Close();
	}
	return;
}
