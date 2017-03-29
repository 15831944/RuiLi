// ansys.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ansys.h"
#include "myproe.h"
/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include"ProMenu.h"
#include"ProUtil.h"
#include"ProMenubar.h"
#include <ProMessage.h>
#include "ProArray.h"
#include <ProNotify.h>
#include "AreaDlg.h"
#include"HandSpikeDlg.h"
#include"ColumnDlg.h"
#include"WorkNumDlg.h"
#include"LoadDlg.h"
#include"RigidityDlg.h"
#include"StabilityDlg.h"
#include"TowerDlg.h"
#include"WorkNum0Dlg.h"
#include"Load0Dlg.h"
#include"Rigidity0Dlg.h"
#include"Stability0Dlg.h"
#include"DrumDlg.h"
#include"WorkNum1Dlg.h"
#include"Load1Dlg.h"
#include"Rigidity1Dlg.h"
#include"Stability1Dlg.h"
#include"HandSpike1Dlg.h"
#include"TreeDlg.h"
#include "FemOpenDlg.h"
#include "FemSave.h"
#include "FemModelSave.h"
#include "SaveModelDlg.h"
#include "ModelQueryDlg.h"
#include "AllFemMdlDlg.h"

#include "UtilDimen.h"

#include "ToleranceDlg.h"
#include "Tolerance.h"

FtpConnectInfo myFtp;
/////ȫ����ģ�ͺ�
int globle_MdlNum;
CString globl_connectString;

  CTreeDlg * treedlg=NULL;

void GetDBAndFtpInfo();

int globalSaveAs(CString path, ProMdl * pSolid);
int globalSaveAs(CString path,CString name, ProMdl *pSolid);
int globalLoadModel(CString path,ProMdl * pSolid);
int globalShowNewWindow(ProMdl * pSolid);
/*--------------------------------------------------------------------*\
Functions declaration  ��������
\*--------------------------------------------------------------------*/
int AreaComputeAction();
int HandspikeLengthAction();
int ColumnSpringAction();
int TowerSpringAction();
int DrumSpringAction();
int IntensionAnalyseAction();
int PerformanceCurveAction();
int PumpIntensionAction();
int SpringAction();
int GearComputeAction();
int HandspikeLength1Action();
int ColumnSpring1Action();
int TowerSpring1Action();
int HookSpringAction();
int Spring1Action();
int ShellFemAction();
int PoleIntensionAction();
int ScrewthreadIntensionAction();
int ComputeAndEmluteAction();
int FemAnalyse1Action();
int FemAnalyse2Action();
int GetMdlNum();                             //�õ�ģ�ͺ�

//�˵���������
//����
void DownLoad();
//��
//void open1();
void open2();

//����
void save1();
void save2();
void FemAnalyseAction();

/************************************************************************/
/* �����ѯ����                                                        */
/************************************************************************/

int MachineUnNoteAction();///��е�ӹ�δע����


int ShowMessageTest();
extern void OutputMainfunction();
extern void OutPutExcel();
extern BOOL GetDBInfo(CString &);
extern CString gl_vdsDBLinkInfo;
extern CMyDatabase gl_db;


static uiCmdAccessState AccessAvailable (uiCmdAccessMode);
//2.�������ģ��Ķ��壬�����ǰ�����ģʽ����Ч
//static uiCmdAccessState UsrAccessPartDefault(uiCmdAccessMode access_mode)//���ʿ��ƺ���
//{
//	ProMdl  mdl;
//	ProMdlType type;
//	if (ProMdlCurrentGet(&mdl)==PRO_TK_NO_ERROR) {
//		ProMdlTypeGet(mdl,&type);
//		if (type==PRO_MDL_PART) {
//			return (ACCESS_AVAILABLE);
//		}
//		else  return (ACCESS_REMOVE);		
//	}
//	else return (ACCESS_REMOVE);
//
//}
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

_RecordsetPtr	m_pRecordset;
_ConnectionPtr m_pConnection;

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		���磺
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CansysApp

BEGIN_MESSAGE_MAP(CansysApp, CWinApp)
END_MESSAGE_MAP()


// CansysApp �ṹ

