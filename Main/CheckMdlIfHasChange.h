#pragma once
//检查模型是否已经改变
typedef enum REVTYPE{
	CHECK_FALSE=-1,
	CHECK_TRUE=0,
	CHECK_HAVE_NO_SWTX=1,
	CHECK_DIM_OR_PARA_WAS_DELTETED=2,
	CHECK_DIM_HAS_CHANGED=3,
	CHECK_PARA_HAS_CHANGED=4,
};
class CCheckMdlIfHasChange
{
public:
	CCheckMdlIfHasChange(void);
	~CCheckMdlIfHasChange(void);
private:
	typedef struct dim_para_item{
		int           type;      //零件或模型TYPE_DIM TYPE_PARA
		long          Num;       //编号
		CString       Name;
		CString       IdNum;     //尺寸和参数的标识
		ProDimension  Dim;       //尺寸
		ProParameter  Para;      //参数
		CString       ParaType;  //参数的类型

		double        mValue;       //模型的初始值
		CString       mParaStrValue;   //不是数字情况下的参数值
		int           mParaIntValue;   //参数是整数情况下的值 

		double        nValue;       //现有实例的值    
		CString       nParaStrValue;   //不是数字情况下的参数值
		int           nParaIntValue;   //参数是整数情况下的值     
	};

	typedef struct asm_mdl_item{
		ProSolid  mdl;
		int       num;
		int       postid;
	}asm_mdl_item;
public:
	//检查可变参数的值是否与原有的不同

	//检查一个具有模型的零件
	REVTYPE CheckPartHasPrtMdl(CDatabase * p_db,ProSolid solid,long PrtNum,int CheckType/*表示检查是否有修改，或者判断是新增的实例*/);
	//检查一个没有模型的零件
	REVTYPE CheckPartNoPrtMdl(CDatabase * p_db,ProSolid solid, long PrtNum,int CheckType);
	//检查一个具有模型的部件
	REVTYPE CheckAsmHasAsmMdl(CDatabase * p_db,ProSolid solid, long AsmNum,int CheckType);
	//检查一个没有模型的部件
	REVTYPE CheckAsmNoAsmMdl(CDatabase * p_db,ProSolid solid, long AsmNum,int CheckType);
	//检查零件
	REVTYPE CheckPart(CDatabase * p_db,ProSolid solid,long PrtNum,int CheckType);
	//检查部件
	REVTYPE CheckAsm(CDatabase * p_db,ProSolid solid,long AsmNum,int CheckType);
    //检查关系式
	REVTYPE CheckRel(CDatabase * p_db,ProSolid solid,long Num,int CheckType/*检查零件0还是检查组件1*/);
	//检查可变参数是否与
	//根据返回值来判断
	CArray<ProDimension,ProDimension> p_ReDimList;         //已经修改过的尺寸
	CStringArray                      p_ReDimYuanlaiVal;   //原来的尺寸值
	CArray<ProParameter,ProParameter> p_ReParaList;        //修改过的参数的值
	CStringArray					  p_ReParaYuanlaiVal;  //原来的参数值
    CString							  error_info;
	long							  changenum;           //零件被修改之后与模型中其它的零件是一致
	CArray<asm_mdl_item,asm_mdl_item>       p_temp_AsmMdlItemList;
	void InitReValue();
};
