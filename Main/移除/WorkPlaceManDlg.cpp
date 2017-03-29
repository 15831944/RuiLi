// WorkPlaceManDlg.cpp : 实现文?
//

#include "stdafx.h"
#include "Main.h"
#include "WorkPlaceManDlg.h"
#include "FtpFile.h"
#include "Part_Temp_Para_Info.h"
#include "Part_Temp_Perf.h"
#include "Part_Temp_Pic.h"
#include "Part_Temp_Rel.h"
#include "MdlDefDlg.h"
#include "AddMdlSeriesDlg.h"
#include "UsrSWTXTable.h"

//extern FtpConnectInfo myFtp;
//extern int DlgIcon;
//extern CArray<UsrSWTXTable1,UsrSWTXTable1>  p_UsrSWTXTableList;  //定义全局的事物特性表列项，
////在程序运行时，要给他一个初始值(无效)
//extern CArray<UsrRelTable,UsrRelTable>    p_UsrRelTableList;   //定义全局的关系列表项数组
//extern CArray<UsrPicture,UsrPicture>   p_UsrPicture;           //图片的全局变量                                                      
//extern CArray<UsrPerfParaTable,UsrPerfParaTable> p_UsrPerfParaTableList;//定义的性能参数表的全局变量
//extern CArray<UsrPartPorp,UsrPartPorp> p_UsrPartPorp;          //定义模型基本信息的全局变量
//
//// CWorkPlaceManDlg 对话框
//extern bool RemoveDireAndFile(CString localdir);
IMPLEMENT_DYNAMIC(CWorkPlaceManDlg, CDialog)
CWorkPlaceManDlg::CWorkPlaceManDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkPlaceManDlg::IDD, pParent)
	
	, m_WorkSpace(_T(""))
	, m_WorkSpaceNum(0)
	,fatherItem(NULL)
	, m_ComNameEditValue(_T(""))
	, m_ComDesingerEditValue(_T(""))
	, m_ComDataTimeValue(COleDateTime::GetCurrentTime())
	, m_ComDeptEditValue(_T(""))
	, m_ComNoteEditValue(_T(""))
	, selectNumber(-1)
	, m_ViewType(0)
{
}

CWorkPlaceManDlg::~CWorkPlaceManDlg()
{
}

void CWorkPlaceManDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE2, m_Tree);
	DDX_Text(pDX, IDC_EDIT1, m_WorkSpace);
	DDX_Control(pDX, IDC_BUTTON2, m_SaveButton);
	DDX_Control(pDX, IDC_BUTTON7, m_InfoDefButton);
	DDX_Control(pDX, IDC_BUTTON3, m_ToBeMdlButton);
	DDX_Control(pDX, IDC_CHECK1, m_ReqName);
	DDX_Control(pDX, IDC_CHECK2, m_ReqDesigner);
	DDX_Control(pDX, IDC_CHECK3, m_ReqSaveTime);
	DDX_Control(pDX, IDC_CHECK4, m_ReqDept);
	DDX_Control(pDX, IDC_CHECK5, m_ReqNote);
	DDX_Control(pDX, IDC_EDIT3, m_ComNameEdit);
	DDX_Text(pDX, IDC_EDIT3, m_ComNameEditValue);
	DDX_Control(pDX, IDC_EDIT4, m_ComDesingerEdit);
	DDX_Text(pDX, IDC_EDIT4, m_ComDesingerEditValue);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_ComDataTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_ComDataTimeValue);
	DDX_Control(pDX, IDC_EDIT6, m_ComDeptEdit);
	DDX_Text(pDX, IDC_EDIT6, m_ComDeptEditValue);
	DDX_Control(pDX, IDC_EDIT7, m_ComNoteEdit);
	DDX_Text(pDX, IDC_EDIT7, m_ComNoteEditValue);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBoxCtrl);
	DDX_Control(pDX, IDC_LIST3, m_List);
	DDX_Control(pDX, IDC_BUTTON6, m_OpenButton);
	DDX_CBIndex(pDX, IDC_COMBO1, m_ViewType);
	DDX_Control(pDX, IDC_BUTTON4, m_ViewButton);
}


BEGIN_MESSAGE_MAP(CWorkPlaceManDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_12117, OnNewWorkSpace)
	ON_COMMAND(ID_12118, OnDelWorkSpace)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE2, OnTvnBeginlabeleditTree2)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE2, OnTvnEndlabeleditTree2)
	ON_COMMAND(ID_12126, OnRename)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE2, OnTvnBegindragTree2)
	ON_NOTIFY(TVN_BEGINRDRAG, IDC_TREE2, OnTvnBeginrdragTree2)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_COMMAND(ID_12119, OnSelectWorkSapce)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, OnNMClickList3)
	ON_COMMAND(ID_12120, On12120)
	ON_COMMAND(ID_Menu12121, OnDelMdl)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_COMMAND(ID_12122, OnSeeMdlPropDlg)
END_MESSAGE_MAP()


// CWorkPlaceManDlg 消息处理程序
BOOL CWorkPlaceManDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON hIcon = AfxGetApp()->LoadIcon (DlgIcon);
	SetIcon (hIcon, TRUE);     // Set small icon  设置小图标
	//检查是否有当前模型，确定按钮的状态

	if(ProMdlCurrentGet((ProMdl*)&solid)!=PRO_TK_NO_ERROR)
	{
		m_SaveButton.EnableWindow(FALSE);
		m_InfoDefButton.EnableWindow(FALSE);
		m_ToBeMdlButton.EnableWindow(FALSE);
	}

	m_ImageList.Create(16, 16, ILC_COLOR, 4, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON5));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON6));
	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	m_Tree.SetTextColor (RGB(7,145,13));
	InitTreeCtrl();

	//设置查询的复选框的状态
	m_ReqName.SetCheck(BST_CHECKED);
    m_ComNameEdit.EnableWindow(TRUE);
	m_ReqDesigner.SetCheck(BST_UNCHECKED);
	m_ComDesingerEdit.EnableWindow(FALSE);
	m_ReqSaveTime.SetCheck(BST_UNCHECKED);
    m_ComDataTime.EnableWindow(FALSE);
	m_ReqDept.SetCheck(BST_UNCHECKED);
    m_ComDeptEdit.EnableWindow(FALSE);
	m_ReqNote.SetCheck(BST_UNCHECKED);
    m_ComNoteEdit.EnableWindow(FALSE);
	//设置打开按钮为不可用
	m_OpenButton.EnableWindow(FALSE);
	//设置查看按钮
	m_ViewButton.EnableWindow(FALSE);
	//CEdit m_ComNameEdit;
	//CString m_ComNameEditValue;
	//CEdit m_ComDesingerEdit;
	//CString m_ComDesingerEditValue;
	//CDateTimeCtrl m_ComDataTime;
	//COleDateTime m_ComDataTimeValue;
	//CEdit m_ComDeptEdit;
	//CString m_ComDeptEditValue;
	//CEdit m_ComNoteEdit;
	//CString m_ComNoteEditValue;
	//CComboBox m_ComboBoxCtrl;
	//CListCtrl m_List;
	//UpdateData(FALSE);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
void CWorkPlaceManDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
}
/************************************************************************/
/* 初始化树控件                                                         */
/************************************************************************/
void CWorkPlaceManDlg::InitTreeCtrl(void)
{
	//添加根节点
	m_Tree.DeleteAllItems();
	HTREEITEM hItemRoot=m_Tree.InsertItem(_T("工作空间"));
	if(m_pSet.IsOpen()) m_pSet.Close();
	
	try
	{
		m_pSet.m_strFilter="";
		if(!m_pSet.Open())
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
			if(m_pSet.m_FatherNum ==0)
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
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		if (m_pSet.IsOpen()) {
			m_pSet.Close();
			delete e;
		}
	}

	m_Tree.Expand(hItemRoot,TVE_EXPAND);
}



void CWorkPlaceManDlg::AddSubPartItem(HTREEITEM item)
{
	HTREEITEM tempItem;
	if(m_pSet.IsEOF()) return;
	m_pSet.MoveFirst();
	do {
		if(m_pSet.m_FatherNum ==(long)m_Tree.GetItemData(item))
		{

			int i=0,j=0;
			int nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			if(m_pSet.m_Type=="mdl") i=1,j=1;
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
/* 右键菜单                                                             */
/************************************************************************/
void CWorkPlaceManDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此添加消息处理程序代码
	CPoint point=GetCurrentMessage()->pt;
	if(WindowFromPoint(point)==&m_Tree)
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(6);
		HTREEITEM hItem=m_Tree.GetSelectedItem();
		TVITEM iTem;
		iTem.hItem=hItem;
		iTem.mask=TVIF_IMAGE;
		m_Tree.GetItem(&iTem);
		if (iTem.iImage!=1) {
			pPopup->EnableMenuItem(ID_12120,MF_DISABLED|MF_GRAYED);
			
		   pPopup->EnableMenuItem(ID_Menu12121,MF_DISABLED|MF_GRAYED);
		   

		}
		else 
		{
			pPopup->EnableMenuItem(ID_12120,MF_ENABLED);
            pPopup->EnableMenuItem(ID_Menu12121,MF_ENABLED);
			//对工作区的操作无效 ID_12117 ID_12119  ID_12126 ID_12118
			pPopup->EnableMenuItem(ID_12117,MF_DISABLED|MF_GRAYED);
			pPopup->EnableMenuItem(ID_12119,MF_DISABLED|MF_GRAYED);
			pPopup->EnableMenuItem(ID_12126,MF_DISABLED|MF_GRAYED);
			pPopup->EnableMenuItem(ID_12118,MF_DISABLED|MF_GRAYED);


		}
		ASSERT(pPopup!=NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);

	}
}
/************************************************************************/
/* 建立新的工作区                                                       */
/************************************************************************/
void CWorkPlaceManDlg::OnNewWorkSpace()
{
	// TODO: 在此添加命令处理程序代码

    CString   name;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	CString   fatherDir="";
	CFtpFile  m_ftp;
	int       newNum=0;
	//创建新工作区间
    try
	{
		if (m_pSet.IsOpen()) m_pSet.Close();
		for(int i=0;i<10000;i++)
		{
			name.Format("新工作区间(%d)",i);
			m_pSet.m_strFilter.Format("[FatherNum]=%d AND [Name]='%s'",(long)m_Tree.GetItemData(hItem),name);
			m_pSet.Open();
			if (m_pSet.IsBOF()) {
                m_pSet.Close();
				break;
			}
			m_pSet.Close();
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		if (m_pSet.IsOpen()) m_pSet.Close();
		return;
	}
	
	try
	{
		if (m_Tree.GetSelectedItem()==m_Tree.GetRootItem()) {
			fatherDir="/建模工作区/零件/"+name;
		}
		else
		{
			m_pSet.m_strFilter.Format("[Number]=%d",(long)m_Tree.GetItemData(hItem));
			m_pSet.Open();
			if (!m_pSet.IsBOF()) {
				m_pSet.MoveFirst();
				fatherDir=m_pSet.m_FileDirectory+"/"+name;
			}
			m_pSet.Close();
		}
        if(!m_ftp.CreateDirectory(myFtp,fatherDir))
		{
			return;
		}

		m_pSet.m_strFilter.Empty();
		m_pSet.Open();
        m_pSet.AddNew();
		m_pSet.m_FatherNum=(long)m_Tree.GetItemData(hItem);
		m_pSet.m_Name=name;
		m_pSet.m_Type="wrk";
		m_pSet.m_FileDirectory=fatherDir;
		m_pSet.m_Datatime=CTime::GetCurrentTime();
		m_pSet.Update();
		m_pSet.Requery();
		m_pSet.MoveLast();
		HTREEITEM newItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T(name), 
			0, 0, 0, 0, m_pSet.m_Number, hItem, NULL);
		newNum=m_pSet.m_Number;
		m_Tree.Expand(hItem,TVE_EXPAND);
		m_pSet.Close();
		//创建目录

	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		if (m_pSet.IsOpen()) m_pSet.Close();
        m_ftp.DeleteDirAndFile(fatherDir,myFtp);
		return;
	}    
}

HTREEITEM CWorkPlaceManDlg::GethItem(long data)
{
	
	return HTREEITEM();
}
void CWorkPlaceManDlg::OnSelectWorkSapce()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if (hItem==m_Tree.GetRootItem()) return;
	m_WorkSpace=m_Tree.GetItemText(hItem);
	m_WorkSpaceNum=(long)m_Tree.GetItemData(hItem);
	fatherItem=hItem;
	UpdateData(FALSE);
}
void CWorkPlaceManDlg::OnDelWorkSpace()
{
	// TODO: 在此添加命令处理程序代码
	CFtpFile  m_ftp;
	CString   sql_lang;
	CString   dir;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if (hItem==m_Tree.GetRootItem()) {
		return;
	}
	try
	{
		if(m_pSet.IsOpen()) m_pSet.Close();
		m_pSet.m_strFilter.Format("[Number]=%d",(long)m_Tree.GetItemData(hItem));
		m_pSet.Open();
		if (m_pSet.IsBOF()) {
			InitTreeCtrl();
			return;
		}
		dir=m_pSet.m_FileDirectory;
		if(!m_ftp.DeleteDirAndFile(dir,myFtp))
		{
			AfxMessageBox("删除工作区间不成功！");
			m_pSet.Close();
			return;
		}
		sql_lang.Format("DELETE FROM [Part_Temp_Mdl_Info] WHERE [Number]=%d OR [FatherNum]=%d OR [FileDirectory] LIKE '%s%%'"
			,(long)m_Tree.GetItemData(hItem),(long)m_Tree.GetItemData(hItem),dir);
		
		m_pSet.m_pDatabase->ExecuteSQL(_T(sql_lang));		
       
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		delete e;
				
	}
	if (m_pSet.IsOpen()) {
		m_pSet.Close();
	}
	InitTreeCtrl();
	return;
}

