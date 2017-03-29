// FemSaveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "FemSaveDlg.h"
#include "FemOpenDlg.h"
#include "FtpFile.h"
// CFemSaveDlg �Ի���

IMPLEMENT_DYNAMIC(CFemSaveDlg, CDialog)
CFemSaveDlg::CFemSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFemSaveDlg::IDD, pParent)
	, m_Name(_T(""))
	, m_Note(_T(""))
	, m_Type(_T(""))
	, m_SelTypeNum(-1)
	, m_SelMdlNum(-1)
{
}

CFemSaveDlg::~CFemSaveDlg()
{
}

void CFemSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_EDIT2, m_Note);
	DDX_Text(pDX, IDC_EDIT3, m_Type);
}


BEGIN_MESSAGE_MAP(CFemSaveDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
//	ON_WM_MOVING()
ON_WM_MOVE()
END_MESSAGE_MAP()


// CFemSaveDlg ��Ϣ�������
/************************************************************************/
/* ����                                                                 */
/************************************************************************/
void CFemSaveDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString  tempPathName;
	CString  serverPathName;
	long     newNumber=0;
	CFtpFile     ftp;
	ProSolid  solid=(ProSolid)GetCurrentMdl();
	UpdateData(TRUE);
	//�жϵ�ǰģ������ɽ�ģ�����û�У����ܽ���
	if (GetMdlInfo(solid).MdlType!=3)
	{
		AfxMessageBox("��ǰģ��û��ȷ������ģ�ͣ��޷��������Ԫ��ģ!");
		return;
	}
	
	if(!CreateTempDir(solid,&tempPathName)) 
	{
		AfxMessageBox(_T("����ģ�͹����г������ȱ����ģ�ͣ�"));
		return;
	}
	try{
		if(!m_pSet.m_pDatabase->CanTransact()) return;
		if(!m_pSet.m_pDatabase->BeginTrans()) 
		{
			AfxMessageBox("���ݿⲻ֧��������ʧ��");
			return;
		}
		m_pSet.AddNew();
		m_pSet.m_Name=m_Name;
		m_pSet.m_FatherNum=(m_SelTypeNum==-1?0:m_SelTypeNum);
		m_pSet.m_Note=m_Note;
		m_pSet.m_Type="mdl";
		m_pSet.m_DateTime=CTime::GetCurrentTime();
		m_pSet.m_FileDirectory="NULL";
		m_pSet.m_ConnPrtMdlNum=GetMdlInfo(solid).Number;
		m_pSet.Update();
		m_pSet.m_strFilter.Empty();
		m_pSet.Requery();
		m_pSet.MoveLast();
		newNumber=m_pSet.m_Number;
		serverPathName.Format("/����Ԫģ��/%ld",newNumber);
		if (!ftp.CreateDirectory(myFtp,serverPathName))
		{
			AfxMessageBox(_T("�ڷ����������ļ���ʧ�ܣ�"));
			m_pSet.m_pDatabase->Rollback();
			return;
		}
		tempPathName=tempPathName+"\\"+GetMdlFileName(solid)+".1";
		if(!ftp.UploadFiletoDirectory(tempPathName,serverPathName,myFtp))
		{
			AfxMessageBox(_T("����ģ���ļ���������������ʧ�ܣ�"));
			m_pSet.m_pDatabase->Rollback();
			return;
		}
		m_pSet.Edit();
		m_pSet.m_FileDirectory = serverPathName+"/"+GetMdlFileName(solid)+".1";
		m_pSet.Update();
		m_pSet.m_pDatabase->CommitTrans();
		ShowFemMdlInList(m_SelTypeNum);
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		RemoveDireAndFile(tempPathName);
		m_pSet.m_pDatabase->Rollback();
	}	
}
/************************************************************************/
/* �޸�                                                                 */
/************************************************************************/
void CFemSaveDlg::OnBnClickedEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
/************************************************************************/
/* �ر�                                                                 */
/************************************************************************/
void CFemSaveDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
/************************************************************************/
/* �½����                                                             */
/************************************************************************/
void CFemSaveDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_pSet.AddNew();
	if(m_Name.IsEmpty())
	{
		AfxMessageBox("���Ʋ���Ϊ��");
		return;
	}
	m_pSet.m_Name=m_Name;
	m_pSet.m_FatherNum=(m_SelTypeNum==-1?0:m_SelTypeNum);
	m_pSet.m_ConnPrtMdlNum=-1;
	m_pSet.m_Note=m_Note;
	m_pSet.m_Type="typ";
	m_pSet.m_FileDirectory="NULL";
	m_pSet.m_DateTime=CTime::GetCurrentTime();
    m_pSet.Update();
	ShowFemMdlTree(m_Tree,m_pSet.m_pDatabase);
}

