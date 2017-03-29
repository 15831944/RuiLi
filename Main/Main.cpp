// Main.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Main.h"
#include "PartMainDlg.h"
#include "RelParaEditDlg.h"
#include "PicMakgeDlg.h"
//#include "MdlDefDlg.h"
#include "MdlPropDefDlg.h"
#include "OptionSetDlg.h"
//#include "WorkPlaceManDlg.h"
#include "NewDlg.h"
//#include "NewFromMdlDlg.h"
#include "AsmParaDefDlg.h"
#include "AsmMdlManDlg.h"
#include "DrawingDlg.h"
#include "UsrSWTXTable.h"
#include "AsmAddComplg.h"
#include "MyDatabase.h"
#include "TempWorkPlaceDlg.h"
#include "MenuOpenDlg.h"
#include "PicEditDlg.h"
#include "TestRelDlg.h"
#include "AsmParaDefGlg.h"
#include "2DrawingDlg.h"
#include <ProUITab.h>
//#include "variantdesign.h"
#include "wshf_dbinfo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define USR_FEAT_REL_STRING  10000
//////////////////////////////////////////////////////////////////////////
//����ȫ�ֱ���
//1.������������Ա�
CArray<UsrSWTXTable1,UsrSWTXTable1>  p_UsrSWTXTableList;  //����ȫ�ֵ��������Ա����
//�ڳ�������ʱ��Ҫ����һ����ʼֵ(��Ч)
//2.����Ĺ�ϵ
//CArray<UsrRelTable,UsrRelTable>    p_UsrRelTableList;   //����ȫ�ֵĹ�ϵ�б�������
//3.�����ͼƬ
CArray<UsrPicture,UsrPicture>   p_UsrPicture;           //ͼƬ��ȫ�ֱ���    
//4.��������ܲ���
CArray<UsrPerfParaTable,UsrPerfParaTable> p_UsrPerfParaTableList;//��������ܲ������ȫ�ֱ���

//5.ģ�͵Ļ������� ע�����ģ���װ��ģ�鹫��
CArray<UsrPartPorp,UsrPartPorp> p_UsrPartPorp;          //����ģ�ͻ�����Ϣ��ȫ�ֱ���

//6.װ�����������
CArray<AsmSmlItem,AsmSmlItem> p_AsmSmlItemList;
//CArray<CRel,CRel&>            p_CRelItemList;
CRelColection g_RelCollection;
//7.װ��Ĺ�ϵ�б�
//CArray<AsmRelItem,AsmRelItem> p_AsmRelItemList;
//8.װ���Ԫ���ı��η�Χ
CArray<AsmCompVarItem,AsmCompVarItem> p_AsmCompItemList;
//////////////////////////////////////////////////////////////////////////
//9.�Ի���ͼ��
int  DlgIcon;//=IDI_PROE;  //���öԻ���ı���ͼ��
////10.����ͼ�б�
//CArray<UsrDrawingItem,UsrDrawingItem> P_UsrDrawingList;
//10.���ݿ�
CMyDatabase * m_db;
CImageList      g_ImageList1;  //��ͼ��
CImageList      g_ImageList2;  //Сͼ��
//////////////////////////////////////////////////////////////////////////
//�Ի���ȫ�ֱ�������ҪΪ���÷�ģʽ��ʽ
//1.����ߴ硢��������Ϣ��ģ
CRelParaEditDlg * dlg_part;
//2.װ��ĳߴ硢��������Ϣ��ģ
CAsmParaDefDlg * dlg_asm;
//3.ͼƬ�༭�Ի���
CPicMakgeDlg * dlg_pic;
//4.���β���
CTestRelDlg * dlg_test;
CAsmParaDefDlg2 * dlg_asm_def_test;
//5.��ģ��װ��
CAsmAddComplg * addComp_dlg=NULL;

C2DrawingDlg * testdlg=NULL;

