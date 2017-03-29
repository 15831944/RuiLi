// PdtModifyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PdtModifyDlg.h"
#include "MyDatabase.h"
#include "CommonFuncClass.h"
#include "DownLoadDlg.h"
#include "DetailInfoClass.h"
#include "ProSolid.h"
#include "ProductClass.h"
#include <ProWindows.h>


// CPrtModifyDlg �Ի���
extern CMyDatabase m_db;
extern CPtrArray p_classarray;
extern CArray <FilePath,FilePath> FilePath_struc;


// CPdtModifyDlg �Ի���

IMPLEMENT_DYNAMIC(CPdtModifyDlg, CDialog)
CPdtModifyDlg::CPdtModifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPdtModifyDlg::IDD, pParent)
	, m_key(_T(""))
	, m_workspace(0)
	, m_audit(FALSE)
{
}

CPdtModifyDlg::~CPdtModifyDlg()
{
}

void CPdtModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KEY, m_key);
	DDX_Control(pDX, IDC_COMBO_KEY, m_combokey);
	DDX_Control(pDX, IDC_TREE_PART, m_treeprt);
	DDX_Control(pDX, IDC_LIST_PART, m_listprt);
	DDX_Radio(pDX, IDC_RADIO_WORKAPACE, m_workspace);
	DDX_Check(pDX, IDC_CHECK_AUDIT, m_audit);
}


BEGIN_MESSAGE_MAP(CPdtModifyDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL2, OnBnClickedCancel2)
	ON_BN_CLICKED(IDC_RADIO_WORKAPACE, OnBnClickedRadioWorkapace)
	ON_BN_CLICKED(IDC_RADIO_PDM, OnBnClickedRadioPdm)
	ON_NOTIFY(NM_CLICK, IDC_TREE_PART, OnNMClickTreePart)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CPdtModifyDlg ��Ϣ�������
BOOL CPdtModifyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	CCommonFuncClass commonfun;
	//ȥ��������û�еĻ�����Ϣ
	commonfun.IsBasicInfoAlreadyHave();
	commonfun.RemoveNotInSession();
	int index=m_combokey.AddString("ͼ��");
	m_combokey.SetCurSel(index);
	m_listprt.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listprt.InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
	m_listprt.InsertColumn(1,"ͼ��",LVCFMT_LEFT,100,-1);
	m_listprt.InsertColumn(2,"����",LVCFMT_LEFT,55,-1);
	m_listprt.InsertColumn(3,"���",LVCFMT_LEFT,55,-1);
	m_listprt.InsertColumn(4,"���״̬",LVCFMT_LEFT,55,-1);
	m_listprt.InsertColumn(5,"�����",LVCFMT_LEFT,55,-1);
	//�õ���Ʒ��ͼ��
	commonfun.ObtainProductModelClass(&m_treeprt);
	return TRUE;  
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ��ѯ��ť                                                             */
/************************************************************************/
void CPdtModifyDlg::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listprt.DeleteAllItems();
	UpdateData(TRUE);
	CString sql;
	if (m_workspace==0 && m_audit)			 //���ҹ���������δ��˵�
	{
		sql.Format("SELECT * FROM NProductInsInfoTable WHERE InsCode LIKE '%%%s%%' AND FinishState='���' AND AuditState='���δͨ��' AND PDMState='δ�ύ'",m_key);
		if (!GetPdtFromBase(sql,FALSE))        //�ڹ���������
		{
			AfxMessageBox("����ʧ��");
			return;
		}
	}
	else
	{
		if (m_workspace==0 && (!m_audit))
		{
			sql.Format("SELECT * FROM NProductInsInfoTable WHERE InsCode LIKE '%%%s%%' AND PDMState='δ�ύ'",m_key);
			if (!GetPdtFromBase(sql,FALSE))     //�ڹ���������
			{
				AfxMessageBox("����ʧ��");
				return;
			}
		}
		else    //����PDM���ݿ�
		{
			sql.Format("SELECT * FROM NProductInsInfoTable WHERE PDMNum LIKE '%%%s%%' AND PDMState='�ύ'",m_key);
			if (!GetPdtFromBase(sql,TRUE))
			{
				AfxMessageBox("����ʧ��");
				return;
			}
		}
	}
}

void CPdtModifyDlg::OnBnClickedCancel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

