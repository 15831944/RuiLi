// PropPage2.cpp : ʵ���ļ�
//���ܲ�����

#include "stdafx.h"
#include "Main.h"
#include "PropPage2.h"


// CPropPage2 �Ի���

IMPLEMENT_DYNAMIC(CPropPage2, CPropertyPage)
CPropPage2::CPropPage2()
	: CPropertyPage(CPropPage2::IDD)
	, m_PerfParaName(_T(""))
	, m_PerfParaValue(_T(""))
	, m_PerfParaNote(_T(""))
	, p_temp_UsrPerfParaTableList(NULL)
	, mdl(NULL)
	, m_index(-1)
	, m_iRadio(0)
{
}

CPropPage2::~CPropPage2()
{
}

void CPropPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_PerfParaName);
	DDX_Text(pDX, IDC_EDIT2, m_PerfParaValue);
	DDX_Text(pDX, IDC_EDIT3, m_PerfParaNote);
	DDX_Radio(pDX, IDC_RADIO1, m_iRadio);
	DDX_Control(pDX, IDC_EDIT4, m_MaxValEdit);
	DDX_Control(pDX, IDC_EDIT5, m_MinValEdit);
}


BEGIN_MESSAGE_MAP(CPropPage2, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
END_MESSAGE_MAP()


// CPropPage2 ��Ϣ�������

int CPropPage2::SetValue(ProMdl solid, CArray<UsrPerfParaTable,UsrPerfParaTable> * pp_temp_UsrPerfParaTableList)
{
	p_temp_UsrPerfParaTableList=pp_temp_UsrPerfParaTableList;
	mdl=solid;
	return 0;
}
/************************************************************************/
/* ��ʾ��ˢ���б��                                                     */
/************************************************************************/
int CPropPage2::SetList(void)
{
	int num=0;
	int indexNo=0;
	
	m_List.DeleteAllItems();
	for(int i=0;i<3;i++)
	{
		m_List.DeleteColumn(0);
	}
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,60);
	m_List.InsertColumn(1,"��ʼֵ",LVCFMT_LEFT,100);
	m_List.InsertColumn(2,"˵��",LVCFMT_LEFT,200);
	num=(int)p_temp_UsrPerfParaTableList->GetCount();
	for(int i=0;i<num;i++)
	{
		if(p_temp_UsrPerfParaTableList->GetAt(i).owner!=mdl) continue;
		if(p_temp_UsrPerfParaTableList->GetAt(i).isCheck!=1) continue;

		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,0,p_temp_UsrPerfParaTableList->GetAt(i).Name,0,0,0,i);
		m_List.SetItemText(indexNo,1,p_temp_UsrPerfParaTableList->GetAt(i).value);
		m_List.SetItemText(indexNo,2,p_temp_UsrPerfParaTableList->GetAt(i).Note);
	}
	m_index=-1;
	return 0;
}

BOOL CPropPage2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_iRadio=0;	
	m_MaxValEdit.EnableWindow(FALSE);
	m_MinValEdit.EnableWindow(FALSE);
	UpdateData(FALSE);
    SetList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ���                                                                  */
