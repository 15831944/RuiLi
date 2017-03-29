// AsmMdlManDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "AsmMdlManDlg.h"
#include "NewPartTypeDlg.h"
#include "AsmMdlOkDlg.h"
#include "asmmanagetableset.h"
#include "checkmdlifhaschange.h"
#include "NProductInsInfoTable.h"
#include "SignIn.h"
// CAsmMdlManDlg 对话框

extern CString strtheright;

IMPLEMENT_DYNAMIC(CAsmMdlManDlg, CDialog)
CAsmMdlManDlg::CAsmMdlManDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmMdlManDlg::IDD, pParent)
{
}

CAsmMdlManDlg::~CAsmMdlManDlg()
{
}

void CAsmMdlManDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_ASM_MDL_TREE, m_Tree);
}


BEGIN_MESSAGE_MAP(CAsmMdlManDlg, CDialog)
	ON_COMMAND(ID_12008, OnNewType)
	ON_COMMAND(ID_Menu12136, OnSelType)
	ON_COMMAND(ID_12010, OnDelType)
	ON_COMMAND(ID_12011, OnDelMdl)
	ON_COMMAND(ID_12134, OnMdlSeries)
	ON_COMMAND(ID_12135, OnFresh)
	ON_COMMAND(ID_12012, OnViewProp)
	ON_MESSAGE(WM_LIST_OK,OnListDlgOk)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(1005, OnBnClicked1005)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_COMMAND(ID_UPDATE, OnUpdateMdl)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CAsmMdlManDlg 消息处理程序

void CAsmMdlManDlg::SetSheetPage(void)
{
	ProSolid solid;                  //调用当前模型的数据
	ProMdlCurrentGet((ProMdl*)&solid);	
	//IsMdlInfoDefAndDef(solid);       //判断模型的基本信息是否已经定义，如果没有则定义
	//m_page1_Info.SetValue(solid,&p_UsrPartPorp);
	m_sheet.AddPage(&m_page1_Info);
	m_page2_Tree.mdl=solid;
	m_sheet.AddPage(&m_page2_Tree);
	m_page3_Swtx.mdl=solid;
	m_sheet.AddPage(&m_page3_Swtx);
	m_page4_Perf.mdl=solid;
	m_sheet.AddPage(&m_page4_Perf);
	m_page5_Rel.mdl=solid;
	m_sheet.AddPage(&m_page5_Rel);
	m_page6_Pic.mdl=solid;
	m_sheet.AddPage(&m_page6_Pic);
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE|WS_GROUP, WS_EX_CONTROLPARENT);
	m_sheet.SetWindowPos(NULL, 0, 0,0,0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

}

BOOL CAsmMdlManDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	if(ProMdlCurrentGet((ProMdl*)&mdl)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("当前没有模型");
		OnClose();
		return FALSE;
	}

	m_ImageList.Create(14, 14, ILC_COLOR32, 4, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));


	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	if (m_pSet.Open())
	{
		ShowNAsmTree(&m_Tree,m_pSet.m_pDatabase,true);
	}
	else
	{
		AfxMessageBox("数据库连接不成功！");
	}
    SetSheetPage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 右键菜单——新建类别                                                 */
