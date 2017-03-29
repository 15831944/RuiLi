// CheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "CheckDlg.h"
#include "ProWindows.h"
#include "Compute.h"
#include "MyDatabase.h"
#include "afxtempl.h"
#include "ProSolid.h"
#include "ProDimension.h"
#include "ProUtil.h"
#include "CommonFuncClass.h"
#include "DetailDlg.h"
#include <stdlib.h>
#include <stdio.h>
#include "FtpFile.h"
#include "ProFeature.h"
#include "ProDrawing.h"
#include "MyDatabase.h"
#include "DetailInfoClass.h"
#include "PrtInfoDlg.h"

extern CStringArray RelationSet;
extern CArray<JuBing,JuBing> JuBing_struc;
extern CArray <Database,Database> Database_struc_temp;
extern   CArray <Database,Database> Database_struc;
extern CMyDatabase m_db;
extern FtpInfo Ftp_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;

/// 
void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data);
// CCheckDlg �Ի���

IMPLEMENT_DYNAMIC(CCheckDlg, CDialog)
CCheckDlg::CCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckDlg::IDD, pParent)
	,s(-1)
{
}

CCheckDlg::~CCheckDlg()
{
}

void CCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DIM, m_dimension);
}


BEGIN_MESSAGE_MAP(CCheckDlg, CDialog)
	//ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_MOVE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_DIM, OnNMClickListDim)
	ON_BN_CLICKED(IDC_BUTTON_OTHER, OnBnClickedButtonOther)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
ON_BN_CLICKED(IDC_BUTTON_NO, OnBnClickedButtonNo)
END_MESSAGE_MAP()


// CCheckDlg ��Ϣ�������

//void CCheckDlg::OnBnClickedOk()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	INT_PTR status;
//	CPartOpenDlg partopendlg;
//    status=	partopendlg.DoModal();
//	
//}

void CCheckDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::DestroyWindow();
	
}

BOOL CCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	CCommonFuncClass commonfun;
	CString str;					      
	BOOL IsHave=FALSE;
	CCompute compute;
	//ȥ��������û�еĻ�����Ϣ
	commonfun.IsBasicInfoAlreadyHave();
	commonfun.RemoveNotInSession();

	if (!m_db.DatabaseState())
	{
		m_db.ReOpen();
	}
	m_dimension.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_dimension.InsertColumn(0,"�ɱ�ߴ�",LVCFMT_LEFT,80,-1);
	m_dimension.InsertColumn(1,"��ֵ",LVCFMT_LEFT,80,-1);
	m_dimension.InsertColumn(2,"��ƫ��",LVCFMT_LEFT,55,-1);
	m_dimension.InsertColumn(3,"��ƫ��",LVCFMT_LEFT,55,-1);
    
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_NO)->EnableWindow(FALSE);
	ProSolid solid;
	ProError status;
	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowMdlGet(w_id,(ProMdl *)&solid);
	s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("�޷��õ���������ģ����Ϣ��");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	partnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum;
	if (partnum=="-1")
	{
		AfxMessageBox("����Ƿ�ģ�ͣ�û�пɱ�ߴ磡");
	}
	else
	{
		Database_struc_temp.RemoveAll();
		if(!compute.GetVariantForPart(solid,partnum,TRUE))
		{
            this->SendMessage(WM_CLOSE,0,0);
			return FALSE;//��������������ʱ�ṹ��
		}
		if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))	 //Ҳ��Ҫ����ƫ����Ϣ
		{
			return FALSE;
		}
		int t=0;
		//��ʾ�����ߴ�
		for (int i=0;i<Database_struc_temp.GetSize();i++)	 //������
		{
			str=Database_struc_temp[i].name;
			int indexNo=m_dimension.InsertItem(LVIF_TEXT|LVIF_PARAM,t,Database_struc_temp[i].name,
				0,0,0,Database_struc_temp[i].number);
			m_dimension.SetItemText(indexNo,1,Database_struc_temp[i].value);
			m_dimension.SetItemText(indexNo,2,Database_struc_temp[i].up);	 //��ƫ��
			m_dimension.SetItemText(indexNo,3,Database_struc_temp[i].down);	 //��ƫ��
			UpdateData(FALSE);
			t++;
		}
	}
	return TRUE;  
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CCheckDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
    ProWindowRepaint(PRO_VALUE_UNUSED);
	// TODO: �ڴ������Ϣ����������
}	

