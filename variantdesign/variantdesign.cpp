// variantdesign.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "variantdesign.h"

//添加PROE头文件
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



//按钮动作函数声明
int ProductUpLoadAction();
int PartModelSearchAction();	   //零件模型查询函数
int ProductSearchAction();		   //组件查询函数  
int PartSearchAction();			   //零件查询函数
int AlterAction();				   //组件变型函数		   
int PartAlterAction();			   //零件变型函数
int AsmModelSearchAction();		   //组件模型查询函数
int PartSaveAction();			   //零件实例保存函数
int PartSaveAsAction();			   //零件另存为函数
int PartNewAction();			   //新建零件函数
int DisPlayDwgAction();			   //显示二维图纸函数
int SolidPlaySolidAction();        //显示三维图的按钮
int CheckAction();				   //检查实例函数
int PartDeleteAction();			   //删除实例按钮
int UserPopupmenusSetup();		   //右键弹出函数
int ProductSaveAction();		   //产品保存函数
int ProductSaveAsAction();		   //产品另存为函数
int AddToPDMAction();              //添加到PDM函数
int EraseAction();                 //拭除内存函数
int CloseWindowsAction();		   //关闭当前对话框
int ProductChectAction();          //产品审核对话框
int ProductDeleteAction();		   //产品删除对话框
int OperateDwgButtonAction();      //操作二维图
int IntegrateButtonAction();       //PDM和VDS对应对话框
int ProductProcessButtonAction();  //产品设计状态对话框
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
//	注意！
//
//		如果此 DLL 动态链接到 MFC
//		DLL，从此 DLL 导出并
//		调入 MFC 的任何函数在函数的最前面
//		都必须添加 AFX_MANAGE_STATE 宏。
//
//		例如：
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CvariantdesignApp

BEGIN_MESSAGE_MAP(CvariantdesignApp, CWinApp)
END_MESSAGE_MAP()


// CvariantdesignApp 结构

