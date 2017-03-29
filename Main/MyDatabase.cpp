// MyDatabse.cpp : ʵ���ļ�
//
#pragma  once
#include "stdafx.h"
#include "MyDatabase.h"


// CMyDatabase

CMyDatabase::CMyDatabase()
{
	/*if(!AfxOleInit())
	{
		AfxMessageBox("ole��ʼ�����ɹ�");
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
		AfxMessageBox("���ݿ��ʼ������");
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
		//AfxMessageBox("���ݿ���������!");
	}
}
// CMyDatabase ��Ա����


int CMyDatabase::Begin()
{
	/*if(!AfxOleInit())
	{
	AfxMessageBox("ole��ʼ�����ɹ�");
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
		AfxMessageBox("���ݿ��ʼ������");
		return 0;
	}
	return 1;
}
///
int CMyDatabase::Open(CString lpszConnect)
{
	///ͨ��JET���ݿ������ACCESS2000���ݿ������

	///	m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data
	///	Source=C:\\test.mdb","","",adModeUnknown);

	//      ��ͨ��DSN��SQL SERVER���ݿ�������ӣ�
	//    m_pConnection->Open(
	//	"driver={SQLServer};Server=127.0.0.1;DATABASE=vckbase;UID=sa;PWD=139","",""
	//	,adModeUnknown);
	//CString lpszConnect="DSN=product;UID=;PWD=";
//	m_pConnection->ConnectionTimeout = 5;///���ó�ʱʱ��Ϊ5��

	//m_pConnection->Open((const char *)lpszConnect,"","",-1);
	
	try
	{
		m_pConnection->Open((const char *)lpszConnect,"","",0);
	}
	catch (_com_error  e) {
		/*AfxMessageBox("���ݿ����Ӵ���");*/
		AfxMessageBox(e.ErrorMessage());
		return 0;
	}
	connectString=lpszConnect;
		return 1;
}

