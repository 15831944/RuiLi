// MdlDefDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "MdlDefDlg.h"
#include "SelectMainPicDlg.h"
#include "UserNameDlg.h"
#include "MaterialDlg.h"
// CMdlDefDlg 对话框

IMPLEMENT_DYNAMIC(CMdlDefDlg, CDialog)
CMdlDefDlg::CMdlDefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMdlDefDlg::IDD, pParent)
	, m_Name(_T(""))
	, m_Code(_T(""))
	, m_Type(_T(""))
	, m_FileName(_T(""))
	, m_Creater(_T(""))
	, m_PicFile(_T(""))
	, m_Note(_T(""))
	, m_FilePath(_T(""))
	, mdl(NULL)
	, m_Material(_T(""))
	, m_DrawingPath(_T(""))
{
}

CMdlDefDlg::~CMdlDefDlg()
{
}

void CMdlDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_EDIT4, m_Code);
	DDX_Text(pDX, IDC_EDIT2, m_Type);
	DDX_Text(pDX, IDC_EDIT5, m_FileName);
	DDX_Text(pDX, IDC_EDIT9, m_PicFile);
	DDX_Text(pDX, IDC_EDIT8, m_Note);
	DDX_Text(pDX, IDC_EDIT10, m_FilePath);
	/*DDX_Text(pDX, IDC_EDIT11, m_DrawingPath);*/
	DDX_Text(pDX, IDC_EDIT3, m_Creater);
	DDX_Text(pDX, IDC_EDIT_MATERIAL, m_Material);
	DDX_Text(pDX, IDC_EDIT_DRAW, m_DrawingPath);
}


BEGIN_MESSAGE_MAP(CMdlDefDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_WM_MOVE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON11, OnBnClickedButton11)
END_MESSAGE_MAP()


BOOL CMdlDefDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
    // ProSolid mdl;
	//获得当前模型	
	ProMdlCurrentGet((ProMdl*)&mdl);
	//IsMdlInfoDefAndDef((ProSolid)mdl);
	InitInfo(mdl);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 初始化对话框的数据                                                   */
/************************************************************************/
int CMdlDefDlg::InitInfo(ProMdl solid)
{
	UsrPartPorp temp_prop;
	temp_prop=GetMdlInfo(solid);		
	m_Name=temp_prop.Name;
	m_Code=temp_prop.Code;
	m_Type=temp_prop.TypeName; 
	m_FileName=GetMdlFileName(solid);
	m_FilePath=GetMdlPath(solid);
	m_Note=temp_prop.Note;
	m_PicFile=temp_prop.pic_path;
	m_Creater=temp_prop.Person;
	if(temp_prop.bHaveDraw)  m_DrawingPath=GetMdlFullFileName(temp_prop.hDraw);
	UpdateData(FALSE);		
	return 0;
}
/************************************************************************/
/*  点击确定之后，把信息给结构                                          */
/************************************************************************/
void CMdlDefDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UsrPartPorp * temp_prop;
	temp_prop=GetMdlPInfo(mdl);
    if(m_Name=="") 
	{
		AfxMessageBox("名称不能为空！");
		return;
	}
	temp_prop->Name=m_Name;
	temp_prop->Note=m_Note;
	temp_prop->pic_path=m_PicFile;
	temp_prop->Person =m_Creater;
	temp_prop->Material=m_Material;
	CMdlDefDlg::OnOK();
}

void CMdlDefDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CMdlDefDlg::OnCancel();
}
/************************************************************************/
/* 选择简图                                                             */
/************************************************************************/
void CMdlDefDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CSelectMainPicDlg dlg;
	dlg.SetMdl(mdl);
	if(dlg.DoModal()==IDOK)
	{
		//返回选中图片的全路径名
		 m_PicFile=dlg.select_file;
		 UpdateData(FALSE);
	}
}

void CMdlDefDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	UsrRepaintWindow();
	// TODO: 在此添加消息处理程序代码
}
void CMdlDefDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
    UsrActivateCurrentWindow();
	CDialog::OnClose();
}

void CMdlDefDlg::OnBnClickedButton4()
{
	UpdateData(TRUE);
	CUserNameDlg dlg;
	dlg.m_UserName=m_Creater;
	if(dlg.DoModal()==IDOK)
	{
		m_Creater=dlg.m_UserName;
		UpdateData(FALSE);
	}
}

void CMdlDefDlg::OnBnClickedButton11()
{
	UpdateData(TRUE);
	CMaterialDlg dlg;
	dlg.m_ID=m_Material;
	if(dlg.DoModal()==IDOK)
	{
		m_Material=dlg.m_ID;
		UpdateData(FALSE);
	}
}
