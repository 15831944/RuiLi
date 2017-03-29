// RelToMdl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "RelToMdl.h"


// CRelToMdl �Ի���

IMPLEMENT_DYNAMIC(CRelToMdl, CDialog)
CRelToMdl::CRelToMdl(CWnd* pParent /*=NULL*/)
	: CDialog(CRelToMdl::IDD, pParent)
	,mdl(NULL)
{
}

CRelToMdl::~CRelToMdl()
{
}

void CRelToMdl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CRelToMdl, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CRelToMdl ��Ϣ�������
/*ȷ���ѹ�ϵ��ӵ�ģ����
 *
 */
void CRelToMdl::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
	//CArray<ProWstring,ProWstring> w_array;    
	int num=0;   
	num=(int)m_List.GetItemCount();

	for(int i=0;i<num;i++)
	{
		if(!ListView_GetCheckState(m_List,i)) continue;
	    AddRelToMdl(mdl,m_List.GetItemText(i,1));
	}   

	OnOK();
}

BOOL CRelToMdl::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_List.InsertColumn(0,"���",LVCFMT_LEFT,40);
	m_List.InsertColumn(1,"��ϵʽ",LVCFMT_LEFT,140);
	ShowRel();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ������Ҫ��ʾ�Ĺ�ϵʽ                                                 */
/************************************************************************/
int CRelToMdl::SetRel(ProMdl solid/*,CArray<UsrRelTable,UsrRelTable>  *p_temp_UsrRelTalbe*/)
{
	//pp_UsrRelTable.Copy(*p_temp_UsrRelTalbe);
	mdl=solid;
	return 0;
}
/************************************************************************/
/* ������Ҫ��ʾ�Ĺ�ϵʽ                                                 */
/************************************************************************/
int CRelToMdl::SetRel(ProMdl solid,CArray<UsrRelTable,UsrRelTable>  *p_temp_UsrRelTalbe)
{
	//pp_UsrRelTable.Copy(*p_temp_UsrRelTalbe);
	mdl=solid;
	return 0;
}
/************************************************************************/
/* ��ʾ��ϵʽ                                                           */
/************************************************************************/
int CRelToMdl::ShowRel(void)
{
	int indexNo=0;	
	//int num=(int)pp_UsrRelTable.GetCount();
	CString				temp;
	CString				rel;
	m_List.DeleteAllItems();
	CArray<CRel,CRel&>  m_RelList;
	CArray<int,int&>	m_IndexList;
	g_RelCollection.GetSolidAllRel((ProSolid)mdl,true,&m_RelList,&m_IndexList);
	for(int i=0;i<m_RelList.GetCount();i++)
	{
		//////////////////////////////////////////////////////////////////////////		
		//if(pp_UsrRelTable[i].owner!=mdl) 
		//{
		//	//pp_UsrRelTable.RemoveAt(i,1);
		//	//num=num-1;
		//	continue;
		//}
		//if(pp_UsrRelTable[i].isCheck!=1) 
		//{
		//	//pp_UsrRelTable.RemoveAt(i,1);
		//	//num=num-1;
		//	continue;
		//}
		//temp.Format("%d",indexNo+1);
  //      m_List.InsertItem(indexNo++,temp);
		//m_List.SetItemText(indexNo-1,1,pp_UsrRelTable[i].rel);
		//////////////////////////////////////////////////////////////////////////
		temp.Format("%d",indexNo+1);
		GetMdlType(mdl)==PRO_MDL_PART?m_RelList[i].GetRelString(rel):m_RelList[i].GetRelStringAsComp(rel);		
		m_List.InsertItem(indexNo++,temp);
		m_List.SetItemText(indexNo-1,1,rel);
		//////////////////////////////////////////////////////////////////////////		
	}
	return 0;
}

int CRelToMdl::AddRelToMdl(ProMdl solid, CString rel)
{
	ProError status;
	ProLine w_line;
	ProWstring *w_array;
	int n_lines;
	ProRelset relset;
	wchar_t *w_ptr;
    ProCharLine c_w_line;
	ProModelitem modelitem;

	//��õ�ǰģ��
	//ProMdlCurrentGet((ProMdl*)&solid);
	//��õ�ǰģ����
	ProMdlToModelitem(solid, &modelitem);
	//ָ���������ʾ����Ϣ�ļ���

	//��ʾ�����ϵʽ����ʾ
    // PRO_LINE_SIZE
	//��������Ĺ�ϵʽ
    //Ϊ�����ϵʽ����������ڴ�
		status = ProArrayAlloc(0, sizeof(ProWstring), 1, (ProArray*)&w_array);
		//��õ�ǰģ�����ϵʽ����ָ��
		status = ProModelitemToRelset(&modelitem, &relset);		
		if (status == PRO_TK_NO_ERROR)
			//�����ǰģ�͵Ĺ�ϵʽ�����ڣ��õ���ϵʽ�ִ�����
			status = ProRelsetRelationsGet(&relset, &w_array);
		else
			//�����ǰģ�͵Ĺ�ϵʽ�������ڴ����µĹ�ϵʽ�����������ָ��
			status = ProRelsetCreate(&modelitem, &relset);
		sprintf(c_w_line,"%s",rel.GetString());
		ProStringToWstring(w_line,c_w_line);
		w_ptr = w_line;
		//���µĹ�ϵʽ��ӵ���ϵʽ�ִ�������
		status = ProArrayObjectAdd((ProArray*)&w_array, PRO_VALUE_UNUSED, 1,
			&w_ptr);
		//�õ���ϵʽ�ִ������Ԫ����Ŀ
		status = ProArraySizeGet((ProArray)w_array, &n_lines);
		//Ϊ��ϵʽ�������µĹ�ϵʽ
		status = ProRelsetRelationsSet(&relset, w_array, n_lines);
        status=ProRelsetRegenerate(&relset);
		if(status!=PRO_TK_NO_ERROR) AfxMessageBox("�޷���ӣ���ת��ģ���ڲ��Ĺ�ϵʽ���������ȷ�Ĺ�ϵʽ��");
		status = ProArrayFree((ProArray*)&w_array);
	
	
	return 0;
}
