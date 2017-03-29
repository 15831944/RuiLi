// PartMainDlg.cpp : 实现文件
//零件目录管理以及零件建模的主窗口
//有零件目录树和属性、关系式、事物特性表、性能参数表、图片浏览等构成

#include "stdafx.h"
#include "Main.h"
#include "PartMainDlg.h"
#include "PartMdlRecordSet.h"
#include "NPartInsInfoTable.h"
#include "NewPartTypeDlg.h"
#include "MdlOkDlg.h"
#include "partVariantTableSet.h"
#include "partPerfparaTableSet.h"
#include "partRelationSet.h"
#include "partRelativeDimParaTableSet.h"
#include "partPicSet.h"
//#include "PartSeriesDlg.h"
#include "AddMdlSeriesDlg.h"
#include "UsrSWTXTable.h"
#include "SignIn.h"
/************************************************************************/
/*  武守飞 为权限管理而添加程序                                         */

//用户信息结构体
CString  strtheright;





/************************************************************************/

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
	/*ON_COMMAND(ID_L_12108, OnL12108)*/
	ON_COMMAND(ID_Menu12107, OnMenuMdlInfo)
	ON_COMMAND(ID_UPDATE, OnUpdateMdl)
END_MESSAGE_MAP()


// CPartMainDlg 消息处理程序



BOOL CPartMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	
	USR_SET_DLG_ICON()
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
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));//需要修改成实例的图标
	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	//设置属性页
	SetSheepPage();
	//显示目录树
	if(m_pSet.Open())
	{
		ShowNPartTree(&m_Tree,m_pSet.m_pDatabase,true);
	}
	else
	{
		AfxMessageBox("数据库连接不成功！");
	}
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
		//如果当前零件不是模型，则不出现更新模型按钮

		//删除模型系列化按钮
		
		if (iTem.iImage!=1) {
			//如果选中的不是零件模型
			pPopup->EnableMenuItem(ID_DEL_PARTMDL,MF_DISABLED|MF_GRAYED);//删除模型
			//pPopup->EnableMenuItem(ID_L_12108,MF_DISABLED|MF_GRAYED);    //模型系列化
			pPopup->EnableMenuItem(ID_UPDATE,MF_DISABLED|MF_GRAYED );    //更新模型
			pPopup->EnableMenuItem(ID_Menu12107,MF_DISABLED|MF_GRAYED);
		}
		//如果选中的不是类别
		if (iTem.iImage!=0)
		{
			pPopup->EnableMenuItem(ID_NEW_PARTTYPE,MF_DISABLED|MF_GRAYED);//新建类别
			pPopup->EnableMenuItem(ID_DEL_PARTTYPE,MF_DISABLED|MF_GRAYED);//删除类别
			pPopup->EnableMenuItem(ID_L_,MF_DISABLED|MF_GRAYED);          //选择类别		
		}
		if (GetMdlInfo(part_solid).MdlType!=3 || GetMdlInfo(part_solid).Number!=(long)m_Tree.GetItemData(hItem))
		{
			pPopup->EnableMenuItem(ID_UPDATE,MF_DISABLED|MF_GRAYED);                   //移除更新模型
		}
		ASSERT(pPopup!=NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);

	}
}

