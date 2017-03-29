// ModelAlterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "ModelAlterDlg.h"
//���PROEͷ�ļ�
#include "ProSolid.h"
#include "ProWindows.h"
#include "ProDimension.h"
#include "ProUtil.h"
#include "ProParameter.h"
#include <ProParamval.h>
#include <ProMdl.h>
#include "ProMessage.h"
#include "dimenchange.h"
#include "dimengroup.h"
#include "MyDatabase.h"
#include "afxtempl.h"
#include <ctype.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <commctrl.h>
#include "ModelQueryDlg.h"
#include "commctrl.h"
#include "Compute.h"
#include "CommonFuncClass.h"
#include "DetailInfoClass.h"
#include "ProductClass.h"


extern CMyDatabase m_db;
//��ϵ�ṹ
CStringArray RelationSet;
CStringArray RelationSetTemp;
CArray<Database,Database> Database_struc;
CArray<Database,Database> Database_struc_temp;
CArray<JuBing,JuBing> JuBing_struc;
extern CPtrArray p_classarray;
// CModelAlterDlg �Ի���

//ȫ�ֱ�������


IMPLEMENT_DYNAMIC(CModelAlterDlg, CDialog)
CModelAlterDlg::CModelAlterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModelAlterDlg::IDD, pParent)
{
//	IsKong=TRUE;
}

CModelAlterDlg::~CModelAlterDlg()
{
}

void CModelAlterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DIMEN, m_showdimen);
}


BEGIN_MESSAGE_MAP(CModelAlterDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DIMEN, OnNMClickListDimen)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DIMEN, OnNMRclickListDimen)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CModelAlterDlg ��Ϣ�������

