// AsmAddComplg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "AsmAddComplg.h"
//#include "PartInsTableSet.h"
#include "NPartInsInfoTable.h"
#include "partInsValueTableSet.h"
#include "partInsPerfValueTableSet.h"
#include "AsmManageTableSet.h"
#include "FtpFile.h"
#include "partPerfparaTableSet.h"
#include "partVariantTableSet.h"
#include "partRelationSet.h"
#include "partDrawingSet.h"
#include "UsrSWTXTable.h"
#include "partPicSet.h"
#include "CAsmPicSet.h"
#include "AsmInsInfoTableSet.h"
#include "AsmInsVariantTableSet.h"
#include "AsmPerformanceInsTableSet.h"
#include "CAsmVarSet.h"
#include "CAsmPerfSet.h"
#include "NPartTypeSet.h"
#include "NPartInsInfoTable.h"
#include "NProductInsInfoTable.h"
#include "NProductInsPerfTable.h"
#include "NProductInsVariantTable.h"
#include "AsmAssembly.h"
// CAsmAddComplg 对话框

IMPLEMENT_DYNAMIC(CAsmAddComplg, CDialog)
CAsmAddComplg::CAsmAddComplg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmAddComplg::IDD, pParent)
	, m_selTab(0)
	, mdlSelectNumber(-1)
{
}

CAsmAddComplg::~CAsmAddComplg()
{
}

void CAsmAddComplg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PART_TREE, m_Tree[0]);
	DDX_Control(pDX, IDC_ASM_TREE, m_Tree[1]);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_PATH_STATIC, m_Path);
	DDX_Control(pDX, IDC_LIST4, m_List2);
	DDX_Control(pDX, IDC_BUTTON2, m_btnOpen);
	DDX_Control(pDX, IDC_RECT_PIC, m_RectPic);
	DDX_Control(pDX, IDC_PARTINS_TREE, m_Tree[2]);
	DDX_Control(pDX, IDC_ASMINS_TREE, m_Tree[3]);
}


BEGIN_MESSAGE_MAP(CAsmAddComplg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
//	ON_NOTIFY(NM_CLICK, IDC_PART_TREE, OnNMClickPartTree)
ON_NOTIFY(TVN_SELCHANGED, IDC_PART_TREE, OnTvnSelchangedPartTree)
ON_NOTIFY(TVN_SELCHANGED, IDC_ASM_TREE, OnTvnSelchangedAsmTree)
ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
//ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
ON_WM_MOVING()
ON_WM_PAINT()
ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
ON_COMMAND(ID_12192, OnAsmBom)
ON_COMMAND(ID_12193, OnAsmPrtList)
ON_WM_CONTEXTMENU()
ON_NOTIFY(TVN_SELCHANGED, IDC_PARTINS_TREE, OnTvnSelchangedPartinsTree)
ON_NOTIFY(TVN_SELCHANGED, IDC_ASMINS_TREE, OnTvnSelchangedAsminsTree)
END_MESSAGE_MAP()


// CAsmAddComplg 消息处理程序

BOOL CAsmAddComplg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	/*RECT		rect;*/
	TCITEM		tcItem;
	RECT		rect1;
	//1.添加Tab页3个，零件模型、产品/组件模型、零件
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("零件模型");
	m_Tab.InsertItem(0, &tcItem);
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("产品/组件模型");
	m_Tab.InsertItem(1, &tcItem);
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("零件");
	m_Tab.InsertItem(2, &tcItem);
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("部件");
	m_Tab.InsertItem(3, &tcItem);

	//2.设置图标
	m_ImageList.Create(14, 14, ILC_COLOR32, 4, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));//需要修改成实例的图标
	m_Tree[0].SetImageList(&m_ImageList,TVSIL_NORMAL);
	m_Tree[1].SetImageList(&m_ImageList,TVSIL_NORMAL);
    m_Tree[2].SetImageList(&m_ImageList,TVSIL_NORMAL);
	m_Tree[3].SetImageList(&m_ImageList,TVSIL_NORMAL);

	//////////////////////////////////////////////////////////////////////////
	if (!m_pSet.Open())
	{
		AfxMessageBox("数据库连接不成功");
		SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	//3.读入数据
    //显示零件模型的类别树，可以根据模型来查找零件
    ShowNPartTree(&m_Tree[0],m_pSet.m_pDatabase,true);
	//显示组件模型的类别树
    ShowNAsmTree(&m_Tree[1],m_pSet.m_pDatabase,true);
	//零件，直接根据分类来查找零件
	ShowPartTypeTree(&m_Tree[2],m_pSet.m_pDatabase);
	//显示组件模型的类别树
	ShowNAsmTree(&m_Tree[3],m_pSet.m_pDatabase,false);


	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"名称",LVCFMT_LEFT,80,-1);
	m_List.InsertColumn(1,"编码",LVCFMT_LEFT,80,-1);
	m_List.InsertColumn(2,"完成状态",LVCFMT_LEFT,80,-1);  //是否完成  "完成" "未完成"
	m_List.InsertColumn(3,"审核状态",LVCFMT_LEFT,80,-1);  //          "审核通过"  "审核未通过"
	m_List.InsertColumn(4,"PDM状态",LVCFMT_LEFT,80,-1);   //          "提交"   "未提交"
	m_List.InsertColumn(6,"存放位置",LVCFMT_LEFT,80,-1); 
	m_List.InsertColumn(7,"设计人员",LVCFMT_LEFT,80,-1); 
	m_List.InsertColumn(8,"备注",LVCFMT_LEFT,200,-1);

    //4.设置图片显示控件的大小和位置	
	m_RectPic.GetWindowRect(&rect1); 
	ScreenToClient(&rect1);
	pic_dlg.Create(IDD_PIC_FORM_DB,this);
	pic_dlg.MoveWindow(&rect1,TRUE);
	pic_dlg.ShowWindow(SW_SHOW);

	//5.初始设置打开装配按钮为不可用，只有当点击了零件之后才能使用这个按钮	
    m_btnOpen.EnableWindow(FALSE);
    
	//6.设置控件m_List2
	m_List2.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List2.InsertColumn(0,"参数名",LVCFMT_LEFT,100,-1);
	m_List2.InsertColumn(1,"参数值",LVCFMT_LEFT,100,-1);
   
	//7.设置树控件的显示状态
	m_Tree[0].ShowWindow(SW_SHOW);
	m_Tree[1].ShowWindow(SW_HIDE);
	m_Tree[2].ShowWindow(SW_HIDE);
	m_Tree[3].ShowWindow(SW_HIDE);
	m_selTab =0;  

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* TAB键切换                                                            */
/************************************************************************/
void CAsmAddComplg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i<4;i++)
	{
		if(m_Tab.GetCurSel()==i)
		{
			m_Tree[i].ShowWindow(SW_SHOW);
			m_selTab=i;
			continue;
		}
		m_Tree[i].ShowWindow(SW_HIDE);
	}
	*pResult = 0;
}
/************************************************************************/
/* 点击零件模型树项的动作 
   1.如果点中的  是类别，则在列表里面显示该类别下的模型
   2.如果点中的  是模型，则在列表里面显示模型的实例                     */
