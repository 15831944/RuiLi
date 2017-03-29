// JointDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "JointDlg.h"
#include "CommonDlg.h"

// CJointDlg 对话框
int GetMdlNum();

IMPLEMENT_DYNCREATE(CJointDlg, CDialog)

CJointDlg::CJointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJointDlg::IDD, pParent)
	, m_F(0)
	, m_t(0)
	, m_l(0)
	, m_L(0)
	, m_K(0)
	, m_G1(0)
	, m_G2(0)
{
}

CJointDlg::~CJointDlg()
{
}

void CJointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_F);
	DDX_Text(pDX, IDC_EDIT3, m_t);
	DDX_Text(pDX, IDC_EDIT2, m_l);
	DDX_Text(pDX, IDC_EDIT4, m_L);
	DDX_Text(pDX, IDC_EDIT5, m_K);
	DDX_Text(pDX, IDC_EDIT6, m_G1);
	DDX_Text(pDX, IDC_EDIT7, m_G2);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}

BOOL CJointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->ReplaceDim();

	m_Combo.AddString("焊缝长度");
	m_Combo.AddString("焊脚长度");
	m_Combo.AddString("被焊件厚度");
	m_Combo.AddString("焊缝总长度");
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CJointDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CJointDlg)
	/*DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)*/
END_DHTML_EVENT_MAP()



// CJointDlg 消息处理程序

HRESULT CJointDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // 除非将焦点设置到控件，否则返回 TRUE
}

HRESULT CJointDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CJointDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double F=m_F,t=m_t,l=m_l;
	m_G1=F/(t*l);
	UpdateData(FALSE);
}

void CJointDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double F=m_F,K=m_K,L=m_L;
	m_G2=F/(0.7*K*L);
	UpdateData(FALSE);
}

void CJointDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CJointDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CJointDlg::OnBnClickedButton5()
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
	if(text=="焊缝长度")
	{
		textstr="焊缝长度";
	}
	else if(text=="焊脚长度")
	{
		textstr="焊脚长度";
	}
	else if(text=="被焊件厚度")
	{
		textstr="被焊件厚度";
	}
	else if(text=="焊缝总长度")
	{
		textstr="焊缝总长度";
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
void CJointDlg::ReplaceDim()
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
		if(textStr=="焊缝长度")
			m_l=textValue;
		else if(textStr=="焊脚长度")
			m_K=textValue;
		else if(textStr=="被焊件厚度")
			m_t=textValue;
		else if(textStr=="焊缝总长度")
			m_L=textValue;
		
		UpdateData(FALSE);

		m_db.MoveNext();
	}
	m_db.Close();

}

double  CJointDlg::GetDimValue(CString str,ProSolid solid)
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

