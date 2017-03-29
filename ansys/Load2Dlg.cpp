// Load2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "Load2Dlg.h"
#include "CommonDlg.h"

// CLoad2Dlg �Ի���
int GetMdlNum();

IMPLEMENT_DYNAMIC(CLoad2Dlg, CDialog)
CLoad2Dlg::CLoad2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoad2Dlg::IDD, pParent)
	, m_g(0)
	, m_d(0)
	, m_f(0)
	, m_r2(0)
	, m_r1(0)
	, m_n(0)
	, m_ff(0)
{
}

CLoad2Dlg::~CLoad2Dlg()
{
}

void CLoad2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_g);
	DDX_Text(pDX, IDC_EDIT2, m_d);
	DDX_Text(pDX, IDC_EDIT3, m_f);
	DDX_Text(pDX, IDC_EDIT4, m_r2);
	DDX_Text(pDX, IDC_EDIT5, m_r1);
	DDX_Text(pDX, IDC_EDIT6, m_n);
	DDX_Text(pDX, IDC_EDIT8, m_ff);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CLoad2Dlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CLoad2Dlg ��Ϣ�������

void CLoad2Dlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	double G=m_g,d=m_d,f=m_f,R2=m_r2,R1=m_r1,n=m_n;
	double m_ff=35*G*d*d*d*d*f/(64*n*(-5*(R2-R1)*(R2-R1)*(R2-R1)+21*R2*(R2-R1)*(R2-R1)+35*R2*R2*R1));
    UpdateData(FALSE);

}

void CLoad2Dlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CLoad2Dlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	double G=m_g,d=m_d,f=m_f,R2=m_r2,R1=m_r1,n=m_n;
	 m_ff=35*G*d*d*d*d*f/(64*n*(-5*(R2-R1)*(R2-R1)*(R2-R1)+21*R2*(R2-R1)*(R2-R1)+35*R2*R2*R1));
	UpdateData(FALSE);
}

BOOL CLoad2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->ReplaceDim();

	m_Combo.AddString("���ɲ��ϵ�ֱ��");
	m_Combo.AddString("���ɵı���");
	m_Combo.AddString("��󵯻�Ȧ�뾶");
	m_Combo.AddString("��С����Ȧ�뾶");
	m_Combo.AddString("���ɵ���Ч����Ȧ��");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CLoad2Dlg::OnBnClickedButton2()
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
	if(text=="���ɲ��ϵ�ֱ��")
	{
		textstr="���ɲ��ϵ�ֱ��";
	}
	else if(text=="���ɵı���")
	{
		textstr="���ɵı���";
	}
	else if(text=="��󵯻�Ȧ�뾶")
	{
		textstr="��󵯻�Ȧ�뾶";
	}
	else if(text=="��С����Ȧ�뾶")
	{
		textstr="��С����Ȧ�뾶";
	}
	else if(text=="���ɵ���Ч����Ȧ��")
	{
		textstr="���ɵ���Ч����Ȧ��";
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
void CLoad2Dlg::ReplaceDim()
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
		if(textStr=="���ɲ��ϵ�ֱ��")
			m_d=textValue;
		else if(textStr=="���ɵı���")
			m_f=textValue;
		else if(textStr=="��󵯻�Ȧ�뾶")
			m_r2=textValue;
		else if(textStr=="��С����Ȧ�뾶")
			m_r1=textValue;
		else if(textStr=="���ɵ���Ч����Ȧ��")
			m_n=textValue;
		
		UpdateData(FALSE);
		
		m_db.MoveNext();
	}
	m_db.Close();

}

double  CLoad2Dlg::GetDimValue(CString str,ProSolid solid)
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
