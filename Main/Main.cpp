// Main.cpp : 定义 DLL 的初始化例程。
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
//定义全局变量
//1.零件的事物特性表
CArray<UsrSWTXTable1,UsrSWTXTable1>  p_UsrSWTXTableList;  //定义全局的事物特性表列项，
//在程序运行时，要给他一个初始值(无效)
//2.零件的关系
//CArray<UsrRelTable,UsrRelTable>    p_UsrRelTableList;   //定义全局的关系列表项数组
//3.零件的图片
CArray<UsrPicture,UsrPicture>   p_UsrPicture;           //图片的全局变量    
//4.零件的性能参数
CArray<UsrPerfParaTable,UsrPerfParaTable> p_UsrPerfParaTableList;//定义的性能参数表的全局变量

//5.模型的基本属性 注：零件模块和装配模块公用
CArray<UsrPartPorp,UsrPartPorp> p_UsrPartPorp;          //定义模型基本信息的全局变量

//6.装配的事物特性
CArray<AsmSmlItem,AsmSmlItem> p_AsmSmlItemList;
//CArray<CRel,CRel&>            p_CRelItemList;
CRelColection g_RelCollection;
//7.装配的关系列表
//CArray<AsmRelItem,AsmRelItem> p_AsmRelItemList;
//8.装配的元件的变形范围
CArray<AsmCompVarItem,AsmCompVarItem> p_AsmCompItemList;
//////////////////////////////////////////////////////////////////////////
//9.对话框图标
int  DlgIcon;//=IDI_PROE;  //设置对话框的标题图标
////10.工程图列表
//CArray<UsrDrawingItem,UsrDrawingItem> P_UsrDrawingList;
//10.数据库
CMyDatabase * m_db;
CImageList      g_ImageList1;  //大图标
CImageList      g_ImageList2;  //小图标
//////////////////////////////////////////////////////////////////////////
//对话框，全局变量，主要为了用非模式方式
//1.零件尺寸、参数的信息建模
CRelParaEditDlg * dlg_part;
//2.装配的尺寸、参数的信息建模
CAsmParaDefDlg * dlg_asm;
//3.图片编辑对话框
CPicMakgeDlg * dlg_pic;
//4.变形测试
CTestRelDlg * dlg_test;
CAsmParaDefDlg2 * dlg_asm_def_test;
//5.从模型装配
CAsmAddComplg * addComp_dlg=NULL;

C2DrawingDlg * testdlg=NULL;

//////////////////////////////////////////////////////////////////////////
//一下为数据库和FTP服务器连接信息
CString         PartConnectSet;   //连接，需要进一步解决
FtpConnectInfo  myFtp;
CString         dbLinkInfo;
CString         dbPdmLinkInfo;
//CString         gWorkDirectory;  //工作目录
int UserPopupmenusSetup();
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//访问函数定义
//1.访问装配模块的定义，如果当前模型是装配则有效
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
//2.访问零件模块的定义，如果当前是零件模式则有效
static uiCmdAccessState UsrAccessPartDefault(uiCmdAccessMode access_mode)//访问控制函数
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
//3.模型基本信息定义模块,只有当前存在零件和装配件时才有效
static uiCmdAccessState UsrAccessCommonDefault(uiCmdAccessMode access_mode)//访问控制函数
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
//3.图片制作按钮,对于零件,组建和工程图都有效
static uiCmdAccessState UsrAccessForPicMake(uiCmdAccessMode access_mode)//访问控制函数
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
//4.对所有都可以有效的访问函数
static uiCmdAccessState UsrAccessDefault(uiCmdAccessMode access_mode)//访问控制函数
{

	return (ACCESS_AVAILABLE);

}
//5.取消，或不可见的
static uiCmdAccessState UsrAccessUNAVAILABLE(uiCmdAccessMode access_mode)//访问控制函数
{

	return (ACCESS_REMOVE);
	//return(ACCESS_AVAILABLE);

}
//6.在组件模式中，消除装配按钮
static uiCmdAccessState UsrAccessUNAVAILABLE_ASSEMBLY_MODE(uiCmdAccessMode access_mode)//访问控制函数
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
//7.自定义装配按钮，只有在装配模式下才可见
static uiCmdAccessState UsrAccessAVAILABLE_ASSEMBLY_MODE(uiCmdAccessMode access_mode)//访问控制函数
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
//8.工程图都效
static uiCmdAccessState UsrAccessForDrawing(uiCmdAccessMode access_mode)//访问控制函数
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
//菜单动作函数
//1.新建1
void new1();
//2.从零件和装配的模型库中继承新建
void new2();
//3.打开本地
void open1();
//4.模型文件管理
void funcMdlManage();
//5.保存到本地
void save1();
//6.保存到工作区
void save2();
//7.基本信息定义
void MdlDef();
//8.零件尺寸、参数，关系建立函数
void funcParaTalbe();//参数表函数
//9.样图制作函数
void funSamplePic();
//10.生成零件模型函数
void funcPartManage();
//11.生成产品模型
void funcProductManage();
//12.装配模型尺寸、参数，关系建立函数
void AsmParaDefAcFn();
void AsmParaDefAcFn2();
//13.选项 //选项设置函数
void funcOption();
//14.工作区     注：可以删除
void workplaceman();
//15.数据库连接信息和服务器连接信息
void GetDBAndFtpInfo();
//16.模型工程图定义
void funcMdlDrawing();
//16.模型工程图设置
void funcMdlDrawingSet();
//17.从模型库中添加模型，进行装配
void funcAssemblyFromMdl();
//18.替换本来的装配，
void funcAssemblyFromLocalMdl();
//19.测试模型变型功能
void funcMdlTest();
//20.把装配成的产品交变型模块审核
void funcProductShenHe();
//////////////////////////////////////////////////////////////////////////
//21.测试
void TestFun();
//22.全局的图标设置，
void SetGImageList();
//23.进入有限元模式
 void funcFemEnter();
