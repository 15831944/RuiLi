// UpdateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "UpdateDlg.h"
#include "MyDatabase.h"
#include "DetailInfoClass.h"
#include "CommonFuncClass.h"

extern CMyDatabase m_db;
extern CPtrArray p_classarray;



// CUpdateDlg �Ի���

IMPLEMENT_DYNAMIC(CUpdateDlg, CDialog)
CUpdateDlg::CUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateDlg::IDD, pParent)
	, m_inscode(_T(""))
	, m_insname(_T(""))
	, m_mdlnum(_T(""))
	, m_designer(_T(""))
	, m_note(_T(""))
	, m_group(_T(""))
	//, m_material(_T(""))
	//, m_unit(_T(""))
	, m_place(_T(""))
	, m_filename(_T(""))
	, m_drwingplace(_T(""))
	, m_drwname(_T(""))
	, m_auditor(_T(""))
{
}

CUpdateDlg::~CUpdateDlg()
{
}

void CUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WORKSPACE, m_select);
	DDX_Text(pDX, IDC_EDIT_CODE, m_inscode);
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_insname);
	DDX_Text(pDX, IDC_EDIT_MDLFRM, m_mdlnum);
	DDX_Text(pDX, IDC_DESIGNER, m_designer);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	DDX_Text(pDX, IDC_EDIT_GRPFRM, m_group);
	//DDX_Text(pDX, IDC_EDIT_MATERIAL, m_material);
	//DDX_Text(pDX, IDC_EDIT_UNIT, m_unit);
	DDX_Text(pDX, IDC_EDIT_PLACE, m_place);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
	DDX_Text(pDX, IDC_EDIT_DRWPLACE, m_drwingplace);
	DDX_Text(pDX, IDC_EDIT_DRWNAME, m_drwname);
	DDX_Text(pDX, IDC_EDIT_AUDITOR, m_auditor);
}


BEGIN_MESSAGE_MAP(CUpdateDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_NO, OnBnClickedButtonNo)
END_MESSAGE_MAP()


// CUpdateDlg ��Ϣ�������

BOOL CUpdateDlg::OnInitDialog()
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
	CString sql;
	if (s==-1)
	{
		return FALSE;
	}
	else
	{
		if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum=="-1")
		{
			m_mdlnum=="��";
		}
		else
		    m_mdlnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlName;
		m_inscode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
		m_insname=((DetailInfoClass *)p_classarray.GetAt(s))->InsName;
		m_designer=((DetailInfoClass *)p_classarray.GetAt(s))->Designer;
		m_group=((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm;
		//m_material=((DetailInfoClass *)p_classarray.GetAt(s))->Material;
		//m_unit=((DetailInfoClass *)p_classarray.GetAt(s))->Unit;
		m_note=((DetailInfoClass *)p_classarray.GetAt(s))->Note;
		m_filename=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
		m_drwname=((DetailInfoClass *)p_classarray.GetAt(s))->DrwName;
		m_drwingplace=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
		m_place=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum!="-1")
	{
		//�������ܲ���
		m_select.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
		m_select.InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
		m_select.InsertColumn(1,"���ܲ�����",LVCFMT_LEFT,80,-1);
		m_select.InsertColumn(2,"���ܲ���",LVCFMT_LEFT,100,-1);
		m_select.InsertColumn(3,"��ע",LVCFMT_LEFT,100,-1);
		//�����ݿ⣬��ʾ��ģ�͵����ܲ���
		int i=1;
		//if (Type=="prt")
		//{
		//	sql.Format("select * from NPartPerfparaTable WHERE PrtNum=%s",((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum);
		//}
		//else
		//	if (Type=="asm")
		//{
		sql.Format("select * from NAsmPerformanceTable WHERE AsmNum=%s",((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum);
		//}
		int Z=m_db.Query(sql);
		if (Z==-1)
		{
			return FALSE;
		}
		else
			if (m_db.Query(sql)!=0)
			{
				m_db.MoveBegin();
				do {
					str.Format("%d",i);
					m_select.InsertItem(i-1,str);
					str=m_db.getString("Name");
					m_select.SetItemText(i-1,1,str);
					str=m_db.getString("Note");
					m_select.SetItemText(i-1,3,str);
					i++;
				} while(m_db.MoveNext());
			}
			for(int i=0;i<(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).GetSize();i++)
			{
				m_select.SetItemText(i,2,(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue)[i]);
			}
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CUpdateDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_auditor=="")
	{
		AfxMessageBox("����д����˵�����");
		return;
	}
	//CTime time=CTime::GetCurrentTime();
	//((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime=time.Format("%Y-%m-%d %H:%M:%S");
	//((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="���ͨ��";
	((DetailInfoClass *)p_classarray.GetAt(s))->Auditor=m_auditor;

	//m_db.m_pConnection->Close();
	//m_db.ReOpen();
	//m_db.m_pConnection->BeginTrans();
	//try
	//{
	//	if (!AuditFunc(s))
	//	{
	//		m_db.m_pConnection->RollbackTrans();
	//		AfxMessageBox("�ύʧ��");
	//	}//Update���ݿ�
	//	else
	//	{
	//		m_db.m_pConnection->CommitTrans();
	//		AfxMessageBox("�ύ�ɹ���");
	//	}

	//}
	//catch (_com_error e) 
	//{
	//	m_db.m_pConnection->RollbackTrans();
	//	AfxMessageBox("�ύʧ�ܣ�");
	//}
	OnOK();
}

void CUpdateDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
void CUpdateDlg::OnBnClickedButtonNo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
	//UpdateData(TRUE);
	//if (m_auditor=="")
	//{
	//	AfxMessageBox("����д����˵�����");
	//	return;
	//}
	//CTime time=CTime::GetCurrentTime();
	////((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime=time.Format("%Y-%m-%d %H:%M:%S");
	////((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="���δͨ��";
	//((DetailInfoClass *)p_classarray.GetAt(s))->Auditor=m_auditor;
	//m_db.m_pConnection->Close();
	//m_db.ReOpen();
	//m_db.m_pConnection->BeginTrans();
	//try
	//{
	//	if (!AuditFunc(s))
	//	{
	//		m_db.m_pConnection->RollbackTrans();
	//		AfxMessageBox("�ύʧ��");
	//	}//Update���ݿ�
	//	else
	//	{
	//		m_db.m_pConnection->CommitTrans();
	//		AfxMessageBox("�ύ�ɹ���");
	//	}

	//}
	//catch (_com_error e) 
	//{
	//	m_db.m_pConnection->RollbackTrans();
	//	AfxMessageBox("�ύʧ�ܣ�");
	//}
}
