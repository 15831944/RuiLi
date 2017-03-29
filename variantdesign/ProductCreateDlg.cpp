// ProductCreateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "ProductCreateDlg.h"
#include "StyleStandDlg.h"
#include "ProWindows.h"

//extern ProductManageInfo ProductManageInfo_struc;
// CProductCreateDlg 对话框

IMPLEMENT_DYNAMIC(CProductCreateDlg, CDialog)
CProductCreateDlg::CProductCreateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProductCreateDlg::IDD, pParent)
	, m_stylename(_T(""))
	, m_stylenum(_T(""))
	, m_stylecode(_T(""))
	, m_briefname(_T(""))
	, m_stylenote(_T(""))
	, m_manager(_T(""))
	, m_upmanager(_T(""))
	, m_stand(_T(""))
	, m_standnote(_T(""))
	, m_productnote(_T(""))
	, m_productname(_T(""))
	, m_productcode(_T(""))
{
}

CProductCreateDlg::~CProductCreateDlg()
{
}

void CProductCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STYLENAME, m_stylename);
	DDX_Text(pDX, IDC_EDIT_STYLENUM, m_stylenum);
	DDX_Text(pDX, IDC_EDIT_STYLECODE, m_stylecode);
	DDX_Text(pDX, IDC_EDIT_BRIEFNAME, m_briefname);
	DDX_Text(pDX, IDC_EDIT_STYLENOTE, m_stylenote);
	DDX_Text(pDX, IDC_EDIT_MANAGER, m_manager);
	DDX_Text(pDX, IDC_EDIT_UPMANAGER, m_upmanager);
	DDX_Text(pDX, IDC_EDIT_STAND, m_stand);
	DDX_Text(pDX, IDC_EDIT_STANDNOTE, m_standnote);
	DDX_Text(pDX, IDC_EDIT_PRODUCTNOTE, m_productnote);
	DDX_Text(pDX, IDC_EDIT_PRODUCTNAME, m_productname);
	DDX_Text(pDX, IDC_EDIT_MAPCODE, m_productcode);
}


BEGIN_MESSAGE_MAP(CProductCreateDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECTSTYLE, OnBnClickedButtonSelectstyle)
	ON_BN_CLICKED(IDC_BUTTON_SELSTAND, OnBnClickedButtonSelstand)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CProductCreateDlg 消息处理程序

void CProductCreateDlg::OnBnClickedButtonSelectstyle()
{
	// TODO: 在此添加控件通知处理程序代码
	CStyleStandDlg stylestanddlg;
	INT_PTR status;
	status=stylestanddlg.DoModal();
	if (status==IDOK)
	{
		//m_stylenum=stylestanddlg.Style;
		//m_stylename=stylestanddlg.StyleName;
		//m_stylecode=stylestanddlg.StyleCode;
		//m_briefname=stylestanddlg.StyleBriefName;
		//m_stylenote=stylestanddlg.StyleNote;
		//m_manager=stylestanddlg.Designer;
		//m_upmanager=stylestanddlg.Manager;
		UpdateData(FALSE);
	}
	else
		if (status==IDCANCEL)
		{
			/*ProductManageInfo_struc.Style="";
			ProductManageInfo_struc.StyleName="";
			ProductManageInfo_struc.StyleCode="";
			ProductManageInfo_struc.StyleBriefName="";
			ProductManageInfo_struc.StyleNote="";
			ProductManageInfo_struc.Designer="";
			ProductManageInfo_struc.Manager="";
			ProductManageInfo_struc.Stand="";
			ProductManageInfo_struc.StandNote="";*/  
			return;
		}

}

void CProductCreateDlg::OnBnClickedButtonSelstand()
{
	// TODO: 在此添加控件通知处理程序代码
	CStyleStandDlg stylestanddlg;
	INT_PTR status;
	status=stylestanddlg.DoModal();
	if (status==IDOK)
	{
		/*m_stylenum=ProductManageInfo_struc.Style;
		m_stylename=ProductManageInfo_struc.StyleName;
		m_stylecode=ProductManageInfo_struc.StyleCode;
		m_briefname=ProductManageInfo_struc.StyleBriefName;
		m_stylenote=ProductManageInfo_struc.StyleNote;
		m_manager=ProductManageInfo_struc.Designer;
		m_upmanager=ProductManageInfo_struc.Manager;
		m_stand=ProductManageInfo_struc.Style;
		m_standnote=ProductManageInfo_struc.StandNote;*/
		//m_stylenum=stylestanddlg.Style;
		//m_stylename=stylestanddlg.StyleName;
		//m_stylecode=stylestanddlg.StyleCode;
		//m_briefname=stylestanddlg.StyleBriefName;
		//m_stylenote=stylestanddlg.StyleNote;
		//m_manager=stylestanddlg.Designer;
		//m_upmanager=stylestanddlg.Manager;
		//m_stand=stylestanddlg.Style;
		//m_standnote=stylestanddlg.StandNote;
		UpdateData(FALSE);
	}
	else
		if (status==IDCANCEL)
		{
			/*ProductManageInfo_struc.Style="";
			ProductManageInfo_struc.StyleName="";
			ProductManageInfo_struc.StyleCode="";
			ProductManageInfo_struc.StyleBriefName="";
			ProductManageInfo_struc.StyleNote="";
			ProductManageInfo_struc.Designer="";
			ProductManageInfo_struc.Manager="";
			ProductManageInfo_struc.Stand="";
			ProductManageInfo_struc.StandNote="";*/
			return;
		}
}

void CProductCreateDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRepaint(PRO_VALUE_UNUSED);
	// TODO: 在此添加消息处理程序代码
}

void CProductCreateDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_stylecode=="" || m_stylename=="" || m_stylenum=="" || m_briefname=="" || m_manager=="" ||
		m_upmanager=="" || m_stand=="" || m_productname=="" || m_productcode=="")
	{
		AfxMessageBox("你的信息不完全！");
		return;
	}
	else
	{
		//ProductManageInfo_struc.Style=m_stylenum;
		//ProductManageInfo_struc.StyleName=m_stylename;
		//ProductManageInfo_struc.StyleCode=m_stylecode;
		//ProductManageInfo_struc.StyleBriefName=m_briefname;
		//ProductManageInfo_struc.StyleNote=m_stylenote;
		//ProductManageInfo_struc.Designer=m_manager;
		//ProductManageInfo_struc.Manager=m_upmanager;
		//ProductManageInfo_struc.Stand=m_stand;
		//ProductManageInfo_struc.StandNote=m_standnote;
		//ProductManageInfo_struc.ProductDwgCode=m_productcode;
		//ProductManageInfo_struc.ProductName=m_productname;
		//ProductManageInfo_struc.ProductNote=m_productnote;
	}
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CProductCreateDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