CvariantdesignApp::CvariantdesignApp()
{
	// TODO: 在此添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CvariantdesignApp 对象

CvariantdesignApp theApp;



// CvariantdesignApp 初始化

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
	ProStringToWstring(MsgFile,"Message.txt");     //信息文件
	ProMenuItemIcon cmd_icon;
	//提示信息



	CString vdLinkInfo,xpdmLinkInfo;

	if(!UsrGetDBAndFtpInfo(Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort,vdLinkInfo,xpdmLinkInfo))
	{
		AfxMessageBox("读注册表错误");
		return PRO_TK_NO_ERROR;
	}
	if(!m_db.Open(vdLinkInfo))
	{
		return PRO_TK_NO_ERROR;
	}
	g_ConnectString=vdLinkInfo;
    //添加变型设计菜单条
	status=ProMenubarMenuAdd("VariantDesign","DesignMenu","Utilities",PRO_B_TRUE,MsgFile);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//添加项目菜单
	ProCmdActionAdd("ProjButtonAct",(uiCmdCmdActFn)ProjAction,uiCmdPrioDefault,
		NULL,PRO_B_TRUE,PRO_B_TRUE,&Proj_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","ProjButton","ProjButton","Adding a Proj push button",
		NULL,PRO_B_TRUE,Proj_cmd_id,MsgFile);
	
	//添加注销用户菜单
	ProCmdActionAdd("UserDelete",(uiCmdCmdActFn)UserInfoDel,uiCmdPrioDefault,
		NULL,PRO_B_TRUE,PRO_B_TRUE,&User_DelID);
	ProMenubarmenuPushbuttonAdd("VariantDesign","UserDel","UserDel","UserDelBtn",
		NULL,PRO_B_TRUE,User_DelID,MsgFile);
	
	//添加零件变型设计按钮
	ProMenubarmenuMenuAdd("VariantDesign","PartSubMenu","PartSubMenu",NULL,PRO_B_TRUE,MsgFile);

	//添加零件新建子按钮
	ProCmdActionAdd("PartNewButtonAct",(uiCmdCmdActFn)PartNewAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&PartNew_cmd_id);
	ProMenubarmenuPushbuttonAdd("PartSubMenu","PartNewButton","PartNewButton","Adding a PartNew push button",
		NULL,PRO_B_TRUE,PartNew_cmd_id,MsgFile);

	sprintf(cmd_icon,"newpart.gif");
	status=ProCmdIconSet(PartNew_cmd_id,cmd_icon);
	status=ProCmdDesignate(PartNew_cmd_id,"PartNewButton","Adding a PartNew push button","Adding a PartNew push button",MsgFile);

	//添加零件实例打开子按钮
	ProCmdActionAdd("PartSearchButtonAct",(uiCmdCmdActFn)PartSearchAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&PartSearch_cmd_id);
	ProMenubarmenuPushbuttonAdd("PartSubMenu","PartSearchButton","PartSearchButton","Adding a PartSearch push button",
		NULL,PRO_B_TRUE,PartSearch_cmd_id,MsgFile);

	sprintf(cmd_icon,"partopen.gif");
	status=ProCmdIconSet(PartSearch_cmd_id,cmd_icon);
	status=ProCmdDesignate(PartSearch_cmd_id,"PartSearchButton","Adding a PartSearch push button","Adding a PartSearch push button",MsgFile);

	//添加零件变型设计子按钮
	ProCmdActionAdd("PartAlterButtonAct",(uiCmdCmdActFn)PartAlterAction,uiCmdPrioDefault,
		PartState,PRO_B_TRUE,PRO_B_TRUE,&PartAlter_cmd_id);
	ProMenubarmenuPushbuttonAdd("PartSubMenu","PartAlterButton","PartAlterButton","Adding a Alter Part push button",
		NULL,PRO_B_TRUE,PartAlter_cmd_id,MsgFile);

	sprintf(cmd_icon,"partvariant.gif");
	status=ProCmdIconSet(PartAlter_cmd_id,cmd_icon);
	status=ProCmdDesignate(PartAlter_cmd_id,"PartAlterButton","Adding a Alter Part push button","Adding a Alter Part push button",MsgFile);

	//添加零件另存为子按钮
	ProCmdActionAdd("PartSaveAsButtonAct",(uiCmdCmdActFn)PartSaveAsAction,uiCmdPrioDefault,
		PartState,PRO_B_TRUE,PRO_B_TRUE,&PartSaveAs_cmd_id);
	ProMenubarmenuPushbuttonAdd("PartSubMenu","PartSaveAsButton","PartSaveAsButton","Adding a SaveAs Part push button",
		NULL,PRO_B_TRUE,PartSaveAs_cmd_id,MsgFile);

	sprintf(cmd_icon,"prtsaveas.gif");
	status=ProCmdIconSet(PartSaveAs_cmd_id,cmd_icon);
	status=ProCmdDesignate(PartSaveAs_cmd_id,"PartSaveAsButton","Adding a SaveAs Part push button","Adding a SaveAs Part push button",MsgFile);

	//添加零件实例保存子按钮
	ProCmdActionAdd("PartSaveButtonAct",(uiCmdCmdActFn)PartSaveAction,uiCmdPrioDefault,
		PartState,PRO_B_TRUE,PRO_B_TRUE,&PartSave_cmd_id);
	ProMenubarmenuPushbuttonAdd("PartSubMenu","PartSaveButton","PartSaveButton","Adding a Save Part push button",
		NULL,PRO_B_TRUE,PartSave_cmd_id,MsgFile);

	sprintf(cmd_icon,"prtsave.gif");
	status=ProCmdIconSet(PartSave_cmd_id,cmd_icon);
	status=ProCmdDesignate(PartSave_cmd_id,"PartSaveButton","Adding a Save Part push button","Adding a Save Part push button",MsgFile);


	//检查已经设计完的实例
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
	//添加产品变型设计按钮
	ProMenubarmenuMenuAdd("VariantDesign","ProductSubMenu","ProductSubMenu",NULL,PRO_B_TRUE,MsgFile);

	//产品实例新建子按钮
	ProCmdActionAdd("AsmModelSearchButtonAct",(uiCmdCmdActFn)AsmModelSearchAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&AsmModelSearch_cmd_id);
	ProMenubarmenuPushbuttonAdd("ProductSubMenu","AsmModelSearchButton","AsmModelSearchButton","Adding a AsmModelSearch push button",
		NULL,PRO_B_TRUE,AsmModelSearch_cmd_id,MsgFile);

	sprintf(cmd_icon,"pdtnew.gif");
	status=ProCmdIconSet(AsmModelSearch_cmd_id,cmd_icon);
	status=ProCmdDesignate(AsmModelSearch_cmd_id,"AsmModelSearchButton","Adding a AsmModelSearch push button","Adding a AsmModelSearch push button",MsgFile);


	//添加产品查询子按钮
	ProCmdActionAdd("ProductSearchButtonAct",(uiCmdCmdActFn)ProductSearchAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&ProductSearch_cmd_id);
	ProMenubarmenuPushbuttonAdd("ProductSubMenu","ProductSearchButton","ProductSearchButton","Adding a ProductSearch push button",
		NULL,PRO_B_TRUE,ProductSearch_cmd_id,MsgFile);

	sprintf(cmd_icon,"pdtopen.gif");
	status=ProCmdIconSet(ProductSearch_cmd_id,cmd_icon);
	status=ProCmdDesignate(ProductSearch_cmd_id,"ProductSearchButton","Adding a ProductSearch push button","Adding a ProductSearch push button",MsgFile);

	//设置产品变型子按钮
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

	//保存产品保存按钮
	ProCmdActionAdd("ProductSaveButtonAct",(uiCmdCmdActFn)ProductSaveAction,uiCmdPrioDefault,
		AsmState,PRO_B_TRUE,PRO_B_TRUE,&Product_Save_cmd_id);
	ProMenubarmenuPushbuttonAdd("ProductSubMenu","ProductSaveButton","ProductSaveButton","Adding a Product Save push button",
		NULL,PRO_B_TRUE,Product_Save_cmd_id,MsgFile);

	sprintf(cmd_icon,"pdtsave.gif");
	status=ProCmdIconSet(Product_Save_cmd_id,cmd_icon);
	status=ProCmdDesignate(Product_Save_cmd_id,"ProductSaveButton","Adding a Product Save push button","Adding a Product Save push button",MsgFile);

	//产品审核按钮
	ProCmdActionAdd("ProductChectButtonAct",(uiCmdCmdActFn)ProductChectAction,uiCmdPrioDefault,
		AsmState,PRO_B_TRUE,PRO_B_TRUE,&Product_Check_cmd_id);
	ProMenubarmenuPushbuttonAdd("ProductSubMenu","ProductCheckButton","ProductCheckButton","Adding a Product Check Product push button",
		NULL,PRO_B_TRUE,Product_Check_cmd_id,MsgFile);

	sprintf(cmd_icon,"checkpdt.gif");
	status=ProCmdIconSet(Product_Check_cmd_id,cmd_icon);
	status=ProCmdDesignate(Product_Check_cmd_id,"ProductCheckButton","Adding a Product Check Product push button","Adding a Product Check Product push button",MsgFile);

	//添加产品入库子按钮
	ProCmdActionAdd("UpLoadButtonAct",(uiCmdCmdActFn)ProductUpLoadAction,uiCmdPrioDefault,
		AsmState,PRO_B_TRUE,PRO_B_TRUE,&Product_UpLoad_cmd_id);
	ProMenubarmenuPushbuttonAdd("ProductSubMenu","ProductUpLoadButton","ProductUpLoadButton","Adding a Product UpLoad Product push button",
		NULL,PRO_B_TRUE,Product_UpLoad_cmd_id,MsgFile);


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//分析计算
	//ProCmdActionAdd("CalculateButtonAct",(uiCmdCmdActFn)CalculateAction,uiCmdPrioDefault,
	//	NULL,PRO_B_TRUE,PRO_B_TRUE,&Calculate_cmd_id);
	//ProMenubarmenuPushbuttonAdd("VariantDesign","CalculateButton","CalculateButton","Adding a Calculate push button",
	//	NULL,PRO_B_TRUE,Calculate_cmd_id,MsgFile);
	//显示二维图纸
	ProCmdActionAdd("DisPlayDwgButtonAct",(uiCmdCmdActFn)DisPlayDwgAction,uiCmdPrioDefault,
		DrawingState,PRO_B_TRUE,PRO_B_TRUE,&DisplayDwg_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","DisplayDwgButton","DisplayDwgButton","Adding a DisplayDwg push button",
		NULL,PRO_B_TRUE,DisplayDwg_cmd_id,MsgFile);

	sprintf(cmd_icon,"twodimshow.gif");
	status=ProCmdIconSet(DisplayDwg_cmd_id,cmd_icon);
	status=ProCmdDesignate(DisplayDwg_cmd_id,"DisplayDwgButton","Adding a DisplayDwg push button","Adding a DisplayDwg push button",MsgFile);

	//显示三维实体
	ProCmdActionAdd("DisPlaySolidButtonAct",(uiCmdCmdActFn)SolidPlaySolidAction,uiCmdPrioDefault,
		SolidState,PRO_B_TRUE,PRO_B_TRUE,&DisplaySolid_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","DisplaySolidButton","DisplaySolidButton","Adding a DisplaySolid push button",
		NULL,PRO_B_TRUE,DisplaySolid_cmd_id,MsgFile);

	sprintf(cmd_icon,"threedimshow.gif");
	status=ProCmdIconSet(DisplaySolid_cmd_id,cmd_icon);
	status=ProCmdDesignate(DisplaySolid_cmd_id,"DisplaySolidButton","Adding a DisplaySolid push button","Adding a DisplaySolid push button",MsgFile);

	//显示二维操作
	ProCmdActionAdd("OperateDwgButtonAct",(uiCmdCmdActFn)OperateDwgButtonAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&OperaterDrw_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","OperateDrwButton","OperateDrwButton","Adding a OperateDrwButton push button",
		NULL,PRO_B_TRUE,OperaterDrw_cmd_id,MsgFile);
	//入库到PDM对话框
	//ProCmdActionAdd("AddToPDMButtonAct",(uiCmdCmdActFn)AddToPDMAction,uiCmdPrioDefault,
	//	AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&AddToPDM_cmd_id);
	//ProMenubarmenuPushbuttonAdd("VariantDesign","AddToPDMButton","AddToPDMButton","Adding a PDM button",
	//	NULL,PRO_B_TRUE,AddToPDM_cmd_id,MsgFile);
	//PDM和VDS对应对话框
	ProCmdActionAdd("IntegrateButtonAct",(uiCmdCmdActFn)IntegrateButtonAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&IntegrateDrw_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","IntegrateDrwButton","IntegrateDrwButton","Adding a IntegrateDrwButton push button",
		NULL,PRO_B_TRUE,IntegrateDrw_cmd_id,MsgFile);
	//产品零件删除按钮
	ProCmdActionAdd("ProductDeleteButtonAct",(uiCmdCmdActFn)ProductDeleteAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&Product_delete_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","ProductDeleteButton","ProductDeleteButton","Adding a Product Delete Product push button",
		NULL,PRO_B_TRUE,Product_delete_cmd_id,MsgFile);

	sprintf(cmd_icon,"delete.gif");
	status=ProCmdIconSet(Product_delete_cmd_id,cmd_icon);
	status=ProCmdDesignate(Product_delete_cmd_id,"ProductDeleteButton","Adding a Product Delete Product push button","Adding a Product Delete Product push button",MsgFile);

	// 添加拭除内存按钮
	ProCmdActionAdd("EraseButtonAct",(uiCmdCmdActFn)EraseAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&Erase_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","EraseButton1","EraseButton1","Adding a Check push button1",
		NULL,PRO_B_TRUE,Erase_cmd_id,MsgFile);

	sprintf(cmd_icon,"memoryout.gif");
	status=ProCmdIconSet(Erase_cmd_id,cmd_icon);
	status=ProCmdDesignate(Erase_cmd_id,"EraseButton1","Adding a Check push button1","Adding a Check push button1",MsgFile);

	//关闭窗口
	ProCmdActionAdd("CloseWindowsButtonAct",(uiCmdCmdActFn)CloseWindowsAction,uiCmdPrioDefault,
		AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&CloseWindows_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","CloseWindowsButton","CloseWindowsButton","Adding a CloseWindows button",
		NULL,PRO_B_TRUE,CloseWindows_cmd_id,MsgFile);

	sprintf(cmd_icon,"closewindow.gif");
	status=ProCmdIconSet(CloseWindows_cmd_id,cmd_icon);
	status=ProCmdDesignate(CloseWindows_cmd_id,"CloseWindowsButton","Adding a CloseWindows button","Adding a CloseWindows button",MsgFile);

	//产品设计进度对话框
	ProCmdActionAdd("ProductProcessButtonAct",(uiCmdCmdActFn)ProductProcessButtonAction,uiCmdPrioDefault,
		AsmState,PRO_B_TRUE,PRO_B_TRUE,&PdtProcess_cmd_id);
	ProMenubarmenuPushbuttonAdd("VariantDesign","ProductProcessButton","ProductProcessButton","Adding a ProductProcessButton push button1",
		NULL,PRO_B_TRUE,PdtProcess_cmd_id,MsgFile);

                    


	Ftp_struc.Database=xpdmLinkInfo;                                   //PDM的连接信息
	Ftp_struc.PDMPath="/Xt/Proven/";                                  //PDM文件的保存的基位置
	//Ftp_struc.LocalPath="E:";


    //得到Pro/e当前的目录
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
//菜单可选
static uiCmdAccessState AccessAvailable (uiCmdAccessMode access_mode)
{
	return (ACCESS_AVAILABLE);
}
//根据不同的状态使按钮可选或者不可选
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
//控制三维显示按钮的显示和移除
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
//控制工程图显示按钮的显示和移除
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
//零件变型动作

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
		AfxMessageBox("对话框创建失败!");
		return status;
	}
	partalterdlg->ShowWindow(SW_SHOW);
	return status;
}



