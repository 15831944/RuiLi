#pragma once



// UsrSWTXTable 命令目标

class UsrSWTXTable1 : public CObject
{
public:
	UsrSWTXTable1();
	UsrSWTXTable1(ProSolid solid,ProParameter * p_para);
	UsrSWTXTable1(ProSolid solid,ProDimension * p_dim);
	UsrSWTXTable1(const UsrSWTXTable1&);
	UsrSWTXTable1& operator = (const UsrSWTXTable1&); //赋值符重载
	virtual ~UsrSWTXTable1();//dim,feat,para,
	ProDimension  Dim;
	ProParameter  Para;
	ProMdl  owner;
	CString id;                //d35,f33,para标识
	CString Name;              //用户重新命名
	CString type;
	CString dimSymbol;          //尺寸才有效
	CString featName;          //特征才有效
	CString subType;
	CString value;
	CString UpLimit;
	CString DwLimit;
	CString Note;
	int     isCheck;  
protected:
	void InitDim(void);
	void InitPara(void);
public:
	void Update(void);
	BOOL SetDimPara(ProSolid solid);
	/*bool operator==(const UsrSWTXTable1 & RightSide) const;*/
};


