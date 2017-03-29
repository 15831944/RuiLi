// AsmAddComplg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "AsmAddComplg.h"
//#include "PartInsTableSet.h"
#include "NPartInsInfoTable.h"
#include "partInsValueTableSet.h"
#include "partInsPerfValueTableSet.h"
#include "AsmManageTableSet.h"
#include "FtpFile.h"
#include "partPerfparaTableSet.h"
#include "partVariantTableSet.h"
#include "partRelationSet.h"
#include "partDrawingSet.h"
#include "UsrSWTXTable.h"
#include "partPicSet.h"
#include "CAsmPicSet.h"
#include "AsmInsInfoTableSet.h"
#include "AsmInsVariantTableSet.h"
#include "AsmPerformanceInsTableSet.h"
#include "CAsmVarSet.h"
#include "CAsmPerfSet.h"
#include "NPartTypeSet.h"
#include "NPartInsInfoTable.h"
#include "NProductInsInfoTable.h"
#include "NProductInsPerfTable.h"
#include "NProductInsVariantTable.h"
#include "AsmAssembly.h"
// CAsmAddComplg �Ի���

IMPLEMENT_DYNAMIC(CAsmAddComplg, CDialog)
CAsmAddComplg::CAsmAddComplg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmAddComplg::IDD, pParent)
	, m_selTab(0)
	, mdlSelectNumber(-1)
{
}

CAsmAddComplg::~CAsmAddComplg()
{
}

void CAsmAddComplg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PART_TREE, m_Tree[0]);
	DDX_Control(pDX, IDC_ASM_TREE, m_Tree[1]);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_PATH_STATIC, m_Path);
	DDX_Control(pDX, IDC_LIST4, m_List2);
	DDX_Control(pDX, IDC_BUTTON2, m_btnOpen);
	DDX_Control(pDX, IDC_RECT_PIC, m_RectPic);
	DDX_Control(pDX, IDC_PARTINS_TREE, m_Tree[2]);
	DDX_Control(pDX, IDC_ASMINS_TREE, m_Tree[3]);
}


BEGIN_MESSAGE_MAP(CAsmAddComplg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
//	ON_NOTIFY(NM_CLICK, IDC_PART_TREE, OnNMClickPartTree)
ON_NOTIFY(TVN_SELCHANGED, IDC_PART_TREE, OnTvnSelchangedPartTree)
ON_NOTIFY(TVN_SELCHANGED, IDC_ASM_TREE, OnTvnSelchangedAsmTree)
ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
//ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
ON_WM_MOVING()
ON_WM_PAINT()
ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
ON_COMMAND(ID_12192, OnAsmBom)
ON_COMMAND(ID_12193, OnAsmPrtList)
ON_WM_CONTEXTMENU()
ON_NOTIFY(TVN_SELCHANGED, IDC_PARTINS_TREE, OnTvnSelchangedPartinsTree)
ON_NOTIFY(TVN_SELCHANGED, IDC_ASMINS_TREE, OnTvnSelchangedAsminsTree)
END_MESSAGE_MAP()


// CAsmAddComplg ��Ϣ�������

BOOL CAsmAddComplg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	/*RECT		rect;*/
	TCITEM		tcItem;
	RECT		rect1;
	//1.���Tabҳ3�������ģ�͡���Ʒ/���ģ�͡����
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("���ģ��");
	m_Tab.InsertItem(0, &tcItem);
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("��Ʒ/���ģ��");
	m_Tab.InsertItem(1, &tcItem);
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("���");
	m_Tab.InsertItem(2, &tcItem);
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("����");
	m_Tab.InsertItem(3, &tcItem);

	//2.����ͼ��
	m_ImageList.Create(14, 14, ILC_COLOR32, 4, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));//��Ҫ�޸ĳ�ʵ����ͼ��
	m_Tree[0].SetImageList(&m_ImageList,TVSIL_NORMAL);
	m_Tree[1].SetImageList(&m_ImageList,TVSIL_NORMAL);
    m_Tree[2].SetImageList(&m_ImageList,TVSIL_NORMAL);
	m_Tree[3].SetImageList(&m_ImageList,TVSIL_NORMAL);

	//////////////////////////////////////////////////////////////////////////
	if (!m_pSet.Open())
	{
		AfxMessageBox("���ݿ����Ӳ��ɹ�");
		SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	//3.��������
    //��ʾ���ģ�͵�����������Ը���ģ�����������
    ShowNPartTree(&m_Tree[0],m_pSet.m_pDatabase,true);
	//��ʾ���ģ�͵������
    ShowNAsmTree(&m_Tree[1],m_pSet.m_pDatabase,true);
	//�����ֱ�Ӹ��ݷ������������
	ShowPartTypeTree(&m_Tree[2],m_pSet.m_pDatabase);
	//��ʾ���ģ�͵������
	ShowNAsmTree(&m_Tree[3],m_pSet.m_pDatabase,false);


	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,80,-1);
	m_List.InsertColumn(1,"����",LVCFMT_LEFT,80,-1);
	m_List.InsertColumn(2,"���״̬",LVCFMT_LEFT,80,-1);  //�Ƿ����  "���" "δ���"
	m_List.InsertColumn(3,"���״̬",LVCFMT_LEFT,80,-1);  //          "���ͨ��"  "���δͨ��"
	m_List.InsertColumn(4,"PDM״̬",LVCFMT_LEFT,80,-1);   //          "�ύ"   "δ�ύ"
	m_List.InsertColumn(6,"���λ��",LVCFMT_LEFT,80,-1); 
	m_List.InsertColumn(7,"�����Ա",LVCFMT_LEFT,80,-1); 
	m_List.InsertColumn(8,"��ע",LVCFMT_LEFT,200,-1);

    //4.����ͼƬ��ʾ�ؼ��Ĵ�С��λ��	
	m_RectPic.GetWindowRect(&rect1); 
	ScreenToClient(&rect1);
	pic_dlg.Create(IDD_PIC_FORM_DB,this);
	pic_dlg.MoveWindow(&rect1,TRUE);
	pic_dlg.ShowWindow(SW_SHOW);

	//5.��ʼ���ô�װ�䰴ťΪ�����ã�ֻ�е���������֮�����ʹ�������ť	
    m_btnOpen.EnableWindow(FALSE);
    
	//6.���ÿؼ�m_List2
	m_List2.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List2.InsertColumn(0,"������",LVCFMT_LEFT,100,-1);
	m_List2.InsertColumn(1,"����ֵ",LVCFMT_LEFT,100,-1);
   
	//7.�������ؼ�����ʾ״̬
	m_Tree[0].ShowWindow(SW_SHOW);
	m_Tree[1].ShowWindow(SW_HIDE);
	m_Tree[2].ShowWindow(SW_HIDE);
	m_Tree[3].ShowWindow(SW_HIDE);
	m_selTab =0;  

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* TAB���л�                                                            */
/************************************************************************/
void CAsmAddComplg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i=0;i<4;i++)
	{
		if(m_Tab.GetCurSel()==i)
		{
			m_Tree[i].ShowWindow(SW_SHOW);
			m_selTab=i;
			continue;
		}
		m_Tree[i].ShowWindow(SW_HIDE);
	}
	*pResult = 0;
}
/************************************************************************/
/* ������ģ������Ķ��� 
   1.������е�  ����������б�������ʾ������µ�ģ��
   2.������е�  ��ģ�ͣ������б�������ʾģ�͵�ʵ��                     */
