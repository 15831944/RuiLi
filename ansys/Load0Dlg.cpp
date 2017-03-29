// Load0Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "Load0Dlg.h"
#include "CommonDlg.h"
#define _USE_MATH_DEFINES
#include <math.h>

// CLoad0Dlg 对话框
int GetMdlNum();

IMPLEMENT_DYNAMIC(CLoad0Dlg, CDialog)
CLoad0Dlg::CLoad0Dlg()
	: CDialog(CLoad0Dlg::IDD)
	, m_g(0)
	, m_d(0)
	, m_a(0)
	, m_r2(0)
	, m_r1(0)
	, m_n(0)
	, m_ri(0)
	, m_d1(0)
	, m_fi(0)
{
}

CLoad0Dlg::~CLoad0Dlg()
{
}

void CLoad0Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_g);
	DDX_Text(pDX, IDC_EDIT2, m_d);
	DDX_Text(pDX, IDC_EDIT3, m_a);
	DDX_Text(pDX, IDC_EDIT4, m_r2);
	DDX_Text(pDX, IDC_EDIT5, m_r1);
	DDX_Text(pDX, IDC_EDIT6, m_n);
	DDX_Text(pDX, IDC_EDIT7, m_ri);
	DDX_Text(pDX, IDC_EDIT8, m_d1);
	DDX_Text(pDX, IDC_EDIT9, m_fi);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CLoad0Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CLoad0Dlg 消息处理程序



void CLoad0Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CLoad0Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double G=m_g,d=m_d,a=m_a,R1=m_r1,R2=m_r2,n=m_n,i=m_ri,Ri;
	 m_d1=d*sqrt(1-(R2-R1)*(R2-R1)/ (n*n*d*d));
     Ri=R2-(R2-R1)*i/n;
	 m_fi=M_PI*G*d*d*d*d*(a-m_d1/(2*M_PI*Ri))/(32*Ri*Ri);
	UpdateData(FALSE);
}

void CLoad0Dlg::OnBnClickedButton3()
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
	if(text=="弹簧材料直径")
	{
		textstr="弹簧材料直径";
	}
	else if(text=="弹簧的螺旋角")
	{
		textstr="弹簧的螺旋角";
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
	else if(text=="接触的弹簧圈i的半径")
	{
		textstr="接触的弹簧圈i的半径";
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

BOOL CLoad0Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->ReplaceDim();

	m_Combo.AddString("弹簧材料直径");
	m_Combo.AddString("弹簧的螺旋角");
	m_Combo.AddString("弹簧的最大半径");
	m_Combo.AddString("弹簧的最小半径");
	m_Combo.AddString("弹簧的有效工作圈数");
	m_Combo.AddString("接触的弹簧圈i的半径");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
void CLoad0Dlg::ReplaceDim()
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
		if(textStr=="弹簧材料直径")
			m_d=textValue;
		else if(textStr=="弹簧的螺旋角")
			m_a=textValue;
		else if(textStr=="弹簧的最大半径")
			m_r2=textValue;
		else if(textStr=="弹簧的最小半径")
			m_r1=textValue;
		else if(textStr=="弹簧的有效工作圈数")
			m_n=textValue;
		else if(textStr=="接触的弹簧圈i的半径")
			m_ri=textValue;
		
		UpdateData(FALSE);
		
		m_db.MoveNext();
	}
	m_db.Close();

}


double  CLoad0Dlg::GetDimValue(CString str,ProSolid solid)
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