//////////////////////////////////////////////////////////////////////////
//һ��Ϊ���ݿ��FTP������������Ϣ
CString         PartConnectSet;   //���ӣ���Ҫ��һ�����
FtpConnectInfo  myFtp;
CString         dbLinkInfo;
CString         dbPdmLinkInfo;
//CString         gWorkDirectory;  //����Ŀ¼
int UserPopupmenusSetup();
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//���ʺ�������
//1.����װ��ģ��Ķ��壬�����ǰģ����װ������Ч
static uiCmdAccessState AccessAvailable (uiCmdAccessMode access_mode)
{
	ProMdl     mdl;
	ProMdlType type;
	if (ProMdlCurrentGet(&mdl)==PRO_TK_NO_ERROR) {
		ProMdlTypeGet(mdl,&type);
		if (type==PRO_MDL_ASSEMBLY) {
			return (ACCESS_AVAILABLE);
		}
		else  return (ACCESS_REMOVE);		
	}
	else return (ACCESS_REMOVE);
}
//2.�������ģ��Ķ��壬�����ǰ�����ģʽ����Ч
static uiCmdAccessState UsrAccessPartDefault(uiCmdAccessMode access_mode)//���ʿ��ƺ���
{
	ProMdl     mdl;
	ProMdlType type;
	if (ProMdlCurrentGet(&mdl)==PRO_TK_NO_ERROR) {
		ProMdlTypeGet(mdl,&type);
		if (type==PRO_MDL_PART) {
			return (ACCESS_AVAILABLE);
		}
		else  return (ACCESS_REMOVE);		
	}
	else return (ACCESS_REMOVE);

}
//3.ģ�ͻ�����Ϣ����ģ��,ֻ�е�ǰ���������װ���ʱ����Ч
static uiCmdAccessState UsrAccessCommonDefault(uiCmdAccessMode access_mode)//���ʿ��ƺ���
{
	ProMdl  mdl;
	ProMdlType type;
	if (ProMdlCurrentGet(&mdl)==PRO_TK_NO_ERROR) {
		ProMdlTypeGet(mdl,&type);
		if (type==PRO_MDL_PART || type==PRO_MDL_ASSEMBLY) {
			return (ACCESS_AVAILABLE);
		}
		else return (ACCESS_REMOVE);
	}
	else 
	{
		return (ACCESS_REMOVE);
	}

}
//3.ͼƬ������ť,�������,�齨�͹���ͼ����Ч
static uiCmdAccessState UsrAccessForPicMake(uiCmdAccessMode access_mode)//���ʿ��ƺ���
{
	ProMdl		 mdl;
	ProMdlType type;
	if (ProMdlCurrentGet(&mdl)==PRO_TK_NO_ERROR) {
		ProMdlTypeGet(mdl,&type); 
		if (type==PRO_MDL_PART || type==PRO_MDL_ASSEMBLY || type==PRO_MDL_DRAWING) {
			return (ACCESS_AVAILABLE);
		}
		else return (ACCESS_REMOVE);
	}
	else 
	{
		return (ACCESS_REMOVE);
	}

}
//4.�����ж�������Ч�ķ��ʺ���
static uiCmdAccessState UsrAccessDefault(uiCmdAccessMode access_mode)//���ʿ��ƺ���
{

	return (ACCESS_AVAILABLE);

}
//5.ȡ�����򲻿ɼ���
static uiCmdAccessState UsrAccessUNAVAILABLE(uiCmdAccessMode access_mode)//���ʿ��ƺ���
{

	return (ACCESS_REMOVE);
	//return(ACCESS_AVAILABLE);

}
//6.�����ģʽ�У�����װ�䰴ť
static uiCmdAccessState UsrAccessUNAVAILABLE_ASSEMBLY_MODE(uiCmdAccessMode access_mode)//���ʿ��ƺ���
{
	//ProMdl     mdl;
	//ProMdlType type;
	//if (ProMdlCurrentGet(&mdl)==PRO_TK_NO_ERROR) 
	//{
	//	ProMdlTypeGet(mdl,&type);
	//	if(type==PRO_MDL_ASSEMBLY) 
	//	{
	//		return  (ACCESS_REMOVE);
	//	}
	//}
	return (ACCESS_INVISIBLE);
}
//7.�Զ���װ�䰴ť��ֻ����װ��ģʽ�²ſɼ�
static uiCmdAccessState UsrAccessAVAILABLE_ASSEMBLY_MODE(uiCmdAccessMode access_mode)//���ʿ��ƺ���
{
	ProMdl     mdl;
	ProMdlType type;
	if (ProMdlCurrentGet(&mdl)==PRO_TK_NO_ERROR) 
	{		
		ProMdlTypeGet(mdl,&type);
		if(type==PRO_MDL_ASSEMBLY) 
		{
			DlgIcon=IDI_ASM;
			return  (ACCESS_AVAILABLE);
		}
	}
	return (ACCESS_REMOVE);
}
//8.����ͼ��Ч
static uiCmdAccessState UsrAccessForDrawing(uiCmdAccessMode access_mode)//���ʿ��ƺ���
{
	ProMdl		 mdl;
	ProMdlType type;
	if (ProMdlCurrentGet(&mdl)==PRO_TK_NO_ERROR) {
		ProMdlTypeGet(mdl,&type); 
		if (type==PRO_MDL_DRAWING) {
			return (ACCESS_AVAILABLE);
		}
		else return (ACCESS_REMOVE);
	}
	else 
	{
		return (ACCESS_REMOVE);
	}

}
//////////////////////////////////////////////////////////////////////////
//�˵���������
//1.�½�1
void new1();
//2.�������װ���ģ�Ϳ��м̳��½�
void new2();
//3.�򿪱���
void open1();
//4.ģ���ļ�����
void funcMdlManage();
//5.���浽����
void save1();
//6.���浽������
void save2();
//7.������Ϣ����
void MdlDef();
//8.����ߴ硢��������ϵ��������
void funcParaTalbe();//��������
//9.��ͼ��������
void funSamplePic();
//10.�������ģ�ͺ���
void funcPartManage();
//11.���ɲ�Ʒģ��
void funcProductManage();
//12.װ��ģ�ͳߴ硢��������ϵ��������
void AsmParaDefAcFn();
void AsmParaDefAcFn2();
//13.ѡ�� //ѡ�����ú���
void funcOption();
//14.������     ע������ɾ��
void workplaceman();
//15.���ݿ�������Ϣ�ͷ�����������Ϣ
void GetDBAndFtpInfo();
//16.ģ�͹���ͼ����
void funcMdlDrawing();
//16.ģ�͹���ͼ����
void funcMdlDrawingSet();
//17.��ģ�Ϳ������ģ�ͣ�����װ��
void funcAssemblyFromMdl();
//18.�滻������װ�䣬
void funcAssemblyFromLocalMdl();
//19.����ģ�ͱ��͹���
void funcMdlTest();
//20.��װ��ɵĲ�Ʒ������ģ�����
void funcProductShenHe();
//////////////////////////////////////////////////////////////////////////
//21.����
void TestFun();
//22.ȫ�ֵ�ͼ�����ã�
void SetGImageList();
//23.��������Ԫģʽ
 void funcFemEnter();
//24.������Ԫģ��
 void funcFemOpen();
//25.��������Ԫģ��
void funcFemSave();

//�õ��Ĺ�������
bool CreatDefDtm();
ProError GemitemAction(ProGeomitem * p_handle,ProAppData geomitem);
void UsrGeomitemPlnGet(ProFeature feature,ProGeomitem *geomitem);
int UsrCreatDefDtm(ProDtmplnConstrType pro_dtmpln_def,ProName dtm_name,ProGeomitem *pln);


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

// CMainApp

BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
	//ON_COMMAND(ID_PART_RENAME, OnTypeRename)
END_MESSAGE_MAP()


// CMainApp �ṹ

CMainApp::CMainApp()
{
	// TODO: �ڴ���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	//SetGImageList();
}


// Ψһ�� CMainApp ����

CMainApp theApp;


// CMainApp ��ʼ��

BOOL CMainApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
    AfxEnableControlContainer();

	//if (!AfxOleInit())
	//{
	//	//AfxMessageBox(IDP_OLE_INIT_FAILED);
	//	AfxMessageBox("Ole��ʼ��ʧ�ܣ�");
	//	return FALSE;
	//}

	//-----------------------------wshf------------------------------
	CString wshfstrFtp,wshfstrFtpUser,wshfstrFtpPwd;
	int wshfiFtpPort;
	CString wshfstrPdmDb;
	wshfUsrGetDBAndFtpInfo(wshfstrFtp,wshfstrFtpUser,wshfstrFtpPwd,wshfiFtpPort,wshfstrDbOpen,wshfstrPdmDb);

	wshfm_db.Begin();
	int status=0;
	status=wshfm_db.Open(wshfstrDbOpen);

	if(status==0)
	{
		AfxMessageBox("���ݿ����Ӵ��󣬳��򽫹ر�");
		return FALSE;
	}





	//---------------------------------------------------------------

	return TRUE;
}

