#pragma once
#include "ProSolid.h"


// DetailInfoClass 命令目标

class DetailInfoClass : public CObject
{
public:
	DetailInfoClass();
	virtual ~DetailInfoClass();
public:
	ProSolid  solid;				   //零件句柄
	CString   InsCode;				   //瑞立图号
	int       SolidID;				   //零件号或者组件号
	int       OldSolidID;              //旧的零件或者组件号
	int       ProductID;               //产品的ID,单个零件为-1
	CString   MdlNum;				   //模型编号
	CString   MdlName;                 //模型名字
	CString   InsName;				   //实例名
	CString   Type;					   //组件还是零件
	CString   Designer;				   //设计者
	CString   FinishTime;              //设计完成时间
	CString   Auditor;				   //审核者
	CString   AuditTime;			   //审核时间
	CString   Material;				   //材料编号
	CString   GrpFrm;                  //所属类别名字
	int       GroupNum;                //所属类别编号

	CString   Unit;                    //尺寸单位

	CString   AuditState;              //审核状态
	CString   DesignState;             //设计状态
	CString   PDMState;                //提交状态

	CString   SolidFilePath;		   //文件保存位置
	CString   SolidFileName;           //三维文件名

	CString   OldSolidFileName;		   //模型的文件名
	CString   OldDrwName;

	CString   DrwName;                 //二维文件名
	CString   DrwFilePath;

	CString   OldSolidFilePath;		   //模型的保存位置
	CString   OldDrwFilePath;

	int       SaveStyle;               //保存方式，0=插入，1=更新，2，无须提交
	BOOL      IsSaved;                 //信息是否已经完整
	CString   IsInsert;                //已经插入过

	CString   TempPath;				   //上传到服务器时，保存文件的临时目录地址
	CString   BackupPathSolid;			   //用来备份服务器下载的地址，以防保存失败的时候可以再上传
	CString   BackupPathDrw;

	int       MdlCompNum;              //在实例中和模型对应的编号，在模型打开是获得

	int       SolidVersion;
	int       DrwVersion;
	BOOL      CurrentSaveFlag;         //当前是否保存过的标志，在保存失败或成功用
	CString   TaskType;                //在创新设计中用

    CStringArray PerformanceValue;     //性能参数值
	CStringArray InsValueStr;          //性能参数



	CString   IsLocalOrServer;         //保存在本地还是服务器
	CString   PDMCode;                 //PDM图号
	CString   OldInsCode;              //要变型的零件或部件的基图号
	CString   Note;







	////////////////////////////////////////////////////////////////不要了
	//CString   PartMap;
	//CString   OldPartMap;
	//CString   PartNum;
	//BOOL      IsHave;       //是否定义了性能参数
	//BOOL      PerHave;       //实例是否有写有性能参数
	//BOOL      IsInsHave;	//是否有当前的实例
	//BOOL      IsVarHave;		//是否有可变参数
	//BOOL      IsThreeModelHave;         //是否有三维的模型存在
	//
	//BOOL      IsMenberHave;
	//BOOL      IsInsCodeNewOrNot;      //确定图号是新实例，还是就实例
	//int       InsNum;       //要存的最大的实例号


};