void CWorkPlaceManDlg::OnTvnBeginlabeleditTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//GetEditControl()->LimitText(127);

	*pResult = 0;
}

void CWorkPlaceManDlg::OnTvnEndlabeleditTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//GetEditControl()->GetWindowText(s);
	CFtpFile m_ftp;
	CString strItem=pTVDispInfo->item.pszText;
	CString oldName="";
	CString oldDir="";
	CString newDir="";
	int     _location=0;
	int     location=0;
	//HTREEITEM hParent=m_Tree.GetParentItem(pTVDispInfo->item);
	//CString strItem

	if (strItem.GetLength()>15) {
		AfxMessageBox("工作区名字长度超过要求！");
		*pResult = 0;
		m_Tree.EditLabel(m_Tree.GetSelectedItem());
		return;
	}
	if (strItem.GetLength()!=0) {
		try
		{
			if (m_pSet.IsOpen()) m_pSet.Close();
			m_pSet.m_strFilter.Format("[Number]=%d",(long)m_Tree.GetItemData(pTVDispInfo->item.hItem));
			m_pSet.Open();
			if(m_pSet.IsBOF())
			{
				m_pSet.Close();
                *pResult = 0;
				return;
			}
			m_pSet.MoveFirst();
			oldName=m_pSet.m_Name;
			oldDir=m_pSet.m_FileDirectory;
			newDir=oldDir;

            do {
				_location=location;
            	location=oldDir.Find(oldName,_location);
				if (location==-1) {
					_location-=1;
					break;
				}
				location+=1;				
				
            } while(1);
            newDir=newDir.Left(_location);
			newDir=newDir+strItem;

			//AfxMessageBox("旧路径"+oldDir+"旧名字"+oldName+"新路进"+newDir);		
			if(oldDir==newDir) 
			{*pResult = 0;
			m_pSet.Close();
			return;
			}
			if(!m_ftp.Rename(oldDir,newDir,myFtp))
			{
				m_pSet.Close();
				*pResult = 0;
				return;
			}
			m_pSet.Edit();
			m_pSet.m_Name=strItem;
			m_pSet.m_FileDirectory=newDir;
			m_pSet.Update();
			m_pSet.Close();

			m_pSet.m_strFilter.Empty();
			m_pSet.Open();
			if(!m_pSet.IsBOF())
			{
				m_pSet.MoveFirst();
				while (!m_pSet.IsEOF()) {
					if (m_pSet.m_FileDirectory.Find(oldDir,0)!=-1) {
                        CString temp=m_pSet.m_FileDirectory;
                        m_pSet.Edit(); 
						m_pSet.m_FileDirectory=newDir+temp.Right(temp.GetLength()-oldDir.GetLength());
						m_pSet.Update();
					}					
					m_pSet.MoveNext();
				}
			}
			m_pSet.Close();
			m_Tree.SetItemText(pTVDispInfo->item.hItem,pTVDispInfo->item.pszText);
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
            if (m_pSet.IsOpen()) m_pSet.Close();			
		}
		
	}
	
	//m_Tree.setite
    *pResult = 0;
}

void CWorkPlaceManDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialog::OnOK();
}

void CWorkPlaceManDlg::OnRename()
{
	// TODO: 在此添加命令处理程序代码
	m_Tree.EditLabel(m_Tree.GetSelectedItem());
}

