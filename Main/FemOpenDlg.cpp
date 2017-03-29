// FemOpenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "FemOpenDlg.h"
#include "PartMdlRecordSet.h"
#include "FtpFile.h"
#include "PartMdlOpen.h"
void ShowFemMdlSubTree(CTreeCtrl & m_Tree,HTREEITEM hItem,CDatabase * m_db)
{
	CFemPrt  m_set(m_db);
	HTREEITEM  tempItem;
	int        nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	m_set.m_strFilter.Format("[FatherNum]=%ld",(long)m_Tree.GetItemData(hItem));
	m_set.Open();
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			if (m_set.m_Type=="typ")
			{
				tempItem = m_Tree.InsertItem(nMarsk,_T(m_set.m_Name), 
					0, 0, 0, 0, m_set.m_Number, hItem, NULL);
				ShowFemMdlSubTree(m_Tree,tempItem,m_db);
			}
			//int ImageNum=m_set.m_Type=="mdl"?0:1;
			//tempItem = m_Tree.InsertItem(nMarsk,_T(m_set.m_Name), 
			//	ImageNum, ImageNum, 0, 0, m_set.m_Number, hItem, NULL);
			//ShowFemMdlSubTree(m_Tree,tempItem,m_db);
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
}
BOOL ShowFemMdlTree(CTreeCtrl & m_Tree,CDatabase * m_db)
{
	CFemPrt  m_set(m_db);
	m_Tree.DeleteAllItems();
	HTREEITEM hItemRoot;
	hItemRoot=m_Tree.InsertItem(_T("有限元模型"));
	m_set.m_strFilter=_T("[FatherNum]=0");
	if (!m_set.Open())
	{
		AfxMessageBox("数据库连接不成功");
		return FALSE;
	}
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			HTREEITEM hItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE
				,_T(m_set.m_Name),0, 0, 0, 0, m_set.m_Number, hItemRoot, NULL);
			ShowFemMdlSubTree(m_Tree,hItem,m_db);
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
	m_Tree.Expand(hItemRoot,TVE_EXPAND);
	return TRUE;
}


// CFemOpenDlg 对话框

IMPLEMENT_DYNAMIC(CFemOpenDlg, CDialog)
CFemOpenDlg::CFemOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFemOpenDlg::IDD, pParent)
{
}

CFemOpenDlg::~CFemOpenDlg()
{
}

void CFemOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CFemOpenDlg, CDialog)
//	ON_NOTIFY(NM_CLICK, IDC_TREE1, OnNMClickTree1)
ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
ON_BN_CLICKED(IDOK, OnBnClickedOk)
ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
//ON_WM_MOVING()
ON_WM_MOVE()
END_MESSAGE_MAP()


// CFemOpenDlg 消息处理程序
BOOL CFemOpenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
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
	return TRUE;
}
//void CFemOpenDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}

void CFemOpenDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int  ImageNum=0,ImageSelNum=0;
	CPartMdlRecordSet  m_tempset(m_pSet.m_pDatabase);
	m_tempset.Open();
	HTREEITEM hItem=m_Tree.GetSelectedItem();
    m_Tree.GetItemImage(hItem,ImageNum,ImageSelNum);
	m_List.DeleteAllItems();
    if (hItem!=m_Tree.GetRootItem() && ImageNum==0)
    {
		m_pSet.m_strFilter.Format("FatherNum=%ld",(long)m_Tree.GetItemData(hItem));
		m_pSet.Requery();
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
	*pResult = 0;
}
/************************************************************************/
/* 打开选中模型                                                         */
/************************************************************************/
void CFemOpenDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码	 
	CFtpFile  m_ftp;
	CString   strFileDirectory;
	CString   strNewDirectory;
	CString   fullFileName;
	CString   FileName;
	ProPath   name;
	ProSolid  part_solid;
	ProType	  type;
	ProName	  w_name;
	int       w_id;
	ProError  status;
	CPartMdlOpen  part_mdl_open;
	POSITION  pos = m_List.GetFirstSelectedItemPosition();
	int		  nItem = m_List.GetNextSelectedItem(pos);
	m_pSet.m_strFilter.Format("Number=%ld",m_List.GetItemData(nItem));
	m_pSet.Requery();
	if (!m_pSet.IsBOF())
	{
		m_pSet.MoveFirst();
		SelectAnDirectory(strFileDirectory,"");
		CreateDirectoryGivenPathAndName(strFileDirectory,m_pSet.m_Name,strNewDirectory);
		fullFileName=strFileDirectory+strNewDirectory;
		if (!m_ftp.DownloadFiletoDirectory(m_pSet.m_FileDirectory,fullFileName,myFtp))
		{
			AfxMessageBox("模型文件下载失败");
			return;
		}
		
		//分割出文件名称
		FileName=m_pSet.m_FileDirectory;
		FileName=FileName.Right(FileName.GetLength()-1);
		do {
			int temp=FileName.Find('/',0);
		    if (temp==-1)
		    {
				break;
		    }
			FileName=FileName.Right(FileName.GetLength()-1);
		} while(1);

        //打开
		fullFileName=fullFileName+"\\"+FileName;
		ProStringToWstring(name,fullFileName.GetBuffer());
		status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&part_solid); 
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("打开模型失败");
			return;
		}	
		status = ProMdlTypeGet(part_solid, (ProMdlType*)&type);
		status=  ProMdlNameGet(part_solid, w_name);
		status=  ProObjectwindowCreate(w_name, type, &w_id);
		if(!status== PRO_TK_NO_ERROR)
		{
			return;
		}
		part_mdl_open.AddPartMdlInfoForOtherUse(m_pSet.m_pDatabase,part_solid,m_pSet.m_ConnPrtMdlNum);
		status = ProMdlDisplay(part_solid);
		status = ProWindowActivate(w_id);
		OnCancel();
	}
	else
	{
		AfxMessageBox("没有选中模型");
		return;
	}	
}

void CFemOpenDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

//void CFemOpenDlg::OnMoving(UINT fwSide, LPRECT pRect)
//{
//	CDialog::OnMoving(fwSide, pRect);
//
//	// TODO: 在此添加消息处理程序代码
//	UsrRepaintWindow();
//}

void CFemOpenDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此添加消息处理程序代码
	UsrRepaintWindow();
}