/************************************************************************/
/* ����ͼ�Ų���ʵ��,TRUE��PDM�飬FALSE��VDS��                           */
/************************************************************************/
BOOL CPdtModifyDlg::GetPdtFromBase(CString sql,BOOL PDM)
{
	CString str;
	CStringArray TypeNum;
	TypeNum.RemoveAll();
	int Z=m_db.Query(sql);

	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		m_listprt.DeleteAllItems();
		return TRUE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			//if (PDM)
			//{
   //            str=m_db.getString("PDMNum");
			//   if (str!="")
			//   {
			//	   str.Format("%d",i+1);
			//	   m_listprt.InsertItem(i,str);
			//	   str=m_db.getString("PDMNum");
			//	   m_listprt.SetItemText(i,1,str);
			//	   str=m_db.getString("Name");
			//	   m_listprt.SetItemText(i,2,str);
			//	   str.Format("%d",m_db.getInt("TypeNum"));
			//	   TypeNum.Add(str);
			//	   str=m_db.getString("FinishState");
			//	   m_listprt.SetItemText(i,4,str);
			//	   str=m_db.getString("Designer");
			//	   m_listprt.SetItemText(i,5,str);
			//	   str.Format("%d",m_db.getInt("PdtNum"));
			//	   m_listprt.SetItemData(i,atol(str));
			//   }
			//}
			//else
			//{
				str.Format("%d",i+1);
				m_listprt.InsertItem(i,str);
				if (PDM)
				{
					str=m_db.getString("PDMNum");
				}
				else
				    str=m_db.getString("InsCode");
				m_listprt.SetItemText(i,1,str);
				str=m_db.getString("Name");
				m_listprt.SetItemText(i,2,str);
				str.Format("%d",m_db.getInt("TypeNum"));
				TypeNum.Add(str);
				str=m_db.getString("FinishState");
				m_listprt.SetItemText(i,4,str);
				str=m_db.getString("Designer");
				m_listprt.SetItemText(i,5,str);
				str.Format("%d",m_db.getInt("PdtNum"));
				m_listprt.SetItemData(i,atol(str));
			//}
			i++;
		} while(m_db.MoveNext());
	}
	for (int i=0;i<TypeNum.GetSize();i++)
	{
		sql.Format("SELECT * FROM NAsmType Where Number=%s",TypeNum[i]);
		Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
		{
			return FALSE;
		}
		else
		{
			m_db.MoveBegin();
			do {
				str=m_db.getString("Name");
				m_listprt.SetItemText(i,3,str);
			} while(m_db.MoveNext());
		}
	}
	return TRUE;
}

void CPdtModifyDlg::OnBnClickedRadioWorkapace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listprt.DeleteAllItems();
	m_workspace=0;
	GetDlgItem(IDC_CHECK_AUDIT)->EnableWindow(TRUE);
}

void CPdtModifyDlg::OnBnClickedRadioPdm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_audit=FALSE;
	m_workspace=1;
	m_listprt.DeleteAllItems();
	GetDlgItem(IDC_CHECK_AUDIT)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CPdtModifyDlg::OnNMClickTreePart(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	HTREEITEM hItem;
	CString str,sql;
	UINT uFlags=0;
	CPoint point;
	int num;
	m_listprt.DeleteAllItems();
	point= GetCurrentMessage()->pt;
	m_treeprt.ScreenToClient(&point);
	hItem =m_treeprt.HitTest(point, &uFlags);
	int i=m_audit;
	if (hItem!=NULL && (uFlags&TVHT_ONITEM))
	{
		num=(int)m_treeprt.GetItemData(hItem);
		if ((!m_audit) && m_workspace==0)
		{
			sql.Format("SELECT * FROM NProductInsInfoTable WHERE TypeNum=%d",num);
			if (!GetPdtFromBase(sql,FALSE))
			{
				AfxMessageBox("����ʧ��");
				return;
			}
		}
		if (m_audit && m_workspace==0)
		{
			sql.Format("SELECT * FROM NProductInsInfoTable WHERE TypeNum=%d AND FinishState='���' AND AuditState='���δͨ��'",num);
			if (!GetPdtFromBase(sql,FALSE))
			{
				AfxMessageBox("����ʧ��");
				return;
			}
		}
		else
			if(m_workspace==1)
			{
				sql.Format("SELECT * FROM NProductInsInfoTable WHERE TypeNum=%d and PDMState='�ύ'",num);
				if (!GetPdtFromBase(sql,TRUE))
				{
					AfxMessageBox("����ʧ��");
					return;
				}
			}
		
	}
	*pResult = 0;
}
/************************************************************************/
/* �򿪰�ť                                                             */
/************************************************************************/
void CPdtModifyDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CProductClass pdtclass;
	BOOL IsSelect=FALSE;
	BOOL IsLocal;
	CCommonFuncClass commonfun;
	CString InsCode,sql,SolidPath,DrwPath;
	for (int i=0;i<m_listprt.GetItemCount();i++)
	{
		if (m_listprt.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsSelect=TRUE;
			if(m_listprt.GetItemText(i,4)=="����")
				IsLocal=TRUE;
			else
				IsLocal=FALSE;
			InsCode.Format("%d",m_listprt.GetItemData(i));           //�õ���Ʒ���
			break;
		}
	}
	if (!IsSelect)
	{
		return;
	}
	sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%s",InsCode);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0 )
	{
		AfxMessageBox("���ͼ�ŵĲ�Ʒ�����ڣ�");
		return;
	}
	else
	{
		m_db.MoveEnd();
	    InsCode.Format("%d",m_db.getInt("PdtNum"));
	}
	if (!pdtclass.OpenProductFile(InsCode))            //������ǲ�Ʒ���
	{
		return;
	}
	OnOK();
}
void CPdtModifyDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;
	status=ProWindowRepaint(PRO_VALUE_UNUSED);

	// TODO: �ڴ������Ϣ����������
}
