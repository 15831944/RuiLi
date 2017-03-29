// MdlSeriesPicShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "MdlSeriesPicShowDlg.h"
#include "PicViewDlg.h"
#include "FtpFile.h"
//extern FtpConnectInfo myFtp;
//extern int DlgIcon;
// CMdlSeriesPicShowDlg 对话框

IMPLEMENT_DYNAMIC(CMdlSeriesPicShowDlg, CDialog)
CMdlSeriesPicShowDlg::CMdlSeriesPicShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMdlSeriesPicShowDlg::IDD, pParent)
	, m_pSet(NULL)
	, m_path(_T(""))
	, mdlNum(0)
{
}

CMdlSeriesPicShowDlg::~CMdlSeriesPicShowDlg()
{
}

void CMdlSeriesPicShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CMdlSeriesPicShowDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CMdlSeriesPicShowDlg 消息处理程序

void CMdlSeriesPicShowDlg::SetMdlNumber(long mdlNumber, CDatabase * p_db)
{
	
	m_pSet=new CPartMdlRecordSet(p_db);
	mdlNum=mdlNumber;
}

BOOL CMdlSeriesPicShowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	if (m_pSet==NULL) {
		return FALSE;
	}
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"文件名",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(1,"说明",LVCFMT_LEFT,120,-1);
	ShowList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CMdlSeriesPicShowDlg::ShowList(void)
{
	int      index=0;
	CFtpFile m_ftp;
	CpartPicSet m_picset(m_pSet->m_pDatabase);

	try
	{
		m_pSet->m_strFilter.Format("[Number]=%d",mdlNum);
		m_pSet->Open();
		if (m_pSet->IsBOF()) {
			m_pSet->Close();
			delete m_pSet;
			return;
		}
		
		m_pSet->Close();
        //TempDirectory		
		DWORD dwResult=GetTempPath (MAX_PATH, szTempPath);
		ASSERT (dwResult);		
		m_picset.m_strFilter.Format("[PrtNum]=%d",mdlNum);
		m_picset.Open();
		if (m_picset.IsBOF()) {
			delete m_pSet;
			return;
		}
		if(!m_ftp.DownloadDirectorytoDirectory2(_T(m_pSet->m_FileDirectory+"/"+"picture/"),_T(CString(szTempPath).TrimRight(_T("\\"))),myFtp))
		{
			AfxMessageBox("下载失败");
		}
		m_picset.MoveFirst();
		while (!m_picset.IsEOF()) {
			m_List.InsertItem(index,m_picset.m_Name);
			m_List.SetItemText(index,1,m_picset.m_Note);
			index++;
			m_picset.MoveNext();
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);

	}
	delete m_pSet;
}

void CMdlSeriesPicShowDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CPicViewDlg dlg;
	int num=0;
	CString file;
	num=(int)m_List.GetItemCount();

	for(int i=0;i<num;i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;
		
			dlg.SetFile(CString(szTempPath)+"picture\\"+m_List.GetItemText(i,0));		
			dlg.DoModal();
			break;
		
	}
}

void CMdlSeriesPicShowDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}
