// PartMainDlg.cpp : ʵ���ļ�
//���Ŀ¼�����Լ������ģ��������
//�����Ŀ¼�������ԡ���ϵʽ���������Ա����ܲ�����ͼƬ����ȹ���

#include "stdafx.h"
#include "Main.h"
#include "PartMainDlg.h"
#include "PartMdlRecordSet.h"
#include "NPartInsInfoTable.h"
#include "NewPartTypeDlg.h"
#include "MdlOkDlg.h"
#include "partVariantTableSet.h"
#include "partPerfparaTableSet.h"
#include "partRelationSet.h"
#include "partRelativeDimParaTableSet.h"
#include "partPicSet.h"
//#include "PartSeriesDlg.h"
#include "AddMdlSeriesDlg.h"
#include "UsrSWTXTable.h"
#include "SignIn.h"
/************************************************************************/
/*  ���ط� ΪȨ�޹������ӳ���                                         */

//�û���Ϣ�ṹ��
CString  strtheright;





/************************************************************************/

IMPLEMENT_DYNAMIC(CPartMainDlg, CDialog)
CPartMainDlg::CPartMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartMainDlg::IDD, pParent)
	, part_solid(NULL)
{
	
}

CPartMainDlg::~CPartMainDlg()
{
}

void CPartMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_Tree);
}


BEGIN_MESSAGE_MAP(CPartMainDlg, CDialog)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_NEW_PARTTYPE, OnNewParttype)
	ON_WM_CLOSE()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_COMMAND(ID_DEL_PARTTYPE, OnDelParttype)
	ON_NOTIFY(NM_CLICK, IDC_TREE, OnNMClickTree)
	ON_COMMAND(ID_L_, OnSelectTypeForMdl)
	ON_COMMAND(ID_L_12105, OnRefreshTree)
	ON_COMMAND(ID_DEL_PARTMDL, OnDelPartmdl)
	/*ON_COMMAND(ID_L_12108, OnL12108)*/
	ON_COMMAND(ID_Menu12107, OnMenuMdlInfo)
	ON_COMMAND(ID_UPDATE, OnUpdateMdl)
END_MESSAGE_MAP()


// CPartMainDlg ��Ϣ�������



BOOL CPartMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(ProMdlCurrentGet((ProMdl*)&part_solid)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ǰû��ģ��");
		OnClose();
		return FALSE;
	}
	//����ͼƬ�б�
	m_ImageList.Create(14, 14, ILC_COLOR32, 4, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));//��Ҫ�޸ĳ�ʵ����ͼ��
	m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	//��������ҳ
	SetSheepPage();
	//��ʾĿ¼��
	if(m_pSet.Open())
	{
		ShowNPartTree(&m_Tree,m_pSet.m_pDatabase,true);
	}
	else
	{
		AfxMessageBox("���ݿ����Ӳ��ɹ���");
	}
	return TRUE;                                     // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ���е��Ҽ��˵�����                                                   */
