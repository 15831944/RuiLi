// FemModelSave.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "FemModelSave.h"
#include "NewPartTypeDlg.h"


// CFemModelSave 对话框

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


// CFemModelSave 消息处理程序

BOOL CFemModelSave::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(ProMdlCurrentGet((ProMdl*)&part_solid)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("当前没有模型");
		//OnClose();
		//return FALSE;
	}
	//设置图片列表
	m_ImageList.Create(14, 14, ILC_COLOR32, 4, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	//显示目录树
	ShowPartMdlTree();
	return TRUE;                      
	// 异常：OCX 属性页应返回 FALSE
}

void CFemModelSave::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}




/*******************************************************************************************\
函数：ShowPartMdlTree(void)
说明：显示树目录
\*******************************************************************************************/
void CFemModelSave::ShowPartMdlTree(void)
{
	//添加根节点
	m_Tree.DeleteAllItems();
	HTREEITEM hItemRoot=m_Tree.InsertItem(_T("有限元模型"));
	if(m_pSet.IsOpen()) m_pSet.Close();
	m_pSet.m_strFilter="";
	if(!m_pSet.Open(CRecordset::dynaset,_T("SELECT * FROM [dbo].[FemPrt]"),CRecordset::readOnly))
	{
		AfxMessageBox("数据库连接不成功！");
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
函数：AddSubPartItem(HTREEITEM item)
说明：显示树的子项
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
/* 树中的右键菜单设置                                                   */
/************************************************************************/
void CFemModelSave::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此添加消息处理程序代码
	CPoint point=GetCurrentMessage()->pt;
	if(WindowFromPoint(point)==&m_Tree)
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(0);
		//根据图片类型，选择菜单是否有效
		HTREEITEM hItem=m_Tree.GetSelectedItem();
		TVITEM iTem;
		iTem.hItem=hItem;
		iTem.mask=TVIF_IMAGE;
		m_Tree.GetItem(&iTem);
		//新建类别ID_NEWCAT 删除类别 ID_DELCAT
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
函数：OnNewcat()
说明：在树控件上点击右键后的新建类别,同时在服务器上创建目录
\***************************************************************************************/
void CFemModelSave::OnNewcat()
{
	// TODO: 在此添加命令处理程序代码

	CNewPartTypeDlg dlg;
	long i=0;
	CString sql_lang;
	CString directory;
	HTREEITEM hItem=m_Tree.GetSelectedItem();

	//如果选中的是根目录
	if(hItem==m_Tree.GetRootItem())
	{
		dlg.SetFatherName("根目录","");
		directory="/有限元模型";
		i=0;
	}
	else   //如果不是根目录
	{
		i=(long)m_Tree.GetItemData(hItem);
		if(m_pSet.IsOpen()) m_pSet.Close();
		sql_lang.Format("SELECT * FROM [dbo].[FemPrt] WHERE [Number] = %d",i);
		m_pSet.Open(AFX_DB_USE_DEFAULT_TYPE,_T(sql_lang),CRecordset::none);
		if(m_pSet.IsBOF()) 
		{			
			dlg.SetFatherName("根目录","");
			i=0;
			directory="/有限元模型";
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
	//打开对话框

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
			directory=directory+"/"+dlg.m_TypeName;//确定需要在服务器上要建的目录
			m_pSet.m_strFilter.Empty();
			m_pSet.Open();
			m_pSet.AddNew();	  
			m_pSet.m_FatherNum=i;                //父项 i
			m_pSet.m_Name=dlg.m_TypeName;           //类别名
			m_pSet.m_Type="typ";                    //类型为类别
			m_pSet.m_Note=dlg.m_TypeNote;           //备注
		//	m_pSet.m_MdlCode=dlg.m_Code;            //编码
			m_pSet.m_FileDirectory=directory;       //目录
			m_pSet.m_DateTime=CTime::GetCurrentTime();
			m_pSet.Update();
			if(!m_ftp.CreateDirectory(myFtp,directory))
			{
				AfxMessageBox("创建类别目录不成功，请检查网络连接设置或父目录是否存在！\n或改目录已经存在！");
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
			AfxMessageBox("新建失败！");
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
	//首先判断有没有下级目录存在，如果有存在则不能删除
	m_pSet.m_strFilter.Format("[FatherNumber] = %d",i);	
	m_pSet.Open();
	if(!m_pSet.IsBOF())
	{
		AfxMessageBox("请先删除该类别下的模型或子类别！");
		m_pSet.Close();
		return;
	}
	m_pSet.Close();

	m_pSet.m_strFilter.Format("[Number] = %d",i);		
	m_pSet.Open();
	if(m_pSet.IsBOF()) 
	{
		AfxMessageBox("当前类别不存在！");
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
		AfxMessageBox("该类别正在被使用，不能删除！");
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