//dll��ʼ������
extern "C" int user_initialize(int argc,char *argv[])
{
	ProError         status;
	ProName          Msg;
	ProSolid         solid;
	//��õ�ǰģ�͵�����
    status=ProMdlCurrentGet((ProMdl*)&solid);

	uiCmdCmdId  uiCmdMdlDef,uiCmdParaButton,uiCmdSamplePicButton,uiCmdMdlManage,
		        uiCmdPartManageButton,uiCmdProductManageButton,uiCmdOptionButton,uiCmdProduceShenheButton,uiCmdDrawingSetButton,
				//,uiCmdNew1,/*uiCmdNew2,*//*uiCmdOpen1,*/uiCmdOpen2,*/
				uiCmdSave1,uiCmdSave2,/*uiCmdAsmParaDefButton,*/uiCmdAsmParaDefButton2,uiCmdDrawingButton,uiCmdAssemblyButton,uiCmdMdlTestButton;
	uiCmdCmdId  uiCmdFemEnter,uiCmdFemOpen,uiCmdFemSave;
	
    ProMenuItemIcon cmd_icon ;
    //������Ϣ�ļ�
	ProStringToWstring(Msg,"message_mdl.txt");
	//��Ӳ˵���
    status=ProMenubarMenuAdd("MdlMenu","��ģ","Utilities",PRO_B_FALSE,Msg);


	//���ģ�͹���
	status=ProCmdActionAdd("ģ���ļ���������",(uiCmdCmdActFn)funcMdlManage,
		uiCmdPrioDefault,UsrAccessDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdMdlManage);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","ģ���ļ�����","ģ���ļ�����",
		"�Թ�������ģ�Ϳ⡢�������������Լ������ļ����й���",NULL,PRO_B_TRUE,uiCmdMdlManage,Msg);

	
	status=ProMenubarmenuMenuAdd("MdlMenu","Sav","���浱ǰģ��",NULL,PRO_B_TRUE ,Msg);

																		//status=ProCmdActionAdd("save1",(uiCmdCmdActFn)save1,
																		//	uiCmdPrioDefault,UsrAccessCommonDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdSave1);	
																		//�õ�Pro/ENGINEER���������
	status=ProCmdCmdIdFind("ProCmdModelSave",&uiCmdSave1);
	status=ProCmdActionAdd("save2",(uiCmdCmdActFn)save2,
		uiCmdPrioDefault,UsrAccessCommonDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdSave2);

	status=ProMenubarmenuPushbuttonAdd("Sav","�����ļ�ϵͳ","�����ļ�ϵͳ",
		"��ģ�ͱ��浽����Ŀ¼����",NULL,PRO_B_TRUE,uiCmdSave1,Msg);
	status=ProMenubarmenuPushbuttonAdd("Sav","ģ�͹�����","ģ�͹�����",
		"��ģ�ͱ��浽ģ�͹�����",NULL,PRO_B_TRUE,uiCmdSave2,Msg);


	//ģ�� ����
	status=ProCmdActionAdd("������Ϣ��������",(uiCmdCmdActFn)MdlDef,
		uiCmdPrioDefault,UsrAccessCommonDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdMdlDef);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","������Ϣ����","������Ϣ����",
	"���嵱ǰģ�͵Ļ�����Ϣ",NULL,PRO_B_TRUE,uiCmdMdlDef,Msg);
	sprintf(cmd_icon,"jbxx.gif");
	status=ProCmdIconSet(uiCmdMdlDef,cmd_icon);
	status = ProCmdDesignate (uiCmdMdlDef,"������Ϣ����","���嵱ǰģ�͵Ļ�����Ϣ","���嵱ǰģ�͵Ļ�����Ϣ",	Msg);
	
	//�������Ա���ť��
	status=ProCmdActionAdd("�����Ϣ��ģ����",(uiCmdCmdActFn)funcParaTalbe,
		uiCmdPrioDefault,UsrAccessPartDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdParaButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","�����Ϣ��ģ","�����Ϣ��ģ",
		"����ģ�͵��������Ա�͹�ϵʽ��Ϣ",NULL,PRO_B_TRUE,uiCmdParaButton,Msg);
	sprintf(cmd_icon,"swtxrel.gif");
	status=ProCmdIconSet(uiCmdParaButton,cmd_icon);	
	status = ProCmdDesignate (uiCmdParaButton,"�����Ϣ��ģ","����ģ�͵��������Ա�͹�ϵʽ��Ϣ","����ģ�͵��������Ա�͹�ϵʽ��Ϣ",	Msg);

																					//���װ���������Ա�͹�ϵ����
																					//status=ProCmdActionAdd("װ����Ϣ��ģ����",(uiCmdCmdActFn)AsmParaDefAcFn,
																					//	uiCmdPrioDefault,AccessAvailable,
																					//	PRO_B_TRUE,PRO_B_TRUE,&uiCmdAsmParaDefButton);
																					//status=ProMenubarmenuPushbuttonAdd("MdlMenu", "װ����Ϣ��ģ", "װ����Ϣ��ģ",
																					//	"����ģ�͵���ɣ�ģ�͵��������Ա�͹�ϵʽ��Ϣ", NULL,
																					//	PRO_B_TRUE, uiCmdAsmParaDefButton, Msg);  

	status=ProCmdActionAdd("װ����Ϣ��ģ����",(uiCmdCmdActFn)AsmParaDefAcFn2,
		uiCmdPrioDefault,AccessAvailable,
		PRO_B_TRUE,PRO_B_TRUE,&uiCmdAsmParaDefButton2);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu", "װ����Ϣ��ģ", "װ����Ϣ��ģ",
		"����ģ�͵���ɣ�ģ�͵��������Ա�͹�ϵʽ��Ϣ", NULL,
		PRO_B_TRUE, uiCmdAsmParaDefButton2, Msg);  
	sprintf(cmd_icon,"swtxrel.gif");
	status=ProCmdIconSet(uiCmdAsmParaDefButton2,cmd_icon);
    status = ProCmdDesignate (uiCmdAsmParaDefButton2,"װ����Ϣ��ģ","����ģ�͵���ɣ�ģ�͵��������Ա�͹�ϵʽ��Ϣ","����ģ�͵���ɣ�ģ�͵��������Ա�͹�ϵʽ��Ϣ",	Msg);

	//ģ�͹���ͼ
	status=ProCmdActionAdd("ģ�͹���ͼ����",(uiCmdCmdActFn)funcMdlDrawing,
		uiCmdPrioDefault,UsrAccessCommonDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdDrawingButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","ģ�͹���ͼֽ","ģ�͹���ͼֽ",
		"��ģ�͵Ĺ���ͼ���й���",NULL,PRO_B_TRUE,uiCmdDrawingButton,Msg);
	sprintf(cmd_icon,"mxgct.gif");
	status=ProCmdIconSet(uiCmdDrawingButton,cmd_icon);
    status = ProCmdDesignate (uiCmdDrawingButton,"ģ�͹���ͼֽ","��ģ�͵Ĺ���ͼ���й���","��ģ�͵Ĺ���ͼ���й���",Msg);

	//��ͼ����������ť��
	status=ProCmdActionAdd("��ͼ��������",(uiCmdCmdActFn)funSamplePic,
		uiCmdPrioDefault,UsrAccessForPicMake,PRO_B_TRUE,PRO_B_TRUE,&uiCmdSamplePicButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","ģ����ͼ����","ģ����ͼ����",
		"�ӵ�ǰģ���У������������Ա�ָ�������Ĳ���ͼƬ",NULL,PRO_B_TRUE,uiCmdSamplePicButton,Msg);
	sprintf(cmd_icon,"picmaker.gif");
	status=ProCmdIconSet(uiCmdSamplePicButton,cmd_icon);
    status = ProCmdDesignate (uiCmdSamplePicButton,"ģ����ͼ����","�ӵ�ǰģ���У������������Ա�ָ�������Ĳ���ͼƬ","�ӵ�ǰģ���У������������Ա�ָ�������Ĳ���ͼƬ",Msg);

	//����ģ��
	status=ProCmdActionAdd("ģ�Ͳ�������",(uiCmdCmdActFn)funcMdlTest,
		uiCmdPrioDefault,UsrAccessCommonDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdMdlTestButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","ģ�ͱ��Ͳ���","ģ�ͱ��Ͳ���",
		"����ģ��,�Լ���ϵʽ���������Ա��Ƿ��������Ҫ��",NULL,PRO_B_TRUE,uiCmdMdlTestButton,Msg);
	sprintf(cmd_icon,"mdltest.gif");
	status=ProCmdIconSet(uiCmdMdlTestButton,cmd_icon);
    status = ProCmdDesignate (uiCmdMdlTestButton,"ģ�ͱ��Ͳ���","����ģ��,�Լ���ϵʽ���������Ա��Ƿ��������Ҫ��","����ģ��,�Լ���ϵʽ���������Ա��Ƿ��������Ҫ��",Msg);

	//���ģ�͹�����ť��
	//�������ģ��
	status=ProCmdActionAdd("�������ģ������",(uiCmdCmdActFn)funcPartManage,
		uiCmdPrioDefault,UsrAccessPartDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdPartManageButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","�������ģ��","�������ģ��",
		"������ǰģ�͵Ĳ�����Ϣ��������ģ�͵����ģ�Ϳ�",NULL,PRO_B_TRUE,uiCmdPartManageButton,Msg);
	sprintf(cmd_icon,"prtmdl.gif");
	status=ProCmdIconSet(uiCmdPartManageButton,cmd_icon);
    status = ProCmdDesignate (uiCmdPartManageButton,"�������ģ��","������ǰģ�͵Ĳ�����Ϣ��������ģ�͵����ģ�Ϳ�","������ǰģ�͵Ĳ�����Ϣ��������ģ�͵����ģ�Ϳ�",Msg);	
	
	//����װ��ģ�ͣ���ť��
	status=ProCmdActionAdd("����װ��ģ������",(uiCmdCmdActFn)funcProductManage,
		uiCmdPrioDefault,AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&uiCmdProductManageButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","����װ��ģ��","����װ��ģ��",
		"������ǰװ��ģ�͵Ĳ�����Ϣ�����װ��ģ�͵�װ��ģ�Ϳ�",NULL,PRO_B_TRUE,uiCmdProductManageButton,Msg);
	sprintf(cmd_icon,"asmmdl.gif");
	status=ProCmdIconSet(uiCmdProductManageButton,cmd_icon);
    status = ProCmdDesignate (uiCmdProductManageButton,"����װ��ģ��","������ǰװ��ģ�͵Ĳ�����Ϣ�����װ��ģ�͵�װ��ģ�Ϳ�","������ǰװ��ģ�͵Ĳ�����Ϣ�����װ��ģ�͵�װ��ģ�Ϳ�",Msg);	

	////װ���Ʒ��ˣ���ť��
	status=ProCmdActionAdd("װ���Ʒ�������",(uiCmdCmdActFn)funcProductShenHe,
		uiCmdPrioDefault,AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&uiCmdProduceShenheButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","װ���Ʒ���","װ���Ʒ���",
		"��������Ͳ���װ��ɵ��²�Ʒ�������ģ��",NULL,PRO_B_TRUE,uiCmdProduceShenheButton,Msg);

    //���ù���ͼ��ϸ��
	status=ProCmdActionAdd("��ϸ����������",(uiCmdCmdActFn)funcMdlDrawingSet,
		uiCmdPrioDefault,UsrAccessDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdDrawingSetButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","��ϸ������","��ϸ������",
		"�Թ���ͼ����ϸ���������",NULL,PRO_B_TRUE,uiCmdDrawingSetButton,Msg);
	sprintf(cmd_icon,"cadset.gif");
	status=ProCmdIconSet(uiCmdDrawingSetButton,cmd_icon);
    status = ProCmdDesignate (uiCmdDrawingSetButton,"��ϸ������","�Թ���ͼ����ϸ���������","�Թ���ͼ����ϸ���������",Msg);	

	//����Ԫģ�Ͳ����˵����
	status=ProMenubarmenuMenuAdd("MdlMenu","FemMenu","����Ԫ��ģ",NULL,PRO_B_TRUE ,Msg);

	status=ProCmdActionAdd("����Ԫģʽ����",(uiCmdCmdActFn)funcFemEnter,
		uiCmdPrioDefault,UsrAccessPartDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdFemEnter);
	status=ProMenubarmenuPushbuttonAdd("FemMenu","����Ԫģʽ","����Ԫģʽ",
		"��������Ԫģʽ����������Ԫ����",NULL,PRO_B_TRUE,uiCmdFemEnter,Msg);

	status=ProCmdActionAdd("������Ԫģ������",(uiCmdCmdActFn)funcFemOpen,
		uiCmdPrioDefault,UsrAccessPartDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdFemOpen);
	status=ProMenubarmenuPushbuttonAdd("FemMenu","������Ԫģ��","������Ԫģ��",
		"����ģ�ʹ���Ӧ������Ԫģ��",NULL,PRO_B_TRUE,uiCmdFemOpen,Msg);

	status=ProCmdActionAdd("��������Ԫģ������",(uiCmdCmdActFn)funcFemSave,
		uiCmdPrioDefault,UsrAccessPartDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdFemSave);
	status=ProMenubarmenuPushbuttonAdd("FemMenu","��������Ԫģ��","��������Ԫģ��",
		"������Ԫģ�ͱ��浽����Ԫģ�Ϳ�",NULL,PRO_B_TRUE,uiCmdFemSave,Msg);


	//ѡ�����ã���ť��
	status=ProCmdActionAdd("ѡ����������",(uiCmdCmdActFn)funcOption,
		uiCmdPrioDefault,UsrAccessDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdOptionButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","������������","������������",
		"���ݿ���������",NULL,PRO_B_TRUE,uiCmdOptionButton,Msg);


	//���öԻ���
	dlg_asm=NULL;			//װ��ߴ硢��������ϵ
	dlg_part=NULL;			//����ߴ硢��������ϵ
	dlg_pic=NULL;			//ͼƬ
	dlg_test=NULL;
	addComp_dlg=NULL;
	dlg_asm_def_test=NULL;
    //���ݿ�������Ϣ
	//������������Ϣ
	GetDBAndFtpInfo();
    m_db=NULL;
   //����ȫ�ֵ�ͼ��
	

    //�Ի������
	//ѡ�����ã���ť��


	//uiCmdCmdId testCmd;
	//status=ProCmdActionAdd("��������",(uiCmdCmdActFn)TestFun,
	//	uiCmdPrioDefault,UsrAccessDefault,PRO_B_TRUE,PRO_B_TRUE,&testCmd);
	//status=ProMenubarmenuPushbuttonAdd("MdlMenu","����","����",
	//	"���ԶԻ���",NULL,PRO_B_TRUE,testCmd,Msg);

	//////���ù����ռ䣨��ť��
	////status=ProCmdActionAdd("�����ռ��������",(uiCmdCmdActFn)workplaceman,
	////	uiCmdPrioDefault,UsrAccessDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdWokePlaceManButton);
	////status=ProMenubarmenuPushbuttonAdd("MdlMenu","�����ռ����","�����ռ����",
	////	"��������Ŀ�Ľ�ģ�������й���",NULL,PRO_B_TRUE,uiCmdWokePlaceManButton,Msg);
	//   
	//status=ProMenubarmenuPushbuttonAdd("File","ѡ��","ѡ��",
	//	"���ݿ���������",NULL,PRO_B_TRUE,openfile,Msg);

   // ProCmdCmdIdFind("Ph_list.Filelist",&openfile);
