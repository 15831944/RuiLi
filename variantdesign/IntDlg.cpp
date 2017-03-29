// IntDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "IntDlg.h"
#include "MyDatabase.h"
#include "ProWindows.h"
#include "ProUtil.h"


extern CMyDatabase m_db;
extern FtpInfo Ftp_struc;
// CIntDlg �Ի���

IMPLEMENT_DYNAMIC(CIntDlg, CDialog)
CIntDlg::CIntDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIntDlg::IDD, pParent)
	, m_code(_T(""))
	, Dcode("")
	, m_vdscode(_T(""))
	, m_part(0)
{
}

CIntDlg::~CIntDlg()
{
}

void CIntDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CODE, m_code);
	DDX_Control(pDX, IDC_LIST_PDM, m_pdmlist);
	DDX_Control(pDX, IDC_LIST_VDS, m_vdslist);
	DDX_Text(pDX, IDC_EDIT_VDSCODE, m_vdscode);
	DDX_Radio(pDX, IDC_RADIO_PART, m_part);
}


BEGIN_MESSAGE_MAP(CIntDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LOOK, OnBnClickedButtonLook)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_PART, OnBnClickedRadioPart)
	ON_BN_CLICKED(IDC_RADIO_ASM, OnBnClickedRadioAsm)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PDM, OnNMClickListPdm)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VDS, OnNMClickListVds)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_VDS, OnNMDblclkListVds)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_OUT, OnBnClickedButtonOut)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CIntDlg ��Ϣ�������
//���Ұ�ť
void CIntDlg::OnBnClickedButtonLook()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!Search(TRUE))                     //���Һ���
	{
		AfxMessageBox("��ѯʧ��");
	}
}

void CIntDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//������ϵͳ�����ݼ�������
	if (!Integrate())
	{
		AfxMessageBox("���ɳ���");
		return;
	}
	else
	    AfxMessageBox("���óɹ�");
	OnOK();
}

