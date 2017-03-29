// variantdesign.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "variantdesign.h"

//���PROEͷ�ļ�
#include "ProMenu.h"
#include "ProUtil.h"
#include "ProMenubar.h"
#include <ProMessage.h>
#include "ProArray.h"
#include <ProNotify.h>
#include "MyDatabase.h"
#include "PartAlterDlg.h"
#include <ProPopupmenu.h>
#include <ProSelbuffer.h>
#include <ProFeature.h>
#include "ProFeatType.h"
#include "PdtSaveAsDlg.h"
#include "ProMdl.h"
#include "CommonFuncClass.h"
#include "DetailInfoClass.h"
#include "PrtNewDlg.h"
#include "ProDrawing.h"
#include "ProWindows.h"
#include "2DrawingDlg.h"
#include "PrtAsmAuditDlg.h"
#include "UploadDlg.h"
#include "DetailDlg.h"
#include "IntDlg.h"
#include "PdtSaveAsDlg.h"
#include "ProductClass.h"
#include "AddToPDMDlg.h"
#include "ProjectDlg.h"
#include "UserDlg.h"
#include "ProMenu.h"

CString g_ConnectString;
CArray <Info,Info> Info_struc;
CArray<StandardInfo,StandardInfo> StandardInfo_struc;
extern CArray <FilePath,FilePath> FilePath_struc;
extern CPtrArray p_classarray;
extern CPrtAsmAuditDlg *prtasmauditdlg;
C2DrawingDlg * drawingDlg=NULL;
AddToPDMDlg *pdmdlg=NULL;
FtpInfo Ftp_struc; 
CMyDatabase m_db;
CMyDatabase m_pdm;
extern FtpInfo Ftp_struc;
extern CUploadDlg  *uploaddlg;
CDetailDlg * detaildlg=NULL;
CPartAlterDlg * partalterdlg=NULL;
uiCmdAccessId access_id;



//��ť������������
int ProductUpLoadAction();
int PartModelSearchAction();	   //���ģ�Ͳ�ѯ����
int ProductSearchAction();		   //�����ѯ����  
int PartSearchAction();			   //�����ѯ����
int AlterAction();				   //������ͺ���		   
int PartAlterAction();			   //������ͺ���
int AsmModelSearchAction();		   //���ģ�Ͳ�ѯ����
int PartSaveAction();			   //���ʵ�����溯��
int PartSaveAsAction();			   //������Ϊ����
int PartNewAction();			   //�½��������
int DisPlayDwgAction();			   //��ʾ��άͼֽ����
int SolidPlaySolidAction();        //��ʾ��άͼ�İ�ť
int CheckAction();				   //���ʵ������
int PartDeleteAction();			   //ɾ��ʵ����ť
int UserPopupmenusSetup();		   //�Ҽ���������
int ProductSaveAction();		   //��Ʒ���溯��
int ProductSaveAsAction();		   //��Ʒ���Ϊ����
int AddToPDMAction();              //��ӵ�PDM����
int EraseAction();                 //�ó��ڴ溯��
int CloseWindowsAction();		   //�رյ�ǰ�Ի���
int ProductChectAction();          //��Ʒ��˶Ի���
int ProductDeleteAction();		   //��Ʒɾ���Ի���
int OperateDwgButtonAction();      //������άͼ
int IntegrateButtonAction();       //PDM��VDS��Ӧ�Ի���
int ProductProcessButtonAction();  //��Ʒ���״̬�Ի���
int ProjAction();
int UserInfoDel();


static uiCmdAccessState AccessAvailable(uiCmdAccessMode);
static uiCmdAccessState AccessRemove (uiCmdAccessMode);
static uiCmdAccessState PartState(uiCmdAccessMode access_mode);
static uiCmdAccessState AsmState(uiCmdAccessMode access_mode);
static uiCmdAccessState DrawingState(uiCmdAccessMode access_mode);
static uiCmdAccessState SolidState(uiCmdAccessMode access_mode);

BOOL UsrGetDBAndFtpInfo(CString & strFtp,CString & strFtpUser,CString & strFtpPwd,int & iFtpPort,CString & vdLinkInfo,CString & pdmLinkInfo);



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

// CvariantdesignApp

BEGIN_MESSAGE_MAP(CvariantdesignApp, CWinApp)
END_MESSAGE_MAP()


// CvariantdesignApp �ṹ

