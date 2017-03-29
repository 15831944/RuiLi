// FemMageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "FemMageDlg.h"
#include "FtpFile.h"
#include "myproe.h"
// CFemMageDlg 对话框

IMPLEMENT_DYNAMIC(CFemMageDlg, CDialog)
CFemMageDlg::CFemMageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFemMageDlg::IDD, pParent)
{
}

CFemMageDlg::~CFemMageDlg()
{
}

void CFemMageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CFemMageDlg, CDialog)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_TREE_OPEN, OnNMClickTreeOpen)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CFemMageDlg 消息处理程序

BOOL CFemMageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CFemMageDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_db.Close();

	CDialog::OnClose();
}


void CFemMageDlg::AddTree()
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

void CFemMageDlg::AddTree(HTREEITEM root,CArray<ModelStruct,ModelStruct> * List,int Fathernum)
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



void CFemMageDlg::OnNMClickTreeOpen(NMHDR *pNMHDR, LRESULT *pResult)
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

void CFemMageDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CFemMageDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