/************************************************************************/
void CPartMainDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �ڴ������Ϣ����������
	CPoint point=GetCurrentMessage()->pt;
	if(WindowFromPoint(point)==&m_Tree)
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(0);
		//����ͼƬ���ͣ�ѡ��˵��Ƿ���Ч
		HTREEITEM hItem=m_Tree.GetSelectedItem();
		TVITEM iTem;
		iTem.hItem=hItem;
		iTem.mask=TVIF_IMAGE;
		m_Tree.GetItem(&iTem);
		//�½����ID_NEW_PARTTYPE ɾ����� ID_DEL_PARTTYPE
		//ɾ��ģ��ID_DEL_PARTMDL  ģ��ϵ�л� ID_L_12108
		//�����ǰ�������ģ�ͣ��򲻳��ָ���ģ�Ͱ�ť

		//ɾ��ģ��ϵ�л���ť
		
		if (iTem.iImage!=1) {
			//���ѡ�еĲ������ģ��
			pPopup->EnableMenuItem(ID_DEL_PARTMDL,MF_DISABLED|MF_GRAYED);//ɾ��ģ��
			//pPopup->EnableMenuItem(ID_L_12108,MF_DISABLED|MF_GRAYED);    //ģ��ϵ�л�
			pPopup->EnableMenuItem(ID_UPDATE,MF_DISABLED|MF_GRAYED );    //����ģ��
			pPopup->EnableMenuItem(ID_Menu12107,MF_DISABLED|MF_GRAYED);
		}
		//���ѡ�еĲ������
		if (iTem.iImage!=0)
		{
			pPopup->EnableMenuItem(ID_NEW_PARTTYPE,MF_DISABLED|MF_GRAYED);//�½����
			pPopup->EnableMenuItem(ID_DEL_PARTTYPE,MF_DISABLED|MF_GRAYED);//ɾ�����
			pPopup->EnableMenuItem(ID_L_,MF_DISABLED|MF_GRAYED);          //ѡ�����		
		}
		if (GetMdlInfo(part_solid).MdlType!=3 || GetMdlInfo(part_solid).Number!=(long)m_Tree.GetItemData(hItem))
		{
			pPopup->EnableMenuItem(ID_UPDATE,MF_DISABLED|MF_GRAYED);                   //�Ƴ�����ģ��
		}
		ASSERT(pPopup!=NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);

	}
}

