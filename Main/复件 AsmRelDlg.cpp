// AsmRelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ASM.h"
#include "AsmRelDlg.h"
#include "AsmRelVistDlg.h"
void AsmSolidDimVisFn(ProSolid solid,CStringArray * dimlist );
extern CArray<AsmRelItem,AsmRelItem> p_AsmRelItemList;
BOOL CheckEquation(CString Expression,CStringArray * importDim,CStringArray * exportDim);
// CAsmRelDlg �Ի���

IMPLEMENT_DYNAMIC(CAsmRelDlg, CDialog)
CAsmRelDlg::CAsmRelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmRelDlg::IDD, pParent)
	,asm_solid(NULL)
{
}

CAsmRelDlg::~CAsmRelDlg()
{
}

void CAsmRelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_EDIT1, m_RelEdit);
	DDX_Control(pDX, IDC_EDIT2, m_RelInfoEdit);
}


BEGIN_MESSAGE_MAP(CAsmRelDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON13, OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON10, OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON12, OnBnClickedButton12)
END_MESSAGE_MAP()


// CAsmRelDlg ��Ϣ�������

BOOL CAsmRelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"���",LVCFMT_LEFT,40);
	m_List.InsertColumn(1,"��ϵʽ",LVCFMT_LEFT,140);
	m_List.InsertColumn(2,"��ϵ��Ϣ",LVCFMT_LEFT,140);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (asm_solid==NULL) 
	{
		if (ProMdlCurrentGet((ProMdl*)&asm_solid)==PRO_TK_NO_ERROR) 
			ShowRelSet(asm_solid);
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ����ϵʽ��ȷ���                                                   */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CStringArray dimlist;
	CStringArray outCstring;
	CString      rel;
	m_RelEdit.GetWindowText(rel);
	rel.Remove(' ');
	if (asm_solid==NULL) {
		ProMdlCurrentGet((ProMdl*)&asm_solid);
	}
	AsmSolidDimVisFn(asm_solid,&dimlist );
	CheckEquation(rel,&dimlist,&outCstring);
}
/************************************************************************/
/* "+"��                                                                */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RelEdit.ReplaceSel("+",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/* "-"��                                                                */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RelEdit.ReplaceSel("-",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/* "*"��                                                                */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RelEdit.ReplaceSel("*",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/* "/"��                                                                */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RelEdit.ReplaceSel("/",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/* "()"��                                                               */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RelEdit.ReplaceSel("()",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/*"="��                                                                 */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RelEdit.ReplaceSel("=",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/* ">"��                                                                */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RelEdit.ReplaceSel(">",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/*  "<"��                                                               */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_RelEdit.ReplaceSel("<",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/* ����ģ���еĹ�ϵʽ                                                   */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAsmRelVistDlg    dlg;
	AsmRelItem        rel_item;
	CStringArray      dimlist;
	CStringArray      outCstring;
	CString           rel;
	int               indexNo;
	CString           temp;
	CStringArray      relSet;
	if (asm_solid==NULL) {
		AfxMessageBox("��ǰ������ģ��");
		return;
	}
	dlg.solid=asm_solid;
	dlg.relSet=&relSet;
	AsmSolidDimVisFn(asm_solid,&dimlist );
	if (dlg.DoModal()==IDOK) {
		for (int i=0;i<relSet.GetCount();i++) {

			///AfxMessageBox(relSet[i]);
            if (CheckEquation(relSet[i],&dimlist,&outCstring)) {
				//����ϵ�Ƿ��Ѿ�����
				int Flag=0;
				for (int j=0;j<(int)p_AsmRelItemList.GetSize();j++) {
					if(p_AsmRelItemList[j].owner!=asm_solid) continue;
					if (relSet[i]==p_AsmRelItemList[j].relation) 
					{
						Flag=1;
						break;
					}
				}
				if(Flag==1) continue;
				rel_item.owner=asm_solid;
				rel_item.relation=relSet[i];
				rel_item.note="";
				indexNo=m_List.GetItemCount();
				temp.Format("%d",indexNo+1);
				m_List.InsertItem(indexNo,temp);
				m_List.SetItemData(indexNo,p_AsmRelItemList.Add(rel_item));
				m_List.SetItemText(indexNo,1,relSet[i]);
				m_List.SetItemText(indexNo,2,rel_item.note);
			}
			else AfxMessageBox("ʧ��");
		}
	}
			
	
}
/************************************************************************/
/* �ѹ�ϵ����б���ȥ                                                   */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString       rel;
	AsmRelItem    rel_item;
	CStringArray  dimlist;
	CStringArray  outCstring;
	m_RelEdit.GetWindowText(rel);
	rel.Remove(' ');
	if (asm_solid==NULL) {
		if (ProMdlCurrentGet((ProMdl*)&asm_solid)!=PRO_TK_NO_ERROR) 
		{
			AfxMessageBox("��ǰ������ģ��");
			return;
		}
	}

	AsmSolidDimVisFn(asm_solid,&dimlist );
	if (CheckEquation(rel,&dimlist,&outCstring)) {
		//����ϵ�Ƿ��Ѿ�����
		int Flag=0;
		for (int j=0;j<(int)p_AsmRelItemList.GetSize();j++) {
			if(p_AsmRelItemList[j].owner!=asm_solid) continue;
			if (rel==p_AsmRelItemList[j].relation) 
			{
				Flag=1;
				AfxMessageBox("��ϵ�Ѿ����ڣ�");
				break;
			}
		}
		if(Flag==0) 
		{

			rel_item.owner=asm_solid;
			m_RelInfoEdit.GetWindowText(rel_item.note);
			rel_item.relation=rel;
			p_AsmRelItemList.Add(rel_item);
		}
	}
	ShowRelSet(asm_solid);	
}
/************************************************************************/
/* ��ʾģ���еĹ�ϵʽ                                                   */
/************************************************************************/
void CAsmRelDlg::ShowRelSet(ProSolid solid)
{
	int       num=0;
	int       indexNo=0;
	CString   csIndexNo;
	m_List.DeleteAllItems();
    num=(int)p_AsmRelItemList.GetSize();
	for (int i=0;i<num;i++) {
		if (p_AsmRelItemList[i].owner!=solid) continue;        
		csIndexNo.Format("%d",indexNo+1);
		m_List.InsertItem(indexNo,csIndexNo);
		m_List.SetItemData(indexNo,i);
		m_List.SetItemText(indexNo,1,p_AsmRelItemList[i].relation);
		m_List.SetItemText(indexNo,2,p_AsmRelItemList[i].note);
		indexNo++;
	}
}