/***************************************************************************************\
函数：OnNewParttype()
说明：在树控件上点击右键后的新建类别,只有在选中类别的情况下有效
\***************************************************************************************/
void CPartMainDlg::OnNewParttype()
{
	// TODO: 在此添加命令处理程序代码

	CNewPartTypeDlg dlg;
	long			i=0;
	long            selNum;
	CString         selName;
	CString         selCode;
	CString         newName;	
	long            newNum;
	CString            newCode;
	int				imageNum;
	int				selImageNum;
	HTREEITEM		hItem=m_Tree.GetSelectedItem();
	//判断选中的是否为类别的图片
	if (hItem==m_Tree.GetRootItem())
	{
		selNum  = 0;
		selName = "根目录";
		selCode = ""; 
	}
	else
	{
		m_Tree.GetItemImage(hItem,imageNum,selImageNum);
		if (imageNum!=0) return;
		m_pSet.m_strFilter.Format("[Number]=%ld",(long)m_Tree.GetItemData(hItem));
		m_pSet.Requery();
		if (!m_pSet.IsBOF())
		{
			m_pSet.MoveFirst();
			selName = m_pSet.m_Name;
			selNum  = m_pSet.m_Number;
			selCode = m_pSet.m_Code;
		}
		else
		{
			return;
		}
	}
	dlg.SetFatherName(selName,selCode);
	if (dlg.DoModal()==IDOK)
	{
		newName=dlg.m_TypeName;
		newCode=dlg.m_Code;
		//以后可能要添加类别的备注 
		//判断输入的类别是否存在
		try
		{
			m_pSet.m_strFilter.Format("[FatherNum]=%ld AND [Name]='%s'",selNum,newName.GetBuffer());
			m_pSet.Requery();
			if (m_pSet.IsBOF())
			{
				m_pSet.m_strFilter.Empty();
				m_pSet.Requery();
				m_pSet.AddNew();
				m_pSet.m_Code=newCode;
				m_pSet.m_Name=newName;
				m_pSet.m_FatherNum=selNum;
				m_pSet.Update();
				m_pSet.m_strFilter.Empty();
				m_pSet.Requery();
				m_pSet.MoveLast();
				newNum=m_pSet.m_Number;
				m_Tree.InsertItem(TVIF_IMAGE|TVIF_PARAM|TVIF_TEXT|TVIF_SELECTEDIMAGE,newName,0,0,0,0,newNum,hItem,NULL);
				m_Tree.Expand(hItem,TVE_EXPAND);
			}
			else
			{
				AfxMessageBox("在同一类别下已经有相同的组件类别存在");
			}
		}	
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			delete    e;
		}
	}
}
/************************************************************************/
/* 右键菜单删除类别                                                     */
/************************************************************************/
void CPartMainDlg::OnDelParttype()
{
	// TODO: 在此添加命令处理程序代码
	long                             i=0;
	CPartMdlRecordSet                m_set(m_pSet.m_pDatabase);
    HTREEITEM                        hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem())  return;
    i=(long)m_Tree.GetItemData(hItem);
    //检查是否有模型挂在这个目录下
	m_set.m_strFilter.Format("[TypeNum]=%ld",i);
	m_set.Open();
	if (!m_set.IsBOF())
	{
		AfxMessageBox("该类别下面有模型，不允许删除！");
		return;
	}
	m_pSet.m_strFilter.Format("[FatherNum]=%ld",i);
	m_pSet.Requery();
	if (!m_pSet.IsBOF())
	{
		AfxMessageBox("该类别下面有子类别，不允许删除！");
		return;
	}
    m_pSet.m_strFilter.Format("[Number]=%ld",i);
	m_pSet.Requery();
	if (!m_pSet.IsBOF())
	{
		try
		{
			m_pSet.MoveFirst();
			m_pSet.Delete();
			m_Tree.DeleteItem(hItem);
			m_pSet.Requery();
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			
		}
	}

}
void CPartMainDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UsrActivateCurrentWindow();
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
	//---------------wshf--------
	//if(curUserInfo.name=="")
	//{
	//	CSignIn m_dlg;
	//	if(m_dlg.DoModal()==IDOK)
	//	{
	//		getRights(curUserInfo.authority);
	//	}
	//	else
	//		return;
	//}
	//if(IsHasRight(strtheright))
	//{
		CMdlOkDlg dlg;
		dlg.part_solid=part_solid;
		dlg.SetDbConnet(m_pSet.m_pDatabase);
		if (dlg.DoModal())
		{
			ShowNPartTree(&m_Tree,m_pSet.m_pDatabase,true);
		}
	//}
	//else
	//	MessageBox("无权限！","提示信息");

}
/************************************************************************/
/* 当前模型模式下，显示当前模型的事物特性表，性能参数表，关系式，图片等信息*/
/************************************************************************/
int CPartMainDlg::SetSheetOfCurrentMdl(void)
{

	ProSolid          solid;                  //调用当前模型的数据
	ProMdlCurrentGet((ProMdl*)&solid);	
    //IsMdlInfoDefAndDef(solid);               //判断模型的基本信息是否已经定义，如果没有则定义
 /*   m_page4.SetValue(solid,&p_UsrPartPorp);*/
	m_sheet.AddPage(&m_page4); 
	m_page1.SetValue(solid,&p_UsrSWTXTableList);
	m_sheet.AddPage(&m_page1);               //事物特性表	
	m_page2.SetValue(solid,&p_UsrPerfParaTableList);	
	m_sheet.AddPage(&m_page2);               //性能参数表	
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
	long     i=0;
	int		 num=0;
	ProSolid solid;		
	ProMdlCurrentGet((ProMdl*)&solid);	

	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem())     return;
	i=(long)m_Tree.GetItemData(hItem);
	try
	{
		m_pSet.m_strFilter.Format("[Number] = %ld",i);
		m_pSet.Requery();
		if(m_pSet.IsBOF()) 
		{
			AfxMessageBox("当前类别已经不存在！");
			ShowNPartTree(&m_Tree,m_pSet.m_pDatabase);
			return;
		}
		m_pSet.MoveFirst();
        //用选中的类别设置模型
		m_page4.m_TypeEdit.SetWindowText(m_pSet.m_Name);
		m_page4.m_CodeEdit.SetWindowText(m_pSet.m_Code+".(请输入)");
        SetMdlInfo(solid)->TypeNum=m_pSet.m_Number;
		SetMdlInfo(solid)->TypeName=m_pSet.m_Name;

		//-------------------whsf---------------
		CString strtheName;
		HTREEITEM hParent,hSelected,hTest;
		hSelected=hItem;
		hTest=hItem;
		do {
			strtheName=m_Tree.GetItemText(hTest);
			if(strtheName=="通用件(TYJ)")
			{
				strtheright="通用件建模";
				return;
			}
			if(strtheName=="弹簧制动气室(ZDQS)")
			{
				strtheright="弹簧制动气室零件建模";
				return;
			}
			if(strtheName=="离合器助力器(ZLQ)")
			{
				strtheright="离合器助力器零件建模";
				return;
			}
			if(strtheName=="转向助力泵(ZDB)")
			{
				strtheright="转向助力泵零件建模";
				return;
			}
			if(strtheName=="制动总泵分泵(ZLB)")
			{
				strtheright="制动总泵分泵零件建模";
				return;
			}
			if(strtheName=="组合开关(ZHKG)")
			{
				strtheright="组合开关零件建模";
				return;
			}
			if(strtheName=="组合仪表(ZHYB)")
			{
				strtheright="组合仪表零件建模";
				return;
			}
			if(strtheName=="电缓速器(HSQ)")
			{
				strtheright="电缓速器零件建模";
				return;
			}
			hParent=m_Tree.GetParentItem(hTest);
			hTest=hParent;
		} while(m_Tree.GetParentItem(hTest));
	}
	catch (CDBException * e) {		
		AfxMessageBox(e->m_strError);	
		
	}	
	return;
}


