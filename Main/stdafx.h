// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once
#include "afxdisp.h"
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果目标平台的版本必须早于下面指定的平台，请修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows 95 和 Windows NT 4 或更高版本的功能。
#define WINVER 0x0400		// 将此更改为针对于 Windows 98 和 Windows 2000 或更高版本的合适的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows NT 4 或更高版本的功能。
#define _WIN32_WINNT 0x0400	// 将此更改为针对于 Windows 2000 或更高版本的合适的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将此更改为针对于 Windows Me 或更高版本的合适的值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 4.0 或更高版本的功能。
#define _WIN32_IE 0x0400	// 将此更改为针对于 IE 5.0 或更高版本的合适的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将为显式的


#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "afxctl.h"
#include <afxsock.h>		// MFC 套接字扩展
#include "afxtempl.h"
#include "ProToolkit.h"
#include "ProMenu.h"
#include "ProMenubar.h"
#include "ProMessage.h"
#include "ProUtil.h"
#include "ProUICmd.h"
#include <ProWindows.h>
#include "PartMdlRecordSet.h"
#include <ProParamval.h>
#include <ProSelection.h>
#include <ProSolid.h> 
#include <ProElement.h>
#include <ProParameter.h>
#include "ProMdl.h"
#include "ProFeatType.h"
#include <ProDtmPln.h>
#include <ProDrawing.h>
#include <ProDtmCsys.h>
#include <ProModelitem.h>
#include <ProRelSet.h>
#include <ProElement.h>
#include "ProElement.h"
#include <ProAsmcomp.h>
#include <ProAssembly.h>
#include <ProAsmcomppath.h>
#include <ProPopupmenu.h>
#include <ProDtlnote.h>
#include <ProSelbuffer.h> 
#include <ProNotify.h>
#include <afxdlgs.h>
#include <afxdhtml.h>
#include <ProDwgtable.h>
#include <windows.h>
#include "afxinet.h"
#include <afx.h> 
#include "UsrSWTXTable.h"
#include "Rel.h"
#include "RelColection.h"
#include "MyDatabase.h"
//////////////////////////////////////////////////////////////////////////
//对话框的图标
#define USR_SET_DLG_ICON() \
	ProMdl		 __mdl;\
	ProMdlType	 __type;\
	DlgIcon=IDI_PROE;\
	if (ProMdlCurrentGet(&__mdl)==PRO_TK_NO_ERROR) {\
	ProMdlTypeGet(__mdl,&__type); \
	if(__type==PRO_MDL_PART) DlgIcon=IDI_PRT;\
	if(__type==PRO_MDL_ASSEMBLY) DlgIcon=IDI_ASM;\
	if(__type==PRO_MDL_DRAWING) DlgIcon=IDI_DRAW;\
	}\
	HICON hIcon = AfxGetApp()->LoadIcon (DlgIcon);\
	SetIcon (hIcon, TRUE); \
//////////////////////////////////////////////////////////////////////////

#define ASM_MDL_PART    0
#define ASM_MDL_ASSEMBLY 1
#define ASM_NO_MDL       2
//////////////////////////////////////////////////////////////////////////
typedef enum usr_icon{
	USR_ICON_UNDEF =-1,
	USR_ICON_TYPE  =0,
	USR_ICON_PRTMDL=1,
	USR_ICON_ASMMDL=2,
	USR_ICON_INSPRT=3,
	USR_ICON_INSASM=4,	
	USR_ICON_FEAT_ROUND=7,
    USR_ICON_FEAT_GROUP_HEAD=8,
    USR_ICON_FEAT_SHELL=9,
    USR_ICON_FEAT_PROTRUSION=10,//USR_ICON_CUT=
    USR_ICON_FEAT_PATTERN_HEAD=12,
    USR_ICON_FEAT_DATUM=13,
    USR_ICON_FEAT_DATUM_POINT=14,
	USR_ICON_FEAT_DATUM_AXIS=15,
	USR_ICON_FEAT_CSYS=16,
	USR_ICON_FEAT_HOLE=17,
	USR_ICON_FEAT_CHAMFER=18,
	USR_ICON_FEATFEAT_RIB=19
}UsrIconEnum;

