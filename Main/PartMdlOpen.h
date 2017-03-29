#pragma once
class CPartMdlOpen
{
public:
	CPartMdlOpen(void);
	~CPartMdlOpen(void);
	CDatabase         *  m_db;
	ProSolid            part_solid;
	ProDrawing          part_draw;
	CString				part_dir;

	//用来打开零件
	BOOL OpenPart(CDatabase * p_db,long part_num);
	//用来打开零件模型
	BOOL Open(CDatabase * p_db,long part_num);
	//导入零件
	BOOL LoadPart(long num);
	//检查是否有相同的存在
	BOOL CheckCurrentPart(long num);
	//导入零件模型
	BOOL LoadSolid(long num);
	//导入零件模型
	BOOL CheckCurrentPartMdl(long num);
	BOOL AddPartMdlInfo(long  PrtNum);
	//给零件添加信息
	BOOL AddPartInfo(long  PrtNum);
	BOOL AddPartMdlInfoForOtherUse(CDatabase * p_db,ProSolid solid,long part_num);
};
