#pragma once
#include "UsrSWTXTable.h"
//��ϵʽ����������������һ��ȫ�ֶ���
class CRelColection
{
public:
	CRelColection(void);
	~CRelColection(void);
	BOOL AddRel(ProSolid owner, CString rel, CString note);
	BOOL DelRel(int indexNo);
	BOOL RemoveAllRel(ProSolid solid);
	BOOL EditRel(int index,CString str,CString note);
	BOOL Update(ProSolid solid);
	BOOL GetSolidAllRel(ProSolid solid, bool bolAsmUplevel,CArray<CRel,CRel&> *,CArray<int,int&> *);
	//����rel���Ƿ��С������������Ƿ�Ҫ���к�׺�Ĺ�ϵʽ�Ƚ�
	BOOL IsRelInCollection(ProSolid solid, CString rel);
	BOOL GetRvDimAndPara(CArray<UsrSWTXTable1,UsrSWTXTable1> * swtx, ProSolid solid,
		CArray<UsrSWTXTable1,UsrSWTXTable1> *);
	CString error_info;
	//���ģ���еĹ�ϵʽ�����гߴ�Ͳ���
	void GetSolidAllRelDimPara(ProSolid,CArray<UsrSWTXTable1,UsrSWTXTable1> &);
	//�ж�һ���ߴ�Ͳ����Ƿ��Ѿ��ڹ�ϵʽ(��ʽ����)�����
	bool IsDimParaInRelSetLeftPart(ProSolid,ProParameter *,ProDimension *);
private:
	CArray<CRel,CRel> m_CRelList;//�ڲ���ϵ�б�
	//TRUE�����Ѿ�����
	BOOL IsRelInColection(ProMdlType ,CString str);
	//���ģ�������еĳߴ�Ͳ���
	void GetSolidAllDimPara(ProSolid,CArray<UsrSWTXTable1,UsrSWTXTable1> &);
	//�жϹ�ϵʽ����Ƿ����������Ա���
	bool HasSwtxItemInRelLeft(ProSolid solid,CRel * rel,CString & csError);
};
