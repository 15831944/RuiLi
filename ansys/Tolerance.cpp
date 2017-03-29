// Tolerance.cpp : 实现文件
//

#include "stdafx.h"
#include "Tolerance.h"
#include "Psapi.h"
#include <tlhelp32.h>



// CTolerance

CTolerance::CTolerance()
{
	m_PageCount=0;
}

CTolerance::~CTolerance()
{
	this->RemovePage();
}


// CTolerance 成员函数
int CTolerance::Load()
{
	_Application ExcelApp; 
	Workbooks wbsMyBooks; 
	_Workbook wbMyBook; 
	Worksheets wssMysheets; 
	_Worksheet wsMysheet; 
	Range rgMyRge; 
	if (!ExcelApp.CreateDispatch("Excel.Application",NULL)) 
	{ 
		AfxMessageBox("创建Excel服务失败!"); 
		exit(1); 
	} 
	ExcelApp.put_Visible(FALSE);
	//利用模板文件建立新文档 
	char path[MAX_PATH];
	GetSystemDirectory(path,MAX_PATH);
	CString strPath = path;
	strPath += "\\公差";
	wbsMyBooks.AttachDispatch(ExcelApp.get_Workbooks(),true); 
	wbMyBook.AttachDispatch(wbsMyBooks.Add(_variant_t(strPath)));
	//得到Worksheets 
	wssMysheets.AttachDispatch(wbMyBook.get_Worksheets(),true);

	CString str="";
    int pageIndex=1;
	int pageCount=20;
	pageCount=wssMysheets.get_Count();
	this->RemovePage();
	
    while(pageIndex<=pageCount)
	{
		ExcelPage * page=new ExcelPage;
		wsMysheet.AttachDispatch(wssMysheets.get_Item(_variant_t(pageIndex)),true);
		rgMyRge.AttachDispatch(wsMysheet.get_Cells(),true);
        str=rgMyRge.get_Item(_variant_t((long)1),_variant_t((long)1));
		if(str=="") break;
        page->strArray.RemoveAll();
		page->Name=str;
		for(int i=1;i<60;i++)
		{
			str=rgMyRge.get_Item(_variant_t((long)2),_variant_t((long)i));
			if(str=="") 
			{page->cols=i-1;break;}
		}

		for(int i=2;i<60;i++)
		{
			str=rgMyRge.get_Item(_variant_t((long)i),_variant_t((long)1));
			if(str=="") 
			{page->rows=i-2;break;}
		}

		for(int i=2;i<=page->rows+1;i++)
		{
			for(int j=1;j<=page->cols;j++)
			{
				str=rgMyRge.get_Item(_variant_t((long)i),_variant_t((long)j));
				page->strArray.Add(str);
			}
		}

		m_PageArray.Add(page);
		pageIndex++;
	}
	
	m_PageCount=pageIndex-1;


    wbsMyBooks.Close();
	ExcelApp.Quit();
	


	rgMyRge.ReleaseDispatch(); 
	wsMysheet.ReleaseDispatch(); 
	wssMysheets.ReleaseDispatch(); 
	wbMyBook.ReleaseDispatch(); 
	wbsMyBooks.ReleaseDispatch(); 
	ExcelApp.ReleaseDispatch();


	this->KillExcel();
	
	return 1;
}


int CTolerance::RemovePage()
{
	ExcelPage * page;
	for(int i=0;i<m_PageArray.GetSize();i++)
	{
       page=m_PageArray[i];
	   page->strArray.RemoveAll();
	   delete page;
	}
	m_PageArray.RemoveAll();
	return 1;
}


int CTolerance::GetCols(int PageIndex)
{
	if(PageIndex<=0||PageIndex>m_PageCount) return 0;

	return m_PageArray[PageIndex]->cols;
}
int CTolerance::GetRows(int PageIndex)
{
	if(PageIndex<=0||PageIndex>m_PageCount) return 0;

	return m_PageArray[PageIndex]->rows;
}

CString CTolerance::GetRowName(int PageIndex,int row)
{
	if(PageIndex<=0||PageIndex>m_PageCount) return "";
    int cols=m_PageArray[PageIndex]->cols;
	int rows=m_PageArray[PageIndex]->rows;
	if(row<0||row>=rows) return "";

	CString str="";
	str=m_PageArray[PageIndex]->strArray[row*cols+0];
	return str;
}
CString CTolerance::GetCellString(int PageIndex,int row,int col)
{
	if(PageIndex<=0||PageIndex>m_PageCount) return "";
	int cols=m_PageArray[PageIndex]->cols;
	int rows=m_PageArray[PageIndex]->rows;
	if(row<0||row>=rows) return "";
	if(col<0||col>=cols) return "";

	CString str="";
	str=m_PageArray[PageIndex]->strArray[row*cols+col];
	return str;
}




int CTolerance::KillExcel()
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("CreateToolhelp32Snapshot调用失败");
		return 0;
	}
	BOOL bMore = ::Process32First(hProcessSnap,&pe32);
	CStringArray strArray;
	CArray<DWORD,DWORD> PIDArray;
	while(bMore)
	{
        strArray.Add(pe32.szExeFile);
		PIDArray.Add(pe32.th32ProcessID);
		bMore = ::Process32Next(hProcessSnap,&pe32);
	}
	::CloseHandle(hProcessSnap);

	DWORD processID,selectID;
	int status=0;
	FILETIME ft1,ft2,ft3,ft4;
	FILETIME oldft;
	CString str;
	HANDLE hProcess;
    for(int i=0;i<PIDArray.GetSize();i++)
	{
	   str=strArray[i];
		 if(str.MakeUpper()!="EXCEL.EXE") continue;

	   processID=PIDArray[i];
       hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,processID);

	   if(GetProcessTimes(hProcess,&ft1,&ft2,&ft3,&ft4)==FALSE) return 0;
	   if(status==0)
	   {
		   status=1;
		   selectID=processID;
		   oldft.dwHighDateTime=ft1.dwHighDateTime;
		   oldft.dwLowDateTime=ft1.dwLowDateTime;
	   }else
	   {
		   int result;
		   result=CompareFileTime(&ft1,&oldft);
		   if(result==1)
		   {
			   selectID=processID;
			   oldft.dwHighDateTime=ft1.dwHighDateTime;
			   oldft.dwLowDateTime=ft1.dwLowDateTime;
		   }
	   }   
       CloseHandle(hProcess);
	}


	if(status==0) return 0;
    hProcess=NULL;
	hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,selectID);
	if(hProcess != NULL)
	{
		//终止进程
		TerminateProcess(hProcess,0);
		::CloseHandle(hProcess);
	}
  return 1;
}




