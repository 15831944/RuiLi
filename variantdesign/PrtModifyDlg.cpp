// PrtModifyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PrtModifyDlg.h"
#include "MyDatabase.h"
#include "CommonFuncClass.h"
#include "DownLoadDlg.h"
#include "DetailInfoClass.h"
#include "ProWindows.h"
#include "PrtClass.h"
#include "ProductClass.h"


// CPrtModifyDlg �Ի���
extern CMyDatabase m_db;
extern CPtrArray p_classarray;
extern FtpInfo Ftp_struc;

IMPLEMENT_DYNAMIC(CPrtModifyDlg, CDialog)
CPrtModifyDlg::CPrtModifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrtModifyDlg::IDD, pParent)
	, m_key(_T(""))
	, m_workspace(0)
	, m_audit(FALSE)
{
}

CPrtModifyDlg::~CPrtModifyDlg()
{
}

void CPrtModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KEY, m_key);
	DDX_Control(pDX, IDC_COMBO_KEY, m_combokey);
	DDX_Control(pDX, IDC_TREE_PART, m_treeprt);
	DDX_Control(pDX, IDC_LIST_PART, m_listprt);
	DDX_Radio(pDX, IDC_RADIO_WORKAPACE, m_workspace);
	DDX_Check(pDX, IDC_CHECK_AUDIT, m_audit);
}


BEGIN_MESSAGE_MAP(CPrtModifyDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_PDM, OnBnClickedRadioPdm)
	ON_BN_CLICKED(IDC_RADIO_WORKAPACE, OnBnClickedRadioWorkapace)
	ON_NOTIFY(NM_CLICK, IDC_TREE_PART, OnNMClickTreePart)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CPrtModifyDlg ��Ϣ�������
/************************************************************************/
/* ��ѯ��ť                                                                     */
/************************************************************************/
void CPrtModifyDlg::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString sql;
	m_listprt.DeleteAllItems();
	if (m_workspace==0 && m_audit)			
	{
	   sql.Format("SELECT * FROM NPartInsInfoTable WHERE InsCode LIKE '%%%s%%' AND FinishState='���' AND AuditState='���δͨ��'",m_key);
	   if (!GetPrtFromBase(sql,FALSE))
	   {
		   AfxMessageBox("����ʧ��");
		   return;
	   }
	}
	else
	{
		if (m_workspace==0 && (!m_audit))
		{
			sql.Format("SELECT * FROM NPartInsInfoTable WHERE InsCode LIKE '%%%s%%' AND PDMState='δ�ύ'",m_key);
			if (!GetPrtFromBase(sql,FALSE))
			{
				AfxMessageBox("����ʧ��");
				return;
			}
		}
		else
		{
			sql.Format("SELECT * FROM NPartInsInfoTable WHERE PDMNum LIKE '%%%s%%' AND PDMState='�ύ'",m_key);
			if (!GetPrtFromBase(sql,TRUE))
			{
				AfxMessageBox("����ʧ��");
				return;
			}//����PDM���ݿ�
		}
	}
	
}

BOOL CPrtModifyDlg::OnInitDialog()
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	int index=m_combokey.AddString("ͼ��");
	m_combokey.SetCurSel(index);
	m_listprt.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listprt.InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
	m_listprt.InsertColumn(1,"ͼ��",LVCFMT_LEFT,100,-1);
	m_listprt.InsertColumn(2,"����",LVCFMT_LEFT,55,-1);
	m_listprt.InsertColumn(3,"���",LVCFMT_LEFT,55,-1);
	m_listprt.InsertColumn(4,"���״̬",LVCFMT_LEFT,55,-1);
    m_listprt.InsertColumn(5,"�����",LVCFMT_LEFT,55,-1);
	UpdateData(FALSE);
	//�õ��������
	commonfun.ObtainPartModelClass(&m_treeprt);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ����ͼ�Ų���ʵ��                                                     */
