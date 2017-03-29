// Load1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "Load1Dlg.h"
#include "CommonDlg.h"
#define _USE_MATH_DEFINES
#include <math.h>


// CLoad1Dlg 对话框
int GetMdlNum();

IMPLEMENT_DYNAMIC(CLoad1Dlg, CDialog)
CLoad1Dlg::CLoad1Dlg()
	: CDialog(CLoad1Dlg::IDD)
	, m_g(0)
	, m_d(0)
	, m_r2(0)
	, m_r1(0)
	, m_t(0)
	, m_ri(0)
	, m_fi(0)
	, m_n(0)
{
}

CLoad1Dlg::~CLoad1Dlg()
{
}

void CLoad1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_g);
	DDX_Text(pDX, IDC_EDIT2, m_d);
	DDX_Text(pDX, IDC_EDIT3, m_r2);
	DDX_Text(pDX, IDC_EDIT4, m_r1);
	DDX_Text(pDX, IDC_EDIT5, m_t);
	DDX_Text(pDX, IDC_EDIT6, m_ri);
	DDX_Text(pDX, IDC_EDIT7, m_fi);
	DDX_Text(pDX, IDC_EDIT8, m_n);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CLoad1Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_load1, OnBnClickedButtonload1)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CLoad1Dlg 消息处理程序


void CLoad1Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CLoad1Dlg::OnBnClickedButtonload1()
{
	UpdateData(TRUE);
	double G=m_g,d=m_d,t=m_t,R1=m_r1,R2=m_r2,n=m_n,i=m_ri,Ri;
	double d1=d*sqrt(1-(R2-R1)*(R2-R1)/(n*n*d*d));
	   Ri=R2-(R2-R1)*i/n;
     m_fi=G*d*d*d*d*(t-d1)/(64*Ri*Ri*Ri);
	UpdateData(FALSE);
}

BOOL CLoad1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->ReplaceDim();

	m_Combo.AddString("接触的弹簧圈的半径");
	m_Combo.AddString("弹簧材料的直径");
	m_Combo.AddString("弹簧的最大半径");
	m_Combo.AddString("弹簧的最小半径");
	m_Combo.AddString("弹簧的有效工作圈数");
	m_Combo.AddString("弹簧的节距");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CLoad1Dlg::OnBnClickedButton1()
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
	if(text=="接触的弹簧圈的半径")
	{
		textstr="接触的弹簧圈的半径";
	}
	else if(text=="弹簧材料的直径")
	{
		textstr="弹簧材料的直径";
	}
	else if(text=="弹簧的最大半径")
	{
		textstr="弹簧的最大半径";
	}
	else if(text=="弹簧的最小半径")
	{
		textstr="弹簧的最小半径";
	}
	else if(text=="弹簧的有效工作圈数")
	{
		textstr="弹簧的有效工作圈数";
	}
	else if("弹簧的节距")
	{
		textstr="弹簧的节距";
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
void CLoad1Dlg::ReplaceDim()
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
		if(textStr=="接触的弹簧圈的半径")
			m_fi=textValue;
		else if(textStr=="弹簧材料的直径")
			m_d=textValue;
		else if(textStr=="弹簧的最大半径")
			m_r2=textValue;
		else if(textStr=="弹簧的最小半径")
			m_r1=textValue;
		else if(textStr=="弹簧的有效工作圈数")
			m_n=textValue;
		else if(textStr=="弹簧的节距")
			m_t=textValue;
		UpdateData(FALSE);
		
		m_db.MoveNext();
	}
	m_db.Close();

}

double  CLoad1Dlg::GetDimValue(CString str,ProSolid solid)
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

