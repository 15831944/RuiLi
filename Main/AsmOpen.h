#pragma once
#include "AsmManageTableSet.h"
#include "AsmInfoFn.h"
class CAsmOpen
{
public:
	CAsmOpen(void);
	~CAsmOpen(void);
	typedef struct asm_mdl_item{
		ProSolid  mdl;
		int       num;
		int       postid;
	}asm_mdl_item;
	long asm_num;
	ProSolid                    asm_solid;                               //顶层模型的句柄
	ProSolid                    asm_drw;
	CAsmManageTableSet *        m_pSet;
	CAsmInfoFn                  asm_inf_fn;  //用于添加组件的信息
	CString                     localDir;
	CArray<asm_mdl_item,asm_mdl_item>       p_temp_AsmMdlItemList;
	CStringArray                            p_temp_rel;      //数据库中的关系，然后替换，
	BOOL Open(CDatabase * p_db,long  num);
	//检查当前的零部件，是否有相同的存在，
	//以及是否同名
	BOOL CheckCurrentMdl(long num);

	BOOL LoadSolid(long num);                                //下载并打开模型，进行某型的编号，得到后缀
	BOOL AddAsmSolidInfo(long num);                          //把组件的基本信息添加进来  
	//BOOL ReplaceRel(CString db_rel,CString * rel);           //替换模型的关系式，然后添加到模型中去
/*	BOOL SetSWTX(); */                                         //还原事物特性表  
	//ProSolid GetComp(int num);
	BOOL AddCompInfo(long num);
	BOOL AddCompPartInfo(ProSolid mdl,long num);             //给零件添加信息
	BOOL AddCompAsmInfo(ProSolid mdl,long num);              //给组件添加信息
	//得到组件的工程图
	ProError LoadCompDrawing(int ResourceType,CString filename,CString filepath,ProDrawing * comp_dra);
};
