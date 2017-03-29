// AllFemMdlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ansys.h"
#include "AllFemMdlDlg.h"
#include "myproe.h"
#include "FemOpenDlg.h" 


// CAllFemMdlDlg �Ի���

IMPLEMENT_DYNAMIC(CAllFemMdlDlg, CDialog)
CAllFemMdlDlg::CAllFemMdlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAllFemMdlDlg::IDD, pParent)
	, m_Path(_T(""))
	, m_Number(_T(""))
{
}

CAllFemMdlDlg::~CAllFemMdlDlg()
{
}

void CAllFemMdlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FEM_MDL, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_Path);
	DDX_Text(pDX, IDC_EDIT2, m_Number);
}


BEGIN_MESSAGE_MAP(CAllFemMdlDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnBnClickedButtonSelect)
	//ON_BN_CLICKED(IDC_BUTTON_Refresh, OnBnClickedButtonRefresh)
END_MESSAGE_MAP()


// CAllFemMdlDlg ��Ϣ�������

BOOL CAllFemMdlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	int status;
	status=m_db.Open(globl_connectString);

	m_List.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,"����",LVCFMT_LEFT,100,-1);
	//m_List.InsertColumn(1,"���",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(1,"��ģʱ��",LVCFMT_LEFT,100,-1);
	/*m_List.InsertColumn(2,"����ģ�ͺ�",LVCFMT_LEFT,100,-1);*/
	m_List.InsertColumn(2,"����·��",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(3,"��ע",LVCFMT_LEFT,200,-1);
	this->GetMdlNum();

	this->AddToList();
	//this->OnBnClickedButtonRefresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}


void CAllFemMdlDlg::AddToList()
{
	m_List.DeleteAllItems();
	CString str,strSql;
	int rows;
	int number=m_MdlNum;
    strSql.Format("select * from FemPrt where ConnPrtMdlNum=%d",number);
	m_db.Query(strSql);
	rows=m_db.getRows();
	m_List.DeleteAllItems();
	if(rows==0) 
	{
		//AfxMessageBox("�Բ���,�����ģ��,���Ƚ�������Ԫģ��");
		//this->SendMessage(WM_CLOSE,0,0);
		return;
	}
	m_db.MoveBegin();
	for(int i=0;i<rows;i++)
	{
		str=m_db.getString("Name");
		m_List.InsertItem(i,str);
		
        str.Format("%d",number);
		m_List.SetItemText(i,1,str);
		str=m_db.getDate("DateTime");
		m_List.SetItemText(i,1,str);
		//str.Format("%d",m_db.getInt("ConnPrtMdlNum"));
		//m_List.SetItemText(i,2,str);
		str=m_db.getString("FileDirectory");
		m_List.SetItemText(i,2,str);
		str=m_db.getString("Note");
		m_List.SetItemText(i,3,str);
		
		number=m_db.getInt("Number");
		m_List.SetItemData(i,(DWORD)number);
		m_db.MoveNext();
	}

}

void CAllFemMdlDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString fullpath;
	CString temp="C:\\proe_temp";
	::CreateDirectory(temp,NULL);
    int index=0;
	for (int i=0;i<m_List.GetItemCount();i++)
	{
		if ( m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			fullpath=m_List.GetItemText(i,2);
            index=fullpath.ReverseFind('/');
			CString name=fullpath.Mid(index+1);
			m_Path=temp+"\\"+name;
			ftpfile.DownloadFiletoFile(fullpath,m_Path,myFtp);
		}
	}
	CString str=m_Path;
	ProSolid solid;
	CFemOpenDlg dlg;
	int status;
	status=dlg.LoadModel(str,(ProMdl *)&solid);
	if(status==0) return;
	util2.Load(solid);
	for(int i=0;i<util1.dimArray.GetSize();i++)
	{
	   util2.SetValue(util1.dimArray[i].Llabel,util1.dimArray[i].value);
	}
	ProSolidRegenerate(solid,PRO_B_TRUE);
	dlg.ShowNewWindow((ProMdl *)&solid);
	OnOK();
}

void CAllFemMdlDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_db.Close();

	CDialog::OnClose();
}

void CAllFemMdlDlg::OnBnClickedButtonSelect()
{
	UpdateData(TRUE);
	char szFilters[]=
		"prt Files (*.prt)|*.prt.*|All Files (*.*)|*.*||";

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg (FALSE, "prt", NULL,
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if( fileDlg.DoModal ()==IDOK )
	{
		m_Path=fileDlg.GetPathName();
	}
	UpdateData(FALSE);
}

int CAllFemMdlDlg::GetMdlNum()
{
	ProError status;
	status=ProMdlCurrentGet((ProMdl*)&rootSolid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�޷��õ���ǰģ��!");
		return 0;
	}
	ProMdlToModelitem(rootSolid,&owner);
	ProName name;  
	ProParamvalue value; 
	ProParameter  param;
	CString strName="RL_MDL";
    ProStringToWstring(name,strName.GetBuffer());
	status = ProParameterInit (&owner,name,&param);
	status=ProParameterValueGet(&param,&value);
	m_MdlNum=value.value.i_val;
	util1.Load(rootSolid);
	return m_MdlNum;
}

void CAllFemMdlDlg::OnBnClickedButtonRefresh()
{
   UpdateData(TRUE);
   m_MdlNum=atoi(m_Number);
   this->AddToList();
   UpdateData(FALSE);
}