void CWorkPlaceManDlg::OnTvnBegindragTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CWorkPlaceManDlg::OnTvnBeginrdragTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
/************************************************************************/
/* 保存模型                                                             */
/************************************************************************/
void CWorkPlaceManDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
    CPart_Temp_Para_Info   m_partParaInfo(m_pSet.m_pDatabase);
    CPart_Temp_Perf   m_partPerf(m_pSet.m_pDatabase);
    CPart_Temp_Pic    m_partPic(m_pSet.m_pDatabase);
    CPart_Temp_Rel    m_partRel(m_pSet.m_pDatabase);
	CString           tempPath;
	CString           fileDir;
	int               num=0;
	int               flag=0;
	int               mdlNumber=0;
	int               indexNo=0;
	CFtpFile          m_ftp;
	CString           name;
	num=(int)p_UsrPartPorp.GetCount();
	for (int i=0;i<num;i++) {
		if(p_UsrPartPorp[i].owner!=solid) continue;
		if(p_UsrPartPorp[i].isDef!=1)     continue;
		flag=1;

	}
	if (flag==0) {
		if (AfxMessageBox("还没有定义模型的基本信息,是否现在定义?")==IDOK) {
			CMdlDefDlg dlg;
			dlg.DoModal();
		}
		else
		{
			return;
		}
	}
	if(m_WorkSpaceNum==0)
	{
		AfxMessageBox("请选择存放工作区！");
		return;
	}
	m_pSet.m_strFilter.Empty();
	m_pSet.Open();
    if (m_pSet.m_pDatabase->CanTransact()) {		
		try
		{
			m_pSet.m_pDatabase->BeginTrans();
			//获取父项存储路进
			m_pSet.m_strFilter.Format("[Number]=%d",m_WorkSpaceNum);
			m_pSet.Requery();
			fileDir=m_pSet.m_FileDirectory;
			//保存模型，建立临时文件夹
			CreateTempDir(&tempPath);
			//添加基本信息
            num=(int)p_UsrPartPorp.GetCount();
			for(int i=0;i<num;i++)
			{
				if(p_UsrPartPorp[i].owner!=solid) continue;
				if(p_UsrPartPorp[i].isDef!=1) continue;
				m_pSet.AddNew();
				
				m_pSet.m_FatherNum=m_WorkSpaceNum;
				m_pSet.m_Name=p_UsrPartPorp[i].Name;
				m_pSet.m_Type="mdl";
				m_pSet.m_MdlerName=p_UsrPartPorp[i].Person;
				m_pSet.m_MdlerDepartment=p_UsrPartPorp[i].Dept;
				m_pSet.m_FileName=p_UsrPartPorp[i].FileName;
				fileDir=fileDir+"/"+p_UsrPartPorp[i].Name;
				name=p_UsrPartPorp[i].Name;
				m_pSet.m_FileDirectory=fileDir;
				m_pSet.m_Note=p_UsrPartPorp[i].Note;
				m_pSet.m_MdlFatherNum=p_UsrPartPorp[i].FatherNumber;
				m_pSet.m_MdlCode=p_UsrPartPorp[i].Code;
				m_pSet.m_MdlType=p_UsrPartPorp[i].Type;
				CTime  time=CTime::GetCurrentTime();
				m_pSet.m_Datatime=time;//.Format("%Y-%A-%d");
				//没有代码
				m_pSet.Update();
				break;
			}
			m_pSet.m_strFilter.Empty();
			m_pSet.Requery();
			m_pSet.MoveLast();
			mdlNumber=m_pSet.m_Number;
			//添加事物特性表
			num=(int)p_UsrSWTXTableList.GetCount();
			m_partParaInfo.Open();
			for(int i=0;i<num;i++)
			{
				if(p_UsrSWTXTableList[i].owner!=solid) continue;
				if(p_UsrSWTXTableList[i].isCheck!=1) continue;
				indexNo++;
				m_partParaInfo.AddNew();
				m_partParaInfo.m_PrtNum=mdlNumber;
				m_partParaInfo.m_Number=indexNo;
				m_partParaInfo.m_Name=p_UsrSWTXTableList[i].Name;
				m_partParaInfo.m_IdNum=p_UsrSWTXTableList[i].id;
                m_partParaInfo.m_ParaFeatName=p_UsrSWTXTableList[i].featName;
				m_partParaInfo.m_DimSymbol=p_UsrSWTXTableList[i].dimSymbol;
				m_partParaInfo.m_Value=p_UsrSWTXTableList[i].value;
				m_partParaInfo.m_Type=p_UsrSWTXTableList[i].type;
				m_partParaInfo.m_SubType=p_UsrSWTXTableList[i].subType;
				m_partParaInfo.m_UpLimit=p_UsrSWTXTableList[i].UpLimit;
				m_partParaInfo.m_DwLimit=p_UsrSWTXTableList[i].DwLimit;
				m_partParaInfo.m_Note=p_UsrSWTXTableList[i].Note;
				m_partParaInfo.Update();
			}
			m_partParaInfo.Close();
			indexNo=0;
			num=(int)p_UsrPerfParaTableList.GetCount();
			m_partPerf.Open();
			//添加性能参数信息
			for(int i=0;i<num;i++)
			{
				if(p_UsrPerfParaTableList[i].owner!=solid) continue;
				if(p_UsrPerfParaTableList[i].isCheck!=1) continue;
				indexNo++;
				m_partPerf.AddNew();
				m_partPerf.m_PrtNum=mdlNumber;
				m_partPerf.m_Number=indexNo;
				m_partPerf.m_Name=p_UsrPerfParaTableList[i].Name;
				m_partPerf.m_Value=p_UsrPerfParaTableList[i].value;
				m_partPerf.m_Note=p_UsrPerfParaTableList[i].Note;
				m_partPerf.Update();

			}
			m_partPerf.Close();
			indexNo=0;
			num=(int)p_UsrRelTableList.GetCount();
			m_partRel.Open();
			//添加关系信息
			for(int i=0;i<num;i++)
			{
				if(p_UsrRelTableList[i].owner!=solid) continue;
				if(p_UsrRelTableList[i].isCheck!=1) continue;
				indexNo++;
				m_partRel.AddNew();
				m_partRel.m_PrtNum=mdlNumber;
				m_partRel.m_Number=indexNo;
				m_partRel.m_Relation=p_UsrRelTableList[i].rel;
				m_partRel.m_Note=p_UsrRelTableList[i].note;
				m_partRel.Update();
			}
			m_partRel.Close();
			indexNo=0;
			num=(int)p_UsrPicture.GetCount();
			m_partPic.Open();
			//添加图片信息
			for(int i=0;i<num;i++)
			{
				if(p_UsrPicture[i].owner!=solid) continue;
				if(p_UsrPicture[i].isCheck!=1) continue;
				indexNo++;
				m_partPic.AddNew();
				m_partPic.m_PrtNum=mdlNumber;
				m_partPic.m_Number=indexNo;
				m_partPic.m_Name=p_UsrPicture[i].fileName;
				m_partPic.m_Note=p_UsrPicture[i].Note;
				m_partPic.m_IsMain=p_UsrPicture[i].isMain;
				m_partPic.Update();
			}
			m_partPic.Close();
			indexNo=0;
			//上传模型
			if (!m_ftp.CreateDirectory(myFtp,fileDir)) {
                 RemoveDireAndFile(tempPath);
				 m_pSet.m_pDatabase->Rollback();
				 m_pSet.Close();
				 return;
			}
			if (!m_ftp.UploadDirectorytoDirectory(tempPath,fileDir,myFtp)) {
				m_ftp.RemoveDirectory(myFtp,fileDir);
				RemoveDireAndFile(tempPath);
				m_pSet.m_pDatabase->Rollback();
				m_pSet.Close();
				return;
			}
			RemoveDireAndFile(tempPath);
			m_Tree.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,name,1,1,0,0,mdlNumber,fatherItem,NULL);
			m_Tree.Expand(fatherItem,TVE_EXPAND);
			m_pSet.m_pDatabase->CommitTrans();
			AfxMessageBox("保存成功！");
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			m_pSet.m_pDatabase->Rollback();
			if (m_pSet.IsOpen()) m_pSet.Close();
			delete e;
            RemoveDireAndFile(tempPath);
			return;

		}
    }
	m_pSet.Close();
}
/* 创建临时目录,保存模型，拷贝图片                                      */
/************************************************************************/
bool CWorkPlaceManDlg::CreateTempDir(CString * csPathName)
{

	ProPath     path;
	ProName     name;
	int         num=0;
	int         indexNo=0;
	CString     typeandversion;
	char        cPathName[PRO_PATH_SIZE];
	ProMdldata  model_info;
	CString     csPathPicture;
	CString     UsrDefName;
	CString     mdltemppath;
	//保存当前模型
	ProMdlSave(solid);
	ProMdlNameGet(solid,name); 
	//获得当前目录（当前模型不一定在当前目录）
	ProDirectoryCurrentGet(path);
	num=(int)p_UsrPartPorp.GetCount();
	//获得定义的零件名
	for(int i=0;i<num;i++)
	{
		if(p_UsrPartPorp[i].owner!=(ProMdl)solid) continue;
		if(p_UsrPartPorp[i].isDef !=1) continue;
		UsrDefName.Format(p_UsrPartPorp[i].Name);
		indexNo=i;
		break;
	}
	//生成临时目录
	(*csPathName)=CString(path)+UsrDefName+"_temp";
	if(!CreateDirectory(csPathName->GetString(),NULL)) 
	{
		return false;
	}
	//备份模型到临时目录
	sprintf(cPathName,"%s",csPathName->GetBuffer());	
	ProStringToWstring(model_info.path,cPathName);	
	ProMdlBackup(solid,&model_info);

	//重新获取模型的文件名
	typeandversion=_T(".prt");
	p_UsrPartPorp[indexNo].FileName=CString(model_info.name)+typeandversion;

	//在临时目录内创建图片目录
	csPathPicture=csPathName->GetString();
	csPathPicture=csPathPicture+"/"+"picture";
	if(!CreateDirectory(csPathPicture,NULL)) 
	{
		RemoveDireAndFile(_T(*csPathName));
		return false;
	}
	//备份图片到临时目录
	num=(int)p_UsrPicture.GetCount();
	for(int i=0;i<num;i++)
	{
		
		if(p_UsrPicture[i].owner!=(ProMdl)solid) continue;
		if(p_UsrPicture[i].isCheck!=1) continue;
		CopyFile(p_UsrPicture[i].filePath+p_UsrPicture[i].fileName,csPathPicture+"/"+p_UsrPicture[i].fileName,FALSE);
	}
	return true;
}


void CWorkPlaceManDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ComNameEdit.EnableWindow(m_ReqName.GetCheck()==BST_UNCHECKED?FALSE:TRUE);

}

void CWorkPlaceManDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ComDesingerEdit.EnableWindow(m_ReqDesigner.GetCheck()==BST_UNCHECKED?FALSE:TRUE);
}

void CWorkPlaceManDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
		m_ComDataTime.EnableWindow(m_ReqSaveTime.GetCheck()==BST_UNCHECKED?FALSE:TRUE);
}

void CWorkPlaceManDlg::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ComDeptEdit.EnableWindow(m_ReqDept.GetCheck()==BST_UNCHECKED?FALSE:TRUE);
}

void CWorkPlaceManDlg::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ComNoteEdit.EnableWindow(m_ReqNote.GetCheck()==BST_UNCHECKED?FALSE:TRUE);
}

void CWorkPlaceManDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sql_lang;
	CString temp;
	UpdateData(TRUE);
	m_ViewButton.EnableWindow(FALSE);
	m_OpenButton.EnableWindow(FALSE);
	if (m_ReqName.GetCheck()==BST_CHECKED) {
        if (!m_ComNameEditValue.IsEmpty()) {
			 temp.Format("[Name] LIKE '%%%s%%'",m_ComNameEditValue.Trim(" "));
         }
		sql_lang=temp;
	}
	if (m_ReqDesigner.GetCheck()==BST_CHECKED) {
		if (!m_ComDesingerEditValue.IsEmpty()) {
			if (sql_lang.IsEmpty()) {
				temp.Format("[MdlerName] LIKE '%%%s%%'",m_ComDesingerEditValue.Trim(" "));
			}
			else
			{
				temp.Format("OR [MdlerName] LIKE '%%%s%%'",m_ComDesingerEditValue.Trim(" "));
			}
			sql_lang=sql_lang+temp;
		}
	}
	if (m_ReqSaveTime.GetCheck()==BST_CHECKED) {
		
				if (sql_lang.IsEmpty()) {
					temp.Format("[Datatime] > '%d-%.2d-%.2d' AND [Datatime] < '%d-%.2d-%.2d'",
						m_ComDataTimeValue.GetYear(),m_ComDataTimeValue.GetMonth(),m_ComDataTimeValue.GetDay()-1,m_ComDataTimeValue.GetYear(),m_ComDataTimeValue.GetMonth(),m_ComDataTimeValue.GetDay()+1);
				}
				else
				{
					temp.Format("OR [Datatime] > '%d-%.2d-%.2d' AND [Datatime] < '%d-%.2d-%.2d'"
						,m_ComDataTimeValue.GetYear(),m_ComDataTimeValue.GetMonth(),m_ComDataTimeValue.GetDay()-1,m_ComDataTimeValue.GetYear(),m_ComDataTimeValue.GetMonth(),m_ComDataTimeValue.GetDay()+1);
				}
				sql_lang=sql_lang+temp;
	}
	if (m_ReqDept.GetCheck()==BST_CHECKED) {
			if (!m_ComDeptEditValue.IsEmpty()) {
				if (sql_lang.IsEmpty()) {
					temp.Format("[MdlerDepartment] LIKE '%%%s%%'",m_ComDeptEditValue.Trim(" "));
				}
				else
				{
					temp.Format("OR [MdlerDepartment] LIKE '%%%s%%'",m_ComDeptEditValue.Trim(" "));
				}
				sql_lang=sql_lang+temp;
		}
	}
	if (m_ReqNote.GetCheck()==BST_CHECKED) {
			if (!m_ComNoteEditValue.IsEmpty()) {
				if (sql_lang.IsEmpty()) {
					temp.Format("[Note] LIKE '%%%s%%'",m_ComNoteEditValue.Trim(" "));
				}
				else
				{
					temp.Format("OR [Note] LIKE '%%%s%%'",m_ComNoteEditValue.Trim(" "));
				}
				sql_lang=sql_lang+temp;
		}
	}
	try
	{
         if (m_pSet.IsOpen()) m_pSet.Close();
		 m_pSet.m_strFilter=sql_lang;
		 //AfxMessageBox(sql_lang);
		 m_pSet.Open();
		 p_ReqList.RemoveAll();
		 if (!m_pSet.IsBOF())
		 {
			 m_pSet.MoveFirst();
			 while (!m_pSet.IsEOF())
			 {
				 RequeryItem tempItem;
				 tempItem.number=m_pSet.m_Number;
				 tempItem.name=m_pSet.m_Name;
				 tempItem.datetime=m_pSet.m_Datatime;
				 tempItem.note=m_pSet.m_Note;
				 tempItem.person=m_pSet.m_MdlerName;
				 tempItem.dept=m_pSet.m_MdlerDepartment;
				 tempItem.type=m_pSet.m_Type;
				 tempItem.filename=m_pSet.m_FileName;
				 p_ReqList.Add(tempItem);
				 m_pSet.MoveNext();
			 }
		 }
		 m_pSet.Close();
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		if (m_pSet.IsOpen()) m_pSet.Close();
		return;
	}
	ShowListWithProp();

}

void CWorkPlaceManDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int window_id;
	ProWindowCurrentGet(&window_id);
	ProWindowActivate(window_id);
	CDialog::OnClose();
}

