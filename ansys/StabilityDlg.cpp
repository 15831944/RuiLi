// StabilityDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "StabilityDlg.h"
#include "CommonDlg.h"
#define _USE_MATH_DEFINES
#include <math.h>

// CStabilityDlg 对话框
int GetMdlNum();

IMPLEMENT_DYNAMIC(CStabilityDlg, CDialog)
CStabilityDlg::CStabilityDlg()
	: CDialog(CStabilityDlg::IDD)
	, m_h0(0)
	, m_d(0)
	, m_u(0)
	, m_fc(0)
	, m_fcfc(0)
	, m_g1(0)
	, m_d1(0)
	, m_n(0)
{
}

CStabilityDlg::~CStabilityDlg()
{
}

void CStabilityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_H0, m_h0);
	DDX_Text(pDX, IDC_EDIT_d, m_d);
	DDX_Text(pDX, IDC_EDIT_U, m_u);
	DDX_Text(pDX, IDC_EDIT_fc, m_fc);
	DDX_Text(pDX, IDC_EDIT_FCFC, m_fcfc);
	DDX_Text(pDX, IDC_EDIT_G1, m_g1);
	DDX_Text(pDX, IDC_EDIT_D1, m_d1);
	DDX_Text(pDX, IDC_EDIT_N, m_n);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CStabilityDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
END_MESSAGE_MAP()


// CStabilityDlg 消息处理程序

void CStabilityDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double D=m_d,H0=m_h0,u=m_u,G=m_g1,d=m_d1,n=m_n;
	double fc=m_fc=0.813*H0*(1-sqrt(1-(6.85*D*D)/(u*u*H0*H0)));
	//double fcfc=m_fcfc=(G*d*d*d*d*fc)/(8*D*D*D*n);
	UpdateData(FALSE);
}

void CStabilityDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double D=m_d,H0=m_h0,u=m_u,G=m_g1,d=m_d1,n=m_n;
	//double fc=m_fc=0.813*H0*(1-sqrt(1-(6.85*D*D)/(u*u*H0*H0)));
	double fcfc=m_fcfc=(G*d*d*d*d*0.813*H0*(1-sqrt(1-(6.85*D*D)/(u*u*H0*H0))))/(8*D*D*D*n);
	UpdateData(FALSE);
}

void CStabilityDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
   CStabilityDlg::OnCancel();
}

void CStabilityDlg::OnBnClickedButton3()
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
	if(text=="弹簧自由高度")
	{
		textstr="弹簧自由高度";
	}
	else if(text=="弹簧中径")
	{
		textstr="弹簧中径";
	}
	else if(text=="弹簧材料的线径")
	{
		textstr="弹簧材料的线径";
	}
	else if(text=="弹簧的工作圈数")
	{
		textstr="弹簧的工作圈数";
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

BOOL CStabilityDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->ReplaceDim();

	m_Combo.AddString("弹簧自由高度");
	m_Combo.AddString("弹簧中径");
	m_Combo.AddString("弹簧材料的线径");
	m_Combo.AddString("弹簧的工作圈数");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CStabilityDlg::ReplaceDim()
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
		if(textStr=="弹簧自由高度")
			 m_h0=textValue;
		else if(textStr=="弹簧中径")
			m_d=textValue;
		else if(textStr=="弹簧材料的线径")
			m_d1=textValue;
		else if(textStr=="弹簧的工作圈数")
			m_n=textValue;
		UpdateData(FALSE);
		
		m_db.MoveNext();
	}
	m_db.Close();

}


double  CStabilityDlg::GetDimValue(CString str,ProSolid solid)
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


