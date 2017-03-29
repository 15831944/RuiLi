// PoleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "PoleDlg.h"
#include "CommonDlg.h"
#define _USE_MATH_DEFINES
#include <math.h>

// CPoleDlg 对话框
int GetMdlNum();

IMPLEMENT_DYNAMIC(CPoleDlg, CDialog)
CPoleDlg::CPoleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPoleDlg::IDD, pParent)
	, m_d(0)
	, m_P(0)
	, m_n(0)
	, m_M(0)
	, m_T(0)
	, m_t(0)
	, m_G(0)
	, m_a(0)
{
}

CPoleDlg::~CPoleDlg()
{
}

void CPoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_d);
	DDX_Text(pDX, IDC_EDIT2, m_P);
	DDX_Text(pDX, IDC_EDIT4, m_n);
	DDX_Text(pDX, IDC_EDIT5, m_M);
	DDX_Text(pDX, IDC_EDIT3, m_T);
	DDX_Text(pDX, IDC_EDIT6, m_t);
	DDX_Text(pDX, IDC_EDIT7, m_G);
	DDX_Text(pDX, IDC_EDIT8, m_a);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CPoleDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
END_MESSAGE_MAP()


// CPoleDlg 消息处理程序

void CPoleDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double P=m_P,n=m_n,d=m_d;
	m_T=(9550000*P)/n;
	UpdateData(FALSE);
}

void CPoleDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double P=m_P,n=m_n,d=m_d;
	m_t=(9550000*P*16)/(n*M_PI*d*d*d);
	UpdateData(FALSE);
}

void CPoleDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double P=m_P,n=m_n,d=m_d,M=m_M,a=m_a;
	double T=m_T=(9550000*P)/n;
	m_G=sqrt(M*M+a*a*T*T)*32/(M_PI*d*d*d);
	UpdateData(FALSE);
}

void CPoleDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CPoleDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CPoleDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CPoleDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int index;
	index=m_Combo.GetCurSel();
	if(index==CB_ERR) return;
	CString text;
	m_Combo.GetLBText(index,text);
	CCommonDlg dlg;
	if(dlg.DoModal()!=IDOK) return;
	CString dlgstr,textstr,modelstr,dimstr,typestr;
	dlgstr="IDD_DIALOG_AREA";
	if(text=="杆件直径")
	{
		textstr="杆件直径";
	}
	
	modelstr.Format("%d",dlg.m_ModelNum);
	dimstr=dlg.m_IdNum;
	if(dlg.m_Table=="NPartManageTable")
		typestr="Part";
	else
		typestr="Asm";
	int status;
	status=m_db.Open(globl_connectString);
	if(status==0) return;
	CString strSql;
	strSql.Format("insert into cb_Relation (DialogID,TextID,ModelID,DimID,Type) values ('%s','%s','%s','%s','%s')",
		dlgstr,textstr,modelstr,dimstr,typestr);	
	status=m_db.Execute(strSql);
	m_db.Close();
}

BOOL CPoleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->ReplaceDim();

	m_Combo.AddString("杆件直径");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
void CPoleDlg::ReplaceDim()
{
	ProSolid solid;
	ProError error;
	error=ProMdlCurrentGet((ProMdl *)&solid);
	if(error!=PRO_TK_NO_ERROR) return;
	int MdlNum;
	MdlNum=GetMdlNum();
	if(MdlNum==-1) return;

	int status;
	int rows;
	status=m_db.Open(globl_connectString);
	if(status==0) return;
	CString strSql;
	CString partType="";
	ProMdlType p_type;
	error=ProMdlTypeGet(solid,&p_type);
	if(p_type==PRO_MDL_PART)
	{
		partType="Part";
	}
	if(p_type==PRO_MDL_ASSEMBLY)
	{
		partType="Asm";
	}

	CString dlgstr="IDD_DIALOG_AREA";
	strSql.Format("select * from cb_Relation where DialogID='%s' and ModelID='%d' and Type='%s'",dlgstr,MdlNum,partType);
	status=m_db.Query(strSql);
	rows=m_db.getRows();
	if(rows==0) return;

	CString textStr;
	CString Dimstr;
	double textValue;
	m_db.MoveBegin();
	for(int i=0;i<rows;i++)
	{
		textStr=m_db.getString("TextID");
		Dimstr=m_db.getString("DimID");
		textValue=GetDimValue(Dimstr,solid);
		if(textStr=="杆件直径")
			m_d=textValue;
		
		UpdateData(FALSE);

		m_db.MoveNext();
	}
	m_db.Close();

}

double CPoleDlg::GetDimValue(CString str,ProSolid solid)
{
	ProError status;
	ProModelitem owner;
	int index;
	index=str.Find("d",0);
	if(index!=0)
	{
		status=ProMdlToModelitem(solid,&owner);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("转换不成功!");
			return -1;
		}
		ProName name;  
		ProParamvalue pvalue; 
		ProParameter  param;

		ProStringToWstring(name,str.GetBuffer());
		status = ProParameterInit (&owner,name,&param);
		if (status!=PRO_TK_NO_ERROR)
		{
			//AfxMessageBox("无法得到参数");
			return -1;
		}
		status=ProParameterValueGet(&param,&pvalue);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("无法取得参数值!");
			return -1;
		}
		double m_value;
		m_value=pvalue.value.d_val;
		return m_value;
	}
	CString DimId=str.Mid(1);
	ProDimension dim;
	dim.type=PRO_DIMENSION;
	dim.id=atoi(DimId);
	dim.owner=(ProMdl)solid;
	ProError error;
	double value;
	error=ProDimensionValueGet(&dim,&value);
	if(error!=PRO_TK_NO_ERROR) return -1;
	return value;
}

