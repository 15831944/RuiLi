// ProjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "ProjectDlg.h"
#include "MyDatabase.h"
#include "PrtClass.h"
#include "DetailInfoClass.h"
#include "ProductClass.h"
#include "ProWindows.h"




extern CMyDatabase m_db;
extern CPtrArray p_classarray;
extern FtpInfo Ftp_struc;

// CProjectDlg �Ի���

IMPLEMENT_DYNAMIC(CProjectDlg, CDialog)
CProjectDlg::CProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectDlg::IDD, pParent)
	//, m_note(_T(""))
	, m_all(FALSE)
	, m_note(_T(""))
{
}

CProjectDlg::~CProjectDlg()
{
}

void CProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODETAIL, m_projdetail);
	DDX_Check(pDX, IDC_CHECK_ALL, m_all);
	DDX_Control(pDX, IDC_LIST_TASK, m_task);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
}


BEGIN_MESSAGE_MAP(CProjectDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODETAIL, OnNMClickListProdetail)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TASK, OnNMClickListTask)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_FINISH, OnBnClickedButtonFinish)
	ON_BN_CLICKED(IDC_BUTTON_REJECT, OnBnClickedButtonReject)
	ON_BN_CLICKED(IDC_BUTTON_ACCEPT, OnBnClickedButtonAccept)
END_MESSAGE_MAP()


// CProjectDlg ��Ϣ�������

BOOL CProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	m_projdetail.DeleteAllItems();
	m_task.DeleteAllItems();
	ListView_SetExtendedListViewStyle(m_projdetail.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_projdetail.InsertColumn(0,"���",LVCFMT_LEFT,20,-1);
	m_projdetail.InsertColumn(1,"��Ŀ����",LVCFMT_LEFT,135,-1);
	m_projdetail.InsertColumn(2,"��Ŀ��",LVCFMT_LEFT,135,-1);
	m_projdetail.InsertColumn(3,"�ͻ�",LVCFMT_LEFT,135,-1);
	m_projdetail.InsertColumn(4,"״̬",LVCFMT_LEFT,135,-1);
	ListView_SetExtendedListViewStyle(m_task.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_task.InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
	m_task.InsertColumn(1,"������",LVCFMT_LEFT,70,-1);
	m_task.InsertColumn(2,"������",LVCFMT_LEFT,70,-1);
	m_task.InsertColumn(3,"��ʼ����",LVCFMT_LEFT,70,-1);
	m_task.InsertColumn(4,"��ֹ����",LVCFMT_LEFT,70,-1);
	m_task.InsertColumn(5,"���״̬",LVCFMT_LEFT,75,-1);

    CString sql,str;
	sql.Format("SELECT * FROM Main where ProjectState='%s'","����������");
	int Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		this->SendMessage(WM_CLOSE,0,0);
        return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			str.Format("%d",i+1);
			m_projdetail.InsertItem(i,str);
			str=m_db.getString("ProjectName");
			m_projdetail.SetItemText(i,1,str);
			//str=m_db.getString("ProjectNo");
			str=m_db.getString("ReqID");
			m_projdetail.SetItemText(i,2,str);
			str=m_db.getString("CustomerName");
			m_projdetail.SetItemText(i,3,str);
			str=m_db.getString("ProjectState");
			m_projdetail.SetItemText(i,4,str);
			//str.Format("%d",m_db.getInt("No"));
			//m_projdetail.SetItemData(i,atoi(str));
			i++;
		} while(m_db.MoveNext());
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ������Ŀ�б����ʾ�����Ŀ������                                     */
/************************************************************************/
void CProjectDlg::OnNMClickListProdetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(FALSE);
	BOOL IsSelect=FALSE;
	m_task.DeleteAllItems();
	for (int i=0;i<m_projdetail.GetItemCount();i++)
	{
		if (m_projdetail.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsSelect=TRUE;
			ProjectID=m_projdetail.GetItemText(i,2);           //�õ���Ŀ���
			break;
		}
	}
	if (!IsSelect)
	{
		return;
	}
	if (!GetTask(ProjectID))
	{
		AfxMessageBox("��������ʧ��");
		return;
	}
	*pResult = 0;
}

