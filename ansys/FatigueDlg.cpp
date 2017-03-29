// FatigueDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "FatigueDlg.h"
#include "CommonDlg.h"
#define _USE_MATH_DEFINES
#include <math.h>
// CFatigueDlg 对话框
int GetMdlNum();

IMPLEMENT_DYNAMIC(CFatigueDlg, CDialog)
CFatigueDlg::CFatigueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFatigueDlg::IDD, pParent)
	, m_d1(0)
	, m_d2(0)
	, m_t(0)
	, m_F1(0)
	, m_F2(0)
	, m_t1(0)
	, m_t2(0)
	
	, m_S(0)
{
}

CFatigueDlg::~CFatigueDlg()
{
}

void CFatigueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_d1);
	DDX_Text(pDX, IDC_EDIT2, m_d2);
	DDX_Text(pDX, IDC_EDIT9, m_t);
	DDX_Text(pDX, IDC_EDIT4, m_F1);
	DDX_Text(pDX, IDC_EDIT5, m_F2);
	DDX_Text(pDX, IDC_EDIT6, m_t1);
	DDX_Text(pDX, IDC_EDIT7, m_t2);
	DDX_Text(pDX, IDC_EDIT8, m_S);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CFatigueDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL1, OnBnClickedButtonCancel1)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL2, OnBnClickedButtonCancel2)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
END_MESSAGE_MAP()


// CFatigueDlg 消息处理程序

void CFatigueDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double D=m_d1,d=m_d2,F1=m_F1,F2=m_F2,t=m_t;
	double C=D/d;
	double m_K=0.615/C+(4*C-1)/(4*C-4);
	m_t1=(8*m_K*m_F1*C)/(M_PI*d*d);
	UpdateData(FALSE);
}

void CFatigueDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double D=m_d1,d=m_d2,F1=m_F1,F2=m_F2,t=m_t;
	double C=D/d;
	double m_K=0.615/C+(4*C-1)/(4*C-4);
     m_t2=(8*m_K*m_F2*C)/(M_PI*d*d);
	UpdateData(FALSE);
}

void CFatigueDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double D=m_d1,d=m_d2,F1=m_F1,F2=m_F2,t=m_t;
	double C=D/d;
	double m_K=0.615/C+(4*C-1)/(4*C-4);
    double m_t1=(8*m_K*m_F1*C)/(M_PI*d*d);
	double m_t2=(8*m_K*m_F2*C)/(M_PI*d*d);
	m_S=(t+0.75*m_t2)/m_t1;
	UpdateData(FALSE);

}

void CFatigueDlg::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CFatigueDlg::OnBnClickedButtonCancel1()
{
	// TODO: 在此添加控件通知处理程序代码
    OnCancel();
}

void CFatigueDlg::OnBnClickedButtonCancel2()
{
	// TODO: 在此添加控件通知处理程序代码
    OnCancel();
}

void CFatigueDlg::OnBnClickedButton4()
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
	if(text=="弹簧中径")
	{
		textstr="弹簧中径";
	}
	else if(text=="弹簧丝的线径")
	{
		textstr="弹簧丝的线径";
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

BOOL CFatigueDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->ReplaceDim();

	m_Combo.AddString("弹簧中径");
	m_Combo.AddString("弹簧丝的线径");
		return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CFatigueDlg::ReplaceDim()
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
		if(textStr=="弹簧中径")
			m_d1=textValue;
		else if(textStr=="弹簧丝的线径")
			m_d2=textValue;
		
		UpdateData(FALSE);

		m_db.MoveNext();
	}
	m_db.Close();

}


double  CFatigueDlg::GetDimValue(CString str,ProSolid solid)
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

