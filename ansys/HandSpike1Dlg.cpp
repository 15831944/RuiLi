// HandSpike1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "HandSpike1Dlg.h"
#include "CommonDlg.h"

// CHandSpike1Dlg �Ի���
int GetMdlNum();

IMPLEMENT_DYNAMIC(CHandSpike1Dlg, CDialog)
CHandSpike1Dlg::CHandSpike1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHandSpike1Dlg::IDD, pParent)
	, m_l1(0)
	, m_l2(0)
	, m_l3(0)
	, m_l(0)
{
}

CHandSpike1Dlg::~CHandSpike1Dlg()
{
}

void CHandSpike1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_L1, m_l1);
	DDX_Text(pDX, IDC_EDIT_L2, m_l2);
	DDX_Text(pDX, IDC_EDIT_L3, m_l3);
	DDX_Text(pDX, IDC_EDIT_L, m_l);
	//DDX_Control(pDX, IDC_pic3, m_handspike1);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CHandSpike1Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CHandSpike1Dlg ��Ϣ�������

void CHandSpike1Dlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
	double L1=m_l1,L2=m_l2,L3=m_l3;
	m_l=L1+L2-L3;
    UpdateData(FALSE);
}

void CHandSpike1Dlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
BOOL CHandSpike1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->ReplaceDim();

	m_Combo.AddString("�Ƹ�����״̬�ߴ�");
	m_Combo.AddString("�Ƹ˵ײ����׸Ƕ˾���");
	m_Combo.AddString("���������ľ��Ƹ˶�����ߴ�");
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//m_handspike1.SetFile("E:\\Program\\pic\\14.jpg");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CHandSpike1Dlg::OnBnClickedButton3()
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
	if(text=="�Ƹ�����״̬�ߴ�")
	{
		textstr="�Ƹ�����״̬�ߴ�";
	}
	else if(text=="�Ƹ˵ײ����׸Ƕ˾���")
	{
		textstr="�Ƹ˵ײ����׸Ƕ˾���";
	}
	else if(text=="���������ľ��Ƹ˶�����ߴ�")
	{
		textstr="���������ľ��Ƹ˶�����ߴ�";
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
void CHandSpike1Dlg::ReplaceDim()
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
		if(textStr=="�Ƹ�����״̬�ߴ�")
			m_l1=textValue;
		else if(textStr=="�Ƹ˵ײ����׸Ƕ˾���")
			m_l2=textValue;
		else if(textStr=="���������ľ��Ƹ˶�����ߴ�")
			m_l3=textValue;
		
		UpdateData(FALSE);

		m_db.MoveNext();
	}
	m_db.Close();

}

double  CHandSpike1Dlg::GetDimValue(CString str,ProSolid solid)
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

