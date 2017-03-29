// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once
#include "afxdisp.h"
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ���Ŀ��ƽ̨�İ汾������������ָ����ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows 95 �� Windows NT 4 ����߰汾�Ĺ��ܡ�
#define WINVER 0x0400		// ���˸���Ϊ����� Windows 98 �� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows NT 4 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0400	// ���˸���Ϊ����� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ���˸���Ϊ����� Windows Me ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 4.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0400	// ���˸���Ϊ����� IE 5.0 ����߰汾�ĺ��ʵ�ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯����Ϊ��ʽ��


#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "afxctl.h"
#include <afxsock.h>		// MFC �׽�����չ
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
//�Ի����ͼ��
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
//��鵱ǰ��ģ�������ݿ���е��Ƿ�һ��
typedef enum check_current_mdl{
	CHECK_NAME_NUM_SAME=0,   //���ƺͱ����ͬ
	CHECK_SNAME_NNUM=1,      //ͬ�����ǲ�ͬ���
	CHECK_NNAME_SNUM=2,      //��ͬ������ͬ���
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
	CString filePath;   //ͼƬ���ڵ��ļ��У�"G:\\ABC\\" ע�⣺��ʽ
	int isCheck;
	int isMain;
}UsrPicture;

typedef struct  perfparatable_item{
	ProMdl		owner;              //�����ж���������
	CString		Name;              //�û���������
	CString		Type;              //���������ͣ�ȡֵ"str""dbl"
	CString		value;
	CString		Note;
	double		MinValue;
	double		MaxValue;
	int			isCheck;               //�жϸ����Ƿ���Ч
}UsrPerfParaTable;
/************************************************************************/
/* �㲿��ģ�͵����
  1.����:ģ��/���ʵ��*/
/************************************************************************/
typedef struct partPropitem {
	ProMdl  owner;             //ģ�͵ľ��
	long    Number;            //ģ�͵ı�ʶ��,Ĭ��Ϊ��1����û�н�ģ
	CString Name;              //ģ�͵����ƣ�Ĭ�����ļ�����ͬ���������¶���
	CString Code;              //����

	//long    PrtNum;            //����Ǿ������������ģ�������¼����ı�ţ�Ĭ��ֵΪ-1�������ڽ�ģ״̬
	//                           //������ֵ���ǣ�1�����¼��ģ�͵��ĸ�ʵ����Number�Ǳ���Ҫ��
	//                           //��ģ�ͺ��ǣ�1����������ֵ���ÿ���

	
	int     MdlType;           //�ж�����Ƿ�Ϊ-1(ʲô������)1(��ģ�͵�ʵ��)2(��ģ�͵���������)3(��ģ��)
	long    MdlNum;            //�������ģ�͵�ʵ������ȡֵΪģ�͵ı��,����ȡ-1
	CString MdlName;           //�������ģ�͵�ʵ��,����ģ�͵�����,�����������һ�� ,Ĭ��Ϊ ""

	long    TypeNum;           //ģ���������ı�ʶ��,,Ĭ��Ϊ��1����û�н�ģ
	CString TypeName;          //ģ���������������	,��TypeNumΪ׼




	bool    bHaveDraw;         //�Ƿ��й��̣�����ͼֻ�������һ���ļ�
    ProMdl  hDraw;             //����ͼ�ľ��

	CString Person;            //��ģ��Ա

	CString Material;          //����

	CString Note;              //��ע

	CString pic_path;          //��ͼ��λ��,g:\ag\pic001.jpg  ·�����ļ���

	bool    IsAudited;         //��Ĭ��Ϊfalse       ��ͨ�����
	int     ResourceType;      //Ĭ��Ϊ0 �㲿����Դ  0�����ļ�ϵͳ   1���ϵͳ������ 2PDMϵͳ

	bool    IsAsmComp;         //Ĭ��Ϊfalse �Ƿ�Ϊװ����Ĳ���   ����װ��Ĳ�������������㲿����ģϵͳ���в���
}UsrPartPorp;