/************************************************************************/
void CAsmAddComplg::OnTvnSelchangedPartTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW			pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CNPartInsInfoTable		m_tempSet(m_pSet.m_pDatabase);
	CpartVariantTableSet	tempVaiantSet(m_pSet.m_pDatabase);
	CpartPerfparaTableSet	tempPerfSet(m_pSet.m_pDatabase);
	long					num=(long)pNMTreeView->itemNew.lParam;
	int						itemImageIndex;
	int						itemSelImage;
	/*CString				    csIndexNo;*/
	CString					csText="";
	//1�����ѡ�е����ͼ�꣬���ݵ�����ͼƬ�������ж�
	m_Tree[0].GetItemImage(pNMTreeView->itemNew.hItem,itemImageIndex,itemSelImage);

	//2��ɾ���б�,����װ�䰴ť������
	m_List.DeleteAllItems();
	m_List2.DeleteAllItems();
	m_btnOpen.EnableWindow(FALSE);

	//3������·�����ı�
	HTREEITEM		hItem=pNMTreeView->itemNew.hItem;
	while (hItem!=m_Tree[0].GetRootItem())
	{
		CString temp=m_Tree[0].GetItemText(hItem);
		csText		=temp+"\\"+csText;
		hItem		=m_Tree[0].GetParentItem(hItem);	   
	}
	csText="\\"+csText;
	m_Path.SetWindowText(csText);
	//���ѡ�еĸ����˳�
	if(pNMTreeView->itemNew.hItem==m_Tree[0].GetRootItem()) return;
		

	//4�����ݲ�ͬ�������
	switch(itemImageIndex) {
	case USR_ICON_TYPE:
		//ѡ������𲻷�Ӧ
		break;
	case USR_ICON_PRTMDL:
		//ѡ�������ģ�ͣ������б��������ʾģ��ʵ��
		//4��1�ڵڶ����б����ʾ��ģ�͵Ŀɱ�����Լ����ܲ�������Ϣ
		tempVaiantSet.m_strFilter.Format("[PrtNum]=%ld",num);
		tempVaiantSet.Open();
		if (!tempVaiantSet.IsBOF())
		{
						tempVaiantSet.MoveFirst();
						do {
							m_List2.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List2.GetItemCount(),tempVaiantSet.m_Name,0,0,0,tempVaiantSet.m_Number);
							m_List2.SetItemText(m_List2.GetItemCount()-1,1,tempVaiantSet.m_Value);
							tempVaiantSet.MoveNext();
						} while(!tempVaiantSet.IsEOF());
		}
		tempPerfSet.m_strFilter.Format("[PrtNum]=%ld",num);
		tempPerfSet.Open();
		if (!tempPerfSet.IsBOF())
		{
						m_List2.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List2.GetItemCount(),"���ܲ���",0,0,0,-1);
						tempPerfSet.MoveFirst();
						do {
							m_List2.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List2.GetItemCount(),tempPerfSet.m_Name,0,0,0,tempPerfSet.m_Number);
							m_List2.SetItemText(m_List2.GetItemCount()-1,1,tempPerfSet.m_Value);
							tempPerfSet.MoveNext();
						} while(!tempPerfSet.IsEOF());
		}

		
		//4.2 ���ģ�͵�ʵ��
        m_tempSet.m_strFilter.Format("[MdlNum]=%ld",num);
		m_tempSet.Open();
		if (!m_tempSet.IsBOF())
		{
			m_btnOpen.EnableWindow(TRUE);
			m_tempSet.MoveFirst();
			do {
				m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List.GetItemCount(),m_tempSet.m_Name,0,0,0,m_tempSet.m_PrtNum);
				m_List.SetItemText(m_List.GetItemCount()-1,1,m_tempSet.m_InsCode);
				m_List.SetItemText(m_List.GetItemCount()-1,2,(m_tempSet.m_FinishState==_T("���")?_T("���"):_T("δ���")));
				m_List.SetItemText(m_List.GetItemCount()-1,3,(m_tempSet.m_AuditState==_T("���ͨ��")?_T("���ͨ��"):_T("���δͨ��")));
				m_List.SetItemText(m_List.GetItemCount()-1,4,(m_tempSet.m_PDMState==_T("�ύ")?_T("�ύ"):_T("δ�ύ")));
				m_List.SetItemText(m_List.GetItemCount()-1,5,(m_tempSet.m_PDMState==_T("�ύ")?"PDMϵͳ":(m_tempSet.m_LocalState=="������"?"���ϵͳ":"�û�����")));
				m_List.SetItemText(m_List.GetItemCount()-1,6,m_tempSet.m_Designer);
				m_List.SetItemText(m_List.GetItemCount()-1,7,m_tempSet.m_Note);
				m_tempSet.MoveNext();
			} while(!m_tempSet.IsEOF());
		}

		//4��3���õ�ǰѡ���ģ��
		mdlSelectNumber=num;

		//4��4���ģ�͵�ͼƬ
		pic_dlg.GetMdlPicSet(0,num,m_pSet.m_pDatabase);
		break;
	default:
		break;
	} 
	*pResult = 0;
}
/************************************************************************/
/* ������ģ������ʱ�����ѡ�е������ģ�͵�ͼ�꣬�����б�����ʾ����ʵ��
 �Լ����Ŀɱ�ߴ������ͼƬ�����										*/
