// FemOpenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "FemOpenDlg.h"
#include "myproe.h"


// CFemOpenDlg 对话框

IMPLEMENT_DYNAMIC(CFemOpenDlg, CDialog)
CFemOpenDlg::CFemOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFemOpenDlg::IDD, pParent)
	, m_Path(_T(""))
{
}

CFemOpenDlg::~CFemOpenDlg()
{
}

void CFemOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_TREE_OPEN, m_Tree);
	DDX_Text(pDX, IDC_EDIT1, m_Path);
}


BEGIN_MESSAGE_MAP(CFemOpenDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_TREE_OPEN, OnNMClickTreeOpen)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnBnClickedButtonSelect)
END_MESSAGE_MAP()


// CFemOpenDlg 消息处理程序

void CFemOpenDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString fullpath;
	CString temp="C:\\proe_temp";
	::CreateDirectory(temp,NULL);
	int index=0;
	for (int i=0;i<m_List.GetItemCount();i++)
	{
		if ( m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			fullpath=m_List.GetItemText(i,3);
			index=fullpath.ReverseFind('/');
			CString name=fullpath.Mid(index+1);
			m_Path=temp+"\\"+name;
			ftpfile.DownloadFiletoFile(fullpath,m_Path,myFtp);
			
		}
	}
   CString str=m_Path;


	ProSolid solid;
	LoadModel(str,(ProMdl *)&solid);
	ShowNewWindow((ProMdl *)&solid);
}

	//根据路径和solid指针打开solid文件
	int CFemOpenDlg::LoadModel(CString path,ProMdl * pSolid)
	{
		ProPath fullpath;
		ProError status;
		ProStringToWstring(fullpath,path.GetBuffer());
		ProMdlType type=PRO_MDL_UNUSED ;
		status=ProMdlLoad(fullpath,type,PRO_B_FALSE,(ProMdl*)pSolid);
		if(status==PRO_TK_NO_ERROR)
		{
			AfxMessageBox("加载成功");
			return 1;
		}
		else
		{
			AfxMessageBox("加载失败");
			return 0;
		}
	}

	//显示模型
	int  CFemOpenDlg::ShowNewWindow(ProMdl * pSolid)
	{
		ProError status;

		int window_id ;
		ProName object_name ;
		ProType  object_type ;
		ProMdlNameGet(*pSolid,object_name);
		ProMdlTypeGet(*pSolid,(ProMdlType *)&object_type);
		ProObjectwindowCreate(object_name,object_type,&window_id);
		ProMdlWindowGet(*pSolid,&window_id);
		status=ProMdlDisplay(*pSolid);
		if(status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("显示失败");
			return 0;
		}
		status=ProWindowActivate(window_id);
		if(status==PRO_TK_NO_ERROR)
		{
			//AfxMessageBox("显示成功");
			return 1;
		}
		return 0;
	}
	//OnOK();


void CFemOpenDlg::OnNMClickTreeOpen(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem;
	UINT uFlags=0;
	CPoint point;
	point= GetCurrentMessage()->pt;///
	////右键菜单
	int number;
	CString type, FileDirectory;
	CString strSql;
	CString str;

	m_Tree.ScreenToClient(&point);///
	hItem =m_Tree.HitTest(point, &uFlags);
	if ( hItem&&(uFlags&TVHT_ONITEM) )
	{
		number=m_Tree.GetItemData(hItem);
		if(number!=0)
		{
			strSql.Format("select * from FemPrt where Number=%d",number);
			int rows;
			rows=m_db.Query(strSql);
			rows=m_db.getRows();
			if(rows==0) return;
			m_db.MoveBegin();
			type=m_db.getString("Type");
			if(type=="mdl")
			{
				strSql.Format("select * from FemPrt where Number=%d",number);
			}
			else
			{
				strSql.Format("select * from FemPrt where FatherNum=%d and Type='mdl'",number);
			}
			m_db.Query(strSql);
			rows=m_db.getRows();
			m_List.DeleteAllItems();
			if(rows==0) return;
			m_db.MoveBegin();
			for(int i=0;i<rows;i++)
			{
				str=m_db.getString("Name");
				m_List.InsertItem(i,str);
				str=m_db.getDate("DateTime");
				m_List.SetItemText(i,1,str);
				str.Format("%d",m_db.getInt("ConnPrtMdlNum"));
				m_List.SetItemText(i,2,str);
				str=m_db.getString("FileDirectory");
				m_List.SetItemText(i,3,str);
				str=m_db.getString("Note");
				m_List.SetItemText(i,4,str);
				m_db.MoveNext();
			}

		}


	}
	*pResult = 0;
}

void CFemOpenDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
   m_db.Close();
	CDialog::OnClose();
}

BOOL CFemOpenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,"名称",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(1,"建模时间",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(2,"关联模型号",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(3,"保存路径",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(4,"备注",LVCFMT_LEFT,200,-1);

	int status;
	status=m_db.Open(globl_connectString);
	m_ImageList.Create(16, 16, ILC_COLOR8, 0, 4);//
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	this->AddTree();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}


void CFemOpenDlg::AddTree()
{
	CString strSql;
	int status=0;
	int rows=0;
	m_Tree.DeleteAllItems();
	HTREEITEM root;
	root=m_Tree.InsertItem("有限元模型",NULL);
	m_Tree.SetItemData(root,0);
	strSql="select * from FemPrt";
	m_db.Query(strSql);
	rows=m_db.getRows();
	if(rows==0) return;
	m_db.MoveBegin();
	ModelStruct ms;
	ModelList.RemoveAll();
	for(int i=0;i<rows;i++)
	{
		ms.Number=m_db.getInt("Number");
		ms.FatherNum=m_db.getInt("FatherNum");
		ms.Name=m_db.getString("Name");
		ms.Type=m_db.getString("Type");
		ModelList.Add(ms);
		m_db.MoveNext();
	}
	AddTree(root,&ModelList,0);
	m_Tree.Expand(root,TVE_EXPAND);
}

void CFemOpenDlg::AddTree(HTREEITEM root,CArray<ModelStruct,ModelStruct> * List,int Fathernum)
{
	int num=List->GetSize();
	CString name;
	HTREEITEM hitem;
	for(int i=0;i<num;i++)
	{
		if((*List)[i].FatherNum==Fathernum)
		{
			name=(*List)[i].Name;
			if((*List)[i].Type=="typ")
			{
				hitem=m_Tree.InsertItem(name,0,0,root);
				m_Tree.SetItemData(hitem,(*List)[i].Number);
				AddTree(hitem,List,(*List)[i].Number);
				///AddTree(hitem,(*List)[i].Number);
			}
			else
				if((*List)[i].Type=="mdl")
				{
					hitem=m_Tree.InsertItem(name,1,1,root);
					m_Tree.SetItemData(hitem,(*List)[i].Number);
					AddTree(hitem,List,(*List)[i].Number);
					///AddTree(hitem,(*List)[i].Number);
				}	
		}
	}
}


void CFemOpenDlg::OnBnClickedButtonSelect()
{
	UpdateData(TRUE);
	char szFilters[]=
		"prt Files (*.prt)|*.prt.*|All Files (*.*)|*.*||";

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg (FALSE, "prt", NULL,
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if( fileDlg.DoModal ()==IDOK )
	{
		m_Path=fileDlg.GetPathName();
	}
	UpdateData(FALSE);
}
