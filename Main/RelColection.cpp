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
/* ��ģ������ӹ�ϵʽ                                                   */
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
	//�жϵ�ʽ���͵Ĺ�ϵʽ,����Ƿ����Ѿ�������ģ�͵��������Ա���,�����,���ܹ����
	if (new_rel.m_iRelType==CRel::rel_type::REL_TYPE_EQUAL && !HasSwtxItemInRelLeft(owner,&new_rel,error_info))
	{
		error_info.Format("�ߴ�(����):%s,�Ѿ���Ϊ�������Ա�,�������ڵ�ʽ��ϵ����Ϊ��������",error_info);
		return FALSE;
	}
	//�ж�ģ�͹�ϵʽ�Ƿ����
	if (!IsRelInCollection(owner,rel))
	{
		new_rel.m_strNote=note;
		m_CRelList.Add(new_rel);
		error_info.Empty();
	}
	else
	{
		error_info=_T("��ϵʽ�Ѿ�����");
		return FALSE;
	}	
	return TRUE;
}
/************************************************************************/
/* ���ݹ�ϵʽ�ı����ɾ��                                               */
/************************************************************************/
BOOL CRelColection::DelRel(int indexNo)
{
	m_CRelList.RemoveAt(indexNo);
	m_CRelList.FreeExtra();
	return TRUE;
}
/************************************************************************/
///* ɾ���ƶ�ģ�����еĹ�ϵʽ,(ɾ��--����ģ�͵���װ�����ڵĹ�ϵʽ��                */
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
/* �޸Ĺ�ϵʽ                                                           */
/************************************************************************/
BOOL CRelColection::EditRel(int index,CString str,CString note)
{
	CString		temp;
	bool		bolIsOk=false;
	//���̣��ж��޸ĵĹ�ϵ�Ƿ��Ѿ����ڣ����û���޸���֮�޸��ı�������޸ģ���ɾ�������½�һ����ϵʽ
	if (index<m_CRelList.GetCount())
	{
		//�ж��Ƿ��Ѿ�����,���������֮�޸��ı������������޸�
		if (IsRelInCollection(m_CRelList[index].owner,str))
		{
			error_info=_T("��ϵʽ�Ѿ�����,���ϵδ�޸�");
			m_CRelList[index].m_strNote=note;
		} 
		else
		{
			//�����ϵʽ����ȷ����ԭΪԭ���Ĺ�ϵʽ�������޸�
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
		error_info=_T("û��ѡ�й�ϵʽ");
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* ���¹�ϵʽ��ɾ���Ѿ������ڵ�                                         */
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
/* �õ�ģ�͵Ĺ�ϵ���������ģ�ͣ��Ƿ�Ҫ�õ����еĹ�ϵʽ��
   ������Ԫ���ڲ��Ĺ�ϵ��Ĭ��Ϊ�ǣ�����������Բ�����                   */
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
/* �ж�һ����ϵʽ�Ƿ��Ѿ���ģ����                                       */
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
/* ���ģ����سߴ�                                                      */
/************************************************************************/
BOOL CRelColection::GetRvDimAndPara(CArray<UsrSWTXTable1,UsrSWTXTable1> * swtx
				, ProSolid solid,CArray<UsrSWTXTable1,UsrSWTXTable1> * p_RvList)
{
	CArray<UsrSWTXTable1,UsrSWTXTable1>  p_AllDimParaList; 
	GetSolidAllRelDimPara(solid,p_AllDimParaList);       //��ù�ϵʽ���еĳߴ����
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
/* �ж�һ����ϵ�Ƿ��Ѿ��ڹ�ϵ����,װ��������Ĺ�ϵʽ�к�׺,��װ������޸�
����Ĺ�ϵʽ��������Ĺ�ϵʽ�����к�׺                                  */
/************************************************************************/
BOOL CRelColection::IsRelInColection(ProMdlType mdl_type,CString str)
{
	CString temp;
	if((str.Find(':',0)!=-1) && mdl_type==PRO_MDL_PART) mdl_type=PRO_MDL_ASSEMBLY;           //������
	for (int i=0;i<m_CRelList.GetCount();i++)
	{
		temp.Empty();
		switch(mdl_type) {
		case PRO_MDL_ASSEMBLY:
			if(m_CRelList[i].GetRelStringAsComp(temp))   //��ϵ��Ҫ�к�׺
			{
				if (str==temp) return TRUE;
			}
			break;
		case PRO_MDL_PART:
			if(m_CRelList[i].GetRelString(temp))         //�����Ǻ�׺
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
/* ���ģ�����еĳߴ�Ͳ�����(ע����Ҫ�Ľ�,��ϵ�е�ĳ���ߴ������ɾ��)  */
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
	//�ٷ���ģ���еĲ���,���������ͱ���Ϊ���ͻ򸡵���
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
/*�ж�һ���ߴ�Ͳ����Ƿ��ڹ�ϵʽ�������(��ʽ����)                      */
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
/* �жϹ�ϵʽ����Ƿ����������Ա���(�ߴ����ֵ�ԵĲ���,�������ַ��Ͳ����͵�)
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
					//�����������Ա���,
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