/************************************************************************/
void CAsmAddComplg::OnTvnSelchangedAsmTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAsmManageTableSet				m_Set(m_pSet.m_pDatabase);
	CNProductInsInfoTable			m_Set1(m_pSet.m_pDatabase);
	CAsmVarSet						m_Set2(m_pSet.m_pDatabase);
	CAsmPerfSet						m_Set3(m_pSet.m_pDatabase);
	long							num=(long)pNMTreeView->itemNew.lParam;
	int								itemImageIndex;
	int								itemSelImage;
	CString							csText;
	//1.���ѡ�е����ͼ�꣬���ݵ�����ͼƬ�������ж�
	m_Tree[1].GetItemImage(pNMTreeView->itemNew.hItem,itemImageIndex,itemSelImage);

	//2��ɾ���б�,����װ�䰴ť������
	m_List.DeleteAllItems();
	m_List2.DeleteAllItems();
	m_btnOpen.EnableWindow(FALSE);

	//3������·�����ı�
	HTREEITEM   hItem=pNMTreeView->itemNew.hItem;
	while (hItem!=m_Tree[1].GetRootItem())
	{
		CString temp=m_Tree[1].GetItemText(hItem);
		csText=temp+"\\"+csText;
		hItem=m_Tree[1].GetParentItem(hItem);	   
	}
	csText="\\"+csText;
	m_Path.SetWindowText(csText);

	//���ѡ�еĸ����˳�
	if(pNMTreeView->itemNew.hItem==m_Tree[1].GetRootItem())
		return;
	switch(itemImageIndex) {
	case USR_ICON_TYPE:
		break;
	case USR_ICON_ASMMDL:
		//���ѡ�������ģ�ͣ������б��������ʾģ�͵�ʵ��
		//4.1�ڵڶ����б����ʾ��ģ�͵Ŀɱ�����Լ����ܲ�������Ϣ
		m_Set2.m_strFilter.Format("[AsmNum]=%ld",num);
		m_Set2.Open();
		if (!m_Set2.IsBOF())
		{
			m_Set2.MoveFirst();
			do {
				m_List2.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List2.GetItemCount(),m_Set2.m_Name,0,0,0,m_Set2.m_Number);
				m_List2.SetItemText(m_List2.GetItemCount()-1,1,m_Set2.m_Value);
				m_Set2.MoveNext();
			} while(!m_Set2.IsEOF());
		}

		m_Set3.m_strFilter.Format("[AsmNum]=%ld",num);
		m_Set3.Open();
		if (!m_Set3.IsBOF())
		{
			m_List2.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List2.GetItemCount(),"���ܲ���",0,0,0,-1);
			m_Set3.MoveFirst();
			do {
				m_List2.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List2.GetItemCount(),m_Set3.m_Name,0,0,0,m_Set3.m_Number);
				m_List2.SetItemText(m_List2.GetItemCount()-1,1,m_Set3.m_Value);
				m_Set3.MoveNext();
			} while(!m_Set3.IsEOF());
		}

		//4.2���ģ�͵�ʵ��
		m_Set1.m_strFilter.Format("[MdlNum]=%ld",num);
		m_Set1.Open();		
		if (!m_Set1.IsBOF())
		{
			m_btnOpen.EnableWindow(TRUE);
			m_Set1.MoveFirst();
			while (!m_Set1.IsEOF())
			{
				m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List.GetItemCount(),m_Set1.m_Name,0,0,0,m_Set1.m_PdtNum);
				m_List.SetItemText(m_List.GetItemCount()-1,1,m_Set1.m_InsCode);
				m_List.SetItemText(m_List.GetItemCount()-1,2,(m_Set1.m_FinishState==_T("���")?_T("���"):_T("δ���")));
				m_List.SetItemText(m_List.GetItemCount()-1,3,(m_Set1.m_AuditState==_T("���ͨ��")?_T("���ͨ��"):_T("���δͨ��")));
				m_List.SetItemText(m_List.GetItemCount()-1,4,(m_Set1.m_PDMState==_T("�ύ")?_T("�ύ"):_T("δ�ύ")));
				m_List.SetItemText(m_List.GetItemCount()-1,5,(m_Set1.m_PDMState==_T("�ύ")?"PDMϵͳ":(m_Set1.m_LocalState=="������"?"���ϵͳ":"�û�����")));
				m_List.SetItemText(m_List.GetItemCount()-1,6,m_Set1.m_Designer);
				m_List.SetItemText(m_List.GetItemCount()-1,7,m_Set1.m_Note);
				m_Set1.MoveNext();
			}
		}
		//4.3���õ�ǰѡ���ģ��
		mdlSelectNumber=num;

		//4.4
		pic_dlg.GetMdlPicSet(1,num,m_pSet.m_pDatabase);
		break;
	default:
		break;
	}
	*pResult = 0;
}
/************************************************************************/
/* �����װ�䡿ģ�Ͱ�ť                                                 */
/************************************************************************/
void CAsmAddComplg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch(m_selTab) {
	case 0:    //ѡ��������ģ�͵�ʵ��
		AssemblePrtMdl();
		break;
	case 1:
		AssembleAsmMdl();
		break;
	case 2:
		AssemblePrtMdl();
		break;
	case 3:
		AssembleAsmMdl();
	default:
		break;
	}

}
/************************************************************************/
/*//װ�����ģ��                                                        */
/************************************************************************/
void  CAsmAddComplg::AssemblePrtMdl()
{
	ProSolid			    assmbly,comp;
	UsrPartPorp             temp_porp;
	CNPartInsInfoTable		m_tempSet(m_pSet.m_pDatabase);
	POSITION				pos = m_List.GetFirstSelectedItemPosition();
	int						nItem = m_List.GetNextSelectedItem(pos);
	//1.��ǰ�Ѿ�����ͬ�����װ�������
	//2.��ǰ�Ѿ�����ͬ�����װ����һ�������
	//3.װ���������������е����ͬ��
	//4.��ǰ�����û�����ڴ��г��ֹ�
	//�ۺϣ���Ϊ������ļ�������Ψһ�ģ�ֻҪ���ǵ�ǰ�Ƿ�����ͬ���ļ����Ϳ�����
	//���ڣ�����ͬһ���������ͬ�����������͹�����Ա仯����ͬһ���������Ȼ���ֶԳ�����
	//���ı��͹�������ǲ�һ�������ԶԱ��͹�������ã�����Ȼ�û�����ѡ��
	//���ѡ����Ϊ������ͬ�ı��͹���������

    if(nItem<0)				return;
					
	ProMdlCurrentGet((ProMdl*)&assmbly);
	m_tempSet.m_strFilter.Format("[PrtNum]=%ld",(long)m_List.GetItemData(nItem));
	m_tempSet.Open();
	if (!m_tempSet.IsBOF())
	{
		//�����ǰ�Ѿ���������ļ�
		//�Ҷ�����ͬ�ĺ�,�����ֱ��װ�䣬������������
		m_MdlDrawItem.RemoveAll();
		if (IsMdlInSession(m_tempSet.m_SolidFileName,PRO_MDL_PART,(ProMdl*)&comp))
		{
            temp_porp=GetMdlInfo(comp);
			CString		csTemp;
			//����ļ�����ͬ����������Ų�ͬ������и���
			if (temp_porp.Number!=m_tempSet.m_PrtNum)
			{
				csTemp.Format("ϵͳ���Ѿ�����ͬ���ļ���(%s)����\n",GetMdlFileName(comp));
				csTemp.Format("%s�Ƿ������(���ܻ�ʹװ��ʧ��)!",csTemp);
				if (AfxMessageBox(csTemp,MB_YESNO|MB_ICONQUESTION)==IDYES)
				{
					if (AssembleComp(assmbly,comp))
					{
						AddPartMdlInfo(comp,(long)m_List.GetItemData(nItem),1,NULL);
						OnClose();
					}
				}		
			}
			else         //����ļ�����ͬ���������ͬ����˵������������ͬһ�����
			{
				if (AssembleComp(assmbly,comp))
				{
					OnClose();
				}
			}
		}		                //�����ǰ���������,���֮����װ�䣬���װ��ɹ����������Ϣ
		else if(AssembleComp(assmbly,(long)m_List.GetItemData(nItem),PRO_MDL_PART))
		{
			OnClose();
		}
	}
}
/************************************************************************/
/*//װ�����ģ��                                                        */
/************************************************************************/
void  CAsmAddComplg::AssembleAsmMdl()
{
	ProSolid			    assmbly;
	ProSolid                comp;
	UsrPartPorp             temp_porp;
	CAsmAssembly            product_open;
	CNProductInsInfoTable	m_tempSet(m_pSet.m_pDatabase);	
	POSITION				pos = m_List.GetFirstSelectedItemPosition();
	int						nItem = m_List.GetNextSelectedItem(pos);	

	if(nItem<0)				return;
				
	ProMdlCurrentGet((ProMdl*)&assmbly);
	m_tempSet.m_strFilter.Format("[PdtNum]=%ld",(long)m_List.GetItemData(nItem));
	m_tempSet.Open();
	if (!m_tempSet.IsBOF())
	{
		//�����ǰ�Ѿ���������ļ�
		//�Ҷ�����ͬ�ĺ�,�����ֱ��װ�䣬������������
		m_MdlDrawItem.RemoveAll();
		if (IsMdlInSession(m_tempSet.m_SolidFileName ,PRO_MDL_ASSEMBLY,(ProMdl*)&comp))
		{
					temp_porp=GetMdlInfo(comp);
					CString   csTemp;
					//����ļ�����ͬ����������Ų�ͬ������и���
					if (temp_porp.Number!=m_tempSet.m_PdtNum)
					{
						csTemp.Format("ϵͳ���Ѿ�����ͬ���ļ���(%s)����",GetMdlFileName(comp));
						csTemp.Format("%s�Ƿ������(���ܻ�ʹװ��ʧ��)!",csTemp);
						if (AfxMessageBox(csTemp,MB_YESNO|MB_ICONQUESTION)==IDYES)
						{
							if (product_open.OpenProduct(m_pSet.m_pDatabase,(long)m_List.GetItemData(nItem),&comp))
							{
								if (AssembleComp(assmbly,comp))
								{
									product_open.AddInfo();
									OnClose();
								}
								else
								{
									product_open.CancelAssembly();
								}
							}
						}		
					}
					else   //����ļ�����ͬ���������ͬ����˵������������ͬһ�����
					{
						if(assmbly!=comp)
						{
							if (AssembleComp(assmbly,comp))
							{
								OnClose();
							}
						}
						else
						{
							AfxMessageBox("���ܰѵ�ǰģ������װ�䵱ǰģ��");
						}
						
					}
		}
		//�����ǰ���������,���֮����װ�䣬���װ��ɹ����������Ϣ
		else
		{
					if (product_open.OpenProduct(m_pSet.m_pDatabase,(long)m_List.GetItemData(nItem),&comp))
					{
						if (AssembleComp(assmbly,comp))
						{
							product_open.AddInfo();
							OnClose();
						}
						else
						{
							product_open.CancelAssembly();
						}
					}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//���б�2��صĺ���
void CAsmAddComplg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CpartInsValueTableSet   	partInsSet(m_pSet.m_pDatabase);
	CpartInsPerfValueTableSet	partInsPerfSet(m_pSet.m_pDatabase);
	CNProductInsVariantTable	asmInsVaSet(m_pSet.m_pDatabase);
	CNProductInsPerfTable		asmInsPerfSet(m_pSet.m_pDatabase);
	int							sel = m_Tab.GetCurSel();
	int							listIndex=0;
	POSITION					pos = m_List.GetFirstSelectedItemPosition();
	int							nItem = m_List.GetNextSelectedItem(pos);

	if(nItem==-1)				return;

	long						lSelInsNum=(long)m_List.GetItemData(nItem);

	if(m_List2.GetItemCount()<=0) return;
	switch(sel)
	{
	case 0:	
		partInsSet.Open();
		while ((long)m_List2.GetItemData(listIndex)!=-1 && listIndex<m_List2.GetItemCount())
		{
			partInsSet.m_strFilter.Format("[PrtNum]=%ld AND [Num]=%ld"
				,lSelInsNum,(long)m_List2.GetItemData(listIndex));
			partInsSet.Requery();
			if (!partInsSet.IsBOF())
			{
				m_List2.SetItemText(listIndex,1,partInsSet.m_Value);
			}
			listIndex++;
			if(listIndex>=m_List2.GetItemCount()) break;
		}
		if (listIndex>=m_List2.GetItemCount()-1) break;
		listIndex++;
		partInsPerfSet.Open();
		while (listIndex<m_List2.GetItemCount())
		{
			partInsPerfSet.m_strFilter.Format("[PrtNum]=%ld AND [Num]=%ld"
				,lSelInsNum,(long)m_List2.GetItemData(listIndex));
			partInsPerfSet.Requery();
			if (!partInsSet.IsBOF())
			{
				m_List2.SetItemText(listIndex,1,partInsPerfSet.m_Value);
			}
			listIndex++;
		}
		break;
	case 1:
		asmInsVaSet.Open();
		while ((long)m_List2.GetItemData(listIndex)!=-1 && listIndex<m_List2.GetItemCount())
		{
			asmInsVaSet.m_strFilter.Format("[PdtNum]=%ld AND [Num]=%ld"
				,lSelInsNum,(long)m_List2.GetItemData(listIndex));
			asmInsVaSet.Requery();
			if (!asmInsVaSet.IsBOF())
			{
				m_List2.SetItemText(listIndex,1,asmInsVaSet.m_Value);
			}
			listIndex++;
			if(listIndex>=m_List2.GetItemCount()) break;
		}
		if (listIndex>=m_List2.GetItemCount()-1) break;
		listIndex++;
		asmInsPerfSet.Open();
		while (listIndex<m_List2.GetItemCount())
		{
			asmInsPerfSet.m_strFilter.Format("[PdtNum]=%ld AND [Num]=%ld"
				,lSelInsNum,(long)m_List2.GetItemData(listIndex));
			asmInsPerfSet.Requery();
			if (!asmInsPerfSet.IsBOF())
			{
				m_List2.SetItemText(listIndex,1,asmInsPerfSet.m_Value);
			}
			listIndex++;
		}
		break;
	case 2:
		break;
	}
	*pResult = 0;
}
void CAsmAddComplg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialog::OnMoving(fwSide, pRect);

	// TODO: �ڴ������Ϣ����������
}
/************************************************************************/
/* �ػ�                                                                 */
/************************************************************************/
void CAsmAddComplg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

/************************************************************************/
/* ɾ��ģ�ͼ�����صĹ���ͼ                                              */
/************************************************************************/
BOOL CAsmAddComplg::DeleteSolidAndDrawing(ProSolid solid,bool bolDeleteDirectory=false)
{
	ProError						status;
	CString							dir;
	CArray<ProDrawing,ProDrawing>  p_DrawingList;
	GetSolidDrawing(solid,&p_DrawingList);

	for (int i=0;i<(int)p_DrawingList.GetCount();i++) 
	    ProMdlErase(p_DrawingList[i]);
	//ProMdlErase(solid);		
	if (bolDeleteDirectory)
	{
		dir=GetMdlPath(solid);
		RemoveDireAndFile(dir);
	}
	status=ProMdlErase((ProMdl)solid);
	return TRUE;
}
/************************************************************************/
/* �õ������Ԫ���ľ��������װ�䣬������̽�����Proe��װ��Ի���       */
/************************************************************************/
BOOL CAsmAddComplg::AssembleComp(ProSolid solid,ProSolid comp)
{
	ProMdlType      type;
	ProMatrix		init_pos= {{ 1.0, 0.0, 0.0, 0.0 }, 
	{0.0, 1.0, 0.0, 0.0}, 
	{0.0, 0.0, 1.0, 0.0}, 
	{0.0, 0.0, 0.0, 1.0}};
	ProAsmcomp		 feature; 
	ProError		 status;
	int             feat_ids[1];
	ProBoolean      p_comp_is_underconstrained;
	status=ProMdlTypeGet((ProMdl)solid,&type);
	if (type!=PRO_MDL_ASSEMBLY)
	{
		AfxMessageBox("��ǰģ�Ͳ������!");
		return FALSE;
	}   
    //�رնԻ���
	ShowWindow(SW_HIDE);
	//1.װ��
	status=ProAsmcompAssemble(solid,comp,init_pos,&feature);
	status=ProAsmcompIsUnderconstrained(&feature,&p_comp_is_underconstrained);
	if (p_comp_is_underconstrained==PRO_B_TRUE)
	{
		status=ProAsmcompConstrRemove(&feature,-1);
	}	
	status=ProAsmcompConstrRedefUI(&feature);
	//ȡ��װ���װ�䲻�ɹ�
	if(status==PRO_TK_USER_ABORT || status==PRO_TK_BAD_INPUTS)
	{
		feat_ids[0]=feature.id;
		ProFeatureDelete(solid,feat_ids,1,NULL,0);
		ShowWindow(SW_SHOW);
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/*//�ӷ�����������������װ��(���)                                      */
/************************************************************************/
BOOL CAsmAddComplg::AssembleComp(ProSolid solid,long InsNum,ProMdlType type)
{
	CStringArray			files;//�������ϵ��ļ�λ��
	//CStringArray            localFiles;//���ط������ϵ��ļ���ģ�ͣ�
	//CStringArray            localDrawingFiles;//���صĹ���ͼ
	CFtpFile                m_ftp;
	CNPartInsInfoTable		m_tempSet(m_pSet.m_pDatabase);	
	CNPartTypeSet           m_parttypeset(m_pSet.m_pDatabase);
	CString                 localDir="";	
	CString                 newDir="";
	CString					csError="";
	ProError				status;
	ProPath					name;
	mdl_draw_item           tempMdlDrawItem;
	tempMdlDrawItem.draw=NULL;
	tempMdlDrawItem.solid=NULL;
	tempMdlDrawItem.DrawFullName=tempMdlDrawItem.SolidFullName="";

	if (type==PRO_MDL_PART)
	{
		m_tempSet.m_strFilter.Format("[PrtNum]=%ld",InsNum);
		m_tempSet.Open();
		if (!m_tempSet.IsBOF())
		{
			//1���ҵ��ƶ����ļ���Ϊģ�͵Ĵ���׼��
			//
			localDir=GetMdlPath((ProMdl)solid);
			if (!SelectAnDirectory(localDir,localDir))
			{
				return FALSE;;
			}			
			//1��1������Ҫ���ص��ļ��У����أ�
			if (!CreateDirectoryGivenPathAndName(localDir,m_tempSet.m_Name,newDir))
			{
				return FALSE;
			}
			//1��2�����ļ�
			if (m_tempSet.m_PDMState==_T("�ύ"))
			{
				//����PDM����ȡģ�ͺ͹���ͼ�ĵ�ַ
				CString  fileTemp1="",fileTemp2="";
				UsrGetPartAndProductFromPdm(m_tempSet.m_PDMNum,0,fileTemp1,fileTemp2);
				if(!m_tempSet.m_SolidFileName.IsEmpty()) 
				{					
					files.Add(fileTemp1);
					tempMdlDrawItem.SolidFullName=localDir+newDir+"\\"+m_tempSet.m_SolidFileName;
				}
				if(!m_tempSet.m_DrawingName.IsEmpty())
				{
					files.Add(fileTemp2);
					tempMdlDrawItem.DrawFullName=localDir+newDir+"\\"+m_tempSet.m_DrawingName;
				}	
				m_ftp.DownloadFilestoDirectory(&files,localDir+newDir,myFtp,csError);
				if (!csError.IsEmpty())
				{
					AfxMessageBox(csError);
					return FALSE;
				}
			}
			else if (m_tempSet.m_LocalState==_T("������"))
			{
				//���ݷ���������ȡģ�ͣ�����ͼ�ĵ�ַ,ͨ�����أ�
				//����Ҫ��һ�����ҹ��̣����Ƿ����ҵ��ƶ�ͼֽ
				if(!m_tempSet.m_SolidFileName.IsEmpty()) 
				{					
					files.Add(m_tempSet.m_SolidPlace+"/"+m_tempSet.m_SolidFileName+".1");
					tempMdlDrawItem.SolidFullName=localDir+newDir+"\\"+m_tempSet.m_SolidFileName;
				}
				if(!m_tempSet.m_DrawingName.IsEmpty())
				{
					files.Add(m_tempSet.m_DrawingPlace+"/"+m_tempSet.m_DrawingName+".1");
					tempMdlDrawItem.DrawFullName=localDir+newDir+"\\"+m_tempSet.m_DrawingName;
				}	
				m_ftp.DownloadFilestoDirectory(&files,localDir+newDir,myFtp,csError);
				if (!csError.IsEmpty())
				{
					AfxMessageBox(csError);
					return FALSE;
				}
			}
			else if (m_tempSet.m_LocalState==_T("����"))
			{
				//���ݱ��أ����ж��Ƿ�Ϊ�������������ж��ƶ���Ŀ¼�Ƿ�������ļ�������Ҫ����
				if(!m_tempSet.m_SolidFileName.IsEmpty()) 
				{					
					tempMdlDrawItem.SolidFullName=m_tempSet.m_SolidPlace+"\\"+m_tempSet.m_SolidFileName;
					files.Add(tempMdlDrawItem.SolidFullName);
				}					
				if(!m_tempSet.m_DrawingName.IsEmpty())   
				{
					tempMdlDrawItem.DrawFullName=m_tempSet.m_DrawingPlace+"\\"+m_tempSet.m_DrawingName;
					files.Add(tempMdlDrawItem.DrawFullName);
				}			
				
				if (!IsFilesExists(files,csError))
				{
					AfxMessageBox(csError);
					return FALSE;
				}
			}
			else 
			{
				AfxMessageBox("�ļ����λ�ò�ȷ�����޷�Ϊ���!");
				return FALSE;
			}
			//2�����ļ�
			if(tempMdlDrawItem.SolidFullName.IsEmpty()) return FALSE;
			ProStringToWstring(name,tempMdlDrawItem.SolidFullName.GetBuffer());
			status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&tempMdlDrawItem.solid); 
			if(status!=PRO_TK_NO_ERROR)
			{
				return FALSE;
			}
			//3��װ��
			if (!AssembleComp(solid,(ProSolid)tempMdlDrawItem.solid))
			{
				return FALSE;
			}
			//4.�򿪹���ͼ
			if(!tempMdlDrawItem.DrawFullName.IsEmpty())
			{
				ProStringToWstring(name,tempMdlDrawItem.DrawFullName.GetBuffer());
				status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&tempMdlDrawItem.draw); 
				if(status!=PRO_TK_NO_ERROR) 
				{
					AfxMessageBox("����ģ�͵Ĺ���ͼʧ��!");
					tempMdlDrawItem.draw=NULL;
				}
			}
			//5.���ģ����Ϣ
			AddPartMdlInfo(tempMdlDrawItem.solid,InsNum,0,&tempMdlDrawItem);
			return TRUE;
		}
	}
	if (type==PRO_MDL_ASSEMBLY)
	{
	}
	return TRUE;
}
BOOL CAsmAddComplg::AddPartMdlInfo(ProMdl mdl,long PrtNum,int type=0/*�Ƿ�����*/,mdl_draw_item * tempMdlDrawItem=NULL)
{
	CNPartInsInfoTable		m_tempSet(m_pSet.m_pDatabase);	
	CNPartTypeSet           m_parttypeset(m_pSet.m_pDatabase);
	UsrPartPorp             temp_porp;
	m_tempSet.m_strFilter.Format("[PrtNum]=%ld",PrtNum);
	m_tempSet.Open();
	if(!m_tempSet.IsBOF())
	{
		m_tempSet.MoveFirst();
		temp_porp.owner  = mdl;
		temp_porp.Number = PrtNum;
		//�ж��������Ƿ�����ģ��
		switch(m_tempSet.m_MdlNum) {
			case -1:
				temp_porp.MdlType=2;           //��ģ�͵���������
				temp_porp.MdlNum =-1;
				break;
			default:
				temp_porp.MdlType=1;           //��ģ�͵�ʵ��
				temp_porp.MdlNum =m_tempSet.m_MdlNum;
				break;
		}			
		temp_porp.Name   =m_tempSet.m_Name;
		temp_porp.Code   =m_tempSet.m_InsCode;
		//���ģ�͵�����Լ��������
		m_parttypeset.m_strFilter.Format("[Number]=%ld",m_tempSet.m_TypeNum);
		m_parttypeset.Open();
		if (!m_parttypeset.IsBOF())
		{
			m_parttypeset.MoveFirst();
			temp_porp.TypeNum=m_tempSet.m_TypeNum;
			temp_porp.TypeName=m_parttypeset.m_Name;
		}
		else
		{
			temp_porp.TypeNum=-1;
			temp_porp.TypeName="";
		}
		temp_porp.Material=m_tempSet.m_Material;
		temp_porp.Person  ="";
		temp_porp.Note    =m_tempSet.m_Note;
		////�ж��Ƿ�ȫ�����
		if (tempMdlDrawItem!=NULL)
		{
			if (tempMdlDrawItem->draw!=NULL)
			{
				temp_porp.bHaveDraw =true;
				temp_porp.hDraw     =tempMdlDrawItem->draw;
			}
			else
			{
				temp_porp.hDraw=NULL;
				temp_porp.bHaveDraw=false;
			}
		}
		temp_porp.IsAudited=(m_tempSet.m_AuditState==_T("���ͨ��")?true:false);
		//�ļ�����Դ
		if (m_tempSet.m_PDMState==_T("�ύ"))
			temp_porp.ResourceType=2;
		else if(m_tempSet.m_LocalState==_T("����"))
			temp_porp.ResourceType=0;
		else if(m_tempSet.m_LocalState==_T("������"))
			temp_porp.ResourceType=1;
		//װ�����Ԫ��
		temp_porp.IsAsmComp=true;
		//���ģ�͵�����
		if (m_tempSet.m_MdlNum>0)
		{
			m_pSet.m_strFilter.Format("[Number]=%ld",m_tempSet.m_MdlNum);
			m_pSet.Requery();
			if (!m_pSet.IsBOF())
			{
				m_pSet.MoveFirst();
				temp_porp.MdlName=m_pSet.m_Name;
			}
		}
		else temp_porp.MdlName="��ģ��";
		//��ģ�͵���Ϣ��ӵ�ȫ�ֱ�����
		SetMdlInfo((ProSolid)mdl,temp_porp);

		//���������Ϣ �������ģ�͵�ʵ������ģ�͵���Ϣ��ӵ�ʵ���У�
		//ע�⣬�������������������ģ�͵Ļ�����Ϣ֮�����
		//��Ϊ�����ڲ�Ҫ�޸�ģ�ͻ�����Ϣ
		if (temp_porp.MdlType==1)
		{
			UsrAddPartMdlOtherInfo(m_pSet.m_pDatabase,mdl,temp_porp.MdlNum);
		}
	}
	return TRUE;
}
/************************************************************************/
/* ��ɹ�ϵ��                                                           */
/************************************************************************/
void CAsmAddComplg::OnAsmBom()
{
	// TODO: �ڴ���������������
}
/************************************************************************/
/* ����嵥                                                             */
/************************************************************************/
void CAsmAddComplg::OnAsmPrtList()
{
	// TODO: �ڴ���������������
}
/************************************************************************/
/* �ʼ��˵�                                                             */
/************************************************************************/
void CAsmAddComplg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: �ڴ������Ϣ����������
	//���ģʽ��
	if (m_Tab.GetCurSel()==1 && WindowFromPoint(point)==&m_Tree[1])
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(8);
		ASSERT(pPopup!=NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	}
}