/************************************************************************/
void CPropPage2::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	UsrPerfParaTable	temp;
	if (!CheckInput(-1))
		return;
	temp.owner=mdl;
	temp.isCheck=1;
	temp.Name=m_PerfParaName;
	temp.Note=m_PerfParaNote;
	temp.value=m_PerfParaValue;
	if (m_iRadio==0)
		temp.Type="str";
	else
	{
		CString			maxCs;
		CString			minCs;		
		m_MaxValEdit.GetWindowText(maxCs);
		m_MinValEdit.GetWindowText(minCs);
		temp.Type="dbl";
		temp.value=m_PerfParaValue;
		temp.MaxValue=atof(maxCs);
		temp.MinValue=atof(minCs);
	}
   p_temp_UsrPerfParaTableList->Add(temp);
   SetList();
}
/************************************************************************/
/* �޸�                                                                 */
/************************************************************************/
void CPropPage2::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_index==-1) 
	{AfxMessageBox("����ѡ��һ�");return;}
	if(!CheckInput(m_index))
		return;
	p_temp_UsrPerfParaTableList->GetAt(m_index).Name=m_PerfParaName;
	p_temp_UsrPerfParaTableList->GetAt(m_index).value=m_PerfParaValue;
	p_temp_UsrPerfParaTableList->GetAt(m_index).Note=m_PerfParaNote;
	if (m_iRadio==0)
		p_temp_UsrPerfParaTableList->GetAt(m_index).Type="str";
	else
	{
		CString			maxCs;
		CString			minCs;		
		m_MaxValEdit.GetWindowText(maxCs);
		m_MinValEdit.GetWindowText(minCs);
		p_temp_UsrPerfParaTableList->GetAt(m_index).Type="dbl";
		p_temp_UsrPerfParaTableList->GetAt(m_index).value=m_PerfParaValue;
		p_temp_UsrPerfParaTableList->GetAt(m_index).MaxValue=atof(maxCs);
		p_temp_UsrPerfParaTableList->GetAt(m_index).MinValue=atof(minCs);
	}
	SetList();
}
/************************************************************************/
/*  ��������б��                                                      */
/************************************************************************/
void CPropPage2::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int			num=0;
	CString		maxCs;
	CString     minCs;
	num=(int)m_List.GetItemCount();
	for(int i=0;i<num;i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;
		m_index=(int)m_List.GetItemData(i);
		m_PerfParaName=p_UsrPerfParaTableList[m_index].Name;
		m_PerfParaValue=p_UsrPerfParaTableList[m_index].value;
		m_PerfParaNote=p_UsrPerfParaTableList[m_index].Note;
		if (p_UsrPerfParaTableList[m_index].Type=="str")
		{
			m_iRadio=0;
			m_MaxValEdit.EnableWindow(FALSE);
			m_MinValEdit.EnableWindow(FALSE);
			m_MaxValEdit.SetWindowText("");
			m_MinValEdit.SetWindowText("");
		}
		else
		{
			m_iRadio=1;
			m_MaxValEdit.EnableWindow(TRUE);
			m_MinValEdit.EnableWindow(TRUE);
			if (p_UsrPerfParaTableList[m_index].MaxValue==0 && p_UsrPerfParaTableList[m_index].MinValue==0)
			{
				maxCs="";
				minCs="";
			}	
			else
			{
				maxCs.Format("%f",p_UsrPerfParaTableList[m_index].MaxValue);
				minCs.Format("%f",p_UsrPerfParaTableList[m_index].MinValue);
			}
			m_MaxValEdit.SetWindowText(maxCs);
			m_MinValEdit.SetWindowText(minCs);
		}
		break;
	}
	UpdateData(FALSE);
	*pResult = 0;
}
/************************************************************************/
/* ���                                                                     */
/************************************************************************/
void CPropPage2::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_PerfParaName="";
	m_PerfParaValue="";
	m_PerfParaNote="";
	m_MinValEdit.SetWindowText("");
	m_MaxValEdit.SetWindowText("");
	m_index=-1;
	UpdateData(FALSE);
}
/************************************************************************/
/*  ɾ��                                                                     */
/************************************************************************/
void CPropPage2::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString temp;
	if(m_index==-1) 
	{
		AfxMessageBox("��ѡ��Ҫɾ�����");
		return;
	}
	temp.Format("ȷ��ɾ�����ܲ��������%s����",p_temp_UsrPerfParaTableList->GetAt(m_index).Name);
	if(AfxMessageBox(temp)!=IDOK) return;
	p_temp_UsrPerfParaTableList->RemoveAt(m_index,1);
	//���
	OnBnClickedButton4();
	
	//ˢ��
	SetList();
}

void CPropPage2::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	m_MaxValEdit.EnableWindow(TRUE);
	m_MinValEdit.EnableWindow(TRUE);
}

void CPropPage2::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_MaxValEdit.EnableWindow(FALSE);
	m_MinValEdit.EnableWindow(FALSE);
}
BOOL CPropPage2::CheckInput(int iEditItem=-1)
{
	CString		maxCs;
	CString		minCs;
	UpdateData(TRUE);
	if(m_PerfParaName=="")
	{
		AfxMessageBox("���������ƣ�");
		return FALSE;
	} 
	if(m_PerfParaValue=="")
	{
		AfxMessageBox("������ֵ��");
		return FALSE;
	} 
	//���ͬ��
	for(int i=0;i<p_temp_UsrPerfParaTableList->GetCount();i++)
	{ 
		if(p_temp_UsrPerfParaTableList->GetAt(i).owner!=mdl) continue;
		if(p_temp_UsrPerfParaTableList->GetAt(i).isCheck!=1) continue;
		if(i==iEditItem && iEditItem!=-1)					 continue;									
		if(m_PerfParaName==p_temp_UsrPerfParaTableList->GetAt(i).Name)
		{
			AfxMessageBox("��������ͬ����");
			return FALSE;
		}
	}
	//�����ֵ��
	if (m_iRadio==1)
	{
		if (!IsDouble(m_PerfParaValue))
		{
			AfxMessageBox("����ֵ���зǷ��ַ�,��������ȷ������!");
			return FALSE;
		}		
		m_MaxValEdit.GetWindowText(maxCs);
		m_MinValEdit.GetWindowText(minCs);
		if (maxCs.IsEmpty() || minCs.IsEmpty())
		{
			m_MaxValEdit.SetWindowText("");
			m_MinValEdit.SetWindowText("");
			return TRUE;
		}
		if (!IsDouble(maxCs)  || !IsDouble(minCs) || atof(maxCs)<atof(minCs))
		{
			AfxMessageBox("�����޵�ֵ����ȷ!");
			return FALSE;
		}
		if (atof(m_PerfParaValue)>atof(maxCs) || atof(m_PerfParaValue)<atof(minCs))
		{
			AfxMessageBox("��ʼֵ���������޵ķ�Χ��!");
			return FALSE;
		}
	} 
	return TRUE;
}