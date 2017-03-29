// PrtAsmAuditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PrtAsmAuditDlg.h"
#include "DetailInfoClass.h"
#include "CommonFuncClass.h"
#include "MyDatabase.h"
#include "ProMdl.h"
#include "ProWindows.h"

extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CPtrArray p_classarray;
extern CPrtAsmAuditDlg *prtasmauditdlg;
// CPrtAsmAuditDlg �Ի���

IMPLEMENT_DYNAMIC(CPrtAsmAuditDlg, CDialog)
CPrtAsmAuditDlg::CPrtAsmAuditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrtAsmAuditDlg::IDD, pParent)
{
	
}

CPrtAsmAuditDlg::~CPrtAsmAuditDlg()
{
	//delete prtasmauditdlg;
}

void CPrtAsmAuditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRTNDAUDIT, m_list);
}


BEGIN_MESSAGE_MAP(CPrtAsmAuditDlg, CDialog)
	//ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_MOVE()
	ON_WM_KEYDOWN()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRTNDAUDIT, OnNMDblclkListPrtndaudit)
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()


// CPrtAsmAuditDlg ��Ϣ�������

BOOL CPrtAsmAuditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CCommonFuncClass commonfun;
    m_list.DeleteAllItems();
	ListView_SetExtendedListViewStyle(m_list.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0,"���",LVCFMT_LEFT,50,-1);
	m_list.InsertColumn(1,"����",LVCFMT_LEFT,120,-1);
	m_list.InsertColumn(2,"����",LVCFMT_LEFT,50,-1);
	s=commonfun.IsBasicInfoHave(ProductSolid);
	if (s==-1)
	{
		AfxMessageBox("�޷��õ��������Ļ�����Ϣ");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	if(!AffirmWhichPartIsNotAlreadyAudit(((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,&m_list))
	{
		m_list.DeleteAllItems();
        return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
BOOL CPrtAsmAuditDlg::ListPart(int *i,CString Name,CListCtrl *list)
{
	CString str;
	str.Format("%d",(*i)+1);
	list->InsertItem(*i,str);
	list->SetItemText(*i,1,Name);
	if (Name.Find(".prt")!=-1)
	{
		str="���";
	}
	else
		str="����";
	list->SetItemText(*i,2,str);
    (*i)++;
	return TRUE;
}
/************************************************************************/
/* �жϲ�����������Ƿ������                                           */
/************************************************************************/
BOOL CPrtAsmAuditDlg::AffirmWhichPartIsNotAlreadyAudit(int SolidID,CListCtrl *m_prtlist)
{
	CString sql,str,str1,FileName;
	int Z,k=0;
	CStringArray SolidIDArray;
	CStringArray TypeArray;
	SolidIDArray.RemoveAll();
	TypeArray.RemoveAll();
	sql.Format("SELECT * FROM NProductBOMTable WHERE PdtNum=%d",SolidID);
	Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			str.Format("%d",m_db.getInt("PrtNum"));
			SolidIDArray.Add(str);
			str=m_db.getString("Type");
			TypeArray.Add(str);
		} while(m_db.MoveNext());
	}
	for (int i=0;i<SolidIDArray.GetSize();i++)
	{
		if (TypeArray[i]=="prt")
		{
			sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%s",SolidIDArray[i]);
			Z=m_db.Query(sql);
			if (Z==0 || Z==-1)
			{
				return FALSE;
			}
			else
			{
				m_db.MoveBegin();
				do {
					str=m_db.getString("AuditState");
                    str1=m_db.getString("FinishState");
					FileName=m_db.getString("SolidFileName");
				} while(m_db.MoveNext());
				if (str1=="δ���")
				{
					str.Format("���%s��û�������ɣ�������˲�Ʒ",FileName);
					AfxMessageBox(str);
					return FALSE;
				}
				else
				if (str=="���δͨ��")
				{
					ListPart(&k,FileName,m_prtlist);
				}
				else
				{
					continue;
				}
			}
		}
		else
			if (TypeArray[i]=="asm")
			{
				sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%s",SolidIDArray[i]);
				Z=m_db.Query(sql);
				if (Z==0 || Z==-1)
				{
					return FALSE;
				}
				else
				{
					m_db.MoveBegin();
					do {
						str=m_db.getString("AuditState");
						str1=m_db.getString("FinishState");
						FileName=m_db.getString("SolidFileName");
					} while(m_db.MoveNext());
					if (str1=="δ���")
					{
						str.Format("����%s��û�������ɣ�������˲�Ʒ",FileName);
						AfxMessageBox(str);
						return FALSE;
					}
					else
						if (str=="���δͨ��")
						{
							ListPart(&k,FileName,m_prtlist);
						}
						else
						{
							continue;
						}
				}
			}
	}
	return TRUE;
}
//void CPrtAsmAuditDlg::OnBnClickedOk()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	OnOK();
//}

void CPrtAsmAuditDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnCancel();
	CDialog::DestroyWindow();
}

void CPrtAsmAuditDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);
	m_list.DeleteAllItems();
	if(!AffirmWhichPartIsNotAlreadyAudit(((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,&m_list))
	{
		m_list.DeleteAllItems();
		return;
	}
	// TODO: �ڴ������Ϣ����������
}

void CPrtAsmAuditDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
     //AfxMessageBox("�ɹ�");
	// TODO: �ڴ������Ϣ����������
}

void CPrtAsmAuditDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	// TODO: �ڴ������Ϣ����������
}

void CPrtAsmAuditDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    //AfxMessageBox("����");
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPrtAsmAuditDlg::OnNMDblclkListPrtndaudit(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    ProError status;
	BOOL IsOk;
	int w_id;
	CString str,Name;
	ProMdlType p_type;
	ProName w_name;
	ProMdl mdl;
	for (int i=0;i<m_list.GetItemCount();i++)
	{
		if (m_list.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsOk=TRUE;
			str=m_list.GetItemText(i,1);
			break;
		}
	}
	int k=str.Find(".prt");
	if (k!=-1)   //˵�������
	{
        Name=str.Left(k);
		p_type=PRO_MDL_PART;
	}
	k=str.Find(".asm");
	if (k!=-1)
	{
		Name=str.Left(k);
		p_type=PRO_MDL_ASSEMBLY;
	}
	ProStringToWstring(w_name,Name.GetBuffer());
	status=ProMdlInit(w_name,p_type,&mdl);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�޷��õ����ģ�͵ľӱ������ֹ������");
	}
	else
	{
		status=ProObjectwindowCreate(w_name,(ProType)p_type,&w_id);
		status=ProMdlDisplay(mdl);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("�޷���ʾ��άͼ��");
			return;
		}
		status=ProWindowActivate(w_id);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("�޷����ǰ���ڣ����ֹ����");
			return;
		}
	}
	*pResult = 0;
}

void CPrtAsmAuditDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//AfxMessageBox("����");
	m_list.DeleteAllItems();
	if(!AffirmWhichPartIsNotAlreadyAudit(((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,&m_list))
	{
		m_list.DeleteAllItems();
		return;
	}
	CDialog::OnNcLButtonDown(nHitTest, point);
}
