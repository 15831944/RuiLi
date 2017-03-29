// GearDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "GearDlg.h"
#define _USE_MATH_DEFINES
#include <math.h>

// CGearDlg 对话框

IMPLEMENT_DYNAMIC(CGearDlg, CDialog)
CGearDlg::CGearDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGearDlg::IDD, pParent)
	, m_z(0)
	, m_m(0)
	, m_a(0)
	, m_x(0)
	, m_d(0)
	, m_Fr(0)
	, m_Fw(0)
	, m_Wk(0)
	, m_ffa(0)
	, m_fha(0)
	, m_k(0)
	, m_fpt(0)
	, m_Fpt(0)
	, m_hh(0)
{
}

CGearDlg::~CGearDlg()

{
}

void CGearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_z);
	DDX_Text(pDX, IDC_EDIT2, m_m);
	DDX_Text(pDX, IDC_EDIT3, m_a);
	DDX_Text(pDX, IDC_EDIT4, m_x);
	DDX_Text(pDX, IDC_EDIT5, m_d);
	DDX_Text(pDX, IDC_EDIT9, m_Fr);
	DDX_Text(pDX, IDC_EDIT11, m_Fw);
	DDX_Text(pDX, IDC_EDIT10, m_ffa);
	DDX_Text(pDX, IDC_EDIT12, m_fha);
	DDX_Text(pDX, IDC_EDIT14, m_k);
	DDX_Text(pDX, IDC_EDIT15, m_fpt);
	DDX_Text(pDX, IDC_EDIT16, m_Fpt);
}


BEGIN_MESSAGE_MAP(CGearDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CGearDlg 消息处理程序

void CGearDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CGearDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//double z=m_z,d=m_d,m=m_m,ha=m_ha,a=m_a,h=m_h,x=m_x,k=m_k;
	//m_k=z*acos(z*cos(a)/(z+2*x))/180+0.5;
	//m_Fr=0.24*m+1.0*sqrt(d)+5.6;
	//m_ffa=2.5*sqrt(m)+0.17*sqrt(d)+0.5;
	//m_fha=2*sqrt(m)+0.14*sqrt(d)+0.5;
	//m_Fw=2*x*m*sin(a);
	//m_Wk=m*cos(a)*(M_PI*(k-0.5)+z*(tan(a)-a))+2*x*m*sin(a);
	//m_fpt=0.3*(m+0.4*sqrt(d))+0.8*d
	//m_Fpt=0.3*(m+0.4*sqrt(d))+h+1.6*sqrt((k-1)*m)
    double z=m_z,d=m_d,m=m_m,a=m_a,x=m_x,k=m_k;
	m_d=z*m;
	m_Fr=0.24*m+1.0*sqrt(d)+5.6;
	m_ffa=2.5*sqrt(m)+0.17*sqrt(d)+0.5;
	m_fha=2*sqrt(m)+0.14*sqrt(d)+0.5;
	m_Fw=2*x*m*sin(a);
	//m_Wk=m*cos(a)*(M_PI*(k-0.5)+z*(tan(a)-a))+2*x*m*sin(a);
    m_fpt=0.3*(m+0.4*sqrt(d))+0.8*d;
	m_Fpt=0.3*(m+0.4*sqrt(d))+0.8*d+1.6*sqrt((k-1)*m);
	UpdateData(False);
}
