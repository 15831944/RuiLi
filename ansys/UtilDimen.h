#pragma once
#include "myproe.h"
#include <afxtempl.h>
typedef struct DimStruct
{
	//ProSolid * root;
   CString solidLabel;
	CString label;
	CString Llabel;
	ProDimension  dimension;
	int postid;
	double value;
	ProAsmcomppath comppath;
}DimStruct , * pDimStruct;

typedef struct ParamStruct
{
	//ProSolid * root;
	CString solidLabel;
	CString label;
	CString Llabel;
	CString type;
	int postid;
	CString value;
	BOOL isChecked;
	ProAsmcomppath comppath;
}ParamStruct , * pParamStruct;
// CUtilDimen ÃüÁîÄ¿±ê

class CUtilDimen : public CObject
{
public:
	CUtilDimen();
	virtual ~CUtilDimen();
public:
	//ProDimension * GetPointer(CString label);
	//double		GetValue(CString label);
	//void ShowDimen(CString label);
	int Load(ProSolid solid);
	int AddAssemblyDim(ProSolid solid,int **a,int j);
    int AddPartDim(ProSolid solid,ProAsmcomppath *comppath);
public:
	int GetPosition(CString label,int postid);
	int GetPosition(CString Llabel);
    int GetPositionEx(CString Llabel);
	double GetValue(CString label,int postid);
	CString GetValue(CString Llabel);
	double GetValue(int index);

	CString GetType(CString label,int postid);
	CString GetType(CString Llabel);
	CString GetType(int index,CString stype);

	int Show(CString label,int postid);
	int Show(CString Llabel);
	int Show(int index);

	int SetValue(CString label,int postid,double value);
	int SetValue(CString Llabel,double value);
	int SetValue(int index,double value);

	int GetTolerance(CString Llabel,double * upper_limit,double * lower_limit);
	int GetTolerance(int index,double * upper_limit,double * lower_limit);

    int SetTolerance(CString Llabel,double upper_limit,double  lower_limit);
	int SetTolerance(int index,double  upper_limit,double lower_limit);

public:
	CArray <DimStruct,DimStruct>  dimArray;
	CArray <ParamStruct,ParamStruct> paramArray;
	ProSolid root;
	CString errorString;
	BOOL isAssembly;

};


