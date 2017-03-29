// MyDatabse.cpp : 实现文件
//
#pragma  once
#include "stdafx.h"
#include "MyDatabase.h"


// CMyDatabase

CMyDatabase::CMyDatabase()
{
	/*if(!AfxOleInit())
	{
		AfxMessageBox("ole初始化不成功");
	}*/
    isTran=FALSE;
	currentIndex=0;
	maxRows=0;
	try
	{
		//m_pConnection.CreateInstance(__uuidof(Connection));
		m_pConnection.CreateInstance("ADODB.Connection");
		m_pRecordset.CreateInstance("ADODB.Recordset");
		m_pCommand.CreateInstance("ADODB.Command");
	}
	catch(...)
	{
		AfxMessageBox("数据库初始化错误！");
	}
}

CMyDatabase::~CMyDatabase()
{
	try
	{
	this->Close();
	if(m_pConnection)
	{
		m_pConnection.Release();
	}
	if(m_pRecordset)
	{
		m_pRecordset.Release();
	}
	if(m_pCommand)
	{
		//m_pCommand.Release();
		;
	}
	}catch (...) 
	{
		//AfxMessageBox("数据库析构错误!");
	}
}
// CMyDatabase 成员函数

///
int CMyDatabase::Open(CString lpszConnect)
{
	///通过JET数据库引擎对ACCESS2000数据库的连接

	///	m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data
	///	Source=C:\\test.mdb","","",adModeUnknown);

	//      不通过DSN对SQL SERVER数据库进行连接：
	//    m_pConnection->Open(
	//	"driver={SQLServer};Server=127.0.0.1;DATABASE=vckbase;UID=sa;PWD=139","",""
	//	,adModeUnknown);
	//CString lpszConnect="DSN=product;UID=;PWD=";
//	m_pConnection->ConnectionTimeout = 5;///设置超时时间为5秒

	//m_pConnection->Open((const char *)lpszConnect,"","",-1);
	
	try
	{
		m_pConnection->Open((const char *)lpszConnect,"","",0);
	}
	catch (...) 
	{
		AfxMessageBox("打开数据库错误");
		return 0;
	}
	connectString=lpszConnect;
		return 1;
}

int CMyDatabase::ReOpen()
{
	if(connectString=="")
	{
        AfxMessageBox("数据库未连接，请连接");
		return 0;
	}
	 return  this->Open(connectString);
}

///
int CMyDatabase::Execute(CString strSql)
{
	_variant_t RecordsAffected;
	int nRows=0;
    try    
	{  
		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		vNULL.scode = DISP_E_PARAMNOTFOUND;///定义为无参数

		m_pCommand->ActiveConnection=m_pConnection;

		m_pCommand->CommandText=strSql.AllocSysString();

		m_pCommand->Execute(&vNULL,&vNULL,adCmdText);

		//m_pCommand->CommandText="SELECT * FROM NPartInsInfoTable  WHERE InsCode = '123456780'";
		//m_pRecordset=m_pCommand->Execute(&vNULL,&vNULL,adCmdText);
		
		//m_pCommand->CommandText="SELECT * FROM NPartInsInfoTable ";
		//m_pRecordset->Open((IDispatch *)m_pCommand,NULL,)
		//if (m_pRecordset->Open(strSql.AllocSysString(),(IDispatch*)m_pConnection,adOpenDynamic,adLockOptimistic,adCmdText)!=S_OK)
		//{
		//	AfxMessageBox("打开失败");
		//}
		//if (m_pRecordset->adoEOF)
		//{
		//	AfxMessageBox("KONG");
		//}
		//else
		//{
		//	//m_pRecordset=m_pConnection->Execute(strSql.AllocSysString(),&RecordsAffected,adCmdText);
		//	//m_pRecordset->MoveFirst();
		//	//m_pRecordset->MoveNext();
		//	CString str="PrtNum";
		//	_variant_t value;
		//	value =m_pRecordset->GetCollect(str.AllocSysString());
		//	if(value.vt==VT_NULL)
		//		return 0;
		//	int num=value.intVal;
		//	str.Format("%d",num);
		//	AfxMessageBox(str);
		//}
		
		//_variant_t RecordsAffected;
		//m_pCommand->ActiveConnection=m_pConnection;
		//m_pRecordset->Open(,m_pCommand,)
		//m_pCommand->CommandText=strSql.AllocSysString();
		///*m_pRecordset=*///m_pConnection->Execute(strSql.AllocSysString(),&RecordsAffected,adCmdText);
		////m_pRecordset->PutActiveConnection(&RecordsAffected);
		////if (m_pRecordset->)
		////{
		////}
		///*m_pRecordset=*/m_pCommand->Execute(NULL, NULL,adCmdText);
	}
	catch (...) 
	{
		AfxMessageBox("数据库错误");
		return 0;
	}
	return 1;
}

