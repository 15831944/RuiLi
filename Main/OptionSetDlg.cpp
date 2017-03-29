// OptionSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "OptionSetDlg.h"
#include "FtpFile.h"
//extern FtpConnectInfo myFtp;
//extern CString        dbLinkInfo;
// COptionSetDlg �Ի���

IMPLEMENT_DYNAMIC(COptionSetDlg, CDialog)
COptionSetDlg::COptionSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionSetDlg::IDD, pParent)
	, m_port(0)
	,m_DSN(_T(""))
	,m_DBUsrName(_T(""))
	,m_DBPwd(_T(""))
	, m_PdmDsn(_T(""))
	, m_PdmDsnUser(_T(""))
	, m_PdmDsnPwd(_T(""))

{
}

COptionSetDlg::~COptionSetDlg()
{
	
}

void COptionSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IPADDRESS1, myFtp.strFtp);
	DDX_Text(pDX, IDC_EDIT1, myFtp.strFtpUser);
	DDX_Text(pDX, IDC_EDIT2, myFtp.strFtpPwd);
	DDX_Text(pDX, IDC_EDIT4, m_DSN);
	DDX_Text(pDX, IDC_EDIT5, m_DBUsrName);
	DDX_Text(pDX, IDC_EDIT6, m_DBPwd);
	DDX_Text(pDX, IDC_EDIT3, myFtp.iFtpPort);
	DDX_Text(pDX, IDC_EDIT11, m_PdmDsn);
	DDX_Text(pDX, IDC_EDIT12, m_PdmDsnUser);
	DDX_Text(pDX, IDC_EDIT7, m_PdmDsnPwd);
	
}


BEGIN_MESSAGE_MAP(COptionSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON10, OnBnClickedButton10)
	ON_WM_MOVING()
	ON_WM_MOVE()
	
END_MESSAGE_MAP()


// COptionSetDlg ��Ϣ�������

void COptionSetDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CFtpFile m_ftp;
	if(m_ftp.TestConnect(myFtp)) AfxMessageBox("�ɹ�");
	else AfxMessageBox("ʧ��");
}

void COptionSetDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	HKEY      hKey;
	struct    HKEY__*ReRootKey;
	TCHAR     *ReSubKey;
	TCHAR     *ReValueName_address,*ReValueName_user,*ReValueName_pwd,*ReValueName_port;
	TCHAR     *ReValueName_DB_NAME,*ReValueName_DB_USER,*ReValueName_DB_PWD;
	TCHAR     *ReValueName_SHUJU;
	//PDM
	TCHAR     *RePdmValueName_DB_NAME,*RePdmValueName_DB_USER,*RePdmValueName_DB_PWD;
	char		content_add[256],content_user[256],content_pwd[256];
	char        content_shujuyuan[256];
	char		 content_dsn_user[256],content_dsn_pwd[256],content_dsn_name[256];
	char		 content_pdm_dsn_user[256],content_pdm_dsn_pwd[256],content_pdm_dsn_name[256];
	int			content_port[256];
	ReRootKey=HKEY_CURRENT_USER;              //ע�����������
	ReSubKey="Software\\PTC\\VariantDesign";  //����ע���ֵ�ĵ�ַ
	ReValueName_address="ftp_address";               //������ֵ������
	ReValueName_user="ftp_user";
	ReValueName_pwd="ftp_pwd";
	ReValueName_port="ftp_port";
	ReValueName_DB_USER="dsn_user";
	ReValueName_DB_PWD="dsn_pwd";
	ReValueName_DB_NAME="dsn";
	ReValueName_SHUJU="SHUJUYUAN";
	RePdmValueName_DB_NAME="pdm_dsn_name";
	RePdmValueName_DB_USER="pdm_dsn_user";
	RePdmValueName_DB_PWD="pdm_dsn_pwd";
	DWORD dwLength=256;
	sprintf(content_add,"%s",myFtp.strFtp);
	sprintf(content_user,"%s",myFtp.strFtpUser);
	sprintf(content_pwd,"%s",myFtp.strFtpPwd);
	content_port[0]=myFtp.iFtpPort;
	sprintf(content_dsn_name,"%s",m_DSN);
	sprintf(content_dsn_user,"%s",m_DBUsrName);
	sprintf(content_dsn_pwd,"%s",m_DBPwd);
	sprintf(content_shujuyuan,"%s",m_DSN);

	sprintf(content_pdm_dsn_name,"%s",m_PdmDsn);
	sprintf(content_pdm_dsn_user,"%s",m_PdmDsnUser);
	sprintf(content_pdm_dsn_pwd,"%s",m_PdmDsnPwd);

	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
	{
		RegSetValueEx(hKey,ReValueName_address,NULL,REG_SZ,(unsigned char *)content_add,myFtp.strFtp.GetLength());
		RegSetValueEx(hKey,ReValueName_user,NULL,REG_SZ,(unsigned char *)content_user,myFtp.strFtpUser.GetLength());
		RegSetValueEx(hKey,ReValueName_pwd,NULL,REG_SZ,(unsigned char *)content_pwd,myFtp.strFtpPwd.GetLength());
		RegSetValueEx(hKey,ReValueName_port,NULL,REG_DWORD,(unsigned char *)content_port,4);
        RegSetValueEx(hKey,ReValueName_DB_NAME,NULL,REG_SZ,(unsigned char *)content_dsn_name,m_DSN.GetLength());

		 RegSetValueEx(hKey,ReValueName_SHUJU,NULL,REG_SZ,(unsigned char *)content_shujuyuan,m_DSN.GetLength());
        RegSetValueEx(hKey,ReValueName_DB_USER,NULL,REG_SZ,(unsigned char *)content_dsn_user,m_DBUsrName.GetLength());
		RegSetValueEx(hKey,ReValueName_DB_PWD,NULL,REG_SZ,(unsigned char *)content_dsn_pwd,m_DBPwd.GetLength());

		RegSetValueEx(hKey,RePdmValueName_DB_NAME,NULL,REG_SZ,(unsigned char *)content_pdm_dsn_name,m_PdmDsn.GetLength());
		RegSetValueEx(hKey,RePdmValueName_DB_USER,NULL,REG_SZ,(unsigned char *)content_pdm_dsn_user,m_PdmDsnUser.GetLength());
		RegSetValueEx(hKey,RePdmValueName_DB_PWD,NULL,REG_SZ,(unsigned char *)content_pdm_dsn_pwd,m_PdmDsnPwd.GetLength());
		RegCloseKey(hKey);
	}
	dbLinkInfo=_T("DSN="+m_DSN+";UID="+m_DBUsrName+";PWD="+m_DBPwd+";DATABASE=RuiLiMdlDb;");
	dbPdmLinkInfo=_T("DSN="+m_PdmDsn+";UID="+m_PdmDsnUser+";PWD="+m_PdmDsnPwd+";");
	//AfxMessageBox(dbLinkInfo);
	OnOK();
}