//typedef struct draw_item{
//	ProMdl    owner;           //����ͼ��ģ�;��
//	int       isHave;          //�Ƿ��й���ͼ
//	ProDrawing hDraw;          //����ͼ�ľ��
//	CString   filename;        //����ͼ���ļ���
//	CString   fileAddress;     //����ͼ�Ĵ��λ��
//	CString   note;            //���幤��ͼ����������ģ��
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
//λ���������ģ��ĳЩ��Ϣ
#define  ERASE_ALL   63    //0111111�������
#define  ERASE_SWTX  1     //0000001�������Ա�
#define  ERASE_REL   2     //0000010��ϵʽ
#define  ERASE_PERF  4     //0000100���ܲ�����
#define  ERASE_PIC   8     //0001000ͼƬ��Ϣ
#define  ERASE_DRW   16    //0010000����ͼ
#define  ERASE_COMP  32    //0100000Ԫ����Ϣ 
//����װ����������Ա���
typedef struct asm_sml_item{
	ProSolid            owner;          //װ�����ϲ㣬��ͬ�뵥�����
	CString             name;
	int                 marsk;          //���ͣ�TYPE_DIM,TYPE_PARA,TYPE_FEAT,TYPE_COMP
	ProDimension        dim;            //�ߴ���
	ProParameter        para;           //������
	CString             note;
}AsmSmlItem,*AsmSmlItemList;
//Ԫ��
typedef struct comp_ins_item{
	int                 insNum;
	CString             insName;
	CString             insCode;
	CString             insNote;
}CompInsItem,*CompInsItemList;

//Ԫ����Ϣ����Ҫ��UsrPartPorp��������Ԫ��,ʵ�ڴ�ģ����װ�����ʱ�������
typedef struct asm_comp_var_item{
	ProSolid            owner;          //װ������
	ProSolid            comp;           //Ԫ������������
	int                 propNum;        //Ԫ�������Ա�ţ��ṹ��ı�ţ�,Ԫ������Ӧ��ģ�͵ı��(ע��Ҫ������ɾ��ģ��)
	int                 type;           // ASM_MDL_PART(���ģ��)   
	                                    //ASM_MDL_ASSEMBLY(װ��ģ��) ASM_NO_MDL(��ģ�Ͳ���)����ͬ�����ڽ������ݿ��ѯ��ʱ���ǲ�ͬ��
	long                insNum;         //ȱʡϵ�У�ʵ������
	long                mdlNum;         //ģ�͵ı��
	CArray<CompInsItem,CompInsItem> *   varFeild;       //Ԫ���ı��ͷ�Χ                
}AsmCompVarItem,*AsmCompVarItemList;


typedef struct asm_rel_item{
	ProSolid            owner;          //��ϵ������ģ��
	CString             relation;       //��ϵʽ
	CString             note;           //��ϵ��˵��
	//ProSolid          mdl;    //��ϵ������Ԫ��
}AsmRelItem,*AsmRelItemList;
//����ͼ�е��Զ�����Ϣ
#define   WM_LIST_OK (WM_USER+2001)
//�ж�ģ���Ƿ�Ϊ��һģ�͵�Ԫ��
extern BOOL IsSolid1InSolid2(ProSolid comp,ProSolid asm_solid);
//���ģ������
extern ProMdlType GetMdlType(ProMdl mdl);
//ģ�ͱ��ݺ������ѵ�ǰģ�ͱ��ݵ��ƶ���Ŀ¼
extern ProError UsrMdlBackUp(ProMdl mdl,ProMdldata * data);
//����������ʺ���
extern ProError partFeatVisAc(ProFeature* p_feature,ProError status,CArray<ProFeature,ProFeature> * app_data);
//����������ʺ���
extern ProError partParaVisAc(ProParameter* p_parameter,ProError status,
					   CArray<ProParameter,ProParameter> * app_data);
