// LoadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "LoadDlg.h"
#include "CommonDlg.h"

// CLoadDlg �Ի���
int GetMdlNum();

IMPLEMENT_DYNAMIC(CLoadDlg, CDialog)
CLoadDlg::CLoadDlg()
	: CDialog(CLoadDlg::IDD)
	, m_n(0)
	, m_dd(0)
	, m_d(0)
	, m_f(0)
	, m_g(0)
	, m_ff(0)
{
}

CLoadDlg::~CLoadDlg()
{
}

void CLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_N, m_n);
	DDX_Text(pDX, IDC_EDIT_DD, m_dd);
	DDX_Text(pDX, IDC_EDIT_d, m_d);
	DDX_Text(pDX, IDC_EDIT_f, m_f);
	DDX_Text(pDX, IDC_EDIT_G, m_g);
	DDX_Text(pDX, IDC_EDIT_FF, m_ff);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CLoadDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CLoadDlg ��Ϣ�������

void CLoadDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	double G=m_g,d=m_d,f=m_f,n=m_n,D=m_dd;
	double F=m_ff=(G*d*d*d*d*f)/(8*n*D*D*D);
	UpdateData(FALSE);
}



void CLoadDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CDialog::OnCancel();
}

BOOL CLoadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	this->ReplaceDim();

	m_Combo.AddString("���ɵ���ЧȦ��");
	m_Combo.AddString("���ɵ��о�");
	m_Combo.AddString("���ɲ��ϵ��߾�");
	m_Combo.AddString("���ɵı���");
	m_Combo.AddString("���ɲ��ϵ��б�ģ��");

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CLoadDlg::ReplaceDim()
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
		if(textStr=="���ɵ���ЧȦ��")
			m_n=textValue;
		else if(textStr=="���ɵ��о�")
			m_dd=textValue;
		else if(textStr=="���ɲ��ϵ��߾�")
			m_d=textValue;
		else if(textStr=="���ɵı���")
			m_f=textValue;
		else if(textStr=="���ɲ��ϵ��б�ģ��")
			m_g=textValue;
		UpdateData(FALSE);
		
		m_db.MoveNext();
	}
	m_db.Close();

}


double  CLoadDlg::GetDimValue(CString str,ProSolid solid)
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
			AfxMessageBox("ת�����ɹ�!");
			return -1;
		}
		ProName name;  
		ProParamvalue pvalue; 
		ProParameter  param;

		ProStringToWstring(name,str.GetBuffer());
		status = ProParameterInit (&owner,name,&param);
		if (status!=PRO_TK_NO_ERROR)
		{
			//AfxMessageBox("�޷��õ�����");
			return -1;
		}
		status=ProParameterValueGet(&param,&pvalue);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("�޷�ȡ�ò���ֵ!");
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


void CLoadDlg::OnBnClickedButton2()
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
	if(text=="���ɵ���ЧȦ��")
	{
		textstr="���ɵ���ЧȦ��";
	}
	else if(text=="���ɵ��о�")
	{
		textstr="���ɵ��о�";
	}
	else if(text=="���ɲ��ϵ��߾�")
	{
		textstr="���ɲ��ϵ��߾�";
	}
	else if(text=="���ɵı���")
	{
		textstr="���ɵı���";
	}
	else if(text=="���ɲ��ϵ��б�ģ��")
	{
		textstr="���ɲ��ϵ��б�ģ��";
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
