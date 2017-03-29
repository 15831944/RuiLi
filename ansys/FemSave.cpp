// FemSave.cpp : 实现文件
//
#include "stdafx.h"
#include "ansys.h"
#include "FemSave.h"
#include "FtpFile.h"
#include "myproe.h"
#include "InputDialog.h"

extern int globle_MdlNum;
// CFemSave 对话框

IMPLEMENT_DYNAMIC(CFemSave, CDialog)
CFemSave::CFemSave(CWnd* pParent /*=NULL*/)
	: CDialog(CFemSave::IDD, pParent)
	, CConnprtMdlNum(_T(""))
	, CFatherNum(_T(""))
	, m_Path(_T(""))
{
}

CFemSave::~CFemSave()
{
}

void CFemSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, CName);
	DDX_Text(pDX, IDC_EDIT3, CType);
	DDX_Text(pDX, IDC_EDIT4, CConnprtMdlNum);
	DDX_Text(pDX, IDC_EDIT7, CFatherNum);
	DDX_Text(pDX, IDC_EDIT5, CDateTime);
	DDX_Text(pDX, IDC_EDIT6, CNote);
	DDX_Text(pDX, IDC_EDIT8, m_Path);
}


BEGIN_MESSAGE_MAP(CFemSave, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
//	ON_NOTIFY(NM_DBLCLK, IDC_TREE_SAVE, OnNMDblclkTreeSave)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_SAVE, OnNMDblclkTreeSave)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_SAVE, OnNMRclickTreeSave)
	ON_COMMAND(ID_NEWCAT, OnNewcat)
	ON_COMMAND(ID_DELCAT, OnDelcat)
	ON_COMMAND(ID_RENAMEFILE, OnRenamefile)
END_MESSAGE_MAP()


// CFemSave 消息处理程序

BOOL CFemSave::OnInitDialog()
{
	CDialog::OnInitDialog();

	int status=0;
	status=m_db.Open(globl_connectString);
	m_Tree.SubclassDlgItem(IDC_TREE_SAVE,this);
	m_Tree.SetConnect(myFtp.strFtp,myFtp.iFtpPort,myFtp.strFtpUser,myFtp.strFtpPwd);

  Refresh();
	// TODO:  在此添加额外的初始化
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CFemSave::Refresh()
{
	
	CString m_path="有限元模型";
	BOOL isCheck=TRUE;
	m_Tree.DisplayTree(m_path,isCheck);

}




bool CFemSave::CopyFilesToFtpServer(long MdlNumber, CString localDir,CString serDir)
{

	//CFtpFile ftp;
	//m_TextEdit.SetWindowText("正在保存模型");
	//m_prgpos=m_prgpos+5;
	//m_PrgCtrl.SetPos(m_prgpos);
	//ftp.CreateDirectory(myFtp,serDir);//
	////还没有实现
	//if(!ftp.UploadDirectorytoDirectory(localDir,serDir,myFtp))
	//{
	//	m_TextEdit.SetWindowText(_T("保存模型文件到服务器过程中失败！"));
	//	m_PrgCtrl.SetPos(0);
	//	return false;
	//}	
	//m_prgpos=m_prgpos+5;
	//m_PrgCtrl.SetPos(m_prgpos);
	return true;
}


void CFemSave::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

//void CFemSave::OnNMDblclkTreeSave(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	HTREEITEM hitem=m_Tree.GetSelectedItem();//
//	CString str;
//	str=m_Tree.GetFullPathEx(hitem);
//	AfxMessageBox(str);
//	*pResult = 0;
//}

void CFemSave::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
		UpdateData(TRUE);
	CFtpFile  ftpfile;
	ProError error;
	ProSolid rootSolid;
	error=ProMdlCurrentGet((ProMdl*)&rootSolid); //获得当前模型
	if (error!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("无法得到当前模型!");
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
    AfxMessageBox(fullpath);
	ftpfile.UploadFiletoDirectory(fullpath, m_Path, myFtp);

	CString strSql;
	int FatherNum,ConnPrtMdl;
	CString Name,Type,FileDirectory,Note,DateTime;
	//Number="";
	FatherNum=atoi(CFatherNum);
	ConnPrtMdl=globle_MdlNum;
	Name=CName;
	Type=CType;
	FileDirectory=m_Path;
	Note=CNote;
	DateTime=CDateTime;
	strSql.Format("insert into FemPrt (FatherNum,Name,Type,ConnPrtMdlNum,FileDirectory,Note,DateTime) values (%d,'%s','%s',%d,'%s','%s','%s')",
		FatherNum,Name,Type,ConnPrtMdl,FileDirectory,Note,DateTime);
	int status;
   status=m_db.Execute(strSql);

   if(status==1)
   {
	   AfxMessageBox("添加成功");
   }
   this->Refresh();
}






void CFemSave::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    m_db.Close();
	CDialog::OnClose();
}

void CFemSave::OnNMDblclkTreeSave(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	HTREEITEM hitem;
	hitem=m_Tree.GetSelectedItem();
    m_Path=m_Tree.GetFullPathEx(hitem);
	UpdateData(FALSE);
	*pResult = 0;
}




void CFemSave::OnNMRclickTreeSave(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hItem;
	UINT uFlags=0;
	CPoint point;
	point= GetCurrentMessage()->pt;///
	////右键菜单

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

void CFemSave::OnNewcat()
{
	// TODO: 在此添加命令处理程序代码
    CFtpFile ftpfile;
	CString strpath=m_Tree.GetFullPathEx(m_hitem);
	CString kpath;
	kpath.Format("%s/新建文件夹",strpath);
	ftpfile.CreateDirectory(myFtp,kpath);//
	m_Tree.ExpandItem(m_hitem,TVE_EXPAND);
}

void CFemSave::OnDelcat()
{
	// TODO: 在此添加命令处理程序代码
	CString kpath;
	kpath=m_Tree.GetFullPathEx(m_hitem);
	CFtpFile ftpfile;
	ftpfile.DeleteDirAndFile(kpath,myFtp);
	m_Tree.ExpandItem(m_hitem,TVE_EXPAND);
}

void CFemSave::OnRenamefile()
{
	// TODO: 在此添加命令处理程序代码
	CString name=m_Tree.GetItemText(m_hitem);
	CInputDialog idlg;
	CString newName;
	idlg.m_Text=name;
	if(idlg.DoModal()!=IDOK)
	{
		return;
	}
	newName=idlg.m_Text;
	if(newName=="") newName=name;
  CString oldpath,newPath;
  oldpath=m_Tree.GetFullPathEx(m_hitem);
  int index;
  index=oldpath.ReverseFind('/');
  newPath=oldpath.Mid(0,index+1)+newName;
  CFtpFile ftpfile;
 
  ftpfile.Rename(oldpath,newPath,myFtp);
  HTREEITEM parent;
  parent=m_Tree.GetParentItem(m_hitem);
	m_Tree.ExpandItem(parent,TVE_EXPAND);


}
