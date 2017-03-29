// RelEditDlg.cpp : ʵ���ļ�
//�Թ�ϵʽ�༭�ĶԻ�����Ϊ�ӶԻ���ʹ��

#include "stdafx.h"
#include "Main.h"
#include "RelEditDlg.h"
#include "RelToMdl.h"

//extern CArray<UsrRelTable,UsrRelTable>    p_UsrRelTableList; 
//extern int DlgIcon;
ProError SolidDimensionVisAct(ProDimension *dimension,ProError status,CStringArray * data);
ProError ParameterActionFn2(ProParameter *Param,ProError status,
						   CStringArray *p_pdata);
//
ProError ParameterFilterFn2(ProParameter *Param,ProError status,
						   CStringArray *p_pdata) ;
//BOOL CheckEquation(CString Expression,CStringArray * importDim,CStringArray * exportDim);
// CRelEditDlg �Ի���

IMPLEMENT_DYNAMIC(CRelEditDlg, CDialog)
CRelEditDlg::CRelEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRelEditDlg::IDD, pParent)
	,mdl(NULL)                                    //Ҫ���в�����ģ��
	, m_RelEdit(_T(""))
	, m_RelNoteEdit(_T(""))
	, iSelRelIndex(-1)
{
}

CRelEditDlg::~CRelEditDlg()
{
}

void CRelEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_RelEdit);
	DDX_Text(pDX, IDC_EDIT2, m_RelNoteEdit);
	DDX_Control(pDX, IDC_EDIT1, m_RelEditCtrl);
}


BEGIN_MESSAGE_MAP(CRelEditDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON12, OnBnClickedButton12)
	ON_BN_CLICKED(IDC_ADD_SYMBOL, OnBnClickedAddSymbol)
	ON_BN_CLICKED(IDC_MINUS_SYMBOL, OnBnClickedMinusSymbol)
	ON_BN_CLICKED(IDC_MULTIPLY_SYMBOL, OnBnClickedMultiplySymbol)
	ON_BN_CLICKED(IDC_DID_SYMBOL, OnBnClickedDidSymbol)
	ON_BN_CLICKED(IDC_BRACKET_SYMBOL, OnBnClickedBracketSymbol)
	ON_BN_CLICKED(IDC_QUEAL_SYMBOL, OnBnClickedQuealSymbol)
	ON_BN_CLICKED(IDC_BUTTON13, OnBnClickedButton13)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON14, OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON16, OnBnClickedButton16)
	ON_BN_CLICKED(IDC_QUEAL_SYMBOL2, OnBnClickedQuealSymbol2)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON15, OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON17, OnBnClickedButton17)
	ON_WM_CHAR()
END_MESSAGE_MAP()
// CRelEditDlg ��Ϣ�������

BOOL CRelEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    m_List.InsertColumn(0,"���",LVCFMT_LEFT,40);
    m_List.InsertColumn(1,"��ϵʽ",LVCFMT_LEFT,140);
	m_List.InsertColumn(2,"��ϵ��Ϣ",LVCFMT_LEFT,140);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    if(mdl!=NULL)  ShowRel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ������SetMdl(ProMdl owner)
