#pragma once
//#import "D:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename ("EOF" ,"adoEOF")
#import "c:\Program Files\Common Files\System\ado\msado15.dll" rename_namespace("Comdata") rename ("EOF" ,"adoEOF")
using namespace Comdata;

// CMyDatabase 命令目标
//#include "Picture.h"

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
	
private:
	CString RollSql(CString strSql);
public:
	BOOL DatabaseState();
	_RecordsetPtr      m_pRecordset;
	_RecordsetPtr      m_pRecordset1;
	
	_ConnectionPtr  m_pConnection;
	_CommandPtr     m_pCommand;
	int ReOpen();
	int Open(CString lpszConnect);
	int Execute(CString strSql);
	int Query(CString strSql);
	int Close();
	int Begin();
///定位操作
	BOOL MoveBegin();
	BOOL MoveEnd();
	BOOL MoveNext();
	BOOL MovePre();
	BOOL MoveLast();
	BOOL Move(long nRowIndex);
///行操作
	long getRows();
	long getCurRow();
///列操作
	long  getColumns();
   CString getString(long x);
   CString getString(CString colName);
   int  getInt(long x);
   int  getInt(CString colName);
   double  getDouble(long x);
   double  getDouble(CString colName);
   CString getDate(long x);
   CString getDate(CString colName);
   int getBinary(CString colName,BYTE ** pBuffer);//二进制操作
   int SetBinary(CString strSql,CString colName,BYTE ** pBuffer,int nSize);
   int SetBinary(CString strSql,CString colName,CString FileName);
    int InsertBinary(CString tableName,CString colName,BYTE ** pBuffer,int nSize);
int InsertBinary(CString tableName,CString colName,CString FileName);  
CString GetConnectString();
	///事务操作
   int BeginTrans();
   int EndTrans();
   int RollBack();
};


