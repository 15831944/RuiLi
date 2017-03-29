// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once
#include "ProAsmcomp.h"
#include <ProAsmcomppath.h>

//#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")

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
#include "afxinet.h"
#include <afx.h>
#include <afxdlgs.h>
#include <afxdhtml.h>

//树结构句柄
typedef struct ItemHandle      //是否知道值的结构
{
	HTREEITEM hItem;         //句柄
	CString   name;          //名字
}ItemHandle;
//服务器地址结构
typedef struct FtpInfo
{
	int iPort;              //端口号
	CString strFtp;         //FTP
	CString FtpName;        //FTP名
	CString FtpPassword;    //FTP密码
	CString Database;       //PDM的连接信息
	CString PDMPath;        //PDM的路径
	CString LocalPath;      //从服务器上下载到本地的位置
}FtpInfo;
//尺寸信息结构
typedef struct Database
{
	ProSolid solid;					   //该变量所属模型
	ProSolid Asmsolid;                 //表明这个结构是在哪里定义过来的
	CString type;					   //变量类型
	CString name;					   //变量的名字，自定义，本身没有什么用的
	CString idnum;					   //尺寸ID
	CString value;					   //尺寸值
	CString oldvalue;                  //修改前的尺寸值
	CString PARADIMSYMBOL;			   //组件中有后缀的变量
	CString paradimsymbol;			   //没有后缀的变量
	CString subtype;				   //子类型，只对参数有用
	CString up;						   //上偏差
	CString down;					   //下偏差
	CString ModelNum;                  //是属于哪个模型过来的
	CString MdlType;                   //在组件中定义的，还是在零件中定义的
	CString note;					   //尺寸的说明
	BOOL    IsOrNot;				   //是否是主变量
	BOOL    IsOk;					   //是否是已经替换
	int	    number;					   //编号
	int     Allnum;
}Database;
//句柄结构
typedef struct JuBing
{
	ProSolid Productsolid;			   //产品句柄
	ProSolid solid;					   //零部件句柄
	ProAsmcomppath comppath;		   //零件的路径
	CString  Name;					   //零件的名字，包括后缀
	CString  Type;                     //判断是零件还是组件
	int number;						   //后缀编号
	int upnum;                         //上一级的编号
}JuBing;
//////////////////////////////////////////////////////////////////////////
typedef struct ItemHandle1      
{
	HTREEITEM hItem;         //句柄
	int   FatherNumber;      //父项
}ItemHandle1;


typedef struct FilePath      //是否知道值的结构
{
	int      SolidID;                              //零件号
	CString  SolidFilePath;                        //三维的服务器文件路径
	CString  SolidName;
	CString  DrwFilePath;                          //二维的服务器文件路径
	CString  DrwName;
	CString  MdlNum;                               //模型号
	CString  InsCode;                              //去查找PDM中的文件位置
	int      MdlCompNum;
	CString  LocalState;                           //保存位置
	CString  IsInsert;                             //是否是在这个产品设计中刚插入的
	BOOL     AddToPDM;                              //判断是否添加到PDM中
	CString  Type;                                 //代表是零件还是组件
	BOOL     IsLocal;
}FilePath;
//和大波的接口用结构
typedef struct Info      
{
	ProSolid  solid;             //零件或部件句柄
	int       SolidID;           //零件号或者部件号
	CString   Type;              //类型，是部件还是零件，零件=“prt",部件=“asm”；
	///////

}Info;

//系列信息
typedef struct StandardInfo    
{
	CString   MdlNum;           //零件模型名称
	int       DimNum;           //零件的尺寸号
	int       StandardNum;      //一个零件的规格号
	CString   DimName;          //尺寸名称
	CString   Value;            //尺寸值
	CString   StandardName;     //系列名称
	CString   Note;             //备注
}StandardInfo;

CString GL_USERNAME="";