/************************************************************************/
BOOL CPrtModifyDlg::GetPrtFromBase(CString sql,BOOL PDM)
{
	CString str;
	CStringArray TypeNumArray;
	TypeNumArray.RemoveAll();
	m_listprt.DeleteAllItems();
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
		int i=0;
		m_db.MoveBegin();
		do {
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
			TypeNum=m_db.getInt("TypeNum");
			str.Format("%d",TypeNum);
			TypeNumArray.Add(str);
			str=m_db.getString("FinishState");
			m_listprt.SetItemText(i,4,str);
			str=m_db.getString("Designer");
			m_listprt.SetItemText(i,5,str);
			str.Format("%d",m_db.getInt("PrtNum"));
			m_listprt.SetItemData(i,atol((str)));
			i++;
		} while(m_db.MoveNext());
		for (int j=0;j<TypeNumArray.GetSize();j++)
		{
			sql.Format("SELECT * FROM NPartType where Number=%s",TypeNumArray[j]);
			Z=m_db.Query(sql);
			if (Z==-1 || Z==0)
			{
				continue;
			}
			else
			{
				str=m_db.getString("Name");
				m_listprt.SetItemText(j,3,str);
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* �������ť                                                         */
/************************************************************************/
void CPrtModifyDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL IsSelect=FALSE;
	ProSolid solid;
	CCommonFuncClass commonfun;
	CPrtClass prtclass;
	CString sql,str;
	for (int i=0;i<m_listprt.GetItemCount();i++)
	{
		if (m_listprt.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			 IsSelect=TRUE;
			 InsCode.Format("%d",m_listprt.GetItemData(i));           //�������
			 break;
		}
	}
	if (!IsSelect)
	{
		AfxMessageBox("��ѡ����Ҫ�򿪵�ģ�ͣ�");
		return;
	}
	
	sql.Format("SELECT * FROM NPartInsInfoTable WHERE PrtNum=%s",InsCode);
    int Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		  return;
	}
	else
	{
		info =new DetailInfoClass;
		info->SolidID=m_db.getInt("PrtNum");
		info->InsCode=m_db.getString("InsCode");             //��Ҫ���������
		info->PDMCode=m_db.getString("PDMNum");
		info->OldSolidFileName=m_db.getString("SolidFileName");
		info->OldDrwName=m_db.getString("DrawingName");
		info->OldSolidFilePath=m_db.getString("SolidPlace");
		info->OldDrwFilePath=m_db.getString("DrawingPlace");
		info->IsLocalOrServer=m_db.getString("LocalState");
		str=m_db.getString("PDMState");
	}
	//��Ҫ�õ�PDM���ļ����ͱ���λ��
	if (str=="�ύ")
	{
		BOOL Find=TRUE;
		if (!prtclass.GetDocumentsFromPDM(Ftp_struc.Database,info,&Find))
		{
			return;
		}
	}
	if (info->IsLocalOrServer=="����")
	{
		//ֱ�Ӵ򿪣�
		CPrtClass prtclass;
		if (!prtclass.OpenPartFile(info->OldSolidFileName,info->OldSolidFilePath,info->OldDrwName,info->OldDrwFilePath,&solid))
		{
			   AfxMessageBox("�����ʧ��");
			   delete info;
			   return;
		}
		else
		{
			info->solid=solid;
			p_classarray.Add(info);
			sql.Format("SELECT * FROM NPartInsInfoTable WHERE PrtNum=%d",info->SolidID);
			if(!prtclass.GetInfoFromDatabaseBase((int)p_classarray.GetSize()-1,sql))        //�õ�����������ϸ��Ϣ
			{
				 AfxMessageBox("�޷��õ�����������ϸ��Ϣ��");
				 return;
			}
			prtclass.ActivatePart(solid);
			if(!commonfun.SetParam((int)p_classarray.GetSize()-1))
			{
				AfxMessageBox("����ͼ�ŵȲ�������");
				return;
			}
		}
	}
	else
		if (info->IsLocalOrServer=="������")
		{
			//CDownLoadDlg downloaddlg;
			//downloaddlg.info=info;
			//downloaddlg.DoModal();//�����Ի���������ѡ��
			if (!OpenPrtFileFromSever()) 
			{
				AfxMessageBox("�����ʧ��");
				return;
			}
		}
	OnOK();
}

void CPrtModifyDlg::OnBnClickedRadioPdm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_workspace=0;
	m_listprt.DeleteAllItems();
	GetDlgItem(IDC_CHECK_AUDIT)->EnableWindow(FALSE);
}

void CPrtModifyDlg::OnBnClickedRadioWorkapace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_CHECK_AUDIT)->EnableWindow(TRUE);
	m_listprt.DeleteAllItems();
	m_workspace=1;
}

