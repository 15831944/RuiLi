// AsmPerfPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "AsmPerfPage.h"
//extern CArray<UsrPerfParaTable,UsrPerfParaTable> p_UsrPerfParaTableList;

// CAsmPerfPage �Ի���

IMPLEMENT_DYNAMIC(CAsmPerfPage, CPropertyPage)
CAsmPerfPage::CAsmPerfPage()
	: CPropertyPage(CAsmPerfPage::IDD)
	, m_PerfParaName(_T(""))
	, m_PerfParaValue(_T(""))
	, m_PerfParaNote(_T(""))
	,mdl(NULL)
	, m_index(-1)
	, m_iRadio(0)
{
}

CAsmPerfPage::~CAsmPerfPage()
{
}

void CAsmPerfPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, 1003, m_PerfParaName);
	DDX_Text(pDX, 1014, m_PerfParaValue);
	DDX_Text(pDX, IDC_EDIT3, m_PerfParaNote);
	DDX_Control(pDX, 1002, m_List);
	DDX_Radio(pDX, IDC_RADIO1, m_iRadio);
	DDX_Control(pDX, IDC_EDIT4, m_MaxValEdit);
	DDX_Control(pDX, IDC_EDIT5, m_MinValEdit);
}


BEGIN_MESSAGE_MAP(CAsmPerfPage, CPropertyPage)
	ON_BN_CLICKED(1004, OnBnClicked1004)
	ON_BN_CLICKED(1012, OnBnClicked1012)
	ON_BN_CLICKED(1005, OnBnClicked1005)
	ON_BN_CLICKED(1013, OnBnClicked1013)
	ON_NOTIFY(NM_CLICK, 1002, OnNMClick1002)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
END_MESSAGE_MAP()


// CAsmPerfPage ��Ϣ�������
/************************************************************************/
/* ���                                                                     */
/************************************************************************/
void CAsmPerfPage::OnBnClicked1004()
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
	p_UsrPerfParaTableList.Add(temp);
	SetList();
}
/************************************************************************/
/* �޸�                                                                     */
/************************************************************************/
void CAsmPerfPage::OnBnClicked1012()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_index==-1) 
	{AfxMessageBox("����ѡ��һ�");return;}
	if(!CheckInput(m_index))
		return;
	p_UsrPerfParaTableList[m_index].Name=m_PerfParaName;
	p_UsrPerfParaTableList[m_index].value=m_PerfParaValue;
	p_UsrPerfParaTableList[m_index].Note=m_PerfParaNote;
	if (m_iRadio==0)
		p_UsrPerfParaTableList[m_index].Type="str";
	else
	{
		CString			maxCs;
		CString			minCs;		
		m_MaxValEdit.GetWindowText(maxCs);
		m_MinValEdit.GetWindowText(minCs);
		p_UsrPerfParaTableList[m_index].Type="dbl";
		p_UsrPerfParaTableList[m_index].value=m_PerfParaValue;
		p_UsrPerfParaTableList[m_index].MaxValue=atof(maxCs);
		p_UsrPerfParaTableList[m_index].MinValue=atof(minCs);
	}
	SetList();
}
/************************************************************************/
/* ɾ��                                                                     */
/************************************************************************/
void CAsmPerfPage::OnBnClicked1005()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString temp;
	if(m_index==-1)
	{
		AfxMessageBox("��ѡ��Ҫɾ�����");
		return;
	}
	temp.Format("ȷ��ɾ�����ܲ��������%s����",p_UsrPerfParaTableList[m_index].Name);
	if(AfxMessageBox(temp)!=IDOK) return;
	p_UsrPerfParaTableList.RemoveAt(m_index,1);
	//���
	OnBnClicked1013();
	//ˢ��
	SetList();
}
/************************************************************************/
/* ��kong                                                                     */
/************************************************************************/
void CAsmPerfPage::OnBnClicked1013()
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

void CAsmPerfPage::OnNMClick1002(NMHDR *pNMHDR, LRESULT *pResult)
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

void CAsmPerfPage::SetList(void)
{
	int		num=0;
	int		indexNo=0;
	m_List.DeleteAllItems();
	do {		
	} while(m_List.DeleteColumn(0)==TRUE);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,60);
	m_List.InsertColumn(1,"��ʼֵ",LVCFMT_LEFT,100);
	m_List.InsertColumn(2,"˵��",LVCFMT_LEFT,200);
	num=(int)p_UsrPerfParaTableList.GetCount();
	for(int i=0;i<num;i++)
	{
		if(p_UsrPerfParaTableList[i].owner!=mdl) continue;
		if(p_UsrPerfParaTableList[i].isCheck!=1) continue;
		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,0,p_UsrPerfParaTableList[i].Name,0,0,0,i);
		m_List.SetItemText(indexNo,1,p_UsrPerfParaTableList[i].value);
		m_List.SetItemText(indexNo,2,p_UsrPerfParaTableList[i].Note);
	}

	m_index=-1;
}

BOOL CAsmPerfPage::OnInitDialog()
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
BOOL CAsmPerfPage::CheckInput(int iEditItem=-1)
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
	for(int i=0;i<p_UsrPerfParaTableList.GetCount();i++)
	{ 
		if(p_UsrPerfParaTableList[i].owner!=mdl) continue;
		if(p_UsrPerfParaTableList[i].isCheck!=1) continue;
		if(i==iEditItem && iEditItem!=-1)					 continue;									
		if(m_PerfParaName==p_UsrPerfParaTableList[i].Name)
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
void CAsmPerfPage::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_MaxValEdit.EnableWindow(FALSE);
	m_MinValEdit.EnableWindow(FALSE);
}

void CAsmPerfPage::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_MaxValEdit.EnableWindow(TRUE);
	m_MinValEdit.EnableWindow(TRUE);
}
