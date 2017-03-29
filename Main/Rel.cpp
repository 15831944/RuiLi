#include "StdAfx.h"
#include "rel.h"
/************************************************************************/
/*类型的使用方法：
  构造函数：输入关系的所属模型已经关系，可以对关系式进行分离，把关系式的
  尺寸和参数提取出来，把关系中尺寸和参数之间的数字以及运算符作为一个字符串
  对关系式的检查：
  得到关系的尺寸和参数
  计算关系的右边和左边的值                                              */
/************************************************************************/

CRel::CRel(void):bolNeedPostID(FALSE)
,m_strNote(_T("")),m_strError(_T("")),m_iRelType(REL_TYPE_NO),mdlType(PRO_MDL_UNUSED)
{
	InitCalSymbol();
}
/************************************************************************/
/* 构造函数，不检查关系正确与否                                         */
/************************************************************************/
CRel::CRel(ProSolid solid,CString strRel)
:bolNeedPostID(FALSE),m_strNote(_T("")),m_strError(_T("")),m_iRelType(REL_TYPE_NO),mdlType(PRO_MDL_UNUSED)
{
	InitCalSymbol();
	owner=solid;
	strRel.Trim();                               //清楚除空格
	sprintf(m_strRel,"%s\0",strRel.MakeLower());    //变小写
	//对于组件的关系式需要添加后缀
	if(solid==NULL)
	{
		return;
	}
	ProMdlTypeGet((ProMdl)solid,&mdlType);
	if (mdlType==PRO_MDL_ASSEMBLY)
	{
		bolNeedPostID=TRUE;
	}
	GetDimParaOfSolid(solid);
	DepartRel();
}
/************************************************************************/
/* 能够返回检查值，可用于任何的关系式检查                               */
/************************************************************************/
CRel::CRel(ProSolid solid,CString strRel,bool & Revbol)
:bolNeedPostID(FALSE),m_strNote(_T("")),m_strError(_T("")),m_iRelType(REL_TYPE_NO),mdlType(PRO_MDL_UNUSED)
{
	InitCalSymbol();
	owner=solid;
	strRel.Trim();                               //清楚除空格
	Revbol=true;
	sprintf(m_strRel,"%s\0",strRel.MakeLower());    //变小写
	//对于组件的关系式需要添加后缀
	if(solid==NULL)
	{
		Revbol=false;
		return;
	}
	ProMdlTypeGet((ProMdl)solid,&mdlType);
	if (mdlType==PRO_MDL_ASSEMBLY)
	{
		bolNeedPostID=TRUE;
	}
	GetDimParaOfSolid(solid);
	//检查关系式的格式
	if (CheckRelSymbol(strRel.MakeLower(),m_strError) &&  CheckEquation(strRel.MakeLower(),m_strError))
	{
		DepartRel();
		if(!m_strError.IsEmpty()) Revbol=false;
	}
	else
	{
		Revbol=false;
	}
	//等式左侧只能有一项
	if (m_iRelType==REL_TYPE_EQUAL && m_leftRel.GetCount()>1)
	{
		m_strError+="等式左侧项太多,请重新设置";
		Revbol=false;
	}
	else if(m_iRelType==REL_TYPE_EQUAL && m_leftRel[0].item_type==0)
	{
		m_strError+="等式左侧没有尺寸和参数,请重新设置";
		Revbol=false;
	}
}
CRel::CRel(ProSolid solid,CString strRel,bool bolPostId/*决定是否要后缀*/,bool & Revbol /*判断是否成功*/)
:bolNeedPostID(FALSE),m_strNote(_T("")),m_strError(_T("")),m_iRelType(REL_TYPE_NO),mdlType(PRO_MDL_UNUSED)
{
	InitCalSymbol();
	owner=solid;
	strRel.Trim();                               //清楚除空格
	Revbol=true;
	sprintf(m_strRel,"%s\0",strRel.MakeLower());    //变小写
	if(solid==NULL)
	{
		Revbol=false;
		return;
	}
	//对于组件的关系式需要添加后缀,以及强制需要后缀
	ProMdlTypeGet((ProMdl)solid,(ProMdlType*)&mdlType);
	if (mdlType==PRO_MDL_ASSEMBLY || bolPostId)
	{
		bolNeedPostID=TRUE;
	}
	GetDimParaOfSolid(solid);
	//检查关系式的格式
	if (CheckRelSymbol(strRel.MakeLower(),m_strError) &&  CheckEquation(strRel.MakeLower(),m_strError))
	{
		DepartRel();
		if(!m_strError.IsEmpty()) Revbol=false;
		if(mdlType==PRO_MDL_PART) bolNeedPostID=FALSE;
	}
	else
	{
		Revbol=false;
	}
	if (m_iRelType==REL_TYPE_EQUAL && m_leftRel.GetCount()>1)
	{
		m_strError+="等式左侧项太多,请重新设置";
		Revbol=false;
	}
	else if(m_iRelType==REL_TYPE_EQUAL && m_leftRel[0].item_type==0)
	{
		m_strError+="等式左侧没有尺寸和参数,请重新设置";
		Revbol=false;
	}
}
CRel::~CRel(void)
{
}
/************************************************************************/
/* 拷贝构造函数                                                         */
/************************************************************************/
CRel::CRel(const CRel & RightSides)
{
	owner=RightSides.owner;
	m_strNote=RightSides.m_strNote;
	m_aryRel.Copy(RightSides.m_aryRel);
	m_iRelType=RightSides.m_iRelType;
	m_aryDimParaItem.Copy(RightSides.m_aryDimParaItem);
	m_leftRel.Copy(RightSides.m_leftRel);
	m_RightRel.Copy(RightSides.m_RightRel);
}
bool CRel::operator==(const CRel & RightSide) const
{ 
	if (owner!=RightSide.owner)
	{
		return false;
	}
	if (m_aryRel.GetCount()==RightSide.m_aryRel.GetCount())
	{
		for (int i=0;i<m_aryRel.GetCount();i++)
		{
			if (!(m_aryRel[i].item_type==RightSide.m_aryRel[i].item_type 
				&& m_aryRel[i].symbol ==RightSide.m_aryRel[i].symbol
				&& m_aryRel[i].dim.type  ==RightSide.m_aryRel[i].dim.type
				&& m_aryRel[i].dim.id  ==RightSide.m_aryRel[i].dim.id
				&& m_aryRel[i].dim.owner  ==RightSide.m_aryRel[i].dim.owner
				&& m_aryRel[i].para.type  ==RightSide.m_aryRel[i].para.type
				&& m_aryRel[i].para.id  ==RightSide.m_aryRel[i].para.id
				&& m_aryRel[i].para.owner.type  ==RightSide.m_aryRel[i].para.owner.type
				&& m_aryRel[i].para.owner.who.model  ==RightSide.m_aryRel[i].para.owner.who.model
				&& m_aryRel[i].para.owner.who.item.type  ==RightSide.m_aryRel[i].para.owner.who.item.type
				&& m_aryRel[i].para.owner.who.item.id  ==RightSide.m_aryRel[i].para.owner.who.item.id
				&& m_aryRel[i].para.owner.who.item.owner  ==RightSide.m_aryRel[i].para.owner.who.item.owner))
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}
CRel & CRel::operator=(const CRel & RightSides)
{
	//TODO: return statement
	owner=RightSides.owner;
	m_strNote=RightSides.m_strNote;
	m_aryRel.Copy(RightSides.m_aryRel);
	m_iRelType=RightSides.m_iRelType;
	m_aryDimParaItem.Copy(RightSides.m_aryDimParaItem);
	m_leftRel.Copy(RightSides.m_leftRel);
	m_RightRel.Copy(RightSides.m_RightRel);
	return *this;
}
/************************************************************************/
/* 添加支持的函数                                                       */
/************************************************************************/
void CRel::InitCalSymbol()
{
	m_calSymbol.RemoveAll();
	m_calSymbol.Add("abs(");
	m_calSymbol.Add("acos(");
	m_calSymbol.Add("asin(");
	m_calSymbol.Add("atan(");
	m_calSymbol.Add("acot(");
	m_calSymbol.Add("cos(");
	m_calSymbol.Add("cosh(");
	m_calSymbol.Add("cot(");
	m_calSymbol.Add("exp(");
	m_calSymbol.Add("floor(");
	m_calSymbol.Add("mod(");
	m_calSymbol.Add("ln(");
	m_calSymbol.Add("log(");
	m_calSymbol.Add("pow(");
	m_calSymbol.Add("sin(");
	m_calSymbol.Add("sinh(");
	m_calSymbol.Add("sqrt(");
	m_calSymbol.Add("tan(");
	m_calSymbol.Add("tanh(");
}
/************************************************************************/
/* 获得关系式                                                           */
/************************************************************************/
BOOL CRel::GetRelString(CString & str)
{
	//double  value=0.0;
	//ProParamvalue proval;
	//for (int i=0;i<m_aryRel.GetCount();i++)
	//{
	//	switch(m_aryRel[i].item_type) {
	//	case 0:
	//		str+=m_aryRel[i].symbol;
	//		break;
	//	case 1:
	//		if(ProDimensionValueGet(&(m_aryRel[i].dim),&value)!=PRO_TK_NO_ERROR)
	//		{
	//			return FALSE;
	//		}
	//		str.Format("%s%f",str,value );
	//		break;
	//	case 2:
	//		if(ProParameterValueGet(&(m_aryRel[i].para),&proval)!=PRO_TK_NO_ERROR)
	//		{
	//			return FALSE;
	//		}
	//		str.Format("%s%f",str,(proval.type==PRO_PARAM_INTEGER?proval.value.i_val:proval.value.d_val));
	//		break;
	//	default:
	//		break;
	//		return FALSE;
	//	}
	//}

	ProName  name;
	int      postId;
	ProSolid para_owner;
	m_strError.Empty();
	str.Empty();
	for (int i=0;i<m_aryRel.GetCount();i++)
	{
		switch(m_aryRel[i].item_type) {
		case 0:
			break;
		case 1:
			if (!IsSolid1InSolid2((ProSolid)m_aryRel[i].dim.owner ,owner))
			{
				m_strError="模型已经不是组件的成员";
				return FALSE;
			}
			if(ProDimensionSymbolGet(&(m_aryRel[i].dim),name)!=PRO_TK_NO_ERROR)
			{
				m_strError.Format("%s尺寸\"%s\"不存在",m_strError,m_aryRel[i].symbol );
				return FALSE;
			}
			if(bolNeedPostID==TRUE && ProSolidToPostfixId((ProSolid)m_aryRel[i].dim.owner ,&postId)!=PRO_TK_NO_ERROR)
			{
				m_strError.Format("%s尺寸\"%s\"不存在",m_strError,m_aryRel[i].symbol );
				return FALSE;
			}
			if (bolNeedPostID==TRUE)
			{
				m_aryRel[i].symbol.Format("%s:%d",CString(name),postId);
			}
			else 
				m_aryRel[i].symbol.Format("%s",CString(name));
			break;
		case 2:
			para_owner=(ProSolid)(m_aryRel[i].para.owner.type==PRM_MODEL?m_aryRel[i].para.owner.who.model:m_aryRel[i].para.owner.who.item.owner);
			if (!IsSolid1InSolid2((ProSolid)para_owner ,owner))
			{
				m_strError="模型已经不是组件的成员";
				return FALSE;
			}
			if(bolNeedPostID==TRUE && ProSolidToPostfixId(para_owner ,&postId)!=PRO_TK_NO_ERROR)
			{
				m_strError.Format("%s参数\"%s\"不存在",m_strError,m_aryRel[i].symbol );
				return FALSE;
			}
			if (bolNeedPostID==TRUE)
				m_aryRel[i].symbol.Format("%s:%d",CString(m_aryRel[i].para.id),postId);
		    else
				m_aryRel[i].symbol.Format("%s",CString(m_aryRel[i].para.id));
			break;
		default:
			break;
			return FALSE;
		}
		str+=m_aryRel[i].symbol;
	}
	return TRUE;
}
/************************************************************************/
/* 获得一个有后缀的关系                                                 */
/************************************************************************/
BOOL  CRel::GetRelStringAsComp(CString & str)
{
	bolNeedPostID=TRUE;
    if (!GetRelString(str))
    {
		if (mdlType!=PRO_MDL_ASSEMBLY) bolNeedPostID=FALSE;
		return FALSE;
    }
	if (mdlType!=PRO_MDL_ASSEMBLY) bolNeedPostID=FALSE;
	return TRUE;
}
/************************************************************************/
/* 获得关系的文本                                                       */
/************************************************************************/
CString  CRel::GetRelNote(void)
{
	return m_strNote;
}
void  CRel::GetRightParamList(CArray<ProParameter,ProParameter&> * p_ParamList)        //获得右边参数（＝ < > 右边的参数）
{
	p_ParamList->RemoveAll();
	for (int i=0;i<m_RightRel.GetCount();i++)
	{
		if (m_RightRel[i].item_type ==2)
		{
			int Flag=0;
			for (int j=0;j<p_ParamList->GetCount();j++)
			{
				if (m_RightRel[i].para.type  ==p_ParamList->GetAt(j).type
					&& m_RightRel[i].para.id  ==p_ParamList->GetAt(j).id
					&& m_RightRel[i].para.owner.type  ==p_ParamList->GetAt(j).owner.type
					&& m_RightRel[i].para.owner.who.model  ==p_ParamList->GetAt(j).owner.who.model
					&& m_RightRel[i].para.owner.who.item.type  ==p_ParamList->GetAt(j).owner.who.item.type
					&& m_RightRel[i].para.owner.who.item.id  ==p_ParamList->GetAt(j).owner.who.item.id
					&& m_RightRel[i].para.owner.who.item.owner  ==p_ParamList->GetAt(j).owner.who.item.owner)
				{
					Flag=1;
					break;
				}
			}
			if(Flag==0) p_ParamList->Add(m_RightRel[i].para);
		}
	}
}
void  CRel::GetRightDimList(CArray<ProDimension,ProDimension&> * p_DimList)         //获得右边尺寸的列表
{
	p_DimList->RemoveAll();
	for (int i=0;i<m_RightRel.GetCount();i++)
	{
		if (m_RightRel[i].item_type==1)
		{
			int Flag=0;
			for (int j=0;j<p_DimList->GetCount();j++)
			{
				if (m_RightRel[i].dim.type  ==p_DimList->GetAt(j).type
					&& m_RightRel[i].dim.id  ==p_DimList->GetAt(j).id
					&& m_RightRel[i].dim.owner  ==p_DimList->GetAt(j).owner)
				{
					Flag=1;
					break;
				}
			}
			if(Flag==0) p_DimList->Add(m_RightRel[i].dim);
		}
	}
}
void  CRel::GetLeftParamList(CArray<ProParameter,ProParameter&> * p_ParamList)         //获得左边的参数列表
{
	p_ParamList->RemoveAll();
	for (int i=0;i<m_leftRel.GetCount();i++)
	{
		if (m_leftRel[i].item_type==2)
		{
			int Flag=0;
			for (int j=0;j<p_ParamList->GetCount();j++)
			{
				if (m_leftRel[i].para.type  ==p_ParamList->GetAt(j).type
					&& m_leftRel[i].para.id  ==p_ParamList->GetAt(j).id
					&& m_leftRel[i].para.owner.type  ==p_ParamList->GetAt(j).owner.type
					&& m_leftRel[i].para.owner.who.model  ==p_ParamList->GetAt(j).owner.who.model
					&& m_leftRel[i].para.owner.who.item.type  ==p_ParamList->GetAt(j).owner.who.item.type
					&& m_leftRel[i].para.owner.who.item.id  ==p_ParamList->GetAt(j).owner.who.item.id
					&& m_leftRel[i].para.owner.who.item.owner  ==p_ParamList->GetAt(j).owner.who.item.owner)
				{
					Flag=1;
					break;
				}
			}
			if(Flag==0) p_ParamList->Add(m_leftRel[i].para);
		}
	}
}
void  CRel::GetLeftDimList(CArray<ProDimension,ProDimension&> * p_DimList)           //获得左边尺寸（= < > 左边的参数）
{
	p_DimList->RemoveAll();
	for (int i=0;i<m_leftRel.GetCount();i++)
	{
		if (m_leftRel[i].item_type==1)
		{
			int Flag=0;
			for (int j=0;j<p_DimList->GetCount();j++)
			{
				if (m_leftRel[i].dim.type  ==p_DimList->GetAt(j).type
					&& m_leftRel[i].dim.id  ==p_DimList->GetAt(j).id
					&& m_leftRel[i].dim.owner  ==p_DimList->GetAt(j).owner)
				{
					Flag=1;
					break;
				}
			}
			if(Flag==0) p_DimList->Add(m_leftRel[i].dim);
		}
	}
}
void  CRel::GetRelAllParamList(CArray<ProParameter,ProParameter&> * p_ParamList)
{
	p_ParamList->RemoveAll();
	for (int i=0;i<m_aryRel.GetCount();i++)
	{
		if (m_aryRel[i].item_type==2)
		{
			int Flag=0;
			for (int j=0;j<p_ParamList->GetCount();j++)
			{
				if (m_aryRel[i].para.type  ==p_ParamList->GetAt(j).type
					&& m_aryRel[i].para.id  ==p_ParamList->GetAt(j).id
					&& m_aryRel[i].para.owner.type  ==p_ParamList->GetAt(j).owner.type
					&& m_aryRel[i].para.owner.who.model  ==p_ParamList->GetAt(j).owner.who.model
					&& m_aryRel[i].para.owner.who.item.type  ==p_ParamList->GetAt(j).owner.who.item.type
					&& m_aryRel[i].para.owner.who.item.id  ==p_ParamList->GetAt(j).owner.who.item.id
					&& m_aryRel[i].para.owner.who.item.owner  ==p_ParamList->GetAt(j).owner.who.item.owner)
				{
					Flag=1;
					break;
				}
			}
			if(Flag==0) p_ParamList->Add(m_aryRel[i].para);
		}
	}
}
void  CRel::GetRelAllDimList(CArray<ProDimension,ProDimension&> * p_DimList)
{
	p_DimList->RemoveAll();
	for (int i=0;i<m_aryRel.GetCount();i++)
	{
		if (m_aryRel[i].item_type==1)
		{
			int Flag=0;
			for (int j=0;j<p_DimList->GetCount();j++)
			{
				if (m_aryRel[i].dim.type  ==p_DimList->GetAt(j).type
					&& m_aryRel[i].dim.id  ==p_DimList->GetAt(j).id
					&& m_aryRel[i].dim.owner  ==p_DimList->GetAt(j).owner)
				{
					Flag=1;
					break;
				}
			}
			if(Flag==0) p_DimList->Add(m_aryRel[i].dim);
		}
	}
}
/************************************************************************/
/* 设置关系式右边参数的值                                               */
/************************************************************************/
BOOL  CRel::SetRightParaValue()
{
	return FALSE;
}
BOOL  CRel::Updata(void)
{
	CString str;
	if(!GetRelString(str))
	{
		return FALSE;
	}
	sprintf(m_strRel,"%s",str.Trim().MakeLower());    //变小写;
	return FALSE;
}
/************************************************************************/
/* 检查关系式                                                           */
/************************************************************************/
BOOL  CRel::CheckRel(void)
{
	return FALSE;
}
/************************************************************************/
/* 检查关系式                                                           */
/************************************************************************/
BOOL  CRel::CheckRel(CString strRel)
{
	if(strRel.Trim().GetLength()>MAX)
	{
		AfxMessageBox("关系式太长");
		return FALSE;
	}
	sprintf(m_strRel,"%s",strRel.MakeLower());	
	return Updata();
}
/************************************************************************/
/* 分离关系式左右两边，确定关系的类型                                   */
/************************************************************************/
void  CRel::DepartRel(void)
{
	char     str_left[MAX];
	char     str_right[MAX];
	char     str_mid[MAX];
	rel_item mid;
	int      j=0,z=0,m=0,temp_switch=0;
	m_strError.Empty();
    m_leftRel.RemoveAll();
	m_RightRel.RemoveAll();
	m_aryRel.RemoveAll();
	for (int i=0;m_strRel[i]!='\0',i<=MAX;i++)
	{
		if(m_strRel[i]=='=' || m_strRel[i]=='>' || m_strRel[i]=='<')
		{
			str_mid[m++]=m_strRel[i];
			str_mid[m]='\0';
			temp_switch=1;
			continue;
		}
		if (temp_switch==0)
		{
			str_left[j++]=m_strRel[i];
			str_left[j]='\0';
		}
		if (temp_switch==1)
		{
			str_right[z++]=m_strRel[i];
			str_right[z]='\0';
		}
	}
	CheckRelPart(str_right,&m_RightRel);
	CheckRelPart(str_left,&m_leftRel);
	mid.symbol=str_mid;
	mid.item_type=0;
	m_aryRel.Append(m_leftRel);
	m_aryRel.Add(mid);
	m_aryRel.Append(m_RightRel);
	//设置关系式的类型
	if (mid.symbol =="=")
		m_iRelType=REL_TYPE_EQUAL;
	else if (mid.symbol==">=")
		m_iRelType=REL_TYPE_MORE_EQUAL;
	else if (mid.symbol=="<=")
		m_iRelType=REL_TYPE_LESS_EQUAL;
	else if (mid.symbol==">")
		m_iRelType=REL_TYPE_MORE;
	else if (mid.symbol=="<")
		m_iRelType=REL_TYPE_LESS;
	else if (mid.symbol=="<>")
		m_iRelType=REL_TYPE_UN_EQUAL;
	else m_iRelType=REL_TYPE_NO;
}
/************************************************************************/
/* 反问模型的所有参数和尺寸                                             */
/************************************************************************/
void CRel::GetDimParaOfSolid(ProSolid solid)
{
	CArray<ProFeature,ProFeature> p_FeatList;
	ProError                      status;
	int                           num=0;
	ProFeattype                   ftype;
	ProSolid                      mdl;
	int                           postId;
	ProModelitem                  modelitem;
	CArray<ProParameter,ProParameter> Parameterlist;
	CArray<ProDimension,ProDimension> p_DimentionList;
	status=ProSolidFeatVisit(solid,(ProFeatureVisitAction)FeatureAsmVisitAcFn
		,NULL,(CArray<ProFeature,ProFeature>*)&p_FeatList);
	if (status!=PRO_TK_NO_ERROR) return;
	num=(int)p_FeatList.GetCount();

	for (int i=0;i<num;i++) {
		status = ProFeatureTypeGet (&p_FeatList[i], &ftype);
		if (ftype==PRO_FEAT_COMPONENT) {
			ProAsmcompMdlGet((ProAsmcomp *)(&p_FeatList[i]),(ProMdl *)&mdl);
			GetDimParaOfSolid(mdl);
		}
		//else
		//{
			status=ProFeatureDimensionVisit(&p_FeatList[i],(ProDimensionVisitAction)partFeatDimVisAc ,
				NULL,(CArray<ProDimension,ProDimension> *)&p_DimentionList);
		//}
	}

	status=ProSolidToPostfixId(solid,&postId);
	num=(int)p_DimentionList.GetCount();
	for (int i=0;i<num;i++) {
		dimpara_item  temp_item;
		ProName symbol;		
		ProDimensionSymbolGet(&p_DimentionList[i],symbol);
		if(bolNeedPostID)
		{
			temp_item.str.Format("%s:%d",CString(symbol),postId);
		}
		else
		{
			temp_item.str.Format("%s",CString(symbol));
		}
		temp_item.str.MakeLower();
		temp_item.type=TYPE_DIM;
		temp_item.dim=p_DimentionList[i];
		m_aryDimParaItem.Add(temp_item);
	}
	//再访问模型中的参数,参数的类型必须为整型或浮点型
	ProMdlToModelitem(solid,&modelitem);
	ProParameterVisit((ProModelitem*)&modelitem,NULL,(ProParameterAction)partAsmParaVisAc,
		(CArray<ProParameter,ProParameter>*)&Parameterlist);
	num=(int)Parameterlist.GetCount();
	for (int i=0;i<num;i++) {
		ProParamvalueType r_type;
		ProParamvalue proval;
		ProParameterValueGet(&Parameterlist[i],&proval);
		ProParamvalueTypeGet(&proval,&r_type);
		if(r_type==PRO_PARAM_DOUBLE||r_type==PRO_PARAM_INTEGER)
		{
			dimpara_item  temp_item;
			if(bolNeedPostID)
			{
				temp_item.str.Format("%s:%d",CString(Parameterlist[i].id),postId);
			}
			else
			{
				temp_item.str.Format("%s",CString(Parameterlist[i].id));
			}
			temp_item.str.MakeLower();
			temp_item.type=TYPE_PARA;
			temp_item.para=Parameterlist[i];
			m_aryDimParaItem.Add(temp_item);
		}
	}
}
BOOL  CRel::Calculate(CString strFormula,double * dblReValue)
{
	m_strError.Empty();
	mathStr.Initial(strFormula.GetBuffer());
	if (!mathStr.checkString())
	{
		(*dblReValue)=0.0;
		m_strError+=_T("表达式有误");
		return FALSE;          //表达式有误
	}
	mathStr.stringToBolan();
	if (mathStr.compvalue()!=0)
	{
		(*dblReValue)=0.0;
		m_strError+=_T("计算中出错");
		return FALSE;         //计算中出错，根据返回值还可一进一步确定原因
	}
	(*dblReValue)=mathStr.getvalue(); 
	return TRUE;
}
/************************************************************************/
/* 对关系式进行分离（只有等式一边）                                     */
/************************************************************************/
int  CRel::CheckRelPart(char rel_str[MAX],CArray<rel_item,rel_item&> * temp_rel_item)
{
	//char st[MAX/2];                   //存放括号的栈
	int top=0,                        //栈顶
		i;
	CArray<int,int> i_ary;            //用于关系式的分离状态判断 1(第一步分离，即可以合并的) 0(没有分离) -1(最后分离出来，可以合并的)
	CArray<dimpara_item,dimpara_item&> temp_dimpara_item;  
	
	for(i=0;rel_str[i]!='\0';i++)
	{
		//AfxMessageBox(rel_str[i]);
		if (rel_str[i]=='(' || rel_str[i]=='[' || rel_str[i]=='{' 
			|| rel_str[i]==')' || rel_str[i]==']' || rel_str[i]=='}' 
			||rel_str[i]=='+' || rel_str[i]=='-' || rel_str[i]=='*' || rel_str[i]=='/')
		{
			i_ary.Add(1);
		}
		else i_ary.Add(0);
		//i_ary.Add(1);
		////如果是左括号，则入栈
		//if(rel_str[i]=='(' || rel_str[i]=='[' || rel_str[i]=='{')
		//{  //如果是左括号，则入栈
		//	top++;
		//	st[top]=rel_str[i];
		//	continue;
		//}

		////小括号
		//if(rel_str[i]==')')   //小括号
		//{  
		//	if(st[top]=='(')   //配对
		//	{
		//		top--;         //出栈
		//		continue;
		//	}
		//	else               //不配对，返回0
		//	{
		//		m_strError+=_T("小括号不配对\n");
		//		return 0;    
		//	}
		//}
		////中括号
		//if(rel_str[i]==']')    
		//{  
		//	if(st[top]=='[')   //配对
		//	{
		//		top--;         //出栈
		//		continue;
		//	}
		//	else 
		//	{
		//		m_strError+=_T("中括号不配对\n");
		//		return 0;    //不配对，返回0
		//	}
		//}
		////大括号
		//if(rel_str[i]=='}')    
		//{  
		//	if(st[top]=='{')   //配对
		//	{
		//		top--;         //出栈
		//		continue;
		//	}
		//	else 
		//	{
		//		m_strError+=_T("大括号不配对\n");
		//	    return 0;    //不配对，返回0
		//	}
		//}
		////运算符
		//if(rel_str[i]=='+' || rel_str[i]=='-' || rel_str[i]=='*' || rel_str[i]=='/')
		//{   //如果有连续运算符则错误
		//	if(rel_str[i+1]=='+' || rel_str[i+1]=='-' || rel_str[i+1]=='*' ||rel_str[i+1]=='/')
		//	{
		//		m_strError+=_T("存在连续的运算符\n");
		//		return 0;  //错误，返回0
		//	}
		//	else
		//		continue;  //无连续运算符情况
		//}
		////其它的都不分离
		//i_ary[i]=0;
	}
	//if(top!=0)       //括号不匹配,返回0
	//{
	//	m_strError+=_T("括号不匹配\n");
	//	return 0;
	//}
	    //测试语句
		//CString test;;
		//for (i=0;rel_str[i]!='\0';i++)
		//{
		//	test.Format("%s%c    %d\n",test,rel_str[i],i_ary[i]);
		//}
		//AfxMessageBox(test);
		//test.Empty();



	for(i=0;rel_str[i]!='\0';i++)
	{
		if((rel_str[i]=='[')||(rel_str[i]=='{'))
			rel_str[i]='(';
		else if((rel_str[i]==']')||(rel_str[i]=='}'))
			rel_str[i]=')';
	}
	//分离除函数
	for (i=0;rel_str[i]!='\0';i++)
	{
		if(i_ary[i]==1) continue;
		if (rel_str[i]=='a' || rel_str[i]=='c' || rel_str[i]=='e' || rel_str[i]=='f' 
			|| rel_str[i]=='m' || rel_str[i]=='l' || rel_str[i]=='p' || rel_str[i]=='s' || rel_str[i]=='t')
		{
			for (int j=0;j<m_calSymbol.GetCount();j++ )
			{
				int m=i;
				//如果跟函数相符，且前面是运算符号的，则为函数
				if(mathStr.seekStr(rel_str,i,m_calSymbol[j].GetBuffer()) && 
					(rel_str[m-1]=='+' || rel_str[m-1]=='-' || rel_str[m-1]=='*' || rel_str[m-1]=='/' || rel_str[m-1]=='='))
				{
					i-=1;
					for(m;m<=i;m++) i_ary[m]=1;
					if(rel_str[i+1]==')')
					{
						m_strError+=_T("函数"+m_calSymbol[j]+")括号内不能为空\n");
						return 0;
					}
					break;
				}
			}
		}
	}
	//for (i=0;rel_str[i]!='\0';i++)
	//{
	//	test.Format("%s%c    %d\n",test,rel_str[i],i_ary[i]);
	//}
	//AfxMessageBox(test);
	//test.Empty();
	//分离除参数与数字
	for (i=0;rel_str[i]!='\0';i++)
	{
		if(i_ary[i]==1) continue;
		//得到运算符和函数之间的字符
		char str[MAX];
		int  c=0;
		int  n,m;
		n=m=i;
		for (m;m<i_ary.GetCount();m++)
		{
			if (i_ary[m]==0)
			{
				str[c++]=rel_str[m];
				i_ary[m]=-1;
			}
			else
			{
				str[c]='\0';
				i=m-1;
				break;
			}
		}
		//对于最后一个，
		if(m==i_ary.GetCount()) str[c]='\0',i=m-1;
		//与参数进行比较
		int Flag=0;
		for (int j=0;j<m_aryDimParaItem.GetCount();j++)
		{
			if (CString(str)==m_aryDimParaItem[j].str)
			{
				temp_dimpara_item.Add(m_aryDimParaItem[j]);
				Flag=1;
				break;
			}
		}
		if(Flag==0)
		{
			if (!mathStr.IsDouble(str))
			{
				m_strError+=_T("无法找到:"+CString(str)+"(该项可能不是整型或实数的参数)\n");
				return 0;
			}
			for (n;n<=i;n++)
			{
				i_ary[n]=1;
			}
		}
	}
	//for (i=0;rel_str[i]!='\0';i++)
	//{
	//	test.Format("%s%c    %d\n",test,rel_str[i],i_ary[i]);
	//}
	//AfxMessageBox(test);
	//test.Empty();

	//检查是否还有没有分离的
	for (i=0;rel_str[i]!='\0';i++)
	{
		if (i_ary[i]==0)
		{
			char temp[MAX];
			int  m=0;
			for (m;m<MAX,i_ary[m+i]==0;m++)
			{
				temp[m]=rel_str[i+m];
			}
			temp[m]='\0';
			m_strError+=_T("无法找到:"+CString(temp)+'\n');
			return 0;
		}
	}
	//for (i=0;rel_str[i]!='\0';i++)
	//{
	//	test.Format("%s%c    %d\n",test,rel_str[i],i_ary[i]);
	//}
	//AfxMessageBox(test);
	//test.Empty();
	//把所有分离出来的项添加到关系项里面
	for (i=0;i<i_ary.GetCount();i++)
	{
		int  m=i;
		if (i_ary[i]==1)
		{
			rel_item temp_item;
			temp_item.item_type=0;			
			do {
				temp_item.symbol+=rel_str[m++];
				if(m==i_ary.GetCount()) break;
			} while(i_ary[m]==1);
			temp_rel_item->Add(temp_item);
			i=m-1;
			continue;
		}
		else if (i_ary[i]==-1)
		{
			rel_item temp_item;			
			do {
				temp_item.symbol+=rel_str[m++];
				if(m==i_ary.GetCount()) break;
			} while(i_ary[m]==-1);
			if(temp_item.symbol==temp_dimpara_item[0].str)
			{
				temp_item.item_type=(temp_dimpara_item[0].type==TYPE_PARA?2:1);
				temp_item.dim=temp_dimpara_item[0].dim;
				temp_item.para=temp_dimpara_item[0].para;
				temp_rel_item->Add(temp_item);
				temp_dimpara_item.RemoveAt(0,1);
				temp_dimpara_item.FreeExtra();
				i=m-1;
			}
			else
			{
				m_strError+=_T("无法找到:"+temp_item.symbol+'\n');
				return 0;
			}
			continue;
		}
		else if (i_ary[i]==0)
		{
			m_strError+=_T("出错");
			return 0;
		}
	}
	return 1;	
}

/************************************************************************/
/* 对关系式的符号进行检查,即必须符合一定的格式才行                      */
/************************************************************************/
bool CRel::CheckRelSymbol(CString rel,CString & error)
{	

	//检查＝ > <号
	//内部函数
	rel.Trim();
	if((GetSymbolNum(rel,"=")+GetSymbolNum(rel,">")+GetSymbolNum(rel,"<"))==0) 
	{error.AppendFormat("关系式没有比较运算符号(=,<,>)\n");return false;}
    if(GetSymbolNum(rel,"=")>1  || GetSymbolNum(rel,">")>1 || GetSymbolNum(rel,"<")>1) 
		{error.AppendFormat("关系式中存在多个重复的比较运算符号(=,<,>)\n");return false;}
    if((GetSymbolNum(rel,"=")+GetSymbolNum(rel,"<=")+GetSymbolNum(rel,"<=") + GetSymbolNum(rel,"<>"))!=1 ) 
		{error.AppendFormat("关系式中的比较运算符不正确，检查是否存在或者是否有多个,或者格式是否正确\n(等与(=),大于等于(>=),小于等于(<=))\n");return false;}
    if(!error.IsEmpty()) return false;
    //检查大小括号是否匹配
	char    rel_str[MAX];
	char    st[MAX/2];
	int     top=0;
	int     sym=0;
	int     i=0;
	sprintf(rel_str,"%s",rel.GetBuffer());
	for(i=0;rel_str[i]!='\0';i++)
	{
		//如果是左括号，则入栈
		if(rel_str[i]=='(' || rel_str[i]=='[' || rel_str[i]=='{')
		{  //如果是左括号，则入栈
			top++;
			st[top]=rel_str[i];
			continue;
		}

		//小括号
		if(rel_str[i]==')')   //小括号
		{  
			if(st[top]=='(')   //配对
			{
				top--;         //出栈
				continue;
			}
			else               //不配对，返回0
			{
				error.AppendFormat("小括号不配对\n");
				return false;    
			}
		}
		//中括号
		if(rel_str[i]==']')    
		{  
			if(st[top]=='[')   //配对
			{
				top--;         //出栈
				continue;
			}
			else 
			{
				error.AppendFormat("中括号不配对\n");
				return false;    //不配对，返回0
			}
		}
		//大括号
		if(rel_str[i]=='}')    
		{  
			if(st[top]=='{')   //配对
			{
				top--;         //出栈
				continue;
			}
			else 
			{
				error.AppendFormat("大括号不配对\n");
				return false;    //不配对，返回0
			}
		}
		//
		if (rel_str[i]=='=' || rel_str[i]=='>' || rel_str[i]=='<')
		{
			top++;
			sym++;
			st[top]=rel_str[i];
		}
		//运算符
		if(rel_str[i]=='+' || rel_str[i]=='-' || rel_str[i]=='*' || rel_str[i]=='/' || rel_str[i]=='=' || rel_str[i]=='>' || rel_str[i]=='<' || rel_str[i]=='.') 
		{   //如果有连续运算符则错误
			if(rel_str[i+1]=='+' || rel_str[i+1]=='-' || rel_str[i+1]=='*' ||rel_str[i+1]=='/'|| rel_str[i+1]=='=' || rel_str[i+1]=='>' || rel_str[i+1]=='<' || rel_str[i+1]=='.')
			{
				error.Format("%s有连续运算符:%c%c\n",error,rel_str[i],rel_str[i+1]);
				return false;  //错误，返回0
			}
			else
				continue;    //无连续运算符情况
		}
	}
	//判断除了＋><之外括号是否配对
	if ((top-sym)!=0)
	{
		error.AppendFormat("括号不配对,请检查\n");
		return false;
	}
	//检查关系首尾是否为运算符

	if (rel_str[0]=='+' || rel_str[0]=='-' || rel_str[0]=='*' || rel_str[0]=='/' || rel_str[0]=='=' || rel_str[0]=='>' || rel_str[0]=='<' || rel_str[0]=='.')
		error.Format("%s关系式开头不能为:%c",error,rel_str[0]);
	i=rel.GetLength()-1;
	if(rel_str[i]=='+' || rel_str[i]=='-' || rel_str[i]=='*' || rel_str[i]=='/' || rel_str[i]=='=' || rel_str[i]=='>' || rel_str[i]=='<' || rel_str[i]=='.')
		error.Format("%s关系式末尾不能为:%c",error,rel_str[i]);
	return (error.IsEmpty()?true:false);
}
int CRel::GetSymbolNum(CString m_str,char * m_sym)
{
	int m_num=0;
	int m_start=0;
	do {
		m_start=m_str.Find(m_sym,m_start);
		if(m_start!=-1) m_num++,m_start++;
	} while(m_start!=-1);
	return m_num;
}

// MathString.cpp: implementation of the CMathString class.
//
////////////////////////////////////////////////////////////////////
// MathString.cpp: implementation of the CMathString class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////                    
//
//                   求算术表达式值的类
//
//              湖南城市学院  信息与计算科学系  
//                        黄江峰
//                 jiangfenghuang@msn.com
//                 jiangfenghuang@163.net
//                       2003.4.5           
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
// 功能：
//      求在字符串中的算术表达式的值，
//   
// 特点：
//      1.支持字符串中含有各种常用函数，如"7.5+sin(6*ln(8))/exp(5)"
//      2.具有很好的纠错能力，能检查出表达式中括号是否配对、库函数是
//        否正确
//      3.运算过程中能检查出各种异常，如除数为0、开方函数sqrt(x)中x<0
//        反余弦函数acos(x)中的x<-1或x>1等
//      4.为支持积分、求方程，算术表达式中可含有x，计算时将用类中的成
//        员变量xx代替 
//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////// 
//
// 使用方法：
//      1. 定义一个CMathString 的对象，用要求的算术表达式将其初始化
//         如： char *s1="7.5+sin(6)/exp(5)";
//              char *s2="4*pow(x,2)+2*x+4";
//              CMathString  mathstr1(s1);
//              CMathString  mathstr2(s2);
//        或调用Initial(char *s)、Initial(char *s,double x)将其初始化
//      2.调用checkString方法检查表达式是否合法
//      3.如果是求积分或j解方程，调用setX(double x)设置表达式中x的值
//      4.调用stringToBolan()将算术表达式转换为波兰表达式
//      5.调用compvalue()对波兰表达式进行计算，若返回值为0，则调
//        用getvalue()取得计算结果，否则可根据返回值判断出错地方
//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//
//                支持的函数的编号及功能
//
// 编号     函数         功能         调用的math.h库函数     备注
//
//   1      abs(x)      取绝对值|x|    fabs(x)      
//   2      acos(x)     反余弦函数     acos(x)            -1<=x<=1
//   3      asin(x)     反正弦函数     asin(x)            -1<=x<=1
//   4      atan(x)     反正切函数     atan(x)
//   5      acot(x)     反余切函数     PI/2-atan(x)
//   6      cos(x)      余弦函数       cos(x)
//   7      cosh(x)     双曲余弦函数   cosh(x)
//   8      cot(x)      余切函数       1/tan(x)            tan(x)!=0
//   9      exp(x)      e的x次方       exp(x)
//  10      floor(x)    ⊥x⊥          floor(x)   求不大于x的最大整数 
//  11      mod(x,y)     x%y           mod(x,y)             y!=0
//  12      ln(x)       取自然对数     log(x)               x>0
//  13      log(x)      取10的对数     log10(x)             x>0
//  14      pow(x,y)    x的y次方       pow(x,y)
//  15      sin(x)      正弦函数       sin(x)
//  16      sinh(x)     双曲正弦函数   sinh(x)
//  17      sqrt(x)     对x开方        sqrt(x)            x>=0
//  18      tan(x)      正切函数       tan(x)
//  19      tanh(x)     双曲正切函数   tanh(x)
//
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Rel.h"
#include "math.h"

#define PI  3.1415926

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMathString::CMathString()
{

}

CMathString::~CMathString()
{

}


CMathString::CMathString(char *str)  //构造函数，将算术表达式str赋给String
{
	strcpy(string,str);            
}

CMathString::CMathString(char *str,double x)
{    //表达式中带未知数的构造函数
	//算术表达式*s赋给string，未知数x赋给xx
	//用于积分表达式
	strcpy(string,str);
	xx=x;
}

void CMathString::setX(double x)      //设置成员变量xx的值
{
	xx=x;
}

int CMathString::seekStr(char *str,int &i, char *s)
{   //检查字符窜str的第i个位置开始的后面字符是否与字符窜s相匹配
	//是的话将i=i+len(s),并返回1，否则返回0
	int j=0;
	for(j=0;j<(int)strlen(s);j++)
		if(str[i+j]!=s[j])
			break;
	if(j==strlen(s))
	{
		i=i+(int)strlen(s);
		return 1;
	}
	else return 0;
}

void CMathString::Initial(char *s)  //初始化函数，将算术表达式s赋给String
{
	strcpy(string,s); 

}

void CMathString::Initial(char *str, double x)
{
	//表达式中带未知数的初始化函数
	//算术表达式*str赋给string，未知数x赋给xx
	//用于积分表达式
	strcpy(string,str);
	xx=x;

}

int CMathString::checkString()
{ //检查输入的表达式是否正确，包括括号是否配对、库函数是否正确，
	//不包括小数点是否正确，若正确，将表达式中的大、中括号该成小括号
	//并在表达式尾加‘#’标识
	//若正确，返回值为1；否则返回0；
	//因积分表达式中有x，所以表达式中的x作为字符处理
	char st[MAX/2];        //存放括号的栈
	int top=0,     //栈顶
		i;
	for(i=0;string[i]!='\0';i++)
	{
		//如果是左括号，则入栈
		if(string[i]=='(' || string[i]=='[' || string[i]=='{')
		{  //如果是左括号，则入栈
			top++;
			st[top]=string[i];
			continue;
		}

		//小括号
		if(string[i]==')')   //小括号
		{  
			if(st[top]=='(')   //配对
			{
				top--;         //出栈
				continue;
			}
			else return 0;    //不配对，返回0
		}
		//中括号
		if(string[i]==']')    
		{  
			if(st[top]=='[')   //配对
			{
				top--;         //出栈
				continue;
			}
			else return 0;    //不配对，返回0
		}
		//大括号
		if(string[i]=='}')    
		{  
			if(st[top]=='{')   //配对
			{
				top--;         //出栈
				continue;
			}
			else return 0;    //不配对，返回0
		}

		//运算符
		if(string[i]=='+' || string[i]=='-' || string[i]=='*' || string[i]=='/')
		{   //如果有连续运算符则错误
			if(string[i+1]=='+' || string[i+1]=='-' || string[i+1]=='*' ||string[i+1]=='/')
				return 0;  //错误，返回0
			else
				continue;  //无连续运算符情况
		}

		//数字、小数点、逗号
		if(('0'<=string[i] && string[i]<='9') || string[i]=='.'
			||string[i]==',')
			continue;

		//未知数x
		if(string[i]=='x')
			continue;

		//是以a开头的函数
		if(string[i]=='a')
		{
			if(seekStr(string,i,"abs")||seekStr(string,i,"acos")||
				seekStr(string,i,"asin")||seekStr(string,i,"atan")||
				seekStr(string,i,"acot"))//是以a开头的库函数
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //如果函数后是括号
					st[++top]=string[i];  // 括号入栈  
					continue;
				}
				else //如果函数后不是括号,返回0
					return 0;
			}
			else    //不是以a开头的库函数,返回0
				return 0;
		}

		//是以c开头的函数
		if(string[i]=='c')
		{
			if(seekStr(string,i,"cos")||seekStr(string,i,"cosh")||
				seekStr(string,i,"cot"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //如果函数后是括号
					st[++top]=string[i];  // 括号入栈  
					continue;
				}
				else //如果函数后不是括号,返回0
					return 0;
			}
			else    //不是以c开头的库函数,返回0
				return 0;
		}
		//是以e开头的函数
		if(string[i]=='e')
		{
			if(seekStr(string,i,"exp"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //如果函数后是括号
					st[++top]=string[i];  // 括号入栈  
					continue;
				}
				else //如果函数后不是括号,返回0
					return 0;
			}
			else    //不是以e开头的库函数,返回0
				return 0;
		}
		//是以f开头的函数
		if(string[i]=='f')
		{
			if(seekStr(string,i,"floor"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //如果函数后是括号
					st[++top]=string[i];  // 括号入栈  
					continue;
				}
				else //如果函数后不是括号,返回0
					return 0;
			}
			else    //不是以f开头的库函数,返回0
				return 0;
		}
		//是以m开头的函数
		if(string[i]=='m')
		{
			if(seekStr(string,i,"mod"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //如果函数后是括号
					st[++top]=string[i];  // 括号入栈  
					continue;
				}
				else //如果函数后不是括号,返回0
					return 0;
			}
			else    //不是以m开头的库函数,返回0
				return 0;
		}
		//是以l开头的函数
		if(string[i]=='l')
		{
			if(seekStr(string,i,"ln")||seekStr(string,i,"log"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //如果函数后是括号
					st[++top]=string[i];  // 括号入栈  
					continue;
				}
				else //如果函数后不是括号,返回0
					return 0;
			}
			else    //不是以l开头的库函数,返回0
				return 0;
		}
		//是以p开头的函数
		if(string[i]=='p')
		{
			if(seekStr(string,i,"pow"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //如果函数后是括号
					st[++top]=string[i];  // 括号入栈  
					continue;
				}
				else //如果函数后不是括号,返回0
					return 0;
			}
			else    //不是以p开头的库函数,返回0
				return 0;
		}
		//是以s开头的函数
		if(string[i]=='s')
		{
			if(seekStr(string,i,"sin")||seekStr(string,i,"sinh")||
				seekStr(string,i,"sqrt"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //如果函数后是括号
					st[++top]=string[i];  // 括号入栈  
					continue;
				}
				else //如果函数后不是括号,返回0
					return 0;
			}
			else    //不是以s开头的库函数,返回0
				return 0;
		}
		//是以t开头的函数
		if(string[i]=='t')
		{
			if(seekStr(string,i,"tan")||seekStr(string,i,"tanh"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //如果函数后是括号
					st[++top]=string[i];  // 括号入栈  
					continue;
				}
				else //如果函数后不是括号,返回0
					return 0;
			}
			else    //不是以t开头的库函数,返回0
				return 0;
		}

		else // 如果string[i]是其他字符,则返回0
			return 0;
	}//for(i=0;string[i]!='\0';i++)

	if(top!=0)       //括号不匹配,返回0
		return 0;

	//表达式正确
	//在表达式尾加‘#’标识
	string[i]='#';
	string[++i]='\0';

	//将表达式中的大、中括号该成小括号
	for(i=0;string[i]!='\0';i++)
	{
		if((string[i]=='[')||(string[i]=='{'))
			string[i]='(';
		else if((string[i]==']')||(string[i]=='}'))
			string[i]=')';
	}
	return 1;	
}