CansysApp::CansysApp()
{
	// TODO: �ڴ���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ�� CansysApp ����

CansysApp theApp;


// CansysApp ��ʼ��

BOOL CansysApp::InitInstance()
{
	CWinApp::InitInstance();
	//AfxOleInit();

	return TRUE;
}
/*=======================================================================*\
FUNCTION: user_initialize()
\*=======================================================================*/
extern "C" int user_initialize()
{



	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProFileName MsgFile;
	//uiCmdCmdId   ComputeAndEmlute_cmd_id,open2_cmd_id,save2_cmd_id,DownLoad_cmd_id,FemAnalyse_cmd_id,FemAnalyse1_cmd_id,FemAnalyse2_cmd_id;
	uiCmdCmdId   ComputeAndEmlute_cmd_id;
	ProStringToWstring(MsgFile, "Message1.txt");//���ò˵���Ϣ�ļ���


	/*=========================================================*\
	  ������Ӳ˵���
	\*=========================================================*/
	status=ProMenubarMenuAdd ("Auxiliary Tool", "Auxiliary Tool",
		"Windows", PRO_B_TRUE, MsgFile);
	// ProMenubarmenuMenuAdd("Auxiliary Tool","UserMenubar","UserMenubar",NULL,PRO_B_TRUE ,MsgFile);


    /************************************************************************/
    /* �����ѯ                                                             */
    /************************************************************************/
	//ProMenubarmenuMenuAdd("Auxiliary Tool","Tolerance Query","Tolerance Query",NULL,PRO_B_TRUE ,MsgFile);

	uiCmdCmdId MachineUnNote_cmd_id;
	ProCmdActionAdd("MachineUnNoteCommand",(uiCmdCmdActFn)MachineUnNoteAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&MachineUnNote_cmd_id);
	ProMenubarmenuPushbuttonAdd("Auxiliary Tool","Tolerance Query","Tolerance Query","Tolerance Query",
		NULL,PRO_B_TRUE,MachineUnNote_cmd_id,MsgFile);
    
	/************************************************************************/
	/* �����ѯ����                                                          */
	/************************************************************************/
	/************************************************************************/
	/* ���EXCEL                                                             */
	/************************************************************************/
	uiCmdCmdId OutExcel_cmd_id;
	status=ProCmdActionAdd("OutputDrwDetail",(uiCmdCmdActFn)ShowMessageTest,uiCmdPrioDefault,AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&OutExcel_cmd_id);
	status=ProMenubarmenuPushbuttonAdd("Auxiliary Tool","OutputAsmDrawingDetail","OutputAsmDrawingDetail","OutputAsmDrawingDetail",NULL,PRO_B_TRUE,OutExcel_cmd_id,MsgFile);
	
	///*=========================================================*\
	//��Ӳ˵���
	//\*=========================================================*/
	//status=ProMenubarMenuAdd ("UserMenubar", "UserMenubar",
	//	"Windows", PRO_B_TRUE, MsgFile);
	/*=========================================================*\
	�˵������¼��Ӳ˵�
	\*=========================================================*/
	//һ�����
	ProCmdActionAdd("ComputeAndEmlute",(uiCmdCmdActFn)ComputeAndEmluteAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&ComputeAndEmlute_cmd_id);
	ProMenubarmenuPushbuttonAdd("Auxiliary Tool","ComputeAndEmluteButton","ComputeAndEmluteButton","Adding a ComputeAndEmlute push button",
		NULL,PRO_B_TRUE,ComputeAndEmlute_cmd_id,MsgFile);
	//����Ԫģ�͹���

//	ProMenubarmenuMenuAdd("UserMenubar","FemMdlManage","FemMdlManage",NULL,PRO_B_TRUE,MsgFile);

	//����ģ��
//	ProCmdActionAdd("DownLoadMdl",(uiCmdCmdActFn)DownLoad,uiCmdPrioDefault,
//		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&DownLoad_cmd_id);
//	ProMenubarmenuPushbuttonAdd("FemMdlManage","DownLoadButton","DownLoadButton","Adding a DownLoad push button",
//		NULL,PRO_B_TRUE,DownLoad_cmd_id,MsgFile);

	//���¼��˵����һ���򿪲˵�
	//ProMenubarmenuMenuAdd("FemMdlManage","OpenMld","OpenMld",NULL,PRO_B_TRUE ,MsgFile);
//	ProCmdActionAdd("Open2Mld",(uiCmdCmdActFn)open2,uiCmdPrioDefault,
//		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&open2_cmd_id);
//	ProMenubarmenuPushbuttonAdd("FemMdlManage","open2Button","open2Button","Adding a open2Button push button",
//		NULL,PRO_B_TRUE,open2_cmd_id,MsgFile);
 
	//���¼��˵�����һ���ڴ򿪱���ģ�Ͳ˵���
	//ProCmdActionAdd("OpenMld1",(uiCmdCmdActFn)open1,uiCmdPrioDefault,
	//	AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&open1_cmd_id);
	//ProMenubarmenuPushbuttonAdd("OpenMld","open1Button","open1Button","Adding a open1Button push button",
	//	NULL,PRO_B_TRUE,open1_cmd_id,MsgFile);
	//���һ���򿪹�����ģ�Ͳ˵���
	//ProCmdActionAdd("OpenMld2",(uiCmdCmdActFn)open2,uiCmdPrioDefault,
	//	AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&open2_cmd_id);
	//ProMenubarmenuPushbuttonAdd("OpenMld","open2Button","open2Button","Adding a open2Button push button",
	//	NULL,PRO_B_TRUE,open2_cmd_id,MsgFile);
	
	//���¼��˵����һ������˵�
	//ProMenubarmenuMenuAdd("FemMdlManage","SaveMld","SaveMld",NULL,PRO_B_TRUE ,MsgFile);
//	ProCmdActionAdd("SaveMld2",(uiCmdCmdActFn)save2,uiCmdPrioDefault,
//		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&save2_cmd_id);
//	ProMenubarmenuPushbuttonAdd("FemMdlManage","save2Button","save2Button","Adding a save2Button push button",
//		NULL,PRO_B_TRUE,save2_cmd_id,MsgFile);
		
	//���¼��˵�����һ���ڱ��浽ģ�Ͳ˵���
	/*ProCmdActionAdd("SaveMld1",(uiCmdCmdActFn)save1,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&save1_cmd_id);
	ProMenubarmenuPushbuttonAdd("SaveMld","save1Button","save1Button","Adding a save1Button push button",
		NULL,PRO_B_TRUE,save1_cmd_id,MsgFile);*/
	//���һ�����浽������ģ�Ͳ˵���
	/*ProCmdActionAdd("SaveMld2",(uiCmdCmdActFn)save2,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&save2_cmd_id);
	ProMenubarmenuPushbuttonAdd("SaveMld","save2Button","save2Button","Adding a save2Button push button",
		NULL,PRO_B_TRUE,save2_cmd_id,MsgFile);*/
    
	//����Ԫģ�Ͳ���
//	ProCmdActionAdd("FemAnalyse",(uiCmdCmdActFn)FemAnalyseAction,uiCmdPrioDefault,
//		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&FemAnalyse_cmd_id);
//	ProMenubarmenuPushbuttonAdd("UserMenubar","FemAnalyseButton","FemAnalyseButton","Adding a FemAnalyse push button",
//		NULL,PRO_B_TRUE,FemAnalyse_cmd_id,MsgFile);

	//����Ԫ����
//	ProCmdActionAdd("FemAnalyse1",(uiCmdCmdActFn)FemAnalyse1Action,uiCmdPrioDefault,
//		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&FemAnalyse1_cmd_id);
//	ProMenubarmenuPushbuttonAdd("UserMenubar","FemAnalyse1Button","FemAnalyse1Button","Adding a FemAnalyse1 push button",
//		NULL,PRO_B_TRUE,FemAnalyse1_cmd_id,MsgFile);

	//����Ԫ����2
//	ProCmdActionAdd("FemAnalyse2",(uiCmdCmdActFn)FemAnalyse2Action,uiCmdPrioDefault,
//		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&FemAnalyse2_cmd_id);
//	ProMenubarmenuPushbuttonAdd("UserMenubar","FemAnalyse2Button","FemAnalyse2Button","Adding a FemAnalyse2 push button",
//		NULL,PRO_B_TRUE,FemAnalyse2_cmd_id,MsgFile);

treedlg=NULL;

	globl_connectString="DSN=ruili;UID=scm;PWD=scm";
	//Ftp����
	myFtp.iFtpPort=23;
	myFtp.strFtp="10.15.43.134";
	myFtp.strFtpPwd="part";
	myFtp.strFtpUser="part";
	GetDBAndFtpInfo();
	return status;

}

/************************************************************************/
/* ��ȡ�����������ݿ�������Ϣ                                           */
/************************************************************************/
void GetDBAndFtpInfo()
{
	HKEY      hKey;
	struct    HKEY__*ReRootKey;
	TCHAR     *ReSubKey;//*ReExitProKey,*RegSubKeyToCreate;
	TCHAR     *ReValueName_address,*ReValueName_user,*ReValueName_pwd,*ReValueName_port;
	TCHAR     *ReValueName_DB_NAME,*ReValueName_DB_USER,*ReValueName_DB_PWD;
	TCHAR     *RePdmValueName_DB_NAME,*RePdmValueName_DB_USER,*RePdmValueName_DB_PWD;
	TCHAR     *ReValueName_SHUJU;
	char      content_add[256]="",content_user[256]="",content_pwd[256]="";
	char      content_dsn_user[256]="",content_dsn_pwd[256]="",content_dsn_name[256]="";
	char      content_pdm_dsn_user[256]="",content_pdm_dsn_pwd[256]="",content_pdm_dsn_name[256]="";
	char      content_shujuyuan[256]="";
	int       content_port[256];

	DWORD     dwType;		//�����ȡ�������� REG_DWORD
	ReRootKey=HKEY_CURRENT_USER;              //ע�����������
	ReSubKey="Software\\PTC\\VariantDesign";  //����ע���ֵ�ĵ�ַ
	//ReExitProKey="Software\\PTC";             //�������������Ҫ����
	//RegSubKeyToCreate="VariantDesign"         //�����ļ�
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
	DWORD dwLength1,dwLength2,dwLength3,dwLength4,dwLength5,dwLength6,dwLength7,dwLength8,dwLength9,dwLength10,dwLength11;
	try
	{
		if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_READ,&hKey)==ERROR_SUCCESS)
		{
			if(RegQueryValueEx(hKey,ReValueName_address,NULL,&dwType,(unsigned char *)content_add,&dwLength1)!=ERROR_SUCCESS) content_add[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_user,NULL,&dwType,(unsigned char *)content_user,&dwLength2)!=ERROR_SUCCESS) content_user[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_pwd,NULL,&dwType,(unsigned char *)content_pwd,&dwLength3)!=ERROR_SUCCESS) content_pwd[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_port,NULL,&dwType,(unsigned char *)content_port,&dwLength4)!=ERROR_SUCCESS) content_port[0]=0;
			if(RegQueryValueEx(hKey,ReValueName_DB_USER,NULL,&dwType,(unsigned char *)content_dsn_user,&dwLength5)!=ERROR_SUCCESS) content_dsn_user[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_DB_NAME,NULL,&dwType,(unsigned char *)content_dsn_name,&dwLength6)!=ERROR_SUCCESS) content_dsn_name[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_DB_PWD,NULL,&dwType,(unsigned char *)content_dsn_pwd,&dwLength7)!=ERROR_SUCCESS) content_dsn_pwd[0]='\0';
			if(RegQueryValueEx(hKey,ReValueName_SHUJU,NULL,&dwType,(unsigned char *)content_shujuyuan,&dwLength8)!=ERROR_SUCCESS) content_shujuyuan[0]='\0';

			if(RegQueryValueEx(hKey,RePdmValueName_DB_USER,NULL,&dwType,(unsigned char *)content_pdm_dsn_user,&dwLength9)!=ERROR_SUCCESS) content_pdm_dsn_user[0]='\0';
			if(RegQueryValueEx(hKey,RePdmValueName_DB_NAME,NULL,&dwType,(unsigned char *)content_pdm_dsn_name,&dwLength10)!=ERROR_SUCCESS) content_pdm_dsn_name[0]='\0';
			if(RegQueryValueEx(hKey,RePdmValueName_DB_PWD,NULL,&dwType,(unsigned char *)content_pdm_dsn_pwd,&dwLength11)!=ERROR_SUCCESS) content_pdm_dsn_pwd[0]='\0';
			RegCloseKey(hKey);

			myFtp.strFtp=CString(content_add);
			myFtp.strFtpUser=CString(content_user);         
			myFtp.strFtpPwd=CString(content_pwd);
			myFtp.iFtpPort=content_port[0];
			globl_connectString=_T("DSN="+CString(content_shujuyuan)+";UID="+CString(content_dsn_user)+";PWD="+CString(content_dsn_pwd));
			//dbPdmLinkInfo=_T("DSN="+CString(content_pdm_dsn_name)+";UID="+CString(content_pdm_dsn_user)+";PWD="+CString(content_pdm_dsn_pwd)+";");
		}
		else
		{
			HKEY hk;
			DWORD disposition;   
			if (RegCreateKeyEx(ReRootKey,ReSubKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hk,&disposition))
			{
				AfxMessageBox("ע���ע��ʧ��,������ֹ�����");
				return;
			}
			RegCloseKey(hk);
			
		}

	}
	catch (...) {
		AfxMessageBox("��ע����ȡ����ʧ��");
	}

}


	/*===============================================================*\
	FUNCTION: user_terminate()
	\*===============================================================*/
	
    extern "C" void user_terminate()
   {
	   	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	  if (treedlg!=NULL)		delete treedlg;

	return;
   }
	/*================================================================*\
	FUNCTION: AccessAvailable ���˵���ķ���Ȩ�����ã���ѡ��
	\*================================================================*/
	static uiCmdAccessState AccessAvailable (uiCmdAccessMode access_mode)
	{
		return (ACCESS_AVAILABLE);
	}
	/*====================================================================*\
	FUNCTION :	ComputeAndEmluteAction() 
	\*====================================================================*/
  

	int ComputeAndEmluteAction()
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		if(treedlg!=NULL)
		{
			delete  treedlg;
		}
		treedlg=new CTreeDlg();
		treedlg->Create(IDD_DIALOG_TREE);
		treedlg->ShowWindow(SW_SHOW);
		return 0;
	}
	/*====================================================================*\
	FUNCTION :	AreaComputeAction() 
	\*====================================================================*/
	int AreaComputeAction()
	{   
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
        CAreaDlg areadlg;
		areadlg.DoModal();
		return 0;
	}
	/*====================================================================*\
	FUNCTION :	HandspikeLengthAction() 
	\*====================================================================*/
	int HandspikeLengthAction()
	{   
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CHandSpikeDlg handspikedlg;
		handspikedlg.DoModal();
		return 0;
	}

	/*====================================================================*\
	FUNCTION :	DownLoad()
	\*====================================================================*/
	void DownLoad()
	{   
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CModelQueryDlg dlg;
		dlg.DoModal();

	}
	/*====================================================================*\
	FUNCTION :	FemAnalyseAction()
	\*====================================================================*/
	void FemAnalyseAction()
	{   
    //   AFX_MANAGE_STATE(AfxGetStaticModuleState());
	   //int m_MdlNum=GetMdlNum();
	   //if (m_MdlNum==-1)
	   //{
		  // AfxMessageBox("����Ƿ�ģ�ͣ��޷�Ϊ������Ԫ����");
		  // return;
	   //}
	   //else
	   //{
		  // CString str,strSql;
		  // int rows;
		  // strSql.Format("select * from FemPrt where ConnPrtMdlNum=%d",m_MdlNum);
		  // m_db.Query(strSql);
		  // rows=m_db.getRows();
		  // if(rows==0)                  //���û������Ԫģ��
		  // {
			 //  AfxMessageBox("�����ģ��,���Ƚ�������Ԫģ��");
			 //  return;
		  // }
		  // else
			 //  if (rows==1) 
			 //  {
    //                 //ֱ�Ӵ��Ǹ�ģ��
				//    m_db.MoveBegin();
				//	do {
				//		str=m_db.getString("FileDirectory");
				//	} while(m_db.MoveNext());

			 //  }
			 //  else
			 //  {
				//   CAllFemMdlDlg dlg;
				//   dlg.DoModal();
			 //  }
	   //}
	}
	/*====================================================================*\
	FUNCTION :	open1()
	\*====================================================================*/
	void open1()
	{   
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CString filePath;///
		static char BASED_CODE szFilter[] = "���(*.prt)|*.prt.*|All Files (*.*)|*.*||";
		CFileDialog  fdlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,NULL,0);
		if(fdlg.DoModal()!=IDOK)
		{
			return;
		}
		filePath=fdlg.GetPathName();
			ProSolid solid;
       globalLoadModel(filePath,(ProMdl *)&solid);
	   globalShowNewWindow((ProMdl *)&solid);

		
	}
	/*====================================================================*\
	FUNCTION :	open2()
	\*====================================================================*/
	void open2()
	{   
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CFemOpenDlg dlg;
		dlg.DoModal();
		
	}
	/*====================================================================*\
	FUNCTION :	save1()
	\*====================================================================*/
	void save1()
	{   
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		ProError error;
		ProSolid rootSolid;
		error=ProMdlCurrentGet((ProMdl*)&rootSolid); //��õ�ǰģ��
		if (error!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("�޷��õ���ǰģ��!");
			return ;
		}
		CString filePath;///
		static char BASED_CODE szFilter[] = "���(*.prt)|*.prt.*|All Files (*.*)|*.*||";
		CFileDialog  fdlg(FALSE,"prt",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,NULL,0);
		if(fdlg.DoModal()!=IDOK)
		{
			return;
		}
		filePath=fdlg.GetPathName();//
		globalSaveAs(filePath,(ProMdl *)&rootSolid);


	}

	/*====================================================================*\
	FUNCTION :	save2()
	\*====================================================================*/
	void save2()
	{   
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		//CFemSave dlg;
		//CFemModelSave dlg;
		CSaveModelDlg dlg;
		dlg.DoModal();
	}
	/*====================================================================*\
	FUNCTION :	IntensionAnalyseAction()
	\*====================================================================*/
	int IntensionAnalyseAction()
	{   
		return 0;
	}

	/*====================================================================*\
	FUNCTION :	PerformanceCurveAction()
	\*====================================================================*/
	int PerformanceCurveAction()
	{   
		return 0;
	}

	/*====================================================================*\
	FUNCTION :	PumpIntensionAction()
	\*====================================================================*/
	int PumpIntensionAction()
	{   
		return 0;
	}
	/*====================================================================*\
	FUNCTION :	SpringAction()
	\*====================================================================*/
	int SpringAction()
	{   
		return 0;
	}
	
	/*====================================================================*\
	FUNCTION :	GearComputeAction()
	\*====================================================================*/
	int GearComputeAction()
	{   
		return 0;
	}
	/*====================================================================*\
	FUNCTION :	HandspikeLength1Action()()
	\*====================================================================*/
	int HandspikeLength1Action()
	{   
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CHandSpike1Dlg handspike1dlg;
		handspike1dlg.DoModal();
		return 0;
	}


	/*====================================================================*\
	FUNCTION :	 FemAnalyse1Action()
	\*====================================================================*/
	int  FemAnalyse1Action()
	{   
		ProError  status;
		ProMacro  w_macro;
		CString strMacro;
		
		//strMacro="~ Select `main_dlg_cur` `MenuBar1`1  `Applications`";
		//ProStringToWstring (w_macro,strMacro.GetBuffer());//
		//status = ProMacroLoad(w_macro); 

		//strMacro="~ Close `main_dlg_cur` `MenuBar1`";
		//ProStringToWstring (w_macro,strMacro.GetBuffer());
		//status = ProMacroLoad(w_macro); 

		//strMacro="~ Select `main_dlg_cur` `Applications.rad_util_app_prt`1  `Mechanica`";
		//ProStringToWstring (w_macro,strMacro.GetBuffer());
		//status = ProMacroLoad(w_macro); 

		//strMacro="~ Activate `sim_unit_info` `ContinuePush`";
		//ProStringToWstring (w_macro,strMacro.GetBuffer());
		//status = ProMacroLoad(w_macro); 

		//strMacro="~ Activate `main_dlg_cur` `ProCmdSimMechSol.sim_me`";
		//ProStringToWstring (w_macro,strMacro.GetBuffer());
		//status = ProMacroLoad(w_macro); 

		strMacro="~ Select `main_dlg_cur` `MenuBar1`1  `Applications`;\
				 ~ Close `main_dlg_cur` `MenuBar1`;\
				 ~ Select `main_dlg_cur` `Applications.rad_util_app_prt`1  `Mechanica`;\
				 ~ Activate `sim_unit_info` `ContinuePush`;";

		ProStringToWstring (w_macro,strMacro.GetBuffer());
		status = ProMacroLoad(w_macro); 

		strMacro=" ~ Activate `main_dlg_cur` `ProCmdSimMechSol.sim_me`;\
				  ~ Select `run` `MenuBar1`1  `FileSTRC`;~ Close `run` `MenuBar1`;\
				  ~ Activate `run` `FileNewStatic`;~ Activate `anl_static_dlg` `okBtn`;";
		ProStringToWstring (w_macro,strMacro.GetBuffer());
		status = ProMacroLoad(w_macro); 

          
		strMacro=" ~ Select `main_dlg_cur` `MenuBar1`1  `Applications`;\
				 ~ Close `main_dlg_cur` `MenuBar1`;\
				 ~ Select `main_dlg_cur` `Applications.rad_util_app_prt`1  `Mechanica`;\
				 ~ Activate `main_dlg_cur` `ProCmdSimMechSol.sim_me`;";
		ProStringToWstring (w_macro,strMacro.GetBuffer());
		status = ProMacroLoad(w_macro); 


		strMacro="~ Select `main_dlg_cur` `MenuBar1`1  `Applications`;\
				 ~ Close `main_dlg_cur` `MenuBar1`;\
				 ~ Select `main_dlg_cur` `Applications.rad_util_app_prt`1  `Mechanica`;\
				 ~ Activate `sim_unit_info` `ContinuePush`;";

		ProStringToWstring (w_macro,strMacro.GetBuffer());
		status = ProMacroLoad(w_macro); 

		strMacro=" ~ Activate `main_dlg_cur` `ProCmdSimMechSol.sim_me`;\
				 ~ Select `run` `MenuBar1`1  `FileSTRC`;~ Close `run` `MenuBar1`;\
				 ~ Activate `run` `FileNewStatic`;~ Activate `anl_static_dlg` `okBtn`;";
		ProStringToWstring (w_macro,strMacro.GetBuffer());
		status = ProMacroLoad(w_macro); 


		strMacro=" ~ Select `main_dlg_cur` `MenuBar1`1  `Applications`;\
				 ~ Close `main_dlg_cur` `MenuBar1`;\
				 ~ Select `main_dlg_cur` `Applications.rad_util_app_prt`1  `Mechanica`;\
				 ~ Activate `main_dlg_cur` `ProCmdSimMechSol.sim_me`;";
		ProStringToWstring (w_macro,strMacro.GetBuffer());
		status = ProMacroLoad(w_macro); 
					
		ProWindowActivate(PRO_VALUE_UNUSED);

		
		return 1;
	}

	/*====================================================================*\
	FUNCTION :	 FemAnalyse2Action()
	\*====================================================================*/
	int  FemAnalyse2Action()
	{   
		ProError  status;
		ProMacro  w_macro;
		CString strMacro;

		strMacro=" ~ Select `main_dlg_cur` `MenuBar1`1  `Applications`;\
				  ~ Close `main_dlg_cur` `MenuBar1`;\
				  ~ Select `main_dlg_cur` `Applications.rad_util_app_prt`1  `Mechanica`;\
				  ~ Activate `main_dlg_cur` `ProCmdSimMechSol.sim_me`;";
		ProStringToWstring (w_macro,strMacro.GetBuffer());
		status = ProMacroLoad(w_macro); 

		/*strMacro="~ Activate `sim_unit_info` `ContinuePush`";
		ProStringToWstring (w_macro,strMacro.GetBuffer());
		status = ProMacroLoad(w_macro); 

		strMacro="~ Activate `main_dlg_cur` `main_dlg_cur`";
		ProStringToWstring (w_macro,strMacro.GetBuffer());
		status = ProMacroLoad(w_macro); */

		//strMacro="~ Activate `sim_unit_info` `ContinuePush`";
		//ProStringToWstring (w_macro,strMacro.GetBuffer());
		//status = ProMacroLoad(w_macro); 

		//strMacro="~ Activate `main_dlg_cur` `ProCmdSimMechSol.sim_me`";
		//ProStringToWstring (w_macro,strMacro.GetBuffer());
		//status = ProMacroLoad(w_macro); 

		//strMacro="~ Select `run` `MenuBar1` \
		//		 1  `FileSTRC`";
		//ProStringToWstring (w_macro,strMacro.GetBuffer());
		//status = ProMacroLoad(w_macro); 

		//strMacro="~ Close `run` `MenuBar1`";
		//ProStringToWstring (w_macro,strMacro.GetBuffer());
		//status = ProMacroLoad(w_macro); 

		//strMacro="~ Activate `run` `FileNewStatic`";
		//ProStringToWstring (w_macro,strMacro.GetBuffer());
		//status = ProMacroLoad(w_macro); 

		//strMacro="~ Activate `anl_static_dlg` `okBtn`";
		//ProStringToWstring (w_macro,strMacro.GetBuffer());
		//status = ProMacroLoad(w_macro); 

		/*strMacro="~ Activate `run` `run`";
		ProStringToWstring (w_macro,strMacro.GetBuffer());
		status = ProMacroLoad(w_macro); */

        ProWindowActivate(PRO_VALUE_UNUSED);

		return 1;
	}

	
	/*====================================================================*\
	FUNCTION :	TowerSpring1Action()
	\*====================================================================*/
	int TowerSpring1Action()
	{   
		return 0;
	}    

	/*====================================================================*\
	FUNCTION :	HookSpringAction()
	\*====================================================================*/
	int HookSpringAction()
	{   
		return 0;
	}    
	/*====================================================================*\
	FUNCTION :	Spring1Action()
	\*====================================================================*/
	int Spring1Action()
	{   
		return 0;
	}

	/*====================================================================*\
	FUNCTION :	ShellFemAction()
	\*====================================================================*/
	int ShellFemAction()
	{   
		return 0;
	}

	/*====================================================================*\
	FUNCTION :	PoleIntensionAction()
	\*====================================================================*/
	int  PoleIntensionAction()
	{   
		return 0;
	}
	/*====================================================================*\
	FUNCTION :	ScrewthreadIntensionAction()
	\*====================================================================*/
	int ScrewthreadIntensionAction()
	{   
		return 0;
	}
