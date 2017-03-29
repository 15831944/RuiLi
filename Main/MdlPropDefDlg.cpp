// MdlPropDefDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "MdlPropDefDlg.h"
#include "SelectMainPicDlg.h"
#include "UserNameDlg.h"
#include "MaterialDlg.h"

// MdlPropDefDlg 对话框

IMPLEMENT_DYNAMIC(MdlPropDefDlg, CDialog)
MdlPropDefDlg::MdlPropDefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MdlPropDefDlg::IDD, pParent)
	, m_Name(_T(""))
	, m_Code(_T(""))
	, m_TypeName(_T(""))
	, m_Person(_T(""))
	, m_Material(_T(""))
	, m_PicPath(_T(""))
	, m_SolidPath(_T(""))
	, m_DrawingPath(_T(""))
	, m_Note(_T(""))
{
}

MdlPropDefDlg::~MdlPropDefDlg()
{
}

void MdlPropDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_EDIT4, m_Code);
	DDX_Text(pDX, IDC_EDIT2, m_TypeName);
	DDX_Text(pDX, IDC_EDIT3, m_Person);
	DDX_Text(pDX, IDC_EDIT_MATERIAL, m_Material);
	DDX_Text(pDX, IDC_EDIT9, m_PicPath);
	DDX_Text(pDX, IDC_EDIT10, m_SolidPath);
	DDX_Text(pDX, IDC_EDIT_DRAW, m_DrawingPath);
	DDX_Text(pDX, IDC_EDIT8, m_Note);
}


BEGIN_MESSAGE_MAP(MdlPropDefDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON11, OnBnClickedButton11)
END_MESSAGE_MAP()


// MdlPropDefDlg 消息处理程序
/************************************************************************/
/* 设置简图                                                             */
/************************************************************************/
void MdlPropDefDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CSelectMainPicDlg dlg;
	dlg.SetMdl(mdl);
	if(dlg.DoModal()==IDOK)
	{
		//返回选中图片的全路径名
		m_PicPath=dlg.select_file;
		UpdateData(FALSE);
	}
}
/************************************************************************/
/* 点击确定之后的动作                                                   */
/************************************************************************/
void MdlPropDefDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UsrPartPorp * temp;
	if(m_Name=="") 
	{
		AfxMessageBox("名称不能为空！");
		return;
	}
    temp=SetMdlInfo(mdl);
	temp->Name=m_Name;
	temp->Note=m_Note;
	temp->pic_path=m_PicPath;
	temp->Person=m_Person;
	temp->Material=m_Material;

	CDialog::OnOK();
}

void MdlPropDefDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

BOOL MdlPropDefDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	// ProSolid mdl;
	//获得当前模型
	int   w_id=0;
	if (ProMdlCurrentGet((ProMdl*)&mdl)==PRO_TK_NO_ERROR)
	{
		InitInfo(mdl);
	}
	else if (ProWindowCurrentGet(&w_id)==PRO_TK_NO_ERROR)
	{
		if (ProWindowMdlGet(w_id,&mdl)==PRO_TK_NO_ERROR)
		{
			InitInfo(mdl);
		}
		else
		{
			AfxMessageBox("当前模型没有设置!");
			OnCancel();
			return FALSE;
		}
	}
	else 
	{
		AfxMessageBox("当前模型没有设置!");
		OnCancel();
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 初始化对话框上的数据                                                 */
/************************************************************************/
int MdlPropDefDlg::InitInfo(ProMdl solid)
{
	UsrPartPorp temp=GetMdlInfo(solid);
	m_Name=temp.Name;
	m_Code=temp.Code;
	m_TypeName=temp.TypeName;
	m_SolidPath=GetMdlFullFileName(solid);
	if(temp.bHaveDraw) m_DrawingPath=GetMdlFullFileName(temp.hDraw);
	m_Note=temp.Note;
	m_PicPath=temp.pic_path;
	m_Person=temp.Person;
	m_Material=temp.Material;
	UpdateData(FALSE);
	return 0;
}

void MdlPropDefDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}

void MdlPropDefDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此添加消息处理程序代码
	UsrRepaintWindow();
}

void MdlPropDefDlg::OnBnClickedButton4()
{
	UpdateData(TRUE);
	CUserNameDlg dlg;
	dlg.m_UserName=m_Person;
	if(dlg.DoModal()==IDOK)
	{
		m_Person=dlg.m_UserName;
		UpdateData(FALSE);
	}
}

void MdlPropDefDlg::OnBnClickedButton11()
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
