//把组建装配进来
#pragma once

class CAsmAssembly
{
public:
	CAsmAssembly(void);
	~CAsmAssembly(void);
	//元件的结构
	typedef struct comp_item{
		CString          type;        //类型 组件和零件 "asm" "prt"
		ProSolid         mdl;         //模型的句柄
		int              postid;      //后缀
		ProDrawing       drw;
		long             num;         //标识号 零件实例 或 组件实例
		long             mdlcomnum;   //这个零部件在产品模型中的编号，用于获得把组件模型的可变信息添加进来
		long             mdlnum;      //模型号 零件模型的编号 或组件模型的编号 如果不是模型 -1
		CString          pdmnum;
		CString          filename;    //文件名
		CString          filefullpath;//文件的存放位置
		CString          drawfilename;    //文件名
		CString          drawfilefullpath;//文件的存放位置
		int              fileresource;//来源 0本地系统  1 设计系统 2 PDM系统
		bool             status;      //是否已经在当前有存在 true 表示存在 false表示不存在
	}comp_item;
	CArray<comp_item,comp_item> p_compitemlist;
	CDatabase * m_db;
	long        m_asm_num;
	CString     error_info;
	//下载组件产品，返回组件句柄
	BOOL  OpenProduct(CDatabase * p_db,long asm_num,ProSolid * asm_com);
	//检查组件产品的所有零部件是否已经在当前打开
	//用户必须确认才行
	//对于组件中已经有的组件与现在的组件是属于同一个的则不添加信息
	//获得零件的存放位置
	BOOL  GetCompInfo(long asm_num);
	BOOL  GetCompMdl();										 //获得模型句柄    
	BOOL  CheckProduct(long asm_num);
	//下载，并把模型导入内存
	BOOL  LoadSolid(long asm_num);
	//装配成功添加信息
	BOOL AddInfo();
	//装配不成功不添加信息
	BOOL CancelAssembly();
	BOOL AddComInfo(long asm_num);                           //给元件添加信息
	BOOL AddCompPartInfo(ProSolid mdl,ProDrawing part_draw,long num);             //给零件添加信息
	BOOL AddCompAsmInfo(ProSolid mdl,ProDrawing part_draw,long num);              //给组件添加信息
	CString  localDir;//下载的目录
	ProSolid asm_solid;
	ProDrawing asm_draw;
};