void CCheckDlg::OnNMClickListDim(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ProSolid solid;
	ProDimension haha;
	CString str;
	BOOL IsHave=FALSE;
	CString type;
	ProError status;
	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowMdlGet(w_id,(ProMdl *)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		return;
	}
	POSITION pos = m_dimension.GetFirstSelectedItemPosition();
	while (pos)
	{
		IsHave=TRUE;
		int nItem = m_dimension.GetNextSelectedItem(pos);
		if (Database_struc_temp[nItem].type!="dim")		   //����ǲ���������ʾ�ߴ�
		{
			ProWindowRepaint(PRO_VALUE_UNUSED);
			ProWindowRefresh(PRO_VALUE_UNUSED);
			return;
		}
		else
		{
			str=Database_struc_temp[nItem].idnum;
			str.Delete(0,1);
			haha.id=atoi(str.GetBuffer());
			haha.type=PRO_DIMENSION;
			haha.owner=((ProMdl)solid);
			ProWindowRepaint(PRO_VALUE_UNUSED);
			ProDimensionShow(&haha,NULL,NULL,NULL);
		}	
	}

	if (!IsHave)
	{
		return;
	}
	*pResult = 0;
}


void CCheckDlg::OnBnClickedButtonOther()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPrtInfoDlg infodlg;
	infodlg.s=s;
	if(IDOK==infodlg.DoModal())	
	{
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_NO)->EnableWindow(TRUE);
	}
}
void CCheckDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�ж�ͼ���Ƿ���ȷ
	CString str;
	CCommonFuncClass commonfun;
	//ProMdlCurrentGet((ProMdl*)&solid);
	//int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("�Ҳ�������������Ϣ���޷�����");
		return;
	}
	//��ʼ����
	//�õ�ʱ��
	CTime time=CTime::GetCurrentTime();
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime=time.Format("%Y-%m-%d %H:%M:%S");
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="���ͨ��";

	m_db.m_pConnection->Close();
	m_db.ReOpen();
	m_db.m_pConnection->BeginTrans();
	try
	{
		if (!CheckPart(s))
		{
			m_db.m_pConnection->RollbackTrans();
			AfxMessageBox("���ʧ��");
		}//Update���ݿ�
		else
		{
			m_db.m_pConnection->CommitTrans();
			AfxMessageBox("�ύ�ɹ���");
		}

	}
	catch (_com_error e) 
	{
		m_db.m_pConnection->RollbackTrans();
		AfxMessageBox("���ʧ�ܣ�");
		//AfxMessageBox(e.ErrorMessage());
	}
	CDialog::DestroyWindow();
}

/************************************************************************/
/* ���δͨ��                                                           */
/************************************************************************/
void CCheckDlg::OnBnClickedButtonNo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (s==-1)
	{
		AfxMessageBox("�Ҳ�������������Ϣ���޷�����");
		return;
	}
	//��ʼ����
	//�õ�ʱ��
	CTime time=CTime::GetCurrentTime();
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime=time.Format("%Y-%m-%d %H:%M:%S");
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="���δͨ��";
	((DetailInfoClass *)p_classarray.GetAt(s))->DesignState="δ���";
	((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime="";
	m_db.m_pConnection->Close();
	m_db.ReOpen();
	m_db.m_pConnection->BeginTrans();
	try
	{
		if (!CheckPart(s))
		{
			m_db.m_pConnection->RollbackTrans();
			AfxMessageBox("���ʧ��");
		}//Update���ݿ�
		else
		{
			m_db.m_pConnection->CommitTrans();
			AfxMessageBox("�ύʧ�ܣ�");
		}

	}
	catch (_com_error e) 
	{
		m_db.m_pConnection->RollbackTrans();
		AfxMessageBox("���ʧ�ܣ�");
		AfxMessageBox(e.ErrorMessage());
	}
	CDialog::DestroyWindow();
}
/************************************************************************/
/* �������ݿ������                                                     */
/************************************************************************/
BOOL CCheckDlg::CheckPart(int s)
{
	CString sql;
	CCompute compute;
	CCommonFuncClass commonfun;
	/////
	sql.Format("UPDATE NPartInsInfoTable Set Name='%s',InsCode='%s',Material='%s',Designer='%s',FinishTime='%s',Auditor='%s',AuditTime='%s',FinishState='%s',AuditState='%s',Note='%s' where PrtNum=%d",
		((DetailInfoClass *)p_classarray.GetAt(s))->InsName,((DetailInfoClass *)p_classarray.GetAt(s))->InsCode,
		((DetailInfoClass *)p_classarray.GetAt(s))->Material,((DetailInfoClass *)p_classarray.GetAt(s))->Designer,
		((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime,((DetailInfoClass *)p_classarray.GetAt(s))->Auditor,
		((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime,((DetailInfoClass *)p_classarray.GetAt(s))->DesignState,
		((DetailInfoClass *)p_classarray.GetAt(s))->AuditState,((DetailInfoClass *)p_classarray.GetAt(s))->Note,
		((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	if(m_db.Execute(sql)==0)
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum=="-1")
	{
		return TRUE;
	}
	else
	{
		Database_struc_temp.RemoveAll();
		if(!compute.GetVariantForPart(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum,TRUE))
		{
			return FALSE;	 //��������������ʱ�ṹ��
		}                   
		if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
		{
			return FALSE;
		}
		if (!commonfun.UpdateVariantAndPerf(s))
		{
			return FALSE;
		}
	}
	return TRUE;
}