/************************************************************************/
void CAsmAddComplg::OnTvnSelchangedPartTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW			pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CNPartInsInfoTable		m_tempSet(m_pSet.m_pDatabase);
	CpartVariantTableSet	tempVaiantSet(m_pSet.m_pDatabase);
	CpartPerfparaTableSet	tempPerfSet(m_pSet.m_pDatabase);
	long					num=(long)pNMTreeView->itemNew.lParam;
	int						itemImageIndex;
	int						itemSelImage;
	/*CString				    csIndexNo;*/
	CString					csText="";
	//1。获得选中的项的图标，根据点击项的图片来进行判断
	m_Tree[0].GetItemImage(pNMTreeView->itemNew.hItem,itemImageIndex,itemSelImage);

	//2。删除列表,设置装配按钮不可用
	m_List.DeleteAllItems();
	m_List2.DeleteAllItems();
	m_btnOpen.EnableWindow(FALSE);

	//3。设置路径的文本
	HTREEITEM		hItem=pNMTreeView->itemNew.hItem;
	while (hItem!=m_Tree[0].GetRootItem())
	{
		CString temp=m_Tree[0].GetItemText(hItem);
		csText		=temp+"\\"+csText;
		hItem		=m_Tree[0].GetParentItem(hItem);	   
	}
	csText="\\"+csText;
	m_Path.SetWindowText(csText);
	//如果选中的根则退出
	if(pNMTreeView->itemNew.hItem==m_Tree[0].GetRootItem()) return;
		

	//4。根据不同情况进行
	switch(itemImageIndex) {
	case USR_ICON_TYPE:
		//选中了类别不反应
		break;
	case USR_ICON_PRTMDL:
		//选中了零件模型，则在列表框里面显示模型实例
		//4。1在第二个列表框显示，模型的可变参数以及性能参数的信息
		tempVaiantSet.m_strFilter.Format("[PrtNum]=%ld",num);
		tempVaiantSet.Open();
		if (!tempVaiantSet.IsBOF())
		{
						tempVaiantSet.MoveFirst();
						do {
							m_List2.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List2.GetItemCount(),tempVaiantSet.m_Name,0,0,0,tempVaiantSet.m_Number);
							m_List2.SetItemText(m_List2.GetItemCount()-1,1,tempVaiantSet.m_Value);
							tempVaiantSet.MoveNext();
						} while(!tempVaiantSet.IsEOF());
		}
		tempPerfSet.m_strFilter.Format("[PrtNum]=%ld",num);
		tempPerfSet.Open();
		if (!tempPerfSet.IsBOF())
		{
						m_List2.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List2.GetItemCount(),"性能参数",0,0,0,-1);
						tempPerfSet.MoveFirst();
						do {
							m_List2.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List2.GetItemCount(),tempPerfSet.m_Name,0,0,0,tempPerfSet.m_Number);
							m_List2.SetItemText(m_List2.GetItemCount()-1,1,tempPerfSet.m_Value);
							tempPerfSet.MoveNext();
						} while(!tempPerfSet.IsEOF());
		}

		
		//4.2 获得模型的实例
        m_tempSet.m_strFilter.Format("[MdlNum]=%ld",num);
		m_tempSet.Open();
		if (!m_tempSet.IsBOF())
		{
			m_btnOpen.EnableWindow(TRUE);
			m_tempSet.MoveFirst();
			do {
				m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List.GetItemCount(),m_tempSet.m_Name,0,0,0,m_tempSet.m_PrtNum);
				m_List.SetItemText(m_List.GetItemCount()-1,1,m_tempSet.m_InsCode);
				m_List.SetItemText(m_List.GetItemCount()-1,2,(m_tempSet.m_FinishState==_T("完成")?_T("完成"):_T("未完成")));
				m_List.SetItemText(m_List.GetItemCount()-1,3,(m_tempSet.m_AuditState==_T("审核通过")?_T("审核通过"):_T("审核未通过")));
				m_List.SetItemText(m_List.GetItemCount()-1,4,(m_tempSet.m_PDMState==_T("提交")?_T("提交"):_T("未提交")));
				m_List.SetItemText(m_List.GetItemCount()-1,5,(m_tempSet.m_PDMState==_T("提交")?"PDM系统":(m_tempSet.m_LocalState=="服务器"?"设计系统":"用户电脑")));
				m_List.SetItemText(m_List.GetItemCount()-1,6,m_tempSet.m_Designer);
				m_List.SetItemText(m_List.GetItemCount()-1,7,m_tempSet.m_Note);
				m_tempSet.MoveNext();
			} while(!m_tempSet.IsEOF());
		}

		//4。3设置当前选择的模型
		mdlSelectNumber=num;

		//4。4获得模型的图片
		pic_dlg.GetMdlPicSet(0,num,m_pSet.m_pDatabase);
		break;
	default:
		break;
	} 
	*pResult = 0;
}
/************************************************************************/
/* 点击组件模型树的时候，如果选中的是组件模型的图标，则在列表中显示它的实例
 以及它的可变尺寸参数、图片的浏览										*/