//////////////////////////////////////////////////////////////////////////
//检查当前的模型与数据库的中的是否一致
typedef enum check_current_mdl{
	CHECK_NAME_NUM_SAME=0,   //名称和编号相同
	CHECK_SNAME_NNUM=1,      //同名但是不同编号
	CHECK_NNAME_SNUM=2,      //不同名但是同编号
	CHECK_NO_MDL=3,
}UsrCheckCurrentMdlState;
//////////////////////////////////////////////////////////////////////////


typedef struct relationtalbe_item{
	ProMdl      owner;
	CString     rel;
	CString     note;
	int         isCheck;
}UsrRelTable;
typedef struct picture_item{
	ProMdl  owner;
	CString fileName;
	CString Note;
	CString filePath;   //图片所在的文件夹，"G:\\ABC\\" 注意：格式
	int isCheck;
	int isMain;
}UsrPicture;

typedef struct  perfparatable_item{
	ProMdl		owner;              //用于判断所属的项
	CString		Name;              //用户重新命名
	CString		Type;              //参数的类型，取值"str""dbl"
	CString		value;
	CString		Note;
	double		MinValue;
	double		MaxValue;
	int			isCheck;               //判断该项是否还有效
}UsrPerfParaTable;
/************************************************************************/
/* 零部件模型的情况
  1.类型:模型/零件实例*/
/************************************************************************/
typedef struct partPropitem {
	ProMdl  owner;             //模型的句柄
	long    Number;            //模型的标识号,默认为－1，即没有建模
	CString Name;              //模型的名称，默认与文件名相同，可以重新定义
	CString Code;              //编码

	//long    PrtNum;            //如果是具体的零件过来的，则必须记录零件的编号，默认值为-1，即处在建模状态
	//                           //如果这个值不是－1，则记录了模型的哪个实例，Number是必须要得
	//                           //在模型号是－1的情况，这个值不用考虑

	
	int     MdlType;           //判断类别是否为-1(什么都不是)1(有模型的实例)2(无模型的零件或组件)3(是模型)
	long    MdlNum;            //如果是有模型的实例，则取值为模型的编号,否则取-1
	CString MdlName;           //如果是有模型的实例,则区模型的名称,这个名称与编号一致 ,默认为 ""

	long    TypeNum;           //模型所属类别的标识号,,默认为－1，即没有建模
	CString TypeName;          //模型所属的类别名称	,以TypeNum为准




	bool    bHaveDraw;         //是否有工程，工程图只允许出现一个文件
    ProMdl  hDraw;             //工程图的句柄

	CString Person;            //建模人员

	CString Material;          //材料

	CString Note;              //备注

	CString pic_path;          //主图的位置,g:\ag\pic001.jpg  路径＋文件名

	bool    IsAudited;         //是默认为false       否通过审核
	int     ResourceType;      //默认为0 零部件来源  0本地文件系统   1设计系统服务器 2PDM系统

	bool    IsAsmComp;         //默认为false 是否为装配件的部件   对于装配的部件将不允许对零部件建模系统进行操作
}UsrPartPorp;




//typedef struct draw_item{
//	ProMdl    owner;           //工程图的模型句柄
//	int       isHave;          //是否有工程图
//	ProDrawing hDraw;          //工程图的句柄
//	CString   filename;        //工程图的文件名
//	CString   fileAddress;     //工程图的存放位置
//	CString   note;            //定义工程图的所包括的模型
//}UsrDrawingItem;




//CString strFtp, CString strFtpUser, CString strFtpPwd,int iFtpPort, CString strDirName
typedef struct ftp_info {
	CString strFtp;
	CString strFtpUser;
	CString strFtpPwd;
    int iFtpPort;
}FtpConnectInfo;


typedef struct insInfo {
	CString Name;
	CString Code;
	CString Person;
	CString Dept;
	CString Note;
	long    mdlNum;
	CTime   m_time;

}MdlInsInfo;


#define  TYPE_DIM   0
#define  TYPE_PARA  1
#define  TYPE_FEAT  2
#define  TYPE_COMP  3
//位操作，清楚模型某些信息
#define  ERASE_ALL   63    //0111111清楚所有
#define  ERASE_SWTX  1     //0000001事物特性表
#define  ERASE_REL   2     //0000010关系式
#define  ERASE_PERF  4     //0000100性能参数表
#define  ERASE_PIC   8     //0001000图片信息
#define  ERASE_DRW   16    //0010000工程图
#define  ERASE_COMP  32    //0100000元件信息 
//定义装配的事物特性表项
typedef struct asm_sml_item{
	ProSolid            owner;          //装配最上层，不同与单个零件
	CString             name;
	int                 marsk;          //类型，TYPE_DIM,TYPE_PARA,TYPE_FEAT,TYPE_COMP
	ProDimension        dim;            //尺寸项
	ProParameter        para;           //参数项
	CString             note;
}AsmSmlItem,*AsmSmlItemList;
//元件
typedef struct comp_ins_item{
	int                 insNum;
	CString             insName;
	CString             insCode;
	CString             insNote;
}CompInsItem,*CompInsItemList;

