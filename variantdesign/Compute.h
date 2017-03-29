#pragma once
#include "ProSolid.h"
#include "ProDrawing.h"
// CCompute ����Ŀ��

class CCompute : public CObject
{
public:
	CCompute();
	virtual ~CCompute();
public:
	//��ȡ���ݺ���
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
	
	//BOOL IsJuBingAlreadyHave();						//������˵���ã�ֻ��Գ²�
	BOOL Interface(ProSolid solid,CString partnum,int InsNum);
	BOOL ReadRelation(CString partnum);
	BOOL ObtainToleranceFromMdl();
	BOOL AsmInterface(int s);



	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	BOOL GetVariantForPart(ProSolid solid,CString partnum,BOOL temp);	 //�����ͼ�����
	BOOL GetVariantForAsm(CString partnum,CString PostID);
	BOOL AddSolidToDatabase(ProSolid solid);
	BOOL AddSolidToDatabaseTemp(ProSolid solid);
	BOOL GetValueFromMdl();					   //��ģ����ȡֵ

	BOOL GetRelationForAsm(CString partnum,CString PostID);
	void ModifyPostIdForRelation(CStringArray *Relatemp,CString PostId);	//Ϊ��Ʒ�޸�����ı��
	BOOL GetRelationForPart(CString partnum);					//������������͵�ʱ����

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