//24.打开有限元模型
 void funcFemOpen();
//25.保存有限元模型
void funcFemSave();

//用到的公共函数
bool CreatDefDtm();
ProError GemitemAction(ProGeomitem * p_handle,ProAppData geomitem);
void UsrGeomitemPlnGet(ProFeature feature,ProGeomitem *geomitem);
int UsrCreatDefDtm(ProDtmplnConstrType pro_dtmpln_def,ProName dtm_name,ProGeomitem *pln);


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

// CMainApp

BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
	//ON_COMMAND(ID_PART_RENAME, OnTypeRename)
END_MESSAGE_MAP()


// CMainApp 结构

CMainApp::CMainApp()
{
	// TODO: 在此添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	//SetGImageList();
}


// 唯一的 CMainApp 对象

CMainApp theApp;


// CMainApp 初始化

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
	//	AfxMessageBox("Ole初始化失败！");
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
		AfxMessageBox("数据库连接错误，程序将关闭");
		return FALSE;
	}





	//---------------------------------------------------------------

	return TRUE;
}

//dll初始化函数
extern "C" int user_initialize(int argc,char *argv[])
{
	ProError         status;
	ProName          Msg;
	ProSolid         solid;
	//获得当前模型的类型
    status=ProMdlCurrentGet((ProMdl*)&solid);

	uiCmdCmdId  uiCmdMdlDef,uiCmdParaButton,uiCmdSamplePicButton,uiCmdMdlManage,
		        uiCmdPartManageButton,uiCmdProductManageButton,uiCmdOptionButton,uiCmdProduceShenheButton,uiCmdDrawingSetButton,
				//,uiCmdNew1,/*uiCmdNew2,*//*uiCmdOpen1,*/uiCmdOpen2,*/
				uiCmdSave1,uiCmdSave2,/*uiCmdAsmParaDefButton,*/uiCmdAsmParaDefButton2,uiCmdDrawingButton,uiCmdAssemblyButton,uiCmdMdlTestButton;
	uiCmdCmdId  uiCmdFemEnter,uiCmdFemOpen,uiCmdFemSave;
	
    ProMenuItemIcon cmd_icon ;
    //定义消息文件
	ProStringToWstring(Msg,"message_mdl.txt");
	//添加菜单条
    status=ProMenubarMenuAdd("MdlMenu","建模","Utilities",PRO_B_FALSE,Msg);


	//添加模型管理
	status=ProCmdActionAdd("模型文件管理命令",(uiCmdCmdActFn)funcMdlManage,
		uiCmdPrioDefault,UsrAccessDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdMdlManage);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","模型文件管理","模型文件管理",
		"对工作区、模型库、现有零件和组件以及本地文件进行管理",NULL,PRO_B_TRUE,uiCmdMdlManage,Msg);

	
	status=ProMenubarmenuMenuAdd("MdlMenu","Sav","保存当前模型",NULL,PRO_B_TRUE ,Msg);

																		//status=ProCmdActionAdd("save1",(uiCmdCmdActFn)save1,
																		//	uiCmdPrioDefault,UsrAccessCommonDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdSave1);	
																		//得到Pro/ENGINEER保存的命令
	status=ProCmdCmdIdFind("ProCmdModelSave",&uiCmdSave1);
	status=ProCmdActionAdd("save2",(uiCmdCmdActFn)save2,
		uiCmdPrioDefault,UsrAccessCommonDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdSave2);

	status=ProMenubarmenuPushbuttonAdd("Sav","保存文件系统","保存文件系统",
		"把模型保存到本地目录当中",NULL,PRO_B_TRUE,uiCmdSave1,Msg);
	status=ProMenubarmenuPushbuttonAdd("Sav","模型工作区","模型工作区",
		"把模型保存到模型工作区",NULL,PRO_B_TRUE,uiCmdSave2,Msg);


	//模型 定义
	status=ProCmdActionAdd("基本信息定义命令",(uiCmdCmdActFn)MdlDef,
		uiCmdPrioDefault,UsrAccessCommonDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdMdlDef);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","基本信息定义","基本信息定义",
	"定义当前模型的基本信息",NULL,PRO_B_TRUE,uiCmdMdlDef,Msg);
	sprintf(cmd_icon,"jbxx.gif");
	status=ProCmdIconSet(uiCmdMdlDef,cmd_icon);
	status = ProCmdDesignate (uiCmdMdlDef,"基本信息定义","定义当前模型的基本信息","定义当前模型的基本信息",	Msg);
	
	//事物特性表（按钮）
	status=ProCmdActionAdd("零件信息建模命令",(uiCmdCmdActFn)funcParaTalbe,
		uiCmdPrioDefault,UsrAccessPartDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdParaButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","零件信息建模","零件信息建模",
		"构建模型的事物特性表和关系式信息",NULL,PRO_B_TRUE,uiCmdParaButton,Msg);
	sprintf(cmd_icon,"swtxrel.gif");
	status=ProCmdIconSet(uiCmdParaButton,cmd_icon);	
	status = ProCmdDesignate (uiCmdParaButton,"零件信息建模","构建模型的事物特性表和关系式信息","构建模型的事物特性表和关系式信息",	Msg);

																					//添加装配事务特性表和关系定义
																					//status=ProCmdActionAdd("装配信息建模命令",(uiCmdCmdActFn)AsmParaDefAcFn,
																					//	uiCmdPrioDefault,AccessAvailable,
																					//	PRO_B_TRUE,PRO_B_TRUE,&uiCmdAsmParaDefButton);
																					//status=ProMenubarmenuPushbuttonAdd("MdlMenu", "装配信息建模", "装配信息建模",
																					//	"定义模型的组成，模型的事物特性表和关系式信息", NULL,
																					//	PRO_B_TRUE, uiCmdAsmParaDefButton, Msg);  

	status=ProCmdActionAdd("装配信息建模命令",(uiCmdCmdActFn)AsmParaDefAcFn2,
		uiCmdPrioDefault,AccessAvailable,
		PRO_B_TRUE,PRO_B_TRUE,&uiCmdAsmParaDefButton2);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu", "装配信息建模", "装配信息建模",
		"定义模型的组成，模型的事物特性表和关系式信息", NULL,
		PRO_B_TRUE, uiCmdAsmParaDefButton2, Msg);  
	sprintf(cmd_icon,"swtxrel.gif");
	status=ProCmdIconSet(uiCmdAsmParaDefButton2,cmd_icon);
    status = ProCmdDesignate (uiCmdAsmParaDefButton2,"装配信息建模","定义模型的组成，模型的事物特性表和关系式信息","定义模型的组成，模型的事物特性表和关系式信息",	Msg);

	//模型工程图
	status=ProCmdActionAdd("模型工程图命令",(uiCmdCmdActFn)funcMdlDrawing,
		uiCmdPrioDefault,UsrAccessCommonDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdDrawingButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","模型工程图纸","模型工程图纸",
		"对模型的工程图进行管理",NULL,PRO_B_TRUE,uiCmdDrawingButton,Msg);
	sprintf(cmd_icon,"mxgct.gif");
	status=ProCmdIconSet(uiCmdDrawingButton,cmd_icon);
    status = ProCmdDesignate (uiCmdDrawingButton,"模型工程图纸","对模型的工程图进行管理","对模型的工程图进行管理",Msg);

	//样图生成器（按钮）
	status=ProCmdActionAdd("样图工具命令",(uiCmdCmdActFn)funSamplePic,
		uiCmdPrioDefault,UsrAccessForPicMake,PRO_B_TRUE,PRO_B_TRUE,&uiCmdSamplePicButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","模型样图制作","模型样图制作",
		"从当前模型中，生成事物特性表指定参数的参照图片",NULL,PRO_B_TRUE,uiCmdSamplePicButton,Msg);
	sprintf(cmd_icon,"picmaker.gif");
	status=ProCmdIconSet(uiCmdSamplePicButton,cmd_icon);
    status = ProCmdDesignate (uiCmdSamplePicButton,"模型样图制作","从当前模型中，生成事物特性表指定参数的参照图片","从当前模型中，生成事物特性表指定参数的参照图片",Msg);

	//测试模型
	status=ProCmdActionAdd("模型测试命令",(uiCmdCmdActFn)funcMdlTest,
		uiCmdPrioDefault,UsrAccessCommonDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdMdlTestButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","模型变型测试","模型变型测试",
		"测试模型,以及关系式和事物特性表是否满足变型要求",NULL,PRO_B_TRUE,uiCmdMdlTestButton,Msg);
	sprintf(cmd_icon,"mdltest.gif");
	status=ProCmdIconSet(uiCmdMdlTestButton,cmd_icon);
    status = ProCmdDesignate (uiCmdMdlTestButton,"模型变型测试","测试模型,以及关系式和事物特性表是否满足变型要求","测试模型,以及关系式和事物特性表是否满足变型要求",Msg);

	//零件模型管理（按钮）
	//生成零件模型
	status=ProCmdActionAdd("生成零件模型命令",(uiCmdCmdActFn)funcPartManage,
		uiCmdPrioDefault,UsrAccessPartDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdPartManageButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","生成零件模型","生成零件模型",
		"构建当前模型的参数信息，添加零件模型到零件模型库",NULL,PRO_B_TRUE,uiCmdPartManageButton,Msg);
	sprintf(cmd_icon,"prtmdl.gif");
	status=ProCmdIconSet(uiCmdPartManageButton,cmd_icon);
    status = ProCmdDesignate (uiCmdPartManageButton,"生成零件模型","构建当前模型的参数信息，添加零件模型到零件模型库","构建当前模型的参数信息，添加零件模型到零件模型库",Msg);	
	
	//生成装配模型（按钮）
	status=ProCmdActionAdd("生成装配模型命令",(uiCmdCmdActFn)funcProductManage,
		uiCmdPrioDefault,AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&uiCmdProductManageButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","生成装配模型","生成装配模型",
		"构建当前装配模型的参数信息，添加装配模型到装配模型库",NULL,PRO_B_TRUE,uiCmdProductManageButton,Msg);
	sprintf(cmd_icon,"asmmdl.gif");
	status=ProCmdIconSet(uiCmdProductManageButton,cmd_icon);
    status = ProCmdDesignate (uiCmdProductManageButton,"生成装配模型","构建当前装配模型的参数信息，添加装配模型到装配模型库","构建当前装配模型的参数信息，添加装配模型到装配模型库",Msg);	

	////装配产品审核（按钮）
	status=ProCmdActionAdd("装配产品审核命令",(uiCmdCmdActFn)funcProductShenHe,
		uiCmdPrioDefault,AccessAvailable,PRO_B_TRUE,PRO_B_TRUE,&uiCmdProduceShenheButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","装配产品审核","装配产品审核",
		"将有零件和部件装配成的新产品交由审核模块",NULL,PRO_B_TRUE,uiCmdProduceShenheButton,Msg);

    //设置工程图明细表
	status=ProCmdActionAdd("明细表设置命令",(uiCmdCmdActFn)funcMdlDrawingSet,
		uiCmdPrioDefault,UsrAccessDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdDrawingSetButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","明细表设置","明细表设置",
		"对工程图的明细表进行设置",NULL,PRO_B_TRUE,uiCmdDrawingSetButton,Msg);
	sprintf(cmd_icon,"cadset.gif");
	status=ProCmdIconSet(uiCmdDrawingSetButton,cmd_icon);
    status = ProCmdDesignate (uiCmdDrawingSetButton,"明细表设置","对工程图的明细表进行设置","对工程图的明细表进行设置",Msg);	

	//有限元模型操作菜单添加
	status=ProMenubarmenuMenuAdd("MdlMenu","FemMenu","有限元建模",NULL,PRO_B_TRUE ,Msg);

	status=ProCmdActionAdd("有限元模式命令",(uiCmdCmdActFn)funcFemEnter,
		uiCmdPrioDefault,UsrAccessPartDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdFemEnter);
	status=ProMenubarmenuPushbuttonAdd("FemMenu","有限元模式","有限元模式",
		"进入有限元模式，设置有限元分析",NULL,PRO_B_TRUE,uiCmdFemEnter,Msg);

	status=ProCmdActionAdd("打开有限元模型命令",(uiCmdCmdActFn)funcFemOpen,
		uiCmdPrioDefault,UsrAccessPartDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdFemOpen);
	status=ProMenubarmenuPushbuttonAdd("FemMenu","打开有限元模型","打开有限元模型",
		"根据模型打开相应的有限元模型",NULL,PRO_B_TRUE,uiCmdFemOpen,Msg);

	status=ProCmdActionAdd("保存有限元模型命令",(uiCmdCmdActFn)funcFemSave,
		uiCmdPrioDefault,UsrAccessPartDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdFemSave);
	status=ProMenubarmenuPushbuttonAdd("FemMenu","保存有限元模型","保存有限元模型",
		"把有限元模型保存到有限元模型库",NULL,PRO_B_TRUE,uiCmdFemSave,Msg);


	//选项设置（按钮）
	status=ProCmdActionAdd("选项设置命令",(uiCmdCmdActFn)funcOption,
		uiCmdPrioDefault,UsrAccessDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdOptionButton);
	status=ProMenubarmenuPushbuttonAdd("MdlMenu","服务连接设置","服务连接设置",
		"数据库连接设置",NULL,PRO_B_TRUE,uiCmdOptionButton,Msg);


	//设置对话框，
	dlg_asm=NULL;			//装配尺寸、参数、关系
	dlg_part=NULL;			//零件尺寸、参数、关系
	dlg_pic=NULL;			//图片
	dlg_test=NULL;
	addComp_dlg=NULL;
	dlg_asm_def_test=NULL;
    //数据库连接信息
	//服务器连接信息
	GetDBAndFtpInfo();
    m_db=NULL;
   //设置全局的图标
	

    //对话框测试
	//选项设置（按钮）


	//uiCmdCmdId testCmd;
	//status=ProCmdActionAdd("测试命令",(uiCmdCmdActFn)TestFun,
	//	uiCmdPrioDefault,UsrAccessDefault,PRO_B_TRUE,PRO_B_TRUE,&testCmd);
	//status=ProMenubarmenuPushbuttonAdd("MdlMenu","测试","测试",
	//	"测试对话框",NULL,PRO_B_TRUE,testCmd,Msg);

	//////设置工作空间（按钮）
	////status=ProCmdActionAdd("工作空间管理命令",(uiCmdCmdActFn)workplaceman,
	////	uiCmdPrioDefault,UsrAccessDefault,PRO_B_TRUE,PRO_B_TRUE,&uiCmdWokePlaceManButton);
	////status=ProMenubarmenuPushbuttonAdd("MdlMenu","工作空间管理","工作空间管理",
	////	"对整个项目的建模工作进行管理",NULL,PRO_B_TRUE,uiCmdWokePlaceManButton,Msg);
	//   
	//status=ProMenubarmenuPushbuttonAdd("File","选项","选项",
	//	"数据库连接设置",NULL,PRO_B_TRUE,openfile,Msg);

   // ProCmdCmdIdFind("Ph_list.Filelist",&openfile);