//将算术表达式string转化成波兰表达式，
//波兰表达式放在bolan[]中
//转换方法：
//1.判定string[i]为数值，直接存入bolan数组中
//2.判定string[i]为'(',将此'('压入栈,
//  若'('前为数字，则在'('前加一对'*'的处理
//3.判定string[i]为')',则将栈stact中左括号'('以后的运算符和函数依次
//  弹出,并入数组bolan中,然后将左括号'('弹出
//4.判定string[i]为'-','+',则将当前栈stack中左括号'('以后（如无'('则
//  将栈stack中所有）的运算符和函数依次弹出，存入数组bolan中,然后将
//  '-'或'+'压入栈stack中
//  如果'-'在第一个位置或其前是'('，即'-'是负号，将'~'（取负）压入stack中
//5.判定string[i]为'*'或'/',则将栈stact中顶端连续的'*'或'/'或函数依次
//  弹出，存入数组bolan中,然后将'*'或'/'压入栈stact中
//6.判定string[i]为函数，则将该函数入栈stact中
//  存入栈stact中变量func的是该函数的编号，详细编号见头文件
//  若该函数前为数字，则在该函数入栈stact前加一次对'*'的处理
//7.判定string[i]=',',将栈stact中'('后的运算符和函数全部弹出，压入数
//  组bolan中，
//8.判定string[i]='x',则用类中未知数变量xx代替，压入数组bolan中 
//9.转换结束时，若栈stact不为空，则将栈内所有运算符和函数弹出，存入
//  数组bolan中
//10.转换结束，在数组bolan尾加一opera='#'作为结束符
int CMathString::stringToBolan()
{
	Bolan stack[MAX];         //存放运算符和函数的栈
	int top=0,         //存放栈stact的下标
		i,           //string的下标
		j=0,           //bolan数组的下标
		t;
	char digitStr[50];     //存放要转化成数值的字符串

	for(i=0;string[i]!='#';i++)
	{
		//判定为数值，直接存入bolan数组中
		if(('0'<=string[i] && string[i]<='9')||string[i]=='.') //数值
		{
			t=0;
			digitStr[t]=string[i];   //数字存入要转化成数值的字符串
			while(('0'<=string[i+1] && string[i+1]<='9')
				||string[i+1]=='.')  
			{                       //将连续数字入要转化成数值的字符串
				digitStr[++t]=string[i+1];
				i++;
			}
			digitStr[++t]='\0';
			//将数字字符串转化成数值,并且入bolan数组
			bolan[j].flag=0;    //标识该节点为数值
			bolan[j].value=stringToDigital(digitStr);
			j++;
			continue;
		}

		//判定为'(',将此'('压入栈
		//若'('前为数字，则在'('前加一对'*'的处理
		if(string[i]=='(')
		{
			if(string[i-1]<='0' || '9'<=string[i-1])//'('前不为数字
			{
				top++;
				stack[top].flag=1;  //标识该节点为运算符
				stack[top].oper='(';
				continue;
			}
			else      //'('前为数字
			{         //加一次对'*'的处理
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//将'*'压入栈stact中
				top++;
				stack[top].flag=1;
				stack[top].oper='*';

				//将'('压入栈stact中
				top++;
				stack[top].flag=1;
				stack[top].oper='(';
				continue;
			}
		}

		//判定为')',则将栈stact中左括号'('以后的运算符和函数依次弹出
		//寸入数组bolan中,然后将左括号'('弹出
		if(string[i]==')')
		{
			while(stack[top].oper!='(')
			{
				bolan[j]=stack[top];
				j++;
				top--;
			}
			top--;  //将左括号'('弹出
			continue;
		}

		//判定为'-','+',则将当前栈stack中左括号'('以后（如无'('则将
		//栈stack中所有）的运算符和函数依次弹出，存入数组bolan中,
		//然后将'-'或'+'压入栈stack中
		//如果'-'在第一个位置或其前是'('，即'-'是负号，将'~'压入stack中
		if(string[i]=='+' || string[i]=='-')
		{
			if(string[i]=='-'&&(i==0 || string[i-1]=='(')) //'-'是负号
			{
				top++;
				stack[top].flag=1;
				stack[top].oper='~';
				continue;
			}
			else             //'-','+'是加减符号
			{
				while(stack[top].oper!='(' && top!=0)
				{
					bolan[j]=stack[top]; //当前栈stack中的运算符和
					//函数依次弹出存入数组bolan中
					j++;
					top--;
				}
				//将'-'或'+'压入栈stack中
				top++;
				stack[top].flag=1;
				stack[top].oper=string[i];
				continue;
			}
		}

		//判定string[i]为'*'或'/',则将栈stact中顶端连续的'*'或'/'或函
		//数依次弹出，存入数组bolan中,然后将'*'或'/'压入栈stact中
		if(string[i]=='*' || string[i]=='/')
		{
			while(stack[top].oper=='*' || stack[top].oper=='/'||
				stack[top].flag==2)
			{
				bolan[j]=stack[top];
				j++;
				top--;
			}
			//将'*'或'/'压入栈stact中
			top++;
			stack[top].flag=1;
			stack[top].oper=string[i];
			continue;
		}

		//判定string[i]为函数，则将该函数入栈stact中
		//存入栈stact中变量func的是该函数的编号，详细编号见头文件
		//若该函数前为数字，则在该函数入栈stact前加一次对'*'的处理
		if(string[i]=='a')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //该函数前为数字
			{              //在该函数入栈stact前加一次对'*'的处理
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//将'*'压入栈stact中
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}

			//为abs(x)函数
			if(seekStr(string,i,"abs"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=1;
				i--;                    //因为在seekStr函数中
				continue;               //i=i+strlen("abs");
				//i已指向函数abs的下一个字符
				//而在for语句中有i++,所以此处
				//i要减1
			}
			//为acos(x)函数
			if(seekStr(string,i,"acos"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=2;
				i--;                     
				continue;  
			}
			//为asin(x)函数
			if(seekStr(string,i,"asin"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=3;
				i--;                     
				continue;  
			}
			//为atan(x)函数
			if(seekStr(string,i,"atan"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=4;
				i--;                     
				continue;  
			}
			//为acot(x)函数
			if(seekStr(string,i,"acot"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=5;
				i--;                     
				continue;  
			}
		}//if(string[i]=='a')
		//string[i]为以c开头的库函数
		if(string[i]=='c')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //该函数前为数字
			{              //在该函数入栈stact前加一次对'*'的处理
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//将'*'压入栈stact中
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//为cos(x)函数
			if(seekStr(string,i,"cos"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=6;
				i--;                     
				continue;  
			}
			//为cosh(x)函数
			if(seekStr(string,i,"cosh"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=7;
				i--;                     
				continue;  
			}
			//为cot(x)函数
			if(seekStr(string,i,"cos"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=8;
				i--;                     
				continue;  
			}
		}//if(string[i]=='c')
		//string[i]为以e开头的库函数
		if(string[i]=='e')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //该函数前为数字
			{              //在该函数入栈stact前加一次对'*'的处理
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//将'*'压入栈stact中
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//为exp(x)函数
			if(seekStr(string,i,"exp"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=9;
				i--;                     
				continue;  
			}
		}//if(string[i]=='e')
		//string[i]为以f开头的库函数
		if(string[i]=='f')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //该函数前为数字
			{              //在该函数入栈stact前加一次对'*'的处理
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//将'*'压入栈stact中
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//为exp(x)函数
			if(seekStr(string,i,"floor"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=10;
				i--;                     
				continue;  
			}
		}//if(string[i]=='f')
		//string[i]为以m开头的库函数
		if(string[i]=='m')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //该函数前为数字
			{              //在该函数入栈stact前加一次对'*'的处理
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//将'*'压入栈stact中
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//为mod(x)函数
			if(seekStr(string,i,"mod"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=11;
				i--;                     
				continue;  
			}
		}//if(string[i]=='m')
		//string[i]为以l开头的库函数
		if(string[i]=='l')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //该函数前为数字
			{              //在该函数入栈stact前加一次对'*'的处理
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//将'*'压入栈stact中
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//为ln(x)函数
			if(seekStr(string,i,"ln"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=12;
				i--;                     
				continue;  
			}
			//为log(x)函数
			if(seekStr(string,i,"log"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=13;
				i--;                     
				continue;  
			}
		}//if(string[i]=='l')
		//string[i]为以p开头的库函数
		if(string[i]=='p')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //该函数前为数字
			{              //在该函数入栈stact前加一次对'*'的处理
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//将'*'压入栈stact中
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//为pow(x)函数
			if(seekStr(string,i,"pow"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=14;
				i--;                     
				continue;  
			}
		}//if(string[i]=='p')
		//string[i]为以s开头的库函数
		if(string[i]=='s')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //该函数前为数字
			{              //在该函数入栈stact前加一次对'*'的处理
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//将'*'压入栈stact中
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//为sin(x)函数
			if(seekStr(string,i,"sin"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=15;
				i--;                     
				continue;  
			}
			//为sinh(x)函数
			if(seekStr(string,i,"sinh"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=16;
				i--;                     
				continue;  
			}
			//为sin(x)函数
			if(seekStr(string,i,"sqrt"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=17;
				i--;                     
				continue;  
			}
		}//if(string[i]=='s')
		//string[i]为以t开头的库函数
		if(string[i]=='t')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //该函数前为数字
			{              //在该函数入栈stact前加一次对'*'的处理
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//将'*'压入栈stact中
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//为tan(x)函数
			if(seekStr(string,i,"tan"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=18;
				i--;                     
				continue;  
			}
			//为tanh(x)函数
			if(seekStr(string,i,"tanh"))   
			{                            //将该函数入栈stact中
				top++;
				stack[top].flag=2;
				stack[top].func=19;
				i--;                     
				continue;  
			}
		}//if(string[i]=='p')

		//判定string[i]=',',将栈stact中'('后的运算符和函数全部弹出，
		//压入数组bolan中，
		if(string[i]==',')
		{
			while(stack[top].oper!='(')
			{
				bolan[j]=stack[top];
				j++;
				top--;
			}
			continue;
		}

		//判定string[i]='x',则用类中未知数变量xx代替，压入数组bolan中 
		if(string[i]=='x')
		{
			bolan[j].flag=0;   //标识为数值
			bolan[j].value=xx;
			j++;
		}
	}

	//转换结束时，若栈stact不为空，则将栈内所有运算符和函数弹出，存入
	//数组bolan中
	while(top!=0)
	{
		bolan[j]=stack[top];
		j++;
		top--;
	}

	//转换结束，在数组bolan尾加一opera='#'作为结束符
	bolan[j].oper='#';

	return 1;
}

//将数字字符串转化成数值
double CMathString::stringToDigital(char *s)
{
	double sum=0,    //转化后的数值
		temp,
		ten=0.1;     //小数变化值
	int i,ch,
		flag=0;    //标识是小数还是整数，0为整数
	for(i=0;s[i]!='\0';i++)
	{
		if(s[i]!='.')
		{
			if(flag==0)  //整数部分
			{
				ch=s[i]-'0';
				sum=sum+ch;
				sum=sum*10;
			}
			else        //小数部分
			{
				ch=s[i]-'0';
				temp=ch*ten;   //小数点移位
				sum=sum+temp;
				ten=ten*0.1;    //改变小数点位置
			}
		}
		else     //小数点
		{
			sum=sum/10;
			flag=1;
		}
	}//for
	if(flag==0)    //无小数部分
		sum=sum/10;
	return sum;

}

//求波兰式bolan的值
//方法如下：
//1.若节点bolan[i]为数值，则如数据栈dataStact
//2.若节点bolan[i]为运算符，则从数据栈dataStact弹出数据进行计算，并
//  将结果压入数据栈dataStact中
//3.若节点bolan[i]为函数，则从数据栈dataStact弹出数据，调用相应的库
//  函数进行计算，并将结果压入数据栈dataStact中
//4.若节点bolan[i]为结束符'#',则数据栈dataStact中的数据弹出，赋给
//  result，并返回0
//返回值:
//1. 计算正确，返回0
//2. 在计算中若除数为0，返回1
//3. 反余弦函数acos(x)中的x不满足条件，返回2
//4. 反正弦函数asin(x)中的x不满足条件，返回3
//5. 余切函数cot(x) 中tan（x）为0，返回4
//6.取模x%y函数mod(x,y)中y为0，返回5
//7.自然对数函数ln(x),如果x<=0,则返回6
//8.取10的对数函数log10(x),如果x<=0,则返回7
//9.开方函数sqrt(x),如果x<0,则返回8
//10.计算中有其他错误，返回-1
int CMathString::compvalue(void)
{
	double  dataStack[MAX/2];   //存放中间数据的数据栈
	int top=0,            //数据栈dataStact的栈顶
		i;              //数组bolan的下标
	for(i=0;bolan[i].oper!='#';i++)
	{
		//节点bolan[i]为数值，则如数据栈dataStact
		if(bolan[i].flag==0)
		{
			top++;
			dataStack[top]=bolan[i].value;
			continue;
		}

		//节点bolan[i]为运算符，则从数据栈dataStact弹出数据进行计算，并
		//将结果压入数据栈dataStact中
		//在计算中若除数为0，返回1
		if(bolan[i].flag==1)
		{
			switch(bolan[i].oper)
			{
			case '+':
				dataStack[top-1]=dataStack[top-1]+dataStack[top];
				top--;
				break;

			case  '-':
				dataStack[top-1]=dataStack[top-1]-dataStack[top];
				top--;
				break;

			case '*':
				dataStack[top-1]=dataStack[top-1]*dataStack[top];
				top--;
				break;

			case '/':
				if(dataStack[top]!=0.0)
				{
					dataStack[top-1]=dataStack[top-1]/dataStack[top];
					top--;
					break;
				}
				else  //除数为0，返回1
				{
					return 1;
				}
			case '~':      //取负
				dataStack[top]=0-dataStack[top];
				break;
			}//switch
			continue;
		}//if(bolan[i].flag==1)

		//若节点bolan[i]为函数，则从数据栈dataStact弹出数据，调用相应
		//的库函数进行计算，并将结果压入数据栈dataStact中
		if(bolan[i].flag==2)
		{
			switch(bolan[i].func)
			{
			case 1:    //求绝对值函数abs(x)
				dataStack[top]=fabs(dataStack[top]);
				break;

			case 2:    //反余弦函数acos(x)
				if(-1.0<=dataStack[top] && dataStack[top]<=1.0)
				{
					dataStack[top]=acos(dataStack[top]);
					break;
				}
				else  //反余弦函数acos(x)中的x不满足条件，返回2
				{
					return 2;
				}

			case 3:   //反正弦函数asin(x)
				if(-1<=dataStack[top] && dataStack[top]<=1)
				{
					dataStack[top]=asin(dataStack[top]);
					break;
				}
				else  //反正弦函数asin(x)中的x不满足条件，返回3
				{
					return 3;
				}

			case 4:   //反正切函数atan(x)
				dataStack[top]=atan(dataStack[top]);
				break;

			case 5:  //反余切函数acot(x)
				dataStack[top]=PI/2-atan(dataStack[top]);
				break;

			case 6:   //余弦函数cos(x)
				dataStack[top]=cos(dataStack[top]);
				break;

			case 7:    //双曲余弦cosh(x)
				dataStack[top]=cosh(dataStack[top]);
				break;

			case 8:    //余切函数cot(x)   cot(x)=1/tan(x)
				if(tan(dataStack[top])!=0)
				{
					dataStack[top]=1/tan(dataStack[top]);
					break;
				}
				else    //余切函数cot(x) 中tan（x）为0，返回4
				{
					return 4;
				}

			case 9:    //e的x次方函数exp(x)
				dataStack[top]=exp(dataStack[top]);
				break;

			case 10:   //求不大于x的最大整数 floor(x)
				dataStack[top]=floor(dataStack[top]);
				break;

			case 11:   //取模x%y函数mod(x,y),即
				if(dataStack[top]!=0)
				{
					dataStack[top-1]=fmod(dataStack[top-1],dataStack[top]);
					top--;
					break;
				}
				else   //取模x%y函数mod(x,y)中y为0，返回5
				{
					return 5;
				}

			case 12:    //自然对数函数ln(x),如果x<=0,则返回6
				if(dataStack[top]>0)
				{          //ln(x)的库函数为log(x)
					dataStack[top]=log(dataStack[top]);
					break;
				}
				else
				{
					return 6;
				}

			case 13:    //取10的对数函数log10(x),如果x<=0,则返回7
				if(dataStack[top]>0)
				{          //log10(x)的库函数为log10(x)
					dataStack[top]=log10(dataStack[top]);
					break;
				}
				else
				{
					return 7;
				}

			case 14:    //x的y次方函数pow(x,y)
				dataStack[top-1]=pow(dataStack[top-1],dataStack[top]);
				top--;
				break;

			case 15:    //正弦函数sin(x)
				dataStack[top]=sin(dataStack[top]);
				break;

			case 16:    //双曲正弦sinh(x)
				dataStack[top]=sinh(dataStack[top]);
				break;

			case 17:   //开方函数sqrt(x),如果x<0,则返回8
				if(dataStack[top]>=0)
				{
					dataStack[top]=sqrt(dataStack[top]);
					break;
				}
				else
				{
					return 8;
				}

			case 18:    //正切函数tan(x)
				dataStack[top]=tan(dataStack[top]);
				break;

			case 19:    //双曲正切函数tanh(x)
				dataStack[top]=tanh(dataStack[top]);
				break;
			}//switch(bolan[i].func)
			continue;
		}//if(bolan[i].flag==2)
	}//for(i=0;bolan[i].oper!='#';i++)
	if(top==1)       //计算正确
	{
		result=dataStack[top];
		return 0;      //返回0
	}
	else               //计算中有其他错误，返回-1
		return -1;   
}

double CMathString::getvalue(void)  //返回计算得到的表达式值
{
	return result;
}


/************************************************************************/
/* 一步式获得值                                                                     */
/************************************************************************/
BOOL CMathString::GetValue(char * s,double * value)
{
	strcpy(string,s); 
    if (!checkString())
    {
		(*value)=0.0;
		return FALSE;
    }
	if (!stringToBolan())
	{
		(*value)=0.0;
		return FALSE;
	}
	if (compvalue()!=0)
	{
		(*value)=0.0;
		return FALSE;
	}
	(*value)=result;
	return TRUE;
}
/************************************************************************/
/* 判断是否为数值                                                       */
/************************************************************************/
bool CMathString::IsDouble(char * s)
{
	int dotnum=0;
	for (int i=0;s[i]!='\0';i++)
	{
		if(s[i]=='.') 
		{
			++dotnum;
			continue;
		}
		if (!isdigit(s[i])) return false;
	}
	return (dotnum>1?false:true);
}
/************************************************************************/
/* 必须在访问出模型的尺寸和参数之后才能执行                             */
/************************************************************************/
BOOL CRel::CheckEquation(CString Expression ,CString & strError)
{

	int     min;
	CString str,left,right;
	str=Expression;
	CStringArray importDim;
	CStringArray exportDim;
	for (int i=0;i<m_aryDimParaItem.GetCount();i++)	importDim.Add(m_aryDimParaItem[i].str);
	min=str.Find("=");
	if(min==-1)
	{
		str="方程没有等号";
		strError+=str;
		return FALSE;
	}

	if(min==0)
	{
		str="方程等号前面无其他项";
		strError+=str;
		return FALSE;
	}

	if(min==str.GetLength()-1)
	{
		str="方程等号后面无其他项";
		strError+=str;
		return FALSE;
	}
	left=str.Left(min);
	right=str.Mid(min+1);
	min=right.Find("=");
	if(min!=-1)
	{
		str="方程等号有两项或更多";
		strError+=str;
		return FALSE;
	}
	if(CheckExpression(left,&importDim,&exportDim,strError))
	{
		if(CheckExpression(left,&importDim,&exportDim,strError))
			return TRUE;
	}
	return FALSE;
}

BOOL CRel::CheckExpression(CString Expression , CStringArray * importDim
						   , CStringArray * exportDim,CString & strError)
{
	int rIndex,min;
	CString str,sub;
	int i;
	str=Expression;
	min=str.Find("(");
	int n=0;
	if(min!=-1)
	{

		for(i=min;i<str.GetLength();i++)
		{
			char ch=str.GetAt(i);
			if(ch=='(')
			{
				n++;
			}
			else if(ch==')')
			{
				n--;
				if(n==0)
				{
					rIndex=i;
					break;
				}
			}
		}
		if(i==str.GetLength())
		{
			str="括号不匹配";
			strError+=str;
			return FALSE;
		}
		sub=str.Mid(min+1,rIndex-min+1-2);///括号里面的字符串
		if(CheckExpression(sub,importDim,exportDim,strError)==FALSE) return FALSE;
		sub=str.Mid(min,rIndex-min+1);///包括括号的字符串
		///函数处理,现略
		CString str1,str3;
		str1=str.Mid(0,min);///获得前半段表达式,不包括括号.
		int a1,a2;
		a1=str1.ReverseFind('+');
		a2=str1.ReverseFind('-');
		if(a2>a1) a1=a2;
		a2=str1.ReverseFind('*');
		if(a2>a1) a1=a2;
		a2=str1.ReverseFind('/');
		if(a2>a1) a1=a2;
		a2=str1.ReverseFind('(');
		if(a2>a1) a1=a2;
		BOOL fff=FALSE;
		if(a1+1<str1.GetLength())
		{
			str3=str1.Mid(a1+1);
			CStringArray  label;
			label.Add("sin");
			label.Add("cos");
			label.Add("log");
			label.Add("exp");
			for(i=0;i<label.GetSize();i++)
			{
				if(str3==label.GetAt(i))
				{
					sub=str3+sub;
					fff=TRUE;
				}
			}
			if(fff==FALSE)
			{
				str="括号前面要有运算符号";
				strError+=str;
				return FALSE;
			}
		}
		////函数处理完毕
		str.Replace(sub,"1.23456789");
		return CheckExpression(str,importDim,exportDim,strError);
	}
	min=0;
	str=Expression.Tokenize("*+/-",min);
	if(str.GetLength()!=min-1)
	{
		str=""+str+"  前有重叠运算符号";
		str.Replace("1.23456789","括号");
		strError+=str;
		return FALSE;
	}
	///是否实数
	if(IsDouble(str))
	{
		if(str==Expression)
		{
			return TRUE;
		}
		else
		{
			if(min==Expression.GetLength()) 
			{
				str="在  "+str+"   后面出现错误,运算符号后面必须有其他项";
				str.Replace("1.23456789","括号");
				strError+=str;
				return FALSE;
			}
			str=Expression.Mid(min);
			return CheckExpression(str,importDim,exportDim,strError);
		}
	}

	/////////////是否尺寸或参数标识
	BOOL nFlag=FALSE;
	BOOL pFlag=TRUE;
	for(i=0;i<importDim->GetSize();i++)
	{
		if(str==importDim->GetAt(i))
		{
			nFlag=TRUE;
			for(int m=0;m<exportDim->GetSize();m++)
			{
				if(str==exportDim->GetAt(m))
					pFlag=FALSE;
			}
			if(pFlag==TRUE)
				exportDim->Add(str);
		}
	}
	if(str=="PI"||str=="E")/////判断常数
		nFlag=TRUE;
	if(nFlag==FALSE) 
	{
		min=str.Find("1.23456789");
		if(min!=-1)
			str="多余的右括号,或不匹配";
		else
			str="找不到"+str+"这一项或该项类型不为数值型,请检查";
		strError+=str;
		return FALSE;
	}

	if(str==Expression)
	{
		return TRUE;
	}
	else
	{
		if(min==Expression.GetLength()) 
		{
			str="在  "+str+"   后面出现错误,运算符号后面必须有其他项";
			str.Replace("1.23456789","括号");
			strError+=str;
			return FALSE;
		}
		str=Expression.Mid(min);
		return CheckExpression(str,importDim,exportDim,strError);
	}
	return TRUE;
}
/************************************************************************/
/* 第二种判断是否为实数的方法                                           */
/************************************************************************/
BOOL CRel::IsDouble(CString str)
{
	CString left,right;
	int c;
	int i;
	int nIndex=str.Find(".");
	if(nIndex!=-1)
	{
		right=str.Mid(nIndex+1);
		left=str.Left(nIndex);
	}
	else
	{
		right="12";
		left=str;
	}
	c=left.GetAt(0);
	if(left.GetLength()==1)
	{
		if(!IsDigit(c)) return FALSE;
	}
	else
	{
		if(!IsDigit(c)||c=='0') return FALSE;
		for( i=1;i<left.GetLength();i++)
		{
			c=left.GetAt(i);
			if(!IsDigit(c)) return FALSE;
		}
	}

	for(i=0;i<right.GetLength();i++)
	{
		c=right.GetAt(i);	  
		if(!IsDigit(c)) return FALSE;
	}
	return TRUE;
}
//获得关系式的右边的算术式,用数字代替符号
CString CRel::GetRelRightFormula(void)
{
	CString temp;
	for (int i=0;i<m_RightRel.GetCount();i++)
	{
		switch(m_RightRel[i].item_type) {
		case 0:
			temp+=m_RightRel[i].symbol;
			break;
		case 1:			
			temp.Format("%s%f",temp,UsrGetDimParaValue(&m_RightRel[i].dim,NULL));
			break;
		case 2:
			temp.Format("%s%f",temp,UsrGetDimParaValue(NULL,&m_RightRel[i].para));
			break;
		default:
			break;
		}		
	}
	return temp;
}
//获得关系式的左边的算术式,用数字代替
CString CRel::GetRelLeftFormula(void)
{
	CString temp;
	for (int i=0;i<m_leftRel.GetCount();i++)
	{
		switch(m_leftRel[i].item_type) {
		case 0:
			temp+=m_leftRel[i].symbol;
			break;
		case 1:			
			temp.Format("%s%f",temp,UsrGetDimParaValue(&m_leftRel[i].dim,NULL));
			break;
		case 2:
			temp.Format("%s%f",temp,UsrGetDimParaValue(NULL,&m_leftRel[i].para));
			break;
		default:
			break;
		}
	}
	return temp;
}
CString CRel::GetRelFormula(void)
{
	CString temp;
	temp=GetRelLeftFormula();
	switch(m_iRelType) {
	case REL_TYPE_EQUAL:
		temp+="=";
		break;
	case REL_TYPE_MORE_EQUAL:
		temp+=">=";
		break;
	case REL_TYPE_LESS_EQUAL:
		temp+="<=";
		break;
	case REL_TYPE_MORE:
		temp+=">";
		break;
	case REL_TYPE_LESS:
		temp+="<";
		break;
	case REL_TYPE_UN_EQUAL:
		temp+="<>";
		break;
	default:
		temp+="=";
		break;
	}
	temp+=GetRelRightFormula();
	return temp;
}