void CPartMainDlg::OnRefreshTree()
{
	// TODO: 在此添加命令处理程序代码
	ShowNPartTree(&m_Tree,m_pSet.m_pDatabase,true);
}

void CPartMainDlg::OnDelPartmdl()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM							hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem())		return;
	if (UsrDelPartMdl(m_pSet.m_pDatabase,(long)m_Tree.GetItemData(hItem)))
	{
		m_Tree.DeleteItem(hItem);	
	}
	else
	{
		AfxMessageBox("删除不成功");
	}
}
///************************************************************************/
///* 右键菜单，模型系列化                                                 */
///************************************************************************/
//void CPartMainDlg::OnL12108()
//{
//	// TODO: 在此添加命令处理程序代码
//	//CPartSeriesDlg dlg;
//	//long          Number=0;
//	//HTREEITEM hItem=m_Tree.GetSelectedItem();
//	//if(hItem==m_Tree.GetRootItem()) return;
// //   Number=(int)m_Tree.GetItemData(hItem);
//	//if(m_pSet.IsOpen()) m_pSet.Close();
//	//m_pSet.m_strFilter.Format("[Number]=%d",Number);
//	////测试数据库连接和服务器连接
//	//if(!m_pSet.Open())
//	//{
//	//	AfxMessageBox("数据不能打开！");
//	//	return;
//	//}
//	//if(m_pSet.IsBOF()) 
//	//{
//	//	m_pSet.Close();
//	//	return;
//	//}
//	//m_pSet.MoveFirst();
//	//if(m_pSet.m_Type!="mdl") 
//	//{
//	//	m_pSet.Close();
//	//	return;
//	//}
//	//dlg.SetMdlNumberAndDB(Number,m_pSet.m_pDatabase);
//	//dlg.DoModal();
//
//}
/************************************************************************/
/* 查看模型的属性                                                       */
/************************************************************************/
void CPartMainDlg::OnMenuMdlInfo()
{
	// TODO: 在此添加命令处理程序代码
	CAddMdlSeriesDlg dlg;
	CPartMdlRecordSet m_Set(m_pSet.m_pDatabase);
	MdlInsInfo       mdlInfo;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if (m_Tree.GetRootItem()==hItem) return;
	long number=(long)m_Tree.GetItemData(hItem);
	try
	{		
		m_Set.m_strFilter.Format("[Number]=%d",number);
		m_Set.Open();
		if (m_Set.IsBOF()) {
			AfxMessageBox("模型不存在");
			return;
		}
		m_Set.MoveFirst();
        mdlInfo.Name=m_Set.m_Name;
		mdlInfo.Code=m_Set.m_Code;
		mdlInfo.Person=m_Set.m_Designer;
		mdlInfo.Note=m_Set.m_Note;
		mdlInfo.m_time=m_Set.m_Datatime;
		mdlInfo.mdlNum=number;
		mdlInfo.Dept=_T("");
		dlg.newMdl=mdlInfo;
		dlg.strCaption="查看属性信息";
		dlg.SetMdlNumberAndCode(-1,"",NULL);
		dlg.DoModal();
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		delete e;
	}
	return;
}
/************************************************************************/
/* 更新零件模型，当前模型必须是模型，即已经添加到模型库中               */
/************************************************************************/
void CPartMainDlg::OnUpdateMdl()
{
	// TODO: 在此添加命令处理程序代码
	CMdlOkDlg  updatamdl;
	if (m_Tree.GetItemData(m_Tree.GetSelectedItem()) != GetMdlInfo(part_solid).Number )
	{
		return;
	}
	if (updatamdl.UpdateMdl(part_solid,GetMdlInfo(part_solid).Number,m_pSet.m_pDatabase))
	{
		m_Tree.SetItemText(m_Tree.GetSelectedItem(),GetMdlInfo(part_solid).Name+GetMdlInfo(part_solid).Code);
	}
}