BOOL CIntDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//�ڳ�ʼ����ѡ�е�ѡ��ť�������
	GetDlgItem(IDC_RADIO_PART)->SetFocus();
	UpdateData(TRUE);
	UpdateData(FALSE);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ListView_SetExtendedListViewStyle(m_pdmlist.m_hWnd,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_pdmlist.InsertColumn(0,"PDM���",LVCFMT_LEFT,60,-1);
	m_pdmlist.InsertColumn(1,"PDMͼ��",LVCFMT_LEFT,100,-1);
	ListView_SetExtendedListViewStyle(m_vdslist.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
	m_vdslist.InsertColumn(0,"VDS���",LVCFMT_LEFT,60,-1);
	m_vdslist.InsertColumn(1,"VDSͼ��",LVCFMT_LEFT,100,-1);
	m_vdslist.InsertColumn(2,"��Ӧ��PDMͼ��",LVCFMT_LEFT,80,-1);
	m_vdslist.InsertColumn(3,"ԭ����PDMͼ��",LVCFMT_LEFT,80,-1);
	m_vdslist.InsertColumn(4,"״̬",LVCFMT_LEFT,50,-1);
	return TRUE;  
	// �쳣��OCX ����ҳӦ���� FALSE
}
//ѡ�����
void CIntDlg::OnBnClickedRadioPart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_part=0;
	UpdateData(FALSE);
}
//ѡ��װ���
void CIntDlg::OnBnClickedRadioAsm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//m_asm=0;
	m_part=1;
	UpdateData(FALSE);
}
/************************************************************************/
/* �õ�PDM��ͼ��                                                        */
/************************************************************************/
void CIntDlg::OnNMClickListPdm(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i=0;i<m_pdmlist.GetItemCount();i++)
	{
		if(m_pdmlist.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			Dcode=m_pdmlist.GetItemText(i,1);
			break;
		}
	}
	*pResult = 0;
}
/************************************************************************/
/* ��PDM��ͼ�ź�VDSͼ�Ŷ�Ӧ                                             */
/************************************************************************/
void CIntDlg::OnNMClickListVds(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i=0;i<m_vdslist.GetItemCount();i++)
	{
		if(m_vdslist.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			m_vdslist.SetItemText(i,2,Dcode);
			break;
		}
	}
	*pResult = 0;
}
/************************************************************************/
/* ��ԭVDS��ͼ��                                                        */
/************************************************************************/
void CIntDlg::OnNMDblclkListVds(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString OldCode;
	for (int i=0;i<m_vdslist.GetItemCount();i++)
	{
		if(m_vdslist.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			OldCode=m_vdslist.GetItemText(i,3);
			m_vdslist.SetItemText(i,2,OldCode);
			break;
		}
	}
	*pResult = 0;
}
//������ϵͳ���ɣ�Ȼ���ٸ���
void CIntDlg::OnBnClickedButtonApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!Integrate())
	{
		AfxMessageBox("���ɳ���");
		return;
	}
	else
	{
		if (Search(FALSE))
		{
			AfxMessageBox("���óɹ�");
		}
	}
}
/************************************************************************/
/* ��PDM�е����ݺ�VDS�е����ݹ���                                       */
/************************************************************************/
//������ݵĶԽӣ�PDM����־�����ã����й������ļ���ɾ��
BOOL CIntDlg::Integrate()
{
	CString PDMCode;
	CString VDSCode;
	CString PDMState="�ύ";
	CString IsNew="F";
	CString sql,str;
	int k;
	BOOL IsOk=TRUE,IsSelect=FALSE;
	m_db.m_pConnection->Close();
	m_db.ReOpen();
	try
	{
		m_db.m_pConnection->BeginTrans();            //��ʼ���ﴦ��
		for (int i=0;i<m_vdslist.GetItemCount();i++)
		{
			if (m_vdslist.GetCheck(i)==TRUE)
			{
				IsSelect=TRUE;
				VDSCode=m_vdslist.GetItemText(i,1);
				PDMCode=m_vdslist.GetItemText(i,2);
				k=(int)m_vdslist.GetItemData(i);
				if (m_part==0)
				{
					sql.Format("UPDATE NPartInsInfoTable Set PDMNum='%s',PDMState='%s' where PrtNum=%d",PDMCode,PDMState,k);
				}
				else
					sql.Format("UPDATE NProductInsInfoTable Set PDMNum='%s',PDMState='%s' where PdtNum=%d",PDMCode,PDMState,k);
				if(!m_db.Execute(sql))
				{
					IsOk=FALSE;
					break;
				}
				//�������Ʒ��ProductBOMTable���"IsNew"��ȫ����Ϊ��F��
				if (m_part!=0)
				{
					sql.Format("UPDATE NProductBOMTable Set IsNew='%s' where PdtNum=%d",IsNew,k);
					if(!m_db.Execute(sql))
					{
						IsOk=FALSE;
						break;
					}
				}
				//��鼯�ɵ��Ƿ���ȷ��ͨ��PDM�е��ļ����Ƿ��VDS�е��ļ�һ�¡�
				if(m_part==0)
				{
					if(!IsDocumentNameTheSave(PDMCode,k,TRUE))
					{
						str.Format("ͼ��Ϊ%s���ļ�����",PDMCode);
						IsOk=FALSE;
						AfxMessageBox(str);
						break;
					}
				}
				else
                    if(!IsDocumentNameTheSave(PDMCode,k,FALSE))
					{
						str.Format("ͼ��Ϊ%s���ļ�����",PDMCode);
						IsOk=FALSE;
						AfxMessageBox(str);
						break;
					}
			}
		}
		if(!IsSelect)
		{
			AfxMessageBox("������Ҫ��Ӧ����ǰ��");
			m_db.m_pConnection->RollbackTrans();
			return FALSE;
		}
		if (IsOk)
		{
			m_db.m_pConnection->CommitTrans();
			return TRUE;
		}
		else
		{
			m_db.m_pConnection->RollbackTrans();
			return FALSE;
		}
			
	}
	catch (...) 
	{
		m_db.m_pConnection->RollbackTrans();
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* ��鵱ǰ��PDM��VDS���ɵ��Ƿ���ȷ                                     */
/************************************************************************/
BOOL CIntDlg::IsDocumentNameTheSave(CString PDMCode,int Num,BOOL IsPrt) //1.��PDM��ͼ�� 2��VDS�е�����ţ������ǲ�Ʒ�ţ�3���Ƿ���������ǣ�װ����
{
	CMyDatabase m_pdm;
    CString  sql,PDMName,VDSName;
	//�õ�VDS���ļ���
	if (IsPrt)
	{
		sql.Format("Select * From NPartInsInfoTable where PrtNum=%d",Num);
	}
	else
		sql.Format("Select * From NProductInsInfoTable where PdtNum=%d",Num);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	if (Z!=0)
	{
		m_db.MoveBegin();
		do {
				VDSName=m_db.getString("SolidFileName");
				VDSName.MakeLower();
				break;
		} while(m_db.MoveNext());
	}
	if(!m_pdm.Open(Ftp_struc.Database))
	{
		return FALSE;
	}
	sql.Format("select * from Documents where Code='%s'",PDMCode);
    Z=m_pdm.Query(sql);
	if (Z==-1 || Z==0)
	{
		m_pdm.Close();
		return FALSE;
	}
	else
	if (Z!=0)
	{
		m_pdm.MoveBegin();
		do {
				PDMName=m_pdm.getString("FileName");
				PDMName.MakeLower();
				if((PDMName.Find(".prt")!=-1))
				{
				    break;
				}
				else
					if((PDMName.Find(".asm")!=-1))
					{
						break;
					}
		} while(m_pdm.MoveNext());
	}
	m_pdm.Close();
	int k=PDMName.ReverseFind('.');
	if (k!=-1)
	{
		PDMName.Delete(k,(PDMName.GetLength()-k));
	}
	if(PDMName==VDSName)
	{
		return TRUE;
	}
	else
		return FALSE;
    return TRUE;
}
/************************************************************************/
/* ȡ��PDM�е����ݺ�VDS�е����ݹ���                                     */
/************************************************************************/
void CIntDlg::OnBnClickedButtonOut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString PDMCode;
	CString VDSCode;
	CString PDMState="δ�ύ";
	CString sql;
	int k;
	BOOL IsOk=TRUE;
	m_db.m_pConnection->Close();
	m_db.ReOpen();
	try
	{
		m_db.m_pConnection->BeginTrans();
		for (int i=0;i<m_vdslist.GetItemCount();i++)
		{
			if (m_vdslist.GetCheck(i)==TRUE)
			{
				k=(int)m_vdslist.GetItemData(i);
				if (m_part==0)
				{
					sql.Format("UPDATE NPartInsInfoTable Set PDMState='%s' where PrtNum=%d",PDMState,k);
				}
				else
					sql.Format("UPDATE NProductInsInfoTable Set PDMState='%s' where PdtNum=%d",PDMState,k);
				if(!m_db.Execute(sql))
				{
					IsOk=FALSE;
					break;
				}
			}
		}
		if (IsOk)
		{
			m_db.m_pConnection->CommitTrans();
		}
		else
		{
			m_db.m_pConnection->RollbackTrans();
			return;
		}
	}
	catch (...) 
	{
		m_db.m_pConnection->RollbackTrans();
		return;
	}
	if (Search(FALSE))
	{
		AfxMessageBox("���óɹ�");
	}
}
/************************************************************************/
/* ��ѯ����                                                                     */
/************************************************************************/
BOOL CIntDlg::Search(BOOL First)
{
	UpdateData(TRUE);
	CString sql;
	CString str;
	m_vdslist.DeleteAllItems();
	if (m_part==0)
	{
		sql.Format("SELECT * FROM NPartInsInfoTable WHERE InsCode LIKE '%%%s%%'",m_vdscode);
	}
	else
	{
		sql.Format("SELECT * FROM NProductInsInfoTable WHERE InsCode LIKE '%%%s%%'",m_vdscode);
	}
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	else
		if (Z!=0)
		{
			int i=0;
			m_db.MoveBegin();
			do {
				str.Format("%d",i+1);
				m_vdslist.InsertItem(i,str);
				str=m_db.getString("InsCode");
				m_vdslist.SetItemText(i,1,str);
				str=m_db.getString("PDMNum");
				m_vdslist.SetItemText(i,2,str);
				m_vdslist.SetItemText(i,3,str);
				str=m_db.getString("PDMState");
				m_vdslist.SetItemText(i,4,str);
				if (m_part==0)
				{
					str.Format("%d",m_db.getInt("PrtNum"));
				}
				else
					str.Format("%d",m_db.getInt("PdtNum"));
				m_vdslist.SetItemData(i,atol(str));
				i++;
			} while(m_db.MoveNext());
		}
		if (First)
		{
			m_pdmlist.DeleteAllItems();
			sql.Format("SELECT * FROM XtdetamsAll_title WHERE Tcode LIKE '%%%s%%'",m_code);
			CMyDatabase m_pdm;
			if(!m_pdm.Open(Ftp_struc.Database))
			{
				return FALSE;
			}
			Z=m_pdm.Query(sql);
			if (Z==-1)
			{
				return FALSE;
			}
			else
				if(Z!=0)
				{
					int i=0;
					m_pdm.MoveBegin();
					do {
						str.Format("%d",i+1);
						m_pdmlist.InsertItem(i,str);
						str=m_pdm.getString("Tcode");
						m_pdmlist.SetItemText(i,1,str);
						i++;
					} while(m_pdm.MoveNext());
				}
				m_pdm.Close();
		}
		return TRUE;
}
void CIntDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);

	// TODO: �ڴ������Ϣ����������
}