//�õ�ģ�ͺ�
int GetMdlNum()
{
	ProSolid rootSolid;
	ProModelitem owner;
	ProError status;
	status=ProMdlCurrentGet((ProMdl*)&rootSolid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�޷��õ���ǰģ��!");
		return -1;
	}
	status=ProMdlToModelitem(rootSolid,&owner);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("ת�����ɹ�!");
		return -1;
	}
	ProName name;  
	ProParamvalue value; 
	ProParameter  param;
	CString strName="RL_MDL";
	ProStringToWstring(name,strName.GetBuffer());
	status = ProParameterInit (&owner,name,&param);
	if (status!=PRO_TK_NO_ERROR)
	{
		//AfxMessageBox("�޷��õ�����");
		return -1;
	}
	status=ProParameterValueGet(&param,&value);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�޷�ȡ�ò���ֵ!");
		return -1;
	}
	CString  m_MdlNum;
	m_MdlNum=CString(value.value.s_val);
//	util1.Load(rootSolid);
	return atoi(m_MdlNum);
}
////��ģ��
//BOOL OpenMdl(CString fullpath)
//{
//	CString fullpath;
//	CString temp="C:\\proe_temp";
//	::CreateDirectory(temp,NULL);
//	int index=0;
//	for (int i=0;i<m_List.GetItemCount();i++)
//	{
//		if ( m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
//		{
//			fullpath=m_List.GetItemText(i,2);
//			index=fullpath.ReverseFind('/');
//			CString name=fullpath.Mid(index+1);
//			m_Path=temp+"\\"+name;
//			ftpfile.DownloadFiletoFile(fullpath,m_Path,myFtp);
//		}
//	}
//	CString str=m_Path;
//	ProSolid solid;
//	CFemOpenDlg dlg;
//	int status;
//	status=dlg.LoadModel(str,(ProMdl *)&solid);
//	if(status==0) return;
//	util2.Load(solid);
//	for(int i=0;i<util1.dimArray.GetSize();i++)
//	{
//		util2.SetValue(util1.dimArray[i].Llabel,util1.dimArray[i].value);
//	}
//	ProSolidRegenerate(solid,PRO_B_TRUE);
//	dlg.ShowNewWindow((ProMdl *)&solid);
//}