//�������ʶ�����������
extern ProError FeatureVisitAction(ProFeature *p_object,ProError status,
							ProAppData app_data);
//�������ʹ��˺���
extern ProError FeatureFilterAction(ProFeature *p_object,
							 ProAppData app_data);
//װ����������ʹ��˺���
extern ProError FeatureAsmFilterAction(ProFeature *p_object,
								ProAppData app_data);
//ģ���������ʺ���
extern ProError  SgCollectFeat(ProSolid solid,			//(In)��ǰģ��
						ProFeature **p_data,BOOL nFlag=TRUE);
//�����ߴ���ʺ���
extern ProError  FeatureCollectDim(ProFeature feature,			//(In)ָ������
							ProDimension **p_data);
//ģ�ͳߴ���ʺ���
extern ProError  SgCollectDim(ProSolid solid,			//(In)ָ��ģ��
					   ProDimension **p_data)	;//(In)�ߴ��������
//�����ߴ���ʶ�������
extern ProError FeatureDimensionVisitAction(ProDimension *p_object,ProError status,
									 ProAppData app_data) ;
//�����ߴ���˺���
extern ProError FeatureDimensionFilterAction(ProDimension *p_object,
									  ProAppData app_data) ;
//�Ƚ����������Ƿ����
extern BOOL ComparaParameter(ProParameter * para1,ProParameter * para2);
//�Ƚ������ߴ��Ƿ����
extern BOOL ComparaDimension(ProDimension * para1,ProDimension * para2);
/////////////////////////////////////////////
//һ��Ϊ�������ߴ����������Ա��õ��ĺ���

extern ProError FeatureAsmVisitAcFn(ProFeature *p_object,ProError status,
							 CArray<ProFeature,ProFeature> * app_data) ;
extern ProError FeatureAsmFilterAcFn(ProFeature *p_object,
							  CArray<ProFeature,ProFeature> * app_data) ;
//�����ĳߴ���ʺ�����ͬMAIN, ��������
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
//��ϵʽ���ʶ�������
extern ProError UsrSolidRelsetVisAc(ProRelset *p_relset
							 ,CArray<ProRelset,ProRelset>* caller_data);
//����ģ���еĹ�ϵʽ
extern BOOL VisitSolidRel(ProSolid solid,CStringArray * relSet);
//���Ը������ϵ����������Ϻ�׺�����ڴ����ģ���л�ù�ϵʱ����ӵ������ʹ��
extern void  ReplaceDimWithPostID(CString * str,CString postid);

//////////////////////////////////////////////////////////////////////////
extern int UsrCreatDefDtm(ProDtmplnConstrType pro_dtmpln_def,ProName dtm_name,ProGeomitem *pln);
extern void UsrGeomitemPlnGet(ProFeature feature,ProGeomitem *geomitem);
extern ProError GemitemAction(ProGeomitem* p_geomitems,ProError status,ProAppData app_data);

//extern ProGeomitem front, top, right,front1,edges,*p_edges,axis;
extern bool CreatDefDtm(void);
///////////////////////////////////////////////////////////////////////////////////
//����ģ�͵Ļ�����Ϣ
extern void  IsMdlInfoDefAndDef(ProSolid mdl);
//�޸�ָ����ģ����Ϣ
extern UsrPartPorp * SetMdlInfo(ProMdl mdl);
extern UsrPartPorp  GetMdlInfo(ProMdl mdl);
extern UsrPartPorp * GetMdlPInfo(ProMdl mdl);
extern void  GetMdlProp(ProSolid solid,int * indexNo);
extern BOOL GetMdlPropWithFileName(CString filename,ProSolid * solid,int * indexNo);
/************************************************************************/
/* ���װ�����Ԫ���ľ��                                               */
/************************************************************************/
extern void  GetAsmCompHand(ProSolid solid,CArray<ProSolid,ProSolid> * p_solidlist);
/************************************************************************/
/* �����������е�����嵥��û�в���                                    /
/************************************************************************/
extern void  GetAsmPartCompHand(ProSolid solid
								,CArray<ProSolid,ProSolid> * p_solidlist,CArray<int,int> * numlist);