void CPrtModifyDlg::OnNMClickTreePart(NMHDR *pNMHDR, LRESULT *pResult)
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
	if (hItem!=NULL && (uFlags&TVHT_ONITEM))
	{
		num=(int)m_treeprt.GetItemData(hItem);
		if (!ObtianPartInfByType(num))
		{
			return;
		}
	}
	*pResult = 0;
}
/************************************************************************/
/* �õ���������������¼����ģ��                                       */
/************************************************************************/
BOOL CPrtModifyDlg::ObtianPartInfByType(int num)
{
	CString sql,str,Type;
	sql.Format("SELECT * FROM NPartType where Number=%d",num);
	int	Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			Type=m_db.getString("Name");
		} while(m_db.MoveNext());
	}
	if (m_workspace==0)
	{
		sql.Format("SELECT * FROM NPartInsInfoTable where TypeNum=%d and PDMState='δ�ύ'",num);
	}
	else
	    sql.Format("SELECT * FROM NPartInsInfoTable where TypeNum=%d and PDMState='�ύ'",num);
	Z=m_db.Query(sql);
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
		int i=0;
		m_db.MoveBegin();
		do {
			str.Format("%d",i+1);
			m_listprt.InsertItem(i,str);
			if (m_workspace==0)
			{
				str=m_db.getString("InsCode");
			}
			else
                str=m_db.getString("PDMNum");
			m_listprt.SetItemText(i,1,str);
			str=m_db.getString("Name");
			m_listprt.SetItemText(i,2,str);
			m_listprt.SetItemText(i,3,Type);
			str=m_db.getString("FinishState");
			m_listprt.SetItemText(i,4,str);
			str=m_db.getString("Designer");
			m_listprt.SetItemText(i,5,str);
			str.Format("%d",m_db.getInt("PrtNum"));
			m_listprt.SetItemData(i,atol(str));
			i++;
		} while(m_db.MoveNext());
	}
	return TRUE;
}
void CPrtModifyDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: �ڴ������Ϣ����������
}
BOOL CPrtModifyDlg::OpenPrtFileFromSever()
{
	ProSolid solid;
	ProError status;
	int w_id,k;
	CString SolidName,DrwName,sql;
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CPrtClass prtclass;
	CString temppath,DrwPath,SolidPath;
	//���������
	if(info->OldSolidFileName.Right(4)!=".prt")
	{
		SolidName=info->OldSolidFileName;
		k=SolidName.ReverseFind('.');
		if (k!=-1)      //˵��û��
		{
			SolidName.Delete(k,(SolidName.GetLength()-k));
			info->SolidFileName=SolidName;
		}
	}
	else
	{
		info->SolidFileName=info->OldSolidFileName;
	}
	if (info->OldDrwName!="")                  //�ж�άͼ
	{
		if(info->OldDrwName.Right(4)!=".drw")
		{
			DrwName=info->OldDrwName;
			k=DrwName.ReverseFind('.');
			if (k!=-1)      //˵��û��
			{
				DrwName.Delete(k,(DrwName.GetLength()-k));
				info->DrwName=DrwName;
			}
		}
		else
		{
			info->DrwName=info->OldDrwName;
		}
	}
	temppath=Ftp_struc.LocalPath;
	if (temppath.Right(1)=="\\")
	{
		k=temppath.ReverseFind('\\');
		if (k!=-1)
		{
			temppath=temppath.Left(k);
		}
	}
	info->SolidFilePath=temppath;
	info->DrwFilePath=temppath;

	//�ж�Ҫ���صģ�����Ŀ¼+ģ�͵����֣��������Ƿ����
	if (!commonfun.IsFileExist(info->SolidFilePath,info->DrwFilePath,info->SolidFileName,info->DrwName))
	{
		delete info;
		return FALSE;
	}
	//���������ж��ڴ����Ƿ��и�ģ��
	//���������Ƿ�����ͬ����
	if (!prtclass.IsPartFileAlreadyInSession(info->SolidFileName,info->DrwName))
	{
		AfxMessageBox("���ڴ����Ѿ���ͬ���ļ����ڣ�������ڴ棬�ٴ�������");
		return FALSE;
	}
	//�����ļ�
	if (!prtclass.DownLoadPart(info->OldSolidFileName,info->OldSolidFilePath,info->OldDrwName,info->OldDrwFilePath,info->SolidFilePath))
	{
		//AfxMessageBox("�޷����ظ����");
		return FALSE;
	}
	else
	{
		if (!prtclass.OpenPartFile(info->OldSolidFileName,info->SolidFilePath,info->OldDrwName,info->DrwFilePath,&solid))
		{
			AfxMessageBox("�����ʧ��");
			return FALSE;
		}
		else
		{
			info->solid=solid;
			p_classarray.Add(info);
			//��Ҫ����
			if (!pdtclass.NewRename(solid,info->SolidFileName,&(info->DrwName)))
			{
				AfxMessageBox("������ʧ�ܣ�");
				return FALSE;
			}
		}
	}
	sql.Format("SELECT * FROM NPartInsInfoTable WHERE PrtNum=%d",info->SolidID);
	if (!prtclass.GetInfoFromDatabaseBase((int)p_classarray.GetSize()-1,sql))
	{
		AfxMessageBox("�޷��������������ϸ��Ϣ");
		return FALSE;
	}
	status = ProMdlDisplay((ProMdl)solid);
	if (!commonfun.SetParam((int)p_classarray.GetSize()-1)) 
	{
		return FALSE;
	}
	ProMdlWindowGet((ProMdl)solid,&w_id);
	status = ProWindowActivate(w_id);
	return TRUE;
}