/************************************************************************/
/* ����ѵ����ģ�͸��ݲ�ͬ��ʵ�����и��� 
��������   ģ����_ʵ����.��չ��       (����ͼҲҪ����)             */
/************************************************************************/
BOOL CAsmAddComplg::CompRenameWithInsNum(long InsNum, ProMdl mdl)
{
	//ProMdlType      type;
	//ProName         oldName;
	//CString         newName;
	//ProError        status;
	//CString         error;
	//   CArray<ProDrawing,ProDrawing>  p_DrawingList;
	//ProMdlTypeGet(mdl,&type);
	//ProMdlNameGet(mdl,oldName);
	//newName.Format("%s_%d",CString(oldName),InsNum);
	//ProStringToWstring(oldName,newName.GetBuffer());
	//status=ProMdlRename((ProMdl)mdl,oldName);
	////����ͬ��
	//if(status!=PRO_TK_NO_ERROR)
	//{
	//	error.Format("�ڴ��б��д��ڡ�%s%s����ģ�ͣ�\n���ļ���ѡ���ģ���ļ�ͬ�������飡"
	//		,CString(oldName),type==PRO_MDL_ASSEMBLY?".asm":".prt");
	//	AfxMessageBox(error);
	//	return FALSE;
	//}

	//p_UsrPartPorp[p_UsrPartPorp.GetCount()-1].FileName.Format("%s.%s",newName,type==PRO_MDL_ASSEMBLY?".asm":".prt");

	////���ģ�͵Ĺ���ͼ
	//GetSolidDrawing((ProSolid)mdl,&p_DrawingList);
	////���Ĺ���ͼ������
	//for (int i=0;i<(int)p_DrawingList.GetCount();i++)
	//{
	//	ProMdlNameGet((ProMdl)p_DrawingList[i],oldName);
	//	newName.Format("%s_%d",CString(oldName),InsNum);
	//	ProStringToWstring(oldName,newName.GetBuffer());
	//	ProMdlRename((ProMdl)p_DrawingList[i],oldName);
	//}

	//ProSolidRegenerate((ProSolid)mdl,0);
	return TRUE;
}
/************************************************************************/
/* ȡ��װ�䣬��װ��ʧ��֮��ɾ��Ԫ��                                   */
/************************************************************************/
BOOL CAsmAddComplg::CancelAssmeblyOrFailure(ProSolid assmbly, ProAsmcomp * feature,long InsNum)
{
	//   int                            feat_ids[1];
	//ProSolid                       solid;
	//CArray<ProDrawing,ProDrawing>  p_DrawingList;
	//feat_ids[0]=feature->id;
	//ProAsmcompMdlGet(feature,(ProMdl*)&solid);	
	//ProFeatureDelete(assmbly,feat_ids,1,NULL,0);
	////ɾ��ģ�ͣ���Ϊ���������һ��������±��Σ�����װ����б�����еģ�����Ҫ���ģ�;�����乤��ͼ,Flag1=1��
	//int Flag=0;
	////�ж���ǰ�Ƿ��Ѿ���װ���������ǣ��򲻴��ڴ������
	//for (int j=0;j<p_AsmCompItemList.GetCount();j++)
	//{
	//	if (p_AsmCompItemList[j].mdlNum==mdlSelectNumber && p_AsmCompItemList[j].insNum==InsNum 
	//		&& p_AsmCompItemList[j].owner == assmbly)   
	//		//ע�⣺���ɾ����Ԫ������û��Ԫ�������ڴ��ţ�������û��ɾ�����ڽ�ģ��֮��Ҫ����
	//	{
	//		Flag=1;
	//		return TRUE;
	//	}
	//}
	//if(Flag==0) 
	//{
	//	GetSolidDrawing(solid,&p_DrawingList);
	//	for (int i=0;i<(int)p_DrawingList.GetCount();i++) {ProMdlErase(p_DrawingList[i]);}
	//	ProMdlErase(solid);
	//}
	return TRUE;
}
/************************************************************************/
/* ��ģ�͵����ڴ�(���)                                                 */
/************************************************************************/
ProMdl CAsmAddComplg::MdlRetrieve(int type, long mdlNum)
{
	//ProPath                 name;
	//ProMdl                  p_handle;
	//ProMdldata              data;
	//ProError                status;
	//CString                 currentDit;
	//CFtpFile                m_ftp;
	//CString                 fullFileName;//����·�����ļ���
	//   CString                 new_name;
	//CpartDrawingSet         m_Set(m_pSet.m_pDatabase);
	//ProDrawing              p_dwing;
	//ProSolid                assmbly;
	//ProName                 tempCheckMdlName;
	//CString                 cstempCheckMdlName;
	//ProMdl *                mdllist;
	//int                     p_count;
	//status=ProMdlCurrentGet((ProMdl*)&assmbly);
	//status=ProMdlDataGet(assmbly,&data);
	//currentDit=CString(data.device)+":"+CString(data.path);
	//new_name=_T("VdTemp_");
	////���ز���ģ��
	//if (mdlNum!=-1) {
	//		if (m_pSet.IsOpen()) 
	//			m_pSet.Close();

	//		m_pSet.m_strFilter.Format("[Number]=%ld",mdlNum);
	//		m_pSet.Open();

	//		if (m_pSet.IsBOF()) 
	//			{AfxMessageBox("ģ���Ѿ������ڣ����ܴ򿪣�");m_pSet.Close();return NULL;}
	//		m_pSet.MoveFirst();

	//		//1.�жϵ�ǰ�ڴ����Ƿ��и�ģ��ͬ�����ļ��Ѿ���
	//		status=ProSessionMdlList((type==0?PRO_MDL_PART:PRO_MDL_ASSEMBLY),&mdllist,&p_count);
	//		for (int i=0;i<p_count;i++)
	//		{
	//			ProMdlNameGet(mdllist[i],tempCheckMdlName);
	//			cstempCheckMdlName.Format("%s%s",CString(tempCheckMdlName),type==0?".prt":".asm");
	//			if(cstempCheckMdlName.MakeLower()==m_pSet.m_FileName.MakeLower())
	//			{
	//				if(AfxMessageBox("�Ѿ���һ����ѡ��ģ���ļ�ͬ����ģ��,\n�Ƿ����װ�䣿",MB_YESNO)==IDYES)
	//				{
	//					return (ProSolid)mdllist[i];
	//				}
	//				ProArrayFree((ProArray*)&mdllist);
	//				return NULL;
	//			}
	//		}

	//		ProArrayFree((ProArray*)&mdllist);
	//		//2.��鵱ǰ�Ƿ����Ѿ�������ͬ���ļ���
	//		for (int i=0;;i++)
	//		{
	//			CString version;				
	//			version.Format("%s%s(%d)",new_name,m_pSet.m_Name,i);
	//			version=currentDit+version;
	//			if (CreateDirectoryA(version,NULL))
	//			{
	//				currentDit=version;
	//				break;
	//			}
	//			if(i>100) 
	//			{
	//				AfxMessageBox("ģ�����ع����У��޷��ڱ��ش�����ʱĿ¼");
	//				return NULL;
	//			}
	//		}

	//		//3.�ӷ������õ���ģ�͵��ļ�
	//		if (!m_ftp.DownloadDirectorytoDirectory(m_pSet.m_FileDirectory,currentDit,myFtp))
	//		{
	//			AfxMessageBox("�ӷ�������ȡ�ļ������в��ɹ�����������");
	//			m_pSet.Close();
	//			return NULL;
	//		}
	//		fullFileName=currentDit+"\\"+m_pSet.m_FileName;
	//		ProStringToWstring(name,fullFileName.GetBuffer());
	//		status=ProMdlLoad(name,PRO_MDL_UNUSED ,PRO_B_FALSE,&p_handle);
	//		if (status!=PRO_TK_NO_ERROR)
	//		{
	//			AfxMessageBox("û�ܴ����ص����ص��ļ�������汾��");
	//			m_pSet.Close();
	//			return NULL;
	//		}
	//		//4.����ģ�����еĹ���ͼ
	//		m_Set.m_strFilter.Format("[PrtNum]=%ld",mdlNum);
	//		m_Set.Open();
	//		if(!m_Set.IsBOF())
	//		{
	//			m_Set.MoveFirst();
	//			do {
	//				fullFileName=currentDit+"\\drawing\\"+m_Set.m_FileName;
	//				ProStringToWstring(name,fullFileName.GetBuffer());
	//				status=ProMdlLoad(name,PRO_MDL_UNUSED ,PRO_B_FALSE,(ProMdl*)&p_dwing);
	//				if (status!=PRO_TK_NO_ERROR)
	//				{
	//					AfxMessageBox("����ģ�͵Ĺ���ͼʧ��");
	//					return p_handle;
	//				}
	//				m_Set.MoveNext();
	//			} while(!m_Set.IsEOF());
	//		}
	//		m_pSet.Close();
	//}
	return NULL/*p_handle*/;
}

