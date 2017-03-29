// SaveModelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "SaveModelDlg.h"
#include "NewPartTypeDlg.h"

extern int globle_MdlNum;
// CSaveModelDlg �Ի���

IMPLEMENT_DYNAMIC(CSaveModelDlg, CDialog)
CSaveModelDlg::CSaveModelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveModelDlg::IDD, pParent)
	, m_Name(_T(""))
	, m_Path(_T(""))
	, m_Note(_T(""))
{
}

CSaveModelDlg::~CSaveModelDlg()
{
}

void CSaveModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SAVE, m_Tree);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_TYPE, m_Path);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_Note);
}


BEGIN_MESSAGE_MAP(CSaveModelDlg, CDialog)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_SAVE, OnNMRclickTreeSave)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_COMMAND(ID_NEWCAT, OnNewcat)
	ON_COMMAND(ID_DELCAT, OnDelcat)
	ON_COMMAND(ID_Refresh, OnRefresh)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_TREE_SAVE, OnNMClickTreeSave)
END_MESSAGE_MAP()


// CSaveModelDlg ��Ϣ�������

BOOL CSaveModelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	int status;
       status=m_db.Open(globl_connectString);
	m_ImageList.Create(16, 16, ILC_COLOR8, 0, 4);//
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	this->AddTree();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CSaveModelDlg::OnNMRclickTreeSave(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hItem;
	UINT uFlags=0;
	CPoint point;
	point= GetCurrentMessage()->pt;///
	////�Ҽ��˵�

	m_Tree.ScreenToClient(&point);///
	hItem =m_Tree.HitTest(point, &uFlags);
	if ( hItem&&(uFlags&TVHT_ONITEM) )
	{
		m_hitem=hItem;
		m_Tree.ClientToScreen(&point);
		CMenu contextMenu;
		CMenu *pMenu;
		contextMenu.LoadMenu(IDR_MENU1);
		pMenu=contextMenu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTALIGN,point.x,point.y,this);
	}

	*pResult = 0;
}

void CSaveModelDlg::OnNMClickTreeSave(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hItem;
	UINT uFlags=0;
	CPoint point;
	point= GetCurrentMessage()->pt;///
	////�Ҽ��˵�
	int number;
	CString type, FileDirectory;
	CString strSql;

	m_Tree.ScreenToClient(&point);///
	hItem =m_Tree.HitTest(point, &uFlags);
	if ( hItem&&(uFlags&TVHT_ONITEM) )
	{
	   number=(int)m_Tree.GetItemData(hItem);
	   if(number!=0)
	   {
		   strSql.Format("select * from FemPrt where Number=%d",number);
		   int rows;
		   rows=m_db.Query(strSql);
		   rows=m_db.getRows();
		   if(rows==0) return;
		   m_db.MoveBegin();
		   type=m_db.getString("Type");
		   FileDirectory=m_db.getString("FileDirectory");
		   if(type!="typ") return;
		   UpdateData(TRUE);
		   m_Path=FileDirectory;
		   m_FatherNum=number;
		   UpdateData(FALSE);

	   }


	}
	*pResult = 0;
}




void CSaveModelDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	ProError error;
	ProSolid rootSolid;
	error=ProMdlCurrentGet((ProMdl*)&rootSolid); //��õ�ǰģ��
	if (error!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�޷��õ���ǰģ��!");
		return ;
	}

	error=ProMdlSave(rootSolid);
	if (error!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�޷�����");
		return ;
	}
	ProMdldata  mdldata;
	error=ProMdlDataGet(rootSolid,&mdldata);
	CString str,name,path,type,host,device;
	int version;
	name=CString(mdldata.name);
	path=CString(mdldata.path);
	type=CString(mdldata.type);
	host=CString(mdldata.host);
	device=CString(mdldata.device);
	version=mdldata.version;
	CString fullpath;
	fullpath.Format("%s:%s%s.%s.%d",device,path,name.MakeLower(),type.MakeLower(),version);
	if(m_Path=="") return;
	//AfxMessageBox(fullpath);
	
	ftpfile.UploadFiletoDirectory(fullpath, m_Path, myFtp);
	CString exepath;
	exepath.Format("%s.%s.%d",name,type,version);

  
	ProModelitem owner;
	ProMdlToModelitem(rootSolid,&owner);
	ProName ppname;  
	ProParamvalue value; 
	ProParameter  param;
	CString strName="RL_MDL";
	ProStringToWstring(ppname,strName.GetBuffer());
	error = ProParameterInit (&owner,ppname,&param);
	error=ProParameterValueGet(&param,&value);
	globle_MdlNum=value.value.i_val;

	CString strSql;
	int FatherNum,ConnPrtMdl;
	CString Name,Type,FileDirectory,Note,DateTime;
	//Number="";
	FatherNum=m_FatherNum;
	ConnPrtMdl=globle_MdlNum;
	Name=m_Name;
	Type="mdl";
	FileDirectory=m_Path+"/"+exepath;
	Note=m_Note;
	COleDateTime   mydate=COleDateTime::GetCurrentTime();
	DateTime.Format("%d-%d-%d",mydate.GetYear(),mydate.GetMonth(),mydate.GetDay());
	
	strSql.Format("insert into FemPrt (FatherNum,Name,Type,ConnPrtMdlNum,FileDirectory,Note,DateTime) values (%d,'%s','%s',%d,'%s','%s','%s')",
		FatherNum,Name,Type,ConnPrtMdl,FileDirectory,Note,DateTime);
	int status;
	status=m_db.Execute(strSql);

	if(status==1)
	{
		AfxMessageBox("��ӳɹ�");
	}
	this->AddTree();
	//OnOK();
}