extern BOOL CheckAsmSWTX(ProSolid asm_solid,AsmSmlItem * p_asmSwtx);
//////////////////////////////////////////////////////////////////////////
//���ID��
extern void GetSolidIDTable(ProSolid asm_solid,CArray<ProFeature,ProFeature> * p_asm_feat_list
					 ,HTREEITEM hItem,CTreeCtrl * p_Tree,CArray<int,int> * p_id);
//////////////////////////////////////////////////////////////////////////
extern bool RemoveDireAndFile(CString localdir);//�Ƴ�����Ŀ¼

//////////////////////////////////////////////////////////////////////////
//����ͼ

extern void GetDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList,CArray<int,int> * p_IndexList);
extern void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);

extern ProMdl GetCurrentMdl();
//////////////////////////////////////////////////////////////////////////
//��������ģ����
extern void AddPartSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item);
extern BOOL ShowPartTree(CTreeCtrl * m_Tree,CDatabase * p_db);
extern void AddAsmSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item);
extern BOOL ShowAsmTree(CTreeCtrl * m_Tree,CDatabase * p_db);
//���ã�����ʾģ�ͣ�ֻ��ʾ���
extern void AddAsmSubItemForOpen(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item);
extern BOOL ShowAsmTreeForOpen(CTreeCtrl * m_Tree,CDatabase * p_db);
extern void AddPartSubItemForOpen(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item);
extern BOOL ShowPartTreeForOpen(CTreeCtrl * m_Tree,CDatabase * p_db);
//��ʱ����ģ����
extern void AddTempAsmSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM hItem);
extern void AddTempPartSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM hItem);
extern BOOL ShowTempAsmTree(CTreeCtrl * m_Tree,CDatabase * p_db);
extern BOOL ShowTempPartTree(CTreeCtrl * m_Tree,CDatabase * p_db);
//////////////////////////////////////////////////////////////////////////
//�������
extern BOOL VarantFn(CString str,ProSolid solid,CString mdlNum,long insNum );
//���ģ�͵�ʵ����Ϣ
extern BOOL GetMdlInsInfo(CDatabase * p_db,int Type,long MdlNum,long InsNum,CompInsItem * item);
extern BOOL ConvertUsrPropNumToCompInsItemNum(int propNum,int * item);

//����ͼ��
extern int GetFeatIconNum(int type);
extern void SetImageListForFeatrure(CImageList * p_imageList);
//���ǰ����
extern ProError UsrActivateCurrentWindow();
//�ػ浱ǰ����    
extern ProError UsrRepaintWindow();
//���ģ����Ϣ
extern void EraseMdlInfo(ProMdl mdl,UINT marsk);
//��õ�ǰģ�͵�ȫ·��
extern CString GetCurrentMdlPath(void);
//����ƶ�ģ�͵�ȫ·��
extern CString GetMdlPath(ProMdl mdl);
//���ģ�͵��ļ���
extern CString GetMdlFileName(ProMdl mdl);
//���ģ�͵�ȫ·��
extern CString GetMdlFullFileName(ProMdl mdl);
//����ģ�͵�����
extern BOOL SetMdlInfo(ProSolid mdl,UsrPartPorp newPorp);
//�ж�һ��ģ���Ƿ�ɾ�����������true���Ǳ�ɾ��
extern bool IsMdlDeleted(ProMdl mdl);
/************************************************************************/
/* ���ģ���Ƿ��б�ɾ����������������Ϣ                               */
/************************************************************************/
extern void UsrClearMdlInfo();
//�����ļ������жϣ�����ļ��ڵ�ǰģ���Ƿ��Ѿ�����
extern bool IsMdlInSession(CString ,ProMdlType /*type=PRO_MDL_PART*/,ProMdl * );
/************************************************************************/
/* ����ƶ�ģ�ͣ��ƶ����Ƶĳߴ�Ͳ�����
type=TYPE_DIM TYPE_PARA                                               */
/************************************************************************/
extern BOOL GetDimPara(ProSolid solid=NULL,ProName name=NULL,ProDimension * dim=NULL,ProParameter * para=NULL,int * type=NULL);
extern CString UsrGetParaValue(ProParameter * para=NULL/*����*/,CString * Type=NULL);
/************************************************************************/
/* ��ò����ĸ�����                                                   */
/************************************************************************/
extern ProSolid UsrGetParameterOwner(ProParameter * para);
/************************************************************************/
/* ��������Ŀ¼(���) ,���                                                  */
/************************************************************************/
extern bool CreateTempDir(ProSolid solid,CString * csPathName);
extern BOOL CreateTempAsmDirectory(ProSolid asm_solid,CString * csPathName);
extern BOOL HaveMdlCurrent();