˵�������ù�ϵ�༭�����༭��ģ��                                     */
/************************************************************************/
void CRelEditDlg::SetMdl(ProMdl owner)
{
	mdl=owner;
}
/************************************************************************/
/* ��ʾ��ϵʽ                                                           */
/************************************************************************/
void CRelEditDlg::ShowRel(void)
{

	//////////////////////////////////////////////////////////////////////////
	//ԭ����
	//int num=0;
	//int indexNo=0;
	//CString id="";
	//num=(int)p_UsrRelTableList.GetCount();
	//m_List.DeleteAllItems();
	//for(int i=0;i<num;i++)
	//{
	//	if(p_UsrRelTableList[i].owner!=mdl) continue;
	//	if(p_UsrRelTableList[i].isCheck!=1) continue;
 //       id.Format(" %d",m_List.GetItemCount()+1);
 //       indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_TEXT,(int)m_List.GetItemCount(),id,0,0,0,i);
	//	m_List.SetItemText(indexNo,1,p_UsrRelTableList[i].rel);
	//	m_List.SetItemText(indexNo,2,p_UsrRelTableList[i].note);
	//}
    //////////////////////////////////////////////////////////////////////////
    



	//////////////////////////////////////////////////////////////////////////
	//if (AfxMessageBox("�µ�",MB_OKCANCEL,-1 )==IDOK)
	//{
	//	num=(int)p_CRelItemList.GetCount();
	//	m_List.DeleteAllItems();
	//	for (int i=0;i<num;i++)
	//	{
	//		CString rel;
	//		if(p_CRelItemList[i].owner!=mdl) continue;
	//		p_CRelItemList[i].GetRelString(rel);
	//		 id.Format(" %d",m_List.GetItemCount()+1);
	//		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_TEXT,(int)m_List.GetItemCount(),id,0,0,0,i);
	//		m_List.SetItemText(indexNo,1,rel);
	//		m_List.SetItemText(indexNo,2,p_CRelItemList[i].GetRelNote());
	//	}
	//}
	//////////////////////////////////////////////////////////////////////////
    //3
	CArray<CRel,CRel&>	p_RelList;
	CArray<int,int&>	p_IndexList;
	CString				id="";
	CString				rel;
	int					indexNo=0;
	m_List.DeleteAllItems();
	g_RelCollection.GetSolidAllRel((ProSolid)mdl,false,&p_RelList,&p_IndexList);
	for (int i=0;i<p_RelList.GetCount();i++)
	{
		    id.Format(" %d",m_List.GetItemCount()+1);
			p_RelList[i].GetRelString(rel);
		    indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,(int)m_List.GetItemCount(),id,0,0,0,p_IndexList[i]);
			m_List.SetItemText(indexNo,1,rel);
			m_List.SetItemText(indexNo,2,p_RelList[i].GetRelNote());
	}
	//////////////////////////////////////////////////////////////////////////
	
	
}
/************************************************************************/
/* ��ϵʽ��Ϣ�༭                                                       */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	INT_PTR nRet = -1;
	UpdateData(TRUE);
	info_edit_dlg.m_note=m_RelNoteEdit;
	nRet=info_edit_dlg.DoModal();
	switch ( nRet )
	{
	case -1: 
		AfxMessageBox("�򿪱༭��ʧ�ܣ�");
		break;
	case IDABORT:
		// Do something
		break;
	case IDOK:
		m_RelNoteEdit=info_edit_dlg.m_note;
		UpdateData(FALSE);
		break;
	case IDCANCEL:
		// Do something
		break;
	default:
		// Do something
		break;
	};

}
/************************************************************************/
/* ��ӹ�ϵʽ������ϵʽ������                                           */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_RelEdit=="") 
	{
		AfxMessageBox("���������ϵʽ��");
		return;
	}


	//////////////////////////////////////////////////////////////////////////
	//ԭ��
	//for(int i=0;i<m_List.GetItemCount();i++)
	//{
	//	if(m_List.GetItemText(i,1)==m_RelEdit) 
	//	{
	//		AfxMessageBox("��ϵʽ�Ѿ����ڣ�");
	//		return;
	//	}
	//}
	//////����ϵʽ�Ƿ���ȷ��Ȼ�����,������������ţ��ߴ�Ͳ����Ƿ���ȷ��
	//UsrRelTable temp_usrreltable;
	//temp_usrreltable.owner=mdl;
	//temp_usrreltable.rel=m_RelEdit;
	//if(CheckRel(m_RelEdit)==false) return;
	//temp_usrreltable.note=m_RelNoteEdit;
	//temp_usrreltable.isCheck=1;
	//p_UsrRelTableList.Add(temp_usrreltable);
    //////////////////////////////////////////////////////////////////////////
    

	//CRel rel((ProSolid)mdl,m_RelEdit);
	//CString str;
	//rel.GetRelString(str);
	//AfxMessageBox(str);
    //////////////////////////////////////////////////////////////////////////
    //�µĹ�ϵʽ
	//bool    bolCheck;
 //   CRel    new_CRel((ProSolid)mdl,m_RelEdit,bolCheck);
 //   if (bolCheck==true)
 //   {
	//	for (int i=0;i<p_CRelItemList.GetCount();i++ )
	//	{
	//		if(p_CRelItemList[i]==new_CRel )
	//		{
	//			AfxMessageBox("��ϵʽ�Ѿ�����");
	//			return;
	//		}
	//	}
	//	p_CRelItemList.Add(new_CRel);
 //   }
	//else 
	//{
	//	AfxMessageBox(new_CRel.m_strError);
	//	return;
	//}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	//3.
	if (!g_RelCollection.AddRel((ProSolid)mdl,m_RelEdit,m_RelNoteEdit))
	{
		AfxMessageBox(g_RelCollection.error_info);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	

	ShowRel();
}
/************************************************************************/
/* �ڹ�ϵʽ�༭���м��롰������                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedAddSymbol()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RelEditCtrl.ReplaceSel("+",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* �ڹ�ϵʽ�༭���м��롰������                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedMinusSymbol()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    m_RelEditCtrl.ReplaceSel("-",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* �ڹ�ϵʽ�༭���м��롰x����                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedMultiplySymbol()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    m_RelEditCtrl.ReplaceSel("*",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* �ڹ�ϵʽ�༭���м��롰/����                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedDidSymbol()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    m_RelEditCtrl.ReplaceSel("/",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* �ڹ�ϵʽ�༭���м��롰()����                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedBracketSymbol()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    m_RelEditCtrl.ReplaceSel("()",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* �ڹ�ϵʽ�༭���м��롰=����                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedQuealSymbol()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    m_RelEditCtrl.ReplaceSel("=",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* �ڹ�ϵʽ�м��롰>��                                                 */
/************************************************************************/
void CRelEditDlg::OnBnClickedQuealSymbol2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RelEditCtrl.ReplaceSel(">",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* �ڹ�ϵʽ�м��롰<��                                                 */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RelEditCtrl.ReplaceSel("<",FALSE);
	m_RelEditCtrl.SetFocus();
}

