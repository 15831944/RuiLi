// UtilDimen.cpp : 实现文件
//

#include "stdafx.h"

#include "UtilDimen.h"

ProError  SgCollectFeat(ProSolid solid,			//(In)当前模型
						ProFeature **p_data,BOOL nFlag=TRUE);

ProError  FeatureCollectDim(ProFeature feature,			//(In)指定特征
							ProDimension **p_data);
ProError  SgCollectDim(ProSolid solid,			//(In)指定模型
					   ProDimension **p_data);	//(In)尺寸对象数组
ProError  sgCollectParameters( 
							  ProModelitem    *p_modelitem,   /* In:  The model item */
							  ProParameter    **p_parameters );

// CUtilDimen

CUtilDimen::CUtilDimen()
{
	errorString="出错";
	isAssembly=TRUE;
}

CUtilDimen::~CUtilDimen()
{
}


// CUtilDimen 成员函数

int CUtilDimen::AddPartDim(ProSolid solid,ProAsmcomppath *comppath)
{
	ProError status;
	ProName symbol;
	double dvalue;
	CString str;
	DimStruct ds;
	ParamStruct ps;
	int i,n;
	CString strPostid="";
	int postid;
	CString solidLabel;
	ProName name;
	ProMdlNameGet(solid,name);
	solidLabel=CString(name);
    ds.solidLabel=solidLabel;
	ps.solidLabel=solidLabel;
	ProSolidToPostfixId(solid,&postid);
	ds.postid=postid;
	ps.postid=postid;
	if(comppath!=NULL)
	{
		ds.comppath.owner =comppath->owner;
		ps.comppath.owner=comppath->owner;
		ds.comppath.table_num=comppath->table_num;
		ps.comppath.table_num=comppath->table_num;
		for(i=0;i<comppath->table_num;i++)
		{
			ds.comppath.comp_id_table[i]=comppath->comp_id_table[i];
			ps.comppath.comp_id_table[i]=comppath->comp_id_table[i];
		}
		//strPostid.Format(":%d",postid);
            strPostid.Format(":%s",solidLabel);
	}
	else if(isAssembly==TRUE)
	{
		//strPostid.Format(":%d",postid);
        strPostid.Format(":%s",solidLabel);
	}

	ProDimension *featureDim;
	status=SgCollectDim(solid,&featureDim);
	status=ProArraySizeGet((ProArray)featureDim,&n);
	for(i=0;i<n;i++)
	{
		//获得尺寸名称
		status = ProDimensionSymbolGet (featureDim+i, symbol);
		status = ProDimensionValueGet (featureDim+i, &dvalue);
		str=CString(symbol);
		ds.label=str;
		ds.value=dvalue;
		str=ds.label+strPostid;
		ds.Llabel=str;
		ds.dimension.id=(featureDim+i)->id;
		ds.dimension.owner=(featureDim+i)->owner;
		ds.dimension.type=(featureDim+i)->type;
		dimArray.Add(ds);
	}
	ProArrayFree((ProArray *)&featureDim);
//////参数
ProModelitem item;
ProMdlToModelitem((ProMdl)solid,&item);
ProParameter * parameter;
ProParamvalue value;
sgCollectParameters(&item,&parameter);
status = ProArraySizeGet((ProArray)parameter, &n);
for(i=0;i<n;i++)
{
	ProParameterValueGet((parameter+i),&value);
	str=CString((parameter+i)->id);
	ps.label=str;
	str=str+strPostid;
	ps.Llabel=str;
	
	switch(value.type) {
		case PRO_PARAM_DOUBLE:
			str.Format("%0.3f",value.value.d_val);
			ps.value=str;
			str.Format("实数型");
			ps.type=str;
			break;
		case PRO_PARAM_STRING:
			str=CString(value.value.s_val);
			ps.value=str;
			str.Format("字符串型");
			ps.type=str;
			break;
		case PRO_PARAM_INTEGER:
			str.Format("%d",value.value.i_val);
		    ps.value=str;
			str.Format("整型");
	        ps.type=str;
			break;
		case PRO_PARAM_BOOLEAN:
			str.Format("%d",value.value.l_val);
			ps.value=str;
			str.Format("整型");
			ps.type=str;
			break;
		case PRO_PARAM_NOTE_ID:
			break;
		case PRO_PARAM_VOID:
			break;
		default:
			break;
	}
	ProBoolean isChecked;
	ProParameterDesignationVerify((parameter+i),&isChecked);
	if(isChecked)
		ps.isChecked=TRUE;
	else
		ps.isChecked=FALSE;
	paramArray.Add(ps);
}
ProArrayFree((ProArray *)&parameter);

	return 1;

}