//操作二维图动作
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
//PDM和VDS集成操作
int IntegrateButtonAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CIntDlg intdlg;
	intdlg.DoModal();
	return 1;
}
//显示产品设计状态对话框
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
//产品另存为动作函数
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
		AfxMessageBox("对不起,您还没有打开模型!");
		return 0;
	}
	status=ProMdlTypeGet(mdl,&p_type);
	if (p_type!=PRO_MDL_ASSEMBLY)
	{
		AfxMessageBox("对不起，当前模型不是是组件类型！");
		return 0;
	}
	pdtsaveasdlg.DoModal();
	return 1;
}
//零件新建动作函数
int PartNewAction()			   //新建零件函数
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPrtNewDlg newdlg;
	newdlg.DoModal();
	return 1;
}
//查看任务模块
int ProjAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString UserName;
	INT_PTR nRet = -1;
	if(GL_USERNAME=="")
	{
		CUserDlg UserDlg;
		nRet=UserDlg.DoModal();                       //用户确认对话框
		UserName=UserDlg.UserName;
		if (nRet==IDOK)
		{
			CProjectDlg dlg;                          
			if (UserDlg.IsOK)                         //如果用户的密码都正确，则进入查看项目管理模块对话框
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
//用户注销
int UserInfoDel()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString UserName;
	INT_PTR nRet = -1;

	if(GL_USERNAME!="")
	{
		GL_USERNAME="";
		AfxMessageBox("注销用户成功！");
	}
	else
		AfxMessageBox("用户还未登陆！");



	//if(GL_USERNAME=="")
	//{
	//	CUserDlg UserDlg;
	//	nRet=UserDlg.DoModal();                       //用户确认对话框
	//	UserName=UserDlg.UserName;
	//	if (nRet==IDOK)
	//	{
	//		CProjectDlg dlg;                          
	//		if (UserDlg.IsOK)                         //如果用户的密码都正确，则进入查看项目管理模块对话框
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
//添加到PDM动作函数
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
//从当前的工程图切换到三维图的按钮
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
	   AfxMessageBox("无法识别当前的工程图！");
	   return 0;
	}
	else
	{
		status=ProWindowCurrentGet(&w_id);
		status=ProWindowClear(w_id);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("无法显示三维图！");
			return 0;
		}
		else
		{
            status=ProMdlDisplay(mdl);
			status=ProDrawingCurrentsolidGet((ProDrawing)mdl,&solid);
			if (status!=PRO_TK_NO_ERROR)
			{
				AfxMessageBox("	无法识别当前的工程图！");
				return 0;
			}
			status=ProMdlNameGet((ProMdl)solid,w_name);
			status=ProMdlTypeGet((ProMdl)solid,&p_type);
			status=ProObjectwindowCreate(w_name,(ProType)p_type,&w_id);
            status=ProMdlDisplay((ProMdl)solid);
			if (status!=PRO_TK_NO_ERROR)
			{
				AfxMessageBox("无法显示三维图！");
				return 0;
			}
			status=ProWindowActivate(w_id);
			if (status!=PRO_TK_NO_ERROR)
			{
				AfxMessageBox("无法激活当前窗口，请手工激活！");
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
	DWORD     dwType;		//定义读取数据类型 REG_DWORD
	ReRootKey=HKEY_CURRENT_USER;              //注册表主键名称
	ReSubKey="Software\\PTC\\VariantDesign";  //欲打开注册表值的地址
	ReValueName_address="ftp_address";               //欲设置值的名称
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
				AfxMessageBox("注册表注册失败,请进行手工设置");
				return false;;
			}
			RegCloseKey(hk);
		}

	}
	catch (...) {
		AfxMessageBox("从注册表读取数据失败");
		return false;
	}
	return true;
}

