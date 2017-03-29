// WormDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "WormDlg.h"
#include "CommonDlg.h"
#define _USE_MATH_DEFINES
#include <math.h>

// CWormDlg �Ի���
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


// CWormDlg ��Ϣ�������

void CWormDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	double d1=m_d1,F=m_F;
	m_G1=(4*F)/(M_PI*d1*d1);
	UpdateData(FALSE);
}

void CWormDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	double d1=m_d1,F1=m_F1;
	m_G2=(4*F1*1.3)/(M_PI*d1*d1);
	UpdateData(FALSE);
}

void CWormDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	double d1=m_d1,F0=m_F0;
	m_G3=(4*F0*1.3)/(M_PI*d1*d1);
	UpdateData(FALSE);
}

void CWormDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	double d=m_d,Fs=m_Fs,m=m_m;
	m_t=(4*Fs)/(M_PI*d*d*m);
	UpdateData(FALSE);
}

void CWormDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	double d=m_d,Fs=m_Fs,h=m_h;
	m_G4=(4*Fs)/(d*h);
	UpdateData(FALSE);
}

void CWormDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CWormDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CWormDlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CWormDlg::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CWormDlg::OnBnClickedButton10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CWormDlg::OnBnClickedButton11()
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
	if(text=="����С��")
	{
		textstr="����С��";
	}
	else if(text=="˨����ױڼ�ѹ�����С�߶�")
	{
		textstr="˨����ױڼ�ѹ�����С�߶�";
	}
	else if(text=="��˨��������ֱ��")
	{
		textstr="��˨��������ֱ��";
	}
	else if(text=="��˨��������Ŀ")
	{
		textstr="��˨��������Ŀ";
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->ReplaceDim();

	m_Combo.AddString("����С��");
	m_Combo.AddString("˨����ױڼ�ѹ�����С�߶�h");
	m_Combo.AddString("��˨��������ֱ��");
	m_Combo.AddString("��˨��������Ŀ");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
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
		if(textStr=="����С��")
			m_d1=textValue;
		else if(textStr=="˨����ױڼ�ѹ�����С�߶�")
			m_h=textValue;
		else if(textStr=="��˨��������ֱ��")
			m_d=textValue;
		else if(textStr=="��˨��������Ŀ")
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