/************************************************************************/
/* �����ѯ����                                                         */
/************************************************************************/

int UsrTableTextAdd(
					ProDwgtable *table,
					int col,
					int row,
					char *text)
{
	ProWstring *lines;
	ProArrayAlloc(1, sizeof(ProWstring), 1, (ProArray*)&lines);
	lines[0] = (wchar_t*)calloc(strlen(text) + 1, sizeof(wchar_t));
	ProStringToWstring(lines[0], text);
	ProDwgtableTextEnter(table, col, row, lines);
	ProArrayFree((ProArray*)&lines);
	return 1;
}

void UserScreenToDwg (
					  ProDrawing drawing,
					  ProPoint3d scn_point,
					  ProPoint3d drw_point)
{
	ProError status;
	ProName w_size;
	ProMatrix  matrix;

	int sheet;
	status=ProDrawingCurrentSheetGet(drawing,&sheet);

	status=ProDrawingSheetTrfGet (drawing, sheet, w_size, matrix);

	status=ProPntTrfEval (scn_point, matrix, drw_point);
}




int ToleranceQuery(ExcelPage & page)
{
	CString str;
	int window_id;
	ProError status;
	ProMdl mdl;
	ProDrawing drawing;
	status=ProMdlCurrentGet(&mdl); //��õ�ǰģ��
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�޷��õ���ǰģ��!");
		return 0;
	}

	ProMdlType mdltype;
	status=ProMdlTypeGet(mdl,&mdltype);
	if(mdltype==PRO_MDL_DRAWING)
	{
		ProMdlCurrentGet((ProMdl *)&drawing);
			ProMdlWindowGet(mdl,&window_id);
	}
	else
	{
		AfxMessageBox("��ǰ���Ƕ�άͼ����!");
		return 0;
	}

	ProMouseButton button;
	ProDwgtabledata tdata;
	ProDwgtable table;
	ProVector pos;
	int columns,rows;
	double width,height;

	double *heights;
	double *widths;
	ProHorzJust *justs;
	//ProHorzJust justs[] = {PROHORZJUST_LEFT,PROHORZJUST_LEFT,PROHORZJUST_LEFT,PROHORZJUST_LEFT,PROHORZJUST_LEFT};

