#pragma once

class CAsmInfoFn
{
public:
	CAsmInfoFn(void);
	~CAsmInfoFn(void);
private:
	//把组件模型的性能参数信息添加到组件中
	BOOL AddAsmMdlPerfParaInfo(CDatabase * p_db,ProMdl asm_solid,long asm_num);
	//把组件模型的图片信息添加到组件中
	BOOL AddAsmMdlPicInfo(CDatabase * p_db,ProMdl asm_solid,long asm_num);
	//把组件模型的事务特性表信息添加到组件模型中
	BOOL AddAsmMdlSmlItemForMdl(CDatabase * p_db,ProMdl asm_solid,long asm_num);
	//把组件模型的事务特性表信息添加到组件实例中去
	BOOL AddAsmMdlSmlItemForIns(CDatabase * p_db,ProMdl asm_solid,long asm_num,long asm_mdl_num);


	//辅助函数
	//根据元件的名称，得到元件的
	BOOL GetComAndComNumList(CDatabase * p_db,ProMdl asm_solid,long asm_num
		,CArray<ProSolid,ProSolid> & solidlist
		,CArray<long,long>	& comnumlist);
public:
	//添加组件的模型信息
	BOOL AddAsmMdlInfo(CDatabase * p_db,ProMdl asm_solid,long asm_num);
	//添加组件的模型信息
	BOOL AddAsmInsInfo(CDatabase * p_db,ProMdl asm_solid,long asm_num);
};
