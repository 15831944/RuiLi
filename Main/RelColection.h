#pragma once
#include "UsrSWTXTable.h"
//关系式集，程序启动就有一个全局对象
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
	//根据rel中是否有“：”来决定是否要与有后缀的关系式比较
	BOOL IsRelInCollection(ProSolid solid, CString rel);
	BOOL GetRvDimAndPara(CArray<UsrSWTXTable1,UsrSWTXTable1> * swtx, ProSolid solid,
		CArray<UsrSWTXTable1,UsrSWTXTable1> *);
	CString error_info;
	//获得模型中的关系式的所有尺寸和参数
	void GetSolidAllRelDimPara(ProSolid,CArray<UsrSWTXTable1,UsrSWTXTable1> &);
	//判断一个尺寸和参数是否已经在关系式(等式类型)的左侧
	bool IsDimParaInRelSetLeftPart(ProSolid,ProParameter *,ProDimension *);
private:
	CArray<CRel,CRel> m_CRelList;//内部关系列表
	//TRUE就是已经存在
	BOOL IsRelInColection(ProMdlType ,CString str);
	//获得模型中所有的尺寸和参数
	void GetSolidAllDimPara(ProSolid,CArray<UsrSWTXTable1,UsrSWTXTable1> &);
	//判断关系式左侧是否有事物特性表项
	bool HasSwtxItemInRelLeft(ProSolid solid,CRel * rel,CString & csError);
};
