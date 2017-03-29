// PartMainDlg.cpp : 实现文件
//零件目录管理以及零件建模的主窗口
//有零件目录树和属性、关系式、事物特性表、性能参数表、图片浏览等构成

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

//// CPartMainDlg 对话框
//extern int DlgIcon;
//void  IsMdlInfoDefAndDef(ProSolid mdl);
//extern CArray<UsrSWTXTable1,UsrSWTXTable1>  p_UsrSWTXTableList;  //定义全局的事物特性表列项，
////在程序运行时，要给他一个初始值(无效)
//extern CArray<UsrRelTable,UsrRelTable>    p_UsrRelTableList;   //定义全局的关系列表项数组
//extern CArray<UsrPicture,UsrPicture>   p_UsrPicture;           //图片的全局变量    
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


// CPartMainDlg 消息处理程序

/*******************************************************************************************\
函数：ShowPartMdlTree(void)
说明：显示树目录
\*******************************************************************************************/
void CPartMainDlg::ShowPartMdlTree(void)
{
	//添加根节点
    m_Tree.DeleteAllItems();
	HTREEITEM hItemRoot=m_Tree.InsertItem(_T("零件模型"));
	if(m_pSet.IsOpen()) m_pSet.Close();
	m_pSet.m_strFilter="";
	if(!m_pSet.Open(CRecordset::dynaset,_T("SELECT * FROM [dbo].[PartManageTable]"),CRecordset::readOnly))
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
函数：AddSubPartItem(HTREEITEM item)
说明：显示树的子项
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
	SetIcon (hIcon, TRUE);     // Set small icon  设置小图标
	// TODO:  在此添加额外的初始化
	if(ProMdlCurrentGet((ProMdl*)&part_solid)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("当前没有模型");
		OnClose();
		return FALSE;
	}
	//设置图片列表
	m_ImageList.Create(14, 14, ILC_COLOR32, 4, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	//设置属性页
	SetSheepPage();
	//显示目录树
	ShowPartMdlTree();
	return TRUE;                                     // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 树中的右键菜单设置                                                   */
/************************************************************************/
void CPartMainDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
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
		//新建类别ID_NEW_PARTTYPE 删除类别 ID_DEL_PARTTYPE
		//删除模型ID_DEL_PARTMDL  模型系列化 ID_L_12108
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
函数：OnNewParttype（）
说明：在树控件上点击右键后的新建类别,同时在服务器上创建目录
\***************************************************************************************/
void CPartMainDlg::OnNewParttype()
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
		directory="/零件模型";
		i=0;
	}
	else   //如果不是根目录
	{
		i=(long)m_Tree.GetItemData(hItem);
		if(m_pSet.IsOpen()) m_pSet.Close();
		sql_lang.Format("SELECT * FROM [dbo].[PartManageTable] WHERE [Number] = %d",i);
		m_pSet.Open(AFX_DB_USE_DEFAULT_TYPE,_T(sql_lang),CRecordset::none);
		if(m_pSet.IsBOF()) 
		{			
			dlg.SetFatherName("根目录","");
			i=0;
			directory="/零件模型";
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
			m_pSet.m_FatherNumber=i;                //父项 i
			m_pSet.m_Name=dlg.m_TypeName;           //类别名
			m_pSet.m_Type="typ";                    //类型为类别
			m_pSet.m_Note=dlg.m_TypeNote;           //备注
			m_pSet.m_MdlCode=dlg.m_Code;            //编码
			m_pSet.m_FileDirectory=directory;       //目录
			m_pSet.m_Datatime=CTime::GetCurrentTime();
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
/************************************************************************/
/* 右键菜单删除类别                                                     */
/************************************************************************/
void CPartMainDlg::OnDelParttype()
{
	// TODO: 在此添加命令处理程序代码
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
void CPartMainDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int window_id;
	ProWindowCurrentGet(&window_id);
	ProWindowActivate(window_id);
	CDialog::OnClose();
}

void CPartMainDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	// TODO: 在此添加消息处理程序代码
    UsrRepaintWindow();
}
/************************************************************************/
/* // 根据复选框的属性来设置相应需要显示的页面                          */
/************************************************************************/
void CPartMainDlg::SetSheepPage(void)
{	
    //如果单选项为指定单前模型
	SetSheetOfCurrentMdl();
}

/************************************************************************/
/* 点击完成建模之后的动作                                               */
/************************************************************************/
void CPartMainDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMdlOkDlg dlg;
	dlg.part_solid=part_solid;
	dlg.SetDbConnet(m_pSet.m_pDatabase);
	if (dlg.DoModal())
	{
		ShowPartMdlTree();
	}
	
}
/************************************************************************/
/* 当前模型模式下，显示当前模型的事物特性表，性能参数表，关系式，图片等信息*/
/************************************************************************/
int CPartMainDlg::SetSheetOfCurrentMdl(void)
{

	ProSolid solid;                  //调用当前模型的数据
	ProMdlCurrentGet((ProMdl*)&solid);	
    IsMdlInfoDefAndDef(solid);       //判断模型的基本信息是否已经定义，如果没有则定义
    m_page4.SetValue(solid,&p_UsrPartPorp);
	m_sheet.AddPage(&m_page4); 
	m_page1.SetValue(solid,&p_UsrSWTXTableList);
	m_sheet.AddPage(&m_page1);         //事物特性表	
	m_page2.SetValue(solid,&p_UsrPerfParaTableList);	
	m_sheet.AddPage(&m_page2);         //性能参数表	
	m_page3.SetValue(solid/*,&p_UsrRelTableList*/);		
	m_sheet.AddPage(&m_page3);               //关系式
	m_page5.SetValue(solid,&p_UsrPicture);		
	m_sheet.AddPage(&m_page5);                    //图片浏览页
	//用Create来创建一个属性页
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE|WS_GROUP, WS_EX_CONTROLPARENT);
	//调整属性页的大小和位置		
	m_sheet.SetWindowPos(NULL, 0, 0,0,0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	return 0;
}
/************************************************************************/
/* 在树形控件中，单击鼠标左键的动作                                     */
/************************************************************************/
void CPartMainDlg::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
}
/************************************************************************/
/* 选择类别                                                             */
/************************************************************************/
void CPartMainDlg::OnSelectTypeForMdl()
{
	// TODO: 在此添加命令处理程序代码
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
			//检查所选择的类别有没有被修改
			//sql_lang.Format("SELECT * FROM [dbo].[PartManageTable] WHERE [Number] = %d",i);
			m_pSet.m_strFilter.Format("[Number] = %d",i);
			m_pSet.Open();
			if(m_pSet.IsBOF()) 
			{
				m_pSet.Close();
				AfxMessageBox("当前类别已经不存在！");
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
			m_page4.m_CodeEdit.SetWindowText(m_pSet.m_MdlCode+".(请输入)");

			num=(int)p_UsrPartPorp.GetCount();
			for(int j=0;j<num;j++)
			{
				if(p_UsrPartPorp[j].owner!=solid) continue;
				if(p_UsrPartPorp[j].isDef!=1) continue;

				p_UsrPartPorp[j].FatherNumber=i;                           //m_pSet.m_Number;
				p_UsrPartPorp[j].Code=m_pSet.m_MdlCode+".(请输入)";        // 模型的代码临时为类别的代码
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
	// TODO: 在此添加命令处理程序代码
	ShowPartMdlTree();
}

void CPartMainDlg::OnDelPartmdl()
{
	// TODO: 在此添加命令处理程序代码
	long       indexNo=0;
	CString    sql_lang;
	CString    message;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem()) return;
    indexNo=(int)m_Tree.GetItemData(hItem);
	if(m_pSet.IsOpen()) m_pSet.Close();
	m_pSet.m_strFilter.Format("[Number]=%d",indexNo);	
	//测试数据库连接和服务器连接
	if(!m_pSet.Open())
	{
		AfxMessageBox("数据不能打开！");
		return;
	}
    if(!m_ftp.TestConnect(myFtp))
	{
		AfxMessageBox("服务器连接失败！");
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
	message.Format("确定要删除，模型[%s]及其系列吗？",_T(m_pSet.m_Name));
	if(AfxMessageBox(message,MB_OK)!=IDOK)
	{
		m_pSet.Close();
		return;
	}
    m_ftp.DeleteDirAndFile(_T(m_pSet.m_FileDirectory),myFtp);	
	try
	{
        m_pSet.Delete();            //删除其下所有相关信息，通过数据库的触发器进行
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
/* 右键菜单，模型系列化                                                 */
/************************************************************************/
void CPartMainDlg::OnL12108()
{
	// TODO: 在此添加命令处理程序代码
	CPartSeriesDlg dlg;
	long          Number=0;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem()) return;
    Number=(int)m_Tree.GetItemData(hItem);
	if(m_pSet.IsOpen()) m_pSet.Close();
	m_pSet.m_strFilter.Format("[Number]=%d",Number);
	//测试数据库连接和服务器连接
	if(!m_pSet.Open())
	{
		AfxMessageBox("数据不能打开！");
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
/* 查看模型的属性                                                       */
/************************************************************************/
void CPartMainDlg::OnMenuMdlInfo()
{
	// TODO: 在此添加命令处理程序代码
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
		dlg.strCaption="查看属性信息";
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