/************************************************************************/
/* װ��Ԫ��                                                             */
/************************************************************************/
BOOL CAsmAddComplg::AssembleComp(ProSolid solid/*ʵ���ľ��*/,long InsNum/*ʵ����*/)
{
	ProSolid		assmbly;
	ProMdlType      type;
	ProMatrix		init_pos= {{ 1.0, 0.0, 0.0, 0.0 }, 
	{0.0, 1.0, 0.0, 0.0}, 
	{0.0, 0.0, 1.0, 0.0}, 
	{0.0, 0.0, 0.0, 1.0}};
	ProAsmcomp		feature; 
	ProError		status;
	status=ProMdlCurrentGet((ProMdl*)&assmbly);
	status=ProMdlTypeGet((ProMdl)assmbly,&type);
	if (type==PRO_MDL_ASSEMBLY)
	{
		//1.װ��
		status=ProAsmcompAssemble(assmbly,solid,init_pos,&feature);
		status=ProAsmcompConstrRemove(&feature,-1);
		status=ProAsmcompConstrRedefUI(&feature);
		//ȡ��װ���װ�䲻�ɹ�
		if(status==PRO_TK_USER_ABORT || status==PRO_TK_BAD_INPUTS)
		{
			CancelAssmeblyOrFailure(assmbly,&feature,InsNum);
			return FALSE;
		}
		//2.������Ϣ�����ģ���Ѿ����ڣ�����Ҫ��д
		int Flag=0;
		for (int j=0;j<p_AsmCompItemList.GetCount();j++)
		{
			if (p_AsmCompItemList[j].mdlNum==mdlSelectNumber && p_AsmCompItemList[j].insNum==InsNum 
				&& p_AsmCompItemList[j].owner == assmbly) 
			{
				Flag=1;
				return TRUE;
			}
		}
		//2.2װ��ɹ��󣬰�ģ�͵���Ϣ�����ڴ�(�µ�ģ��)
		if (Flag==0)
		{	//2.2.1		
			//AddPartMdlInfo((ProMdl)solid,InsNum);
			//2.2.2����ģ�͵�ȱʡϵ�кã���ģ���ļ�����������
			if(!CompRenameWithInsNum(InsNum,(ProMdl)solid)) 
			{
				CancelAssmeblyOrFailure(assmbly,&feature,InsNum);
				return FALSE;
			}
			//2.2.3�ѵ�ǰѡ���ģ�͵�ʵ����ϵ�У���Ϣ����Ϊȱʡֵ
			AsmCompVarItem	temp_item;
			CompInsItem		item;
			temp_item.owner=assmbly;
			temp_item.comp=solid;
			temp_item.mdlNum=mdlSelectNumber;
			temp_item.type=ASM_MDL_PART;
			temp_item.insNum=InsNum;
			temp_item.varFeild=new CArray<CompInsItem,CompInsItem>();
			if(GetMdlInsInfo(m_pSet.m_pDatabase,ASM_MDL_PART,mdlSelectNumber,InsNum,&item)) 
				temp_item.varFeild->Add(item);
			temp_item.propNum=(int)p_UsrPartPorp.GetCount()-1;
			p_AsmCompItemList.Add(temp_item);
		}
		return TRUE;
	}
	return FALSE;
}
void CAsmAddComplg::OnTvnSelchangedPartinsTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	long							num=(long)pNMTreeView->itemNew.lParam;
	CNPartInsInfoTable				m_Set1(m_pSet.m_pDatabase);
	m_Set1.m_strFilter.Format("[TypeNum]=%ld",num);
	m_Set1.Open();	
	m_List.DeleteAllItems();
	m_List2.DeleteAllItems();

	m_btnOpen.EnableWindow(FALSE);
	if (!m_Set1.IsBOF())
	{
		m_btnOpen.EnableWindow(TRUE);
		m_Set1.MoveFirst();
		while (!m_Set1.IsEOF())
		{
			m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List.GetItemCount(),m_Set1.m_Name,0,0,0,m_Set1.m_PrtNum);
			m_List.SetItemText(m_List.GetItemCount()-1,1,m_Set1.m_InsCode);
			m_List.SetItemText(m_List.GetItemCount()-1,2,(m_Set1.m_FinishState==_T("���")?_T("���"):_T("δ���")));
			m_List.SetItemText(m_List.GetItemCount()-1,3,(m_Set1.m_AuditState==_T("���ͨ��")?_T("���ͨ��"):_T("���δͨ��")));
			m_List.SetItemText(m_List.GetItemCount()-1,4,(m_Set1.m_PDMState==_T("�ύ")?_T("�ύ"):_T("δ�ύ")));
			m_List.SetItemText(m_List.GetItemCount()-1,5,(m_Set1.m_PDMState==_T("�ύ")?"PDMϵͳ":(m_Set1.m_LocalState=="������"?"���ϵͳ":"�û�����")));
			m_List.SetItemText(m_List.GetItemCount()-1,6,m_Set1.m_Designer);
			m_List.SetItemText(m_List.GetItemCount()-1,7,m_Set1.m_Note);
			m_Set1.MoveNext();
		}
	}
	*pResult = 0;
}