void CSaveModelDlg::AddTree()
{
	CString strSql;
	int status=0;
	int rows=0;
	m_Tree.DeleteAllItems();
    HTREEITEM root;
	root=m_Tree.InsertItem("����Ԫģ��",NULL);
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

void CSaveModelDlg::AddTree(HTREEITEM root,CArray<ModelStruct,ModelStruct> * List,int Fathernum)
{
	int num=(int)List->GetSize();
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



void CSaveModelDlg::OnNewcat()
{
	// TODO: �ڴ���������������
	CString str,strSql;
	 CString FileDirectory,strDate,Name,type;
	int number;
	number=(int)m_Tree.GetItemData(m_hitem);
	if(number!=0)
	{
	strSql.Format("select * from FemPrt where Number=%d",number);
	int rows;
	rows=m_db.Query(strSql);
	rows=m_db.getRows();
	if(rows==0) return;
	m_db.MoveBegin();
     Name=m_db.getString("Name");
	 type=m_db.getString("Type");
	FileDirectory=m_db.getString("FileDirectory");
	if(type!="typ") return;
	}
	else
	{
		Name="����Ԫģ��";
		type="typ";
		FileDirectory="/����Ԫģ��";
	}
	CNewPartTypeDlg dlg;
	dlg.m_FatherName=Name;
	if(dlg.DoModal()==IDOK)
	{
       Name=dlg.m_TypeName;
	   CString note;
	   note=dlg.m_TypeNote;
	   COleDateTime   mydate=COleDateTime::GetCurrentTime();
	   str.Format("%d-%d-%d",mydate.GetYear(),mydate.GetMonth(),mydate.GetDay());
       strDate=str;
	   int ConnPrtMdl=0;
	   FileDirectory=FileDirectory+"/"+Name;
	   strSql.Format("insert into FemPrt (FatherNum,Name,Type,ConnPrtMdlNum,FileDirectory,Note,DateTime) values (%d,'%s','%s',%d,'%s','%s','%s')",
		   number,Name,type,ConnPrtMdl,FileDirectory,note,strDate);
	  int status=m_db.Execute(strSql);
	  if(status==1)
	  {
		  this->AddTree();
		ftpfile.CreateDirectory(myFtp,FileDirectory);
	  }



	}


}

void CSaveModelDlg::OnDelcat()
{
	CString strSql;
	int number;
	number=(int)m_Tree.GetItemData(m_hitem);
	strSql.Format("select * from FemPrt where FatherNum=%d",number);
	int rows;
	rows=m_db.Query(strSql);
	rows=m_db.getRows();
	if(rows!=0)
	{
		AfxMessageBox("����ɾ��������µ�ģ�ͻ������");
		return;
	}
strSql.Format("select * from FemPrt where Number=%d",number);
m_db.Query(strSql);
rows=m_db.getRows();
if(rows==0)
{
	AfxMessageBox("��ǰ��𲻴��ڣ�");
	return;
}
m_db.MoveBegin();
CString FileDirectory;
FileDirectory=m_db.getString("FileDirectory");
BOOL result;
result=ftpfile.DeleteDirAndFile(FileDirectory,myFtp);
if(result==FALSE)
{
	AfxMessageBox("����ɾ���ļ���");
	return;
}
strSql.Format("delete from FemPrt where Number=%d",number);
int status;
status=m_db.Execute(strSql);
this->AddTree();
}

void CSaveModelDlg::OnRefresh()
{
	// TODO: �ڴ���������������
	this->AddTree();
}

void CSaveModelDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    m_db.Close();
	CDialog::OnClose();
}