//元件信息，还要与UsrPartPorp结合来表达元件,实在从模型中装配进来时进行添加
typedef struct asm_comp_var_item{
	ProSolid            owner;          //装配件句柄
	ProSolid            comp;           //元件所属零件句柄
	int                 propNum;        //元件的属性编号（结构体的编号）,元件所对应的模型的编号(注：要考虑在删除模型)
	int                 type;           // ASM_MDL_PART(零件模型)   
	                                    //ASM_MDL_ASSEMBLY(装配模型) ASM_NO_MDL(非模型产生)，不同类型在进行数据库查询的时候是不同的
	long                insNum;         //缺省系列（实例）号
	long                mdlNum;         //模型的编号
	CArray<CompInsItem,CompInsItem> *   varFeild;       //元件的变型范围                
}AsmCompVarItem,*AsmCompVarItemList;


typedef struct asm_rel_item{
	ProSolid            owner;          //关系所属的模型
	CString             relation;       //关系式
	CString             note;           //关系的说明
	//ProSolid          mdl;    //关系所属的元件
}AsmRelItem,*AsmRelItemList;
//工程图中的自定义消息
#define   WM_LIST_OK (WM_USER+2001)
//判断模型是否为另一模型的元件
extern BOOL IsSolid1InSolid2(ProSolid comp,ProSolid asm_solid);
//获得模型类型
extern ProMdlType GetMdlType(ProMdl mdl);
//模型备份函数，把当前模型备份到制定的目录
extern ProError UsrMdlBackUp(ProMdl mdl,ProMdldata * data);
//零件特征访问函数
extern ProError partFeatVisAc(ProFeature* p_feature,ProError status,CArray<ProFeature,ProFeature> * app_data);
//零件参数访问函数
extern ProError partParaVisAc(ProParameter* p_parameter,ProError status,
					   CArray<ProParameter,ProParameter> * app_data);
//特征访问动作函数函数
extern ProError FeatureVisitAction(ProFeature *p_object,ProError status,
							ProAppData app_data);
//特征访问过滤函数
extern ProError FeatureFilterAction(ProFeature *p_object,
							 ProAppData app_data);
//装配件特征访问过滤函数
extern ProError FeatureAsmFilterAction(ProFeature *p_object,
								ProAppData app_data);
//模型特征访问函数
extern ProError  SgCollectFeat(ProSolid solid,			//(In)当前模型
						ProFeature **p_data,BOOL nFlag=TRUE);
//特征尺寸访问函数
extern ProError  FeatureCollectDim(ProFeature feature,			//(In)指定特征
							ProDimension **p_data);
//模型尺寸访问函数
extern ProError  SgCollectDim(ProSolid solid,			//(In)指定模型
					   ProDimension **p_data)	;//(In)尺寸对象数组
//特征尺寸访问动作函数
extern ProError FeatureDimensionVisitAction(ProDimension *p_object,ProError status,
									 ProAppData app_data) ;
//特征尺寸过滤函数
extern ProError FeatureDimensionFilterAction(ProDimension *p_object,
									  ProAppData app_data) ;
//比较两个参数是否相等
extern BOOL ComparaParameter(ProParameter * para1,ProParameter * para2);
//比较两个尺寸是否相等
extern BOOL ComparaDimension(ProDimension * para1,ProDimension * para2);
/////////////////////////////////////////////
//一下为参数，尺寸中事务特性表用到的函数

extern ProError FeatureAsmVisitAcFn(ProFeature *p_object,ProError status,
							 CArray<ProFeature,ProFeature> * app_data) ;
extern ProError FeatureAsmFilterAcFn(ProFeature *p_object,
							  CArray<ProFeature,ProFeature> * app_data) ;
//特征的尺寸访问函数，同MAIN, 可以整合
extern ProError partFeatDimVisAc(ProDimension *dimension,ProError status,
						  CArray<ProDimension,ProDimension> * app_data);