void CAsmAddComplg::OnTvnSelchangedAsminsTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	long							num=(long)pNMTreeView->itemNew.lParam;
	CNProductInsInfoTable   		m_Set1(m_pSet.m_pDatabase);
	m_Set1.m_strFilter.Format("[TypeNum]=%ld",num);
	m_Set1.Open();	
	m_List.DeleteAllItems();
	m_List2.DeleteAllItems();
	m_btnOpen.EnableWindow(FALSE);
	if (!m_Set1.IsBOF())
	{
		m_btnOpen.EnableWindow(TRUE);
		m_Set1.MoveFirst();
		while (!m_Set1.IsEOF())
		{
			m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,m_List.GetItemCount(),m_Set1.m_Name,0,0,0,m_Set1.m_PdtNum);
			m_List.SetItemText(m_List.GetItemCount()-1,1,m_Set1.m_InsCode);
			m_List.SetItemText(m_List.GetItemCount()-1,2,(m_Set1.m_FinishState==_T("���")?_T("���"):_T("δ���")));
			m_List.SetItemText(m_List.GetItemCount()-1,3,(m_Set1.m_AuditState==_T("���ͨ��")?_T("���ͨ��"):_T("���δͨ��")));
			m_List.SetItemText(m_List.GetItemCount()-1,4,(m_Set1.m_PDMState==_T("�ύ")?_T("�ύ"):_T("δ�ύ")));
			m_List.SetItemText(m_List.GetItemCount()-1,5,(m_Set1.m_PDMState==_T("�ύ")?"PDMϵͳ":(m_Set1.m_LocalState=="������"?"���ϵͳ":"�û�����")));
			m_List.SetItemText(m_List.GetItemCount()-1,6,m_Set1.m_Designer);
			m_List.SetItemText(m_List.GetItemCount()-1,7,m_Set1.m_Note);
			m_Set1.MoveNext();
		}
	}
	*pResult = 0;
}