//�����ݿ��ͼƬ���в���
extern BOOL  DBSavePic(CLongBinary * m_lobFile,CString FileFullName,CRecordset * m_Set,CString & error_info);
extern BOOL  DBOpenPic(CLongBinary * m_lobFile,CString  FileFullName/*����,����ΪNULL*/,CString & error_info,IPicture ** pic/*����������ΪNULL*/);
extern BOOL  DBOpenPicSaveAsFile(CLongBinary * m_lobFile,CString  FileFullName);

//��óߴ�Ͳ���ֵ,һ��ֻ������һ��,��һ��Ϊ��
extern double UsrGetDimParaValue(ProDimension * dim=NULL/*�ߴ�*/
						  ,ProParameter * para=NULL/*����*/);

//�жϱ����ļ��Ƿ����
extern bool IsFilesExists(CString csPath,CString csFilter
				   ,CStringArray & files/*�ļ�����*/,CString & csError);
//�жϱ����ļ��Ƿ����
extern bool IsFilesExists(CStringArray & filesFullPath/*�ļ�����*/,CString & csError);
extern UsrCheckCurrentMdlState  UsrCheckCurrentMdl(ProMdlType mdltype,CString filename/*������չ����*/,long num,ProMdl * mdl=NULL);
//////////////////////////////////////////////////////////////////////////
//��ʾĿ¼�ṹ�ĺ���
//��ʾ���ģ�͵�Ŀ¼���ṹ
extern BOOL ShowNPartTree(CTreeCtrl * m_Tree,CDatabase * p_db,bool IfNeedMdl=false);
//��ʾ���ģ�͵�Ŀ¼�ṹ
extern BOOL ShowNAsmTree(CTreeCtrl * m_Tree,CDatabase * p_db,bool IfNeedMdl=false);
//��ʾ���ʵ��
extern BOOL ShowPartTypeTree(CTreeCtrl * m_Tree,CDatabase * p_db);
//����·����·��������Ҫ"\\"���Լ��ļ��еĳ�ʼ�����������ֽ����ļ���
extern BOOL CreateDirectoryGivenPathAndName(CString csPath,CString inName,CString & outName);
//ѡ��Ŀ¼
extern bool  SelectAnDirectory(CString & selDir,CString defDir);
//�������ģ����Ϣ��ӵ����
extern BOOL UsrAddPartMdlOtherInfo(CDatabase * p_db,ProMdl mdl,long PrtNum);
//ɾ�����ģ��
extern BOOL  UsrDelPartMdl(CDatabase * p_db,long num);
//ɾ�����ģ��
extern BOOL  UsrDelAsmMdl(CDatabase * p_db,long num);
//�����ģ�͵���Ϣ��ӽ���
////solidlist��comnumlist��ģ����BOM���еı�ŵĶ�Ӧ
//extern BOOL UsrAddAsmMdlOtherInfo(CDatabase * p_db,ProMdl asm_solid
//						,long asm_num/*,CArray<ProSolid,ProSolid> & solidlist
//						,CArray<long,long> & comnumlist*/);
//��ʾģ�ͳߴ�
extern ProError UsrShowDim(ProSolid solid,ProDimension  * dim);
//���Ԫ����Ԫ����
extern bool UsrGetSolidComppath(ProSolid assmbely,ProSolid comp,CArray<int,int> & p_IDTalbe);
//���ģ�͵��Ƿ��Ѿ���ʹ��
extern bool CheckIfCodeHavebeenUsed(long/* TypeNum=-1*/,CString/* code*/,CDatabase *);
//���ļ��������ڴ�,�ļ�������ȫ·����
extern bool UsrMdlLoad(CStringArray & files,ProMdl ** mdllist);