extern ProError partAsmParaVisAc(ProParameter* p_parameter,ProError status,
						  CArray<ProParameter,ProParameter> * app_data);


//////////////////////////////////////////////
extern void AsmSolidDimVisFn(ProSolid solid,CStringArray * dimlist );
//////////////////////////////////////////////////////////////////////////
extern BOOL CheckEquation(CString Expression,CStringArray * importDim,CStringArray * exportDim);
extern BOOL CheckExpression(CString Expression,CStringArray * importDim,CStringArray * exportDim);
extern BOOL IsDigit(int c);
extern BOOL IsDouble(CString str);
//////////////////////////////////////////////////////////////////////////
//关系式访问动作函数
extern ProError UsrSolidRelsetVisAc(ProRelset *p_relset
							 ,CArray<ProRelset,ProRelset>* caller_data);
//访问模型中的关系式
extern BOOL VisitSolidRel(ProSolid solid,CStringArray * relSet);
//给以个零件关系，后面添加上后缀，用于从零件模型中获得关系时，添加到组件中使用
extern void  ReplaceDimWithPostID(CString * str,CString postid);

//////////////////////////////////////////////////////////////////////////
extern int UsrCreatDefDtm(ProDtmplnConstrType pro_dtmpln_def,ProName dtm_name,ProGeomitem *pln);
extern void UsrGeomitemPlnGet(ProFeature feature,ProGeomitem *geomitem);
extern ProError GemitemAction(ProGeomitem* p_geomitems,ProError status,ProAppData app_data);

//extern ProGeomitem front, top, right,front1,edges,*p_edges,axis;
extern bool CreatDefDtm(void);
///////////////////////////////////////////////////////////////////////////////////
//设置模型的基本信息
extern void  IsMdlInfoDefAndDef(ProSolid mdl);
//修改指定的模型信息
extern UsrPartPorp * SetMdlInfo(ProMdl mdl);
extern UsrPartPorp  GetMdlInfo(ProMdl mdl);
extern UsrPartPorp * GetMdlPInfo(ProMdl mdl);
extern void  GetMdlProp(ProSolid solid,int * indexNo);
extern BOOL GetMdlPropWithFileName(CString filename,ProSolid * solid,int * indexNo);
/************************************************************************/
/* 获得装配件的元件的句柄                                               */
/************************************************************************/
extern void  GetAsmCompHand(ProSolid solid,CArray<ProSolid,ProSolid> * p_solidlist);
/************************************************************************/
/* 获得组件下所有的零件清单，没有部件                                    /
/************************************************************************/
extern void  GetAsmPartCompHand(ProSolid solid
								,CArray<ProSolid,ProSolid> * p_solidlist,CArray<int,int> * numlist);
extern BOOL CheckAsmSWTX(ProSolid asm_solid,AsmSmlItem * p_asmSwtx);
//////////////////////////////////////////////////////////////////////////
//获得ID表
extern void GetSolidIDTable(ProSolid asm_solid,CArray<ProFeature,ProFeature> * p_asm_feat_list
					 ,HTREEITEM hItem,CTreeCtrl * p_Tree,CArray<int,int> * p_id);
//////////////////////////////////////////////////////////////////////////
extern bool RemoveDireAndFile(CString localdir);//移除本地目录

//////////////////////////////////////////////////////////////////////////
//工程图

extern void GetDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList,CArray<int,int> * p_IndexList);
extern void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);

extern ProMdl GetCurrentMdl();
//////////////////////////////////////////////////////////////////////////
//零件。组件模型树
extern void AddPartSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item);
extern BOOL ShowPartTree(CTreeCtrl * m_Tree,CDatabase * p_db);
extern void AddAsmSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item);
extern BOOL ShowAsmTree(CTreeCtrl * m_Tree,CDatabase * p_db);
//打开用，不显示模型，只显示类别
extern void AddAsmSubItemForOpen(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item);
extern BOOL ShowAsmTreeForOpen(CTreeCtrl * m_Tree,CDatabase * p_db);
extern void AddPartSubItemForOpen(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item);
extern BOOL ShowPartTreeForOpen(CTreeCtrl * m_Tree,CDatabase * p_db);
//临时工作模型树
extern void AddTempAsmSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM hItem);
extern void AddTempPartSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM hItem);
extern BOOL ShowTempAsmTree(CTreeCtrl * m_Tree,CDatabase * p_db);
extern BOOL ShowTempPartTree(CTreeCtrl * m_Tree,CDatabase * p_db);
//////////////////////////////////////////////////////////////////////////
//变形设计
extern BOOL VarantFn(CString str,ProSolid solid,CString mdlNum,long insNum );
//获得模型的实例信息
extern BOOL GetMdlInsInfo(CDatabase * p_db,int Type,long MdlNum,long InsNum,CompInsItem * item);
extern BOOL ConvertUsrPropNumToCompInsItemNum(int propNum,int * item);