//	ProCmdAccessFuncAdd(openfile,(uiCmdAccessFn)UsrAccessDefault,uiCmdOptionButton);
  //////////////////////////////////////////////////////////////////////////
	//status=ProMenubarMenuAdd ("AsmMenu", "װ�佨ģ",
	//	"Utilities", PRO_B_TRUE, Msg);
	////��ӻ�����Ϣ���尴ť
	//status=ProCmdActionAdd("װ��ģ�Ͷ���",(uiCmdCmdActFn)AsmDefAcFn,
	//	uiCmdPrioDefault,AccessAvailable,
	//	PRO_B_TRUE,PRO_B_TRUE,&uiCmdAsmDefButton);

	//status=ProMenubarmenuPushbuttonAdd("AsmMenu", "������Ϣ����1", "������Ϣ����1",
	//	"����ģ�͵Ļ�����Ϣ1", NULL,
	//	PRO_B_TRUE, uiCmdAsmDefButton, Msg);	
//////////////////////////////////////////////////////////////////////////
//����Ҽ��˵�


//////////////////////////////////////////////////////////////////////////


	//�������ݿ�������Ϣ

    //�ߴ�����༭�Ի�������ָ��Ϊ��
	
	//CDatabase db;
	//if(!db.OpenEx(dbLinkInfo,CDatabase::openReadOnly |CDatabase::noOdbcDialog))
	//{
	//	funcOption();
	//}
	//else
	//{
	//	CFtpAndDBInfoSet m_pSet;
	//	m_pSet.Open();		
 //       if(m_pSet.IsBOF()) 
	//	{
 //           m_pSet.Close();
	//		funcOption();		
	//	}
	//	else
	//	{
	//		m_pSet.MoveFirst();
	//		myFtp.strFtp.Format("%d.%d.%d.%d",m_pSet.m_ip0,m_pSet.m_ip1,m_pSet.m_ip2,m_pSet.m_ip3); //"10.15.43.134";
	//		myFtp.strFtpUser=m_pSet.m_FtpUsr;
	//		myFtp.strFtpPwd=m_pSet.m_FtpPwd;
	//		myFtp.iFtpPort=m_pSet.m_Port;
	//		m_pSet.Close();
	//		
	//	}	

	//}  

	//����Proeԭ���Ĵ򿪰�ť
	//,openfile
	//uiCmdAccessId access_id;
	//ProCmdCmdIdFind("ProCmdModelOpen",&openfile);
	//ProCmdAccessFuncAdd(openfile,(uiCmdAccessFn)UsrAccessUNAVAILABLE,&access_id);
	///*ProCmdActionAdd("ProCmdModelOpen",(uiCmdCmdActFn)funSamplePic,
	//uiCmdPrioDefault,UsrAccessUNAVAILABLE,PRO_B_TRUE,PRO_B_TRUE,&openfile);*/
	//,openfile


	//װ�䰴ť����,����ԭ����װ�䰴ť�����һ��װ�䰴ť��֧�ִ�ģ���н���װ��Ĺ���
  
	status=ProCmdActionAdd("װ������",(uiCmdCmdActFn)funcAssemblyFromMdl,
		uiCmdPrioDefault,UsrAccessAVAILABLE_ASSEMBLY_MODE,PRO_B_TRUE,PRO_B_TRUE,&uiCmdAssemblyButton);
	status=ProMenubarmenuPushbuttonAdd("Insert.cb_component","ģ��","ģ��",
		"�ӱ�����Ƶ�ģ���л���㲿������װ��","pshCompAssemble",PRO_B_FALSE,uiCmdAssemblyButton,Msg);
	sprintf(cmd_icon,"compAssemblyFromMdl.gif");
	status=ProCmdIconSet(uiCmdAssemblyButton,cmd_icon);
    status = ProCmdDesignate (uiCmdAssemblyButton,"ģ��","�ӱ�����Ƶ�ģ���л���㲿������װ��","�ӱ�����Ƶ�ģ���л���㲿������װ��",Msg);

	UserPopupmenusSetup();
	return 0;
}
//��������
extern "C" void  user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    p_UsrSWTXTableList.RemoveAll(); 
	//p_UsrRelTableList.RemoveAll();
	p_UsrPicture.RemoveAll();                                                               
	p_UsrPerfParaTableList.RemoveAll();
	p_UsrPartPorp.RemoveAll();  
	if (dlg_part!=NULL)		delete dlg_part;
	if (dlg_asm!=NULL)		delete dlg_asm;
    if (dlg_pic!=NULL)		delete dlg_pic;
	if (dlg_test!=NULL)		delete dlg_test;
	if (addComp_dlg!=NULL)	delete addComp_dlg;
	if (dlg_asm_def_test!=NULL)		delete dlg_asm_def_test;
	if(testdlg!=NULL) delete testdlg;
}

