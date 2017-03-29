// AsmMdlManDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "AsmMdlManDlg.h"
#include "NewPartTypeDlg.h"
#include "AsmMdlOkDlg.h"
#include "asmmanagetableset.h"
#include "checkmdlifhaschange.h"
#include "NProductInsInfoTable.h"
#include "SignIn.h"
// CAsmMdlManDlg �Ի���

extern CString strtheright;

IMPLEMENT_DYNAMIC(CAsmMdlManDlg, CDialog)
CAsmMdlManDlg::CAsmMdlManDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmMdlManDlg::IDD, pParent)
{
}

CAsmMdlManDlg::~CAsmMdlManDlg()
{
}

void CAsmMdlManDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_ASM_MDL_TREE, m_Tree);
}


BEGIN_MESSAGE_MAP(CAsmMdlManDlg, CDialog)
	ON_COMMAND(ID_12008, OnNewType)
	ON_COMMAND(ID_Menu12136, OnSelType)
	ON_COMMAND(ID_12010, OnDelType)
	ON_COMMAND(ID_12011, OnDelMdl)
	ON_COMMAND(ID_12134, OnMdlSeries)
	ON_COMMAND(ID_12135, OnFresh)
	ON_COMMAND(ID_12012, OnViewProp)
	ON_MESSAGE(WM_LIST_OK,OnListDlgOk)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(1005, OnBnClicked1005)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_COMMAND(ID_UPDATE, OnUpdateMdl)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CAsmMdlManDlg ��Ϣ�������

void CAsmMdlManDlg::SetSheetPage(void)
{
	ProSolid solid;                  //���õ�ǰģ�͵�����
	ProMdlCurrentGet((ProMdl*)&solid);	
	//IsMdlInfoDefAndDef(solid);       //�ж�ģ�͵Ļ�����Ϣ�Ƿ��Ѿ����壬���û������
	//m_page1_Info.SetValue(solid,&p_UsrPartPorp);
	m_sheet.AddPage(&m_page1_Info);
	m_page2_Tree.mdl=solid;
	m_sheet.AddPage(&m_page2_Tree);
	m_page3_Swtx.mdl=solid;
	m_sheet.AddPage(&m_page3_Swtx);
	m_page4_Perf.mdl=solid;
	m_sheet.AddPage(&m_page4_Perf);
	m_page5_Rel.mdl=solid;
	m_sheet.AddPage(&m_page5_Rel);
	m_page6_Pic.mdl=solid;
	m_sheet.AddPage(&m_page6_Pic);
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE|WS_GROUP, WS_EX_CONTROLPARENT);
	m_sheet.SetWindowPos(NULL, 0, 0,0,0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

}

BOOL CAsmMdlManDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(ProMdlCurrentGet((ProMdl*)&mdl)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ǰû��ģ��");
		OnClose();
		return FALSE;
	}

	m_ImageList.Create(14, 14, ILC_COLOR32, 4, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));


	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	if (m_pSet.Open())
	{
		ShowNAsmTree(&m_Tree,m_pSet.m_pDatabase,true);
	}
	else
	{
		AfxMessageBox("���ݿ����Ӳ��ɹ���");
	}
    SetSheetPage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* �Ҽ��˵������½����                                                 */
