#pragma once
//#import "D:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename ("EOF" ,"adoEOF")
#import "c:\Program Files\Common Files\System\ado\msado15.dll" rename_namespace("Comdata") rename ("EOF" ,"adoEOF")
using namespace Comdata;

// CMyDatabase ����Ŀ��
#include "Picture.h"

class CMyDatabase : public CObject
{
public:
	CMyDatabase();
	virtual ~CMyDatabase();
private:
	long currentIndex;
    long maxRows;
	BOOL isTran;
	CString connectString;
	CMyPicture m_Pic;
private:
	CString RollSql(CString strSql);
public:
	BOOL DatabaseState();
	_RecordsetPtr      m_pRecordset;
	
	_ConnectionPtr  m_pConnection;
	_CommandPtr     m_pCommand;
	int ReOpen();
	int Open(CString lpszConnect);
	int Execute(CString strSql);
	int Query(CString strSql);
	int Close();
///��λ����
	BOOL MoveBegin();
	BOOL MoveEnd();
	BOOL MoveNext();
	BOOL MovePre();
	BOOL MoveLast();
	BOOL Move(long nRowIndex);
///�в���
	long getRows();
	long getCurRow();
///�в���
	long  getColumns();
   CString getString(long x);
   CString getString(CString colName);
   int  getInt(long x);
   int  getInt(CString colName);
   double  getDouble(long x);
   double  getDouble(CString colName);
   CString getDate(long x);
   CString getDate(CString colName);
   int getBinary(CString colName,BYTE ** pBuffer);//�����Ʋ���
   ///�������
   int BeginTrans();
   int EndTrans();
   int RollBack();
};