/************************************************************************/
/* ȫ���½�                                                             */
/************************************************************************/
void new1()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	CNewDlg			dlg;
	ProMdl			model;
	ProMdldata		data;
	int				w_id;
	if(dlg.DoModal()==IDOK)
	{
		if (ProMdlCurrentGet((ProMdl*)&model)==PRO_TK_NO_ERROR) {
			ProMdlDataGet(model,&data);
			if (data.type==PRO_MDL_PART || data.type==PRO_MDL_ASSEMBLY)
			{
				CreatDefDtm();
			}
		}
	}
	else
	{
		if(ProWindowCurrentGet(&w_id)==PRO_TK_NO_ERROR){

			ProWindowActivate(w_id);
		}
	}


}
/************************************************************************/
/* �̳��½�                                                             */
/************************************************************************/
void new2()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//CNewFromMdlDlg  dlg;
	//dlg.DoModal();
}
/************************************************************************/
/* �򿪱���                                                             */
/************************************************************************/
void open1()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	CString			ext1;
	ProPath			name;
	ProMdl			mdl;
	ProName			w_name;
	ProType			type;
	ProError		status;
	int				dot;
	int				w_id;
	CString			path;
	CFileDialog		dlg(TRUE,"prt","*.prt*;*.asm*;*.drw*"
		,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST
		,"PRO/ENGINEET �ļ�(*.prt *.asm *.drw)|*.prt*;*.asm*;*.drw*||");

	if (dlg.DoModal()==IDOK) {
		dot=dlg.GetPathName().Find('.',dlg.GetPathName().GetLength()-2);
		if (dot!=-1) {
			path=dlg.GetPathName().Left(dlg.GetPathName().GetLength()-2);
		}
		else
		{
			dot=dlg.GetPathName().Find('.',dlg.GetPathName().GetLength()-3);
			if (dot!=-1) {
				path=dlg.GetPathName().Left(dlg.GetPathName().GetLength()-3);
			}
			else path=dlg.GetPathName();

		}
		ProStringToWstring(name,_T(path.GetBuffer()+'\0'));
		status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,&mdl); 
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("���ļ�ʧ��");
			return;
		}
		status = ProMdlTypeGet(mdl, (ProMdlType*)&type);
		status= ProMdlNameGet(mdl, w_name);
		status= ProObjectwindowCreate(w_name, type, &w_id);
		if(!status== PRO_TK_NO_ERROR)
		{
			return;
		}
		status = ProMdlDisplay(mdl);
		status = ProWindowActivate(w_id);
	}
	else
	{
		if(ProWindowCurrentGet(&w_id)==PRO_TK_NO_ERROR){

			ProWindowActivate(w_id);
		}
	}	
}
/************************************************************************/
/* �򿪹������ļ�                                                       */
/************************************************************************/
void funcMdlManage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	CMenuOpenDlg dlg_open;
	dlg_open.DoModal();
}
/************************************************************************/
/* ���浽����                                                           */
/************************************************************************/
void save1()
{
	ProMdl mdl;
	if(ProMdlCurrentGet((ProMdl*)&mdl)==PRO_TK_NO_ERROR)
	{
		ProMdlSave(mdl);
	}
}
/************************************************************************/
/* ���浽��������                                                       */
/************************************************************************/
void save2()
{
	////workplaceman();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();

	CTempWorkPlaceDlg dlg;
	ProMdl            mdl;
	ProMdlType        type; 
	ProError          status;
	status=ProMdlCurrentGet((ProMdl*)&mdl);
    status=ProMdlTypeGet(mdl,&type);
	switch(type) {
	case PRO_MDL_PART:
		dlg.SetMode(type);
		break;
	case PRO_MDL_ASSEMBLY:
		dlg.SetMode(type);
		break;
	default:
		dlg.SetMode(PRO_MDL_UNUSED);
		break;
	}
	dlg.DoModal();
}
/************************************************************************/
/* �����ռ����                                                         */
/************************************************************************/
void workplaceman()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	//CWorkPlaceManDlg dlg;
	//dlg.DoModal();
}
/************************************************************************/
/* ģ�ͻ�����Ϣ����                                                     */
/************************************************************************/
void MdlDef()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	ProSolid		solid;
	MdlPropDefDlg	dlg;
	if(ProMdlCurrentGet((ProMdl*)&solid)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ǰ������ģ�ͣ�");
		return;
	}	
	dlg.DoModal();
	UsrActivateCurrentWindow();
}
/************************************************************************/
/* ����������ߴ硢��ϵ                                                 */
/************************************************************************/
void funcParaTalbe()//��������
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	if (dlg_part==NULL) {
		dlg_part=new CRelParaEditDlg();
		dlg_part->Create(IDD_DIALOG1);
		dlg_part->ShowWindow(SW_SHOW);
	}
	else
	{
		delete dlg_part;
		dlg_part=new CRelParaEditDlg();
		dlg_part->Create(IDD_DIALOG1);
		dlg_part->ShowWindow(SW_SHOW);
	}
	UsrActivateCurrentWindow();
	return;
}
/************************************************************************/
/* װ��������ߴ硢��ϵ                                                 */
/************************************************************************/
void AsmParaDefAcFn()
{   
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	if (dlg_asm==NULL) {
		dlg_asm=new CAsmParaDefDlg();
		dlg_asm->Create(IDD_ASM1);
		dlg_asm->ShowWindow(SW_SHOW);
	}
	else
	{
		delete dlg_asm;
		dlg_asm=new CAsmParaDefDlg();
		dlg_asm->Create(IDD_ASM1);
		dlg_asm->ShowWindow(SW_SHOW);
	}
	UsrActivateCurrentWindow();
}
void AsmParaDefAcFn2()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	//dlg_test.DoModal();
	if (dlg_asm_def_test==NULL) {
		dlg_asm_def_test=new CAsmParaDefDlg2();
		//ProMdlCurrentGet((ProMdl*)&(dlg_test->solid));
		dlg_asm_def_test->Create(IDD_ASM_DEF);
		dlg_asm_def_test->ShowWindow(SW_SHOW);
	}
	else
	{
		delete dlg_asm_def_test;
		dlg_asm_def_test=new CAsmParaDefDlg2();
		//ProMdlCurrentGet((ProMdl*)&(dlg_test->solid));
		dlg_asm_def_test->Create(IDD_ASM_DEF);
		dlg_asm_def_test->ShowWindow(SW_SHOW);
	}
	UsrActivateCurrentWindow();
}
/************************************************************************/
/*//��ͼ��������                                                        */
/************************************************************************/
void funSamplePic()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
    //���������Ҫ��ȫ�ֱ���������
    ProPath			path;
	CString			csPath;
	ProDirectoryCurrentGet(path);
	csPath=CString(path);
	if (dlg_pic==NULL) {
		dlg_pic=new CPicMakgeDlg();
		dlg_pic->SetPath(csPath);
		dlg_pic->Create(IDD_DIALOG4);
		dlg_pic->ShowWindow(SW_SHOW);
	}
	else
	{
		delete dlg_pic;
		dlg_pic=new CPicMakgeDlg();
		dlg_pic->SetPath(csPath);
		dlg_pic->Create(IDD_DIALOG4);
		dlg_pic->m_nStartX=0;
		dlg_pic->m_nStartY=0;
		dlg_pic->m_nEndX=0;
		dlg_pic->m_nEndY=0;
		dlg_pic->ShowWindow(SW_SHOW);
	}
	UsrActivateCurrentWindow();
}
/************************************************************************/
/* �������ģ�ͺ���                                                     */
/************************************************************************/
void funcPartManage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
    CPartMainDlg		part_dlg;
	part_dlg.DoModal();
	UsrActivateCurrentWindow();
}
/************************************************************************/
/* ����װ��ģ�ͺ���                                                     */
/************************************************************************/
void funcProductManage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	CAsmMdlManDlg		asm_man_dlg;
	asm_man_dlg.DoModal();	
	UsrActivateCurrentWindow();
}
//ѡ�����ú���,�������ݿ�������Ϣ�ͷ�����������Ϣ
void funcOption()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	COptionSetDlg		option_dlg;
	option_dlg.DoModal();
	UsrActivateCurrentWindow();
}
//19.����ģ�ͱ��͹���
void funcMdlTest()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	if (dlg_test==NULL) {
		dlg_test=new CTestRelDlg();
		ProMdlCurrentGet((ProMdl*)&(dlg_test->solid));
		dlg_test->Create(IDD_TEST_VARIANT);
		dlg_test->ShowWindow(SW_SHOW);
	}
	else
	{
		delete dlg_test;
		dlg_test=new CTestRelDlg();
		ProMdlCurrentGet((ProMdl*)&(dlg_test->solid));
		dlg_test->Create(IDD_TEST_VARIANT);
		dlg_test->ShowWindow(SW_SHOW);
	}
	UsrActivateCurrentWindow();
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
	/*TCHAR     *ReWorkDirectory;*/
	char      content_add[256]="",content_user[256]="",content_pwd[256]="";
	char      content_dsn_user[256]="",content_dsn_pwd[256]="",content_dsn_name[256]="";
	char      content_pdm_dsn_user[256]="",content_pdm_dsn_pwd[256]="",content_pdm_dsn_name[256]="";
	char      content_shujuyuan[256]="";
	int       content_port[256];
  /*  char      content_work_directory[256]="";*/

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
	/*ReWorkDirectory="work_directory";*/
	DWORD dwLength1,dwLength2,dwLength3,dwLength4,dwLength5,dwLength6,dwLength7,dwLength8,dwLength9,dwLength10,dwLength11/*,dwLength12*/;


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

			////����Ŀ¼
			//if(RegQueryValueEx(hKey,ReWorkDirectory,NULL,&dwType,(unsigned char *)content_work_directory,&dwLength12)!=ERROR_SUCCESS) content_work_directory[0]='\0';

			RegCloseKey(hKey);

			myFtp.strFtp=CString(content_add);
			myFtp.strFtpUser=CString(content_user);         
			myFtp.strFtpPwd=CString(content_pwd);
			myFtp.iFtpPort=content_port[0];
			dbLinkInfo=_T("DSN="+CString(content_shujuyuan)+";UID="+CString(content_dsn_user)+";PWD="+CString(content_dsn_pwd)+";DATABASE=RuiLiMdlDb;");
			dbPdmLinkInfo=_T("DSN="+CString(content_pdm_dsn_name)+";UID="+CString(content_pdm_dsn_user)+";PWD="+CString(content_pdm_dsn_pwd)+";");
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
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
			COptionSetDlg option_dlg;
			option_dlg.DoModal();
		}

	}
	catch (...) {
		AfxMessageBox("��ע����ȡ����ʧ��");
	}

}
/************************************************************************/
/* װ��ͼ                                                               */
/************************************************************************/
void funcMdlDrawing()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	CDrawingDlg dlg;
	int window_id;
	INT_PTR iReturnValue=dlg.DoModal();
	if(iReturnValue!=IDCANCEL)
	{
		ProWindowCurrentGet(&window_id);
		ProWindowActivate(window_id);
	}
}
/************************************************************************/
/* װ���㲿������ģ����ѡȡ                                             */
/************************************************************************/
void funcAssemblyFromMdl()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	//CAsmAddComplg addComp_dlg;
	//addComp_dlg.DoModal();
	//UsrActivateCurrentWindow();	
	if (addComp_dlg==NULL) {
		addComp_dlg=new CAsmAddComplg();
		addComp_dlg->Create(IDD_ASM_ADD_COMP);
		addComp_dlg->ShowWindow(SW_SHOW);
	}
	else
	{
		delete addComp_dlg;
		addComp_dlg=new CAsmAddComplg();
		addComp_dlg->Create(IDD_ASM_ADD_COMP);
		addComp_dlg->ShowWindow(SW_SHOW);
	}
	/*addComp_dlg->m_Tab.SetCurSel(0);*/
	UsrActivateCurrentWindow();
}
//#include "asmopen.h"
//#include "PartMdlOpen.h"
//#include "PartMdlRecordSet.h"
//#include "2DrawingDlg.h"
//extern "C" __declspec(dllimport) BOOL AddAsmInterface(CArray<Info,Info> *Info_struc);
void TestFun()
{
	//Info  temp;
	//CArray<Info,Info>  tempo;
	//temp.solid=(ProSolid)GetCurrentMdl();
	//temp.SolidID= GetMdlInfo(GetCurrentMdl()).Number;
	//temp.Type="prt";
	//tempo.Add(temp);
	//AddAsmInterface(&tempo);
	//UsrClearMdlInfo();
	//////dlg_test.DoModal();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (testdlg==NULL) {
		testdlg=new C2DrawingDlg();
		//ProMdlCurrentGet((ProMdl*)&(dlg_test->solid));
		testdlg->Create(IDD_DIALOG_2DDRAW);
		testdlg->ShowWindow(SW_SHOW);
	}
	else
	{
		delete testdlg;
		testdlg=new C2DrawingDlg();
		//ProMdlCurrentGet((ProMdl*)&(dlg_test->solid));
		testdlg->Create(IDD_DIALOG_2DDRAW);
		testdlg->ShowWindow(SW_SHOW);
	}
	UsrActivateCurrentWindow();

	//CPartMdlRecordSet m_set;
	//m_set.Open();
	//CAsmOpen m_open;
	//m_open.Open(m_set.m_pDatabase,16);
	//m_open.OpenPart(m_set.m_pDatabase,9);
	
}

