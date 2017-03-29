// UsrSWTXTable.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "UsrSWTXTable.h"
//ProError partParaVisAc(ProParameter* p_parameter,ProError status,
//					   CArray<ProParameter,ProParameter> * app_data);

// UsrSWTXTable
UsrSWTXTable1::UsrSWTXTable1()
	:CObject(),owner(NULL),
	id(_T(""))
	,Name(_T(""))
	,dimSymbol(_T(""))
	,featName(_T(""))
	,subType(_T(""))
	,value(_T(""))
	,UpLimit(_T(""))
	,DwLimit(_T(""))
	,Note(_T(""))
	,isCheck(0) 
	,type(_T(""))
{	
}
UsrSWTXTable1::UsrSWTXTable1(ProSolid solid,ProDimension * p_dim)
	:CObject(),owner(NULL),
	id(_T(""))
	,Name(_T(""))
	,dimSymbol(_T(""))
	,featName(_T(""))
	,subType(_T(""))
	,value(_T(""))
	,UpLimit(_T(""))
	,DwLimit(_T(""))
	,Note(_T(""))
	,isCheck(0) 
	,type(_T(""))
{
	if(solid==NULL) return; 
	owner=solid;
	if(p_dim==NULL) return;
     Dim=*p_dim;
	InitDim();	
}
UsrSWTXTable1::UsrSWTXTable1(ProSolid solid,ProParameter * p_para)
:CObject(),owner(NULL),
id(_T(""))
,Name(_T(""))
,dimSymbol(_T(""))
,featName(_T(""))
,subType(_T(""))
,value(_T(""))
,UpLimit(_T(""))
,DwLimit(_T(""))
,Note(_T(""))
,isCheck(0) 
,type(_T(""))
{
	if(solid==NULL) return; 
	owner=solid;
	if(p_para==NULL) return;
	Para=*p_para;
	InitPara();	
}
UsrSWTXTable1::UsrSWTXTable1(const UsrSWTXTable1& RightSides)
{
	id=RightSides.id;
	Name=RightSides.Name;
	dimSymbol=RightSides.dimSymbol;
	featName=RightSides.featName;
	subType=RightSides.subType;
	value=RightSides.value;
	UpLimit=RightSides.UpLimit;
	DwLimit=RightSides.DwLimit;
	Note=RightSides.Note;
	isCheck=RightSides.isCheck;
	Dim=RightSides.Dim;
	Para=RightSides.Para;
	type=RightSides.type;
	owner=RightSides.owner;
}
UsrSWTXTable1 & UsrSWTXTable1::operator = (const UsrSWTXTable1& RightSides)
{
	id=RightSides.id;
	Name=RightSides.Name;
	dimSymbol=RightSides.dimSymbol;
	featName=RightSides.featName;
	subType=RightSides.subType;
	value=RightSides.value;
	UpLimit=RightSides.UpLimit;
	DwLimit=RightSides.DwLimit;
	Note=RightSides.Note;
	isCheck=RightSides.isCheck;
	Dim=RightSides.Dim;
	Para=RightSides.Para;
	type=RightSides.type;
	owner=RightSides.owner;
	return *this;
}
UsrSWTXTable1::~UsrSWTXTable1()
{
}
/************************************************************************/
/* 当类型为尺寸时的初始化                                               */
/************************************************************************/
void UsrSWTXTable1::InitDim(void)
{	
	ProName              symbol;
	ProCharName          cSymbol;
	double               d_value;
	ProDimensiontype     dim_type;
	double               upper_limit,lower_limit;
	//1.ID
	id.Format("d%d",Dim.id);
    //2.类型
	type=_T("dim");
    //3.名称
	ProDimensionSymbolGet(&Dim,symbol);
	ASSERT(symbol!=NULL);

	ProWstringToString(cSymbol,symbol);
	dimSymbol=_T(cSymbol);
	
	Name=dimSymbol;
	//4.值
	ProDimensionValueGet(&Dim,&d_value);
	value.Format("%f",d_value);
	//5.子类型
	ProDimensionTypeGet(&Dim,&dim_type);
	switch(dim_type) {
			case PRODIMTYPE_LINEAR:
				//csType="线性";
				subType="PRODIMTYPE_LINEAR";
				break;
			case PRODIMTYPE_RADIUS:
				//csType="半径";
				subType="PRODIMTYPE_RADIUS";
				break;
			case PRODIMTYPE_DIAMETER:
				//csType="直径";
				subType="PRODIMTYPE_DIAMETER";
				break;
			case PRODIMTYPE_ANGLE:
				//csType="角度";
				subType="PRODIMTYPE_ANGLE";
				break;
			default:
				//csType="未知";
				subType="";
				break;
	}
    //6.获得上下偏差
    ProDimensionToleranceGet(&Dim,&upper_limit,&lower_limit);
    UpLimit.Format("%f",upper_limit);
    DwLimit.Format("%f",lower_limit);
	//7.删除标记,判断参数所属的特征是否已经不存在
	dimSymbol.IsEmpty()?isCheck=0:isCheck=1;
}

