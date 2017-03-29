// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once
#include "ProAsmcomp.h"
#include <ProAsmcomppath.h>

//#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")

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
#include "afxinet.h"
#include <afx.h>
#include <afxdlgs.h>
#include <afxdhtml.h>

//���ṹ���
typedef struct ItemHandle      //�Ƿ�֪��ֵ�Ľṹ
{
	HTREEITEM hItem;         //���
	CString   name;          //����
}ItemHandle;
//��������ַ�ṹ
typedef struct FtpInfo
{
	int iPort;              //�˿ں�
	CString strFtp;         //FTP
	CString FtpName;        //FTP��
	CString FtpPassword;    //FTP����
	CString Database;       //PDM��������Ϣ
	CString PDMPath;        //PDM��·��
	CString LocalPath;      //�ӷ����������ص����ص�λ��
}FtpInfo;
//�ߴ���Ϣ�ṹ
typedef struct Database
{
	ProSolid solid;					   //�ñ�������ģ��
	ProSolid Asmsolid;                 //��������ṹ�������ﶨ�������
	CString type;					   //��������
	CString name;					   //���������֣��Զ��壬����û��ʲô�õ�
	CString idnum;					   //�ߴ�ID
	CString value;					   //�ߴ�ֵ
	CString oldvalue;                  //�޸�ǰ�ĳߴ�ֵ
	CString PARADIMSYMBOL;			   //������к�׺�ı���
	CString paradimsymbol;			   //û�к�׺�ı���
	CString subtype;				   //�����ͣ�ֻ�Բ�������
	CString up;						   //��ƫ��
	CString down;					   //��ƫ��
	CString ModelNum;                  //�������ĸ�ģ�͹�����
	CString MdlType;                   //������ж���ģ�����������ж����
	CString note;					   //�ߴ��˵��
	BOOL    IsOrNot;				   //�Ƿ���������
	BOOL    IsOk;					   //�Ƿ����Ѿ��滻
	int	    number;					   //���
	int     Allnum;
}Database;
//����ṹ
typedef struct JuBing
{
	ProSolid Productsolid;			   //��Ʒ���
	ProSolid solid;					   //�㲿�����
	ProAsmcomppath comppath;		   //�����·��
	CString  Name;					   //��������֣�������׺
	CString  Type;                     //�ж�������������
	int number;						   //��׺���
	int upnum;                         //��һ���ı��
}JuBing;
//////////////////////////////////////////////////////////////////////////
typedef struct ItemHandle1      
{
	HTREEITEM hItem;         //���
	int   FatherNumber;      //����
}ItemHandle1;


typedef struct FilePath      //�Ƿ�֪��ֵ�Ľṹ
{
	int      SolidID;                              //�����
	CString  SolidFilePath;                        //��ά�ķ������ļ�·��
	CString  SolidName;
	CString  DrwFilePath;                          //��ά�ķ������ļ�·��
	CString  DrwName;
	CString  MdlNum;                               //ģ�ͺ�
	CString  InsCode;                              //ȥ����PDM�е��ļ�λ��
	int      MdlCompNum;
	CString  LocalState;                           //����λ��
	CString  IsInsert;                             //�Ƿ����������Ʒ����иղ����
	BOOL     AddToPDM;                              //�ж��Ƿ���ӵ�PDM��
	CString  Type;                                 //����������������
	BOOL     IsLocal;
}FilePath;
//�ʹ󲨵Ľӿ��ýṹ
typedef struct Info      
{
	ProSolid  solid;             //����򲿼����
	int       SolidID;           //����Ż��߲�����
	CString   Type;              //���ͣ��ǲ���������������=��prt",����=��asm����
	///////

}Info;

//ϵ����Ϣ
typedef struct StandardInfo    
{
	CString   MdlNum;           //���ģ������
	int       DimNum;           //����ĳߴ��
	int       StandardNum;      //һ������Ĺ���
	CString   DimName;          //�ߴ�����
	CString   Value;            //�ߴ�ֵ
	CString   StandardName;     //ϵ������
	CString   Note;             //��ע
}StandardInfo;

CString GL_USERNAME="";