int CUtilDimen::AddAssemblyDim(ProSolid solid,int **a,int j)
{
	int i,n;
	CString str;
	ProError status;
	ProFeature * feature;
	ProFeattype   featType;
	ProMdlType  mdlType;
	int id;
	SgCollectFeat(solid,&feature,FALSE);
	status = ProArraySizeGet((ProArray)feature, &n);
	for(i=0;i<n;i++)
	{
		status =ProFeatureTypeGet(feature+i, &featType);
		id=(feature+i)->id;
		if(featType==PRO_FEAT_COMPONENT)
		{
			ProSolid newSolid;               
			status=ProAsmcompMdlGet((ProAsmcomp *)(feature+i),(ProMdl *)&newSolid);
			if(status==PRO_TK_NO_ERROR)
			{
				int *table=new int[j+1];
				for(int t=0;t<j;t++)
				{
					table[t]=*(*a+t);
				}
				table[j]=id;
				ProAsmcomppath comppath;
                status=ProAsmcomppathInit(root,table,j+1,&comppath);
				AddPartDim(newSolid,&comppath);
				ProMdlTypeGet(newSolid,&mdlType);
				if(mdlType==PRO_MDL_PART)
				{
					ProName name;  
					ProName partName;
					ProParamvalue value; 
					ProParameter  param;
					ProModelitem owner;
					ProMdlToModelitem(newSolid,&owner);
					str="DrawingNumber";
					ProStringToWstring(name,str.GetBuffer());
                    ProMdlNameGet(newSolid,partName);
					str=CString(partName);
					value.type=PRO_PARAM_STRING;
					ProLine *pLine;
					pLine=&(value.value.s_val);
					ProStringToWstring(*pLine,str.GetBuffer());
					status=ProParameterCreate (&owner,name,&value,&param);
				}
				if(mdlType==PRO_MDL_ASSEMBLY)
				AddAssemblyDim(newSolid,&table,j+1);
				delete [] table;
			}
		}
	}
	

	return 1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CUtilDimen::Load(ProSolid solid)
{
	root=solid;
	dimArray.RemoveAll();
	paramArray.RemoveAll();
    ProMdlType  mdlType;
	ProMdlTypeGet(root,&mdlType);
	if(mdlType==PRO_MDL_ASSEMBLY)
	{
		isAssembly=TRUE;
	int *a=new int[1];
	AddPartDim(root,NULL);
	AddAssemblyDim(root,&a,0);
	delete [] a;
	}
	else
	{
		isAssembly=FALSE;
		AddPartDim(root,NULL);
	}
	return 1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CUtilDimen::GetPosition(CString label,int postid)
{
	int i;
	for(i=0;i<dimArray.GetSize();i++)
	{
		if((dimArray[i].label==label)&&(dimArray[i].postid==postid))
			return i;
	}
	return -1;
}

int CUtilDimen::GetPosition(CString Llabel)
{
	
	int i;
	for(i=0;i<dimArray.GetSize();i++)
	{
		if((dimArray[i].Llabel==Llabel))
			return i;
	}
	return -1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CUtilDimen::GetPositionEx(CString Llabel)
{
	
	int i;
	for(i=0;i<paramArray.GetSize();i++)
	{
		if((paramArray[i].Llabel==Llabel))
			return i;
	}
	return -1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
double CUtilDimen::GetValue(CString label,int postid)
{
	int i;
	i=GetPosition(label,postid);
	if(i!=-1)
	    return dimArray[i].value;
	else
		return -1;
}

CString CUtilDimen::GetValue(CString Llabel)
{

	int i;
	i=GetPosition(Llabel);
	CString str;
	if(i!=-1)
	{
		str.Format("%0.2f",dimArray[i].value);
		return str;
	}
	i=GetPositionEx(Llabel);
	if(i!=-1)
	{
		str.Format("%s",paramArray[i].value);
		return str;
	}
	return "";
}

////
double CUtilDimen::GetValue(int index)
{
	int i;
	i=index;
	if(i!=-1)
		return dimArray[i].value;
	else
		return -1;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CUtilDimen::GetType(CString label,int postid)
{
	CString str;
	int i;
	i=GetPosition(label,postid);
	if(i!=-1)	
		return GetType(i,"尺寸");
	return errorString;

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CUtilDimen::GetType(int index,CString stype)
{
	ProError status;
	ProDimensiontype type;
	CString str;
	int i;
	i=index;
	if(i==-1)
		return errorString;
	if(stype=="尺寸")
	{
	status=ProDimensionTypeGet(&(dimArray[i].dimension),&type);
	switch(type) {
			case PRODIMTYPE_UNKNOWN:
				str="未知类型";
				return str;
				break;
			case PRODIMTYPE_LINEAR:
				str="线性尺寸";
				return str;
				break;
			case PRODIMTYPE_RADIUS:
				str="半径尺寸";
				return str;
				break;
			case PRODIMTYPE_DIAMETER:
				str="直径尺寸";
				return str;
				break;
			case PRODIMTYPE_ANGLE:
				str="角度尺寸";
				return str;
				break;
			default:
				str=errorString;
				return str;
				break;
	        }
	    }
	else if(stype=="参数")
	{
	      return paramArray[i].type+"参数";
	}
	return errorString;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CString CUtilDimen::GetType(CString Llabel)
{
	int nIndex=-1;
	nIndex=GetPosition(Llabel);
	if(nIndex!=-1)
		return GetType(nIndex,"尺寸");
  nIndex=GetPositionEx(Llabel);
  if(nIndex!=-1)
	  return GetType(nIndex,"参数");
	return "";
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CUtilDimen::Show(CString label,int postid)
{
	int i;
	i=GetPosition(label,postid);
	if(i!=-1)
	{
		return Show(i);
	}
	return -1;
}

int CUtilDimen::Show(CString Llabel)
{
	int nIndex=-1;
	nIndex=GetPosition(Llabel);
	if(nIndex!=-1)
		return Show(nIndex);
	return -1;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CUtilDimen::Show(int index)
{
	int i;
	ProError status;
    int post;
	i=index;
	if(i!=-1)
	{
		ProSolidDisplay(root);
		status=ProSolidToPostfixId(root,&post);
		if(post==dimArray[i].postid)
        status=ProDimensionShow(&(dimArray[i].dimension),NULL,NULL,NULL);
		else
		status=ProDimensionShow(&(dimArray[i].dimension),NULL,NULL,&(dimArray[i].comppath));
		if(status==PRO_TK_NO_ERROR)
			return 1;
		else
			return 0;
	}
	return -1;
}



int CUtilDimen::SetValue(CString Llabel,double value)
{
	int i;
	ProError status;
	i=GetPosition(Llabel);
	if(i!=-1)
	{
		status=ProDimensionValueSet(&(dimArray[i].dimension),value);
		if(status==PRO_TK_NO_ERROR)
		{
			dimArray[i].value=value;
			return 1;
		}
		return 0;
	}
	i=GetPositionEx(Llabel);
		if(i!=-1)
		{
			ProParamvalue pvalue; 
			ProParameter  param;
			ProName name; 
			CString str;
            ProStringToWstring(name,paramArray[i].label.GetBuffer());
			str=paramArray[i].type;
			if(str=="实数型")
			{
				pvalue.type=PRO_PARAM_DOUBLE;
				pvalue.value.d_val=atof(paramArray[i].value);
			}
			if(str=="整型")
			{
				pvalue.type=PRO_PARAM_INTEGER;
				pvalue.value.d_val=atoi(paramArray[i].value);
			}
			if(str=="字符串型")
			{
				pvalue.type=PRO_PARAM_STRING;
				ProLine *pLine;
				pLine=&(pvalue.value.s_val);
				ProStringToWstring(*pLine,paramArray[i].value.GetBuffer());
			}
            ProSolid temp;
			ProModelitem owner;
            ProPostfixIdToSolid(paramArray[i].postid,&temp);
            ProMdlToModelitem((ProMdl)temp,&owner);
			status = ProParameterInit (&owner, name,&param);
			status = ProParameterValueSet (&param, &pvalue);
			if(status==PRO_TK_NO_ERROR)
			{return 1;}
			else
			{
				str.Format("参数%s更新不成功",paramArray[i].label);
				AfxMessageBox(str);
				return 0;
			}
		}
	return -1;
}

int CUtilDimen::SetValue(int index,double value)
{
	int i;
	ProError status;
	i=index;
	if(i!=-1)
	{
		status=ProDimensionValueSet(&(dimArray[i].dimension),value);
		if(status==PRO_TK_NO_ERROR)
		{
			dimArray[i].value=value;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return -1;
}

int CUtilDimen::GetTolerance(CString Llabel,double * upper_limit,double * lower_limit)
{
	int i;
	ProError status;
	i=GetPosition(Llabel);
	if(i!=-1)
	{
		status=ProDimensionToleranceGet(&(dimArray[i].dimension),upper_limit,lower_limit);
		if(status==PRO_TK_NO_ERROR)
			return 1;
		return 0;
	}
	return -1;
}


int CUtilDimen::GetTolerance(int index,double * upper_limit,double * lower_limit)
{
	int i;
	ProError status;
	i=index;
	if(i!=-1)
	{
		status=ProDimensionToleranceGet(&(dimArray[i].dimension),upper_limit,lower_limit);
		if(status==PRO_TK_NO_ERROR)
			return 1;
		return 0;
	}
	return -1;
}


int CUtilDimen::SetTolerance(CString Llabel,double  upper_limit,double lower_limit)
{
	int i;
	ProError status;
	i=GetPosition(Llabel);
	if(i!=-1)
	{
		status=ProDimensionToleranceSet(&(dimArray[i].dimension),upper_limit,lower_limit);
		if(status==PRO_TK_NO_ERROR)
			return 1;
		return 0;
	}
	return -1;
}


int CUtilDimen::SetTolerance(int index,double  upper_limit,double  lower_limit)
{
	int i;
	ProError status;
	i=index;
	if(i!=-1)
	{
		status=ProDimensionToleranceSet(&(dimArray[i].dimension),upper_limit,lower_limit);
		if(status==PRO_TK_NO_ERROR)
			return 1;
		return 0;
	}
	return -1;
}
