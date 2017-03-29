// FemModelSave.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "FemModelSave.h"
#include "NewPartTypeDlg.h"


// CFemModelSave �Ի���

IMPLEMENT_DYNAMIC(CFemModelSave, CDialog)
CFemModelSave::CFemModelSave(CWnd* pParent /*=NULL*/)
	: CDialog(CFemModelSave::IDD, pParent)
{
}

CFemModelSave::~CFemModelSave()
{
}

void CFemModelSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SAVE, m_Tree);
}


BEGIN_MESSAGE_MAP(CFemModelSave, CDialog)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_COMMAND(ID_NEWCAT, OnNewcat)
	ON_COMMAND(ID_DELCAT, OnDelcat)
	ON_COMMAND(ID_Refresh, OnRefresh)
END_MESSAGE_MAP()


// CFemModelSave ��Ϣ�������

BOOL CFemModelSave::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(ProMdlCurrentGet((ProMdl*)&part_solid)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ǰû��ģ��");
		//OnClose();
		//return FALSE;
	}
	//����ͼƬ�б�
	m_ImageList.Create(14, 14, ILC_COLOR32, 4, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	//��ʾĿ¼��
	ShowPartMdlTree();
	return TRUE;                      
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CFemModelSave::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}




/*******************************************************************************************\
������ShowPartMdlTree(void)
˵������ʾ��Ŀ¼
\*******************************************************************************************/
void CFemModelSave::ShowPartMdlTree(void)
{
	//��Ӹ��ڵ�
	m_Tree.DeleteAllItems();
	HTREEITEM hItemRoot=m_Tree.InsertItem(_T("����Ԫģ��"));
	if(m_pSet.IsOpen()) m_pSet.Close();
	m_pSet.m_strFilter="";
	if(!m_pSet.Open(CRecordset::dynaset,_T("SELECT * FROM [dbo].[FemPrt]"),CRecordset::readOnly))
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
		if(m_pSet.m_FatherNum==0)
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
void CFemModelSave::AddSubPartItem(HTREEITEM item)
{
	//CString sql_lang;
	HTREEITEM tempItem;
	if(m_pSet.IsEOF()) return;
	m_pSet.MoveFirst();
	do {
		if(m_pSet.m_FatherNum==(long)m_Tree.GetItemData(item))
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


/************************************************************************/
/* ���е��Ҽ��˵�����                                                   */
/************************************************************************/
void CFemModelSave::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
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
		//�½����ID_NEWCAT ɾ����� ID_DELCAT
		if (iTem.iImage!=2) {
		//	pPopup->EnableMenuItem(ID_DELCAT,MF_DISABLED|MF_GRAYED);
			//pPopup->EnableMenuItem(ID_NEWCAT,MF_DISABLED|MF_GRAYED);
		}
		else 
		{
			pPopup->EnableMenuItem(ID_DELCAT,MF_ENABLED);
			pPopup->EnableMenuItem(ID_NEWCAT,MF_ENABLED);

		}
		ASSERT(pPopup!=NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);

	}
}











/***************************************************************************************\
������OnNewcat()
˵���������ؼ��ϵ���Ҽ�����½����,ͬʱ�ڷ������ϴ���Ŀ¼
\***************************************************************************************/
void CFemModelSave::OnNewcat()
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
		directory="/����Ԫģ��";
		i=0;
	}
	else   //������Ǹ�Ŀ¼
	{
		i=(long)m_Tree.GetItemData(hItem);
		if(m_pSet.IsOpen()) m_pSet.Close();
		sql_lang.Format("SELECT * FROM [dbo].[FemPrt] WHERE [Number] = %d",i);
		m_pSet.Open(AFX_DB_USE_DEFAULT_TYPE,_T(sql_lang),CRecordset::none);
		if(m_pSet.IsBOF()) 
		{			
			dlg.SetFatherName("��Ŀ¼","");
			i=0;
			directory="/����Ԫģ��";
		}
		else
		{           
			m_pSet.MoveFirst();
			if (m_pSet.m_Type=="mdl") {
				m_pSet.Close();
				return;
			}
			//dlg.SetFatherName(m_pSet.m_Name,m_pSet.m_MdlCode);
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
			m_pSet.m_FatherNum=i;                //���� i
			m_pSet.m_Name=dlg.m_TypeName;           //�����
			m_pSet.m_Type="typ";                    //����Ϊ���
			m_pSet.m_Note=dlg.m_TypeNote;           //��ע
		//	m_pSet.m_MdlCode=dlg.m_Code;            //����
			m_pSet.m_FileDirectory=directory;       //Ŀ¼
			m_pSet.m_DateTime=CTime::GetCurrentTime();
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

void CFemModelSave::OnDelcat()
{
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

void CFemModelSave::OnRefresh()
{
	ShowPartMdlTree();
}
