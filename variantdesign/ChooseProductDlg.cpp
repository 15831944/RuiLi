// ChooseProductDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "ChooseProductDlg.h"
#include "MyDatabase.h"
#include "afxtempl.h"
#include "ProWindows.h"
#include "CheckDlg.h"
#include "SelectDlg.h"
#include "afxtempl.h"
#include "ProDrawing.h"
#include "MyDatabase.h"
#include "CommonFuncClass.h"
#include "Compute.h"
#include "FtpFile.h"
#include "DownLoadDlg.h"
#include "DetailInfoClass.h"


extern CArray<ProDrawing,ProDrawing>  DrawingList;
void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern FtpInfo Ftp_struc;
extern CPtrArray p_classarray;




//extern CMyDatabase m_pdm;
CArray<ItemHandle,ItemHandle> treehandle;
IMPLEMENT_DYNAMIC(CChooseProductDlg, CDialog)
CChooseProductDlg::CChooseProductDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseProductDlg::IDD, pParent)
	, m_show(0)
{
}

CChooseProductDlg::~CChooseProductDlg()
{
}

void CChooseProductDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_listproduct);
	DDX_Radio(pDX, IDC_RADIO_PART, m_show);
}


BEGIN_MESSAGE_MAP(CChooseProductDlg, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_RADIO_PART, OnBnClickedRadioPart)
	ON_BN_CLICKED(IDC_RADIO_ASM, OnBnClickedRadioAsm)
END_MESSAGE_MAP()


// CChooseProductDlg ��Ϣ�������

void CChooseProductDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRepaint(PRO_VALUE_UNUSED);

	// TODO: �ڴ������Ϣ����������
}

void CChooseProductDlg::OnBnClickedButtonCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CChooseProductDlg::OnBnClickedButtonOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL IsSelect=FALSE;

	CCommonFuncClass commonfun;
	CString InsCode,sql;
	for (int i=0;i<m_listproduct.GetItemCount();i++)
	{
		if (m_listproduct.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsSelect=TRUE;
			InsCode=m_listproduct.GetItemText(i,1);
			if (m_show==0)					  //˵�������
			{
				sql.Format("SELECT * FROM NPartInsInfoTable WHERE InsCode='%s'",InsCode);
				//if (!commonfun.GetInfoFromDatabaseBase(sql,0,FALSE,TRUE,""))
				//{
				//	AfxMessageBox("�޷��õ�������Ϣ");
				//	return;
				//}
			}
			else
			{
				;//�Բ�Ʒ����
			}
		}
	}
	if (!IsSelect)
	{
		AfxMessageBox("��ѡ����Ҫ�򿪵�ģ�ͣ�");
		return;
	}
	int s=commonfun.IsBasicInfoHave(InsCode);
	if (s==-1)
	{
		AfxMessageBox("�޷��õ�������Ϣ");
		return;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="����")
	{
		//ֱ�Ӵ򿪣�
		//if(!commonfun.OpenPartFile(((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFileName,
		//	((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwName,
		//	((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFilePath,
		//	((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwFilePath,s))
		//{
		//	AfxMessageBox("��ʧ��");
		//}
		//else
		//{
		// //   checkdlg.Create(IDD_DIALOG3,NULL);
		//	//checkdlg.ShowWindow(SW_SHOWNORMAL);
		//}
	}
	else
		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="������")
		{
			OnOK();
			CDownLoadDlg downloaddlg;
			downloaddlg.s=s;
			downloaddlg.IsCheck=TRUE;
			downloaddlg.DoModal();//�����Ի���������ѡ��
		}
		OnOK();
}

BOOL CChooseProductDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	CCommonFuncClass commonfun;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_listproduct.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listproduct.InsertColumn(0,"���",LVCFMT_LEFT,20,-1);
	m_listproduct.InsertColumn(1,"ͼ��",LVCFMT_LEFT,80,-1);
	m_listproduct.InsertColumn(2,"����",LVCFMT_LEFT,100,-1);
	m_listproduct.InsertColumn(3,"���",LVCFMT_LEFT,55,-1);
	m_listproduct.InsertColumn(4,"����",LVCFMT_LEFT,55,-1);
	m_listproduct.InsertColumn(5,"����λ��",LVCFMT_LEFT,55,-1);
    m_listproduct.InsertColumn(6,"�����",LVCFMT_LEFT,55,-1);
	UpdateData(FALSE);
	CString sql;
	sql.Format("SELECT * FROM NPartInsInfoTable Where FinishState='���' AND AuditState='���δͨ��'");
	SHOW(&m_listproduct,sql);
	return TRUE;  // return TRUE unless you set the focus to a control
}

/************************************************************************/
/* ��ʾ����˵����                                                     */
/************************************************************************/
void CChooseProductDlg::OnBnClickedRadioPart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listproduct.DeleteAllItems();
	CString sql;
	sql.Format("SELECT * FROM NPartInsInfoTable Where FinishState='���' AND AuditState='���δͨ��'");
}
/************************************************************************/
/* ��ʾ����˵Ĳ�Ʒ                                                     */
/************************************************************************/
void CChooseProductDlg::OnBnClickedRadioAsm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listproduct.DeleteAllItems();
	CString sql;
	sql.Format("SELECT * FROM NProductInsInfoTable Where FinishState='���' AND AuditState='���δͨ��'");
	SHOW(&m_listproduct,sql);
}
/************************************************************************/
/* ��ʾȫ��                                                             */
/************************************************************************/
void CChooseProductDlg::OnBnClickedRadioAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}
/************************************************************************/
/* ��ʾ����                                                             */
/************************************************************************/
BOOL CChooseProductDlg::SHOW(CListCtrl *m_list,CString sql)
{
	CString str;
	CStringArray GroupNum;
	GroupNum.RemoveAll();
	int i=0;
	 int Z=m_db.Query(sql);
	 if (Z==-1)
	 {
		 return FALSE;
	 }
	 if (Z==0)
	 {
		 return TRUE;
	 }
	 else
	 {
		 m_db.MoveBegin();
		 do {
			 str.Format("%d",i+1);
		 	m_list->InsertItem(i,str);
			str=m_db.getString("InsCode");
			m_list->SetItemText(i,1,str);
			str=m_db.getString("Name");
			m_list->SetItemText(i,2,str);
			str.Format("%d",m_db.getInt("TypeNum"));
			GroupNum.Add(str);
			str="���";
			m_list->SetItemText(i,4,str);
			str=m_db.getString("LocalState");
			m_list->SetItemText(i,5,str);
			str=m_db.getString("Designer");
			m_list->SetItemText(i,6,str);
		 } while(m_db.MoveNext());
		 for (int j=0;j<GroupNum.GetSize();j++)
		 {
			 sql.Format("SELECT * FROM NPartType where Number=%s",GroupNum[j]);
			 Z=m_db.Query(sql);
			 if (Z==-1 || Z==0)
			 {
				 continue;
			 }
			 else
			 {
				 str=m_db.getString("Name");
				 m_list->SetItemText(j,3,str);
			 }
		 }
	 }
	 return TRUE;
}