//让大波调用的函数
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
		//根据零件或者部件的ID得到各自的详细信息
		uu.Type=(*(Info_struc))[i].Type;
		int j=(*(Info_struc))[i].SolidID;
		uu.SolidID=(*(Info_struc))[i].SolidID;
		FilePath_struc.Add(uu);
	}
	//连接数据库
	CString vdLinkInfo,xpdmLinkInfo;

	if(!UsrGetDBAndFtpInfo(Ftp_struc.strFtp,Ftp_struc.FtpName,Ftp_struc.FtpPassword,Ftp_struc.iPort,vdLinkInfo,xpdmLinkInfo))
	{
		AfxMessageBox("读注册表错误");
		return FALSE;
	}
	m_db.Close();
	if(!m_db.Open(vdLinkInfo))
	{
		return FALSE;
	}

	//得到零件或者部件的详细信息
	if(!pdtclass.GetAllPartInfoForAsm((*(Info_struc))[0].solid,TRUE))
	{
		return FALSE;
	}
	status=ProMdlCurrentGet(&mdl);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("对不起,您还没有打开模型!");
		return 0;
	}
	status=ProMdlTypeGet(mdl,&p_type);
	if (p_type!=PRO_MDL_ASSEMBLY)
	{
		AfxMessageBox("对不起，当前模型不是是组件类型！");
		return 0;
	}
	//调用产品另存为对话框
	pdtsaveasdlg.DoModal();
	m_db.Close();
	return TRUE;
}