//特征图像
extern int GetFeatIconNum(int type);
extern void SetImageListForFeatrure(CImageList * p_imageList);
//激活当前窗口
extern ProError UsrActivateCurrentWindow();
//重绘当前窗口    
extern ProError UsrRepaintWindow();
//清楚模型信息
extern void EraseMdlInfo(ProMdl mdl,UINT marsk);
//获得当前模型的全路径
extern CString GetCurrentMdlPath(void);
//获得制定模型的全路径
extern CString GetMdlPath(ProMdl mdl);
//获得模型的文件名
extern CString GetMdlFileName(ProMdl mdl);
//获得模型的全路径
extern CString GetMdlFullFileName(ProMdl mdl);
//设置模型的属性
extern BOOL SetMdlInfo(ProSolid mdl,UsrPartPorp newPorp);
//判断一个模型是否被删除，如果返回true则是被删除
extern bool IsMdlDeleted(ProMdl mdl);
/************************************************************************/
/* 检查模型是否有被删除，如果有则清楚信息                               */
/************************************************************************/
extern void UsrClearMdlInfo();
//根据文件名来判断，这个文件在当前模型是否已经存在
extern bool IsMdlInSession(CString ,ProMdlType /*type=PRO_MDL_PART*/,ProMdl * );
/************************************************************************/
/* 获得制定模型，制定名称的尺寸和参数项
type=TYPE_DIM TYPE_PARA                                               */
/************************************************************************/
extern BOOL GetDimPara(ProSolid solid=NULL,ProName name=NULL,ProDimension * dim=NULL,ProParameter * para=NULL,int * type=NULL);
extern CString UsrGetParaValue(ProParameter * para=NULL/*参数*/,CString * Type=NULL);
/************************************************************************/
/* 获得参数的父项句柄                                                   */
/************************************************************************/
extern ProSolid UsrGetParameterOwner(ProParameter * para);
/************************************************************************/
/* 创建领事目录(零件) ,组件                                                  */
/************************************************************************/
extern bool CreateTempDir(ProSolid solid,CString * csPathName);
extern BOOL CreateTempAsmDirectory(ProSolid asm_solid,CString * csPathName);
extern BOOL HaveMdlCurrent();

//对数据库的图片进行操作
extern BOOL  DBSavePic(CLongBinary * m_lobFile,CString FileFullName,CRecordset * m_Set,CString & error_info);
extern BOOL  DBOpenPic(CLongBinary * m_lobFile,CString  FileFullName/*输入,可以为NULL*/,CString & error_info,IPicture ** pic/*如果不想可以为NULL*/);
extern BOOL  DBOpenPicSaveAsFile(CLongBinary * m_lobFile,CString  FileFullName);

//获得尺寸和参数值,一次只能输入一个,另一个为空
extern double UsrGetDimParaValue(ProDimension * dim=NULL/*尺寸*/
						  ,ProParameter * para=NULL/*参数*/);

//判断本地文件是否存在
extern bool IsFilesExists(CString csPath,CString csFilter
				   ,CStringArray & files/*文件名称*/,CString & csError);