//	rows=4;columns=4;
//	width=30;height=10;
//	rows=tol.GetRows()+1;columns=tol.m_Endcol-tol.m_Startcol+1+1;
	rows=page.rows+1;columns=page.cols;

	ProOutline outline;

	if(ProMousePickGet(PRO_ANY_BUTTON, &button, pos) != PRO_TK_NO_ERROR)
		return 0;
	if(ProMouseBoxInput(pos,outline)!=PRO_TK_NO_ERROR)
		return 0;

	double sx,sy;

	ProPoint3d point1,point2;
	for(int i=0;i<3;i++)
	{
		point1[i]=outline[0][i];
		point2[i]=outline[1][i];
	}

	pos[0]=point1[0]<point2[0]?point1[0]:point2[0];
	pos[1]=point1[1]>point2[1]?point1[1]:point2[1];

	ProPoint3d p1,p2;
	UserScreenToDwg(drawing,point1,p1);
	UserScreenToDwg(drawing,point2,p2);
	sx=p2[0]-p1[0];if(sx<0) sx=-sx;
    sy=p2[1]-p1[1];if(sy<0) sy=-sy;

	

	

	width=sx/columns;
	height=sy/rows;
	ProDwgtabledataAlloc(&tdata);
	ProDwgtabledataOriginSet(tdata, pos);
	//ProDwgtabledataSizetypeSet(tdata, PRODWGTABLESIZE_CHARACTERS);
	ProDwgtabledataSizetypeSet(tdata, PRODWGTABLESIZE_SCREEN);

   // widths = (double*)calloc(columns, sizeof(double));
	widths =new double[columns];
	for(int i=0;i<columns;i++)
		widths[i] =width;
	//justs=(ProHorzJust*)calloc(columns, sizeof(ProHorzJust));
	justs =new ProHorzJust[columns];
	for(int i=0;i<columns;i++)
		justs[i] =PROHORZJUST_CENTER;
	ProDwgtabledataColumnsSet(tdata, columns, widths, justs);
	//free(widths);free(justs);
	delete [] widths;delete [] justs;
	//heights = (double*)calloc(rows, sizeof(double));
	heights =new double[rows];
	for(int i=0;i<rows;i++)
		heights[i] =height;
	ProDwgtabledataRowsSet(tdata,rows, heights);
	//free(heights);
	delete []heights;
	/*--------------------------------------------------------------------*\    
	Create the table
	\*--------------------------------------------------------------------*/    
	ProMdlCurrentGet((ProMdl*)&drawing);
	ProDrawingTableCreate(drawing, tdata, PRO_B_FALSE, &table);

    ProDwgtableCellsMerge(&table,1,1,columns,1,0);
	str=page.Name;
    UsrTableTextAdd(&table,1,1,str.GetBuffer());

		for(int i=2;i<rows+1;i++)
		{
			for(int j=1;j<columns+1;j++)
			{
			 str=page.strArray[(i-2)*page.cols+j-1];
			 UsrTableTextAdd(&table,j,i,str.GetBuffer());
			}
		}
	

	ProDwgtableDisplay(&table);
	status=ProWindowActivate(window_id);


	return 1;
}


