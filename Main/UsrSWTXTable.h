#pragma once



// UsrSWTXTable ����Ŀ��

class UsrSWTXTable1 : public CObject
{
public:
	UsrSWTXTable1();
	UsrSWTXTable1(ProSolid solid,ProParameter * p_para);
	UsrSWTXTable1(ProSolid solid,ProDimension * p_dim);
	UsrSWTXTable1(const UsrSWTXTable1&);
	UsrSWTXTable1& operator = (const UsrSWTXTable1&); //��ֵ������
	virtual ~UsrSWTXTable1();//dim,feat,para,
	ProDimension  Dim;
	ProParameter  Para;
	ProMdl  owner;
	CString id;                //d35,f33,para��ʶ
	CString Name;              //�û���������
	CString type;
	CString dimSymbol;          //�ߴ����Ч
	CString featName;          //��������Ч
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