//判断本地文件是否存在
extern bool IsFilesExists(CStringArray & filesFullPath/*文件名称*/,CString & csError);
extern UsrCheckCurrentMdlState  UsrCheckCurrentMdl(ProMdlType mdltype,CString filename/*包括扩展符号*/,long num,ProMdl * mdl=NULL);
//////////////////////////////////////////////////////////////////////////
//显示目录结构的函数
//显示零件模型的目录树结构
extern BOOL ShowNPartTree(CTreeCtrl * m_Tree,CDatabase * p_db,bool IfNeedMdl=false);
//显示组件模型的目录结构
extern BOOL ShowNAsmTree(CTreeCtrl * m_Tree,CDatabase * p_db,bool IfNeedMdl=false);
//显示零件实例
extern BOOL ShowPartTypeTree(CTreeCtrl * m_Tree,CDatabase * p_db);
//给定路径，路径后面需要"\\"，以及文件夹的初始名，根据名字建立文件夹
extern BOOL CreateDirectoryGivenPathAndName(CString csPath,CString inName,CString & outName);
//选择目录
extern bool  SelectAnDirectory(CString & selDir,CString defDir);
//把零件的模型信息添加到零件
extern BOOL UsrAddPartMdlOtherInfo(CDatabase * p_db,ProMdl mdl,long PrtNum);
//删除零件模型
extern BOOL  UsrDelPartMdl(CDatabase * p_db,long num);
//删除组件模型
extern BOOL  UsrDelAsmMdl(CDatabase * p_db,long num);
//把组件模型的信息添加进来
////solidlist与comnumlist是模型与BOM表中的编号的对应
//extern BOOL UsrAddAsmMdlOtherInfo(CDatabase * p_db,ProMdl asm_solid
//						,long asm_num/*,CArray<ProSolid,ProSolid> & solidlist
//						,CArray<long,long> & comnumlist*/);
//显示模型尺寸
extern ProError UsrShowDim(ProSolid solid,ProDimension  * dim);
//获得元件的元件表
extern bool UsrGetSolidComppath(ProSolid assmbely,ProSolid comp,CArray<int,int> & p_IDTalbe);
//检查模型的是否已经被使用
extern bool CheckIfCodeHavebeenUsed(long/* TypeNum=-1*/,CString/* code*/,CDatabase *);
//把文件都导入内存,文件必须是全路径的
extern bool UsrMdlLoad(CStringArray & files,ProMdl ** mdllist);

//丛pdm寻找零件、部件的存放位置信息
extern BOOL UsrGetPartAndProductFromPdm(CString PdmNum,int type,CString & d3file,CString & d2file);
////0.全局变量  工程图
//extern CArray<UsrDrawingItem,UsrDrawingItem> P_UsrDrawingList;
//定义全局变量
//1.零件的事物特性表
extern CArray<UsrSWTXTable1,UsrSWTXTable1>  p_UsrSWTXTableList;  //定义全局的事物特性表列项，
//在程序运行时，要给他一个初始值(无效)
//2.零件的关系
//extern  CArray<UsrRelTable,UsrRelTable>    p_UsrRelTableList;   //定义全局的关系列表项数组
//3.零件的图片
extern CArray<UsrPicture,UsrPicture>   p_UsrPicture;           //图片的全局变量    
//4.零件的性能参数
extern CArray<UsrPerfParaTable,UsrPerfParaTable> p_UsrPerfParaTableList;//定义的性能参数表的全局变量

//5.模型的基本属性 注：零件模块和装配模块公用  在删除某一项之前，要判断组件元件中的propnum对其引用，应该相应修改
extern CArray<UsrPartPorp,UsrPartPorp> p_UsrPartPorp;          //定义模型基本信息的全局变量

//6.装配的事物特性
extern CArray<AsmSmlItem,AsmSmlItem> p_AsmSmlItemList;
//7.装配的关系列表
//extern CArray<AsmRelItem,AsmRelItem> p_AsmRelItemList;
//extern CArray<CRel,CRel&>            p_CRelItemList;
extern CRelColection g_RelCollection;
//8.装配的元件的变形范围
extern CArray<AsmCompVarItem,AsmCompVarItem> p_AsmCompItemList;
//9.ftp
extern FtpConnectInfo myFtp;
//10.图标
extern int  DlgIcon;//=IDI_PROE;  //设置对话框的标题图标
//11.数据库连接
extern CString         dbLinkInfo;
extern CString			dbPdmLinkInfo;
//12.全局的图标函数
extern CImageList      g_ImageList1;  //大图标
extern CImageList      g_ImageList2;  //小图标


typedef struct Info      
{
	ProSolid  solid;         //零件或部件句柄
	int       SolidID;           //零件号或者部件号
	CString   Type;            //类型，是部件还是零件，零件=“prt",部件=“asm”；
}Info;




//----------------------------------wshf--------------------------------
CString wshfstrDbOpen;
CMyDatabase wshfm_db;

typedef struct UserInfo 
{
	CString name;
	CString authority;
}UserInfo;
//用户权限结构体
typedef struct UserRight
{
	CString right;
}UserRight;

UserInfo curUserInfo;
CArray<UserRight,UserRight> curUserRights;
UserRight useright;