////
int CMyDatabase::Query(CString strSql)
{
		int i=0;
	try
	{
			_variant_t RecordsAffected;
			m_pRecordset=m_pConnection->Execute(strSql.AllocSysString(),&RecordsAffected,adCmdText);
			try
			{
				m_pRecordset->MoveFirst();
			}
			catch(...)
			{
				maxRows=0;
				return 0;
			}
			while(!m_pRecordset->adoEOF)
			{
				i++;
				m_pRecordset->MoveNext();
			}
			maxRows=i;
			m_pRecordset->MoveFirst();
	}
	catch (...) 
	{
		AfxMessageBox("数据库查询错误");
		return -1;
	}
		return i;
}

///
int CMyDatabase::Close()
{
	  isTran=FALSE;
	if(m_pConnection->State) 
		m_pConnection->Close(); ///如果已经打开了连接则关闭它
	return 0;
}

BOOL CMyDatabase::DatabaseState()
{
	if(m_pConnection->State) 
		return TRUE;
	else
		return FALSE;
}
int CMyDatabase::BeginTrans()
{
	this->Close();
	this->ReOpen();
	int i=0;
	CString strSql="BEGIN TRAN";
	i=this->Execute(strSql);
	if(i==1)
		  isTran=TRUE;
	return i;
}
int CMyDatabase::EndTrans()
{
	int i=0;
	CString strSql="COMMIT TRAN";
	i=this->Execute(strSql);
	if(i==1)
	  isTran=FALSE;
		return i;
}
int CMyDatabase::RollBack()
 {
	 int i=0;
	 CString strSql="ROLLBACK  TRAN";
	 i=this->Execute(strSql);
	 if(i==1)
		 isTran=FALSE;
		 return i;
 }

///
BOOL CMyDatabase::MoveBegin()
{
	try
	{
	m_pRecordset->MoveFirst();
	if(m_pRecordset->adoEOF)
		return FALSE;
	currentIndex=1;
	return TRUE;
	}
	catch (...) 
	{
		AfxMessageBox("数据库移动到第一条记录,出现错误");
	}
	return FALSE;
}

///
BOOL CMyDatabase::MoveEnd()
{
	Move(maxRows);
	return TRUE;
}