void CProjectDlg::OnNMClickListTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	UpdateData(FALSE);
	BOOL IsSelect=FALSE;
	CString sql,str;
	for (int i=0;i<m_task.GetItemCount();i++)
	{
		if (m_task.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsSelect=TRUE;
			TaskID=(int)m_task.GetItemData(i);           //�õ���Ʒ���
			break;
		}
	}
	if (!IsSelect)
	{
		return;
	}
	else
	{
		sql.Format("SELECT * FROM rl_Task where ProjectID='%s' AND ID=%d",ProjectID,TaskID);
		int Z=m_db.Query(sql);
		if (Z==-1)
		{
			return;
		}
		else
		{
			m_db.MoveBegin();
			do {
				m_note=m_db.getString("Description");         //�õ����������
				ElementID=m_db.getInt("ElementID");           //Ԫ��ID
			} while(m_db.MoveNext());
		}
		UpdateData(FALSE);
	}
	*pResult = 0;
}
/************************************************************************/
/* ���OK��ť��ʼ������                                               */
/************************************************************************/
void CProjectDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sql,str;
	info=new DetailInfoClass;
	CFileFind ff;
	strLocalDirectoryPath="C:\\proe_temp";
	CString Saveplace;
	BOOL a=ff.FindFile(strLocalDirectoryPath);
	if(!a)
	{
		if (strLocalDirectoryPath.Right(1)!=":")
		{
			if(::CreateDirectory(strLocalDirectoryPath,NULL)==0)
			{
				delete info;
				return;
			}
		}

	}
	//��ѯ��Ŀ��ɱ�
	sql.Format("SELECT * FROM rl_ProjectCompose where ProjectID='%s' AND ElementID=%d",ProjectID,ElementID);
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		delete info;
		return;
	}
	else
	{
		m_db.MoveBegin();
		do {
			info->SolidID=-1;
			info->TaskType=m_db.getString("TaskType");          //�������ͣ�A��B��C��D��
			info->OldInsCode=m_db.getString("InsCode");         //��ͼ��
			info->InsCode=m_db.getString("Tuhao");              //��ͼ��
			str=m_db.getString("PartType");                     //�������װ���
            if(str=="���")
				info->Type="prt";
			else
				info->Type="asm";
			info->InsName=m_db.getString("Name");               //��Ʒ�������
			info->MdlNum.Format("%d",m_db.getInt("ModelID"));   //ģ�ͺ�
			//info->SolidFilePath=m_db.getString("PlaceFolder");
			//info->DrwFilePath=info->SolidFilePath;
			//info->SolidFileName=m_db.getString("FileName");
		} while(m_db.MoveNext());
	}
	OnOK();
	if (info->TaskType=="A")
	{
		AfxMessageBox("�������������");
	}
	else
		if (info->TaskType=="B")               //����ԭ��ģ�����
		{
			//������ģ��
			if (!OpenMdl(info))
			{
				AfxMessageBox("��ģ��ʧ��");
				delete info;
				return;
			}
		}
		else
			if (info->TaskType=="C")           //�����������
			{
				//������������߲���
				if (!OpenExistPart(info))
				{
					AfxMessageBox("���ļ�ʧ��");
					delete info;
					return;
				}
			}
			else
				if (info->TaskType=="D")        //�½����
				{
					//�½����
					if (!CreateExistPart(info))
					{
						AfxMessageBox("�½��ļ�����");
						delete info;
						return;
					}
				}
				else
				{
					AfxMessageBox("��������Ǵ����");
					delete info;
					return;
				}
				//info->SolidFilePath=m_db.getString("PlaceFolder");                  //���±���λ��
				//info->DrwFilePath=info->SolidFilePath;
	OnOK();
}
/************************************************************************/
/*  �õ�ģ�͵���ϸ��Ϣ,��ģ��                                         */
/************************************************************************/
BOOL CProjectDlg::OpenMdl(DetailInfoClass *info)
{
	int s=-1;
	BOOL IsLocal;
	CPrtClass prtclass;
	CProductClass pdtclass;
    if (info->Type=="prt")
    {
		if(!GetPartModelInfo(info->MdlNum))
		{
			AfxMessageBox("�õ�ģ����Ϣ����");
			return FALSE;
		}
		int k=info->OldSolidFilePath.Find(":");
		if (k==-1)
		{
			IsLocal=FALSE;
		}
		else
		{
			IsLocal=TRUE;
		}
		//����ģ��,��ģ��
		if(!prtclass.OpenPartFile(info,&s,IsLocal))
		{
			return FALSE;
		}
		else
		{
			info->IsInsert="T";
			prtclass.ActivatePart(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
		}
    }
	else
		if (info->Type=="asm")
		{
            if (!GetAsmModelInfo(info->MdlNum))
            {
				AfxMessageBox("�õ�ģ����Ϣ����");
				return FALSE;
            }
			info->DrwFilePath=strLocalDirectoryPath;           //��Ҫ���صı���Ŀ¼
			info->SolidFilePath=strLocalDirectoryPath;
			if (!pdtclass.NewProductFile(info,&s,TRUE))
			{
				return FALSE;
			}
			else
			{
				info->IsInsert="T";
				if (!pdtclass.ActivateAsm(info->solid))
				{
					AfxMessageBox("�򿪴���ʧ��");
					return FALSE;
				}
			}
		}
		return TRUE;
}
/************************************************************************/
/* ����������Ͳ���                                                   */
/************************************************************************/
BOOL CProjectDlg::OpenExistPart(DetailInfoClass *info)
{
	int s=-1;
	BOOL IsLocal;
	CString sql,str;
	CPrtClass prtclass;
	CProductClass pdtclass;
	if (info->Type=="prt")
	{   
		sql.Format("SELECT * FROM NPartInsInfoTable WHERE InsCode='%s'",info->OldInsCode);
	}
	else
        sql.Format("SELECT * FROM NProductInsInfoTable WHERE InsCode='%s'",info->OldInsCode);
	int Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		return FALSE;
	}
    else
	{
		m_db.MoveBegin();
		do {
			if (info->Type=="prt")
			{
				info->OldSolidID=m_db.getInt("PrtNum");
				info->GroupNum=m_db.getInt("TypeNum");
			}
			else
			{
				info->OldSolidID=m_db.getInt("PdtNum");
				info->GroupNum=m_db.getInt("TypeNum");
			}
			break;
		} while(m_db.MoveNext());
	}
	if (info->Type=="prt")
	{
		str.Format("%d",info->OldSolidID);
		if(!GetPartInfo(str))
		{
			AfxMessageBox("�õ������Ϣ����");
			return FALSE;
		}
		int k=info->OldSolidFilePath.Find(":");
		if (k==-1)
		{
			IsLocal=FALSE;
		}
		else
		{
			IsLocal=TRUE;
		}
		//����ģ��,��ģ��
		if(!prtclass.OpenPartFile(info,&s,IsLocal))
		{
			return FALSE;
		}
		else
		{
			info->IsInsert="T";
			prtclass.ActivatePart(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
		}
	}
	else
		if (info->Type=="asm")
		{
			info->DrwFilePath=strLocalDirectoryPath;                    //��Ҫ���صı���Ŀ¼
			info->SolidFilePath=strLocalDirectoryPath;                  //��Ҫ���صı���Ŀ¼
			info->SolidFileName=info->InsCode+".asm";
            info->DrwName=info->InsCode+".drw";
            //�õ���Ʒ����
			sql.Format("SELECT * FROM NAsmType where Number=%d",info->GroupNum);
			Z=m_db.Query(sql);
			if (Z==-1 || Z==0)
			{
				return FALSE;
			}
			else
			{
				m_db.MoveBegin();
				do {
					info->GrpFrm=m_db.getString("Name");
				} while(m_db.MoveNext());
			}
			if (!pdtclass.NewProductFile(info,&s,FALSE))
			{
				return FALSE;
			}
			else
				if (!pdtclass.ActivateAsm(info->solid))
				{
					AfxMessageBox("�򿪴���ʧ��");
					return FALSE;
				}
		}
    return TRUE;
}
/************************************************************************/
/* ���½����                                                         */
/************************************************************************/
BOOL CProjectDlg::CreateExistPart(DetailInfoClass *info)
{
    ProError status;
	ProName  w_name;
	ProSolid solid;
	int w_id;
	ProStringToWstring(w_name,info->InsCode.GetBuffer());
	if (info->Type=="prt")
	{
        status=ProSolidCreate(w_name,PRO_PART,&solid);
	}
	else
		status=ProSolidCreate(w_name,PRO_ASSEMBLY,&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	status=ProMdlDisplay((ProMdl)solid);
	status=ProWindowCurrentGet(&w_id);
	status = ProWindowActivate(w_id);
	p_classarray.Add(info);
    return TRUE;
}
/************************************************************************/
/* �õ����ģ�͵���ϸ��Ϣ                                               */
/************************************************************************/
BOOL CProjectDlg::GetPartModelInfo(CString MdlNum)
{
	CString sql,str;
	sql.Format("SELECT * FROM NPartManageTable WHERE Number=%s",MdlNum);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			info->MdlName=m_db.getString("Name");
			info->GroupNum=m_db.getInt("TypeNum");
			info->OldSolidFileName=m_db.getString("SolidFileName");
			info->OldSolidFilePath=m_db.getString("SolidFilePlace");
			info->SolidFileName=info->InsCode+".prt";
			info->DrwName=info->InsCode+".drw";
			info->OldDrwName=m_db.getString("DrawingFileName");
			info->OldDrwFilePath=m_db.getString("DrawingFilePlace");
			info->Material==m_db.getString("Material");
			info->DrwFilePath=strLocalDirectoryPath;
			info->SolidFilePath=strLocalDirectoryPath;
			break;
		} while(m_db.MoveNext());
	}		
	sql.Format("SELECT * FROM NPartType where Number=%d",info->GroupNum);
	Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			info->GrpFrm=m_db.getString("Name");
		} while(m_db.MoveNext());
	}
	info->DesignState="δ���";
	info->AuditState="���δͨ��";
	info->IsLocalOrServer="����";
	info->PDMState="δ�ύ";
	//�õ�ģ�͵����ܲ���
	//�õ����ܲ�����ֵ
	sql.Format("SELECT * FROM NPartPerfparaTable WHERE PrtNum=%s",MdlNum);
	Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		info->PerformanceValue.RemoveAll();
	}
	else
	{
		m_db.MoveBegin();
		do {
			str=m_db.getString("Value");
			info->PerformanceValue.Add(str);
		} while(m_db.MoveNext());
	}
	UpdateData(FALSE);
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CProjectDlg::GetAsmModelInfo(CString MdlNum)
{
	CString sql,str;
	sql.Format("SELECT * FROM NAsmManageTable WHERE Number=%s",MdlNum);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			info->MdlName=m_db.getString("Name");
			info->GroupNum=m_db.getInt("TypeNum");
			info->OldSolidFileName=m_db.getString("SolidFileName");
			info->OldSolidFilePath=m_db.getString("SolidFilePlace");
			info->SolidFileName=info->InsCode+".asm";
			info->DrwName=info->InsCode+".drw";
			info->OldDrwName=m_db.getString("DrawingFileName");
			info->OldDrwFilePath=m_db.getString("DrawingFilePlace");
			info->DrwFilePath=strLocalDirectoryPath;
			info->SolidFilePath=strLocalDirectoryPath;
			break;
		} while(m_db.MoveNext());
	}		
	sql.Format("SELECT * FROM NAsmType where Number=%d",info->GroupNum);
	Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			info->GrpFrm=m_db.getString("Name");
		} while(m_db.MoveNext());
	}
	info->DesignState="δ���";
	info->AuditState="���δͨ��";
	info->IsLocalOrServer="����";
	info->PDMState="δ�ύ";
	//�õ�ģ�͵����ܲ���
	//�õ����ܲ�����ֵ
	sql.Format("SELECT * FROM NAsmPerformanceTable WHERE AsmNum=%s",MdlNum);
	Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		info->PerformanceValue.RemoveAll();
	}
	else
	{
		m_db.MoveBegin();
		do {
			str=m_db.getString("Value");
			info->PerformanceValue.Add(str);
		} while(m_db.MoveNext());
	}
	UpdateData(FALSE);
	return TRUE;
}
/************************************************************************/
/* �õ��������ϸ��Ϣ                                                   */
/************************************************************************/
BOOL CProjectDlg::GetPartInfo(CString InsCode)            //�������
{
	CString sql,str;
	CPrtClass prtclass;
	BOOL Find;
	sql.Format("select * from NPartInsInfoTable WHERE PrtNum=%s",InsCode);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			info->OldInsCode=m_db.getString("InsCode");
			info->PDMCode=m_db.getString("PDMNum");
			info->PDMState=m_db.getString("PDMState");
			info->OldSolidID=m_db.getInt("PrtNum");
			info->GroupNum=m_db.getInt("TypeNum");
		    info->MdlNum.Format("%d",m_db.getInt("MdlNum"));
			info->OldSolidFileName=m_db.getString("SolidFileName");
			info->OldSolidFilePath=m_db.getString("SolidPlace");
			info->OldDrwName=m_db.getString("DrawingName");;
			info->OldDrwFilePath=m_db.getString("DrawingPlace");
			info->DrwFilePath=strLocalDirectoryPath;
			info->SolidFilePath=strLocalDirectoryPath;
			info->SolidFileName=info->InsCode+".prt";
			info->DrwName=info->InsCode+".drw";
			break;
		} while(m_db.MoveNext());
	}
	//�õ����
	sql.Format("SELECT * FROM NPartType where Number=%d",info->GroupNum);
	Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			info->GrpFrm=m_db.getString("Name");
		} while(m_db.MoveNext());
	}
	//�õ�ģ����
	sql.Format("SELECT * FROM NPartManageTable WHERE Number=%s",info->MdlNum);
	Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			info->MdlName=m_db.getString("Name");
			break;
		} while(m_db.MoveNext());
	}
	//	�õ�PDM�ı���λ�ú��ļ���
	if (info->PDMState=="�ύ")
	{
		if (!prtclass.GetDocumentsFromPDM(Ftp_struc.Database,info,&Find))    //��ε��ã�����������û����
		{
			return FALSE;
		}
	}
	info->DesignState="δ���";
	info->AuditState="���δͨ��";
	info->IsLocalOrServer="����";
	info->PDMState="δ�ύ";
	//�������ܲ���
	sql.Format("SELECT * FROM NPartInsPerfTable WHERE PrtNum=%d",info->OldSolidID);
	Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		info->PerformanceValue.RemoveAll();
	}
	else
	{
		m_db.MoveBegin();
		do {
			str=m_db.getString("Value");
			info->PerformanceValue.Add(str);
		} while(m_db.MoveNext());
	}
	return TRUE;
}
void CProjectDlg::OnBnClickedButtonFinish()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sql;
	for (int i=0;i<m_task.GetItemCount();i++)
	{
		if (m_task.GetCheck(i)==TRUE)
		{
			TaskID=(int)m_task.GetItemData(i);
			sql.Format("SELECT * FROM rl_Task where ProjectID='%s' AND ID=%d",ProjectID,TaskID);
			int Z=m_db.Query(sql);
			if (Z==-1)
			{
				return;
			}
			else
			{
				m_db.MoveBegin();
				do {
					ElementID=m_db.getInt("ElementID");
				} while(m_db.MoveNext());
			}
			sql.Format("Update rl_Task Set State='���' where ProjectID='%s' and ID=%d",ProjectID,TaskID);
			try
			{
				m_db.m_pConnection->Close();
				m_db.ReOpen();
				m_db.m_pConnection->BeginTrans();
				if(m_db.Execute(sql)==0)
				{
					m_db.m_pConnection->RollbackTrans();
					return;
				}
				else
				{
					sql.Format("Update rl_ProjectCompose Set State='���' where ProjectID='%s' and ElementID=%d",ProjectID,ElementID);
					if(m_db.Execute(sql)==0)
					{
						m_db.m_pConnection->RollbackTrans();
						return;
					}
					m_db.m_pConnection->CommitTrans();
				}
			}
            catch (...)
			{
				m_db.m_pConnection->RollbackTrans();
            }
		}
	}
	m_task.DeleteAllItems();
	if (!GetTask(ProjectID))
	{
		AfxMessageBox("��������ʧ��");
		return;
	}
}
/************************************************************************/
/* ����Ҫ��õ���ѡ��Ŀ������                                               */
/************************************************************************/
BOOL CProjectDlg::GetTask(CString ProjectID)        // ��Ŀ��ţ�������
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	BOOL IsSelect=FALSE;
	CString sql,str;
	if (m_all)       //��ʾ��ǰѡ����Ŀ����������
		sql.Format("SELECT * FROM rl_Task where ProjectID='%s'",ProjectID);
	else
		sql.Format("SELECT * FROM rl_Task where ProjectID='%s' and TaskUser='%s'",ProjectID,UserName);
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	else
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
				m_task.InsertItem(i,str);
				str=m_db.getString("Name");
				m_task.SetItemText(i,1,str);
				str=m_db.getString("TaskUser");
				m_task.SetItemText(i,2,str);
				str=m_db.getString("StartDate");
                m_task.SetItemText(i,3,str);
				str=m_db.getString("EndDate");
				m_task.SetItemText(i,4,str);
				str=m_db.getString("State");
				m_task.SetItemText(i,5,str);
				str.Format("%d",m_db.getInt("ID"));
				m_task.SetItemData(i,atoi(str));
			i++;
		} while(m_db.MoveNext());
	}
	return TRUE;
}
//�ܾ���ť
void CProjectDlg::OnBnClickedButtonReject()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sql;
	for (int i=0;i<m_task.GetItemCount();i++)
	{
		if (m_task.GetCheck(i)==TRUE)
		{
			TaskID=(int)m_task.GetItemData(i);
			sql.Format("SELECT * FROM rl_Task where ProjectID='%s' AND ID=%d",ProjectID,TaskID);
			int Z=m_db.Query(sql);
			if (Z==-1)
			{
				return;
			}
			else
			{
				m_db.MoveBegin();
				do {
					ElementID=m_db.getInt("ElementID");
				} while(m_db.MoveNext());
			}
			sql.Format("Update rl_Task Set State='�ܾ�' where ProjectID='%s' and ID=%d",ProjectID,TaskID);
			try
			{
				m_db.m_pConnection->Close();
				m_db.ReOpen();
				m_db.m_pConnection->BeginTrans();
				if(m_db.Execute(sql)==0)
				{
					m_db.m_pConnection->RollbackTrans();
					return;
				}
				else
				{
					sql.Format("Update rl_ProjectCompose Set State='�ܾ�' where ProjectID='%s' and ElementID=%d",ProjectID,ElementID);
					if(m_db.Execute(sql)==0)
					{
						m_db.m_pConnection->RollbackTrans();
						return;
					}
					m_db.m_pConnection->CommitTrans();
				}
			}
			catch (...)
			{
				m_db.m_pConnection->RollbackTrans();
			}
		}
	}
	m_task.DeleteAllItems();
	if (!GetTask(ProjectID))
	{
		AfxMessageBox("��������ʧ��");
		return;
	}
}
//����
void CProjectDlg::OnBnClickedButtonAccept()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sql;
	for (int i=0;i<m_task.GetItemCount();i++)
	{
		if (m_task.GetCheck(i)==TRUE)
		{
			TaskID=(int)m_task.GetItemData(i);
			sql.Format("SELECT * FROM rl_Task where ProjectID='%s' AND ID=%d",ProjectID,TaskID);
			int Z=m_db.Query(sql);
			if (Z==-1)
			{
				return;
			}
			else
			{
				m_db.MoveBegin();
				do {
					ElementID=m_db.getInt("ElementID");
				} while(m_db.MoveNext());
			}
			sql.Format("Update rl_Task Set State='����' where ProjectID='%s' and ID=%d",ProjectID,TaskID);
			try
			{
				m_db.m_pConnection->Close();
				m_db.ReOpen();
				m_db.m_pConnection->BeginTrans();
				if(m_db.Execute(sql)==0)
				{
					m_db.m_pConnection->RollbackTrans();
					return;
				}
				else
				{
					sql.Format("Update rl_ProjectCompose Set State='����' where ProjectID='%s' and ElementID=%d",ProjectID,ElementID);
					if(m_db.Execute(sql)==0)
					{
						m_db.m_pConnection->RollbackTrans();
						return;
					}
					m_db.m_pConnection->CommitTrans();
				}
			}
			catch (...)
			{
				m_db.m_pConnection->RollbackTrans();
			}
		}
	}
	m_task.DeleteAllItems();
	if (!GetTask(ProjectID))
	{
		AfxMessageBox("��������ʧ��");
		return;
	}
}