//////////////////////////////////////////////////////////////////////////
//�Ҽ��˵�,ֱ�Ӵ��ּ��ѳߴ�����������Ա�
void AddTo()
{
	ProSelection	*ret_buff; 
	ProError		status;
	int				num=0;
	ProModelitem	p_mdl_item ;
	ProSolid		solid;
	status=ProSelbufferSelectionsGet ( &ret_buff); 
	status=ProArraySizeGet((ProArray)ret_buff,&num);
	for (int i=0;i<num;i++)
	{
		status=ProSelectionModelitemGet (ret_buff[i],&p_mdl_item);
		if (status==PRO_TK_NO_ERROR )
		{
			if (p_mdl_item.type==PRO_DIMENSION )
			{
				int flag=0;
				status=ProMdlCurrentGet((ProMdl*)&solid);
				switch(GetMdlType(solid)) {
				case PRO_MDL_PART:
					
					for(int j=0;j<p_UsrSWTXTableList.GetCount();j++)
					{
						if(p_UsrSWTXTableList[j].owner!=(ProMdl)solid) continue;
						if(p_UsrSWTXTableList[j].isCheck!=1) continue;
						if(p_UsrSWTXTableList[j].type!="dim") continue;
						if (ComparaDimension(&p_UsrSWTXTableList[j].Dim,(ProDimension*)&p_mdl_item))
						{
							flag=1;
							break;
						}
					}
					//�ж��Ƿ��ڹ�ϵʽ�����
					if (!g_RelCollection.IsDimParaInRelSetLeftPart(solid,NULL,(ProDimension*)&p_mdl_item))
					{
						flag=1;
						AfxMessageBox("��ѡ��ĳߴ��Ѿ��ڹ�ϵʽ�����,���ܹ���Ϊ�������Ա������!");
					}
					if(flag==0) 
					{
						UsrSWTXTable1 temp_UsrSWTXTable(solid,(ProDimension*)&p_mdl_item);
						p_UsrSWTXTableList.Add(temp_UsrSWTXTable);        //��ӵ�������
					}
					break;
				case PRO_MDL_ASSEMBLY:
					//�ж��Ƿ��Ѿ�����
					for (int j=0;j<(int)p_AsmSmlItemList.GetCount();j++) {
						if(p_AsmSmlItemList[j].marsk!=TYPE_DIM) continue;	
						if(p_AsmSmlItemList[j].owner!=solid)    continue;
						if (ComparaDimension((ProDimension*)&p_mdl_item,&p_AsmSmlItemList[j].dim)) 
						{
							flag=1;
							break;
						}
					}
					if (!g_RelCollection.IsDimParaInRelSetLeftPart((ProSolid)p_mdl_item.owner,NULL,(ProDimension*)&p_mdl_item))
					{
						flag=1;
						AfxMessageBox("��ѡ��ĳߴ��Ѿ��ڹ�ϵʽ�����,���ܹ���Ϊ�������Ա������!");
					}
					if(flag==0)
					{//��ѡ�еĳߴ�����������Ա�
						AsmSmlItem tempAsm;
						ProName    symbol;
						int        postId;
						CString    temp;
						tempAsm.owner=solid;
						tempAsm.marsk=TYPE_DIM;
						ProDimensionSymbolGet((ProDimension*)&p_mdl_item,symbol);
						ProSolidToPostfixId((ProSolid)p_mdl_item.owner,&postId);
						temp=CString(symbol);
						if (temp=="") temp.Format("d%d:d",p_mdl_item.id,postId);
						else temp.Format("%s:%d",temp,postId);
						tempAsm.name=temp;
						tempAsm.dim=p_mdl_item;
						p_AsmSmlItemList.Add(tempAsm);
					}					

					break;
				default:
					break;
				}				

			}
		}
	}
	status=ProArrayFree((ProArray*)&ret_buff);

}
ProError UserPopupmenuNotification (ProMenuName name)
{
	ProPopupMenuId	menu_id;
	uiCmdCmdId		cmd_id;
	ProError		status;
	ProLine			label;
	ProLine			help;
	ProFileName      Msg;
	ProSelection    *ret_buff; 
	int				num=0;
	ProModelitem	p_mdl_item ;
	//�жϵ�ǰ��û��ѡ��
	status=ProSelbufferSelectionsGet ( &ret_buff);
	if (status!=PRO_TK_NO_ERROR) return PRO_TK_NO_ERROR;
	status=ProArraySizeGet((ProArray)ret_buff,&num);
	if (num==1)
	{
		status=ProSelectionModelitemGet (ret_buff[0],&p_mdl_item);
		if (p_mdl_item.type!=PRO_DIMENSION) return PRO_TK_NO_ERROR;
		ProStringToWstring(Msg,"message_mdl.txt");
		/*--------------------------------------------------------------------*\
		Check if the menu being created matches the menu name we want.
		\*--------------------------------------------------------------------*/
		ProPopupmenuIdGet (name, &menu_id);

		if (strcmp (name, "Edit Modify Menu") == 0)
		{
			status = ProCmdActionAdd ("�����������Ա�����",
				(uiCmdCmdActFn)AddTo,
				uiProe2ndImmediate, 
				UsrAccessDefault,
				PRO_B_FALSE, PRO_B_FALSE, &cmd_id);
			ProStringToWstring(label,"���������Ա�");
			ProStringToWstring(help,"��ѡ�еĳߴ����ģ�͵��������Ա�");
			/*--------------------------------------------------------------------*\
			Add the button to the end of the popup menu.
			\*--------------------------------------------------------------------*/
			status = ProPopupmenuButtonAdd (menu_id, PRO_VALUE_UNUSED,
				"���",
				label, help,
				cmd_id,
				NULL,
				NULL);
		}
	}
	status=ProArrayFree((ProArray*)&ret_buff);
	return PRO_TK_NO_ERROR;
}
int UserPopupmenusSetup()
{
	ProError status;
	//uiCmdCmdId cmd_id;
	//ProName          Msg;
	//ProStringToWstring(Msg,"message_mdl.txt");

	//status = ProMenubarmenuPushbuttonAdd ("EditModify",
	//	"�����������Ա�",
	//	"�����������Ա�",
	//	"���뵽�������Ա�",
	//	NULL, PRO_B_TRUE, 
	//	cmd_id, Msg);
	status = ProNotificationSet (PRO_POPUPMENU_CREATE_POST,
		(ProFunction)UserPopupmenuNotification);
   return 0;
}
//////////////////////////////////////////////////////////////////////////
//��������ȫ��ͼ��ĺ���
void SetGImageList()
{
	g_ImageList1.Create(14, 14, ILC_COLOR32, 4, 4);
	g_ImageList1.Add(AfxGetApp()->LoadIcon(IDI_ICON1));  //UsrIconEnum.USR_ICON_TYPE
	g_ImageList1.Add(AfxGetApp()->LoadIcon(IDI_PRT));    //UsrIconEnum.USR_ICON_PRTMDL
	g_ImageList1.Add(AfxGetApp()->LoadIcon(IDI_PRT));    //UsrIconEnum.USR_ICON_ASMMDL
}
//20.��װ��ɵĲ�Ʒ������ģ�����
//�˺������ñ���ģ���DLL�ڵĺ���
extern "C" __declspec(dllimport) BOOL AddAsmInterface(CArray<Info,Info> *Info_struc);
void funcProductShenHe()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProSolid  solid=(ProSolid)GetCurrentMdl();
	CArray<ProSolid,ProSolid> p_solidlist;
	p_solidlist.Add(solid);
	GetAsmCompHand(solid,&p_solidlist);	
	CArray<Info,Info>  tempo;
 	for(int i=0;i<p_solidlist.GetCount();i++)
	{
		Info  temp;
		temp.solid=p_solidlist[i];
		if (i==0)
		{
			temp.SolidID=-1;
		}
		else
		{
			temp.SolidID=GetMdlInfo(temp.solid).Number;
		}		
		temp.Type=GetMdlType(p_solidlist[i])==PRO_MDL_PART?"prt":"asm";
		tempo.Add(temp);
		//AfxMessageBox(GetMdlInfo(p_solidlist[i]).Name);
	}
	//HINSTANCE hIntance;
	//typedef BOOL (AddAsmInterface)(CArray<Info,Info> * Info_struc);
	//AddAsmInterface * pFunction;
	//VERIFY(hIntance=::LoadLibrary("D:\\Program Files\\proeWildfire 2.0\\bin\\variantdesign.dll"));

	//VERIFY(pFunction=(AddAsmInterface *)::GetProcAddress(hIntance,"AddAsmInterface"));

	//(*pFunction)(&tempo);
	//VERIFY(FreeLibrary(hIntance));
	AddAsmInterface(&tempo);
}

//23.��������Ԫģʽ
void funcFemEnter()
{
	ProError  status;
	ProMacro  w_macro;
	CString strMacro; 

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
}
//24.������Ԫģ��
#include "FemOpenDlg.h"
void funcFemOpen()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CFemOpenDlg  dlg;
	dlg.DoModal();
	/*UsrActivateCurrentWindow();*/
}
#include "FemSaveDlg.h"
//25.��������Ԫģ��
void funcFemSave()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CFemSaveDlg  dlg;
	dlg.DoModal();
	//UsrActivateCurrentWindow();
}
void funcMdlDrawingSet()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (testdlg==NULL) {
		testdlg=new C2DrawingDlg();
		//ProMdlCurrentGet((ProMdl*)&(dlg_test->solid));
		testdlg->Create(IDD_DIALOG_2DDRAW);
		testdlg->ShowWindow(SW_SHOW);
	}
	else
	{
		delete testdlg;
		testdlg=new C2DrawingDlg();
		//ProMdlCurrentGet((ProMdl*)&(dlg_test->solid));
		testdlg->Create(IDD_DIALOG_2DDRAW);
		testdlg->ShowWindow(SW_SHOW);
	}
	UsrActivateCurrentWindow();
}