CvariantdesignApp::CvariantdesignApp()
{
	// TODO: �ڴ���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ�� CvariantdesignApp ����

CvariantdesignApp theApp;



// CvariantdesignApp ��ʼ��

BOOL CvariantdesignApp::InitInstance()
{
	CWinApp::InitInstance();
	//AfxOleInit();

	CoInitialize(NULL);
	AfxEnableControlContainer();

	return TRUE;
}


extern "C" int user_initialize()
{
	g_ConnectString="";
	ProError       status;
	ProPath        widepath;
	CString        path;
	ProFileName    MsgFile;
	uiCmdCmdId     Product_UpLoad_cmd_id,Alter_cmd_id,ProductSearch_cmd_id,PartSearch_cmd_id,PartAlter_cmd_id;
	uiCmdCmdId     AsmModelSearch_cmd_id,PartSave_cmd_id,DisplayDwg_cmd_id,Check_cmd_id,Product_Save_cmd_id;
	uiCmdCmdId     PartSaveAs_cmd_id,Erase_cmd_id,CloseWindows_cmd_id,Product_Check_cmd_id,Product_delete_cmd_id,Product_SaveAs_cmd_id,User_DelID;
	uiCmdCmdId	   PartNew_cmd_id,DisplaySolid_cmd_id,OperaterDrw_cmd_id,PdtProcess_cmd_id,IntegrateDrw_cmd_id/*,AddToPDM_cmd_id*/,Proj_cmd_id;
	ProStringToWstring(MsgFile,"Message.txt");     //��Ϣ�ļ�
	ProMenuItemIcon cmd_icon;
	//��ʾ��Ϣ



	CString vdLinkInfo,xpdmLinkInfo;

	if(!UsrGetDBAndFtpInfo(Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort,vdLinkInfo,xpdmLinkInfo))
	{
		AfxMessageBox("��ע������");
		return PRO_TK_NO_ERROR;
	}
	if(!m_db.Open(vdLinkInfo))
	{
		return PRO_TK_NO_ERROR;
	}
	g_ConnectString=vdLinkInfo;
    //��ӱ�����Ʋ˵���
	status=ProMenubarMenuAdd("VariantDesign","DesignMenu","Utilities",PRO_B_TRUE,MsgFile);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//�����Ŀ�˵�
	ProCmdActionAdd("ProjButtonAct",(uiCmdCmdActFn)ProjAction,uiCmdPrioDefault,
		NULL,PRO_B_TRUE,PRO_B_TRUE,&Proj_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","ProjButton","ProjButton","Adding a Proj push button",
		NULL,PRO_B_TRUE,Proj_cmd_id,MsgFile);
	
	//���ע���û��˵�
	ProCmdActionAdd("UserDelete",(uiCmdCmdActFn)UserInfoDel,uiCmdPrioDefault,
		NULL,PRO_B_TRUE,PRO_B_TRUE,&User_DelID);
	ProMenubarmenuPushbuttonAdd("VariantDesign","UserDel","UserDel","UserDelBtn",
		NULL,PRO_B_TRUE,User_DelID,MsgFile);
	
	//������������ư�ť
	ProMenubarmenuMenuAdd("VariantDesign","PartSubMenu","PartSubMenu",NULL,PRO_B_TRUE,MsgFile);

	//�������½��Ӱ�ť
	ProCmdActionAdd("PartNewButtonAct",(uiCmdCmdActFn)PartNewAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&PartNew_cmd_id);
	ProMenubarmenuPushbuttonAdd("PartSubMenu","PartNewButton","PartNewButton","Adding a PartNew push button",
		NULL,PRO_B_TRUE,PartNew_cmd_id,MsgFile);

	sprintf(cmd_icon,"newpart.gif");
	status=ProCmdIconSet(PartNew_cmd_id,cmd_icon);
	status=ProCmdDesignate(PartNew_cmd_id,"PartNewButton","Adding a PartNew push button","Adding a PartNew push button",MsgFile);

	//������ʵ�����Ӱ�ť
	ProCmdActionAdd("PartSearchButtonAct",(uiCmdCmdActFn)PartSearchAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&PartSearch_cmd_id);
	ProMenubarmenuPushbuttonAdd("PartSubMenu","PartSearchButton","PartSearchButton","Adding a PartSearch push button",
		NULL,PRO_B_TRUE,PartSearch_cmd_id,MsgFile);

	sprintf(cmd_icon,"partopen.gif");
	status=ProCmdIconSet(PartSearch_cmd_id,cmd_icon);
	status=ProCmdDesignate(PartSearch_cmd_id,"PartSearchButton","Adding a PartSearch push button","Adding a PartSearch push button",MsgFile);

	//��������������Ӱ�ť
	ProCmdActionAdd("PartAlterButtonAct",(uiCmdCmdActFn)PartAlterAction,uiCmdPrioDefault,
		PartState,PRO_B_TRUE,PRO_B_TRUE,&PartAlter_cmd_id);
	ProMenubarmenuPushbuttonAdd("PartSubMenu","PartAlterButton","PartAlterButton","Adding a Alter Part push button",
		NULL,PRO_B_TRUE,PartAlter_cmd_id,MsgFile);

	sprintf(cmd_icon,"partvariant.gif");
	status=ProCmdIconSet(PartAlter_cmd_id,cmd_icon);
	status=ProCmdDesignate(PartAlter_cmd_id,"PartAlterButton","Adding a Alter Part push button","Adding a Alter Part push button",MsgFile);

	//���������Ϊ�Ӱ�ť
	ProCmdActionAdd("PartSaveAsButtonAct",(uiCmdCmdActFn)PartSaveAsAction,uiCmdPrioDefault,
		PartState,PRO_B_TRUE,PRO_B_TRUE,&PartSaveAs_cmd_id);
	ProMenubarmenuPushbuttonAdd("PartSubMenu","PartSaveAsButton","PartSaveAsButton","Adding a SaveAs Part push button",
		NULL,PRO_B_TRUE,PartSaveAs_cmd_id,MsgFile);

	sprintf(cmd_icon,"prtsaveas.gif");
	status=ProCmdIconSet(PartSaveAs_cmd_id,cmd_icon);
	status=ProCmdDesignate(PartSaveAs_cmd_id,"PartSaveAsButton","Adding a SaveAs Part push button","Adding a SaveAs Part push button",MsgFile);

	//������ʵ�������Ӱ�ť
	ProCmdActionAdd("PartSaveButtonAct",(uiCmdCmdActFn)PartSaveAction,uiCmdPrioDefault,
		PartState,PRO_B_TRUE,PRO_B_TRUE,&PartSave_cmd_id);
	ProMenubarmenuPushbuttonAdd("PartSubMenu","PartSaveButton","PartSaveButton","Adding a Save Part push button",
		NULL,PRO_B_TRUE,PartSave_cmd_id,MsgFile);

	sprintf(cmd_icon,"prtsave.gif");
	status=ProCmdIconSet(PartSave_cmd_id,cmd_icon);
	status=ProCmdDesignate(PartSave_cmd_id,"PartSaveButton","Adding a Save Part push button","Adding a Save Part push button",MsgFile);


	//����Ѿ�������ʵ��
	ProCmdActionAdd("CheckButtonAct",(uiCmdCmdActFn)CheckAction,uiCmdPrioDefault,
		PartState,PRO_B_TRUE,PRO_B_TRUE,&Check_cmd_id);
	ProMenubarmenuPushbuttonAdd("PartSubMenu","CheckButton","CheckButton","Adding a Check push button",
		NULL,PRO_B_TRUE,Check_cmd_id,MsgFile);

	sprintf(cmd_icon,"checkpart.gif");
	status=ProCmdIconSet(Check_cmd_id,cmd_icon);
	status=ProCmdDesignate(Check_cmd_id,"CheckButton","Adding a Check push button","Adding a Check push button",MsgFile);

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//��Ӳ�Ʒ������ư�ť
	ProMenubarmenuMenuAdd("VariantDesign","ProductSubMenu","ProductSubMenu",NULL,PRO_B_TRUE,MsgFile);

	//��Ʒʵ���½��Ӱ�ť
	ProCmdActionAdd("AsmModelSearchButtonAct",(uiCmdCmdActFn)AsmModelSearchAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&AsmModelSearch_cmd_id);
	ProMenubarmenuPushbuttonAdd("ProductSubMenu","AsmModelSearchButton","AsmModelSearchButton","Adding a AsmModelSearch push button",
		NULL,PRO_B_TRUE,AsmModelSearch_cmd_id,MsgFile);

	sprintf(cmd_icon,"pdtnew.gif");
	status=ProCmdIconSet(AsmModelSearch_cmd_id,cmd_icon);
	status=ProCmdDesignate(AsmModelSearch_cmd_id,"AsmModelSearchButton","Adding a AsmModelSearch push button","Adding a AsmModelSearch push button",MsgFile);


	//��Ӳ�Ʒ��ѯ�Ӱ�ť
	ProCmdActionAdd("ProductSearchButtonAct",(uiCmdCmdActFn)ProductSearchAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&ProductSearch_cmd_id);
	ProMenubarmenuPushbuttonAdd("ProductSubMenu","ProductSearchButton","ProductSearchButton","Adding a ProductSearch push button",
		NULL,PRO_B_TRUE,ProductSearch_cmd_id,MsgFile);

	sprintf(cmd_icon,"pdtopen.gif");
	status=ProCmdIconSet(ProductSearch_cmd_id,cmd_icon);
	status=ProCmdDesignate(ProductSearch_cmd_id,"ProductSearchButton","Adding a ProductSearch push button","Adding a ProductSearch push button",MsgFile);

	//���ò�Ʒ�����Ӱ�ť
	ProCmdActionAdd("AlterButtonAct",(uiCmdCmdActFn)AlterAction,uiCmdPrioDefault,
		AsmState,PRO_B_TRUE,PRO_B_TRUE,&Alter_cmd_id);
	ProMenubarmenuPushbuttonAdd("ProductSubMenu","AlterButton","AlterButton","Adding a Alter Product push button",
		NULL,PRO_B_TRUE,Alter_cmd_id,MsgFile);

	sprintf(cmd_icon,"pdtvariant.gif");
	status=ProCmdIconSet(Alter_cmd_id,cmd_icon);
	status=ProCmdDesignate(Alter_cmd_id,"AlterButton","Adding a Alter Product push button","Adding a Alter Product push button",MsgFile);

	ProCmdActionAdd("ProductSaveAsButtonAct",(uiCmdCmdActFn)ProductSaveAsAction,uiCmdPrioDefault,
		AsmState,PRO_B_TRUE,PRO_B_TRUE,&Product_SaveAs_cmd_id);
	ProMenubarmenuPushbuttonAdd("ProductSubMenu","ProductSaveAsButton","ProductSaveAsButton","Adding a Product Save As push button",
		NULL,PRO_B_TRUE,Product_SaveAs_cmd_id,MsgFile);

	sprintf(cmd_icon,"pdtsaveas.gif");
	status=ProCmdIconSet(Product_SaveAs_cmd_id,cmd_icon);
	status=ProCmdDesignate(Product_SaveAs_cmd_id,"ProductSaveAsButton","Adding a Product Save As push button","Adding a Product Save As push button",MsgFile);

	//�����Ʒ���水ť
	ProCmdActionAdd("ProductSaveButtonAct",(uiCmdCmdActFn)ProductSaveAction,uiCmdPrioDefault,
		AsmState,PRO_B_TRUE,PRO_B_TRUE,&Product_Save_cmd_id);
	ProMenubarmenuPushbuttonAdd("ProductSubMenu","ProductSaveButton","ProductSaveButton","Adding a Product Save push button",
		NULL,PRO_B_TRUE,Product_Save_cmd_id,MsgFile);

	sprintf(cmd_icon,"pdtsave.gif");
	status=ProCmdIconSet(Product_Save_cmd_id,cmd_icon);
	status=ProCmdDesignate(Product_Save_cmd_id,"ProductSaveButton","Adding a Product Save push button","Adding a Product Save push button",MsgFile);

	//��Ʒ��˰�ť
	ProCmdActionAdd("ProductChectButtonAct",(uiCmdCmdActFn)ProductChectAction,uiCmdPrioDefault,
		AsmState,PRO_B_TRUE,PRO_B_TRUE,&Product_Check_cmd_id);
	ProMenubarmenuPushbuttonAdd("ProductSubMenu","ProductCheckButton","ProductCheckButton","Adding a Product Check Product push button",
		NULL,PRO_B_TRUE,Product_Check_cmd_id,MsgFile);

	sprintf(cmd_icon,"checkpdt.gif");
	status=ProCmdIconSet(Product_Check_cmd_id,cmd_icon);
	status=ProCmdDesignate(Product_Check_cmd_id,"ProductCheckButton","Adding a Product Check Product push button","Adding a Product Check Product push button",MsgFile);

	//��Ӳ�Ʒ����Ӱ�ť
	ProCmdActionAdd("UpLoadButtonAct",(uiCmdCmdActFn)ProductUpLoadAction,uiCmdPrioDefault,
		AsmState,PRO_B_TRUE,PRO_B_TRUE,&Product_UpLoad_cmd_id);
	ProMenubarmenuPushbuttonAdd("ProductSubMenu","ProductUpLoadButton","ProductUpLoadButton","Adding a Product UpLoad Product push button",
		NULL,PRO_B_TRUE,Product_UpLoad_cmd_id,MsgFile);


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//��������
	//ProCmdActionAdd("CalculateButtonAct",(uiCmdCmdActFn)CalculateAction,uiCmdPrioDefault,
	//	NULL,PRO_B_TRUE,PRO_B_TRUE,&Calculate_cmd_id);
	//ProMenubarmenuPushbuttonAdd("VariantDesign","CalculateButton","CalculateButton","Adding a Calculate push button",
	//	NULL,PRO_B_TRUE,Calculate_cmd_id,MsgFile);
	//��ʾ��άͼֽ
	ProCmdActionAdd("DisPlayDwgButtonAct",(uiCmdCmdActFn)DisPlayDwgAction,uiCmdPrioDefault,
		DrawingState,PRO_B_TRUE,PRO_B_TRUE,&DisplayDwg_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","DisplayDwgButton","DisplayDwgButton","Adding a DisplayDwg push button",
		NULL,PRO_B_TRUE,DisplayDwg_cmd_id,MsgFile);

	sprintf(cmd_icon,"twodimshow.gif");
	status=ProCmdIconSet(DisplayDwg_cmd_id,cmd_icon);
	status=ProCmdDesignate(DisplayDwg_cmd_id,"DisplayDwgButton","Adding a DisplayDwg push button","Adding a DisplayDwg push button",MsgFile);

	//��ʾ��άʵ��
	ProCmdActionAdd("DisPlaySolidButtonAct",(uiCmdCmdActFn)SolidPlaySolidAction,uiCmdPrioDefault,
		SolidState,PRO_B_TRUE,PRO_B_TRUE,&DisplaySolid_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","DisplaySolidButton","DisplaySolidButton","Adding a DisplaySolid push button",
		NULL,PRO_B_TRUE,DisplaySolid_cmd_id,MsgFile);

	sprintf(cmd_icon,"threedimshow.gif");
	status=ProCmdIconSet(DisplaySolid_cmd_id,cmd_icon);
	status=ProCmdDesignate(DisplaySolid_cmd_id,"DisplaySolidButton","Adding a DisplaySolid push button","Adding a DisplaySolid push button",MsgFile);

	//��ʾ��ά����
	ProCmdActionAdd("OperateDwgButtonAct",(uiCmdCmdActFn)OperateDwgButtonAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&OperaterDrw_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","OperateDrwButton","OperateDrwButton","Adding a OperateDrwButton push button",
		NULL,PRO_B_TRUE,OperaterDrw_cmd_id,MsgFile);
	//��⵽PDM�Ի���
	//ProCmdActionAdd("AddToPDMButtonAct",(uiCmdCmdActFn)AddToPDMAction,uiCmdPrioDefault,
	//	AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&AddToPDM_cmd_id);
	//ProMenubarmenuPushbuttonAdd("VariantDesign","AddToPDMButton","AddToPDMButton","Adding a PDM button",
	//	NULL,PRO_B_TRUE,AddToPDM_cmd_id,MsgFile);
	//PDM��VDS��Ӧ�Ի���
	ProCmdActionAdd("IntegrateButtonAct",(uiCmdCmdActFn)IntegrateButtonAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&IntegrateDrw_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","IntegrateDrwButton","IntegrateDrwButton","Adding a IntegrateDrwButton push button",
		NULL,PRO_B_TRUE,IntegrateDrw_cmd_id,MsgFile);
	//��Ʒ���ɾ����ť
	ProCmdActionAdd("ProductDeleteButtonAct",(uiCmdCmdActFn)ProductDeleteAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&Product_delete_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","ProductDeleteButton","ProductDeleteButton","Adding a Product Delete Product push button",
		NULL,PRO_B_TRUE,Product_delete_cmd_id,MsgFile);

	sprintf(cmd_icon,"delete.gif");
	status=ProCmdIconSet(Product_delete_cmd_id,cmd_icon);
	status=ProCmdDesignate(Product_delete_cmd_id,"ProductDeleteButton","Adding a Product Delete Product push button","Adding a Product Delete Product push button",MsgFile);

	// ����ó��ڴ水ť
	ProCmdActionAdd("EraseButtonAct",(uiCmdCmdActFn)EraseAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&Erase_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","EraseButton1","EraseButton1","Adding a Check push button1",
		NULL,PRO_B_TRUE,Erase_cmd_id,MsgFile);

	sprintf(cmd_icon,"memoryout.gif");
	status=ProCmdIconSet(Erase_cmd_id,cmd_icon);
	status=ProCmdDesignate(Erase_cmd_id,"EraseButton1","Adding a Check push button1","Adding a Check push button1",MsgFile);

	//�رմ���
	ProCmdActionAdd("CloseWindowsButtonAct",(uiCmdCmdActFn)CloseWindowsAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&CloseWindows_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","CloseWindowsButton","CloseWindowsButton","Adding a CloseWindows button",
		NULL,PRO_B_TRUE,CloseWindows_cmd_id,MsgFile);

	sprintf(cmd_icon,"closewindow.gif");
	status=ProCmdIconSet(CloseWindows_cmd_id,cmd_icon);
	status=ProCmdDesignate(CloseWindows_cmd_id,"CloseWindowsButton","Adding a CloseWindows button","Adding a CloseWindows button",MsgFile);

	//��Ʒ��ƽ��ȶԻ���
	ProCmdActionAdd("ProductProcessButtonAct",(uiCmdCmdActFn)ProductProcessButtonAction,uiCmdPrioDefault,
		AsmState,PRO_B_TRUE,PRO_B_TRUE,&PdtProcess_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","ProductProcessButton","ProductProcessButton","Adding a ProductProcessButton push button1",
		NULL,PRO_B_TRUE,PdtProcess_cmd_id,MsgFile);

                    


	Ftp_struc.Database=xpdmLinkInfo;                                   //PDM��������Ϣ
	Ftp_struc.PDMPath="/Xt/Proven/";                                  //PDM�ļ��ı���Ļ�λ��
	//Ftp_struc.LocalPath="E:";


    //�õ�Pro/e��ǰ��Ŀ¼
	status=ProDirectoryCurrentGet (widepath);
	if (status!=PRO_TK_NO_ERROR) 
	{
		Ftp_struc.LocalPath="c:";
		return FALSE;
	}
	else
	{
		//ProWstringToString(path.GetBuffer(),widepath);
		path=CString(widepath);
		if(path.Right(1)=="\\")
		{
			int k=path.ReverseFind('\\');
			Ftp_struc.LocalPath=path.Left(k);
		}
	}
	//if(!m_db.Open("DSN=ruili;UID=scm;PWD=scm"))
	//{
	//	return PRO_TK_NO_ERROR;
	//}
	//if(!m_pdm.Open("DSN=xtpdm;UID=scm;PWD=scm"))
	//{
	//	return PRO_TK_NO_ERROR;
	//}
	//if(!m_pdm.Open(xpdmLinkInfo))
	//{
	//	return PRO_TK_NO_ERROR;
	//}

	return PRO_TK_NO_ERROR;
}
extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if(drawingDlg!=NULL) delete drawingDlg;
   if(prtasmauditdlg!=NULL) delete prtasmauditdlg;
   if(uploaddlg!=NULL)    delete uploaddlg;
   if(detaildlg!=NULL)   delete detaildlg;
   if (pdmdlg!=NULL)     delete pdmdlg;
   if (partalterdlg!=NULL) delete partalterdlg;
   
   for (int i=0;i<p_classarray.GetSize();i++)
   {
	   ((DetailInfoClass *)p_classarray.GetAt(0))->PerformanceValue.RemoveAll();
	   ((DetailInfoClass *)p_classarray.GetAt(0))->InsValueStr.RemoveAll();
	   delete (DetailInfoClass *)p_classarray.GetAt(0);
	   p_classarray.RemoveAt(0);
   }
	m_db.Close();
	return;
}
//�˵���ѡ
static uiCmdAccessState AccessAvailable (uiCmdAccessMode access_mode)
{
	return (ACCESS_AVAILABLE);
}
//���ݲ�ͬ��״̬ʹ��ť��ѡ���߲���ѡ
static uiCmdAccessState PartState(uiCmdAccessMode access_mode)
{
	 ProError status;
	 ProMdlType p_type;
	 ProMdl mdl;
	 CCommonFuncClass commonfun;
	 //ProSolid ProductSolid;
	 status=ProMdlCurrentGet(&mdl);
	 if (status!=PRO_TK_NO_ERROR)
	 {
		return (ACCESS_REMOVE);
	 }
	 else
	 {
		status = ProMdlTypeGet(mdl,&p_type);
		if (status!=PRO_TK_NO_ERROR)
		{
			return (ACCESS_REMOVE);
		}
		else
			if (p_type==PRO_MDL_PART)
			{
				return (ACCESS_AVAILABLE);
			}
			else
				if (p_type==PRO_MDL_ASSEMBLY)
				{
					return (ACCESS_REMOVE);
				}
	 }	
	  return (ACCESS_REMOVE);
}
//������ά��ʾ��ť����ʾ���Ƴ�
static uiCmdAccessState SolidState(uiCmdAccessMode access_mode)
{
	ProError status;
	ProMdlType p_type;
	ProMdl mdl;
	status=ProMdlCurrentGet(&mdl);
	if (status!=PRO_TK_NO_ERROR)
	{
		return (ACCESS_REMOVE);
	}
	else
	{
		status = ProMdlTypeGet(mdl,&p_type);
		if (status!=PRO_TK_NO_ERROR)
		{
			return (ACCESS_REMOVE);
		}
		else
			if (p_type==PRO_MDL_DRAWING)
			{
				return (ACCESS_AVAILABLE);
			}
			else
				return (ACCESS_REMOVE);
	}	
	return (ACCESS_REMOVE);
}
//���ƹ���ͼ��ʾ��ť����ʾ���Ƴ�
static uiCmdAccessState DrawingState(uiCmdAccessMode access_mode)
{
	ProError status;
	ProMdlType p_type;
	ProMdl mdl;
	status=ProMdlCurrentGet(&mdl);
	if (status!=PRO_TK_NO_ERROR)
	{
		return (ACCESS_REMOVE);
	}
	else
	{
		status = ProMdlTypeGet(mdl,&p_type);
		if (status!=PRO_TK_NO_ERROR)
		{
			return (ACCESS_REMOVE);
		}
		else
			if (p_type==PRO_MDL_DRAWING)
			{
				return (ACCESS_REMOVE);
			}
			else
				return (ACCESS_AVAILABLE);	
	}
	return (ACCESS_REMOVE);
}
static uiCmdAccessState AsmState(uiCmdAccessMode access_mode)
{
	ProError status;
	ProMdlType p_type;
	ProMdl mdl;
	CCommonFuncClass commonfun;
	status=ProMdlCurrentGet(&mdl);
	if (status!=PRO_TK_NO_ERROR)
	{
		return (ACCESS_REMOVE);
	}
	else
	{
		status = ProMdlTypeGet(mdl,&p_type);
		if (status!=PRO_TK_NO_ERROR)
		{
			return (ACCESS_REMOVE);
		}
		else
			if (p_type==PRO_MDL_PART)
			{
					return (ACCESS_REMOVE);
			}
			else
				if (p_type==PRO_MDL_ASSEMBLY)
				{
					return (ACCESS_AVAILABLE);
				}
	}	
	 return (ACCESS_REMOVE);
}
//������Ͷ���

int PartAlterAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int status;
    
	if (partalterdlg!=NULL)
	{
		delete partalterdlg;
	}
	partalterdlg= new CPartAlterDlg;
	status=partalterdlg->Create(IDD_DIALOG_PARTALTER,NULL);
	if(status==0)
	{
		AfxMessageBox("�Ի��򴴽�ʧ��!");
		return status;
	}
	partalterdlg->ShowWindow(SW_SHOW);
	return status;
}



//������άͼ����
int OperateDwgButtonAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(drawingDlg!=NULL)
	{
		delete drawingDlg;
	}
	drawingDlg=new C2DrawingDlg();
	drawingDlg->m_state=1;
	drawingDlg->Create(IDD_DIALOG_2DDRAW,NULL);
	drawingDlg->ShowWindow(SW_SHOW);
	return 1;
}
//PDM��VDS���ɲ���
int IntegrateButtonAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CIntDlg intdlg;
	intdlg.DoModal();
	return 1;
}
//��ʾ��Ʒ���״̬�Ի���
int ProductProcessButtonAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(detaildlg!=NULL)
	{
		delete detaildlg;
	}
	detaildlg=new CDetailDlg;
	detaildlg->Create(IDD_DIALOG_DETAIL,NULL);
	detaildlg->ShowWindow(SW_SHOW);
	return 1;
}
//��Ʒ���Ϊ��������
int ProductSaveAsAction()
{
	ProError status;
	ProMdl   mdl;
	ProMdlType p_type;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPdtSaveAsDlg pdtsaveasdlg;
	status=ProMdlCurrentGet(&mdl);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�Բ���,����û�д�ģ��!");
		return 0;
	}
	status=ProMdlTypeGet(mdl,&p_type);
	if (p_type!=PRO_MDL_ASSEMBLY)
	{
		AfxMessageBox("�Բ��𣬵�ǰģ�Ͳ�����������ͣ�");
		return 0;
	}
	pdtsaveasdlg.DoModal();
	return 1;
}
//����½���������
int PartNewAction()			   //�½��������
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPrtNewDlg newdlg;
	newdlg.DoModal();
	return 1;
}
//�鿴����ģ��
int ProjAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString UserName;
	INT_PTR nRet = -1;
	if(GL_USERNAME=="")
	{
		CUserDlg UserDlg;
		nRet=UserDlg.DoModal();                       //�û�ȷ�϶Ի���
		UserName=UserDlg.UserName;
		if (nRet==IDOK)
		{
			CProjectDlg dlg;                          
			if (UserDlg.IsOK)                         //����û������붼��ȷ�������鿴��Ŀ����ģ��Ի���
			{
				dlg.UserName=UserName;
				dlg.DoModal();
			}
		}
	}
	else
	{
		CProjectDlg dlg;                          
		dlg.UserName=GL_USERNAME;
		dlg.DoModal();
	}
	return 1;
}
//�û�ע��
int UserInfoDel()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString UserName;
	INT_PTR nRet = -1;

	if(GL_USERNAME!="")
	{
		GL_USERNAME="";
		AfxMessageBox("ע���û��ɹ���");
	}
	else
		AfxMessageBox("�û���δ��½��");



	//if(GL_USERNAME=="")
	//{
	//	CUserDlg UserDlg;
	//	nRet=UserDlg.DoModal();                       //�û�ȷ�϶Ի���
	//	UserName=UserDlg.UserName;
	//	if (nRet==IDOK)
	//	{
	//		CProjectDlg dlg;                          
	//		if (UserDlg.IsOK)                         //����û������붼��ȷ�������鿴��Ŀ����ģ��Ի���
	//		{
	//			dlg.UserName=UserName;
	//			dlg.DoModal();
	//		}
	//	}
	//}
	//else
	//{
	//	CProjectDlg dlg;                          
	//	dlg.UserName=GL_USERNAME;
	//	dlg.DoModal();
	//}
	return 1;
}
//��ӵ�PDM��������
int AddToPDMAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(pdmdlg!=NULL)
	{
		delete pdmdlg;
	}
	pdmdlg=new AddToPDMDlg;
	pdmdlg->Create(IDD_DIALOG_ADDTOPDM,NULL);
	pdmdlg->SetWindowPos(NULL,350,250,0,0,SWP_NOSIZE);
	pdmdlg->ShowWindow(SW_SHOW);
	return 1;
}
//�ӵ�ǰ�Ĺ���ͼ�л�����άͼ�İ�ť
int SolidPlaySolidAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());  
	int w_id;
	ProError status;
	ProMdl mdl;
	ProMdlType p_type;
	ProName w_name;
	ProSolid solid;
	status=ProMdlCurrentGet(&mdl);
	if (status!=PRO_TK_NO_ERROR)
	{
	   AfxMessageBox("�޷�ʶ��ǰ�Ĺ���ͼ��");
	   return 0;
	}
	else
	{
		status=ProWindowCurrentGet(&w_id);
		status=ProWindowClear(w_id);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("�޷���ʾ��άͼ��");
			return 0;
		}
		else
		{
            status=ProMdlDisplay(mdl);
			status=ProDrawingCurrentsolidGet((ProDrawing)mdl,&solid);
			if (status!=PRO_TK_NO_ERROR)
			{
				AfxMessageBox("	�޷�ʶ��ǰ�Ĺ���ͼ��");
				return 0;
			}
			status=ProMdlNameGet((ProMdl)solid,w_name);
			status=ProMdlTypeGet((ProMdl)solid,&p_type);
			status=ProObjectwindowCreate(w_name,(ProType)p_type,&w_id);
            status=ProMdlDisplay((ProMdl)solid);
			if (status!=PRO_TK_NO_ERROR)
			{
				AfxMessageBox("�޷���ʾ��άͼ��");
				return 0;
			}
			status=ProWindowActivate(w_id);
			if (status!=PRO_TK_NO_ERROR)
			{
				AfxMessageBox("�޷����ǰ���ڣ����ֹ����");
				return 0;
			}
		}
	}
	return 1;
}
BOOL UsrGetDBAndFtpInfo(CString & strFtp,CString & strFtpUser,CString & strFtpPwd,int & iFtpPort,CString & vdLinkInfo,CString & pdmLinkInfo)
{
	HKEY      hKey;
	struct    HKEY__*ReRootKey;
	TCHAR     *ReSubKey;//*ReExitProKey,*RegSubKeyToCreate;
	TCHAR     *ReValueName_address,*ReValueName_user,*ReValueName_pwd,*ReValueName_port;
	TCHAR     *ReValueName_DB_NAME,*ReValueName_DB_USER,*ReValueName_DB_PWD;
	TCHAR     *RePdmValueName_DB_NAME,*RePdmValueName_DB_USER,*RePdmValueName_DB_PWD;
	TCHAR     *ReValueName_SHUJU;
	char      content_add[256],content_user[256],content_pwd[256];
	char      content_dsn_user[256],content_dsn_pwd[256],content_dsn_name[256];
	char      content_pdm_dsn_user[256],content_pdm_dsn_pwd[256],content_pdm_dsn_name[256];
	char      content_shujuyuan[256];
	int       content_port[256];
	DWORD     dwType;		//�����ȡ�������� REG_DWORD
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
	strFtp.Empty();
	strFtpPwd.Empty();
	strFtpUser.Empty();
	iFtpPort=0;
	vdLinkInfo.Empty();
	pdmLinkInfo.Empty();
	DWORD dwLength1,dwLength2,dwLength3,dwLength4,dwLength5,dwLength6,dwLength7,dwLength8,dwLength9,dwLength10;
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
			if(RegQueryValueEx(hKey,RePdmValueName_DB_NAME,NULL,&dwType,(unsigned char *)content_pdm_dsn_name,&dwLength6)!=ERROR_SUCCESS) content_pdm_dsn_name[0]='\0';
			if(RegQueryValueEx(hKey,RePdmValueName_DB_PWD,NULL,&dwType,(unsigned char *)content_pdm_dsn_pwd,&dwLength10)!=ERROR_SUCCESS) content_pdm_dsn_pwd[0]='\0';
			
			RegCloseKey(hKey);
			strFtp=CString(content_add);
			strFtpUser=CString(content_user);         
			strFtpPwd=CString(content_pwd);
			iFtpPort=content_port[0];
			vdLinkInfo=_T("DSN="+CString(content_shujuyuan)+";UID="+CString(content_dsn_user)+";PWD="+CString(content_dsn_pwd)+";DATABASE=RuiLiMdlDb;");
			pdmLinkInfo=_T("DSN="+CString(content_pdm_dsn_name)+";UID="+CString(content_pdm_dsn_user)+";PWD="+CString(content_pdm_dsn_pwd)+";");
		}	 
		else
		{
			HKEY hk;
			DWORD disposition;   
			if (RegCreateKeyEx(ReRootKey,ReSubKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hk,&disposition))
			{
				AfxMessageBox("ע���ע��ʧ��,������ֹ�����");
				return false;;
			}
			RegCloseKey(hk);
		}

	}
	catch (...) {
		AfxMessageBox("��ע����ȡ����ʧ��");
		return false;
	}
	return true;
}

