// MdlRelVisitDlg.cpp : ʵ���ļ�
//���ڷ��ʵ�ǰģ�͵����й�ϵʽ
//���ڴӵ�ǰ��ϵ��ѡ������Ҫ�Ĺ�ϵ��ӵ����ݿ�

#include "stdafx.h"
#include "Main.h"
#include "MdlRelVisitDlg.h"
//ProError UsrSolidRelsetVisAc(ProRelset *p_relset,CArray<ProRelset,ProRelset>* caller_data);
//extern int DlgIcon;
// CMdlRelVisitDlg �Ի���

IMPLEMENT_DYNAMIC(CMdlRelVisitDlg, CDialog)
CMdlRelVisitDlg::CMdlRelVisitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMdlRelVisitDlg::IDD, pParent)
	,mdl(NULL)

{
}

CMdlRelVisitDlg::~CMdlRelVisitDlg()
{
}

void CMdlRelVisitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CMdlRelVisitDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CMdlRelVisitDlg ��Ϣ�������
/************************************************************************/
/* ���ģ�͵Ĺ�ϵʽ                                                     */
/************************************************************************/
bool CMdlRelVisitDlg::GetMdlRel(void)
{
    int num=0;
    //����ģ�͵Ĺ�ϵʽ
	if(ProSolidRelsetVisit(mdl,(ProSolidRelsetVisitAction)UsrSolidRelsetVisAc,
		(CArray<ProRelset,ProRelset>*)&p_ProRelSetList)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("���ʹ�ϵʽʧ�ܣ�");
		return false;
	}
		

    num=(int)p_ProRelSetList.GetCount();
	for(int i=0;i<num;i++)
	{
        ProWstring *w_array;                        //�ַ�������
		int n_lines;
	  	ProArrayAlloc(0,sizeof(ProWstring),1,(ProArray*)&w_array);
        ProRelsetRelationsGet(&p_ProRelSetList[i],&w_array);
		ProArraySizeGet((ProArray)w_array,&n_lines);
		for(int j=0;j<n_lines;j++)
		{
			CString temp;
			temp=CString(w_array[j]);
			//�ѿ��и��ų�
			if(temp=="") continue;
			rel_stringList.Add(temp);
		}
        ProWstringproarrayFree(w_array);

	}
	return true;
}

void CMdlRelVisitDlg::ShowMdlRel(void)
{
	int indexNo=0;
	int num=0;
	int no=0;
	CString temp;
	m_List.DeleteAllItems();
	num=(int)rel_stringList.GetCount();
	for(int i=0;i<num;i++)
	{
		temp.Format("%d",++no);
		indexNo=m_List.InsertItem(no-1,temp);
		m_List.SetItemText(indexNo,1,rel_stringList[i]);
		//�����ģ�����Ѿ����ڣ���ѡ��
		//3
		if(g_RelCollection.IsRelInCollection((ProSolid)mdl,rel_stringList[i]))
		{
			ListView_SetCheckState(m_List,i,TRUE);
		}
	}
}

void CMdlRelVisitDlg::SetMdl(ProMdl owner)
{
	mdl=owner;
}

BOOL CMdlRelVisitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_List.InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
	m_List.InsertColumn(1,"��ϵʽ",LVCFMT_LEFT,140,-1);
	if(!GetMdlRel()) return FALSE;

	ShowMdlRel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
//ProError UsrSolidRelsetVisAc(ProRelset *p_relset,CArray<ProRelset,ProRelset>* caller_data)
//{
//	
//	caller_data->Add(*p_relset);
//	return PRO_TK_NO_ERROR;
//	
//}

void CMdlRelVisitDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for(int i=0;i<(int)m_List.GetItemCount();i++)
	{
		if(!ListView_GetCheckState(m_List,i)) continue;
		rel_stringList_select.Add(m_List.GetItemText(i,1));
	}
	OnOK();
}

