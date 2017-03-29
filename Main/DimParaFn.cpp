#include "stdafx.h"
#include "resource.h"
/************************************************************************/
/* 获得尺寸和参数的值(只输入一项)                                       */
/************************************************************************/
double UsrGetDimParaValue(ProDimension * dim/*尺寸*/
						  ,ProParameter * para/*参数*/)
{
	double			reVal=0.0;
	ProParamvalue	proval;
	if (dim!=NULL)
	{
		ProDimensionValueGet(dim,&reVal);
	}
	if (para!=NULL)
	{
		ProParameterValueGet(para,&proval);
		switch(proval.type) {
		case PRO_PARAM_DOUBLE:
			reVal=proval.value.d_val;
			break;
		case PRO_PARAM_INTEGER:
			reVal=(double)proval.value.i_val;
			break;
		default:
			reVal=0.0;
			break;
		}
	}
	return reVal;
}
/************************************************************************/
/* 获得参数的值                                                         */
/************************************************************************/
CString UsrGetParaValue(ProParameter * para/*参数*/,CString * Type)
{
	CString			value="";
	CString         subType;
	ProParamvalue	proval;
	if (para!=NULL)
	{
		ProParameterValueGet(para,&proval);
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
	}	
	(*Type)=subType;
	return value;
}
/************************************************************************/
/* 显示尺寸                                                             */
/************************************************************************/
ProError UsrShowDim(ProSolid solid,ProDimension  * dim)
{
	ProError   status;
	if (GetMdlType((ProMdl)solid)==PRO_MDL_PART)
	{
		status=ProDimensionShow(dim,NULL,NULL,NULL);
	}
	if (GetMdlType((ProMdl)solid)==PRO_MDL_ASSEMBLY)
	{
		CArray<int,int> p_IDTalbe;
		int             p_IDTalbe1[PRO_MAX_ASSEM_LEVEL];
		ProAsmcomppath  p_handle;
		if (UsrGetSolidComppath(solid,ProSolid(dim->owner),p_IDTalbe))
		{
			for (int i=0;i<p_IDTalbe.GetCount();i++)
			{
				p_IDTalbe1[i]=p_IDTalbe[i];
			}
			status=ProAsmcomppathInit(solid,p_IDTalbe1,(int)p_IDTalbe.GetCount(),&p_handle);
			status=ProDimensionShow(dim,NULL,NULL,&p_handle);
		}
		else
		{
			status=ProDimensionShow(dim,NULL,NULL,NULL);
		}
	}
	return PRO_TK_NO_ERROR;
}
/************************************************************************/
/* 获得制定模型，制定名称的尺寸和参数项
type=TYPE_DIM TYPE_PARA                                               */
/************************************************************************/
BOOL GetDimPara(ProSolid solid,ProName name,ProDimension * dim,ProParameter * para,int * type)
{
	//首先查找参数，如果参数中找不到，就找尺寸
	ProModelitem   owner;
	ProError       status;
	ProDimension   *datalist;
	ProName        symbol;
	int            num=0;
	CString        temp,temp1,temp2;
	CArray<ProParameter,ProParameter> Parameterlist;
	ProArrayAlloc(0,sizeof(ProDimension),1,(ProArray*)&datalist);
	status=SgCollectDim(solid,(ProDimension **)&datalist);	    //(In)尺寸对象数组
	if (status==PRO_TK_NO_ERROR)
	{
		ProArraySizeGet(datalist,&num);
		for (int i=0;i<num;i++)
		{
			if(ProDimensionSymbolGet(&(datalist[i]),symbol)==PRO_TK_NO_ERROR)
			{
				temp1=CString(symbol);temp2=CString(name);
				if(temp1.MakeLower() ==temp2.MakeLower())
				{
					(*type)=TYPE_DIM;
					(*dim)=datalist[i];
					ProArrayFree((ProArray*)&datalist);
					return TRUE;
				}
			}
			else
			{
				temp.Format("d%d",datalist[i].id);
				temp2=CString(name);
				if(temp==temp2.MakeLower())
				{
					(*type)=TYPE_DIM;
					(*dim)=datalist[i];
					ProArrayFree((ProArray*)&datalist);
					return TRUE;
				}
			}

		}
	}
	status=ProMdlToModelitem((ProMdl)solid,&owner);
	status=ProParameterVisit(&owner,NULL,(ProParameterAction)partAsmParaVisAc
		,(CArray<ProParameter,ProParameter>*)&Parameterlist);
	num=(int)Parameterlist.GetCount();
	for (int i=0;i<num;i++)
	{
		temp1=CString(Parameterlist[i].id);temp2=CString(name);
		if (temp1.MakeLower() == temp2.MakeLower() )
		{
			(*type)=TYPE_PARA;
			(*para)=Parameterlist[i];
			return TRUE;
		}
	}
	AfxMessageBox("没有找到："+CString(name));
	return FALSE;
}
/************************************************************************/
/* 比较两个参数是否相等                                                 */
/************************************************************************/
BOOL ComparaParameter(ProParameter * para1,ProParameter * para2)
{
	if (CString(para1->id)==CString(para2->id) && para1->type==para2->type && para1->owner.type==para2->owner.type) 
	{
		switch(para1->owner.type) {
		case PRM_MODEL:
			if (para1->owner.who.model == para2->owner.who.model)  return TRUE;
			else return FALSE;
			break;
		default:
			if (para1->owner.who.item.id==para2->owner.who.item.id && para1->owner.who.item.owner==para2->owner.who.item.owner
				&& para1->owner.who.item.type==para2->owner.who.item.type)  return TRUE;
			else return FALSE;
			break;
		}
	}
	else return FALSE;
}
/************************************************************************/
/* 比较两个参数是否相等                                                 */
/************************************************************************/
BOOL ComparaDimension(ProDimension * para1,ProDimension * para2)
{
	if (para1->id==para2->id && para1->owner==para2->owner && para1->type==para2->type)
	{
		return TRUE;
	}
	return FALSE;
}
/************************************************************************/
/* 获得参数的父项句柄                                                   */
/************************************************************************/
ProSolid UsrGetParameterOwner(ProParameter * para)
{
	return (ProSolid)(para->owner.type==PRM_MODEL?para->owner.who.model:para->owner.who.item.owner);
}