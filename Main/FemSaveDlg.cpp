// FemSaveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "FemSaveDlg.h"
#include "FemOpenDlg.h"
#include "FtpFile.h"
// CFemSaveDlg 对话框

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


// CFemSaveDlg 消息处理程序
/************************************************************************/
/* 保存                                                                 */
/************************************************************************/
void CFemSaveDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString  tempPathName;
	CString  serverPathName;
	long     newNumber=0;
	CFtpFile     ftp;
	ProSolid  solid=(ProSolid)GetCurrentMdl();
	UpdateData(TRUE);
	//判断当前模型是完成建模，如果没有，则不能进行
	if (GetMdlInfo(solid).MdlType!=3)
	{
		AfxMessageBox("当前模型没有确认生成模型，无法完成有限元建模!");
		return;
	}
	
	if(!CreateTempDir(solid,&tempPathName)) 
	{
		AfxMessageBox(_T("备份模型过程中出错，请先保存该模型！"));
		return;
	}
	try{
		if(!m_pSet.m_pDatabase->CanTransact()) return;
		if(!m_pSet.m_pDatabase->BeginTrans()) 
		{
			AfxMessageBox("数据库不支持事务处理，失败");
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
		serverPathName.Format("/有限元模型/%ld",newNumber);
		if (!ftp.CreateDirectory(myFtp,serverPathName))
		{
			AfxMessageBox(_T("在服务器创建文件夹失败！"));
			m_pSet.m_pDatabase->Rollback();
			return;
		}
		tempPathName=tempPathName+"\\"+GetMdlFileName(solid)+".1";
		if(!ftp.UploadFiletoDirectory(tempPathName,serverPathName,myFtp))
		{
			AfxMessageBox(_T("保存模型文件到服务器过程中失败！"));
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
/* 修改                                                                 */
/************************************************************************/
void CFemSaveDlg::OnBnClickedEdit()
{
	// TODO: 在此添加控件通知处理程序代码
}
/************************************************************************/
/* 关闭                                                                 */
/************************************************************************/
void CFemSaveDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
/************************************************************************/
/* 新建类别                                                             */
/************************************************************************/
void CFemSaveDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pSet.AddNew();
	if(m_Name.IsEmpty())
	{
		AfxMessageBox("名称不能为空");
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

	// TODO:  在此添加额外的初始化
	m_ImageList.Create(12,12,ILC_COLOR16,4,4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	m_pSet.Open();
	ShowFemMdlTree(m_Tree,m_pSet.m_pDatabase);

	//列表框
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"名称",LVCFMT_LEFT,60,-1);
	m_List.InsertColumn(1,"关联模型",LVCFMT_LEFT,60,-1);
	m_List.InsertColumn(2,"建模时间",LVCFMT_LEFT,100,-1);	
	m_List.InsertColumn(3,"备注",LVCFMT_LEFT,200,-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}


void CFemSaveDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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
		//		//获得关联模型名称
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
			//获得关联模型名称
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
	// TODO: 在此添加控件通知处理程序代码
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
//	// TODO: 在此添加消息处理程序代码
//	UsrRepaintWindow();
//}

void CFemSaveDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此添加消息处理程序代码
	UsrRepaintWindow();
}
