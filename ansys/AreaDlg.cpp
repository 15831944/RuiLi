// AreaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "AreaDlg.h"
#include "CommonDlg.h"
#define _USE_MATH_DEFINES
#include <math.h>

// CAreaDlg �Ի���
int GetMdlNum();

IMPLEMENT_DYNAMIC(CAreaDlg, CDialog)
CAreaDlg::CAreaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAreaDlg::IDD, pParent)
	, m_diam1(0)
	, m_diam2(0)
	, m_s(0)
{
}

CAreaDlg::~CAreaDlg()
{
}

void CAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DIAM1, m_diam1);
	DDX_Text(pDX, IDC_EDIT_DIAM2, m_diam2);
	DDX_Text(pDX, IDC_EDIT_AREA, m_s);
	//DDX_Control(pDX, IDC_PIC1, m_pic);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
}


BEGIN_MESSAGE_MAP(CAreaDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CAreaDlg ��Ϣ�������

void CAreaDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	double D1=m_diam1,D2=m_diam2;
    m_s=(D1*D1+D2*D2+D1*D2)*M_PI/12;
	UpdateData(FALSE);
}

void CAreaDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL CAreaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_pic.SetFile("E:\\Program\\pic\\12.jpg");


	this->ReplaceDim();

	m_Combo.AddString("�˸���ĤƬѹ�ߵ�ֱ��");
	m_Combo.AddString("������Чֱ��");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CAreaDlg::OnBnClickedButton3()
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
	if(text=="�˸���ĤƬѹ�ߵ�ֱ��")
	{
		textstr="�˸���ĤƬѹ�ߵ�ֱ��";
	}
	else
	{
		textstr="������Чֱ��";
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




void CAreaDlg::ReplaceDim()
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
	  if(textStr=="�˸���ĤƬѹ�ߵ�ֱ��")
		  m_diam1=textValue;
	  else if(textStr=="������Чֱ��")
		  m_diam2=textValue;
	  UpdateData(FALSE);

	  m_db.MoveNext();
  }
  m_db.Close();

}


double  CAreaDlg::GetDimValue(CString str,ProSolid solid)
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