//��pdmѰ������������Ĵ��λ����Ϣ
extern BOOL UsrGetPartAndProductFromPdm(CString PdmNum,int type,CString & d3file,CString & d2file);
////0.ȫ�ֱ���  ����ͼ
//extern CArray<UsrDrawingItem,UsrDrawingItem> P_UsrDrawingList;
//����ȫ�ֱ���
//1.������������Ա�
extern CArray<UsrSWTXTable1,UsrSWTXTable1>  p_UsrSWTXTableList;  //����ȫ�ֵ��������Ա����
//�ڳ�������ʱ��Ҫ����һ����ʼֵ(��Ч)
//2.����Ĺ�ϵ
//extern  CArray<UsrRelTable,UsrRelTable>    p_UsrRelTableList;   //����ȫ�ֵĹ�ϵ�б�������
//3.�����ͼƬ
extern CArray<UsrPicture,UsrPicture>   p_UsrPicture;           //ͼƬ��ȫ�ֱ���    
//4.��������ܲ���
extern CArray<UsrPerfParaTable,UsrPerfParaTable> p_UsrPerfParaTableList;//��������ܲ������ȫ�ֱ���

//5.ģ�͵Ļ������� ע�����ģ���װ��ģ�鹫��  ��ɾ��ĳһ��֮ǰ��Ҫ�ж����Ԫ���е�propnum�������ã�Ӧ����Ӧ�޸�
extern CArray<UsrPartPorp,UsrPartPorp> p_UsrPartPorp;          //����ģ�ͻ�����Ϣ��ȫ�ֱ���

//6.װ�����������
extern CArray<AsmSmlItem,AsmSmlItem> p_AsmSmlItemList;
//7.װ��Ĺ�ϵ�б�
//extern CArray<AsmRelItem,AsmRelItem> p_AsmRelItemList;
//extern CArray<CRel,CRel&>            p_CRelItemList;
extern CRelColection g_RelCollection;
//8.װ���Ԫ���ı��η�Χ
extern CArray<AsmCompVarItem,AsmCompVarItem> p_AsmCompItemList;
//9.ftp
extern FtpConnectInfo myFtp;
//10.ͼ��
extern int  DlgIcon;//=IDI_PROE;  //���öԻ���ı���ͼ��
//11.���ݿ�����
extern CString         dbLinkInfo;
extern CString			dbPdmLinkInfo;
//12.ȫ�ֵ�ͼ�꺯��
extern CImageList      g_ImageList1;  //��ͼ��
extern CImageList      g_ImageList2;  //Сͼ��


typedef struct Info      
{
	ProSolid  solid;         //����򲿼����
	int       SolidID;           //����Ż��߲�����
	CString   Type;            //���ͣ��ǲ���������������=��prt",����=��asm����
}Info;




//----------------------------------wshf--------------------------------
CString wshfstrDbOpen;
CMyDatabase wshfm_db;

typedef struct UserInfo 
{
	CString name;
	CString authority;
}UserInfo;
//�û�Ȩ�޽ṹ��
typedef struct UserRight
{
	CString right;
}UserRight;

UserInfo curUserInfo;
CArray<UserRight,UserRight> curUserRights;
UserRight useright;