BOOL CMyDatabase::MoveLast()
{
	//m_pRecordset->MoveLast();
	return TRUE;
}
///
BOOL CMyDatabase::MoveNext()
{
	try
	{
	if(!m_pRecordset->adoEOF)
	{
		m_pRecordset->MoveNext();
		currentIndex++;
		if(m_pRecordset->adoEOF)
		{
			MovePre();
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
	}
	catch (...) 
	{
		AfxMessageBox("数据库移动到下一条记录,出现错误");
	}
	return FALSE;
}

///
BOOL CMyDatabase::MovePre()
{
	long index;
	if(currentIndex>1)
	{
		index=currentIndex-1;
		Move(index);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

////
BOOL CMyDatabase::Move( long  nRowIndex)
{
    try
	{

			if(nRowIndex>maxRows|| nRowIndex<1)
			{
				AfxMessageBox("超出记录集范围！");
					return FALSE;
			}
			m_pRecordset->MoveFirst();
			long index=nRowIndex;
			while(--index)
			{
				m_pRecordset->MoveNext();
			}
			currentIndex=nRowIndex;
			return TRUE;
	}
	catch (...) 
	{
		//AfxMessageBox(e->ErrorMessage());
	}
	return FALSE;
}

///
long CMyDatabase::getRows()
{
	return maxRows;
}

///
long CMyDatabase::getCurRow()
{
	return currentIndex;
}

///
long CMyDatabase::getColumns()
{
	try
	{
	long ncol=m_pRecordset->Fields->Count;
	return ncol;
	}
	catch (...) 
	{
		AfxMessageBox("数据库列错误");
	}
	return 0;
}

////
CString CMyDatabase::getString(long x)
{
	try
	{
		_variant_t value;
		
		value = m_pRecordset->GetCollect(_variant_t(x));
		if(value.vt==VT_NULL)
			return "";
		CString str=CString(value.bstrVal);
		return str.TrimRight();
    }
	catch (...) 
	{
		AfxMessageBox("数据库返回字符串值,出现错误");
	}
    return NULL;
}
////
CString CMyDatabase::getString(CString colName)
{
	try
	{
	_variant_t value;
	value =m_pRecordset->GetCollect(colName.AllocSysString());
	if(value.vt==VT_NULL)
		return "";
	CString str=CString(value.bstrVal);;
	return str.TrimRight();
	}
	catch (...) 
	{
		AfxMessageBox("数据库返回字符串值,出现错误");
	}
	return NULL;
}

///
int CMyDatabase::getInt(long x)
{
	try
	{
		_variant_t value;
	value = m_pRecordset->GetCollect(_variant_t(x));
	if(value.vt==VT_NULL)
		return -1;
	int num=value.intVal;
	return num;
    }
	catch (...) 
	{
		AfxMessageBox("数据库返回整数值,出现错误");
	}
    return -1;
}

///
int CMyDatabase::getInt(CString colName)
{
	try
	{
		_variant_t value;
		value = m_pRecordset->GetCollect(colName.AllocSysString());
		if(value.vt==VT_NULL)
			return -1;
		int num=value.intVal;
		return num;
    }
	catch (...) 
	{
		AfxMessageBox("数据库返回整数值,出现错误");
	}
    return -1;
}

///
double  CMyDatabase::getDouble(long x)
{
	try
	{
	_variant_t value;
	value = m_pRecordset->GetCollect(_variant_t(x));
	if(value.vt==VT_NULL)
		return -1;
	double num=value.dblVal;
	return num;
	}
	catch (...) 
	{
		AfxMessageBox("数据库返回实数值,出现错误");
	}
	return -1;
}

///
double  CMyDatabase::getDouble(CString colName)
{
	try
	{
	_variant_t value;
	value = m_pRecordset->GetCollect(colName.AllocSysString());
	if(value.vt==VT_NULL)
		return -1;
	double num=value.dblVal;
	return num;
	}
	catch (...) 
	{
		AfxMessageBox("数据库返回实数值,出现错误");
	}
	return -1;
}

///
CString CMyDatabase::getDate(long  x)
{
	try
	{
		_variant_t value;
		value = m_pRecordset->GetCollect(_variant_t(x));
		//Date date=value.date;
		COleDateTime   mydate(value.date);
		CString str;
		str.Format("%d年%d月%d日",mydate.GetYear(),mydate.GetMonth(),mydate.GetDay());
		return str;
	}
	catch (...) 
	{
		AfxMessageBox("数据库返回日期值,出现错误");
	}
	return NULL;

}

///
CString CMyDatabase::getDate(CString colName)
{
	try
	{
		_variant_t value;
		value = m_pRecordset->GetCollect(colName.AllocSysString());
		//Date date=value.date;
		COleDateTime   mydate(value.date);
		CString str;
		str.Format("%d年%d月%d日",mydate.GetYear(),mydate.GetMonth(),mydate.GetDay());
		return str;
	}
	catch (...) 
	{
		AfxMessageBox("数据库返回日期值,出现错误");
	}
	return NULL;
}
int CMyDatabase::getBinary(CString colName,BYTE ** pBuffer)
{
	long nSize ;
	try
	{
	    nSize = m_pRecordset->GetFields()->GetItem(colName.AllocSysString())->ActualSize;
		if(nSize > 0)
		{
			_variant_t	varBLOB;
			varBLOB = m_pRecordset->GetFields()->GetItem(colName.AllocSysString())->GetChunk(nSize);
			if(varBLOB.vt == (VT_ARRAY | VT_UI1))
			{
				if(*pBuffer = new BYTE [nSize+1])		///重新申请必要的存储空间
				{	
					char *pBuf = NULL;
					SafeArrayAccessData(varBLOB.parray,(void **)&pBuf);
					memcpy(*pBuffer,pBuf,nSize);				///复制数据到缓冲区m_pBMPBuffer
					SafeArrayUnaccessData (varBLOB.parray);
					//CFile file;
					//file.Open("c:\\1.jpg",CFile::modeCreate|CFile::modeWrite,NULL);
					//file.Write(*pBuffer,nSize);
					//file.Close();
					pBuf=0;
				}
			}
		}
		else
			return 0;
	}
	catch (_com_error e) 
	{
	  return 0;
	}
	return nSize;
}





CString CMyDatabase::RollSql(CString strSql)
{

	return "";
}