/************************************************************************/
void CAsmMdlManDlg::OnNewType()
{
	// TODO: �ڴ���������������

	CNewPartTypeDlg dlg;
	long			i=0;
	long            selNum;
	CString         selName;
	CString         selCode;
	CString         newName;	
	long            newNum;
	CString         newCode;
	int				imageNum;
	int				selImageNum;
	HTREEITEM		hItem=m_Tree.GetSelectedItem();
	//�ж�ѡ�е��Ƿ�Ϊ����ͼƬ
	if (hItem==m_Tree.GetRootItem())
	{
		selNum  = 0;
		selName = "��Ŀ¼";
		selCode = ""; 
	}
	else
	{
		m_Tree.GetItemImage(hItem,imageNum,selImageNum);
		if (imageNum!=0) return;
		m_pSet.m_strFilter.Format("[Number]=%ld",(long)m_Tree.GetItemData(hItem));
		m_pSet.Requery();
		if (!m_pSet.IsBOF())
		{
			m_pSet.MoveFirst();
			selName = m_pSet.m_Name;
			selNum  = m_pSet.m_Number;
			selCode = m_pSet.m_Code;
		}
		else
		{
			return;
		}
	}
	dlg.SetFatherName(selName,selCode);
	if (dlg.DoModal()==IDOK)
	{
		newName=dlg.m_TypeName;
		newCode=dlg.m_Code;
		//�Ժ����Ҫ������ı�ע 
		try
		{
			//�ж����������Ƿ����
			m_pSet.m_strFilter.Format("[FatherNum]=%ld AND [Name]='%s'",selNum,newName.GetBuffer());
			m_pSet.Requery();
			if (m_pSet.IsBOF())
			{
				m_pSet.m_strFilter.Empty();
				m_pSet.Requery();
				m_pSet.AddNew();
				m_pSet.m_Code=newCode;
				m_pSet.m_Name=newName;
				m_pSet.m_FatherNum=selNum;
				m_pSet.Update();
				m_pSet.m_strFilter.Empty();
				m_pSet.Requery();
				m_pSet.MoveLast();
				newNum=m_pSet.m_Number;
				m_Tree.InsertItem(TVIF_IMAGE|TVIF_PARAM|TVIF_TEXT|TVIF_SELECTEDIMAGE,newName,0,0,0,0,newNum,hItem,NULL);
				m_Tree.Expand(hItem,TVE_EXPAND);
			}
			else
			{
				AfxMessageBox("��ͬһ������Ѿ�����ͬ�����������");
			}
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			delete    e;
		}
	}
}
/************************************************************************/
/*  �Ҽ��˵�����ѡ�����                                                */
/************************************************************************/
void CAsmMdlManDlg::OnSelType()
{
	// TODO: �ڴ���������������
	long     i=0;
	int		 num=0;
	ProSolid solid;		
	ProMdlCurrentGet((ProMdl*)&solid);	

	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem())     return;
	i=(long)m_Tree.GetItemData(hItem);
	try
	{
		m_pSet.m_strFilter.Format("[Number] = %ld",i);
		m_pSet.Requery();
		if(m_pSet.IsBOF()) 
		{
			AfxMessageBox("��ǰ����Ѿ������ڣ�");
			ShowNAsmTree(&m_Tree,m_pSet.m_pDatabase);
			return;
		}
		m_pSet.MoveFirst();
		//��ѡ�е��������ģ��
		m_page1_Info.m_MdlType=m_pSet.m_Name;
		m_page1_Info.m_MdlCode=m_pSet.m_Code+".(������)";
		m_page1_Info.UpdateData(FALSE);
		SetMdlInfo(solid)->TypeNum=m_pSet.m_Number;
		SetMdlInfo(solid)->TypeName=m_pSet.m_Name;
	}
	catch (CDBException * e) {		
		AfxMessageBox(e->m_strError);	
		
	}	


	//-------------------whsf---------------
	CString strtheName;
	HTREEITEM hParent,hSelected,hTest;
	hSelected=hItem;
	hTest=hItem;
	do {
		strtheName=m_Tree.GetItemText(hTest);
		if(strtheName=="�����ƶ�����(ZDQS)")
		{
			strtheright="�����ƶ����Ҳ�Ʒ��ģ";
			return;
		}
		if(strtheName=="�����������(ZLQ)")
		{
			strtheright="�������������Ʒ��ģ";
			return;
		}
		if(strtheName=="ת��������(ZDB)")
		{
			strtheright="ת�������ò�Ʒ��ģ";
			return;
		}
		if(strtheName=="�ƶ��ܱ÷ֱ�(ZLB)")
		{
			strtheright="�ƶ��ܱ÷ֱò�Ʒ��ģ";
			return;
		}
		if(strtheName=="��Ͽ���(ZHKG)")
		{
			strtheright="��Ͽ��ز�Ʒ��ģ";
			return;
		}
		if(strtheName=="����Ǳ�(ZHYB)")
		{
			strtheright="����Ǳ��Ʒ��ģ";
			return;
		}
		if(strtheName=="�绺����(HSQ)")
		{
			strtheright="�绺������Ʒ��ģ";
			return;
		}
		hParent=m_Tree.GetParentItem(hTest);
		hTest=hParent;
	} while(m_Tree.GetParentItem(hTest));







	return;
}
/************************************************************************/
/* �Ҽ��˵�����ɾ�����                                               */
/************************************************************************/
void CAsmMdlManDlg::OnDelType()
{
	// TODO: �ڴ���������������
	long                             i=0;
	CAsmManageTableSet               m_set(m_pSet.m_pDatabase);
	HTREEITEM                        hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem())  return;
	i=(long)m_Tree.GetItemData(hItem);
	//����Ƿ���ģ�͹������Ŀ¼��
	m_set.m_strFilter.Format("[TypeNum]=%ld",i);
	m_set.Open();
	if (!m_set.IsBOF())
	{
		AfxMessageBox("�����������ģ�ͣ�������ɾ����");
		return;
	}

	m_pSet.m_strFilter.Format("[FatherNum]=%ld",i);
	m_pSet.Requery();
	if (!m_pSet.IsBOF())
	{
		AfxMessageBox("���������������𣬲�����ɾ����");
		return;
	}

	m_pSet.m_strFilter.Format("[Number]=%ld",i);
	m_pSet.Requery();
	if (!m_pSet.IsBOF())
	{
		try
		{
			m_pSet.MoveFirst();
			m_pSet.Delete();
			m_Tree.DeleteItem(hItem);
			m_pSet.Requery();
		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			
		}
	}
}
/************************************************************************/
/* �Ҽ��˵�����ɾ��ģ��                                                 */
/************************************************************************/
void CAsmMdlManDlg::OnDelMdl()
{
	// TODO: �ڴ���������������
	//���
    HTREEITEM							hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem())		return;
	if (UsrDelAsmMdl(m_pSet.m_pDatabase,(long)m_Tree.GetItemData(hItem)))
	{
		m_Tree.DeleteItem(hItem);	
	}
	else
	{
		AfxMessageBox("ɾ�����ɹ�");
	}
}
/************************************************************************/
/* �Ҽ��˵�����ģ��ϵ�л�                                               */
/************************************************************************/
void CAsmMdlManDlg::OnMdlSeries()
{
	// TODO: �ڴ���������������
}
/************************************************************************/
/*  �Ҽ��˵�����ˢ��Ŀ¼                                                */
/************************************************************************/
void CAsmMdlManDlg::OnFresh()
{
	// TODO: �ڴ���������������
	ShowNAsmTree(&m_Tree,m_pSet.m_pDatabase,true);
}
/************************************************************************/
/*  �Ҽ��˵������鿴����                                                */
/************************************************************************/
void CAsmMdlManDlg::OnViewProp()
{
	// TODO: �ڴ���������������
}

void CAsmMdlManDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �ڴ������Ϣ����������
	CPoint point=GetCurrentMessage()->pt;
	if(WindowFromPoint(point)==&m_Tree)
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(1);	
		//����ͼƬ���ж���ѡ�����
		HTREEITEM hItem=m_Tree.GetSelectedItem();
		TVITEM iTem;
		iTem.hItem=hItem;
		iTem.mask=TVIF_IMAGE;
		m_Tree.GetItem(&iTem);
		if (iTem.iImage!=2)
		{
			//���ѡ�еĲ���ģ��
			pPopup->EnableMenuItem(ID_12011,MF_DISABLED|MF_GRAYED);//ɾ��ģ��
			pPopup->EnableMenuItem(ID_UPDATE,MF_DISABLED|MF_GRAYED);//����ģ��
		}
		if (iTem.iImage!=0)
		{
			//���ѡ�еĲ������
			pPopup->EnableMenuItem(ID_12008,MF_DISABLED|MF_GRAYED);//�½����
			pPopup->EnableMenuItem(ID_Menu12136,MF_DISABLED|MF_GRAYED);//ɾ�����
			pPopup->EnableMenuItem(ID_12010,MF_DISABLED|MF_GRAYED);          //ѡ�����
		}
		if (GetMdlInfo(mdl).MdlType!=3 || GetMdlInfo(mdl).Number!=(long)m_Tree.GetItemData(hItem))
		{
			pPopup->EnableMenuItem(ID_UPDATE,MF_DISABLED|MF_GRAYED);   
		}
		ASSERT(pPopup!=NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	}
}
/************************************************************************/
/* ����ģ�Ͱ�ť                                                         */
/************************************************************************/
void CAsmMdlManDlg::OnBnClicked1005()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//---------------wshf--------
	//if(curUserInfo.name=="")
	//{
	//	CSignIn m_dlg;
	//	if(m_dlg.DoModal()==IDOK)
	//	{
	//		getRights(curUserInfo.authority);
	//	}
	//	else
	//		return;
	//}
	//if(IsHasRight(strtheright))
	//{
		CAsmMdlOkDlg	dlg;
		ProSolid		solid;                  //���õ�ǰģ�͵�����
		ProMdlCurrentGet((ProMdl*)&solid);	
		dlg.asm_solid=solid;
		if(!m_pSet.IsOpen()) m_pSet.Open();
		dlg.m_pSet.m_pDatabase=m_pSet.m_pDatabase;
		if (dlg.DoModal()==IDOK)
			ShowNAsmTree(&m_Tree,m_pSet.m_pDatabase,true);	
	//}
	//else
	//	MessageBox("��Ȩ�ޣ�","��ʾ��Ϣ");

















	
}
/************************************************************************/
/* ��װ���������Ա���ת��ģ�͵��������Ա���                             */
/************************************************************************/
BOOL CAsmMdlManDlg::GetSWTEItem(void)
{
	return 0;
}
/************************************************************************/
/* ģ����ҳ��ѡ��ģ�ͱ��η�Χ����Ϣ������                             */
/************************************************************************/
LRESULT CAsmMdlManDlg::OnListDlgOk(WPARAM wParam,LPARAM lParam)
{
	//m_page2_Tree.OnListDlgOk(wParam,lParam);
	return 1;
}
/************************************************************************/
/* ����㲿���Ƿ��иı�                                                 */
/************************************************************************/
void CAsmMdlManDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CArray<ProSolid ,ProSolid>   solidlist;	
	UsrPartPorp					 temp_porp;
	if (!m_pSet.IsOpen())
	{
		if (m_pSet.Open())
		{
			AfxMessageBox("���ݿ�����ʧ��");
			return;
		}
	}
	GetAsmCompHand(mdl,&solidlist);
	CCheckMdlIfHasChange m_check;
	for (int i=0;i<solidlist.GetCount();i++)
	{
		temp_porp=GetMdlInfo(solidlist[i]);
		//��������������ģ��
		if (temp_porp.MdlType==1 && GetMdlType(solidlist[i])==PRO_MDL_PART)
		{
			if (m_check.CheckPartHasPrtMdl(m_pSet.m_pDatabase,solidlist[i],temp_porp.Number,0/*����Ƿ��иı�*/)!=CHECK_TRUE)
			{
			}
		}
	}
}
/************************************************************************/
/* ����ģ��                                                             */
/************************************************************************/
void CAsmMdlManDlg::OnUpdateMdl()
{
	// TODO: �ڴ���������������
	CAsmMdlOkDlg updatamdl;
	if (m_Tree.GetItemData(m_Tree.GetSelectedItem())!= GetMdlInfo(mdl).Number)
	{
		return;
	}
	if (updatamdl.UpdateMdl(mdl,GetMdlInfo(mdl).Number,m_pSet.m_pDatabase))
	{
		m_Tree.SetItemText(m_Tree.GetSelectedItem(),GetMdlInfo(mdl).Name+GetMdlInfo(mdl).Code);
	}
}

void CAsmMdlManDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: �ڴ������Ϣ����������
	UsrRepaintWindow();
}

int CAsmMdlManDlg::getRights(CString strAuthor)
{
	curUserRights.RemoveAll();
	if(strAuthor!="")
	{
		int nStart,nEnd,nLength,nCount;
		nLength=strAuthor.GetLength();
		nStart=0;
		nEnd=0;
		while(nEnd!=nLength-1)
		{
			//nEnd=str.Find(':',nStart);
			//if(nEnd!=-1)
			//{
			//	nCount=nEnd-nStart;
			//	userAttr.strStyle=str.Mid(nStart,nCount);
			//	nStart=nEnd+1;
			//}
			//else
			//	break;
			nEnd=strAuthor.Find('*',nStart);
			if(nEnd!=-1)
			{
				nCount=nEnd-nStart;
				useright.right=strAuthor.Mid(nStart,nCount);
				nStart=nEnd+1;
			}
			else
				break;
			curUserRights.Add(useright);
		}
	}
	return 0;
}

BOOL CAsmMdlManDlg::IsHasRight(CString theRight)
{
	int nCount=0;
	nCount=curUserRights.GetCount();
	if(nCount==0)
		return FALSE;
	UserRight tempUserRight;
	for(int i=0;i<nCount;i++)
	{
		tempUserRight=curUserRights.GetAt(i);
		if(tempUserRight.right=="ϵͳ����Ա")
			return TRUE;
	}
	for(int i=0;i<nCount;i++)
	{
		tempUserRight=curUserRights.GetAt(i);
		if(tempUserRight.right==theRight)
			return TRUE;
	}
	return FALSE;
}
