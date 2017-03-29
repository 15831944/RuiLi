// UsrSWTXTable.cpp : ʵ���ļ�
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
/* ������Ϊ�ߴ�ʱ�ĳ�ʼ��                                               */
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
    //2.����
	type=_T("dim");
    //3.����
	ProDimensionSymbolGet(&Dim,symbol);
	ASSERT(symbol!=NULL);

	ProWstringToString(cSymbol,symbol);
	dimSymbol=_T(cSymbol);
	
	Name=dimSymbol;
	//4.ֵ
	ProDimensionValueGet(&Dim,&d_value);
	value.Format("%f",d_value);
	//5.������
	ProDimensionTypeGet(&Dim,&dim_type);
	switch(dim_type) {
			case PRODIMTYPE_LINEAR:
				//csType="����";
				subType="PRODIMTYPE_LINEAR";
				break;
			case PRODIMTYPE_RADIUS:
				//csType="�뾶";
				subType="PRODIMTYPE_RADIUS";
				break;
			case PRODIMTYPE_DIAMETER:
				//csType="ֱ��";
				subType="PRODIMTYPE_DIAMETER";
				break;
			case PRODIMTYPE_ANGLE:
				//csType="�Ƕ�";
				subType="PRODIMTYPE_ANGLE";
				break;
			default:
				//csType="δ֪";
				subType="";
				break;
	}
    //6.�������ƫ��
    ProDimensionToleranceGet(&Dim,&upper_limit,&lower_limit);
    UpLimit.Format("%f",upper_limit);
    DwLimit.Format("%f",lower_limit);
	//7.ɾ�����,�жϲ��������������Ƿ��Ѿ�������
	dimSymbol.IsEmpty()?isCheck=0:isCheck=1;
}

/************************************************************************/
/* ������Ϊ����ʱ��ĳ�ʼ��                                             */
/************************************************************************/
void UsrSWTXTable1::InitPara(void)
{
	ProParamvalue proval;
	//1.��ʶ��/����/����
	id=CString(Para.id);
	Name=id;
	type=_T("para");
	featName=id;
	//2.����ֵ/������
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
	//3.ɾ�����,�жϲ����Ƿ��Ѿ���ɽ�������ֵΪ�գ���ò�������ɾ��
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
/* ͨ����������ֵ������ʼ��Dim��Para                                  */
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
