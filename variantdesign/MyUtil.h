#pragma once
#include <afxtempl.h>

// CMyUtil ÃüÁîÄ¿±ê

class CMyUtil : public CObject
{
public:
	CMyUtil();
	virtual ~CMyUtil();
public:
	
public:

	 CString ReplaceString(CString str,CString str1,CString str2);
     BOOL IsDigit(int c);//////
	 BOOL IsDouble(CString str);  /////
	 BOOL IsFunction(CString str);		//////////
	 BOOL CheckExpression(CString Expression,CStringArray * importDim,CStringArray * exportDim);
     BOOL CheckEquation(CString Expression,CStringArray * importDim,CStringArray * exportDim);

	 double CalString(CString expression);
	 double ComputeEquation(CString expression,CString x);
     int FindX(CString expression,CString x);	///////
    CString ReplaceXtoDouble(CString expression,CString x,double result);
    CString ReplaceXtoString(CString expression,CString x,CString result);	/////////


	CString AddPostfix(CString expression,CString strNum);	///////
	int AddPostfix(CStringArray *strArray,CString strNum);	////////

	 
     
	 BOOL CheckInEquation(CString Expression,CStringArray * importDim,CStringArray * exportDim);
	 BOOL ComputeInEquation(CString expression);
};