BOOL CModelAlterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	CString str;					      
	ProSolid solid,ProductSolid;
	ProError status;
	BOOL IsHave=FALSE;
	JuBing uu;
	ProMdlType p_type;
	CString name,post;
	CCompute compute;
	CCommonFuncClass commonfun;
	ListView_SetExtendedListViewStyle(m_showdimen.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_showdimen.InsertColumn(0,"�ɱ�ߴ�",LVCFMT_LEFT,95,-1);
	m_showdimen.InsertColumn(1,"��ֵ",LVCFMT_LEFT,80,-1);
	m_showdimen.InsertColumn(2,"��ƫ��",LVCFMT_LEFT,70,-1);
	m_showdimen.InsertColumn(3,"��ƫ��",LVCFMT_LEFT,70,-1);
	m_showdimen.InsertColumn(4,"��ע",LVCFMT_LEFT,100,-1);
	m_showdimen.SetReadOnlyColumns(0,false);
	m_showdimen.SetReadOnlyColumns(4,false);

	//����չ�Ի���

	Database_struc.RemoveAll();					//��ճߴ�ṹ��Ϣ
	Database_struc_temp.RemoveAll();            //�����ʱ�ߴ�ṹ��Ϣ
	RelationSet.RemoveAll();				    //��չ�ϵ��Ϣ
	m_showdimen.DeleteAllItems();				//��ճߴ��б�
	JuBing_struc.RemoveAll();					//��վ���б�
	status=ProMdlCurrentGet((ProMdl*)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�Բ���,����û�д�ģ��!");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
    //	�õ���ǰģ�͵������
	ProductSolid=commonfun.ObtainTheTopSolid(solid);
	if (ProductSolid==NULL)
	{
		AfxMessageBox("�����������Ʒ����Ѳ���Ҫ�Ĳ�Ʒ�ر�,���ó��ڴ�");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	status=ProMdlTypeGet((ProMdl)ProductSolid,&p_type);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ʼ������");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	if (p_type==PRO_MDL_PART)
	{
		AfxMessageBox("�����������ڲ�Ʒ������������ͷ�ʽ����");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	//�õ���Ʒ��ģ�ͺ�
    int	s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("�޷��õ������Ʒ��ģ����Ϣ��");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	mdlnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum;
	if (mdlnum=="-1")
	{
		AfxMessageBox("����Ƿ�ģ�ͣ��޷����ͣ�");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	if(!compute.ObtainAsmFromBaseTemp(solid,mdlnum))
	{
		AfxMessageBox("�޷��õ���ģ�͵Ŀɱ�ߴ�");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	if(!compute.ObtainValueFromMdlTemp(solid))
	{
		return FALSE;
	}
	int t=0;
	//��ʾ�����ߴ�
	for(int i=0;i<Database_struc_temp.GetSize();i++)	
	{
		int indexNo=m_showdimen.InsertItem(LVIF_TEXT|LVIF_PARAM,t,Database_struc_temp[i].name,
			0,0,0,Database_struc_temp[i].number);
		m_showdimen.SetItemText(indexNo,1,Database_struc_temp[i].value);
		m_showdimen.SetItemText(indexNo,2,Database_struc_temp[i].up);	 //��ƫ��
		m_showdimen.SetItemText(indexNo,3,Database_struc_temp[i].down);	 //��ƫ��
		m_showdimen.SetItemText(indexNo,4,Database_struc_temp[i].note);
		t++;
	}
	return TRUE;
}

void CModelAlterDlg::OnBnClickedCancel()
{
	  CDialog::DestroyWindow();
}

void CModelAlterDlg::OnBnClickedButtonApply()
{
	CCompute compute;
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	ProSolid solid;
	ProError status;
	BOOL IsOk=TRUE;
	CString str;
	status=ProMdlCurrentGet((ProMdl*)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�Բ���,����û�д�ģ��!");
		return;
	}
	Database_struc_temp.RemoveAll();
	if(!compute.ObtainAsmFromBaseTemp(solid,mdlnum))
	{
		AfxMessageBox("�޷��õ���ģ�͵Ŀɱ�ߴ�");
		return;
	}
	if(!compute.ObtainValueFromMdlTemp(solid))
	{
		return;
	}
	//�õ��б�������
	for (int i=0;i<m_showdimen.GetItemCount();i++)
	{
		Database_struc_temp[i].oldvalue=Database_struc_temp[i].value;
		Database_struc_temp[i].value=m_showdimen.GetItemText(i,1);
		Database_struc_temp[i].up=m_showdimen.GetItemText(i,2);
		Database_struc_temp[i].down=m_showdimen.GetItemText(i,3);
	}
	////�õ���Ʒ��ģ�ͺ�
    int	s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("�޷��õ������Ʒ��ģ����Ϣ��");
		return;
	}
	mdlnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum;
	if (mdlnum=="-1")
	{
		AfxMessageBox("����Ƿ�ģ�ͣ��޷����ͣ�");
		return;
	}
	//�õ���ϵʽ�Ͳ�Ʒ����ı���
	Database_struc.RemoveAll();
	RelationSet.RemoveAll();
	//����˶������Ͷ���ϵʽ
	if(!compute.GetAllVariantAndRelativeVariant((ProSolid)solid,mdlnum))
	{
		return ;
	}
	RelationSetTemp.RemoveAll();
	RelationSetTemp.Copy(RelationSet);
	//��ʱ�Ľṹ������ʽ�Ľṹ
	for (int i=0;i<Database_struc_temp.GetSize();i++)
	{
		for (int j=0;j<Database_struc.GetSize();j++)
		{
			if (Database_struc[j].PARADIMSYMBOL==Database_struc_temp[i].PARADIMSYMBOL)
			{
				Database_struc[j].oldvalue=Database_struc_temp[i].oldvalue;       //����ʧ��ʹ��ԭ��
				Database_struc[j].value=Database_struc_temp[i].value;
				Database_struc[j].up=Database_struc_temp[i].up;
				Database_struc[j].down=Database_struc_temp[i].down;
				Database_struc[j].IsOk=TRUE;
			}
		}
	}
	//�õ��������������ֵ
	if (!compute.GetValueFromMdl())
	{
		return;
	}
	//�����б��������ݼ���
	for (int i=0;i<Database_struc.GetSize();i++)
	{
		  if (Database_struc[i].IsOk)
		  {
			   compute.UpdateAllRelation(Database_struc[i].PARADIMSYMBOL,i);
		  }
	}
	//�ж��Ƿ��Ѿ�ȫ����������
	for (int j=0;j<RelationSet.GetSize();j++)
	{
		if(compute.IsOnlyOne(j)!="����")
		{
			IsOk=FALSE;
			AfxMessageBox("�㽨�Ĺ�ϵʽ�����޷�Ϊ����㣡");
			break;
		}
	} 
	if (IsOk)
	{
		if(!compute.AsmGeneration(solid))
		{
			//AfxMessageBox("����ʧ�ܣ�");
			return;
		}
		ProWindowActivate(PRO_VALUE_UNUSED);
		//�ж���Щ�Ѿ�����
		if(!pdtclass.FindProductMenberForProductWhichIsModify(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE))
		{
			return;
		}
		Database_struc_temp.RemoveAll();
		if(!compute.ObtainAsmFromBaseTemp(solid,mdlnum))
		{
			AfxMessageBox("�޷��õ���ģ�͵Ŀɱ�ߴ�");
			return;
		}
	}
	CDialog::DestroyWindow();
}

void CModelAlterDlg::OnNMClickListDimen(NMHDR *pNMHDR, LRESULT *pResult)
{
	ProDimension haha;
	CCompute compute;
	ProAsmcomppath comppath;
	ProSolid solid,solidtemp;
	ProError status;
	CString str,MdlNum;
	int k,postID;
	status=ProMdlCurrentGet((ProMdl*)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�Բ���,����û�д�ģ��!");
		return;
	}
	//Ҫ����
	BOOL IsHave=FALSE;
	for (k=0;k<m_showdimen.GetItemCount();k++)
	{
		if(m_showdimen.GetItemState(k,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsHave=TRUE;
			if (Database_struc_temp[k].type=="dim")
			{
				str=Database_struc_temp[k].idnum;
				break;
			}
			else 
			{
				ProWindowRepaint(PRO_VALUE_UNUSED);
				ProWindowRefresh(PRO_VALUE_UNUSED);
				return;
			}
				
		}
	}
	if (!IsHave)
	{
		return;
	}
	IsHave=FALSE;
	str.Delete(0,1);
    haha.id=atoi(str.GetBuffer());
	int j=Database_struc_temp[k].PARADIMSYMBOL.Find(":");
	str=Database_struc_temp[k].PARADIMSYMBOL;
	str.Delete(0,j+1);
	postID=atoi(str.GetBuffer());
	JuBing_struc.RemoveAll();
	if (!compute.ObtainJuBing(solid))
	{
		return;
	}
	for (int i=0;i<JuBing_struc.GetSize();i++)
	{
		if (postID==JuBing_struc[i].number)
		{
			IsHave=TRUE;
			comppath=JuBing_struc[i].comppath;
			solidtemp=JuBing_struc[i].solid;
			break;
		}
	}
	if (!IsHave)
	{
		return;
	}
	haha.type=PRO_DIMENSION;
	haha.owner=((ProMdl)solidtemp);
	ProWindowRepaint(PRO_VALUE_UNUSED);
	if (solidtemp==JuBing_struc[0].solid)
	{
		ProDimensionShow(&haha,NULL,NULL,NULL);
	}
	else
		ProDimensionShow(&haha,NULL,NULL,&comppath);
	ProWindowRefresh(PRO_VALUE_UNUSED);	           				 
	*pResult = 0;
}

void CModelAlterDlg::OnBnClickedButtonOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	return;
}

void CModelAlterDlg::OnNMRclickListDimen(NMHDR *pNMHDR, LRESULT *pResult)
{
	//TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CModelAlterDlg::OnLvnHotTrackListDimen(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	 TODO: �ڴ���ӿؼ�֪ͨ����������
	//CWnd* pW=GetDlgItem(IDC_LIST_DIMEN);  
	//LVHITTESTINFO lvhti;
	//POINT myPoint=GetCurrentMessage()->pt;
	//m_showdimen.ScreenToClient(&myPoint);
	//lvhti.pt = myPoint;
	//m_showdimen.SubItemHitTest(&lvhti);
	//if (lvhti.iItem==-1) 
	//{
	//	m_tooltip.UpdateTipText("����ʾ",pW,0);
	//	*pResult = 0;
	//	return;
	//}
	////if (lvhti.iItem>1)
	////{
	////	m_tooltip.UpdateTipText("",pW,0);
	////	*pResult = 0;
	////	return;
	////}
	//if (lvhti.iSubItem==-1) {
	//	m_tooltip.UpdateTipText("",pW,0);
	//	*pResult = 0;
	//	return;
	//}
	//////������м�յ�����Ϊ
	////if (lvhti.iSubItem>1 && p_ColProp[lvhti.iSubItem].para_num==-1) {
	////	m_tooltip.UpdateTipText("",pW,0);
	////		*pResult = 0;
	////	return;
	////}
	//m_tooltip.UpdateTipText("FDGFDGF"/*CeShi[lvhti.iItem+1]*/,pW,0);
	*pResult = 0;
}

BOOL CModelAlterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	//m_tooltip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}


void CModelAlterDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: �ڴ������Ϣ����������
}