void CWorkPlaceManDlg::ShowListWithProp(void)
{
	int  num=0;
	m_List.DeleteAllItems();
	for (int i=0;i<10;i++)
	{
		m_List.DeleteColumn(0);
	}
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"名称",LVCFMT_LEFT,80,-1);
	m_List.InsertColumn(1,"类型",LVCFMT_LEFT,35,-1);
	m_List.InsertColumn(2,"文件名",LVCFMT_LEFT,80,-1);
	m_List.InsertColumn(3,"设计员",LVCFMT_LEFT,50,-1);
	m_List.InsertColumn(4,"部门",LVCFMT_LEFT,50,-1);
	m_List.InsertColumn(5,"备注",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(6,"日期",LVCFMT_LEFT,70,-1);
	num=(int)p_ReqList.GetCount();
	for (int i=0;i<num;i++)
	{
		m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,i,p_ReqList[i].name,0,0,0,p_ReqList[i].number);
		m_List.SetItemText(i,1,p_ReqList[i].type=="mdl"?"模型":"工作区"); 
		m_List.SetItemText(i,2,p_ReqList[i].filename);
		m_List.SetItemText(i,3,p_ReqList[i].person);
		m_List.SetItemText(i,4,p_ReqList[i].dept);
		m_List.SetItemText(i,5,p_ReqList[i].note);
		CString datatime;
		datatime.Format("%d-%d-%d",p_ReqList[i].datetime.GetYear(),p_ReqList[i].datetime.GetMonth(),p_ReqList[i].datetime.GetDay());
		m_List.SetItemText(i,6,datatime);
	}
}
/************************************************************************/
/* 查看模型的尺寸，参数，关系，图片等信息                                                                      */
/************************************************************************/
void CWorkPlaceManDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CPart_Temp_Para_Info   m_partParaInfo(m_pSet.m_pDatabase);
	CPart_Temp_Perf   m_partPerf(m_pSet.m_pDatabase);
	CPart_Temp_Pic    m_partPic(m_pSet.m_pDatabase);
	CPart_Temp_Rel    m_partRel(m_pSet.m_pDatabase);
	int               indexNo=0;
	CString           csindexNO="";
	UpdateData(TRUE);
	//int cur=m_ComboBoxCtrl.GetCurSel();
	if(selectNumber==-1) return;
	//m_ViewButton.EnableWindow(TRUE);
	switch(m_ViewType) {
	case 0:  //关系
		m_partRel.m_strFilter.Format("[PrtNum]=%d",selectNumber);
		m_partRel.Open();
		if (m_partRel.IsBOF()) {
			AfxMessageBox("模型没有定义关系式！");
			return;
		}
		m_List.RemoveItemAndCol();
		m_List.InsertColumn(0,"编号",LVCFMT_LEFT,50,-1);
		m_List.InsertColumn(1,"关系式",LVCFMT_LEFT,100,-1);
		m_List.InsertColumn(2,"说明",LVCFMT_LEFT,100,-1);
		m_partRel.MoveFirst();
        while (!m_partRel.IsEOF()) {			
			csindexNO.Format("%d",indexNo+1);
			m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,indexNo,csindexNO,0,0,0,-1);
			m_List.SetItemText(indexNo,1,m_partRel.m_Relation);
			m_List.SetItemText(indexNo,2,m_partRel.m_Note);
			m_partRel.MoveNext();
			indexNo++;

        }
		break;
	case 1:  //事务特性表
        m_partParaInfo.m_strFilter.Format("[PrtNum]=%d",selectNumber);
		m_partParaInfo.Open();
		if (m_partParaInfo.IsBOF()) {
			AfxMessageBox("模型没有定义事物特性表");
			return;
		}
		m_List.RemoveItemAndCol();
		m_List.InsertColumn(0,"编号",LVCFMT_LEFT,50);
		m_List.InsertColumn(1,"标识",LVCFMT_LEFT,50);                  //
		m_List.InsertColumn(2,"名称",LVCFMT_LEFT,50);                  //重新命名得到，默认为尺寸符号和参数和特征的名称
		m_List.InsertColumn(3,"符号",LVCFMT_LEFT,60);                  //尺寸的符号，以及参数和特征的名称
		m_List.InsertColumn(4,"类型",LVCFMT_LEFT,50);
		m_List.InsertColumn(5,"子类型",LVCFMT_LEFT,50);
		m_List.InsertColumn(6,"值",LVCFMT_LEFT,50);
		m_List.InsertColumn(7,"上偏差",LVCFMT_LEFT,50);
		m_List.InsertColumn(8,"下偏差",LVCFMT_LEFT,50);
		m_List.InsertColumn(9,"变型规则",LVCFMT_LEFT,80);
		m_partParaInfo.MoveFirst();
		while (!m_partParaInfo.IsEOF()) {
            csindexNO.Format("%d",indexNo+1);
			m_List.InsertItem(indexNo,csindexNO);
			m_List.SetItemText(indexNo,1,m_partParaInfo.m_IdNum);
			m_List.SetItemText(indexNo,2,m_partParaInfo.m_Name);
			m_List.SetItemText(indexNo,3,m_partParaInfo.m_DimSymbol);
			m_List.SetItemText(indexNo,4,m_partParaInfo.m_Type);
			m_List.SetItemText(indexNo,5,GetSubType(m_partParaInfo.m_SubType));
			m_List.SetItemText(indexNo,6,m_partParaInfo.m_Value);
			m_List.SetItemText(indexNo,7,m_partParaInfo.m_UpLimit);
			m_List.SetItemText(indexNo,8,m_partParaInfo.m_DwLimit);
			m_List.SetItemText(indexNo,9,m_partParaInfo.m_Note);
            m_partParaInfo.MoveNext();
			indexNo++;
		}
		break;
	case 2:  //性能参数表
		m_partPerf.m_strFilter.Format("[PrtNum]=%d",selectNumber);
		m_partPerf.Open();
		if (m_partPerf.IsBOF()) {
			AfxMessageBox("模型没有定义性能参数！");
			return;
		}
		m_List.RemoveItemAndCol();
		m_List.InsertColumn(0,"编号",LVCFMT_LEFT,50,-1);
		m_List.InsertColumn(1,"名称",LVCFMT_LEFT,100,-1);
		m_List.InsertColumn(2,"值",LVCFMT_LEFT,100,-1);
		m_List.InsertColumn(3,"说明",LVCFMT_LEFT,100,-1);
		m_partPerf.MoveFirst();
		while (!m_partPerf.IsEOF()) {			
			csindexNO.Format("%d",indexNo+1);
			m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,indexNo,csindexNO,0,0,0,-1);
			m_List.SetItemText(indexNo,1,m_partPerf.m_Name);
			m_List.SetItemText(indexNo,2,m_partPerf.m_Value);
			m_List.SetItemText(indexNo,2,m_partPerf.m_Note);
			m_partPerf.MoveNext();
			indexNo++;

		}
		break;
	default:  //样图
		m_partPic.m_strFilter.Format("[PrtNum]=%d",selectNumber);
		m_partPic.Open();
		if (m_partPic.IsBOF()) {
			AfxMessageBox("模型没有样图！");
			return;
		}
		m_List.RemoveItemAndCol();
		m_List.InsertColumn(0,"编号",LVCFMT_LEFT,50,-1);
		m_List.InsertColumn(1,"文件名",LVCFMT_LEFT,100,-1);
		m_List.InsertColumn(2,"说明",LVCFMT_LEFT,100,-1);
		m_partPic.MoveFirst();
		while (!m_partPic.IsEOF()) {			
			csindexNO.Format("%d",indexNo+1);
			m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,indexNo,csindexNO,0,0,0,-1);
			m_List.SetItemText(indexNo,1,m_partPic.m_Name);
			m_List.SetItemText(indexNo,2,m_partPic.m_Note);
			m_partPic.MoveNext();
			indexNo++;

		}
		break;
	}
}

void CWorkPlaceManDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	int window_id;
	if(ProWindowCurrentGet(&window_id)==PRO_TK_NO_ERROR)  ProWindowRefresh(window_id);
	// TODO: 在此添加消息处理程序代码
}

void CWorkPlaceManDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	CMdlDefDlg dlg;
	dlg.DoModal();
}
/************************************************************************/
/* 打开选中的模型，应该是当前选中的模型                                 */
/************************************************************************/
void CWorkPlaceManDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	ProError   status;
	ProPath    wcurrentDir;
	CString    currentDit;
	CFtpFile   m_ftp;
	CString    fullFileName;//包括路径的文件名
	ProPath    name;
	ProType    type;
	ProName    w_name;
	int        w_id;
	ProMdl     mdl;
	status=ProDirectoryCurrentGet(wcurrentDir);
	currentDit=CString(wcurrentDir);
	//int k=currentDit.ReverseFind('\\');
	//currentDit=currentDit.Left(k);
	if (selectNumber!=-1) {
		try
		{
			if (m_pSet.IsOpen()) m_pSet.Close();
			m_pSet.m_strFilter.Format("[Number]=%d",selectNumber);
			m_pSet.Open();
			if (m_pSet.IsBOF()) 
			{AfxMessageBox("模型已经不存在，不能打开！");m_pSet.Close();return;}
			m_pSet.MoveFirst();
			//检查当前是否有已经存在相同文件名的模型打开
			for (int i=0;;i++)
			{
				CString version;				
				version.Format("%s(%d)",m_pSet.m_Name,i);
				version=currentDit+version;
				if (CreateDirectoryA(version,NULL))
				{
					currentDit=version;
					break;
				}
			}
            if (!m_ftp.DownloadDirectorytoDirectory(m_pSet.m_FileDirectory,currentDit,myFtp))
            {
				AfxMessageBox("从服务器读取文件过程中不成功，请检查网络");
				m_pSet.Close();
				return;
            }
			fullFileName=currentDit+"\\"+m_pSet.m_FileName;
			//AfxMessageBox(fullFileName);
            ProStringToWstring(name,fullFileName.GetBuffer());
			status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,&mdl); 
			if (status!=PRO_TK_NO_ERROR)
			{
				AfxMessageBox("没能打开下载到本地的文件！请检查版本！");
				m_pSet.Close();
				return;
			}
			status = ProMdlTypeGet(mdl, (ProMdlType*)&type);
			status= ProMdlNameGet(mdl, w_name);
			status= ProObjectwindowCreate(w_name, type, &w_id);
			if(!status== PRO_TK_NO_ERROR)
			{
				return;
			}
			status = ProMdlDisplay(mdl);
			

		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			if(m_pSet.IsOpen()) m_pSet.Close();
			return;
		}
		m_pSet.Close();

	}
	CDialog::OnOK();
	status = ProWindowActivate(w_id);
	AddMdlParaRelPic(_T(currentDit+"\\"),mdl);

}
/************************************************************************/
/* 点击列表                                                             */
/************************************************************************/
void CWorkPlaceManDlg::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
   int num=0;
   num=(int)m_List.GetItemCount();
   m_OpenButton.EnableWindow(FALSE);
   m_ViewButton.EnableWindow(FALSE);
   for (int i=0;i<num;i++)
   {
	   if ((long)m_List.GetItemData(i)==-1)
	   {
		   continue;
	   }
	   if (m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
	   {
		   selectNumber=(long)m_List.GetItemData(i);
		   if (m_List.GetItemText(i,1)=="模型")
		   {
			   m_OpenButton.EnableWindow(TRUE);
			   m_ViewButton.EnableWindow(TRUE);
		   }
		   break;
	   }
   }
	*pResult = 0;
}
/************************************************************************/
/* localDir=g:\\dfd\\    类型                                           */
/************************************************************************/
void CWorkPlaceManDlg::AddMdlParaRelPic(CString localDir,ProMdl mdl)
{
	CPart_Temp_Para_Info   m_partParaInfo(m_pSet.m_pDatabase);
	CPart_Temp_Perf   m_partPerf(m_pSet.m_pDatabase);
	CPart_Temp_Pic    m_partPic(m_pSet.m_pDatabase);
	CPart_Temp_Rel    m_partRel(m_pSet.m_pDatabase);
	int               propNum=0;
	int               num=0;
	if (selectNumber==-1)
	{
		return;
	}
	num=(int)p_UsrPartPorp.GetCount();
	for (int i=0;i<num;i++)
	{
		if (p_UsrPartPorp[i].isDef!=1) continue;
		if (p_UsrPartPorp[i].owner==mdl) return;
	}
	try
	{
		if (m_pSet.IsOpen()) m_pSet.Close();
		m_pSet.m_strFilter.Format("[Number]=%d",selectNumber);
		m_pSet.Open();
		if (m_pSet.IsBOF()) 
		{
			m_pSet.Close();
			return;
		}
		//添加属性信息
        UsrPartPorp tempProp;
        tempProp.owner=mdl;tempProp.Name=m_pSet.m_Name;tempProp.Person=m_pSet.m_MdlerName;
		tempProp.Dept=m_pSet.m_MdlerDepartment;tempProp.isDef=1;tempProp.FilePath=localDir;
		tempProp.FileName=m_pSet.m_FileName;tempProp.FatherNumber=-1;tempProp.Code="";
		tempProp.Note=m_pSet.m_Note;
		tempProp.FatherNumber=m_pSet.m_MdlFatherNum;
		tempProp.Code=m_pSet.m_MdlCode;
		tempProp.Type=m_pSet.m_MdlType;


		IsMdlInfoDefAndDef((ProSolid)mdl);  //没有定义就先定义
		GetMdlProp((ProSolid)mdl,&propNum); //得到定义的序号
		p_UsrPartPorp[propNum]=tempProp;//进行修改
		//propNum=(int)p_UsrPartPorp.Add(tempProp);
		//添加参数信息
        m_partParaInfo.m_strFilter.Format("[PrtNum]=%d",selectNumber);
		m_partParaInfo.Open();
		if (!m_partParaInfo.IsBOF())
		{
			m_partParaInfo.MoveFirst();
			while (!m_partParaInfo.IsEOF())
			{
				UsrSWTXTable1 temp;
				temp.id=m_partParaInfo.m_IdNum;
				temp.Name=m_partParaInfo.m_Name;
				temp.isCheck=1;
				temp.Note=m_partParaInfo.m_Note;
				temp.featName=m_partParaInfo.m_ParaFeatName;
				temp.dimSymbol=m_partParaInfo.m_DimSymbol;
				temp.value=m_partParaInfo.m_Value;
				temp.owner=mdl;
				temp.type=m_partParaInfo.m_Type;
				temp.subType=m_partParaInfo.m_SubType;
				temp.DwLimit=m_partParaInfo.m_DwLimit;
				temp.UpLimit=m_partParaInfo.m_UpLimit;
				temp.SetDimPara((ProSolid)mdl);
				p_UsrSWTXTableList.Add(temp);
				m_partParaInfo.MoveNext();
			}
		}
		m_partParaInfo.Close();
		//添加性能参数信息
        m_partPerf.m_strFilter.Format("[PrtNum]=%d",selectNumber);
		m_partPerf.Open();
		if (!m_partPerf.IsBOF())
		{
			m_partPerf.MoveFirst();
			while (!m_partPerf.IsEOF())
			{
				UsrPerfParaTable temp_perf;
				temp_perf.isCheck=1;temp_perf.Name=m_partPerf.m_Name;
				temp_perf.owner=mdl;temp_perf.value=m_partPerf.m_Value;
				temp_perf.Note=m_partPerf.m_Note;
				p_UsrPerfParaTableList.Add(temp_perf);
				m_partPerf.MoveNext();
			}
		}
		m_partPerf.Close();
		//添加关系式信息
        m_partRel.m_strFilter.Format("[PrtNum]=%d",selectNumber);
		m_partRel.Open();
		if (!m_partRel.IsBOF())
		{
			m_partRel.MoveFirst();
			while (!m_partRel.IsEOF())
			{
				UsrRelTable temp_rel;
				temp_rel.isCheck=1;
				temp_rel.owner=mdl;
				temp_rel.rel=m_partRel.m_Relation;
				temp_rel.note=m_partRel.m_Note;
				p_UsrRelTableList.Add(temp_rel);
				m_partRel.MoveNext();
			}
		}
		m_partRel.Close();
		//添加图片信息
		m_partPic.m_strFilter.Format("[PrtNum]=%d",selectNumber);
		m_partPic.Open();
		if (!m_partPic.IsBOF())
		{
			m_partPic.MoveFirst();
			while (!m_partPic.IsEOF())
			{
				UsrPicture temp_pic;
				temp_pic.isCheck=1;
				temp_pic.isMain=(int)m_partPic.m_IsMain;
				if (temp_pic.isMain==1)
				{
					p_UsrPartPorp[propNum].pic_path=localDir+"picture\\"+m_partPic.m_Name;
				}
				temp_pic.fileName=m_partPic.m_Name;
				temp_pic.Note=m_partPic.m_Note;
				temp_pic.filePath=localDir+"picture\\";
				temp_pic.owner=mdl;
				p_UsrPicture.Add(temp_pic);
				m_partPic.MoveNext();
			}
		}
		m_partPic.Close();
		
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		if (m_pSet.IsOpen()) m_pSet.Close();
	}
	if (m_pSet.IsOpen()) m_pSet.Close();	
}
/************************************************************************/
/* 检查是否有相同的文件已经打开，如果有，则么修改当前打开的文件，或者关闭
当前打开的文件                                                          */
/************************************************************************/
bool CWorkPlaceManDlg::CheckMdl(CString fileName)
{
	return false;
}

