// CommonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "CommonDlg.h"


// CCommonDlg 对话框

IMPLEMENT_DYNAMIC(CCommonDlg, CDialog)
CCommonDlg::CCommonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommonDlg::IDD, pParent)
{
	m_Table="NPartManageTable";
}

CCommonDlg::~CCommonDlg()
{
}

void CCommonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Control(pDX, IDC_LIST_DIM, m_List);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CCommonDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, OnNMClickTree1)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DIM, OnNMClickListDim)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCommonDlg 消息处理程序

BOOL CCommonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_List.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,"参数名称",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(1,"参数值",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(2,"参数系统默认名",LVCFMT_LEFT,100,-1);

	m_Combo.AddString("零件模型");
    m_Combo.AddString("装配模型");
	m_Combo.SelectString(0,"零件模型");

	int status;
	status=m_db.Open(globl_connectString);
	//status=m_db.Open("DSN=ruili;UID=scm;PWD=scm;");
	

	m_ImageList.Create(16, 16, ILC_COLOR8, 0, 3);//
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	if(status!=0)
	this->AddTree();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CCommonDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}




void CCommonDlg::AddTree()
{
	CString strSql;
	int status=0;
	int rows=0;
	//	HTREEITEM root;
	//root=m_Tree.InsertItem("模型树",NULL);

	
	strSql="select * from "+m_Table;
	m_db.Query(strSql);
	rows=m_db.getRows();
	if(rows==0) return;
	m_db.MoveBegin();
	ModelStruct ms;
	ModelList.RemoveAll();
	for(int i=0;i<rows;i++)
	{
		ms.Number=m_db.getInt("Number");
		ms.FatherNum=m_db.getInt("TypeNum");
		ms.Name=m_db.getString("Name");
		ms.Type="mdl";
		ModelList.Add(ms);
		m_db.MoveNext();
	}


	strSql="select * from NAsmType";
	m_db.Query(strSql);
	rows=m_db.getRows();
	if(rows==0) return;
	m_db.MoveBegin();

	for(int i=0;i<rows;i++)
	{
		ms.Number=m_db.getInt("Number");
		ms.FatherNum=m_db.getInt("FatherNum");
		ms.Name=m_db.getString("Name");
		ms.Type="typ";
		ModelList.Add(ms);
		m_db.MoveNext();
	}

	AddTree(NULL,&ModelList,0);
}

void CCommonDlg::AddTree(HTREEITEM root,CArray<ModelStruct,ModelStruct> * List,int Fathernum)
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
					//AddTree(hitem,List,(*List)[i].Number);
					///AddTree(hitem,(*List)[i].Number);
				}	
		}
	}
}


void CCommonDlg::AddTree(HTREEITEM root,int Number)
{
	CString strSql;
	strSql.Format("select * from AsmInsInfoTable where AsmNum=%d",Number);
	m_db.Query(strSql);
	int rows;
	rows=m_db.getRows();
	if(rows==0) return;
	m_db.MoveBegin();
	int InsNum;
	HTREEITEM hitem;
	CString InsName,InsCode,str;
	for(int i=0;i<rows;i++)
	{
		InsNum=m_db.getInt("InsNum");
		InsName=m_db.getString("InsName");
		InsCode=m_db.getString("InsCode");
		str=InsCode+"("+InsName+")";
		hitem=m_Tree.InsertItem(str,2,2,root);
		m_Tree.SetItemData(hitem,InsNum);
		m_db.MoveNext();
	}
}


void CCommonDlg::AddToDimList(int ModelNum)
{
    m_List.DeleteAllItems();  
	m_ModelNum=ModelNum;
	CString strSql;
   int rows,status;
   strSql.Format("select * from NPartVariantTable where PrtNum=%d",ModelNum);
   status=m_db.Query(strSql);
   rows=m_db.getRows();
   if(rows==0) return;
   m_db.MoveBegin();
   CString str;
   for(int i=0;i<rows;i++)
   {
      str=m_db.getString("Name");
	  m_List.InsertItem(i,str);
	  str=m_db.getString("Value");
	  m_List.SetItemText(i,1,str);
	  str=m_db.getString("IdNum");
	  m_List.SetItemText(i,2,str);
	  m_db.MoveNext();
   }

}

void CCommonDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UINT uFlags=0;
	CPoint point;
	HTREEITEM hItem;
	point= GetCurrentMessage()->pt;///
	m_Tree.ScreenToClient(&point);///
	hItem =m_Tree.HitTest(point, &uFlags);
	int	i=0;
	CString name;
	int asmNum;
	if ( hItem&&(uFlags&TVHT_ONITEM) )
	{
		name=m_Tree.GetItemText(hItem);
		int start=0;
		int index=name.Find("(",start);
		if(index!=-1)
		{
			name=name.Mid(0,index);
		}
		else
		{
			asmNum=m_Tree.GetItemData(hItem);
			this->AddToDimList(asmNum);
		}
	}
	*pResult = 0;
}

void CCommonDlg::OnNMClickListDim(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i<m_List.GetItemCount();i++)
	{
		if ( m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			m_IdNum=m_List.GetItemText(i,2);
		}
	}
	*pResult = 0;
}

void CCommonDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int index;
	index=m_Combo.GetCurSel();
	if(index==CB_ERR) return;
	CString str;
	m_Combo.GetLBText(index,str);
	if(str=="零件模型")
	{
		m_Table="NPartManageTable";
	}
	else
		if(str=="装配模型")
		{
			m_Table="NAsmManageTable";
		}
	m_Tree.DeleteAllItems();
	this->AddTree();
}

void CCommonDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ProWindowRepaint(PRO_VALUE_UNUSED);

	CDialog::OnClose();
}