//�ô󲨵��õĺ���
extern "C" __declspec(dllexport) BOOL AddAsmInterface(CArray<Info,Info> *Info_struc)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	FilePath_struc.RemoveAll();
	CProductClass pdtclass;
	CPdtSaveAsDlg pdtsaveasdlg;
	FilePath uu;
	ProMdl mdl;
	ProError status;
	ProMdlType p_type;
	for (int i=0;i<Info_struc->GetSize();i++)
	{
		//����������߲�����ID�õ����Ե���ϸ��Ϣ
		uu.Type=(*(Info_struc))[i].Type;
		int j=(*(Info_struc))[i].SolidID;
		uu.SolidID=(*(Info_struc))[i].SolidID;
		FilePath_struc.Add(uu);
	}
	//�������ݿ�
	CString vdLinkInfo,xpdmLinkInfo;

	if(!UsrGetDBAndFtpInfo(Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort,vdLinkInfo,xpdmLinkInfo))
	{
		AfxMessageBox("��ע������");
		return FALSE;
	}
	m_db.Close();
	if(!m_db.Open(vdLinkInfo))
	{
		return FALSE;
	}

	//�õ�������߲�������ϸ��Ϣ
	if(!pdtclass.GetAllPartInfoForAsm((*(Info_struc))[0].solid,TRUE))
	{
		return FALSE;
	}
	status=ProMdlCurrentGet(&mdl);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�Բ���,����û�д�ģ��!");
		return 0;
	}
	status=ProMdlTypeGet(mdl,&p_type);
	if (p_type!=PRO_MDL_ASSEMBLY)
	{
		AfxMessageBox("�Բ��𣬵�ǰģ�Ͳ�����������ͣ�");
		return 0;
	}
	//���ò�Ʒ���Ϊ�Ի���
	pdtsaveasdlg.DoModal();
	m_db.Close();
	return TRUE;
}