int CPartMainDlg::getRights(CString strAuthor)
{
	curUserRights.RemoveAll();
	if(strAuthor!="")
	{
		int nStart,nEnd,nLength,nCount;
		nLength=strAuthor.GetLength();
		nStart=0;
		nEnd=0;
		while(nEnd!=nLength-1)
		{
			//nEnd=str.Find(':',nStart);
			//if(nEnd!=-1)
			//{
			//	nCount=nEnd-nStart;
			//	userAttr.strStyle=str.Mid(nStart,nCount);
			//	nStart=nEnd+1;
			//}
			//else
			//	break;
			nEnd=strAuthor.Find('*',nStart);
			if(nEnd!=-1)
			{
				nCount=nEnd-nStart;
				useright.right=strAuthor.Mid(nStart,nCount);
				nStart=nEnd+1;
			}
			else
				break;
			curUserRights.Add(useright);
		}
	}
	return 0;
}

BOOL CPartMainDlg::IsHasRight(CString theRight)
{
	int nCount=0;
	nCount=curUserRights.GetCount();
	if(nCount==0)
		return FALSE;
	UserRight tempUserRight;
	for(int i=0;i<nCount;i++)
	{
		tempUserRight=curUserRights.GetAt(i);
		if(tempUserRight.right=="系统管理员")
			return TRUE;
	}
	for(int i=0;i<nCount;i++)
	{
		tempUserRight=curUserRights.GetAt(i);
		if(tempUserRight.right==theRight)
			return TRUE;
	}
	return FALSE;
}