int MachineUnNoteAction()///��е�ӹ�δע����
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	ProError status;
	ProMdl mdl;
	ProDrawing drawing;
	status=ProMdlCurrentGet(&mdl); //��õ�ǰģ��
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�޷��õ���ǰģ��!");
		return 0;
	}

	ProMdlType mdltype;
	status=ProMdlTypeGet(mdl,&mdltype);
	if(mdltype==PRO_MDL_DRAWING)
	{
		ProMdlCurrentGet((ProMdl *)&drawing);
	}
	else
	{
		AfxMessageBox("��ǰ���Ƕ�άͼ����!");
		return 0;
	}

	CToleranceDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
      ToleranceQuery(dlg.m_OutPage);
	}

    //status=ToleranceQuery();
	return 1;
}





int ShowMessageTest()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	GetDBInfo(gl_vdsDBLinkInfo);
	//�������ݿ�
	gl_db.Begin();
	int status=0;
	status=gl_db.Open(gl_vdsDBLinkInfo);
	if(status==0)
	{
		AfxMessageBox("���ݿ����Ӵ��󣬳��򽫹ر�");
		return 0;
	}


	//ado�����й�
	m_pConnection.CreateInstance("ADODB.Connection");
	try
	{
		m_pConnection->ConnectionTimeout = 8;
		m_pConnection->Open((const char *)gl_vdsDBLinkInfo,"","",adModeUnknown);
		//m_pConnection->Open("Driver=SQL Server;Database=RuiLiMdlDb;Server=(local);UID=zjuvds;PWD=zjuvds;","","",adModeUnknown);
	}
	catch(_com_error e)///��׽�쳣
	{
		AfxMessageBox("m_pConnection���ݿ�����ʧ��!");
		return 0;
	} 

	OutputMainfunction();
	OutPutExcel();
	return 0;
}