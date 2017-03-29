// WormDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "WormDlg.h"
#include "CommonDlg.h"
#define _USE_MATH_DEFINES
#include <math.h>

// CWormDlg 对话框
int GetMdlNum();

IMPLEMENT_DYNAMIC(CWormDlg, CDialog)
CWormDlg::CWormDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWormDlg::IDD, pParent)
	, m_d1(0)
	, m_F(0)
	, m_F1(0)
	, m_F0(0)
	, m_d(0)
	, m_Fs(0)
	, m_h(0)
	, m_G1(0)
	, m_G2(0)
	, m_G3(0)
	, m_t(0)
	, m_G4(0)
	, m_m(0)
{
}

CWormDlg::~CWormDlg()
{
}

void CWormDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_d1);
	DDX_Text(pDX, IDC_EDIT3, m_F);
	DDX_Text(pDX, IDC_EDIT5, m_F1);
	DDX_Text(pDX, IDC_EDIT2, m_F0);
	DDX_Text(pDX, IDC_EDIT4, m_d);
	DDX_Text(pDX, IDC_EDIT6, m_Fs);
	DDX_Text(pDX, IDC_EDIT7, m_h);
	DDX_Text(pDX, IDC_EDIT8, m_G1);
	DDX_Text(pDX, IDC_EDIT9, m_G2);
	DDX_Text(pDX, IDC_EDIT10, m_G3);
	DDX_Text(pDX, IDC_EDIT11, m_t);
	DDX_Text(pDX, IDC_EDIT12, m_G4);
	DDX_Text(pDX, IDC_EDIT13, m_m);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CWormDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnBnClickedButton11)
END_MESSAGE_MAP()


// CWormDlg 消息处理程序

void CWormDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double d1=m_d1,F=m_F;
	m_G1=(4*F)/(M_PI*d1*d1);
	UpdateData(FALSE);
}

void CWormDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double d1=m_d1,F1=m_F1;
	m_G2=(4*F1*1.3)/(M_PI*d1*d1);
	UpdateData(FALSE);
}

void CWormDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double d1=m_d1,F0=m_F0;
	m_G3=(4*F0*1.3)/(M_PI*d1*d1);
	UpdateData(FALSE);
}

void CWormDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double d=m_d,Fs=m_Fs,m=m_m;
	m_t=(4*Fs)/(M_PI*d*d*m);
	UpdateData(FALSE);
}

void CWormDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double d=m_d,Fs=m_Fs,h=m_h;
	m_G4=(4*Fs)/(d*h);
	UpdateData(FALSE);
}

void CWormDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CWormDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CWormDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CWormDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CWormDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CWormDlg::OnBnClickedButton11()
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
	if(text=="螺纹小径")
	{
		textstr="螺纹小径";
	}
	else if(text=="栓干与孔壁挤压面的最小高度")
	{
		textstr="栓干与孔壁挤压面的最小高度";
	}
	else if(text=="螺栓抗剪截面直径")
	{
		textstr="螺栓抗剪截面直径";
	}
	else if(text=="螺栓抗剪面数目")
	{
		textstr="螺栓抗剪面数目";
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

BOOL CWormDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->ReplaceDim();

	m_Combo.AddString("螺纹小径");
	m_Combo.AddString("栓干与孔壁挤压面的最小高度h");
	m_Combo.AddString("螺栓抗剪截面直径");
	m_Combo.AddString("螺栓抗剪面数目");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
void CWormDlg::ReplaceDim()
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
		if(textStr=="螺纹小径")
			m_d1=textValue;
		else if(textStr=="栓干与孔壁挤压面的最小高度")
			m_h=textValue;
		else if(textStr=="螺栓抗剪截面直径")
			m_d=textValue;
		else if(textStr=="螺栓抗剪面数目")
			m_m=textValue;
		
		UpdateData(FALSE);
		
		m_db.MoveNext();
	}
	m_db.Close();

}

double  CWormDlg::GetDimValue(CString str,ProSolid solid)
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