/***************************************************************************************\
������OnNewParttype()
˵���������ؼ��ϵ���Ҽ�����½����,ֻ����ѡ�������������Ч
\***************************************************************************************/
void CPartMainDlg::OnNewParttype()
{
	// TODO: �ڴ���������������

	CNewPartTypeDlg dlg;
	long			i=0;
	long            selNum;
	CString         selName;
	CString         selCode;
	CString         newName;	
	long            newNum;
	CString            newCode;
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
		//�ж����������Ƿ����
		try
		{
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
/* �Ҽ��˵�ɾ�����                                                     */
/************************************************************************/
void CPartMainDlg::OnDelParttype()
{
	// TODO: �ڴ���������������
	long                             i=0;
	CPartMdlRecordSet                m_set(m_pSet.m_pDatabase);
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
void CPartMainDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UsrActivateCurrentWindow();
	CDialog::OnClose();
}

void CPartMainDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	// TODO: �ڴ������Ϣ����������
    UsrRepaintWindow();
}
/************************************************************************/
/* // ���ݸ�ѡ���������������Ӧ��Ҫ��ʾ��ҳ��                          */
/************************************************************************/
void CPartMainDlg::SetSheepPage(void)
{	
    //�����ѡ��Ϊָ����ǰģ��
	SetSheetOfCurrentMdl();
}

/************************************************************************/
/* �����ɽ�ģ֮��Ķ���                                               */
/************************************************************************/
void CPartMainDlg::OnBnClickedButton2()
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
		CMdlOkDlg dlg;
		dlg.part_solid=part_solid;
		dlg.SetDbConnet(m_pSet.m_pDatabase);
		if (dlg.DoModal())
		{
			ShowNPartTree(&m_Tree,m_pSet.m_pDatabase,true);
		}
	//}
	//else
	//	MessageBox("��Ȩ�ޣ�","��ʾ��Ϣ");

}
/************************************************************************/
/* ��ǰģ��ģʽ�£���ʾ��ǰģ�͵��������Ա����ܲ�������ϵʽ��ͼƬ����Ϣ*/
/************************************************************************/
int CPartMainDlg::SetSheetOfCurrentMdl(void)
{

	ProSolid          solid;                  //���õ�ǰģ�͵�����
	ProMdlCurrentGet((ProMdl*)&solid);	
    //IsMdlInfoDefAndDef(solid);               //�ж�ģ�͵Ļ�����Ϣ�Ƿ��Ѿ����壬���û������
 /*   m_page4.SetValue(solid,&p_UsrPartPorp);*/
	m_sheet.AddPage(&m_page4); 
	m_page1.SetValue(solid,&p_UsrSWTXTableList);
	m_sheet.AddPage(&m_page1);               //�������Ա�	
	m_page2.SetValue(solid,&p_UsrPerfParaTableList);	
	m_sheet.AddPage(&m_page2);               //���ܲ�����	
	m_page3.SetValue(solid/*,&p_UsrRelTableList*/);		
	m_sheet.AddPage(&m_page3);               //��ϵʽ
	m_page5.SetValue(solid,&p_UsrPicture);		
	m_sheet.AddPage(&m_page5);                    //ͼƬ���ҳ
	//��Create������һ������ҳ
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE|WS_GROUP, WS_EX_CONTROLPARENT);
	//��������ҳ�Ĵ�С��λ��		
	m_sheet.SetWindowPos(NULL, 0, 0,0,0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	return 0;
}
/************************************************************************/
/* �����οؼ��У������������Ķ���                                     */
/************************************************************************/
void CPartMainDlg::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}
/************************************************************************/
/* ѡ�����                                                             */
/************************************************************************/
void CPartMainDlg::OnSelectTypeForMdl()
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
			ShowNPartTree(&m_Tree,m_pSet.m_pDatabase);
			return;
		}
		m_pSet.MoveFirst();
        //��ѡ�е��������ģ��
		m_page4.m_TypeEdit.SetWindowText(m_pSet.m_Name);
		m_page4.m_CodeEdit.SetWindowText(m_pSet.m_Code+".(������)");
        SetMdlInfo(solid)->TypeNum=m_pSet.m_Number;
		SetMdlInfo(solid)->TypeName=m_pSet.m_Name;

		//-------------------whsf---------------
		CString strtheName;
		HTREEITEM hParent,hSelected,hTest;
		hSelected=hItem;
		hTest=hItem;
		do {
			strtheName=m_Tree.GetItemText(hTest);
			if(strtheName=="ͨ�ü�(TYJ)")
			{
				strtheright="ͨ�ü���ģ";
				return;
			}
			if(strtheName=="�����ƶ�����(ZDQS)")
			{
				strtheright="�����ƶ����������ģ";
				return;
			}
			if(strtheName=="�����������(ZLQ)")
			{
				strtheright="����������������ģ";
				return;
			}
			if(strtheName=="ת��������(ZDB)")
			{
				strtheright="ת�������������ģ";
				return;
			}
			if(strtheName=="�ƶ��ܱ÷ֱ�(ZLB)")
			{
				strtheright="�ƶ��ܱ÷ֱ������ģ";
				return;
			}
			if(strtheName=="��Ͽ���(ZHKG)")
			{
				strtheright="��Ͽ��������ģ";
				return;
			}
			if(strtheName=="����Ǳ�(ZHYB)")
			{
				strtheright="����Ǳ������ģ";
				return;
			}
			if(strtheName=="�绺����(HSQ)")
			{
				strtheright="�绺���������ģ";
				return;
			}
			hParent=m_Tree.GetParentItem(hTest);
			hTest=hParent;
		} while(m_Tree.GetParentItem(hTest));
	}
	catch (CDBException * e) {		
		AfxMessageBox(e->m_strError);	
		
	}	
	return;
}


void CPartMainDlg::OnRefreshTree()
{
	// TODO: �ڴ���������������
	ShowNPartTree(&m_Tree,m_pSet.m_pDatabase,true);
}

