// WorkNumDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "WorkNumDlg.h"
#include "TreeDlg.h"
#include "CommonDlg.h"

// CWorkNumDlg 对话框
int GetMdlNum();

IMPLEMENT_DYNAMIC(CWorkNumDlg, CDialog)
CWorkNumDlg::CWorkNumDlg()
	: CDialog(CWorkNumDlg::IDD)
	, m_g(0)
	, m_d(0)
	, m_f(0)
	, m_ff(0)
	, m_dd(0)
	, m_n(0)
{
}

CWorkNumDlg::~CWorkNumDlg()
{
}

void CWorkNumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_G, m_g);
	DDX_Text(pDX, IDC_EDIT_d, m_d);
	DDX_Text(pDX, IDC_EDIT_f, m_f);
	DDX_Text(pDX, IDC_EDIT_FF, m_ff);
	DDX_Text(pDX, IDC_EDIT_DD, m_dd);
	DDX_Text(pDX, IDC_EDIT_N, m_n);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CWorkNumDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CWorkNumDlg 消息处理程序

void CWorkNumDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	double G=m_g,d=m_d,f=m_f,F=m_ff,D=m_dd;
	double n=m_n=(G*d*d*d*d*f)/(8*F*D*D*D);
	UpdateData(FALSE);

}


void CWorkNumDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CWorkNumDlg::OnBnClickedButton3()
{
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
	if(text=="弹簧材料的切变模量")
	{
		textstr="弹簧材料的切变模量";
	}
	else if(text=="线径")
	{
		textstr="线径";
	}
	else if(text=="弹簧变形") 
	{
        textstr="弹簧变形";
	}
	else if(text=="载荷")
	{
        textstr="载荷";
	}
	else 
	{
        textstr="弹簧中径";
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
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CWorkNumDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->ReplaceDim();

	m_Combo.AddString("弹簧材料的切变模量");
	m_Combo.AddString("线径");
	m_Combo.AddString("弹簧变形");
	m_Combo.AddString("载荷");
	m_Combo.AddString("弹簧中径");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CWorkNumDlg::ReplaceDim()
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
		if(textStr=="弹簧材料的切变模量")
			m_g=textValue;
		else if(textStr=="线径")
			m_d=textValue;
		else if(textStr=="弹簧变形")
			m_f=textValue;
		else if(textStr=="载荷")
			m_ff=textValue;
		else if(textStr=="弹簧中径")
			m_dd=textValue;
		UpdateData(FALSE);
		
		m_db.MoveNext();
	}
	m_db.Close();

}


double  CWorkNumDlg::GetDimValue(CString str,ProSolid solid)
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