/************************************************************************/
void CAsmMdlManDlg::OnNewType()
{
	// TODO: 在此添加命令处理程序代码

	CNewPartTypeDlg dlg;
	long			i=0;
	long            selNum;
	CString         selName;
	CString         selCode;
	CString         newName;	
	long            newNum;
	CString         newCode;
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
		try
		{
			//判断输入的类别是否存在
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
/*  右键菜单——选择类别                                                */
/************************************************************************/
void CAsmMdlManDlg::OnSelType()
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
			ShowNAsmTree(&m_Tree,m_pSet.m_pDatabase);
			return;
		}
		m_pSet.MoveFirst();
		//用选中的类别设置模型
		m_page1_Info.m_MdlType=m_pSet.m_Name;
		m_page1_Info.m_MdlCode=m_pSet.m_Code+".(请输入)";
		m_page1_Info.UpdateData(FALSE);
		SetMdlInfo(solid)->TypeNum=m_pSet.m_Number;
		SetMdlInfo(solid)->TypeName=m_pSet.m_Name;
	}
	catch (CDBException * e) {		
		AfxMessageBox(e->m_strError);	
		
	}	


	//-------------------whsf---------------
	CString strtheName;
	HTREEITEM hParent,hSelected,hTest;
	hSelected=hItem;
	hTest=hItem;
	do {
		strtheName=m_Tree.GetItemText(hTest);
		if(strtheName=="弹簧制动气室(ZDQS)")
		{
			strtheright="弹簧制动气室产品建模";
			return;
		}
		if(strtheName=="离合器助力器(ZLQ)")
		{
			strtheright="离合器助力器产品建模";
			return;
		}
		if(strtheName=="转向助力泵(ZDB)")
		{
			strtheright="转向助力泵产品建模";
			return;
		}
		if(strtheName=="制动总泵分泵(ZLB)")
		{
			strtheright="制动总泵分泵产品建模";
			return;
		}
		if(strtheName=="组合开关(ZHKG)")
		{
			strtheright="组合开关产品建模";
			return;
		}
		if(strtheName=="组合仪表(ZHYB)")
		{
			strtheright="组合仪表产品建模";
			return;
		}
		if(strtheName=="电缓速器(HSQ)")
		{
			strtheright="电缓速器产品建模";
			return;
		}
		hParent=m_Tree.GetParentItem(hTest);
		hTest=hParent;
	} while(m_Tree.GetParentItem(hTest));







	return;
}
/************************************************************************/
/* 右键菜单——删除类别                                               */
/************************************************************************/
void CAsmMdlManDlg::OnDelType()
{
	// TODO: 在此添加命令处理程序代码
	long                             i=0;
	CAsmManageTableSet               m_set(m_pSet.m_pDatabase);
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
/************************************************************************/
/* 右键菜单——删除模型                                                 */
/************************************************************************/
void CAsmMdlManDlg::OnDelMdl()
{
	// TODO: 在此添加命令处理程序代码
	//检查
    HTREEITEM							hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem())		return;
	if (UsrDelAsmMdl(m_pSet.m_pDatabase,(long)m_Tree.GetItemData(hItem)))
	{
		m_Tree.DeleteItem(hItem);	
	}
	else
	{
		AfxMessageBox("删除不成功");
	}
}
/************************************************************************/
/* 右键菜单——模型系列化                                               */
/************************************************************************/
void CAsmMdlManDlg::OnMdlSeries()
{
	// TODO: 在此添加命令处理程序代码
}
/************************************************************************/
/*  右键菜单——刷新目录                                                */
/************************************************************************/
void CAsmMdlManDlg::OnFresh()
{
	// TODO: 在此添加命令处理程序代码
	ShowNAsmTree(&m_Tree,m_pSet.m_pDatabase,true);
}
/************************************************************************/
/*  右键菜单——查看属性                                                */
/************************************************************************/
void CAsmMdlManDlg::OnViewProp()
{
	// TODO: 在此添加命令处理程序代码
}

void CAsmMdlManDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此添加消息处理程序代码
	CPoint point=GetCurrentMessage()->pt;
	if(WindowFromPoint(point)==&m_Tree)
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(1);	
		//根据图片来判断所选择的项
		HTREEITEM hItem=m_Tree.GetSelectedItem();
		TVITEM iTem;
		iTem.hItem=hItem;
		iTem.mask=TVIF_IMAGE;
		m_Tree.GetItem(&iTem);
		if (iTem.iImage!=2)
		{
			//如果选中的不是模型
			pPopup->EnableMenuItem(ID_12011,MF_DISABLED|MF_GRAYED);//删除模型
			pPopup->EnableMenuItem(ID_UPDATE,MF_DISABLED|MF_GRAYED);//更新模型
		}
		if (iTem.iImage!=0)
		{
			//如果选中的不是类别
			pPopup->EnableMenuItem(ID_12008,MF_DISABLED|MF_GRAYED);//新建类别
			pPopup->EnableMenuItem(ID_Menu12136,MF_DISABLED|MF_GRAYED);//删除类别
			pPopup->EnableMenuItem(ID_12010,MF_DISABLED|MF_GRAYED);          //选择类别
		}
		if (GetMdlInfo(mdl).MdlType!=3 || GetMdlInfo(mdl).Number!=(long)m_Tree.GetItemData(hItem))
		{
			pPopup->EnableMenuItem(ID_UPDATE,MF_DISABLED|MF_GRAYED);   
		}
		ASSERT(pPopup!=NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	}
}
/************************************************************************/
/* 生成模型按钮                                                         */
/************************************************************************/
void CAsmMdlManDlg::OnBnClicked1005()
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
		CAsmMdlOkDlg	dlg;
		ProSolid		solid;                  //调用当前模型的数据
		ProMdlCurrentGet((ProMdl*)&solid);	
		dlg.asm_solid=solid;
		if(!m_pSet.IsOpen()) m_pSet.Open();
		dlg.m_pSet.m_pDatabase=m_pSet.m_pDatabase;
		if (dlg.DoModal()==IDOK)
			ShowNAsmTree(&m_Tree,m_pSet.m_pDatabase,true);	
	//}
	//else
	//	MessageBox("无权限！","提示信息");

















	
}
/************************************************************************/
/* 把装配事物特性表项转成模型的事务特性表项                             */
/************************************************************************/
BOOL CAsmMdlManDlg::GetSWTEItem(void)
{
	return 0;
}
/************************************************************************/
/* 模型树页中选择模型变形范围的消息处理函数                             */
/************************************************************************/
LRESULT CAsmMdlManDlg::OnListDlgOk(WPARAM wParam,LPARAM lParam)
{
	//m_page2_Tree.OnListDlgOk(wParam,lParam);
	return 1;
}
/************************************************************************/
/* 检查零部件是否有改变                                                 */
/************************************************************************/
void CAsmMdlManDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CArray<ProSolid ,ProSolid>   solidlist;	
	UsrPartPorp					 temp_porp;
	if (!m_pSet.IsOpen())
	{
		if (m_pSet.Open())
		{
			AfxMessageBox("数据库连接失败");
			return;
		}
	}
	GetAsmCompHand(mdl,&solidlist);
	CCheckMdlIfHasChange m_check;
	for (int i=0;i<solidlist.GetCount();i++)
	{
		temp_porp=GetMdlInfo(solidlist[i]);
		//如果是零件而且有模型
		if (temp_porp.MdlType==1 && GetMdlType(solidlist[i])==PRO_MDL_PART)
		{
			if (m_check.CheckPartHasPrtMdl(m_pSet.m_pDatabase,solidlist[i],temp_porp.Number,0/*检查是否有改变*/)!=CHECK_TRUE)
			{
			}
		}
	}
}
/************************************************************************/
/* 更新模型                                                             */
/************************************************************************/
void CAsmMdlManDlg::OnUpdateMdl()
{
	// TODO: 在此添加命令处理程序代码
	CAsmMdlOkDlg updatamdl;
	if (m_Tree.GetItemData(m_Tree.GetSelectedItem())!= GetMdlInfo(mdl).Number)
	{
		return;
	}
	if (updatamdl.UpdateMdl(mdl,GetMdlInfo(mdl).Number,m_pSet.m_pDatabase))
	{
		m_Tree.SetItemText(m_Tree.GetSelectedItem(),GetMdlInfo(mdl).Name+GetMdlInfo(mdl).Code);
	}
}

void CAsmMdlManDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此添加消息处理程序代码
	UsrRepaintWindow();
}

int CAsmMdlManDlg::getRights(CString strAuthor)
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

BOOL CAsmMdlManDlg::IsHasRight(CString theRight)
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