/************************************************************************/
/* ɾ����ϵʽ                                                           */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//////////////////////////////////////////////////////////////////////////
	//�µ�
	//if (iSelRelIndex!=-1)
	//{
	//	p_CRelItemList.RemoveAt(iSelRelIndex);
	//	p_CRelItemList.FreeExtra();
	//	m_RelEdit="";
	//	m_RelNoteEdit="";
	//	UpdateData(FALSE);
	//	//ShowRel();
	//}
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	////ԭ��
	//if (iSelRelIndex!=-1)
	//{
	//	p_UsrRelTableList.RemoveAt(iSelRelIndex);
	//	p_UsrRelTableList.FreeExtra();
	//	m_RelEdit="";
	//	m_RelNoteEdit="";
	//	UpdateData(FALSE);
	//	iSelRelIndex=-1;
	//	ShowRel();
	//}
	//else
	//{
	//	AfxMessageBox("����ѡ��Ҫ�޸ĵĹ�ϵʽ");
	//}
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//3
	if (iSelRelIndex!=-1)
	{
		if (g_RelCollection.DelRel(iSelRelIndex))
		{
			iSelRelIndex=-1;
			ShowRel();
		}
		else
		{
			AfxMessageBox(g_RelCollection.error_info);
		}
	}
	else
	{
		AfxMessageBox("����ѡ��Ҫ�޸ĵĹ�ϵʽ");
	}
	//////////////////////////////////////////////////////////////////////////
	
}
/************************************************************************/
/* �����б��������ı༭���������Ӧ����Ϣ                           */
/************************************************************************/
void CRelEditDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		if(m_List.GetItemState(i, LVIS_SELECTED)!=LVIS_SELECTED) continue;
		m_RelEdit=m_List.GetItemText(i,1);
		m_RelNoteEdit=m_List.GetItemText(i,2);
		iSelRelIndex=(int)m_List.GetItemData(i);
		UpdateData(FALSE);
		break;
	}
	*pResult = 0;
}
/************************************************************************/
/* ����ģ���еĹ�ϵʽ��ѡ�����                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton14()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int num=0;
	int flag=0;
    CMdlRelVisitDlg mdl_rel_vis_dlg;
	mdl_rel_vis_dlg.SetMdl(mdl);
	if(mdl_rel_vis_dlg.DoModal()!=IDOK) return;
	num=(int)mdl_rel_vis_dlg.rel_stringList_select.GetCount();
	for(int i=0;i<num;i++)
	{
		//ԭ��
		//////////////////////////////////////////////////////////////////////////		
		//UsrRelTable temp;
		//temp.owner=mdl;
  //      if(CheckRel(mdl_rel_vis_dlg.rel_stringList_select[i])==false) continue;
	 //  //�ж��Ƿ��Ѿ�����
		//for(int j=0;j<m_List.GetItemCount();j++)
		//{
		//	if(m_List.GetItemText(j,1)==mdl_rel_vis_dlg.rel_stringList_select[i])  flag=1;
		//}
		//if(flag==1)
		//{
		//	flag=0;
		//	continue;
		//}
		//temp.rel=mdl_rel_vis_dlg.rel_stringList_select[i];
		//temp.isCheck=1;
		//temp.note="";
  //      p_UsrRelTableList.Add(temp);
		//////////////////////////////////////////////////////////////////////////
  


		//////////////////////////////////////////////////////////////////////////
		////�µ�
		//bool    bolCheck;
		//CRel    new_CRel((ProSolid)mdl,mdl_rel_vis_dlg.rel_stringList_select[i],bolCheck);
		//if (bolCheck==true)
		//{
		//	flag=0;
		//	for (int j=0;j<p_CRelItemList.GetCount();j++ )
		//	{
		//		if(p_CRelItemList[j]==new_CRel )
		//		{
		//			flag=1;
		//			break;
		//		}
		//	}
		//	if(flag==0) p_CRelItemList.Add(new_CRel);
		//}
		//else 
		//{
		//	AfxMessageBox(new_CRel.m_strError);
		//	continue;
		//}
		//////////////////////////////////////////////////////////////////////////
		//3
		g_RelCollection.AddRel((ProSolid)mdl,mdl_rel_vis_dlg.rel_stringList_select[i],"");
		//////////////////////////////////////////////////////////////////////////
		
		
	}
	ShowRel();
}
/************************************************************************/
/* ����ϵʽ�Ƿ���ȷ                                                   */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton16()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//m_RelEdit.Remove(' ');
	//ԭ��
	//////////////////////////////////////////////////////////////////////////	
	//if(CheckRel(m_RelEdit)==true)  AfxMessageBox("��ϵ��ȷ��");
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	bool    bolCheck;
	CRel    new_CRel((ProSolid)mdl,m_RelEdit,bolCheck);
	if(bolCheck) 
		AfxMessageBox("��ϵ��ȷ");
	else AfxMessageBox(new_CRel.m_strError);
	//////////////////////////////////////////////////////////////////////////
	
}
/************************************************************************/
/*���ڼ��û��ָ���Ĺ�ϵʽ�Ƿ���ȷ                                      */
/************************************************************************/
bool CRelEditDlg::CheckRel(CString rel)
{
    CString error;
	//�ȼ�����
   rel.Remove(' ');
   error.Append(CheckSymbol(rel));
   if(!error.IsEmpty()) 
   {AfxMessageBox(error);return false;}
   //��һ�����
   error.Append(CheckDimAndPara(rel));
   if(!error.IsEmpty()) {/*AfxMessageBox(error);*/return false;}
   return true;
}
/************************************************************************/
/*��飽 <  > >= <=,���м������ܹ�ֻ����һ��,�Լ�˳��                   */
/************************************************************************/
CString CRelEditDlg::CheckSymbol(CString rel)
{
	CString error;
	int a=0;
	int b=0;
	int c=0;
	int d=0;
	int e=0;
	int temp=0;
    int temp1=0;
	//��飽 > <��
	do {
		temp=rel.Find("=",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>1) error.AppendFormat("���ڶ��=�ţ�\n");

	temp=0;
	do {
		temp=rel.Find("<",temp);
		if(temp!=-1) b++,temp++;
	} while(temp!=-1);
	if(b>1) error.AppendFormat("���ڶ��<�ţ�\n");

	temp=0;
	do {
		temp=rel.Find(">",temp);
		if(temp!=-1) c++,temp++;
	} while(temp!=-1);
	if(c>1) error.AppendFormat("���ڶ��>�ţ�\n");

	if(a==1&&b==1&&(rel.Find("=<",0)!=-1)) error.AppendFormat("��=<�����Ų���ȷ��Ӧ��Ϊ��<=��\n");
	if(a==1&&b==1&&(rel.Find("<=",0)==-1)) error.AppendFormat("��<���롰=�����ֿܷ���\n");
	if(a==1&&c==1&&(rel.Find("=>",0)!=-1)) error.AppendFormat("��=>�����Ų���ȷ��Ӧ��Ϊ��>=��\n");
	if(a==1&&c==1&&(rel.Find(">=",0)==-1)) error.AppendFormat("��>���롰=�����ֿܷ���\n");
	//�������

	temp=0;
	temp1=0;
	d=0;
	e=0;
	do {
		temp=rel.Find("(",temp);

		if(temp!=-1) d++,temp++;
		//if(temp1!=-1) e++,temp1++;
	} while(temp!=-1);

	do {
		temp1=rel.Find(")",temp1);
		if(temp1!=-1) e++,temp1++;
	} while(temp1!=-1);

	if(e!=d) error.AppendFormat("��(���롰)��������ƥ�䣡\n");

	temp=0,temp1=0;
	a=0;
	//����Ƿ���˫��++ -- / / * * . . +- -+ +/ /+  *+ +* *- -* 00
    do {
    	temp=rel.Find("++",temp);
		if(temp!=-1) a++,temp++;
    } while(temp!=-1);
	if(a>=1) error.AppendFormat("���ڶ��+������һ��\n");
	temp=0;
	a=0;
	do {
		temp=rel.Find("--",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("���ڶ��-������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("////",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("���ڶ��////������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("**",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("���ڶ��**������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("..",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("���ڶ��.������һ��\n");
	
	temp=0;
	a=0;
	do {
		temp=rel.Find("+-",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("+-�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("-+",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("-+�Ų�������һ��\n");
    
	temp=0;
	a=0;
	do {
		temp=rel.Find("+//",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("+//�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("*+",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("*+�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("+*",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" +*�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("*-",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" *-�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("-*",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" -*�Ų�������һ��\n");

	//temp=0;
	//a=0;
	//do {
	//	temp=rel.Find("0",temp);
	//	if(temp!=-1) a++,temp++;
	//} while(temp!=-1);
	//if(a>1) error.AppendFormat(" 00�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("=-",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" =-�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("=+",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" =+�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("+=",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" +=�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("=.",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" =.�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find(".=",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" .=�Ų�������һ��\n");
   
	temp=0;
	a=0;
	do {
		temp=rel.Find("*=",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" *=�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("=*",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("=*�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("=//",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("=//�Ų�������һ��\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("//=",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("//=�Ų�������һ��\n");


	temp=0;
	CString str;
	str=rel.Left(1);
    temp=str.FindOneOf("+-//*=<>^");
	if(temp!=-1) error.AppendFormat("��һ���������������\n");
	temp=0;
	str=rel.Right(1);
	temp=str.FindOneOf("+-//*=<>^");
	if(temp!=-1) error.AppendFormat("��һ���������������\n");	

	return error;
}
/************************************************************************/
/* ���ߴ磬�����Ƿ����
   ���ߴ���������Ȼ�����еĳߴ�Ͳ�����Ȼ��������ϵ�еĳߴ�Ͳ�����
   ��������������ģ���У������ʹ��                                   */
/************************************************************************/
CString CRelEditDlg::CheckDimAndPara(CString rel)
{
	CStringArray dimpara;
	CStringArray dim;
	CStringArray para;
    CStringArray dimpara2;
	CString error;
	ProModelitem modelitem;
																		//�ֽ���ߴ磬����
																		//RelFenli(rel,&dimpara);
																		//�жϳߴ磬�����Ƿ����
	//1.���ʳߴ�
    ProSolidDimensionVisit((ProSolid)mdl,PRO_B_FALSE,(ProDimensionVisitAction)SolidDimensionVisAct,
		NULL,(CStringArray*)&dim);
    //2.���ʲ���	
	ProMdlToModelitem(mdl, &modelitem);
	ProParameterVisit(&modelitem,
		(ProParameterFilter)ParameterFilterFn2,
		(ProParameterAction )ParameterActionFn2 ,(CStringArray*)&para);  

	dimpara2.Append(dim);
	dimpara2.Append(para);	
	//dimpara2.Add(">=");
	//dimpara2.Add("<=");
	//�жϴӹ�ϵʽ�з��������
	if(!CheckEquation(rel,&dimpara2,&dimpara)) error=_T("����");
	//dimpara2.RemoveAll();dimpara2.FreeExtra();
	//error.Append(CompareCsarry1ToCsarry2(&dimpara,&dimpara2));
	return error;
}
/************************************************************************/
/*// ����һ����ϵʽ��������ߴ�Ͳ���                                   */
/************************************************************************/
void  CRelEditDlg::RelFenli(CString str,CStringArray * CanshuName)
{
	//CStringArray CanshuName;    //
	CString  resToken;
	BOOL     IsHave;
	int      curPos= 0;
	int      j=0;
	resToken=str.Tokenize("=+-*/()<>",curPos);
	while (resToken !="")
	{
		if(iswdigit(resToken.GetAt(0))==0)
		{
            int num=0;
			num=(int)CanshuName->GetSize();
			if(num==0)
			{
				CanshuName->Add(resToken);
			}
			else
			{
				IsHave=false;
				for(int i=0;i<num;i++)
				{
					if(CanshuName->GetAt(i)==resToken)
					{
						IsHave=true;
						break;
					}
				}
				if (!IsHave) 
				{
					CanshuName->Add(resToken); 
				}
			}
				
		}

	resToken=str.Tokenize("=+-*/()><",curPos);
	}	
    //return CanshuName;
}
/************************************************************************/
/* ���ʳߴ�ķ��ŵ�                                                     */
/************************************************************************/
ProError SolidDimensionVisAct(ProDimension *dimension,
							  ProError status,CStringArray * data)
{
	CString temp;
	ProName symbol;
	ProDimensionSymbolGet(dimension,symbol);
	temp=CString(symbol);
    data->Add(temp);
	return PRO_TK_NO_ERROR;
}
/************************************************************************/
/* �������ʷ��ʺ���                                                     */
/************************************************************************/
ProError ParameterActionFn2(ProParameter *Param,ProError status,
							CStringArray *p_pdata)
{
	CString temp;
	temp=CString(Param->id);
	
	//�Ѳ�����ΪСд
	//temp.MakeLower();
	p_pdata->Add(temp);
	return PRO_TK_NO_ERROR;

}
/************************************************************************/
/* �������˺���,ֵ����DOUBLe                                            */
/************************************************************************/
ProError ParameterFilterFn2(ProParameter *Param,ProError status,
							CStringArray *p_pdata)
{ 
	ProParamvalueType r_type;
	ProParamvalue proval;
	ProParameterValueGet(Param,&proval);
	ProParamvalueTypeGet(&proval,&r_type);
	if(r_type==PRO_PARAM_DOUBLE||r_type==PRO_PARAM_INTEGER)
	{
		return PRO_TK_NO_ERROR;
	}
    return PRO_TK_CONTINUE;
}
/************************************************************************/
/* �Ƚ������ַ�������                                                   */
/************************************************************************/
CString CRelEditDlg::CompareCsarry1ToCsarry2(CStringArray * Csarry1, CStringArray * Csarry2)
{
	int num1=0,num2=0;
	int flag=0;
	num1=(int)Csarry1->GetCount();//��ϵʽ�еĳߴ����
	num2=(int)Csarry2->GetCount();//ģ���еĳߴ����
	CString error;
	for(int i=0;i<num1;i++)
	{
		flag=0;
		for(int j=0;j<num2;j++)
		{
			//���ַ������ȱ��Сд
			(*Csarry1)[i].MakeLower();
            (*Csarry2)[j].MakeLower();
			if((*Csarry1)[i]==(*Csarry2)[j])
			{
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			CString temp;
			temp.Format("����%s,���ǳߴ�Ҳ���ǲ�������ȷ�ϣ�\n",(*Csarry1)[i]);
			error.Append(temp);
		}
	}
	return error;
}
/************************************************************************/
/* ������ϵʽ��ģ����                                                   */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRelToMdl dlg;
	//dlg.SetRel(mdl,&p_UsrRelTableList);
	dlg.SetRel(mdl);
	dlg.DoModal();

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

}
/************************************************************************/
/* �޸Ĺ�ϵʽ                                                           */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton17()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//////////////////////////////////////////////////////////////////////////
	//ԭ��
	//if (iSelRelIndex!=-1)
	//{
	//	if(!CheckRel(m_RelEdit)) return;
	//	p_UsrRelTableList[iSelRelIndex].rel=m_RelEdit;
	//	p_UsrRelTableList[iSelRelIndex].note=m_RelNoteEdit;
	//	ShowRel();
	//}
	//else
	//{
	//	AfxMessageBox("����ѡ��Ҫ�޸ĵĹ�ϵʽ");
	//}
	//////////////////////////////////////////////////////////////////////////
	
    //////////////////////////////////////////////////////////////////////////
    //3
	if (iSelRelIndex!=-1)
	{
		if (g_RelCollection.EditRel(iSelRelIndex,m_RelEdit,m_RelNoteEdit))
		{
			ShowRel();
		}
		else
		{
			AfxMessageBox(g_RelCollection.error_info);
		}
	}
	else
	{
		AfxMessageBox("����ѡ��Ҫ�޸ĵĹ�ϵʽ");
	}

	//////////////////////////////////////////////////////////////////////////
	//if (iSelRelIndex!=-1)
	//{
	//	bool    bolCheck;
	//	CRel    new_CRel((ProSolid)mdl,m_RelEdit,bolCheck);
	//	if (bolCheck==true)
	//	{
	//		int flag=0;
	//		for (int j=0;j<p_CRelItemList.GetCount();j++ )
	//		{
	//			if(p_CRelItemList[j]==new_CRel && j!=iSelRelIndex)
	//			{
	//				flag=1;
	//				break;
	//			}
	//		}
	//		if(flag==0) 
	//		{
	//			p_CRelItemList[iSelRelIndex].GetRelString(new_CRel.m_strNote);
	//			p_CRelItemList.RemoveAt(iSelRelIndex);
	//			iSelRelIndex=(int)p_CRelItemList.Add(new_CRel);
	//		}
	//	}
	//	else 
	//	{
	//		AfxMessageBox(new_CRel.m_strError);
	//	}
	//}
	//else
	//{
	//	AfxMessageBox("����ѡ��Ҫ�޸ĵĹ�ϵʽ");
	//}
	//////////////////////////////////////////////////////////////////////////
	
}

void CRelEditDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnChar(nChar, nRepCnt, nFlags);
}