BOOL COptionSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	int i=3;
    m_DSN=dbLinkInfo.Tokenize("=;",i);
	i=i+3;
	m_DBUsrName=dbLinkInfo.Tokenize("=;",i);
	i=i+3;
	m_DBPwd=dbLinkInfo.Tokenize("=;",i);

	i=3;
	m_PdmDsn=dbPdmLinkInfo.Tokenize("=;",i);
	i=i+3;
	m_PdmDsnUser=dbPdmLinkInfo.Tokenize("=;",i);
	i=i+3;
	m_PdmDsnPwd=dbPdmLinkInfo.Tokenize("=;",i);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void COptionSetDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
    dbLinkInfo=_T("DSN="+m_DSN+";UID="+m_DBUsrName+";PWD="+m_DBPwd+";DATABASE=RuiLiMdlDb;");
	CDatabase db;
	if(db.Open(m_DSN,FALSE,FALSE,_T("ODBC;UID="+m_DBUsrName+";PWD="+m_DBPwd+";DATABASE=RuiLiMdlDb;"),TRUE)) AfxMessageBox("�ɹ�");
}

void COptionSetDlg::OnBnClickedButton10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	dbPdmLinkInfo=_T("DSN="+m_PdmDsn+";UID="+m_PdmDsnUser+";PWD="+m_PdmDsnPwd+";");
	CDatabase db;
	if(db.Open(m_PdmDsn,FALSE,FALSE,_T("ODBC;UID="+m_PdmDsnUser+";PWD="+m_PdmDsnPwd+";"),TRUE)) AfxMessageBox("�ɹ�");
}

void COptionSetDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialog::OnMoving(fwSide, pRect);

	// TODO: �ڴ������Ϣ����������
	
}

void COptionSetDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: �ڴ������Ϣ����������
	UsrRepaintWindow();
}
///************************************************************************/
///* ���ù���Ŀ¼���ļ�����Ĭ��ָ����Ŀ¼��                               */
///************************************************************************/
//void COptionSetDlg::OnBnClickedButton3()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	ProPath       directory;
//	ProPath       defdirpath;
//	ProName       name;
//	ProError	  status;
//	CString       selDir;
//	CString       defDir;
//	ProStringToWstring(name,"ѡ�񱣴�λ��");
//	if (!defDir.IsEmpty())
//	{
//		ProStringToWstring(defdirpath,defDir.GetBuffer());
//		status=ProDirectoryChoose(name,NULL,NULL,defdirpath,directory);
//	}
//	else
//	{
//		status=ProDirectoryChoose(name,NULL,NULL,NULL,directory);
//	}
//	if (status==PRO_TK_NO_ERROR)
//	{
//		selDir=CString(directory)+"\\";
//	}
//}