void CPartMainDlg::OnDelPartmdl()
{
	// TODO: �ڴ���������������
	HTREEITEM							hItem=m_Tree.GetSelectedItem();
	if(hItem==m_Tree.GetRootItem())		return;
	if (UsrDelPartMdl(m_pSet.m_pDatabase,(long)m_Tree.GetItemData(hItem)))
	{
		m_Tree.DeleteItem(hItem);	
	}
	else
	{
		AfxMessageBox("ɾ�����ɹ�");
	}
}
///************************************************************************/
///* �Ҽ��˵���ģ��ϵ�л�                                                 */
///************************************************************************/
//void CPartMainDlg::OnL12108()
//{
//	// TODO: �ڴ���������������
//	//CPartSeriesDlg dlg;
//	//long          Number=0;
//	//HTREEITEM hItem=m_Tree.GetSelectedItem();
//	//if(hItem==m_Tree.GetRootItem()) return;
// //   Number=(int)m_Tree.GetItemData(hItem);
//	//if(m_pSet.IsOpen()) m_pSet.Close();
//	//m_pSet.m_strFilter.Format("[Number]=%d",Number);
//	////�������ݿ����Ӻͷ���������
//	//if(!m_pSet.Open())
//	//{
//	//	AfxMessageBox("���ݲ��ܴ򿪣�");
//	//	return;
//	//}
//	//if(m_pSet.IsBOF()) 
//	//{
//	//	m_pSet.Close();
//	//	return;
//	//}
//	//m_pSet.MoveFirst();
//	//if(m_pSet.m_Type!="mdl") 
//	//{
//	//	m_pSet.Close();
//	//	return;
//	//}
//	//dlg.SetMdlNumberAndDB(Number,m_pSet.m_pDatabase);
//	//dlg.DoModal();
//
//}
/************************************************************************/
/* �鿴ģ�͵�����                                                       */
/************************************************************************/
void CPartMainDlg::OnMenuMdlInfo()
{
	// TODO: �ڴ���������������
	CAddMdlSeriesDlg dlg;
	CPartMdlRecordSet m_Set(m_pSet.m_pDatabase);
	MdlInsInfo       mdlInfo;
	HTREEITEM hItem=m_Tree.GetSelectedItem();
	if (m_Tree.GetRootItem()==hItem) return;
	long number=(long)m_Tree.GetItemData(hItem);
	try
	{		
		m_Set.m_strFilter.Format("[Number]=%d",number);
		m_Set.Open();
		if (m_Set.IsBOF()) {
			AfxMessageBox("ģ�Ͳ�����");
			return;
		}
		m_Set.MoveFirst();
        mdlInfo.Name=m_Set.m_Name;
		mdlInfo.Code=m_Set.m_Code;
		mdlInfo.Person=m_Set.m_Designer;
		mdlInfo.Note=m_Set.m_Note;
		mdlInfo.m_time=m_Set.m_Datatime;
		mdlInfo.mdlNum=number;
		mdlInfo.Dept=_T("");
		dlg.newMdl=mdlInfo;
		dlg.strCaption="�鿴������Ϣ";
		dlg.SetMdlNumberAndCode(-1,"",NULL);
		dlg.DoModal();
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		delete e;
	}
	return;
}
/************************************************************************/
/* �������ģ�ͣ���ǰģ�ͱ�����ģ�ͣ����Ѿ���ӵ�ģ�Ϳ���               */
/************************************************************************/
void CPartMainDlg::OnUpdateMdl()
{
	// TODO: �ڴ���������������
	CMdlOkDlg  updatamdl;
	if (m_Tree.GetItemData(m_Tree.GetSelectedItem()) != GetMdlInfo(part_solid).Number )
	{
		return;
	}
	if (updatamdl.UpdateMdl(part_solid,GetMdlInfo(part_solid).Number,m_pSet.m_pDatabase))
	{
		m_Tree.SetItemText(m_Tree.GetSelectedItem(),GetMdlInfo(part_solid).Name+GetMdlInfo(part_solid).Code);
	}
}

int CPartMainDlg::getRights(CString strAuthor)
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

BOOL CPartMainDlg::IsHasRight(CString theRight)
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
