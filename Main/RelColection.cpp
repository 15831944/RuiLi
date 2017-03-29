#include "StdAfx.h"
#include "relcolection.h"

CRelColection::CRelColection(void)
:error_info(_T(""))
{
}

CRelColection::~CRelColection(void)
{
}
/************************************************************************/
/* 往模型中添加关系式                                                   */
/************************************************************************/
BOOL CRelColection::AddRel(ProSolid owner, CString rel, CString note)
{
	bool IsOk=false;
	CRel new_rel(owner,rel,IsOk);
	CString temp;
	if (IsOk==false)
	{
		error_info=new_rel.m_strError;
		return FALSE;
	}
	//判断等式类型的关系式,左侧是否是已经存在与模型的事物特性表中,如果是,则不能够添加
	if (new_rel.m_iRelType==CRel::rel_type::REL_TYPE_EQUAL && !HasSwtxItemInRelLeft(owner,&new_rel,error_info))
	{
		error_info.Format("尺寸(参数):%s,已经作为事物特性表,不能再在等式关系中作为被驱动项",error_info);
		return FALSE;
	}
	//判断模型关系式是否存在
	if (!IsRelInCollection(owner,rel))
	{
		new_rel.m_strNote=note;
		m_CRelList.Add(new_rel);
		error_info.Empty();
	}
	else
	{
		error_info=_T("关系式已经存在");
		return FALSE;
	}	
	return TRUE;
}
/************************************************************************/
/* 根据关系式的标号来删除                                               */
/************************************************************************/
BOOL CRelColection::DelRel(int indexNo)
{
	m_CRelList.RemoveAt(indexNo);
	m_CRelList.FreeExtra();
	return TRUE;
}
/************************************************************************/
///* 删除制定模型所有的关系式,(删除--包括模型的子装配体内的关系式）                */
/************************************************************************/
BOOL CRelColection::RemoveAllRel(ProSolid solid)
{
	//CArray<ProSolid,ProSolid> solid_list;
	//solid_list.Add(solid);
	//GetAsmCompHand(solid,&solid_list);
	for (int i=0;i<m_CRelList.GetCount();i++)
	{
		//for (int j=0;j<solid_list.GetCount();j++)
		//{
		//	if (m_CRelList[i].owner==solid_list[j])
		//	{
		//		m_CRelList.RemoveAt(i);
		//		m_CRelList.FreeExtra();
		//		i-=1;
		//		break;
		//	}
		//}
		if (m_CRelList[i].owner==solid)
		{
			m_CRelList.RemoveAt(i);
			m_CRelList.FreeExtra();
			i-=1;
			break;
		}
	}
	return TRUE;
}
/************************************************************************/
/* 修改关系式                                                           */
/************************************************************************/
BOOL CRelColection::EditRel(int index,CString str,CString note)
{
	CString		temp;
	bool		bolIsOk=false;
	//过程：判断修改的关系是否已经存在，如果没有修改则之修改文本，如果修改，则删除，并新建一个关系式
	if (index<m_CRelList.GetCount())
	{
		//判断是否已经存在,如果存在则之修改文本，不存在则修改
		if (IsRelInCollection(m_CRelList[index].owner,str))
		{
			error_info=_T("关系式已经存在,或关系未修改");
			m_CRelList[index].m_strNote=note;
		} 
		else
		{
			//如果关系式不正确，则还原为原来的关系式，即不修改
			CRel   new_rel(m_CRelList[index].owner,str,(str.Find(":",0)!=-1?true:false),bolIsOk);
			if(bolIsOk)
			{
				m_CRelList[index]=new_rel;
				m_CRelList[index].m_strNote=note;
			}
			else
			{
				error_info=new_rel.m_strError;
				return FALSE;
			}
		}
	}
	else
	{
		error_info=_T("没有选中关系式");
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 更新关系式，删除已经不存在的                                         */
/************************************************************************/
BOOL CRelColection::Update(ProSolid solid)
{
	CArray<ProSolid,ProSolid> solid_list;
	solid_list.Add(solid);
	GetAsmCompHand(solid,&solid_list);
	for (int i=0;i<m_CRelList.GetCount();i++)
	{
		for (int j=0;j<solid_list.GetCount();j++)
		{
			if (m_CRelList[i].owner==solid_list[j])
			{
				CString temp;
				if(!m_CRelList[i].GetRelString (temp))
				{
					m_CRelList.RemoveAt(i);
					m_CRelList.FreeExtra();
					i-=1;
					break;
				}
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 得到模型的关系，对于组件模型，是否要得到所有的关系式，
   即包括元件内部的关系，默认为是，对于零件可以不考虑                   */
/************************************************************************/
BOOL CRelColection::GetSolidAllRel(ProSolid solid, bool bolAsmUplevel
								   ,CArray<CRel,CRel&> * p_CRelList,CArray<int,int&> * p_IndexList)
{
	CArray<ProSolid,ProSolid> solid_list;
	solid_list.Add(solid);
	Update(solid);
	if(bolAsmUplevel) GetAsmCompHand(solid,&solid_list);
	for (int i=0;i<m_CRelList.GetCount();i++ )
	{
		for (int j=0;j<solid_list.GetCount();j++)
		{
			if (m_CRelList[i].owner==solid_list[j])
			{
				p_CRelList->Add(m_CRelList[i]);
				p_IndexList->Add(i);
				break;
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 判断一个关系式是否已经在模型中                                       */
/************************************************************************/
BOOL CRelColection::IsRelInCollection(ProSolid solid, CString rel)
{
	ProMdlType mdl_type;
	if(ProMdlTypeGet(solid,&mdl_type)==PRO_TK_NO_ERROR)
	{
		return IsRelInColection(mdl_type,rel);
	}
	return FALSE;
}
/************************************************************************/
/* 获得模型相关尺寸                                                      */
/************************************************************************/
BOOL CRelColection::GetRvDimAndPara(CArray<UsrSWTXTable1,UsrSWTXTable1> * swtx
				, ProSolid solid,CArray<UsrSWTXTable1,UsrSWTXTable1> * p_RvList)
{
	CArray<UsrSWTXTable1,UsrSWTXTable1>  p_AllDimParaList; 
	GetSolidAllRelDimPara(solid,p_AllDimParaList);       //获得关系式所有的尺寸参数
	for (int j=0;j<p_AllDimParaList.GetCount();j++)
	{
		int Flag=0;
		for (int i=0;i<swtx->GetCount();i++)
		{/*
			AfxMessageBox(p_AllDimParaList[j].type +"   "+swtx->GetAt(i).type);
			CString temp;
			temp=swtx->GetAt(i).Name;
			AfxMessageBox(temp);*/
			if(p_AllDimParaList[j].type==swtx->GetAt(i).type)
			{


				if (p_AllDimParaList[j].type=="dim")
				{
					if(ComparaDimension(&p_AllDimParaList[j].Dim,&(swtx->GetAt(i).Dim)))
					{
						Flag=1;
						break;
					}
				}
				else if (p_AllDimParaList[j].type=="para")
				{
					if(ComparaParameter(&p_AllDimParaList[j].Para,&(swtx->GetAt(i).Para)))
					{
						Flag=1;
						break;
					}
				}
			}
		}
		if(Flag==0) p_RvList->Add(p_AllDimParaList[j]);
	}	
	return TRUE;
}
/************************************************************************/
/* 判断一个关系是否已经在关系集内,装配件进来的关系式有后缀,在装配件中修改
零件的关系式，则进来的关系式里面有后缀                                  */
/************************************************************************/
BOOL CRelColection::IsRelInColection(ProMdlType mdl_type,CString str)
{
	CString temp;
	if((str.Find(':',0)!=-1) && mdl_type==PRO_MDL_PART) mdl_type=PRO_MDL_ASSEMBLY;           //如果零件
	for (int i=0;i<m_CRelList.GetCount();i++)
	{
		temp.Empty();
		switch(mdl_type) {
		case PRO_MDL_ASSEMBLY:
			if(m_CRelList[i].GetRelStringAsComp(temp))   //关系需要有后缀
			{
				if (str==temp) return TRUE;
			}
			break;
		case PRO_MDL_PART:
			if(m_CRelList[i].GetRelString(temp))         //不考虑后缀
			{
				if (str==temp) return TRUE;
			}
			break;
		default:
			return FALSE;
		}
	}
	return FALSE;
}
/************************************************************************/
/* 获得模型所有的尺寸和参数列(注：需要改进,关系中的某个尺寸如果被删除)  */
/************************************************************************/
void CRelColection::GetSolidAllDimPara(ProSolid solid,CArray<UsrSWTXTable1,UsrSWTXTable1> & p_DimParaList)
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
			GetSolidAllDimPara(mdl,p_DimParaList);
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
		UsrSWTXTable1 temp_DimPara(mdl,&p_DimentionList[i]);
		p_DimParaList.Add(temp_DimPara);
	}
	//再访问模型中的参数,参数的类型必须为整型或浮点型
	ProMdlToModelitem(solid,&modelitem);
	ProParameterVisit((ProModelitem*)&modelitem,NULL,(ProParameterAction)partAsmParaVisAc,
		(CArray<ProParameter,ProParameter>*)&Parameterlist);
	num=(int)Parameterlist.GetCount();
	for (int i=0;i<num;i++) {
		ProParamvalueType	r_type;
		ProParamvalue		proval;
		ProParameterValueGet(&Parameterlist[i],&proval);
		ProParamvalueTypeGet(&proval,&r_type);
		if(r_type==PRO_PARAM_DOUBLE||r_type==PRO_PARAM_INTEGER)
		{
			UsrSWTXTable1	temp_DimPara(mdl,&Parameterlist[i]);
			p_DimParaList.Add(temp_DimPara);
		}
	}
}
void CRelColection::GetSolidAllRelDimPara(ProSolid mdl,CArray<UsrSWTXTable1,UsrSWTXTable1> &p_DimParaList)
{
	CArray<CRel,CRel&>					p_RelList;
	CArray<int,int&>					p_IndexList;
	CArray<ProDimension,ProDimension&>  p_temp_DimList;
	CArray<ProParameter,ProParameter&>  p_temp_ParamList;
	CArray<ProDimension,ProDimension&>  p_DimList;
	CArray<ProParameter,ProParameter&>  p_ParamList;
	CString								id="";
	CString								rel;
	int									indexNo=0;
	GetSolidAllRel(mdl,true,&p_RelList,&p_IndexList);
	for (int i=0;i<p_RelList.GetCount();i++)
	{
		//CString test;
		//p_RelList[i].GetRelString(test);
		//AfxMessageBox(test);


		p_temp_DimList.RemoveAll();
		p_temp_ParamList.RemoveAll();
		p_RelList[i].GetRelAllParamList(&p_temp_ParamList);
		p_RelList[i].GetRelAllDimList(&p_temp_DimList);
		//test.Format("%d",p_temp_DimList.GetCount());
		//AfxMessageBox(test);

		for (int m=0;m<p_temp_ParamList.GetCount();m++)
		{
			int Flag1=0;
			for (int m1=0;m1<p_ParamList.GetCount();m1++)
			{
				if(ComparaParameter(&p_ParamList[m1],&p_temp_ParamList[m]))
				{
					Flag1=1;
					break;
				}
			}
			if(Flag1==0) p_ParamList.Add(p_temp_ParamList[m]);
		}
		for (int n=0;n<p_temp_DimList.GetCount();n++)
		{
			int Flag2=0;
			for (int n1=0;n1<p_DimList.GetCount();n1++)
			{
				if (ComparaDimension(&p_DimList[n1],&p_temp_DimList[n]))
				{
					Flag2=1;
					break;
				}
			}
			if(Flag2==0) p_DimList.Add(p_temp_DimList[n]);
		}
	}
	for (int i=0;i<p_DimList.GetCount();i++)
	{
		UsrSWTXTable1	temp_DimPara(mdl,&p_DimList[i]);
		p_DimParaList.Add(temp_DimPara);
	}
	for (int i=0;i<p_ParamList.GetCount();i++)
	{
		UsrSWTXTable1	temp_DimPara(mdl,&p_ParamList[i]);
		p_DimParaList.Add(temp_DimPara);
	}
}
/************************************************************************/
/*判断一个尺寸和参数是否在关系式集的左侧(等式类型)                      */
/************************************************************************/
bool CRelColection::IsDimParaInRelSetLeftPart(ProSolid solid
											  ,ProParameter * para=NULL,ProDimension * dim=NULL)
{
	CArray<CRel,CRel&>					p_RelList;
	CArray<int,int&>					p_IndexList;
	CArray<ProDimension,ProDimension&>  p_temp_DimList;
	CArray<ProParameter,ProParameter&>  p_temp_ParamList;
	CArray<ProDimension,ProDimension&>  p_DimList;
	CArray<ProParameter,ProParameter&>  p_ParamList;
	GetSolidAllRel(solid,true,&p_RelList,&p_IndexList);
	for (int i=0;i<p_RelList.GetCount();i++)
	{
		if(p_RelList[i].m_iRelType!=CRel::rel_type::REL_TYPE_EQUAL)
			continue;
		p_temp_DimList.RemoveAll();
		p_temp_ParamList.RemoveAll();
		p_RelList[i].GetLeftDimList(&p_temp_DimList);
		p_RelList[i].GetLeftParamList(&p_temp_ParamList);
		p_DimList.Append(p_temp_DimList);
		p_ParamList.Append(p_temp_ParamList);
	}
	if (dim!=NULL)
	{
		for (int i=0;i<p_DimList.GetCount();i++)
		{
			if (ComparaDimension(dim,&p_DimList[i]))
				return false;
		}
	}
	if (para!=NULL)
	{
		for (int i=0;i<p_ParamList.GetCount();i++)
		{
			if (ComparaParameter(para,&p_ParamList[i]))
				return false;
		}
	}
	return true;
}
/************************************************************************/
/* 判断关系式左侧是否有事物特性表项(尺寸或数值性的参数,不考虑字符和布儿型的)
																		*/
/************************************************************************/
bool CRelColection::HasSwtxItemInRelLeft(ProSolid solid,CRel * rel,CString & csError)
{
	CArray<ProDimension,ProDimension&>  p_DimList;
	CArray<ProParameter,ProParameter&>  p_ParamList;
	rel->GetLeftDimList(&p_DimList);
	rel->GetLeftParamList(&p_ParamList);
	csError.Empty();
	switch(GetMdlType(solid)) {
	case PRO_MDL_PART:
				for(int i=0;i<(int)p_UsrSWTXTableList.GetCount();i++)
				{
					//更新事物特性表项,
					p_UsrSWTXTableList[i].Update();
					if(p_UsrSWTXTableList[i].isCheck!=1)	 continue;
					if(p_UsrSWTXTableList[i].owner!=solid)	 continue;
					if (p_UsrSWTXTableList[i].type=="dim")
					{
						for (int m=0;m<p_DimList.GetCount();m++)
						{
							if (ComparaDimension(&p_DimList[m],&p_UsrSWTXTableList[i].Dim))
							{
								csError.Format("%s %s",csError,p_UsrSWTXTableList[i].dimSymbol);
							}
						}
					}
					else if (p_UsrSWTXTableList[i].type=="para"
						      && (p_UsrSWTXTableList[i].subType=="PRO_PARAM_DOUBLE" || p_UsrSWTXTableList[i].subType=="PRO_PARAM_INTEGER"))
					{
						for (int m=0;m<p_ParamList.GetCount();m++)
						{
							if (ComparaParameter(&p_ParamList[m],&p_UsrSWTXTableList[i].Para))
							{
								csError.Format("%s %s",csError,p_UsrSWTXTableList[i].featName);
							}
						}
					}
				}
		break;
	case PRO_MDL_ASSEMBLY:
				for (int i=0;i<p_AsmSmlItemList.GetCount();i++)
				{
					if(p_AsmSmlItemList[i].owner!=solid) continue;
					if (!CheckAsmSWTX(solid,&p_AsmSmlItemList[i]))
					{
						p_AsmSmlItemList.RemoveAt(i,1);
						i--;
						continue;
					}
					if (p_AsmSmlItemList[i].marsk==TYPE_DIM)
					{
						for (int m=0;m<p_DimList.GetCount();m++)
						{
							if (ComparaDimension(&p_DimList[m],&p_AsmSmlItemList[i].dim))
							{
								csError.Format("%s d%d",csError,p_AsmSmlItemList[i].dim.id);
							}
						}
					}
					else if (p_AsmSmlItemList[i].marsk==TYPE_PARA)
					{
						ProParamvalue			proval;
						ProParameterValueGet(&p_AsmSmlItemList[i].para,&proval);
						if (proval.type==PRO_PARAM_DOUBLE || proval.type==PRO_PARAM_INTEGER)
						{
							for (int m=0;m<p_ParamList.GetCount();m++)
							{
								if (ComparaParameter(&p_ParamList[m],&p_AsmSmlItemList[i].para))
								{
									csError.Format("%s %s",csError,CString(p_AsmSmlItemList[i].para.id));
								}
							}
						}
					}
				}
		break;
	default:
		return false;
		break;
	}
    if (csError.IsEmpty())
		return true;
	else return false;
}