void CWorkPlaceManDlg::On12120()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	//m_Tree.getite
	selectNumber=(int)m_Tree.GetItemData(hItem);
	OnBnClickedButton6();
}

void CWorkPlaceManDlg::OnDelMdl()
{
	// TODO: 在此添加命令处理程序代码
	CFtpFile  m_ftp;
	CString   sql_lang;
	CString   dir;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if (hItem==m_Tree.GetRootItem()) {
		return;
	}
	try
	{
		if(m_pSet.IsOpen()) m_pSet.Close();
		m_pSet.m_strFilter.Format("[Number]=%d",(long)m_Tree.GetItemData(hItem));
		m_pSet.Open();
		if (m_pSet.IsBOF()) {
			InitTreeCtrl();
			return;
		}
		dir=m_pSet.m_FileDirectory;
		if(!m_ftp.DeleteDirAndFile(dir,myFtp))
		{
			AfxMessageBox("删除模型不成功！");
			m_pSet.Close();
			return;
		}
		sql_lang.Format("DELETE FROM [Part_Temp_Mdl_Info] WHERE [Number]=%d OR [FatherNum]=%d OR [FileDirectory] LIKE '%s%%'"
			,(long)m_Tree.GetItemData(hItem),(long)m_Tree.GetItemData(hItem),dir);

		m_pSet.m_pDatabase->ExecuteSQL(_T(sql_lang));		

	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		delete e;

	}
	if (m_pSet.IsOpen()) {
		m_pSet.Close();
	}
	InitTreeCtrl();
}

CString CWorkPlaceManDlg::GetSubType(CString subtype)
{
	CString reType;
	if (subtype=="PRO_PARAM_DOUBLE") {
		reType="浮点型";
	}
	else if (subtype=="PRO_PARAM_STRING") {
		reType="字符串";
	}
	else if (subtype=="PRO_PARAM_INTEGER") {
		reType="整型";
	}
	else if (subtype=="PRO_PARAM_BOOLEAN") {
		reType="布尔型";
	}
	else if (subtype=="PRO_PARAM_NOTE_ID") {
		reType="注释号";
	}
	else if (subtype=="PRODIMTYPE_LINEAR") {
		reType="线性";
	}
	else if (subtype=="PRODIMTYPE_RADIUS") {
		reType="半径";
	}
	else if (subtype=="PRODIMTYPE_DIAMETER") {
		reType="直径";
	}
	else if (subtype=="角度") {
		reType="PRODIMTYPE_ANGLE";
	}
	else {
		reType="未知";
	}
	return reType;
}
/************************************************************************/
/* 返回                                                                 */
/************************************************************************/
void CWorkPlaceManDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
		ShowListWithProp();
		m_ViewButton.EnableWindow(FALSE);
		m_OpenButton.EnableWindow(FALSE);
}
/************************************************************************/
/* 查看属性                                                             */
/************************************************************************/
void CWorkPlaceManDlg::OnSeeMdlPropDlg()
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
			return;
		}
		m_pSet.MoveFirst();
		mdlInfo.Name=m_pSet.m_Name;
		mdlInfo.Code="未定义";
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
