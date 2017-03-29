// NewDlg.cpp : 实现文件
//新建对话框

#include "stdafx.h"
#include "Main.h"
#include "NewDlg.h"
#include <ProUtil.h> 

IMPLEMENT_DYNAMIC(CNewDlg, CDialog)
CNewDlg::CNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewDlg::IDD, pParent)
	, m_Name(_T(""))
	, m_Template(0)
	, m_radioFlag(0)
	, m_FileName(_T(""))
{
}

CNewDlg::~CNewDlg()
{
}

void CNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_CBIndex(pDX, IDC_COMBO1, m_Template);
	DDX_Control(pDX, IDC_RADIO4, m_RadioDrawing);
	DDX_Text(pDX, IDC_EDIT2, m_FileName);
}


BEGIN_MESSAGE_MAP(CNewDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CNewDlg 消息处理程序

void CNewDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
    
	if (m_FileName=="") {
		AfxMessageBox("请输入名称！");
		return;
	}
	if (m_radioFlag==1) {
		if(!mynewfileprt(m_FileName.GetBuffer()+'\0')) return;
	}
	if (m_radioFlag==3) {
		if (!mynewfileasm(m_FileName.GetBuffer()+'\0')) return;
	}
	if (m_radioFlag==4) {
		if (!mynewfiledwg(m_FileName.GetBuffer()+'\0')) return;
	}
	OnOK();
}

void CNewDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}
/************************************************************************/
/* 新建零件                                                             */
/************************************************************************/
bool CNewDlg::mynewfileprt(char  filename[])
{
	ProError			err;
	ProLine				buff;
	ProMdl				model;
	int					w_id;

	ProStringToWstring(buff,filename);
	err = ProSolidCreate(buff, PRO_PART, (ProSolid *)&model);

	if (err!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("文件已存在或文件名不合法!");
		return false;
	}

	if (err==PRO_TK_NO_ERROR) {
		ProMdlDisplay(model);
		ProMdlWindowGet(model,&w_id);
		ProWindowActivate(w_id);
	}
    SetMdlInfo(model)->Name=m_Name;
	return true;
}
BOOL CNewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	m_RadioDrawing.SetCheck(BST_CHECKED);
    m_radioFlag=4;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 点击组建                                                             */
/************************************************************************/
void CNewDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radioFlag=3;
}

void CNewDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radioFlag=1;
}

void CNewDlg::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radioFlag=4;
	//AfxMessageBox("4");
}

bool CNewDlg::mynewfileasm(char  filename[])
{
	ProError		err;
	ProLine			buff;
	ProMdl			model;
	int				w_id;


	ProStringToWstring(buff,filename);
	err = ProSolidCreate(buff,PRO_ASSEMBLY,(ProSolid *)&model);

	if (err!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("文件已存在或文件名不合法!");
		return false;
	}

	if (err==PRO_TK_NO_ERROR) {
		ProMdlDisplay(model);
		ProMdlWindowGet(model,&w_id);
		ProWindowActivate(w_id);
	}
	SetMdlInfo(model)->Name=m_Name;
	return true;
}

bool CNewDlg::mynewfiledwg(char  filename[])
{
	ProMdl				solid_mdl;
	ProMdldata			data;
	ProModel			model;
	ProName				dwg_template,name;
	ProDrawing			drawing;
	ProError			status;
	char				strtmp[PRO_PATH_SIZE];
    ProDwgcreateOptions option=PRODWGCREATE_DISPLAY_DRAWING|PRODWGCRTERR_TEMPLATE_USED ;
    ProDwgcreateErrs	errors;
	
	status=ProMdlCurrentGet(&solid_mdl);
    if (status!=PRO_TK_NO_ERROR) {
		AfxMessageBox("当前没有模型！请打开一个模型之后，在生成工程图！");
		return false;
    }
	status=ProMdlDataGet(solid_mdl,&data);
	ProWstringToString(strtmp,data.name);
	ProStringToWstring(model.name,strtmp);
	ProWstringToString(strtmp,data.type);
	ProStringToWstring(model.type,strtmp);
	//设置绘图模版名
	ProStringToWstring(dwg_template,"c_drawing");
	ProStringToWstring(name,filename);
	ProDrawingFromTmpltCreate(name,dwg_template,&model,option,&drawing,&errors);
	return true;
}

void CNewDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此添加消息处理程序代码
	UsrRepaintWindow();
}
