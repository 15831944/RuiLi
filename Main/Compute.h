#pragma once
//#include "ProSolid.h"
#include "afxtempl.h"
// CCompute ����Ŀ��
typedef struct Database
{
	ProSolid	solid;					   //�ñ�������ģ��
	CString		type;					   //��������
	CString		name;					   //���������֣��Զ��壬����û��ʲô�õ�
	CString		idnum;					   //�ߴ�ID
	CString		value;					   //�ߴ�ֵ
	CString		PARADIMSYMBOL;			   //������к�׺�ı���
	CString		paradimsymbol;			   //û�к�׺�ı���
	CString		subtype;				   //�����ͣ�ֻ�Բ�������
	CString		note;					   //�ߴ��˵��
	BOOL		IsOrNot;				   //�Ƿ���������
	BOOL		IsOk;					   //�Ƿ����Ѿ��滻
	int			number;					   //���
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
	//��ȡ���ݺ���
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
	BOOL IsJuBingAlreadyHave();						//������˵���ã�ֻ��Գ²�
	BOOL Interface(ProSolid solid,CString partnum,int InsNum);
	BOOL ReadRelation(/*ProSolid solid,*/CString partnum);
	void ObtianToleranceFromMdl();
	BOOL AsmInterface(ProSolid solid,CString partnum,int InsNum);
	BOOL ObtianPartFromBaseTemp(CString partnum);				 //��û�д�ģ�͵�ʱ��������ȡ���ݿ�
	CString ObtainModelNum(ProSolid solid);
public:
	BOOL InRelation;
	BOOL IsGen;
	int post_id;
};