/************************************************************************/
void CAsmAddComplg::OnTvnSelchangedAsmTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CAsmManageTableSet				m_Set(m_pSet.m_pDatabase);
	CNProductInsInfoTable			m_Set1(m_pSet.m_pDatabase);
	CAsmVarSet						m_Set2(m_pSet.m_pDatabase);
	CAsmPerfSet						m_Set3(m_pSet.m_pDatabase);
	long							num=(long)pNMTreeView->itemNew.lParam;
	int								itemImageIndex;
	int								itemSelImage;
	CString							csText;
	//1.获得选中的项的图标，根据点击项的图片来进行判断
	m_Tree[1].GetItemImage(pNMTreeView->itemNew.hItem,itemImageIndex,itemSelImage);

	//2。删除列表,设置装配按钮不可用
	m_List.DeleteAllItems();
	m_List2.DeleteAllItems();
	m_btnOpen.EnableWindow(FALSE);

	//3。设置路径的文本
	HTREEITEM   hItem=pNMTreeView->itemNew.hItem;
	while (hItem!=m_Tree[1].GetRootItem())
	{
		CString temp=m_Tree[1].GetItemText(hItem);
		csText=temp+"\\"+csText;
		hItem=m_Tree[1].GetParentItem(hItem);	   
	}
	csText="\\"+csText;
	m_Path.SetWindowText(csText);

	//如果选中的根则退出
	if(pNMTreeView->itemNew.hItem==m_Tree[1].GetRootItem())
		return;
	switch(itemImageIndex) {
	case USR_ICON_TYPE:
		break;
	case USR_ICON_ASMMDL:
		//如果选中了组件模型，则在列表框里面显示模型的实例
		//4.1在第二个列表框显示，模型的可变参数以及性能参数的信息
		m_Set2.m_strFilter.Format("[AsmNum]=%ld",num);
		m_Set2.Open();
		if (!m_Set2.IsBOF())
		{
			m_Set2.MoveFirst();
			do {
				m_List2.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List2.GetItemCount(),m_Set2.m_Name,0,0,0,m_Set2.m_Number);
				m_List2.SetItemText(m_List2.GetItemCount()-1,1,m_Set2.m_Value);
				m_Set2.MoveNext();
			} while(!m_Set2.IsEOF());
		}

		m_Set3.m_strFilter.Format("[AsmNum]=%ld",num);
		m_Set3.Open();
		if (!m_Set3.IsBOF())
		{
			m_List2.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List2.GetItemCount(),"性能参数",0,0,0,-1);
			m_Set3.MoveFirst();
			do {
				m_List2.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List2.GetItemCount(),m_Set3.m_Name,0,0,0,m_Set3.m_Number);
				m_List2.SetItemText(m_List2.GetItemCount()-1,1,m_Set3.m_Value);
				m_Set3.MoveNext();
			} while(!m_Set3.IsEOF());
		}

		//4.2获得模型的实例
		m_Set1.m_strFilter.Format("[MdlNum]=%ld",num);
		m_Set1.Open();		
		if (!m_Set1.IsBOF())
		{
			m_btnOpen.EnableWindow(TRUE);
			m_Set1.MoveFirst();
			while (!m_Set1.IsEOF())
			{
				m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List.GetItemCount(),m_Set1.m_Name,0,0,0,m_Set1.m_PdtNum);
				m_List.SetItemText(m_List.GetItemCount()-1,1,m_Set1.m_InsCode);
				m_List.SetItemText(m_List.GetItemCount()-1,2,(m_Set1.m_FinishState==_T("完成")?_T("完成"):_T("未完成")));
				m_List.SetItemText(m_List.GetItemCount()-1,3,(m_Set1.m_AuditState==_T("审核通过")?_T("审核通过"):_T("审核未通过")));
				m_List.SetItemText(m_List.GetItemCount()-1,4,(m_Set1.m_PDMState==_T("提交")?_T("提交"):_T("未提交")));
				m_List.SetItemText(m_List.GetItemCount()-1,5,(m_Set1.m_PDMState==_T("提交")?"PDM系统":(m_Set1.m_LocalState=="服务器"?"设计系统":"用户电脑")));
				m_List.SetItemText(m_List.GetItemCount()-1,6,m_Set1.m_Designer);
				m_List.SetItemText(m_List.GetItemCount()-1,7,m_Set1.m_Note);
				m_Set1.MoveNext();
			}
		}
		//4.3设置当前选择的模型
		mdlSelectNumber=num;

		//4.4
		pic_dlg.GetMdlPicSet(1,num,m_pSet.m_pDatabase);
		break;
	default:
		break;
	}
	*pResult = 0;
}
/************************************************************************/
/* 点击【装配】模型按钮                                                 */
/************************************************************************/
void CAsmAddComplg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_selTab) {
	case 0:    //选择的是零件模型的实例
		AssemblePrtMdl();
		break;
	case 1:
		AssembleAsmMdl();
		break;
	case 2:
		AssemblePrtMdl();
		break;
	case 3:
		AssembleAsmMdl();
	default:
		break;
	}

}
/************************************************************************/
/*//装配零件模型                                                        */
/************************************************************************/
void  CAsmAddComplg::AssemblePrtMdl()
{
	ProSolid			    assmbly,comp;
	UsrPartPorp             temp_porp;
	CNPartInsInfoTable		m_tempSet(m_pSet.m_pDatabase);
	POSITION				pos = m_List.GetFirstSelectedItemPosition();
	int						nItem = m_List.GetNextSelectedItem(pos);
	//1.当前已经有相同的零件装在组件中
	//2.当前已经有相同的零件装在另一个组件中
	//3.装配进来的零件与现有的零件同名
	//4.当前的零件没有在内存中出现过
	//综合，因为零件的文件名都是唯一的，只要考虑当前是否有相同的文件名就可以了
	//对于，对于同一个组件内相同的零件，其变型规则可以变化，即同一个零件，虽然出现对出，但
	//他的变型规则可能是不一样，所以对变型规则的设置，可以然用户进行选择
	//如果选择作为具有相同的变型规则的零件，

    if(nItem<0)				return;
					
	ProMdlCurrentGet((ProMdl*)&assmbly);
	m_tempSet.m_strFilter.Format("[PrtNum]=%ld",(long)m_List.GetItemData(nItem));
	m_tempSet.Open();
	if (!m_tempSet.IsBOF())
	{
		//如果当前已经存在这个文件
		//且都有相同的号,则可以直接装配，否则跳出提醒
		m_MdlDrawItem.RemoveAll();
		if (IsMdlInSession(m_tempSet.m_SolidFileName,PRO_MDL_PART,(ProMdl*)&comp))
		{
            temp_porp=GetMdlInfo(comp);
			CString		csTemp;
			//如果文件名相同，但是零件号不同，则进行覆盖
			if (temp_porp.Number!=m_tempSet.m_PrtNum)
			{
				csTemp.Format("系统中已经有相同的文件名(%s)存在\n",GetMdlFileName(comp));
				csTemp.Format("%s是否继续打开(可能会使装配失败)!",csTemp);
				if (AfxMessageBox(csTemp,MB_YESNO|MB_ICONQUESTION)==IDYES)
				{
					if (AssembleComp(assmbly,comp))
					{
						AddPartMdlInfo(comp,(long)m_List.GetItemData(nItem),1,NULL);
						OnClose();
					}
				}		
			}
			else         //如果文件名相同且零件号相同，则说明两者是属于同一个零件
			{
				if (AssembleComp(assmbly,comp))
				{
					OnClose();
				}
			}
		}		                //如果当前不存在这个,则打开之后在装配，如果装配成功，则添加信息
		else if(AssembleComp(assmbly,(long)m_List.GetItemData(nItem),PRO_MDL_PART))
		{
			OnClose();
		}
	}
}
/************************************************************************/
/*//装配组件模型                                                        */
/************************************************************************/
void  CAsmAddComplg::AssembleAsmMdl()
{
	ProSolid			    assmbly;
	ProSolid                comp;
	UsrPartPorp             temp_porp;
	CAsmAssembly            product_open;
	CNProductInsInfoTable	m_tempSet(m_pSet.m_pDatabase);	
	POSITION				pos = m_List.GetFirstSelectedItemPosition();
	int						nItem = m_List.GetNextSelectedItem(pos);	

	if(nItem<0)				return;
				
	ProMdlCurrentGet((ProMdl*)&assmbly);
	m_tempSet.m_strFilter.Format("[PdtNum]=%ld",(long)m_List.GetItemData(nItem));
	m_tempSet.Open();
	if (!m_tempSet.IsBOF())
	{
		//如果当前已经存在这个文件
		//且都有相同的号,则可以直接装配，否则跳出提醒
		m_MdlDrawItem.RemoveAll();
		if (IsMdlInSession(m_tempSet.m_SolidFileName ,PRO_MDL_ASSEMBLY,(ProMdl*)&comp))
		{
					temp_porp=GetMdlInfo(comp);
					CString   csTemp;
					//如果文件名相同，但是零件号不同，则进行覆盖
					if (temp_porp.Number!=m_tempSet.m_PdtNum)
					{
						csTemp.Format("系统中已经有相同的文件名(%s)存在",GetMdlFileName(comp));
						csTemp.Format("%s是否继续打开(可能会使装配失败)!",csTemp);
						if (AfxMessageBox(csTemp,MB_YESNO|MB_ICONQUESTION)==IDYES)
						{
							if (product_open.OpenProduct(m_pSet.m_pDatabase,(long)m_List.GetItemData(nItem),&comp))
							{
								if (AssembleComp(assmbly,comp))
								{
									product_open.AddInfo();
									OnClose();
								}
								else
								{
									product_open.CancelAssembly();
								}
							}
						}		
					}
					else   //如果文件名相同且零件号相同，则说明两者是属于同一个零件
					{
						if(assmbly!=comp)
						{
							if (AssembleComp(assmbly,comp))
							{
								OnClose();
							}
						}
						else
						{
							AfxMessageBox("不能把当前模型里面装配当前模型");
						}
						
					}
		}
		//如果当前不存在这个,则打开之后在装配，如果装配成功，则添加信息
		else
		{
					if (product_open.OpenProduct(m_pSet.m_pDatabase,(long)m_List.GetItemData(nItem),&comp))
					{
						if (AssembleComp(assmbly,comp))
						{
							product_open.AddInfo();
							OnClose();
						}
						else
						{
							product_open.CancelAssembly();
						}
					}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//跟列表2相关的函数
void CAsmAddComplg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CpartInsValueTableSet   	partInsSet(m_pSet.m_pDatabase);
	CpartInsPerfValueTableSet	partInsPerfSet(m_pSet.m_pDatabase);
	CNProductInsVariantTable	asmInsVaSet(m_pSet.m_pDatabase);
	CNProductInsPerfTable		asmInsPerfSet(m_pSet.m_pDatabase);
	int							sel = m_Tab.GetCurSel();
	int							listIndex=0;
	POSITION					pos = m_List.GetFirstSelectedItemPosition();
	int							nItem = m_List.GetNextSelectedItem(pos);

	if(nItem==-1)				return;

	long						lSelInsNum=(long)m_List.GetItemData(nItem);

	if(m_List2.GetItemCount()<=0) return;
	switch(sel)
	{
	case 0:	
		partInsSet.Open();
		while ((long)m_List2.GetItemData(listIndex)!=-1 && listIndex<m_List2.GetItemCount())
		{
			partInsSet.m_strFilter.Format("[PrtNum]=%ld AND [Num]=%ld"
				,lSelInsNum,(long)m_List2.GetItemData(listIndex));
			partInsSet.Requery();
			if (!partInsSet.IsBOF())
			{
				m_List2.SetItemText(listIndex,1,partInsSet.m_Value);
			}
			listIndex++;
			if(listIndex>=m_List2.GetItemCount()) break;
		}
		if (listIndex>=m_List2.GetItemCount()-1) break;
		listIndex++;
		partInsPerfSet.Open();
		while (listIndex<m_List2.GetItemCount())
		{
			partInsPerfSet.m_strFilter.Format("[PrtNum]=%ld AND [Num]=%ld"
				,lSelInsNum,(long)m_List2.GetItemData(listIndex));
			partInsPerfSet.Requery();
			if (!partInsSet.IsBOF())
			{
				m_List2.SetItemText(listIndex,1,partInsPerfSet.m_Value);
			}
			listIndex++;
		}
		break;
	case 1:
		asmInsVaSet.Open();
		while ((long)m_List2.GetItemData(listIndex)!=-1 && listIndex<m_List2.GetItemCount())
		{
			asmInsVaSet.m_strFilter.Format("[PdtNum]=%ld AND [Num]=%ld"
				,lSelInsNum,(long)m_List2.GetItemData(listIndex));
			asmInsVaSet.Requery();
			if (!asmInsVaSet.IsBOF())
			{
				m_List2.SetItemText(listIndex,1,asmInsVaSet.m_Value);
			}
			listIndex++;
			if(listIndex>=m_List2.GetItemCount()) break;
		}
		if (listIndex>=m_List2.GetItemCount()-1) break;
		listIndex++;
		asmInsPerfSet.Open();
		while (listIndex<m_List2.GetItemCount())
		{
			asmInsPerfSet.m_strFilter.Format("[PdtNum]=%ld AND [Num]=%ld"
				,lSelInsNum,(long)m_List2.GetItemData(listIndex));
			asmInsPerfSet.Requery();
			if (!asmInsPerfSet.IsBOF())
			{
				m_List2.SetItemText(listIndex,1,asmInsPerfSet.m_Value);
			}
			listIndex++;
		}
		break;
	case 2:
		break;
	}
	*pResult = 0;
}
void CAsmAddComplg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialog::OnMoving(fwSide, pRect);

	// TODO: 在此添加消息处理程序代码
}
/************************************************************************/
/* 重绘                                                                 */
/************************************************************************/
void CAsmAddComplg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

/************************************************************************/
/* 删除模型及其相关的工程图                                              */
/************************************************************************/
BOOL CAsmAddComplg::DeleteSolidAndDrawing(ProSolid solid,bool bolDeleteDirectory=false)
{
	ProError						status;
	CString							dir;
	CArray<ProDrawing,ProDrawing>  p_DrawingList;
	GetSolidDrawing(solid,&p_DrawingList);

	for (int i=0;i<(int)p_DrawingList.GetCount();i++) 
	    ProMdlErase(p_DrawingList[i]);
	//ProMdlErase(solid);		
	if (bolDeleteDirectory)
	{
		dir=GetMdlPath(solid);
		RemoveDireAndFile(dir);
	}
	status=ProMdlErase((ProMdl)solid);
	return TRUE;
}
/************************************************************************/
/* 得到组件和元件的句柄来进行装配，这个过程将调用Proe的装配对话框       */
/************************************************************************/
BOOL CAsmAddComplg::AssembleComp(ProSolid solid,ProSolid comp)
{
	ProMdlType      type;
	ProMatrix		init_pos= {{ 1.0, 0.0, 0.0, 0.0 }, 
	{0.0, 1.0, 0.0, 0.0}, 
	{0.0, 0.0, 1.0, 0.0}, 
	{0.0, 0.0, 0.0, 1.0}};
	ProAsmcomp		 feature; 
	ProError		 status;
	int             feat_ids[1];
	ProBoolean      p_comp_is_underconstrained;
	status=ProMdlTypeGet((ProMdl)solid,&type);
	if (type!=PRO_MDL_ASSEMBLY)
	{
		AfxMessageBox("当前模型不是组件!");
		return FALSE;
	}   
    //关闭对话框
	ShowWindow(SW_HIDE);
	//1.装配
	status=ProAsmcompAssemble(solid,comp,init_pos,&feature);
	status=ProAsmcompIsUnderconstrained(&feature,&p_comp_is_underconstrained);
	if (p_comp_is_underconstrained==PRO_B_TRUE)
	{
		status=ProAsmcompConstrRemove(&feature,-1);
	}	
	status=ProAsmcompConstrRedefUI(&feature);
	//取消装配和装配不成功
	if(status==PRO_TK_USER_ABORT || status==PRO_TK_BAD_INPUTS)
	{
		feat_ids[0]=feature.id;
		ProFeatureDelete(solid,feat_ids,1,NULL,0);
		ShowWindow(SW_SHOW);
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/*//从服务器＆本地下载来装配(零件)                                      */
/************************************************************************/
BOOL CAsmAddComplg::AssembleComp(ProSolid solid,long InsNum,ProMdlType type)
{
	CStringArray			files;//服务器上的文件位置
	//CStringArray            localFiles;//本地服务器上的文件（模型）
	//CStringArray            localDrawingFiles;//本地的工程图
	CFtpFile                m_ftp;
	CNPartInsInfoTable		m_tempSet(m_pSet.m_pDatabase);	
	CNPartTypeSet           m_parttypeset(m_pSet.m_pDatabase);
	CString                 localDir="";	
	CString                 newDir="";
	CString					csError="";
	ProError				status;
	ProPath					name;
	mdl_draw_item           tempMdlDrawItem;
	tempMdlDrawItem.draw=NULL;
	tempMdlDrawItem.solid=NULL;
	tempMdlDrawItem.DrawFullName=tempMdlDrawItem.SolidFullName="";

	if (type==PRO_MDL_PART)
	{
		m_tempSet.m_strFilter.Format("[PrtNum]=%ld",InsNum);
		m_tempSet.Open();
		if (!m_tempSet.IsBOF())
		{
			//1。找到制定的文件，为模型的打开作准备
			//
			localDir=GetMdlPath((ProMdl)solid);
			if (!SelectAnDirectory(localDir,localDir))
			{
				return FALSE;;
			}			
			//1。1建立需要下载的文件夹（本地）
			if (!CreateDirectoryGivenPathAndName(localDir,m_tempSet.m_Name,newDir))
			{
				return FALSE;
			}
			//1。2查找文件
			if (m_tempSet.m_PDMState==_T("提交"))
			{
				//根据PDM来获取模型和工程图的地址
				CString  fileTemp1="",fileTemp2="";
				UsrGetPartAndProductFromPdm(m_tempSet.m_PDMNum,0,fileTemp1,fileTemp2);
				if(!m_tempSet.m_SolidFileName.IsEmpty()) 
				{					
					files.Add(fileTemp1);
					tempMdlDrawItem.SolidFullName=localDir+newDir+"\\"+m_tempSet.m_SolidFileName;
				}
				if(!m_tempSet.m_DrawingName.IsEmpty())
				{
					files.Add(fileTemp2);
					tempMdlDrawItem.DrawFullName=localDir+newDir+"\\"+m_tempSet.m_DrawingName;
				}	
				m_ftp.DownloadFilestoDirectory(&files,localDir+newDir,myFtp,csError);
				if (!csError.IsEmpty())
				{
					AfxMessageBox(csError);
					return FALSE;
				}
			}
			else if (m_tempSet.m_LocalState==_T("服务器"))
			{
				//根据服务器来获取模型＆工程图的地址,通过下载，
				//首先要有一个查找过程，看是否有找到制定图纸
				if(!m_tempSet.m_SolidFileName.IsEmpty()) 
				{					
					files.Add(m_tempSet.m_SolidPlace+"/"+m_tempSet.m_SolidFileName+".1");
					tempMdlDrawItem.SolidFullName=localDir+newDir+"\\"+m_tempSet.m_SolidFileName;
				}
				if(!m_tempSet.m_DrawingName.IsEmpty())
				{
					files.Add(m_tempSet.m_DrawingPlace+"/"+m_tempSet.m_DrawingName+".1");
					tempMdlDrawItem.DrawFullName=localDir+newDir+"\\"+m_tempSet.m_DrawingName;
				}	
				m_ftp.DownloadFilestoDirectory(&files,localDir+newDir,myFtp,csError);
				if (!csError.IsEmpty())
				{
					AfxMessageBox(csError);
					return FALSE;
				}
			}
			else if (m_tempSet.m_LocalState==_T("本地"))
			{
				//根据本地，来判断是否为本机，方法是判断制定的目录是否有这个文件，不需要下载
				if(!m_tempSet.m_SolidFileName.IsEmpty()) 
				{					
					tempMdlDrawItem.SolidFullName=m_tempSet.m_SolidPlace+"\\"+m_tempSet.m_SolidFileName;
					files.Add(tempMdlDrawItem.SolidFullName);
				}					
				if(!m_tempSet.m_DrawingName.IsEmpty())   
				{
					tempMdlDrawItem.DrawFullName=m_tempSet.m_DrawingPlace+"\\"+m_tempSet.m_DrawingName;
					files.Add(tempMdlDrawItem.DrawFullName);
				}			
				
				if (!IsFilesExists(files,csError))
				{
					AfxMessageBox(csError);
					return FALSE;
				}
			}
			else 
			{
				AfxMessageBox("文件存放位置不确定，无法为你打开!");
				return FALSE;
			}
			//2。打开文件
			if(tempMdlDrawItem.SolidFullName.IsEmpty()) return FALSE;
			ProStringToWstring(name,tempMdlDrawItem.SolidFullName.GetBuffer());
			status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&tempMdlDrawItem.solid); 
			if(status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
			//3。装配
			if (!AssembleComp(solid,(ProSolid)tempMdlDrawItem.solid))
			{
				return FALSE;
			}
			//4.打开工程图
			if(!tempMdlDrawItem.DrawFullName.IsEmpty())
			{
				ProStringToWstring(name,tempMdlDrawItem.DrawFullName.GetBuffer());
				status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&tempMdlDrawItem.draw); 
				if(status!=PRO_TK_NO_ERROR) 
				{
					AfxMessageBox("导入模型的工程图失败!");
					tempMdlDrawItem.draw=NULL;
				}
			}
			//5.添加模型信息
			AddPartMdlInfo(tempMdlDrawItem.solid,InsNum,0,&tempMdlDrawItem);
			return TRUE;
		}
	}
	if (type==PRO_MDL_ASSEMBLY)
	{
	}
	return TRUE;
}
BOOL CAsmAddComplg::AddPartMdlInfo(ProMdl mdl,long PrtNum,int type=0/*是否已有*/,mdl_draw_item * tempMdlDrawItem=NULL)
{
	CNPartInsInfoTable		m_tempSet(m_pSet.m_pDatabase);	
	CNPartTypeSet           m_parttypeset(m_pSet.m_pDatabase);
	UsrPartPorp             temp_porp;
	m_tempSet.m_strFilter.Format("[PrtNum]=%ld",PrtNum);
	m_tempSet.Open();
	if(!m_tempSet.IsBOF())
	{
		m_tempSet.MoveFirst();
		temp_porp.owner  = mdl;
		temp_porp.Number = PrtNum;
		//判断这个零件是否来自模型
		switch(m_tempSet.m_MdlNum) {
			case -1:
				temp_porp.MdlType=2;           //无模型的零件或组件
				temp_porp.MdlNum =-1;
				break;
			default:
				temp_porp.MdlType=1;           //有模型的实例
				temp_porp.MdlNum =m_tempSet.m_MdlNum;
				break;
		}			
		temp_porp.Name   =m_tempSet.m_Name;
		temp_porp.Code   =m_tempSet.m_InsCode;
		//获得模型的类别以及类别名称
		m_parttypeset.m_strFilter.Format("[Number]=%ld",m_tempSet.m_TypeNum);
		m_parttypeset.Open();
		if (!m_parttypeset.IsBOF())
		{
			m_parttypeset.MoveFirst();
			temp_porp.TypeNum=m_tempSet.m_TypeNum;
			temp_porp.TypeName=m_parttypeset.m_Name;
		}
		else
		{
			temp_porp.TypeNum=-1;
			temp_porp.TypeName="";
		}
		temp_porp.Material=m_tempSet.m_Material;
		temp_porp.Person  ="";
		temp_porp.Note    =m_tempSet.m_Note;
		////判断是否全新添加
		if (tempMdlDrawItem!=NULL)
		{
			if (tempMdlDrawItem->draw!=NULL)
			{
				temp_porp.bHaveDraw =true;
				temp_porp.hDraw     =tempMdlDrawItem->draw;
			}
			else
			{
				temp_porp.hDraw=NULL;
				temp_porp.bHaveDraw=false;
			}
		}
		temp_porp.IsAudited=(m_tempSet.m_AuditState==_T("审核通过")?true:false);
		//文件的来源
		if (m_tempSet.m_PDMState==_T("提交"))
			temp_porp.ResourceType=2;
		else if(m_tempSet.m_LocalState==_T("本地"))
			temp_porp.ResourceType=0;
		else if(m_tempSet.m_LocalState==_T("服务器"))
			temp_porp.ResourceType=1;
		//装配件的元件
		temp_porp.IsAsmComp=true;
		//获得模型的名称
		if (m_tempSet.m_MdlNum>0)
		{
			m_pSet.m_strFilter.Format("[Number]=%ld",m_tempSet.m_MdlNum);
			m_pSet.Requery();
			if (!m_pSet.IsBOF())
			{
				m_pSet.MoveFirst();
				temp_porp.MdlName=m_pSet.m_Name;
			}
		}
		else temp_porp.MdlName="无模型";
		//把模型的信息添加到全局变量中
		SetMdlInfo((ProSolid)mdl,temp_porp);

		//添加其他信息 如果是有模型的实例，把模型的信息添加到实例中，
		//注意，这个函数必须在设置完模型的基本信息之后调用
		//因为，在内部要修改模型基本信息
		if (temp_porp.MdlType==1)
		{
			UsrAddPartMdlOtherInfo(m_pSet.m_pDatabase,mdl,temp_porp.MdlNum);
		}
	}
	return TRUE;
}
/************************************************************************/
/* 组成关系表                                                           */
/************************************************************************/
void CAsmAddComplg::OnAsmBom()
{
	// TODO: 在此添加命令处理程序代码
}
/************************************************************************/
/* 零件清单                                                             */
/************************************************************************/
void CAsmAddComplg::OnAsmPrtList()
{
	// TODO: 在此添加命令处理程序代码
}
/************************************************************************/
/* 邮件菜单                                                             */
/************************************************************************/
void CAsmAddComplg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此添加消息处理程序代码
	//组件模式下
	if (m_Tab.GetCurSel()==1 && WindowFromPoint(point)==&m_Tree[1])
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(8);
		ASSERT(pPopup!=NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	}
}

/************************************************************************/
/* 用与把导入的模型根据不同的实例进行改名 
命名规则：   模型名_实例号.扩展名       (工程图也要更改)             */
/************************************************************************/
BOOL CAsmAddComplg::CompRenameWithInsNum(long InsNum, ProMdl mdl)
{
	//ProMdlType      type;
	//ProName         oldName;
	//CString         newName;
	//ProError        status;
	//CString         error;
	//   CArray<ProDrawing,ProDrawing>  p_DrawingList;
	//ProMdlTypeGet(mdl,&type);
	//ProMdlNameGet(mdl,oldName);
	//newName.Format("%s_%d",CString(oldName),InsNum);
	//ProStringToWstring(oldName,newName.GetBuffer());
	//status=ProMdlRename((ProMdl)mdl,oldName);
	////存在同名
	//if(status!=PRO_TK_NO_ERROR)
	//{
	//	error.Format("内存列表中存在“%s%s”的模型！\n该文件与选择的模型文件同名，请检查！"
	//		,CString(oldName),type==PRO_MDL_ASSEMBLY?".asm":".prt");
	//	AfxMessageBox(error);
	//	return FALSE;
	//}

	//p_UsrPartPorp[p_UsrPartPorp.GetCount()-1].FileName.Format("%s.%s",newName,type==PRO_MDL_ASSEMBLY?".asm":".prt");

	////获得模型的工程图
	//GetSolidDrawing((ProSolid)mdl,&p_DrawingList);
	////更改工程图的名称
	//for (int i=0;i<(int)p_DrawingList.GetCount();i++)
	//{
	//	ProMdlNameGet((ProMdl)p_DrawingList[i],oldName);
	//	newName.Format("%s_%d",CString(oldName),InsNum);
	//	ProStringToWstring(oldName,newName.GetBuffer());
	//	ProMdlRename((ProMdl)p_DrawingList[i],oldName);
	//}

	//ProSolidRegenerate((ProSolid)mdl,0);
	return TRUE;
}
/************************************************************************/
/* 取消装配，或装配失败之后，删除元件                                   */
/************************************************************************/
BOOL CAsmAddComplg::CancelAssmeblyOrFailure(ProSolid assmbly, ProAsmcomp * feature,long InsNum)
{
	//   int                            feat_ids[1];
	//ProSolid                       solid;
	//CArray<ProDrawing,ProDrawing>  p_DrawingList;
	//feat_ids[0]=feature->id;
	//ProAsmcompMdlGet(feature,(ProMdl*)&solid);	
	//ProFeatureDelete(assmbly,feat_ids,1,NULL,0);
	////删除模型，分为两种情况：一、加入的新变形；二、装配件中本身就有的（不需要清楚模型句柄及其工程图,Flag1=1）
	//int Flag=0;
	////判断先前是否已经有装配过，如果是，则不从内存中清楚
	//for (int j=0;j<p_AsmCompItemList.GetCount();j++)
	//{
	//	if (p_AsmCompItemList[j].mdlNum==mdlSelectNumber && p_AsmCompItemList[j].insNum==InsNum 
	//		&& p_AsmCompItemList[j].owner == assmbly)   
	//		//注意：如果删除了元件，但没有元件还在内存着，则这里没有删除，在建模完之后要考虑
	//	{
	//		Flag=1;
	//		return TRUE;
	//	}
	//}
	//if(Flag==0) 
	//{
	//	GetSolidDrawing(solid,&p_DrawingList);
	//	for (int i=0;i<(int)p_DrawingList.GetCount();i++) {ProMdlErase(p_DrawingList[i]);}
	//	ProMdlErase(solid);
	//}
	return TRUE;
}
/************************************************************************/
/* 把模型导入内存(零件)                                                 */
/************************************************************************/
ProMdl CAsmAddComplg::MdlRetrieve(int type, long mdlNum)
{
	//ProPath                 name;
	//ProMdl                  p_handle;
	//ProMdldata              data;
	//ProError                status;
	//CString                 currentDit;
	//CFtpFile                m_ftp;
	//CString                 fullFileName;//包括路径的文件名
	//   CString                 new_name;
	//CpartDrawingSet         m_Set(m_pSet.m_pDatabase);
	//ProDrawing              p_dwing;
	//ProSolid                assmbly;
	//ProName                 tempCheckMdlName;
	//CString                 cstempCheckMdlName;
	//ProMdl *                mdllist;
	//int                     p_count;
	//status=ProMdlCurrentGet((ProMdl*)&assmbly);
	//status=ProMdlDataGet(assmbly,&data);
	//currentDit=CString(data.device)+":"+CString(data.path);
	//new_name=_T("VdTemp_");
	////下载并打开模型
	//if (mdlNum!=-1) {
	//		if (m_pSet.IsOpen()) 
	//			m_pSet.Close();

	//		m_pSet.m_strFilter.Format("[Number]=%ld",mdlNum);
	//		m_pSet.Open();

	//		if (m_pSet.IsBOF()) 
	//			{AfxMessageBox("模型已经不存在，不能打开！");m_pSet.Close();return NULL;}
	//		m_pSet.MoveFirst();

	//		//1.判断当前内存中是否有跟模型同名的文件已经打开
	//		status=ProSessionMdlList((type==0?PRO_MDL_PART:PRO_MDL_ASSEMBLY),&mdllist,&p_count);
	//		for (int i=0;i<p_count;i++)
	//		{
	//			ProMdlNameGet(mdllist[i],tempCheckMdlName);
	//			cstempCheckMdlName.Format("%s%s",CString(tempCheckMdlName),type==0?".prt":".asm");
	//			if(cstempCheckMdlName.MakeLower()==m_pSet.m_FileName.MakeLower())
	//			{
	//				if(AfxMessageBox("已经打开一个与选定模型文件同名的模型,\n是否继续装配？",MB_YESNO)==IDYES)
	//				{
	//					return (ProSolid)mdllist[i];
	//				}
	//				ProArrayFree((ProArray*)&mdllist);
	//				return NULL;
	//			}
	//		}

	//		ProArrayFree((ProArray*)&mdllist);
	//		//2.检查当前是否有已经存在相同的文件夹
	//		for (int i=0;;i++)
	//		{
	//			CString version;				
	//			version.Format("%s%s(%d)",new_name,m_pSet.m_Name,i);
	//			version=currentDit+version;
	//			if (CreateDirectoryA(version,NULL))
	//			{
	//				currentDit=version;
	//				break;
	//			}
	//			if(i>100) 
	//			{
	//				AfxMessageBox("模型下载过程中，无法在本地创建临时目录");
	//				return NULL;
	//			}
	//		}

	//		//3.从服务器得到基模型的文件
	//		if (!m_ftp.DownloadDirectorytoDirectory(m_pSet.m_FileDirectory,currentDit,myFtp))
	//		{
	//			AfxMessageBox("从服务器读取文件过程中不成功，请检查网络");
	//			m_pSet.Close();
	//			return NULL;
	//		}
	//		fullFileName=currentDit+"\\"+m_pSet.m_FileName;
	//		ProStringToWstring(name,fullFileName.GetBuffer());
	//		status=ProMdlLoad(name,PRO_MDL_UNUSED ,PRO_B_FALSE,&p_handle);
	//		if (status!=PRO_TK_NO_ERROR)
	//		{
	//			AfxMessageBox("没能打开下载到本地的文件！请检查版本！");
	//			m_pSet.Close();
	//			return NULL;
	//		}
	//		//4.导入模型所有的工程图
	//		m_Set.m_strFilter.Format("[PrtNum]=%ld",mdlNum);
	//		m_Set.Open();
	//		if(!m_Set.IsBOF())
	//		{
	//			m_Set.MoveFirst();
	//			do {
	//				fullFileName=currentDit+"\\drawing\\"+m_Set.m_FileName;
	//				ProStringToWstring(name,fullFileName.GetBuffer());
	//				status=ProMdlLoad(name,PRO_MDL_UNUSED ,PRO_B_FALSE,(ProMdl*)&p_dwing);
	//				if (status!=PRO_TK_NO_ERROR)
	//				{
	//					AfxMessageBox("导入模型的工程图失败");
	//					return p_handle;
	//				}
	//				m_Set.MoveNext();
	//			} while(!m_Set.IsEOF());
	//		}
	//		m_pSet.Close();
	//}
	return NULL/*p_handle*/;
}

/************************************************************************/
/* 装配元件                                                             */
/************************************************************************/
BOOL CAsmAddComplg::AssembleComp(ProSolid solid/*实例的句柄*/,long InsNum/*实例号*/)
{
	ProSolid		assmbly;
	ProMdlType      type;
	ProMatrix		init_pos= {{ 1.0, 0.0, 0.0, 0.0 }, 
	{0.0, 1.0, 0.0, 0.0}, 
	{0.0, 0.0, 1.0, 0.0}, 
	{0.0, 0.0, 0.0, 1.0}};
	ProAsmcomp		feature; 
	ProError		status;
	status=ProMdlCurrentGet((ProMdl*)&assmbly);
	status=ProMdlTypeGet((ProMdl)assmbly,&type);
	if (type==PRO_MDL_ASSEMBLY)
	{
		//1.装配
		status=ProAsmcompAssemble(assmbly,solid,init_pos,&feature);
		status=ProAsmcompConstrRemove(&feature,-1);
		status=ProAsmcompConstrRedefUI(&feature);
		//取消装配和装配不成功
		if(status==PRO_TK_USER_ABORT || status==PRO_TK_BAD_INPUTS)
		{
			CancelAssmeblyOrFailure(assmbly,&feature,InsNum);
			return FALSE;
		}
		//2.输入信息，如果模型已经存在，则不需要填写
		int Flag=0;
		for (int j=0;j<p_AsmCompItemList.GetCount();j++)
		{
			if (p_AsmCompItemList[j].mdlNum==mdlSelectNumber && p_AsmCompItemList[j].insNum==InsNum 
				&& p_AsmCompItemList[j].owner == assmbly) 
			{
				Flag=1;
				return TRUE;
			}
		}
		//2.2装配成功后，把模型的信息读入内存(新的模型)
		if (Flag==0)
		{	//2.2.1		
			//AddPartMdlInfo((ProMdl)solid,InsNum);
			//2.2.2根据模型的缺省系列好，对模型文件进行重命名
			if(!CompRenameWithInsNum(InsNum,(ProMdl)solid)) 
			{
				CancelAssmeblyOrFailure(assmbly,&feature,InsNum);
				return FALSE;
			}
			//2.2.3把当前选择的模型的实例（系列）信息，作为缺省值
			AsmCompVarItem	temp_item;
			CompInsItem		item;
			temp_item.owner=assmbly;
			temp_item.comp=solid;
			temp_item.mdlNum=mdlSelectNumber;
			temp_item.type=ASM_MDL_PART;
			temp_item.insNum=InsNum;
			temp_item.varFeild=new CArray<CompInsItem,CompInsItem>();
			if(GetMdlInsInfo(m_pSet.m_pDatabase,ASM_MDL_PART,mdlSelectNumber,InsNum,&item)) 
				temp_item.varFeild->Add(item);
			temp_item.propNum=(int)p_UsrPartPorp.GetCount()-1;
			p_AsmCompItemList.Add(temp_item);
		}
		return TRUE;
	}
	return FALSE;
}
void CAsmAddComplg::OnTvnSelchangedPartinsTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	long							num=(long)pNMTreeView->itemNew.lParam;
	CNPartInsInfoTable				m_Set1(m_pSet.m_pDatabase);
	m_Set1.m_strFilter.Format("[TypeNum]=%ld",num);
	m_Set1.Open();	
	m_List.DeleteAllItems();
	m_List2.DeleteAllItems();

	m_btnOpen.EnableWindow(FALSE);
	if (!m_Set1.IsBOF())
	{
		m_btnOpen.EnableWindow(TRUE);
		m_Set1.MoveFirst();
		while (!m_Set1.IsEOF())
		{
			m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List.GetItemCount(),m_Set1.m_Name,0,0,0,m_Set1.m_PrtNum);
			m_List.SetItemText(m_List.GetItemCount()-1,1,m_Set1.m_InsCode);
			m_List.SetItemText(m_List.GetItemCount()-1,2,(m_Set1.m_FinishState==_T("完成")?_T("完成"):_T("未完成")));
			m_List.SetItemText(m_List.GetItemCount()-1,3,(m_Set1.m_AuditState==_T("审核通过")?_T("审核通过"):_T("审核未通过")));
			m_List.SetItemText(m_List.GetItemCount()-1,4,(m_Set1.m_PDMState==_T("提交")?_T("提交"):_T("未提交")));
			m_List.SetItemText(m_List.GetItemCount()-1,5,(m_Set1.m_PDMState==_T("提交")?"PDM系统":(m_Set1.m_LocalState=="服务器"?"设计系统":"用户电脑")));
			m_List.SetItemText(m_List.GetItemCount()-1,6,m_Set1.m_Designer);
			m_List.SetItemText(m_List.GetItemCount()-1,7,m_Set1.m_Note);
			m_Set1.MoveNext();
		}
	}
	*pResult = 0;
}

