// RigidityDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "RigidityDlg.h"
#include "CommonDlg.h"

// CRigidityDlg �Ի���
int GetMdlNum();

IMPLEMENT_DYNAMIC(CRigidityDlg, CDialog)
CRigidityDlg::CRigidityDlg()
	: CDialog(CRigidityDlg::IDD)
	, m_d(0)
	, m_dd(0)
	, m_g(0)
	, m_n(0)
	, m_f1(0)
{
}

CRigidityDlg::~CRigidityDlg()
{
}

void CRigidityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_d, m_d);
	DDX_Text(pDX, IDC_EDIT_DD, m_dd);
	DDX_Text(pDX, IDC_EDIT_G, m_g);
	DDX_Text(pDX, IDC_EDIT_N, m_n);
	DDX_Text(pDX, IDC_EDIT_F1, m_f1);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CRigidityDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CRigidityDlg ��Ϣ�������

void CRigidityDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	double G=m_g,d=m_d,n=m_n,D=m_dd;
	double F1=m_f1=(G*d*d*d*d)/(8*n*D*D*D);
	UpdateData(FALSE);
}

void CRigidityDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    CRigidityDlg::OnCancel();
}

void CRigidityDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	if(text=="����˿�߾�")
	{
		textstr="����˿�߾�";
	}
	else if(text=="�����о�")
	{
		textstr="�����о�";
	}
	else if(text=="���ɲ��ϵ��б�ģ��")
	{
		textstr="���ɲ��ϵ��б�ģ��";
	}
	else if(text=="���ɵĹ���Ȧ��")
	{
		textstr="���ɵĹ���Ȧ��";
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

BOOL CRigidityDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->ReplaceDim();

	m_Combo.AddString("����˿�߾�");
	m_Combo.AddString("�����о�");
	m_Combo.AddString("���ɲ��ϵ��б�ģ��");
	m_Combo.AddString("���ɵĹ���Ȧ��");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CRigidityDlg::ReplaceDim()
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
		if(textStr=="����˿�߾�")
			m_d=textValue;
		else if(textStr=="�����о�")
			m_dd=textValue;
		else if(textStr=="���ɲ��ϵ��б�ģ��")
			m_g=textValue;
		else if(textStr=="���ɵĹ���Ȧ��")
			m_n=textValue;
		UpdateData(FALSE);
		
		m_db.MoveNext();
	}
	m_db.Close();

}


double  CRigidityDlg::GetDimValue(CString str,ProSolid solid)
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