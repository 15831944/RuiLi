// UploadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <ProToolkit.h>
#include "variantdesign.h"
#include <ProMenuBar.h>
#include "UploadDlg.h"
#include "ProWindows.h"
#include "afxtempl.h"
#include "MyDatabase.h"
#include "Compute.h"
#include "CommonFuncClass.h"
#include "ProUtil.h"
#include "ProMdl.h"
#include "ProSolid.h"
#include "DetailDlg.h"
#include "ProductMenberBasicInfoDlg.h"
#include "UpdateDlg.h"
#include "PrtAsmAuditDlg.h"

extern CUploadDlg  uploaddlg;
extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CArray <Database,Database> Database_struc_temp;
extern CArray <Database,Database> Database_struc;
extern CPtrArray p_classarray;
CPrtAsmAuditDlg *prtasmauditdlg=NULL;


ProError SolidFeatGet(ProSolid solid,ProFeature **p_data,BOOL nFlag);
// CUploadDlg �Ի���

IMPLEMENT_DYNAMIC(CUploadDlg, CDialog)
CUploadDlg::CUploadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUploadDlg::IDD, pParent)
{
	
}

CUploadDlg::~CUploadDlg()
{

}

void CUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ASM, m_asmmodeltree);
	DDX_Control(pDX, IDC_LIST_DIM, m_dim);
}


BEGIN_MESSAGE_MAP(CUploadDlg, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_ASM, OnNMDblclkTreeAsm)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_TREE_ASM, OnNMClickTreeAsm)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DIM, OnNMClickListDim)
	ON_BN_CLICKED(IDC_BUTTON_PDTINFO, OnBnClickedButtonPdtinfo)
	ON_BN_CLICKED(IDC_BUTTON_FALSE, OnBnClickedButtonFalse)
END_MESSAGE_MAP()


// CUploadDlg ��Ϣ�������

BOOL CUploadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CCompute compute;
	CCommonFuncClass commonfun;
	//ȥ��������û�еĻ�����Ϣ
	commonfun.IsBasicInfoAlreadyHave();
	commonfun.RemoveNotInSession();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	m_ImageList.Create(16, 16, ILC_COLOR8, 0, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_asmmodeltree.SetImageList(&m_ImageList,TVSIL_NORMAL);

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FALSE)->EnableWindow(FALSE);
	ProError status;
	ProMdlType p_type;
	CString sql,str;
	status=ProMdlCurrentGet((ProMdl*)&Productsolid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�Բ�����û�д�װ��ģ�ͣ�");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	status=ProMdlTypeGet((ProMdl)Productsolid,&p_type);
	if (p_type!=PRO_ASSEMBLY)
	{
	    AfxMessageBox("�ⲻ�ǲ�Ʒ������������");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	else
	{
		s=commonfun.IsBasicInfoHave(Productsolid);
		if (s==-1)
		{
			AfxMessageBox("�޷��õ������Ʒ����Ϣ���޷�Ϊ�����");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
	}

	m_dim.SetExtendedStyle(LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_dim.InsertColumn(0,"�ɱ�ߴ�",LVCFMT_LEFT,95,-1);
	m_dim.InsertColumn(1,"��ֵ",LVCFMT_LEFT,80,-1);
	m_dim.InsertColumn(2,"��ƫ��",LVCFMT_LEFT,70,-1);
	m_dim.InsertColumn(3,"��ƫ��",LVCFMT_LEFT,70,-1);
	m_dim.InsertColumn(4,"��ע",LVCFMT_LEFT,100,-1);

	//��ʾ��Ʒ�Ŀɱ����
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum!=-1)
	{
		if (!ListDim(s,"asm",&m_dim))
		{
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
	}
	//�õ���Ʒ��ģ����
	commonfun.AddProductToTree(Productsolid,&m_asmmodeltree);
	//�ж���������û��������
	if (!IsProductCanAudit(s))
	{
		str="�����㲿��û����ˣ��Ƿ���˻�û����˵��㲿��";
		if(AfxMessageBox(str,MB_YESNO)==IDYES)
		{
             //����һ���Ի���
			if (prtasmauditdlg!=NULL)
			{
				delete prtasmauditdlg;
			}
			prtasmauditdlg = new CPrtAsmAuditDlg;
			prtasmauditdlg->ProductSolid=Productsolid;
			prtasmauditdlg->Create(IDD_DIALOG_PRTNDAUDIT,NULL);
			prtasmauditdlg->ShowWindow(SW_SHOW);
		}
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	return TRUE;
}

void CUploadDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRepaint(PRO_VALUE_UNUSED);
}

void CUploadDlg::OnBnClickedOk()
{
	CTime time=CTime::GetCurrentTime();
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime=time.Format("%Y-%m-%d %H:%M:%S");
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="���ͨ��";

	m_db.m_pConnection->Close();
	m_db.ReOpen();
	m_db.m_pConnection->BeginTrans();
	try
	{
		if (!AuditFunc(s))
		{
			m_db.m_pConnection->RollbackTrans();
			AfxMessageBox("�ύʧ��");
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
		AfxMessageBox("�ύʧ�ܣ�");
	}
}

void CUploadDlg::OnNMDblclkTreeAsm(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CUploadDlg::OnBnClickedCancel()
{
	ProError status;
	status = ProMdlDisplay((ProMdl)Productsolid);
	if(status!= PRO_TK_NO_ERROR)
	{
		return;
	}
	int w_id;
	ProWindowCurrentGet(&w_id);
	ProWindowActivate(w_id);
	CDialog::DestroyWindow();
}
void CUploadDlg::OnNMClickTreeAsm(NMHDR *pNMHDR, LRESULT *pResult)
{
	//HTREEITEM hItem;
	//UINT uFlags=0;
	//CPoint point;
	//CString Name,str;
	//ProName w_name;			     
	//ProError status;
	//ProMdl  mdl;
	//ProMdlType type;
	//BOOL IsHave=FALSE;
	//point= GetCurrentMessage()->pt;
	//m_asmmodeltree.ScreenToClient(&point);
	//hItem =m_asmmodeltree.HitTest(point, &uFlags);
	//if (hItem!=NULL && (uFlags&TVHT_ONITEM))
	//{
	//	Name=m_asmmodeltree.GetItemText(hItem);
	//	//UpdateData(FALSE);
	//}
	//else
	//	return;
	//if(m_asmmodeltree.GetChildItem(hItem)!=NULL)
	//{
	//	type=PRO_MDL_ASSEMBLY;
	//}
	//else
	//{  type=PRO_MDL_PART;
	//}
	//int k=Name.Find(".");
	//str=Name.Left(k);
	//ProStringToWstring(w_name,str.GetBuffer());
	//status=ProMdlInit(w_name,type,&mdl);
	//if (status!=PRO_TK_NO_ERROR)
	//{
	//	return;
	//}
	//status = ProMdlDisplay(mdl);
	//int w_id;
	//ProWindowCurrentGet(&w_id);
	//ProWindowActivate(w_id);
	*pResult = 0;
}

/************************************************************************/
/* �ж������Ʒ�Ƿ�������                                             */
/************************************************************************/
BOOL CUploadDlg::IsProductCanAudit(int s)
{
	 CString sql,str;
	 sql.Format("SELECT * FROM NProductInsInfoTable WHERE PdtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	 int Z=m_db.Query(sql);
	 if (Z==-1 || Z==0)
	 {
		 AfxMessageBox("û�������Ʒ���޷����");
		 return FALSE;
	 }
	 else
	 {
		 m_db.MoveBegin();
		 do {
		 	str=m_db.getString("FinishState");
			if (str=="δ���")
			{
				AfxMessageBox("�����Ʒ��û�������ɣ����������ƣ������");
				return FALSE;
			}
			str=m_db.getString("AuditState");
			if (str=="���ͨ��")
			{
				AfxMessageBox("�����Ʒ�Ѿ����ͨ�������������");
				return FALSE;
			}
		 } while(m_db.MoveNext());
	 }
	 //�ж�������µ��㲿���Ƿ��Ѿ����
	 BOOL IsAudit;
	 CString FileName;
	 if (!AffirmPartIsAlreadyAudit(((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,&IsAudit,&FileName))
	 {
         return FALSE;
	 }
	 if (!IsAudit)
	 {
		 //str.Format("�ļ�%s��û����ˣ��������",FileName);
		 //AfxMessageBox(str);
		 return FALSE;
	 }
	 return TRUE;
}
/************************************************************************/
/* �жϲ�����������Ƿ������                                           */
/************************************************************************/
BOOL CUploadDlg::AffirmPartIsAlreadyAudit(int SolidID,BOOL *IsAudit,CString *FileName)
{
	CString sql,str;
	int Z;
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
					*FileName=m_db.getString("SolidFileName");
				} while(m_db.MoveNext());
				if (str=="���ͨ��")
				{
					*IsAudit=TRUE;
				}
				else
				{
					*IsAudit=FALSE;
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
                        *FileName=m_db.getString("SolidFileName");
					} while(m_db.MoveNext());
					if (str=="���ͨ��")
					{
						*IsAudit=TRUE;
						return FALSE;
					}
					else
					{
						*IsAudit=FALSE;
						return FALSE;
					}
				}

			}
		if (!(*IsAudit))
		{
			return TRUE;
		}
	}
	return TRUE;
}

void CUploadDlg::OnNMClickListDim(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ProDimension haha;
	CCompute compute;
	ProAsmcomppath comppath;
	ProSolid solidtemp;
	CString str;
	int k,postID;
	BOOL IsHave=FALSE;
	for (k=0;k<m_dim.GetItemCount();k++)
	{
		if(m_dim.GetItemState(k,LVIS_SELECTED)==LVIS_SELECTED)
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
	str.Delete(0,1);
	haha.id=atoi(str.GetBuffer());

	int j=Database_struc_temp[k].PARADIMSYMBOL.Find(":");
	if (j!=-1)
	{
		str=Database_struc_temp[k].PARADIMSYMBOL;
		str.Delete(0,j+1);
		postID=atoi(str.GetBuffer());
		JuBing_struc.RemoveAll();
		if (!compute.ObtainJuBing(Productsolid))
		{
			return;
		}
		for (int i=0;i<JuBing_struc.GetSize();i++)
		{
			if (postID==JuBing_struc[i].number)
			{
				comppath=JuBing_struc[i].comppath;
				solidtemp=JuBing_struc[i].solid;
				break;
			}
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
	}
	else
	{
		haha.type=PRO_DIMENSION;
		haha.owner=(m_menmdl);
		ProWindowRepaint(PRO_VALUE_UNUSED);
		ProDimensionShow(&haha,NULL,NULL,NULL);
	}
		ProWindowRefresh(PRO_VALUE_UNUSED);
	*pResult = 0;
}
/************************************************************************/
/*  �ж�����Ƿ���Ա���                                                */
/************************************************************************/
BOOL CUploadDlg::IsAsmCanAudit(ProSolid solid,int i,CListCtrl *list)
{
	CString Type;
	Type=list->GetItemText(i,5);
	if (Type=="���")
	{
		return TRUE;
	}
	else
	{
		if (solid!=Productsolid)			//˵���ǲ���
		{
			for (int j=0;j<list->GetItemCount();j++)
			{
				if ((list->GetItemText(j,5)=="���") && (list->GetItemText(j,4)=="�����"))
				{
					AfxMessageBox("����������������˲���");
					return FALSE;
				}
			}
		}
		else
		{
			for (int j=1;j<list->GetItemCount();j++)
			{
				if ((list->GetItemText(j,4)=="�����"))
				{
					AfxMessageBox("����������/����������˲�Ʒ");
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
BOOL CUploadDlg::ListDim(int s,CString Type,CListCtrl *list)
{
	Database_struc_temp.RemoveAll();
	CCompute compute;
	list->DeleteAllItems();
	int t=0;
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum==-1)
	{
		AfxMessageBox("�ⲻ��ģ��,û�пɱ����");
		return TRUE;
	}
	else
	{
		if (Type=="���")
		{
			if(!compute.GetVariantForPart(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum,TRUE))//��������������ʱ�ṹ��
			{
				return FALSE;
			}
			if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))		 //Ҳ��Ҫ����ƫ����Ϣ  
			{
				return FALSE;
			}

			for (int i=0;i<Database_struc_temp.GetSize();i++)	 //������
			{
				int indexNo=list->InsertItem(LVIF_TEXT|LVIF_PARAM,t,Database_struc_temp[i].name,
					0,0,0,Database_struc_temp[i].number);
				list->SetItemText(indexNo,1,Database_struc_temp[i].value);
				list->SetItemText(indexNo,2,Database_struc_temp[i].up);	 //��ƫ��
				list->SetItemText(indexNo,3,Database_struc_temp[i].down);	 //��ƫ��
				list->SetItemText(indexNo,4,Database_struc_temp[i].note);
				t++;
			}
		}
		else
		{
			if(!compute.ObtainAsmFromBaseTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum))
			{
				//AfxMessageBox("�޷��õ���ģ�͵Ŀɱ�ߴ�");
				return FALSE;
			}
			if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
			{
				return FALSE;
			}
			//��ʾ�����ߴ�
			for(int i=0;i<Database_struc_temp.GetSize();i++)	
			{
				int indexNo=list->InsertItem(LVIF_TEXT|LVIF_PARAM,t,Database_struc_temp[i].name,
					0,0,0,Database_struc_temp[i].number);
				list->SetItemText(indexNo,1,Database_struc_temp[i].value);
				list->SetItemText(indexNo,2,Database_struc_temp[i].up);	 //��ƫ��
				list->SetItemText(indexNo,3,Database_struc_temp[i].down);	 //��ƫ��
				list->SetItemText(indexNo,4,Database_struc_temp[i].note);
				t++;
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* �鿴��Ʒ������Ϣ                                                     */
/************************************************************************/
void CUploadDlg::OnBnClickedButtonPdtinfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CUpdateDlg dlg;
	dlg.s=s;
	if(dlg.DoModal()==IDOK)
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_FALSE)->EnableWindow(TRUE);
	}
}
BOOL CUploadDlg::AuditFunc(int s)
{
	CString sql;
	sql.Format("UPDATE NProductInsInfoTable Set Auditor='%s',AuditTime='%s',AuditState='%s' where PdtNum=%d",
		((DetailInfoClass *)p_classarray.GetAt(s))->Auditor,
		((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime,
		((DetailInfoClass *)p_classarray.GetAt(s))->AuditState,
		((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	if(m_db.Execute(sql)==0)
	{
		return FALSE;
	}
		return TRUE;
}
void CUploadDlg::OnBnClickedButtonFalse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CTime time=CTime::GetCurrentTime();
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime=time.Format("%Y-%m-%d %H:%M:%S");
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="���δͨ��";

	m_db.m_pConnection->Close();
	m_db.ReOpen();
	m_db.m_pConnection->BeginTrans();
	try
	{
		if (!AuditFunc(s))
		{
			m_db.m_pConnection->RollbackTrans();
			AfxMessageBox("�ύʧ��");
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
		AfxMessageBox("�ύʧ�ܣ�");
	}
	CDialog::DestroyWindow();
}
