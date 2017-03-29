#pragma once
#include "ProSolid.h"
#include "ProDrawing.h"
// CCompute 命令目标

class CCompute : public CObject
{
public:
	CCompute();
	virtual ~CCompute();
public:
	//读取数据函数
	BOOL ObtainFromBase(ProSolid solid,CString partnum);
	BOOL ReadRelation(ProSolid solid);
	BOOL UpdateAllRelation(/*ProSolid solid,*/CString str,int j);
	int UpdateOneRelation(/*ProSolid solid,*/CString str,int i,int h,int j);
	CString IsOnlyOne(/*ProSolid solid,*/int i);
	BOOL UpdateOneDim(/*ProSolid solid,*/int i,int k,int j,CString str);
	int UpdateDim(/*ProSolid solid,*/CString str,CString value);
	double CalString(CString expression);
	double ComputeEquation(CString expression,CString x);


	//BOOL ObtainValueFromMdl(ProSolid solid,int i);

	BOOL ReadRelationFromAsm(ProSolid solid,CString AsmNum);

	
	//BOOL ObtianNameFromPostId(CString partnum,Database *uu);

	CString AlterRelationFromPost(CString Relation);

	BOOL ObtainAllVariantAndRelativeVariant(ProSolid solid,CString ProductNum);
	
	BOOL ModifyPostId(ProSolid solid,CString ProductNum,CString partnum,int n_dim);
	BOOL ModifyPostIdTemp(ProSolid solid,CString Asmnum,CString partnum,int n_dim);

	void ObtainPostIdFromRelation();
	//int ObtainFromBaseTemp(ProSolid solid,CString partnum);
	BOOL ObtainAsmFromBase(ProSolid solid,CString AsmNum);
	
	//BOOL IsJuBingAlreadyHave();						//对我来说不用，只针对陈波
	BOOL Interface(ProSolid solid,CString partnum,int InsNum);
	BOOL ReadRelation(CString partnum);
	BOOL ObtainToleranceFromMdl();
	BOOL AsmInterface(int s);



	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	BOOL GetVariantForPart(ProSolid solid,CString partnum,BOOL temp);	 //给变型计算用
	BOOL GetVariantForAsm(CString partnum,CString PostID);
	BOOL AddSolidToDatabase(ProSolid solid);
	BOOL AddSolidToDatabaseTemp(ProSolid solid);
	BOOL GetValueFromMdl();					   //从模型中取值

	BOOL GetRelationForAsm(CString partnum,CString PostID);
	void ModifyPostIdForRelation(CStringArray *Relatemp,CString PostId);	//为产品修改零件的编号
	BOOL GetRelationForPart(CString partnum);					//给单个零件变型的时候用

    //BOOL ObtainAsmFromBaseTemp(CString AsmNum);
	BOOL ObtainAsmFromBaseTemp(ProSolid solid,CString partnum);

	BOOL GetAllVariantAndRelativeVariant(ProSolid Productsolid,CString ProductNum);
	BOOL IsVariantAlreadyHave();
	void ObtainPostIdFromVariant();
	BOOL ObtainValueFromMdlTemp(ProSolid solid);

	BOOL ObtainJuBing(ProSolid solid);
	void ObtainJuBingFromAsm(ProSolid solid,int **a,int j,int upnum);
	void ObtainJuBingFromPart(ProSolid solid,ProAsmcomppath *comppath,int upnum);

	BOOL AsmGeneration(ProSolid solid);

public:
	BOOL InRelation;
	BOOL IsGen;
	int post_id;
};


