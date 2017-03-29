// HandSpikeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "HandSpikeDlg.h"
#include "CommonDlg.h"
#define _USE_MATH_DEFINES

int GetMdlNum();
// CHandSpikeDlg �Ի���

IMPLEMENT_DYNAMIC(CHandSpikeDlg, CDialog)
CHandSpikeDlg::CHandSpikeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHandSpikeDlg::IDD, pParent)
	, m_l1(_T(""))
	, m_l2(_T(""))
	, m_l3(_T(""))
	, m_l(_T(""))
{
}

CHandSpikeDlg::~CHandSpikeDlg()
{
}

void CHandSpikeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_L1, m_l1);
	DDX_Text(pDX, IDC_EDIT_L2, m_l2);
	DDX_Text(pDX, IDC_EDIT_L3, m_l3);
	DDX_Text(pDX, IDC_EDIT_L, m_l);
	//DDX_Control(pDX, IDC_handspike, m_handspike);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CHandSpikeDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CHandSpikeDlg ��Ϣ�������

void CHandSpikeDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	double  L1=atof(m_l1);
	double  L2=atof(m_l2);
	double  L3=atof(m_l3);
	double  L=L1-L2+L3;
	m_l.Format("%0.3f",L);
	UpdateData(FALSE);
}

void CHandSpikeDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL CHandSpikeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
  //  m_handspike.SetFile("E:\\Program\\pic\\13.jpg");

	this->ReplaceDim();

	m_Combo.AddString("�˸��ڳ�");
	m_Combo.AddString("���Ӳ�ھ���");
	m_Combo.AddString("��¶�ߴ�");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CHandSpikeDlg::OnBnClickedButton3()
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
	dlgstr="IDD_DIALOG_HANDSPIKE";
	if(text=="�˸��ڳ�")
	{
		textstr="�˸��ڳ�";
	}
	else if(text=="���Ӳ�ھ���")
	{
		textstr="���Ӳ�ھ���";
	}
	else if(text=="��¶�ߴ�")
	{
		textstr="��¶�ߴ�";
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




void CHandSpikeDlg::ReplaceDim()
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
	CString dlgstr="IDD_DIALOG_HANDSPIKE";
	strSql.Format("select * from cb_Relation where DialogID='%s' and ModelID='%d'",dlgstr,MdlNum);
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
		if(textStr=="�˸��ڳ�")
		  m_l1.Format("%f",textValue);
		else if(textStr=="���Ӳ�ھ���")
		  m_l2.Format("%f",textValue);
		else if(textStr=="��¶�ߴ�")
		  m_l3.Format("%f",textValue);
		UpdateData(FALSE);

		m_db.MoveNext();
	}
	m_db.Close();

}


double  CHandSpikeDlg::GetDimValue(CString str,ProSolid solid)
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