//	ProCmdAccessFuncAdd(openfile,(uiCmdAccessFn)UsrAccessDefault,uiCmdOptionButton);
  //////////////////////////////////////////////////////////////////////////
	//status=ProMenubarMenuAdd ("AsmMenu", "装配建模",
	//	"Utilities", PRO_B_TRUE, Msg);
	////添加基本信息定义按钮
	//status=ProCmdActionAdd("装配模型定义",(uiCmdCmdActFn)AsmDefAcFn,
	//	uiCmdPrioDefault,AccessAvailable,
	//	PRO_B_TRUE,PRO_B_TRUE,&uiCmdAsmDefButton);

	//status=ProMenubarmenuPushbuttonAdd("AsmMenu", "基本信息定义1", "基本信息定义1",
	//	"定义模型的基本信息1", NULL,
	//	PRO_B_TRUE, uiCmdAsmDefButton, Msg);	
//////////////////////////////////////////////////////////////////////////
//添加右键菜单


//////////////////////////////////////////////////////////////////////////


	//设置数据库连接信息

    //尺寸参数编辑对话框，设置指针为空
	
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

	//消除Proe原来的打开按钮
	//,openfile
	//uiCmdAccessId access_id;
	//ProCmdCmdIdFind("ProCmdModelOpen",&openfile);
	//ProCmdAccessFuncAdd(openfile,(uiCmdAccessFn)UsrAccessUNAVAILABLE,&access_id);
	///*ProCmdActionAdd("ProCmdModelOpen",(uiCmdCmdActFn)funSamplePic,
	//uiCmdPrioDefault,UsrAccessUNAVAILABLE,PRO_B_TRUE,PRO_B_TRUE,&openfile);*/
	//,openfile


	//装配按钮改造,消除原来的装配按钮，添加一个装配按钮，支持从模型中进行装配的过程
  
	status=ProCmdActionAdd("装配命令",(uiCmdCmdActFn)funcAssemblyFromMdl,
		uiCmdPrioDefault,UsrAccessAVAILABLE_ASSEMBLY_MODE,PRO_B_TRUE,PRO_B_TRUE,&uiCmdAssemblyButton);
	status=ProMenubarmenuPushbuttonAdd("Insert.cb_component","模型","模型",
		"从变形设计的模型中获得零部件进行装配","pshCompAssemble",PRO_B_FALSE,uiCmdAssemblyButton,Msg);
	sprintf(cmd_icon,"compAssemblyFromMdl.gif");
	status=ProCmdIconSet(uiCmdAssemblyButton,cmd_icon);
    status = ProCmdDesignate (uiCmdAssemblyButton,"模型","从变形设计的模型中获得零部件进行装配","从变形设计的模型中获得零部件进行装配",Msg);

	UserPopupmenusSetup();
	return 0;
}
//结束函数
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
/* 全新新建                                                             */
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
/* 继承新建                                                             */
/************************************************************************/
void new2()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//CNewFromMdlDlg  dlg;
	//dlg.DoModal();
}
/************************************************************************/
/* 打开本地                                                             */
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
		,"PRO/ENGINEET 文件(*.prt *.asm *.drw)|*.prt*;*.asm*;*.drw*||");

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
			AfxMessageBox("打开文件失败");
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
/* 打开工作区文件                                                       */
/************************************************************************/
void funcMdlManage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	CMenuOpenDlg dlg_open;
	dlg_open.DoModal();
}
/************************************************************************/
/* 保存到本地                                                           */
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
/* 保存到工作区中                                                       */
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
/* 工作空间管理                                                         */
/************************************************************************/
void workplaceman()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	//CWorkPlaceManDlg dlg;
	//dlg.DoModal();
}
/************************************************************************/
/* 模型基本信息定义                                                     */
/************************************************************************/
void MdlDef()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	ProSolid		solid;
	MdlPropDefDlg	dlg;
	if(ProMdlCurrentGet((ProMdl*)&solid)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("当前不存在模型！");
		return;
	}	
	dlg.DoModal();
	UsrActivateCurrentWindow();
}
/************************************************************************/
/* 零件参数、尺寸、关系                                                 */
/************************************************************************/
void funcParaTalbe()//参数表函数
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
/* 装配参数、尺寸、关系                                                 */
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
/*//样图制作函数                                                        */
/************************************************************************/
void funSamplePic()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
    //下面的设置要用全局变量来进行
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
/* 生成零件模型函数                                                     */
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
/* 生成装配模型函数                                                     */
/************************************************************************/
void funcProductManage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	CAsmMdlManDlg		asm_man_dlg;
	asm_man_dlg.DoModal();	
	UsrActivateCurrentWindow();
}
//选项设置函数,设置数据库连接信息和服务器连接信息
void funcOption()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UsrClearMdlInfo();
	COptionSetDlg		option_dlg;
	option_dlg.DoModal();
	UsrActivateCurrentWindow();
}
//19.测试模型变型功能
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
/* 读取服务器，数据库连接信息                                           */
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

	DWORD     dwType;		//定义读取数据类型 REG_DWORD
	ReRootKey=HKEY_CURRENT_USER;              //注册表主键名称
	ReSubKey="Software\\PTC\\VariantDesign";  //欲打开注册表值的地址
	//ReExitProKey="Software\\PTC";             //如果键不存在则要创建
	//RegSubKeyToCreate="VariantDesign"         //创建的键
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

			////工作目录
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
				AfxMessageBox("注册表注册失败,请进行手工设置");
				return;
			}
			RegCloseKey(hk);
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
			COptionSetDlg option_dlg;
			option_dlg.DoModal();
		}

	}
	catch (...) {
		AfxMessageBox("从注册表读取数据失败");
	}

}
/************************************************************************/
/* 装配图                                                               */
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
/* 装配零部件，从模型中选取                                             */
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
//右键菜单,直接从又键把尺寸加入事物特性表
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
					//判断是否在关系式的左侧
					if (!g_RelCollection.IsDimParaInRelSetLeftPart(solid,NULL,(ProDimension*)&p_mdl_item))
					{
						flag=1;
						AfxMessageBox("所选择的尺寸已经在关系式的左侧,不能够作为事物特性表项添加!");
					}
					if(flag==0) 
					{
						UsrSWTXTable1 temp_UsrSWTXTable(solid,(ProDimension*)&p_mdl_item);
						p_UsrSWTXTableList.Add(temp_UsrSWTXTable);        //添加到数组中
					}
					break;
				case PRO_MDL_ASSEMBLY:
					//判断是否已经存在
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
						AfxMessageBox("所选择的尺寸已经在关系式的左侧,不能够作为事物特性表项添加!");
					}
					if(flag==0)
					{//被选中的尺寸加入事物特性表
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
	//判断当前有没有选中
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
			status = ProCmdActionAdd ("加入事物特性表命令",
				(uiCmdCmdActFn)AddTo,
				uiProe2ndImmediate, 
				UsrAccessDefault,
				PRO_B_FALSE, PRO_B_FALSE, &cmd_id);
			ProStringToWstring(label,"加入事物性表");
			ProStringToWstring(help,"把选中的尺寸加入模型的事物特性表");
			/*--------------------------------------------------------------------*\
			Add the button to the end of the popup menu.
			\*--------------------------------------------------------------------*/
			status = ProPopupmenuButtonAdd (menu_id, PRO_VALUE_UNUSED,
				"添加",
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
	//	"加入事物特性表",
	//	"加入事物特性表",
	//	"加入到事物特性表",
	//	NULL, PRO_B_TRUE, 
	//	cmd_id, Msg);
	status = ProNotificationSet (PRO_POPUPMENU_CREATE_POST,
		(ProFunction)UserPopupmenuNotification);
   return 0;
}
//////////////////////////////////////////////////////////////////////////
//用来设置全局图标的函数
void SetGImageList()
{
	g_ImageList1.Create(14, 14, ILC_COLOR32, 4, 4);
	g_ImageList1.Add(AfxGetApp()->LoadIcon(IDI_ICON1));  //UsrIconEnum.USR_ICON_TYPE
	g_ImageList1.Add(AfxGetApp()->LoadIcon(IDI_PRT));    //UsrIconEnum.USR_ICON_PRTMDL
	g_ImageList1.Add(AfxGetApp()->LoadIcon(IDI_PRT));    //UsrIconEnum.USR_ICON_ASMMDL
}
//20.把装配成的产品交变型模块审核
//此函数调用变型模块的DLL内的函数
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

//23.进入有限元模式
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
//24.打开有限元模型
#include "FemOpenDlg.h"
void funcFemOpen()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CFemOpenDlg  dlg;
	dlg.DoModal();
	/*UsrActivateCurrentWindow();*/
}
#include "FemSaveDlg.h"
//25.保存有限元模型
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