// PartSWTXDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "PartSWTXDlg.h"
#include "SwtxTableItemEditDlg.h"

// CPartSWTXDlg �Ի���

IMPLEMENT_DYNAMIC(CPartSWTXDlg, CDialog)
CPartSWTXDlg::CPartSWTXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartSWTXDlg::IDD, pParent),mdl(NULL)
{
}

CPartSWTXDlg::~CPartSWTXDlg()
{
}

void CPartSWTXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_ASM_SWTX_LIST, m_List);
}


BEGIN_MESSAGE_MAP(CPartSWTXDlg, CDialog)
	ON_COMMAND(ID_12045, OnDel)
	ON_COMMAND(ID_12081, OnEdit)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_12080, OnFresh)
	ON_NOTIFY(NM_CLICK, IDD_ASM_SWTX_LIST, OnNMClickAsmSwtxList)
END_MESSAGE_MAP()


// CPartSWTXDlg ��Ϣ�������

BOOL CPartSWTXDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,100);                  //���������õ���Ĭ��Ϊ�ߴ���źͲ���������������
	m_List.InsertColumn(1,"����",LVCFMT_LEFT,100);                  //�ߴ�ķ��ţ��Լ�����������������
	m_List.InsertColumn(2,"����",LVCFMT_LEFT,50);
	m_List.InsertColumn(3,"������",LVCFMT_LEFT,50);
	m_List.InsertColumn(4,"ֵ",LVCFMT_LEFT,80);
	m_List.InsertColumn(5,"��ƫ��",LVCFMT_LEFT,30);
	m_List.InsertColumn(6,"��ƫ��",LVCFMT_LEFT,30);
	m_List.InsertColumn(7,"���͹���",LVCFMT_LEFT,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CPartSWTXDlg::ShowSWTXList(ProSolid solid)
{
	int         indexNo=0;
	m_List.DeleteAllItems();	
	mdl=solid;
	for(int i=0;i<(int)p_UsrSWTXTableList.GetCount();i++)
	{
		//�����������Ա���,
		p_UsrSWTXTableList[i].Update();
		if(p_UsrSWTXTableList[i].isCheck!=1)	 continue;
		if(p_UsrSWTXTableList[i].owner!=solid)		continue;

		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,m_List.GetItemCount(),p_UsrSWTXTableList[i].Name,0,0,0,i);
		//���ڷ���Ҫ�������֣�����ǳߴ����÷��ţ�����ǲ�������������������
		if(p_UsrSWTXTableList[i].type=="dim")
		{
			m_List.SetItemText(indexNo,1,p_UsrSWTXTableList[i].dimSymbol);
		}
		else if (p_UsrSWTXTableList[i].type=="para") {
			m_List.SetItemText(indexNo,1,p_UsrSWTXTableList[i].featName);

		}
		m_List.SetItemText(indexNo,2,p_UsrSWTXTableList[i].type);



		//�ж������ͣ��ߴ�ġ������ġ������ġ�ģ�͵�
		//���¹��ڳߴ�
		if(p_UsrSWTXTableList[i].subType=="PRODIMTYPE_LINEAR")
		{
			m_List.SetItemText(indexNo,3,"����");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRODIMTYPE_RADIUS") {
			m_List.SetItemText(indexNo,3,"�뾶");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRODIMTYPE_DIAMETER") {
			m_List.SetItemText(indexNo,3,"ֱ��");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRODIMTYPE_ANGLE") {
			m_List.SetItemText(indexNo,3,"�Ƕ�");
		}
		//���¹��ڲ���
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_DOUBLE") {
			m_List.SetItemText(indexNo,3,"������");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_STRING") {
			m_List.SetItemText(indexNo,3,"�ַ���");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_INTEGER") {
			m_List.SetItemText(indexNo,3,"����");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_BOOLEAN") {
			m_List.SetItemText(indexNo,3,"������");
		}
		//����������Ҫ��һ������
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_NOTE_ID") {
			m_List.SetItemText(indexNo,3,"ע�ͺ�");
		}
		else if (p_UsrSWTXTableList[i].subType=="PRO_PARAM_VOID") {
			m_List.SetItemText(indexNo,3,"������");
		}
		else{
			//��������ֱ��������������ProFeatureTypenameGet()���
			m_List.SetItemText(indexNo,3,p_UsrSWTXTableList[i].subType);
		}

		/************************************************************************/
		/* ��Ҫ���Ԫ�������ܹ�                                                 */
		/************************************************************************/
		//else if (p_UsrSWTXTableList[i].subType=="") {
		//	m_List2.SetItemText(indexNo,4,"δ֪");
		//}
		m_List.SetItemText(indexNo,4,p_UsrSWTXTableList[i].value);
		m_List.SetItemText(indexNo,5,p_UsrSWTXTableList[i].UpLimit);
		m_List.SetItemText(indexNo,6,p_UsrSWTXTableList[i].DwLimit);
		m_List.SetItemText(indexNo,7,p_UsrSWTXTableList[i].Note);
	}
}

void CPartSWTXDlg::OnDel()
{
	// TODO: �ڴ���������������
	int				 num=0;
	CArray<int,int>  delItem;
	num=(int)m_List.GetItemCount();
	for (int i=0;i<num;i++) {
		if (m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED) {	
			delItem.Add((int)m_List.GetItemData(i));
		}
	}
	num=(int)delItem.GetCount();
	for (int i=0;i<num;i++) {
		p_UsrSWTXTableList.RemoveAt(delItem[i]-i,1);		
	}
	p_UsrSWTXTableList.FreeExtra();
	ShowSWTXList(mdl);
}

void CPartSWTXDlg::OnEdit()
{
	// TODO: �ڴ���������������
	int num=0;
	num=(int)m_List.GetItemCount();
	for (int i=0;i<num;i++) {
		if (m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED) {	
			CSwtxTableItemEditDlg dlg;
			dlg.m_Name=m_List.GetItemText(i,0);
			dlg.m_Note=m_List.GetItemText(i,7);
			if(dlg.DoModal()==IDOK)
			{ 
				p_UsrSWTXTableList[(int)m_List.GetItemData(i)].Name =dlg.m_Name;
				p_UsrSWTXTableList[(int)m_List.GetItemData(i)].Note=dlg.m_Note;
			}
		}
	}
	ShowSWTXList(mdl);
}

void CPartSWTXDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: �ڴ������Ϣ����������
	if((WindowFromPoint(point)==&m_List))
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(2);
		ASSERT(pPopup!=NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	}
}

void CPartSWTXDlg::OnFresh()
{
	// TODO: �ڴ���������������
	ShowSWTXList(mdl);
}

void CPartSWTXDlg::OnNMClickAsmSwtxList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos=m_List.GetFirstSelectedItemPosition();
	int      nItem=m_List.GetNextSelectedItem(pos);
	if (nItem>=0)
	{
		int index=(int)m_List.GetItemData(nItem);
		if (p_UsrSWTXTableList[index].type=="dim")
		{
			ProDimensionShow(&p_UsrSWTXTableList[index].Dim,NULL,NULL,NULL);
		}
	}
	*pResult = 0;
}