/************************************************************************/
/* 当类型为参数时候的初始化                                             */
/************************************************************************/
void UsrSWTXTable1::InitPara(void)
{
	ProParamvalue proval;
	//1.标识号/名称/类型
	id=CString(Para.id);
	Name=id;
	type=_T("para");
	featName=id;
	//2.参数值/子类型
	ProParameterValueGet(&Para,&proval);
	switch(proval.type) {
	case PRO_PARAM_DOUBLE:
		subType="PRO_PARAM_DOUBLE";
		value.Format("%f",proval.value.d_val);
		break;
	case PRO_PARAM_STRING:
		subType="PRO_PARAM_STRING";
		value=CString(proval.value.s_val);;
		break;
	case PRO_PARAM_INTEGER:
		value.Format("%d",proval.value.i_val);
		subType="PRO_PARAM_INTEGER";
		break;
	case PRO_PARAM_BOOLEAN:
		subType="PRO_PARAM_BOOLEAN";
		value.Format("%d",proval.value.l_val);
		break;
	default:
		subType=_T("");
		value=_T("");
		break;
	}
	//3.删除标记,判断参数是否已经北山出，如果值为空，则该参数将被删除
	value.IsEmpty()?isCheck=0:isCheck=1;
}

void UsrSWTXTable1::Update(void)
{
	CString  temp_name;
	CString  temp_note;
	temp_name=Name;
	temp_note=Note;
	if (type==_T("dim"))
	{
		InitDim();
	}
	if (type==_T("para"))
	{
		InitPara();
	}
	Name=temp_name;
	Note=temp_note;
}
/************************************************************************/
/* 通过输入各项的值，来初始化Dim和Para                                  */
/************************************************************************/
BOOL UsrSWTXTable1::SetDimPara(ProSolid solid)
{
	
	if (type==_T("dim"))
	{
		CString temp_id=id;
		Dim.id=atoi(temp_id.Trim("d").GetBuffer());
		Dim.type=PRO_DIMENSION ;
		Dim.owner=(ProMdl)solid;
		return TRUE;
	}
	else if (type==_T("para"))
	{
		CArray<ProParameter,ProParameter> Parameterlist;
		ProModelitem modelitem;
		ProMdlToModelitem((ProMdl)solid,&modelitem);
		ProParameterVisit(&modelitem,NULL,(ProParameterAction)partParaVisAc,
			(CArray<ProParameter,ProParameter>*)&Parameterlist); 
		for (int i=0;i<(int)Parameterlist.GetCount();i++)
		{
			if(id==CString(Parameterlist[i].id)) 
			{
				Para=Parameterlist[i];
				break;
			}
		}
		return TRUE;
	}
	else return FALSE;
}

//bool UsrSWTXTable1::operator==(const UsrSWTXTable1 & RightSide) const
//{
//	if (id==RightSides.id && )
//	{
//	}
//	id=RightSides.id;
//	Name=RightSides.Name;
//	dimSymbol=RightSides.dimSymbol;
//	featName=RightSides.featName;
//	subType=RightSides.subType;
//	value=RightSides.value;
//	UpLimit=RightSides.UpLimit;
//	DwLimit=RightSides.DwLimit;
//	Note=RightSides.Note;
//	isCheck=RightSides.isCheck;
//	Dim=RightSides.Dim;
//	Para=RightSides.Para;
//	type=RightSides.type;
//	owner=RightSides.owner;
//	return false;
//}
