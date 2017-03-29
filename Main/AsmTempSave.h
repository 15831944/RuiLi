#pragma once
#include "AsmTempManageTableSet.h"
class CAsmTempSave
{
public:
	CAsmTempSave(void);
	~CAsmTempSave(void);
	void MarkComp(ProSolid comp, int upLevel);
	typedef struct asm_comp_item {
		ProSolid mdl;       //元件句柄
		int      num;       //元件的编号
		int      upLevel;   //上层编号
		int      postId;    //后缀
	}asm_comp_item,*AsmCompItemList;

	//相关尺寸、参数表
	typedef struct Asm_Para_item {
		int      solid_num;     //所在模型的编号，作为添加到数据库中的后缀
		CString  type;          //"para" "dim"
		CString  id;            //"名称/符号 ： 模型号"
		CString  DimSymbol;     //尺寸符号
		CString  Name;          //尺寸符号，参数名称 有后缀
		CString  featName;          //特征才有效
		CString  SubType;       //子类
		CString  Value;         //参数，尺寸的值  ，值不能够为空，要判断
		CString  UpLimit;
		CString  DwLimit;
		CString  Note;
	}Asm_Para_item,*AsmParaItemList;
	CArray<AsmRelItem,AsmRelItem> tempRelList;               //替换后缀之后的关系
	CArray<asm_comp_item,asm_comp_item> p_AsmCompItem;       //模型编号与后缀对应关系
	CArray<Asm_Para_item,Asm_Para_item> p_AsmRelRvItem;      //模型的相关尺寸项
	CArray<Asm_Para_item,Asm_Para_item> P_temp_AsmSmlItem;   //模型的事务特性表项
	CAsmTempManageTableSet * m_pSet;
	int                asm_num;                              //添加到数据库中，装配体的编号，初始值为-1
	long               asm_father_num;
	CString            fileDirectory;                        //模型在服务器中的存放位置
	CString            fileLodcalDir;                        //模型在本地的存放位置
	ProSolid   asm_solid;                                      //装配件的句柄,需要在弹出对话框之前付值
	//BOOL       ReplaceRelPostIdWithMdlNum(void);
	//BOOL       GetRelRvItem(void);
	void       GetSWTXCStringArray(CStringArray * cs_swtx_item);
	int        GetSolidNum(ProSolid solid);
	BOOL       GetSWTXItem(void);
	BOOL InsertAsmPropInfo(void);
	BOOL InsertAsmMakeupInfo(void);
	BOOL InsertSWTX(void);
	//BOOL InsertRelInfo(void);
	BOOL InsertPerfInfo(void);
	BOOL InsertAsmPicInfo(void);
	BOOL CopyLocalDirToServer(void);
	BOOL InsertDrwInfo(void);
	BOOL Save(ProSolid solid,long fatherNum,CDatabase * p_db);
};