BOOL CFemSaveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ImageList.Create(12,12,ILC_COLOR16,4,4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	m_pSet.Open();
	ShowFemMdlTree(m_Tree,m_pSet.m_pDatabase);

	//�б��
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,60,-1);
	m_List.InsertColumn(1,"����ģ��",LVCFMT_LEFT,60,-1);
	m_List.InsertColumn(2,"��ģʱ��",LVCFMT_LEFT,100,-1);	
	m_List.InsertColumn(3,"��ע",LVCFMT_LEFT,200,-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}


void CFemSaveDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int  ImageNum=0,ImageSelNum=0;
	//CPartMdlRecordSet  m_tempset(m_pSet.m_pDatabase);
	//m_tempset.Open();
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	m_Tree.GetItemImage(hItem,ImageNum,ImageSelNum);
	m_List.DeleteAllItems();
	if (hItem!=m_Tree.GetRootItem() && ImageNum==0)
	{
		ShowFemMdlInList((long)m_Tree.GetItemData(hItem));
		//m_pSet.m_strFilter.Format("FatherNum=%ld",(long)m_Tree.GetItemData(hItem));
		//m_pSet.Requery();
		//if (!m_pSet.IsBOF())
		//{
		//	m_pSet.MoveFirst();
		//	do {
		//		//��ù���ģ������
		//		m_tempset.m_strFilter.Format("Number=%ld",m_pSet.m_ConnPrtMdlNum);
		//		m_tempset.Requery();
		//		if (!m_tempset.IsBOF())
		//		{
		//			m_tempset.MoveFirst();

		//			m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List.GetItemCount()
		//				,m_pSet.m_Name,0,0,0,m_pSet.m_Number);
		//			m_List.SetItemText(m_List.GetItemCount()-1,1,m_pSet.m_DateTime.Format("%Y-%A-%d %H:%M:%S"));
		//			m_List.SetItemText(m_List.GetItemCount()-1,2,m_tempset.m_Name);
		//			m_List.SetItemText(m_List.GetItemCount()-1,3,m_pSet.m_Note);
		//		} 
		//		m_pSet.MoveNext();
		//	} while(!m_pSet.IsEOF());
		//}
	}

	if (hItem==m_Tree.GetRootItem())
		m_SelTypeNum=-1;
	else
	    m_SelTypeNum=(long)m_Tree.GetItemData(hItem);
	UpdateData(TRUE);
	m_Type=m_Tree.GetItemText(hItem);
	UpdateData(FALSE);
	*pResult = 0;
}
void CFemSaveDlg::ShowFemMdlInList(long typnum)
{
	CPartMdlRecordSet  m_tempset(m_pSet.m_pDatabase);	
	m_tempset.Open();
	m_pSet.m_strFilter.Format("FatherNum=%ld",typnum);
	m_pSet.Requery();
	m_List.DeleteAllItems();
	m_SelMdlNum=-1;
	if (!m_pSet.IsBOF())
	{
		m_pSet.MoveFirst();
		do {
			//��ù���ģ������
			m_tempset.m_strFilter.Format("Number=%ld",m_pSet.m_ConnPrtMdlNum);
			m_tempset.Requery();
			if (!m_tempset.IsBOF())
			{
				m_tempset.MoveFirst();

				m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List.GetItemCount()
					,m_pSet.m_Name,0,0,0,m_pSet.m_Number);
				m_List.SetItemText(m_List.GetItemCount()-1,1,m_tempset.m_Name);
				m_List.SetItemText(m_List.GetItemCount()-1,2,m_pSet.m_DateTime.Format("%Y-%A-%d"));				
				m_List.SetItemText(m_List.GetItemCount()-1,3,m_pSet.m_Note);
			} 
			m_pSet.MoveNext();
		} while(!m_pSet.IsEOF());
	}
}
void CFemSaveDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION  pos = m_List.GetFirstSelectedItemPosition();
	int		  nItem = m_List.GetNextSelectedItem(pos);
	
	if (nItem!=-1)
	{
		m_pSet.m_strFilter.Format("Number=%ld",(long)m_List.GetItemData(nItem));
		m_pSet.Requery();
		if (!m_pSet.IsBOF())
		{
			UpdateData(TRUE);
			m_Name=m_pSet.m_Name;
			m_Note=m_pSet.m_Note;
			UpdateData(FALSE);
			m_SelMdlNum=(long)m_List.GetItemData(nItem);
		}
	}
	else
	{
		m_SelMdlNum=-1;
	}
	*pResult = 0;
}

//void CFemSaveDlg::OnMoving(UINT fwSide, LPRECT pRect)
//{
//	CDialog::OnMoving(fwSide, pRect);
//
//	// TODO: �ڴ������Ϣ����������
//	UsrRepaintWindow();
//}

void CFemSaveDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: �ڴ������Ϣ����������
	UsrRepaintWindow();
}