void CAsmAddComplg::OnTvnSelchangedAsminsTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	long							num=(long)pNMTreeView->itemNew.lParam;
	CNProductInsInfoTable   		m_Set1(m_pSet.m_pDatabase);
	m_Set1.m_strFilter.Format("[TypeNum]=%ld",num);
	m_Set1.Open();	
	m_List.DeleteAllItems();
	m_List2.DeleteAllItems();
	m_btnOpen.EnableWindow(FALSE);
	if (!m_Set1.IsBOF())
	{
		m_btnOpen.EnableWindow(TRUE);
		m_Set1.MoveFirst();
		while (!m_Set1.IsEOF())
		{
			m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List.GetItemCount(),m_Set1.m_Name,0,0,0,m_Set1.m_PdtNum);
			m_List.SetItemText(m_List.GetItemCount()-1,1,m_Set1.m_InsCode);
			m_List.SetItemText(m_List.GetItemCount()-1,2,(m_Set1.m_FinishState==_T("完成")?_T("完成"):_T("未完成")));
			m_List.SetItemText(m_List.GetItemCount()-1,3,(m_Set1.m_AuditState==_T("审核通过")?_T("审核通过"):_T("审核未通过")));
			m_List.SetItemText(m_List.GetItemCount()-1,4,(m_Set1.m_PDMState==_T("提交")?_T("提交"):_T("未提交")));
			m_List.SetItemText(m_List.GetItemCount()-1,5,(m_Set1.m_PDMState==_T("提交")?"PDM系统":(m_Set1.m_LocalState=="服务器"?"设计系统":"用户电脑")));
			m_List.SetItemText(m_List.GetItemCount()-1,6,m_Set1.m_Designer);
			m_List.SetItemText(m_List.GetItemCount()-1,7,m_Set1.m_Note);
			m_Set1.MoveNext();
		}
	}
	*pResult = 0;
}