int CMyDatabase::ReOpen()
{
	if(connectString=="")
	{
             AfxMessageBox("δ���ӣ�������");
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
		vNULL.scode = DISP_E_PARAMNOTFOUND;///����Ϊ�޲���

		m_pCommand->ActiveConnection=m_pConnection;

		m_pCommand->CommandText=strSql.AllocSysString();

		m_pCommand->Execute(&vNULL,&vNULL,adCmdText);

		//m_pCommand->CommandText="SELECT * FROM NPartInsInfoTable  WHERE InsCode = '123456780'";
		//m_pRecordset=m_pCommand->Execute(&vNULL,&vNULL,adCmdText);
		
		//m_pCommand->CommandText="SELECT * FROM NPartInsInfoTable ";
		//m_pRecordset->Open((IDispatch *)m_pCommand,NULL,)
		//if (m_pRecordset->Open(strSql.AllocSysString(),(IDispatch*)m_pConnection,adOpenDynamic,adLockOptimistic,adCmdText)!=S_OK)
		//{
		//	AfxMessageBox("��ʧ��");
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
	catch (_com_error * e) 
	{
		AfxMessageBox(e->ErrorMessage());
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
		AfxMessageBox("���ݿ��ѯ����");
		maxRows=0;
		return -1;
	}
		return i;
}

///
int CMyDatabase::Close()
{
	  isTran=FALSE;
	if(m_pConnection->State) 
		m_pConnection->Close(); ///����Ѿ�����������ر���
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
		AfxMessageBox("���ݿ��ƶ�����һ����¼,���ִ���");
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
		AfxMessageBox("���ݿ��ƶ�����һ����¼,���ִ���");
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
				AfxMessageBox("������¼����Χ��");
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
	catch (_com_error *e) 
	{
		AfxMessageBox(e->ErrorMessage());
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
		AfxMessageBox("���ݿ��д���");
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
	AfxMessageBox("���ݿⷵ���ַ���ֵ,���ִ���");
}
return "";
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
		AfxMessageBox("���ݿⷵ���ַ���ֵ,���ִ���");
	}
	return "";
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
		AfxMessageBox("���ݿⷵ������ֵ,���ִ���");
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
		AfxMessageBox("���ݿⷵ������ֵ,���ִ���");
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
		AfxMessageBox("���ݿⷵ��ʵ��ֵ,���ִ���");
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
		AfxMessageBox("���ݿⷵ��ʵ��ֵ,���ִ���");
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
		str.Format("%d��%d��%d��",mydate.GetYear(),mydate.GetMonth(),mydate.GetDay());
		return str;
	}
	catch (...) 
	{
		AfxMessageBox("���ݿⷵ������ֵ,���ִ���");
	}
	return "";

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
		str.Format("%d��%d��%d��",mydate.GetYear(),mydate.GetMonth(),mydate.GetDay());
		return str;
	}
	catch (...) 
	{
		AfxMessageBox("���ݿⷵ������ֵ,���ִ���");
	}
	return "";
}
int CMyDatabase::getBinary(CString colName,BYTE ** pBuffer)
{
	//long nSize ;
	//try
	//{
	//    nSize = m_pRecordset->GetFields()->GetItem(colName.AllocSysString())->ActualSize;
	//	if(nSize > 0)
	//	{
	//		_variant_t	varBLOB;
	//		varBLOB = m_pRecordset->GetFields()->GetItem(colName.AllocSysString())->GetChunk(nSize);
	//		if(varBLOB.vt == (VT_ARRAY | VT_UI1))
	//		{
	//			if(*pBuffer = new BYTE [nSize+1])		///���������Ҫ�Ĵ洢�ռ�
	//			{	
	//				char *pBuf = NULL;
	//				SafeArrayAccessData(varBLOB.parray,(void **)&pBuf);
	//				memcpy(*pBuffer,pBuf,nSize);				///�������ݵ�������m_pBMPBuffer
	//				SafeArrayUnaccessData (varBLOB.parray);
	//				pBuf=0;
	//			}
	//		}
	//	}
	//	else
	//		return 0;
	//}
	//catch (_com_error e) 
	//{
	//  return 0;
	//}
	//return nSize;
	return 0;
}

 int CMyDatabase::SetBinary(CString strSql,CString colName,BYTE ** pBuffer,int nSize)
 {
	//this->Query(strSql);
	//int nrows;
	//nrows=this->getRows();
	//if(nrows==0) return 0;
	//this->MoveBegin();
	// try
	// {
	// BYTE *pBuf =* pBuffer;     ///������һ����ǰ�pBuffer���jpg���ݷŵ�����
	// VARIANT			varBLOB;
	// SAFEARRAY		*psa;
	// SAFEARRAYBOUND	rgsabound[1]; 
	// if(pBuf)
	// {    
	//	 rgsabound[0].lLbound = 0;
	//	 rgsabound[0].cElements = nSize;
	//	 psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
	//	 for (long i = 0; i < (long)nSize; i++)
	//		 SafeArrayPutElement (psa, &i, pBuf++);
	//	 varBLOB.vt = VT_ARRAY | VT_UI1;
	//	 varBLOB.parray = psa;
	//	 m_pRecordset->GetFields()->GetItem(colName.AllocSysString())->AppendChunk(varBLOB);
	// }
	// m_pRecordset->Update();
	// }catch(...)
	// {
	//	 AfxMessageBox("�洢�����ƶ������");
	//	 return 0;
	// }
	 return 1;
 }

 int CMyDatabase::SetBinary(CString strSql,CString colName,CString FileName)
 {
	//CFile file;
	//int status;
	//status=file.Open(FileName,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary);
	//if(status==0) return 0;
	//int nSize = file.GetLength();          //�ȵõ�jpg�ļ�����
	//BYTE * pBuffer = new BYTE [nSize];  //���ļ��Ĵ�С�ڶ�������һ���ڴ�

	//if (file.Read(pBuffer, nSize) > 0 )  
	//{
 //         status=this->SetBinary(strSql,colName,&pBuffer,nSize);
	//	  return status;
	//}
	return 0;
 }




 int CMyDatabase::InsertBinary(CString tableName,CString colName,BYTE ** pBuffer,int nSize)
 {
//    CString strSql;
//	strSql.Format("select * from %s" ,tableName);
//	m_pRecordset->Close();
//	m_pRecordset->Open("SELECT * FROM sim_Image ORDER BY ID",_variant_t((IDispatch*)m_pConnection,true),adOpenStatic,adLockOptimistic,adCmdText);
//	 m_pRecordset->AddNew();
//	 try
//	 {
//		 BYTE *pBuf =* pBuffer;     ///������һ����ǰ�pBuffer���jpg���ݷŵ�����
//		 VARIANT			varBLOB;
//		 SAFEARRAY		*psa;
//		 SAFEARRAYBOUND	rgsabound[1]; 
//		 if(pBuf)
//		 {    
//			 rgsabound[0].lLbound = 0;
//			 rgsabound[0].cElements = nSize;
//			 psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
//			 for (long i = 0; i < (long)nSize; i++)
//				 SafeArrayPutElement (psa, &i, pBuf++);
//			 varBLOB.vt = VT_ARRAY | VT_UI1;
//			 varBLOB.parray = psa;
//			 m_pRecordset->GetFields()->GetItem(colName.AllocSysString())->AppendChunk(varBLOB);
//		 }
//		 m_pRecordset->Update();
//	 }catch(...)
//	 {
//		 AfxMessageBox("�洢�����ƶ������");
//		 return 0;
//	 }
//int id=this->getInt("ID");
//	 return id;
	 return 0;
 }

 int CMyDatabase::InsertBinary(CString tableName,CString colName,CString FileName)
 {
	 //CFile file;
	 //int status;
	 //int id=0;
	 //status=file.Open(FileName,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary);
	 //if(status==0) return 0;
	 //int nSize = file.GetLength();          //�ȵõ�jpg�ļ�����
	 //BYTE * pBuffer = new BYTE [nSize];  //���ļ��Ĵ�С�ڶ�������һ���ڴ�

	 //if (file.Read(pBuffer, nSize) > 0 )  
	 //{
		// CString strSql;
		// try
		// {
		// m_pRecordset->Open("SELECT * FROM sim_Image ORDER BY ID",_variant_t((IDispatch*)m_pConnection,true),adOpenStatic,adLockOptimistic,adCmdText);
		// m_pRecordset->AddNew();
	
		//	 BYTE *pBuf =pBuffer;     ///������һ����ǰ�pBuffer���jpg���ݷŵ�����
		//	 VARIANT			varBLOB;
		//	 SAFEARRAY		*psa;
		//	 SAFEARRAYBOUND	rgsabound[1]; 
		//	 if(pBuf)
		//	 {    
		//		 rgsabound[0].lLbound = 0;
		//		 rgsabound[0].cElements = nSize;
		//		 psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
		//		 for (long i = 0; i < (long)nSize; i++)
		//			 SafeArrayPutElement (psa, &i, pBuf++);
		//		 varBLOB.vt = VT_ARRAY | VT_UI1;
		//		 varBLOB.parray = psa;
		//		 m_pRecordset->GetFields()->GetItem("Name")->AppendChunk(varBLOB);
		//	 }
		//	
		//	 m_pRecordset->Update();
		// }catch(...)
		// {
		//	 AfxMessageBox("�洢�����ƶ������");
		//	 return 0;
		// }
		// m_pRecordset->Close();
		//// m_pRecordset->Release();
		// //strSql.Format("select * from sim_Image order by ID");
		// //this->Query(strSql);
		// //int rows=this->getRows();//
		// //this->Move(rows);
		// //int id=this->getInt("ID");
		// return 1;
	 //}
	 return 0;
 }


CString CMyDatabase::RollSql(CString strSql)
{

	return "";
}

CString CMyDatabase::GetConnectString()
{
	return connectString;
}