#pragma once
//#include "ProSolid.h"
#include "afxtempl.h"
// CCompute 命令目标
typedef struct Database
{
	ProSolid	solid;					   //该变量所属模型
	CString		type;					   //变量类型
	CString		name;					   //变量的名字，自定义，本身没有什么用的
	CString		idnum;					   //尺寸ID
	CString		value;					   //尺寸值
	CString		PARADIMSYMBOL;			   //组件中有后缀的变量
	CString		paradimsymbol;			   //没有后缀的变量
	CString		subtype;				   //子类型，只对参数有用
	CString		note;					   //尺寸的说明
	BOOL		IsOrNot;				   //是否是主变量
	BOOL		IsOk;					   //是否是已经替换
	int			number;					   //编号
	int			Allnum;
	CString		up;
	CString		down;
}Database;
class CCompute : public CObject
{
public:
	CCompute();
	virtual ~CCompute();
public:
	//读取数据函数
	BOOL ObtianFromBase(ProSolid solid,CString partnum);
	BOOL ReadRelation(ProSolid solid);
	BOOL UpdateAllRelation(/*ProSolid solid,*/CString str,int j);
	int UpdateOneRelation(/*ProSolid solid,*/CString str,int i,int h,int j);
	CString IsOnlyOne(/*ProSolid solid,*/int i);
	BOOL UpdateOneDim(/*ProSolid solid,*/int i,int k,int j,CString str);
	int UpdateDim(/*ProSolid solid,*/CString str,CString value);
	double CalString(CString expression);
	double ComputeEquation(CString expression,CString x);
	BOOL ObtainJuBing(ProSolid solid/*,CString Asmnum*/);
	void ObtainJuBingFromAsm(ProSolid solid,int **a,int j,int upnum);
	void ObtainJuBingFromPart(ProSolid solid,ProAsmcomppath *comppath,int upnum);
	//void ObtainPartNumFromName(int AsmNum);
	void ObtianValueFromMdl(ProSolid solid,int i);
	int ObtianAsmFromBaseTemp(ProSolid solid,CString partnum);
	BOOL ReadRelationFromAsm(ProSolid solid,CString AsmNum);
	BOOL ObtianAsmFromBaseTemp(CString AsmNum);
	//ProSolid ObtainJuBingFromPart(CString variant,int t);
	BOOL AsmGeneration(ProSolid solid);
	BOOL ObtianNameFromPostId(CString partnum,Database * uu);
	//BOOL IsTheSameProduct(ProSolid Productsolid,ProSolid solid);
	CString AlterRelationFromPost(CString Relation);
	//CString ObtainMapCode(ProSolid solid);
	BOOL ObtainAllVariantAndRelativeVariant(ProSolid solid,CString ProductNum);
	BOOL IsAlreadyHave();
	BOOL ModifyPostId(ProSolid solid,CString ProductNum,CString partnum,int n_dim);
	BOOL ModifyPostIdTemp(ProSolid solid,CString Asmnum,CString partnum,int n_dim);
	void ObtianPostIdFromName(ProSolid solid,CString name,Database * uu);
	void ObtainPostIdFromRelation();
	int ObtianFromBaseTemp(ProSolid solid,CString partnum);
	BOOL ObtianAsmFromBase(ProSolid solid,CString AsmNum);
	void ObtianValueFromMdlTemp(ProSolid solid);
	BOOL IsJuBingAlreadyHave();						//对我来说不用，只针对陈波
	BOOL Interface(ProSolid solid,CString partnum,int InsNum);
	BOOL ReadRelation(/*ProSolid solid,*/CString partnum);
	void ObtianToleranceFromMdl();
	BOOL AsmInterface(ProSolid solid,CString partnum,int InsNum);
	BOOL ObtianPartFromBaseTemp(CString partnum);				 //在没有打开模型的时候用来读取数据库
	CString ObtainModelNum(ProSolid solid);
public:
	BOOL InRelation;
	BOOL IsGen;
	int post_id;
};


