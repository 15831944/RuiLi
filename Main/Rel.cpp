#include "StdAfx.h"
#include "rel.h"
/************************************************************************/
/*���͵�ʹ�÷�����
  ���캯���������ϵ������ģ���Ѿ���ϵ�����ԶԹ�ϵʽ���з��룬�ѹ�ϵʽ��
  �ߴ�Ͳ�����ȡ�������ѹ�ϵ�гߴ�Ͳ���֮��������Լ��������Ϊһ���ַ���
  �Թ�ϵʽ�ļ�飺
  �õ���ϵ�ĳߴ�Ͳ���
  �����ϵ���ұߺ���ߵ�ֵ                                              */
/************************************************************************/

CRel::CRel(void):bolNeedPostID(FALSE)
,m_strNote(_T("")),m_strError(_T("")),m_iRelType(REL_TYPE_NO),mdlType(PRO_MDL_UNUSED)
{
	InitCalSymbol();
}
/************************************************************************/
/* ���캯����������ϵ��ȷ���                                         */
/************************************************************************/
CRel::CRel(ProSolid solid,CString strRel)
:bolNeedPostID(FALSE),m_strNote(_T("")),m_strError(_T("")),m_iRelType(REL_TYPE_NO),mdlType(PRO_MDL_UNUSED)
{
	InitCalSymbol();
	owner=solid;
	strRel.Trim();                               //������ո�
	sprintf(m_strRel,"%s\0",strRel.MakeLower());    //��Сд
	//��������Ĺ�ϵʽ��Ҫ��Ӻ�׺
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
/* �ܹ����ؼ��ֵ���������κεĹ�ϵʽ���                               */
/************************************************************************/
CRel::CRel(ProSolid solid,CString strRel,bool & Revbol)
:bolNeedPostID(FALSE),m_strNote(_T("")),m_strError(_T("")),m_iRelType(REL_TYPE_NO),mdlType(PRO_MDL_UNUSED)
{
	InitCalSymbol();
	owner=solid;
	strRel.Trim();                               //������ո�
	Revbol=true;
	sprintf(m_strRel,"%s\0",strRel.MakeLower());    //��Сд
	//��������Ĺ�ϵʽ��Ҫ��Ӻ�׺
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
	//����ϵʽ�ĸ�ʽ
	if (CheckRelSymbol(strRel.MakeLower(),m_strError) &&  CheckEquation(strRel.MakeLower(),m_strError))
	{
		DepartRel();
		if(!m_strError.IsEmpty()) Revbol=false;
	}
	else
	{
		Revbol=false;
	}
	//��ʽ���ֻ����һ��
	if (m_iRelType==REL_TYPE_EQUAL && m_leftRel.GetCount()>1)
	{
		m_strError+="��ʽ�����̫��,����������";
		Revbol=false;
	}
	else if(m_iRelType==REL_TYPE_EQUAL && m_leftRel[0].item_type==0)
	{
		m_strError+="��ʽ���û�гߴ�Ͳ���,����������";
		Revbol=false;
	}
}
CRel::CRel(ProSolid solid,CString strRel,bool bolPostId/*�����Ƿ�Ҫ��׺*/,bool & Revbol /*�ж��Ƿ�ɹ�*/)
:bolNeedPostID(FALSE),m_strNote(_T("")),m_strError(_T("")),m_iRelType(REL_TYPE_NO),mdlType(PRO_MDL_UNUSED)
{
	InitCalSymbol();
	owner=solid;
	strRel.Trim();                               //������ո�
	Revbol=true;
	sprintf(m_strRel,"%s\0",strRel.MakeLower());    //��Сд
	if(solid==NULL)
	{
		Revbol=false;
		return;
	}
	//��������Ĺ�ϵʽ��Ҫ��Ӻ�׺,�Լ�ǿ����Ҫ��׺
	ProMdlTypeGet((ProMdl)solid,(ProMdlType*)&mdlType);
	if (mdlType==PRO_MDL_ASSEMBLY || bolPostId)
	{
		bolNeedPostID=TRUE;
	}
	GetDimParaOfSolid(solid);
	//����ϵʽ�ĸ�ʽ
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
		m_strError+="��ʽ�����̫��,����������";
		Revbol=false;
	}
	else if(m_iRelType==REL_TYPE_EQUAL && m_leftRel[0].item_type==0)
	{
		m_strError+="��ʽ���û�гߴ�Ͳ���,����������";
		Revbol=false;
	}
}
CRel::~CRel(void)
{
}
/************************************************************************/
/* �������캯��                                                         */
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
/* ���֧�ֵĺ���                                                       */
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
/* ��ù�ϵʽ                                                           */
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
				m_strError="ģ���Ѿ���������ĳ�Ա";
				return FALSE;
			}
			if(ProDimensionSymbolGet(&(m_aryRel[i].dim),name)!=PRO_TK_NO_ERROR)
			{
				m_strError.Format("%s�ߴ�\"%s\"������",m_strError,m_aryRel[i].symbol );
				return FALSE;
			}
			if(bolNeedPostID==TRUE && ProSolidToPostfixId((ProSolid)m_aryRel[i].dim.owner ,&postId)!=PRO_TK_NO_ERROR)
			{
				m_strError.Format("%s�ߴ�\"%s\"������",m_strError,m_aryRel[i].symbol );
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
				m_strError="ģ���Ѿ���������ĳ�Ա";
				return FALSE;
			}
			if(bolNeedPostID==TRUE && ProSolidToPostfixId(para_owner ,&postId)!=PRO_TK_NO_ERROR)
			{
				m_strError.Format("%s����\"%s\"������",m_strError,m_aryRel[i].symbol );
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
/* ���һ���к�׺�Ĺ�ϵ                                                 */
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
/* ��ù�ϵ���ı�                                                       */
/************************************************************************/
CString  CRel::GetRelNote(void)
{
	return m_strNote;
}
void  CRel::GetRightParamList(CArray<ProParameter,ProParameter&> * p_ParamList)        //����ұ߲������� < > �ұߵĲ�����
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
void  CRel::GetRightDimList(CArray<ProDimension,ProDimension&> * p_DimList)         //����ұ߳ߴ���б�
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
void  CRel::GetLeftParamList(CArray<ProParameter,ProParameter&> * p_ParamList)         //�����ߵĲ����б�
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
void  CRel::GetLeftDimList(CArray<ProDimension,ProDimension&> * p_DimList)           //�����߳ߴ磨= < > ��ߵĲ�����
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
/* ���ù�ϵʽ�ұ߲�����ֵ                                               */
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
	sprintf(m_strRel,"%s",str.Trim().MakeLower());    //��Сд;
	return FALSE;
}
/************************************************************************/
/* ����ϵʽ                                                           */
/************************************************************************/
BOOL  CRel::CheckRel(void)
{
	return FALSE;
}
/************************************************************************/
/* ����ϵʽ                                                           */
/************************************************************************/
BOOL  CRel::CheckRel(CString strRel)
{
	if(strRel.Trim().GetLength()>MAX)
	{
		AfxMessageBox("��ϵʽ̫��");
		return FALSE;
	}
	sprintf(m_strRel,"%s",strRel.MakeLower());	
	return Updata();
}
/************************************************************************/
/* �����ϵʽ�������ߣ�ȷ����ϵ������                                   */
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
	//���ù�ϵʽ������
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
/* ����ģ�͵����в����ͳߴ�                                             */
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
	//�ٷ���ģ���еĲ���,���������ͱ���Ϊ���ͻ򸡵���
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
		m_strError+=_T("���ʽ����");
		return FALSE;          //���ʽ����
	}
	mathStr.stringToBolan();
	if (mathStr.compvalue()!=0)
	{
		(*dblReValue)=0.0;
		m_strError+=_T("�����г���");
		return FALSE;         //�����г������ݷ���ֵ����һ��һ��ȷ��ԭ��
	}
	(*dblReValue)=mathStr.getvalue(); 
	return TRUE;
}
/************************************************************************/
/* �Թ�ϵʽ���з��루ֻ�е�ʽһ�ߣ�                                     */
/************************************************************************/
int  CRel::CheckRelPart(char rel_str[MAX],CArray<rel_item,rel_item&> * temp_rel_item)
{
	//char st[MAX/2];                   //������ŵ�ջ
	int top=0,                        //ջ��
		i;
	CArray<int,int> i_ary;            //���ڹ�ϵʽ�ķ���״̬�ж� 1(��һ�����룬�����Ժϲ���) 0(û�з���) -1(��������������Ժϲ���)
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
		////����������ţ�����ջ
		//if(rel_str[i]=='(' || rel_str[i]=='[' || rel_str[i]=='{')
		//{  //����������ţ�����ջ
		//	top++;
		//	st[top]=rel_str[i];
		//	continue;
		//}

		////С����
		//if(rel_str[i]==')')   //С����
		//{  
		//	if(st[top]=='(')   //���
		//	{
		//		top--;         //��ջ
		//		continue;
		//	}
		//	else               //����ԣ�����0
		//	{
		//		m_strError+=_T("С���Ų����\n");
		//		return 0;    
		//	}
		//}
		////������
		//if(rel_str[i]==']')    
		//{  
		//	if(st[top]=='[')   //���
		//	{
		//		top--;         //��ջ
		//		continue;
		//	}
		//	else 
		//	{
		//		m_strError+=_T("�����Ų����\n");
		//		return 0;    //����ԣ�����0
		//	}
		//}
		////������
		//if(rel_str[i]=='}')    
		//{  
		//	if(st[top]=='{')   //���
		//	{
		//		top--;         //��ջ
		//		continue;
		//	}
		//	else 
		//	{
		//		m_strError+=_T("�����Ų����\n");
		//	    return 0;    //����ԣ�����0
		//	}
		//}
		////�����
		//if(rel_str[i]=='+' || rel_str[i]=='-' || rel_str[i]=='*' || rel_str[i]=='/')
		//{   //�������������������
		//	if(rel_str[i+1]=='+' || rel_str[i+1]=='-' || rel_str[i+1]=='*' ||rel_str[i+1]=='/')
		//	{
		//		m_strError+=_T("���������������\n");
		//		return 0;  //���󣬷���0
		//	}
		//	else
		//		continue;  //��������������
		//}
		////�����Ķ�������
		//i_ary[i]=0;
	}
	//if(top!=0)       //���Ų�ƥ��,����0
	//{
	//	m_strError+=_T("���Ų�ƥ��\n");
	//	return 0;
	//}
	    //�������
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
	//���������
	for (i=0;rel_str[i]!='\0';i++)
	{
		if(i_ary[i]==1) continue;
		if (rel_str[i]=='a' || rel_str[i]=='c' || rel_str[i]=='e' || rel_str[i]=='f' 
			|| rel_str[i]=='m' || rel_str[i]=='l' || rel_str[i]=='p' || rel_str[i]=='s' || rel_str[i]=='t')
		{
			for (int j=0;j<m_calSymbol.GetCount();j++ )
			{
				int m=i;
				//����������������ǰ����������ŵģ���Ϊ����
				if(mathStr.seekStr(rel_str,i,m_calSymbol[j].GetBuffer()) && 
					(rel_str[m-1]=='+' || rel_str[m-1]=='-' || rel_str[m-1]=='*' || rel_str[m-1]=='/' || rel_str[m-1]=='='))
				{
					i-=1;
					for(m;m<=i;m++) i_ary[m]=1;
					if(rel_str[i+1]==')')
					{
						m_strError+=_T("����"+m_calSymbol[j]+")�����ڲ���Ϊ��\n");
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
	//���������������
	for (i=0;rel_str[i]!='\0';i++)
	{
		if(i_ary[i]==1) continue;
		//�õ�������ͺ���֮����ַ�
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
		//�������һ����
		if(m==i_ary.GetCount()) str[c]='\0',i=m-1;
		//��������бȽ�
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
				m_strError+=_T("�޷��ҵ�:"+CString(str)+"(������ܲ������ͻ�ʵ���Ĳ���)\n");
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

	//����Ƿ���û�з����
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
			m_strError+=_T("�޷��ҵ�:"+CString(temp)+'\n');
			return 0;
		}
	}
	//for (i=0;rel_str[i]!='\0';i++)
	//{
	//	test.Format("%s%c    %d\n",test,rel_str[i],i_ary[i]);
	//}
	//AfxMessageBox(test);
	//test.Empty();
	//�����з������������ӵ���ϵ������
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
				m_strError+=_T("�޷��ҵ�:"+temp_item.symbol+'\n');
				return 0;
			}
			continue;
		}
		else if (i_ary[i]==0)
		{
			m_strError+=_T("����");
			return 0;
		}
	}
	return 1;	
}

/************************************************************************/
/* �Թ�ϵʽ�ķ��Ž��м��,���������һ���ĸ�ʽ����                      */
/************************************************************************/
bool CRel::CheckRelSymbol(CString rel,CString & error)
{	

	//��飽 > <��
	//�ڲ�����
	rel.Trim();
	if((GetSymbolNum(rel,"=")+GetSymbolNum(rel,">")+GetSymbolNum(rel,"<"))==0) 
	{error.AppendFormat("��ϵʽû�бȽ��������(=,<,>)\n");return false;}
    if(GetSymbolNum(rel,"=")>1  || GetSymbolNum(rel,">")>1 || GetSymbolNum(rel,"<")>1) 
		{error.AppendFormat("��ϵʽ�д��ڶ���ظ��ıȽ��������(=,<,>)\n");return false;}
    if((GetSymbolNum(rel,"=")+GetSymbolNum(rel,"<=")+GetSymbolNum(rel,"<=") + GetSymbolNum(rel,"<>"))!=1 ) 
		{error.AppendFormat("��ϵʽ�еıȽ����������ȷ������Ƿ���ڻ����Ƿ��ж��,���߸�ʽ�Ƿ���ȷ\n(����(=),���ڵ���(>=),С�ڵ���(<=))\n");return false;}
    if(!error.IsEmpty()) return false;
    //����С�����Ƿ�ƥ��
	char    rel_str[MAX];
	char    st[MAX/2];
	int     top=0;
	int     sym=0;
	int     i=0;
	sprintf(rel_str,"%s",rel.GetBuffer());
	for(i=0;rel_str[i]!='\0';i++)
	{
		//����������ţ�����ջ
		if(rel_str[i]=='(' || rel_str[i]=='[' || rel_str[i]=='{')
		{  //����������ţ�����ջ
			top++;
			st[top]=rel_str[i];
			continue;
		}

		//С����
		if(rel_str[i]==')')   //С����
		{  
			if(st[top]=='(')   //���
			{
				top--;         //��ջ
				continue;
			}
			else               //����ԣ�����0
			{
				error.AppendFormat("С���Ų����\n");
				return false;    
			}
		}
		//������
		if(rel_str[i]==']')    
		{  
			if(st[top]=='[')   //���
			{
				top--;         //��ջ
				continue;
			}
			else 
			{
				error.AppendFormat("�����Ų����\n");
				return false;    //����ԣ�����0
			}
		}
		//������
		if(rel_str[i]=='}')    
		{  
			if(st[top]=='{')   //���
			{
				top--;         //��ջ
				continue;
			}
			else 
			{
				error.AppendFormat("�����Ų����\n");
				return false;    //����ԣ�����0
			}
		}
		//
		if (rel_str[i]=='=' || rel_str[i]=='>' || rel_str[i]=='<')
		{
			top++;
			sym++;
			st[top]=rel_str[i];
		}
		//�����
		if(rel_str[i]=='+' || rel_str[i]=='-' || rel_str[i]=='*' || rel_str[i]=='/' || rel_str[i]=='=' || rel_str[i]=='>' || rel_str[i]=='<' || rel_str[i]=='.') 
		{   //�������������������
			if(rel_str[i+1]=='+' || rel_str[i+1]=='-' || rel_str[i+1]=='*' ||rel_str[i+1]=='/'|| rel_str[i+1]=='=' || rel_str[i+1]=='>' || rel_str[i+1]=='<' || rel_str[i+1]=='.')
			{
				error.Format("%s�����������:%c%c\n",error,rel_str[i],rel_str[i+1]);
				return false;  //���󣬷���0
			}
			else
				continue;    //��������������
		}
	}
	//�жϳ��ˣ�><֮�������Ƿ����
	if ((top-sym)!=0)
	{
		error.AppendFormat("���Ų����,����\n");
		return false;
	}
	//����ϵ��β�Ƿ�Ϊ�����

	if (rel_str[0]=='+' || rel_str[0]=='-' || rel_str[0]=='*' || rel_str[0]=='/' || rel_str[0]=='=' || rel_str[0]=='>' || rel_str[0]=='<' || rel_str[0]=='.')
		error.Format("%s��ϵʽ��ͷ����Ϊ:%c",error,rel_str[0]);
	i=rel.GetLength()-1;
	if(rel_str[i]=='+' || rel_str[i]=='-' || rel_str[i]=='*' || rel_str[i]=='/' || rel_str[i]=='=' || rel_str[i]=='>' || rel_str[i]=='<' || rel_str[i]=='.')
		error.Format("%s��ϵʽĩβ����Ϊ:%c",error,rel_str[i]);
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
//                   ���������ʽֵ����
//
//              ���ϳ���ѧԺ  ��Ϣ������ѧϵ  
//                        �ƽ���
//                 jiangfenghuang@msn.com
//                 jiangfenghuang@163.net
//                       2003.4.5           
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
// ���ܣ�
//      �����ַ����е��������ʽ��ֵ��
//   
// �ص㣺
//      1.֧���ַ����к��и��ֳ��ú�������"7.5+sin(6*ln(8))/exp(5)"
//      2.���кܺõľ����������ܼ������ʽ�������Ƿ���ԡ��⺯����
//        ����ȷ
//      3.����������ܼ��������쳣�������Ϊ0����������sqrt(x)��x<0
//        �����Һ���acos(x)�е�x<-1��x>1��
//      4.Ϊ֧�ֻ��֡��󷽳̣��������ʽ�пɺ���x������ʱ�������еĳ�
//        Ա����xx���� 
//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////// 
//
// ʹ�÷�����
//      1. ����һ��CMathString �Ķ�����Ҫ����������ʽ�����ʼ��
//         �磺 char *s1="7.5+sin(6)/exp(5)";
//              char *s2="4*pow(x,2)+2*x+4";
//              CMathString  mathstr1(s1);
//              CMathString  mathstr2(s2);
//        �����Initial(char *s)��Initial(char *s,double x)�����ʼ��
//      2.����checkString���������ʽ�Ƿ�Ϸ�
//      3.���������ֻ�j�ⷽ�̣�����setX(double x)���ñ��ʽ��x��ֵ
//      4.����stringToBolan()���������ʽת��Ϊ�������ʽ
//      5.����compvalue()�Բ������ʽ���м��㣬������ֵΪ0�����
//        ��getvalue()ȡ�ü�����������ɸ��ݷ���ֵ�жϳ���ط�
//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//
//                ֧�ֵĺ����ı�ż�����
//
// ���     ����         ����         ���õ�math.h�⺯��     ��ע
//
//   1      abs(x)      ȡ����ֵ|x|    fabs(x)      
//   2      acos(x)     �����Һ���     acos(x)            -1<=x<=1
//   3      asin(x)     �����Һ���     asin(x)            -1<=x<=1
//   4      atan(x)     �����к���     atan(x)
//   5      acot(x)     �����к���     PI/2-atan(x)
//   6      cos(x)      ���Һ���       cos(x)
//   7      cosh(x)     ˫�����Һ���   cosh(x)
//   8      cot(x)      ���к���       1/tan(x)            tan(x)!=0
//   9      exp(x)      e��x�η�       exp(x)
//  10      floor(x)    ��x��          floor(x)   �󲻴���x��������� 
//  11      mod(x,y)     x%y           mod(x,y)             y!=0
//  12      ln(x)       ȡ��Ȼ����     log(x)               x>0
//  13      log(x)      ȡ10�Ķ���     log10(x)             x>0
//  14      pow(x,y)    x��y�η�       pow(x,y)
//  15      sin(x)      ���Һ���       sin(x)
//  16      sinh(x)     ˫�����Һ���   sinh(x)
//  17      sqrt(x)     ��x����        sqrt(x)            x>=0
//  18      tan(x)      ���к���       tan(x)
//  19      tanh(x)     ˫�����к���   tanh(x)
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


CMathString::CMathString(char *str)  //���캯�������������ʽstr����String
{
	strcpy(string,str);            
}

CMathString::CMathString(char *str,double x)
{    //���ʽ�д�δ֪���Ĺ��캯��
	//�������ʽ*s����string��δ֪��x����xx
	//���ڻ��ֱ��ʽ
	strcpy(string,str);
	xx=x;
}

void CMathString::setX(double x)      //���ó�Ա����xx��ֵ
{
	xx=x;
}

int CMathString::seekStr(char *str,int &i, char *s)
{   //����ַ���str�ĵ�i��λ�ÿ�ʼ�ĺ����ַ��Ƿ����ַ���s��ƥ��
	//�ǵĻ���i=i+len(s),������1�����򷵻�0
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

void CMathString::Initial(char *s)  //��ʼ�����������������ʽs����String
{
	strcpy(string,s); 

}

void CMathString::Initial(char *str, double x)
{
	//���ʽ�д�δ֪���ĳ�ʼ������
	//�������ʽ*str����string��δ֪��x����xx
	//���ڻ��ֱ��ʽ
	strcpy(string,str);
	xx=x;

}

int CMathString::checkString()
{ //�������ı��ʽ�Ƿ���ȷ�����������Ƿ���ԡ��⺯���Ƿ���ȷ��
	//������С�����Ƿ���ȷ������ȷ�������ʽ�еĴ������Ÿó�С����
	//���ڱ��ʽβ�ӡ�#����ʶ
	//����ȷ������ֵΪ1�����򷵻�0��
	//����ֱ��ʽ����x�����Ա��ʽ�е�x��Ϊ�ַ�����
	char st[MAX/2];        //������ŵ�ջ
	int top=0,     //ջ��
		i;
	for(i=0;string[i]!='\0';i++)
	{
		//����������ţ�����ջ
		if(string[i]=='(' || string[i]=='[' || string[i]=='{')
		{  //����������ţ�����ջ
			top++;
			st[top]=string[i];
			continue;
		}

		//С����
		if(string[i]==')')   //С����
		{  
			if(st[top]=='(')   //���
			{
				top--;         //��ջ
				continue;
			}
			else return 0;    //����ԣ�����0
		}
		//������
		if(string[i]==']')    
		{  
			if(st[top]=='[')   //���
			{
				top--;         //��ջ
				continue;
			}
			else return 0;    //����ԣ�����0
		}
		//������
		if(string[i]=='}')    
		{  
			if(st[top]=='{')   //���
			{
				top--;         //��ջ
				continue;
			}
			else return 0;    //����ԣ�����0
		}

		//�����
		if(string[i]=='+' || string[i]=='-' || string[i]=='*' || string[i]=='/')
		{   //�������������������
			if(string[i+1]=='+' || string[i+1]=='-' || string[i+1]=='*' ||string[i+1]=='/')
				return 0;  //���󣬷���0
			else
				continue;  //��������������
		}

		//���֡�С���㡢����
		if(('0'<=string[i] && string[i]<='9') || string[i]=='.'
			||string[i]==',')
			continue;

		//δ֪��x
		if(string[i]=='x')
			continue;

		//����a��ͷ�ĺ���
		if(string[i]=='a')
		{
			if(seekStr(string,i,"abs")||seekStr(string,i,"acos")||
				seekStr(string,i,"asin")||seekStr(string,i,"atan")||
				seekStr(string,i,"acot"))//����a��ͷ�Ŀ⺯��
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //���������������
					st[++top]=string[i];  // ������ջ  
					continue;
				}
				else //���������������,����0
					return 0;
			}
			else    //������a��ͷ�Ŀ⺯��,����0
				return 0;
		}

		//����c��ͷ�ĺ���
		if(string[i]=='c')
		{
			if(seekStr(string,i,"cos")||seekStr(string,i,"cosh")||
				seekStr(string,i,"cot"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //���������������
					st[++top]=string[i];  // ������ջ  
					continue;
				}
				else //���������������,����0
					return 0;
			}
			else    //������c��ͷ�Ŀ⺯��,����0
				return 0;
		}
		//����e��ͷ�ĺ���
		if(string[i]=='e')
		{
			if(seekStr(string,i,"exp"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //���������������
					st[++top]=string[i];  // ������ջ  
					continue;
				}
				else //���������������,����0
					return 0;
			}
			else    //������e��ͷ�Ŀ⺯��,����0
				return 0;
		}
		//����f��ͷ�ĺ���
		if(string[i]=='f')
		{
			if(seekStr(string,i,"floor"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //���������������
					st[++top]=string[i];  // ������ջ  
					continue;
				}
				else //���������������,����0
					return 0;
			}
			else    //������f��ͷ�Ŀ⺯��,����0
				return 0;
		}
		//����m��ͷ�ĺ���
		if(string[i]=='m')
		{
			if(seekStr(string,i,"mod"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //���������������
					st[++top]=string[i];  // ������ջ  
					continue;
				}
				else //���������������,����0
					return 0;
			}
			else    //������m��ͷ�Ŀ⺯��,����0
				return 0;
		}
		//����l��ͷ�ĺ���
		if(string[i]=='l')
		{
			if(seekStr(string,i,"ln")||seekStr(string,i,"log"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //���������������
					st[++top]=string[i];  // ������ջ  
					continue;
				}
				else //���������������,����0
					return 0;
			}
			else    //������l��ͷ�Ŀ⺯��,����0
				return 0;
		}
		//����p��ͷ�ĺ���
		if(string[i]=='p')
		{
			if(seekStr(string,i,"pow"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //���������������
					st[++top]=string[i];  // ������ջ  
					continue;
				}
				else //���������������,����0
					return 0;
			}
			else    //������p��ͷ�Ŀ⺯��,����0
				return 0;
		}
		//����s��ͷ�ĺ���
		if(string[i]=='s')
		{
			if(seekStr(string,i,"sin")||seekStr(string,i,"sinh")||
				seekStr(string,i,"sqrt"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //���������������
					st[++top]=string[i];  // ������ջ  
					continue;
				}
				else //���������������,����0
					return 0;
			}
			else    //������s��ͷ�Ŀ⺯��,����0
				return 0;
		}
		//����t��ͷ�ĺ���
		if(string[i]=='t')
		{
			if(seekStr(string,i,"tan")||seekStr(string,i,"tanh"))
			{
				if(string[i]=='('||string[i]=='['||string[i]=='{')
				{   //���������������
					st[++top]=string[i];  // ������ջ  
					continue;
				}
				else //���������������,����0
					return 0;
			}
			else    //������t��ͷ�Ŀ⺯��,����0
				return 0;
		}

		else // ���string[i]�������ַ�,�򷵻�0
			return 0;
	}//for(i=0;string[i]!='\0';i++)

	if(top!=0)       //���Ų�ƥ��,����0
		return 0;

	//���ʽ��ȷ
	//�ڱ��ʽβ�ӡ�#����ʶ
	string[i]='#';
	string[++i]='\0';

	//�����ʽ�еĴ������Ÿó�С����
	for(i=0;string[i]!='\0';i++)
	{
		if((string[i]=='[')||(string[i]=='{'))
			string[i]='(';
		else if((string[i]==']')||(string[i]=='}'))
			string[i]=')';
	}
	return 1;	
}

//���������ʽstringת���ɲ������ʽ��
//�������ʽ����bolan[]��
//ת��������
//1.�ж�string[i]Ϊ��ֵ��ֱ�Ӵ���bolan������
//2.�ж�string[i]Ϊ'(',����'('ѹ��ջ,
//  ��'('ǰΪ���֣�����'('ǰ��һ��'*'�Ĵ���
//3.�ж�string[i]Ϊ')',��ջstact��������'('�Ժ��������ͺ�������
//  ����,��������bolan��,Ȼ��������'('����
//4.�ж�string[i]Ϊ'-','+',�򽫵�ǰջstack��������'('�Ժ�����'('��
//  ��ջstack�����У���������ͺ������ε�������������bolan��,Ȼ��
//  '-'��'+'ѹ��ջstack��
//  ���'-'�ڵ�һ��λ�û���ǰ��'('����'-'�Ǹ��ţ���'~'��ȡ����ѹ��stack��
//5.�ж�string[i]Ϊ'*'��'/',��ջstact�ж���������'*'��'/'��������
//  ��������������bolan��,Ȼ��'*'��'/'ѹ��ջstact��
//6.�ж�string[i]Ϊ�������򽫸ú�����ջstact��
//  ����ջstact�б���func���Ǹú����ı�ţ���ϸ��ż�ͷ�ļ�
//  ���ú���ǰΪ���֣����ڸú�����ջstactǰ��һ�ζ�'*'�Ĵ���
//7.�ж�string[i]=',',��ջstact��'('���������ͺ���ȫ��������ѹ����
//  ��bolan�У�
//8.�ж�string[i]='x',��������δ֪������xx���棬ѹ������bolan�� 
//9.ת������ʱ����ջstact��Ϊ�գ���ջ������������ͺ�������������
//  ����bolan��
//10.ת��������������bolanβ��һopera='#'��Ϊ������
int CMathString::stringToBolan()
{
	Bolan stack[MAX];         //���������ͺ�����ջ
	int top=0,         //���ջstact���±�
		i,           //string���±�
		j=0,           //bolan������±�
		t;
	char digitStr[50];     //���Ҫת������ֵ���ַ���

	for(i=0;string[i]!='#';i++)
	{
		//�ж�Ϊ��ֵ��ֱ�Ӵ���bolan������
		if(('0'<=string[i] && string[i]<='9')||string[i]=='.') //��ֵ
		{
			t=0;
			digitStr[t]=string[i];   //���ִ���Ҫת������ֵ���ַ���
			while(('0'<=string[i+1] && string[i+1]<='9')
				||string[i+1]=='.')  
			{                       //������������Ҫת������ֵ���ַ���
				digitStr[++t]=string[i+1];
				i++;
			}
			digitStr[++t]='\0';
			//�������ַ���ת������ֵ,������bolan����
			bolan[j].flag=0;    //��ʶ�ýڵ�Ϊ��ֵ
			bolan[j].value=stringToDigital(digitStr);
			j++;
			continue;
		}

		//�ж�Ϊ'(',����'('ѹ��ջ
		//��'('ǰΪ���֣�����'('ǰ��һ��'*'�Ĵ���
		if(string[i]=='(')
		{
			if(string[i-1]<='0' || '9'<=string[i-1])//'('ǰ��Ϊ����
			{
				top++;
				stack[top].flag=1;  //��ʶ�ýڵ�Ϊ�����
				stack[top].oper='(';
				continue;
			}
			else      //'('ǰΪ����
			{         //��һ�ζ�'*'�Ĵ���
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//��'*'ѹ��ջstact��
				top++;
				stack[top].flag=1;
				stack[top].oper='*';

				//��'('ѹ��ջstact��
				top++;
				stack[top].flag=1;
				stack[top].oper='(';
				continue;
			}
		}

		//�ж�Ϊ')',��ջstact��������'('�Ժ��������ͺ������ε���
		//��������bolan��,Ȼ��������'('����
		if(string[i]==')')
		{
			while(stack[top].oper!='(')
			{
				bolan[j]=stack[top];
				j++;
				top--;
			}
			top--;  //��������'('����
			continue;
		}

		//�ж�Ϊ'-','+',�򽫵�ǰջstack��������'('�Ժ�����'('��
		//ջstack�����У���������ͺ������ε�������������bolan��,
		//Ȼ��'-'��'+'ѹ��ջstack��
		//���'-'�ڵ�һ��λ�û���ǰ��'('����'-'�Ǹ��ţ���'~'ѹ��stack��
		if(string[i]=='+' || string[i]=='-')
		{
			if(string[i]=='-'&&(i==0 || string[i-1]=='(')) //'-'�Ǹ���
			{
				top++;
				stack[top].flag=1;
				stack[top].oper='~';
				continue;
			}
			else             //'-','+'�ǼӼ�����
			{
				while(stack[top].oper!='(' && top!=0)
				{
					bolan[j]=stack[top]; //��ǰջstack�е��������
					//�������ε�����������bolan��
					j++;
					top--;
				}
				//��'-'��'+'ѹ��ջstack��
				top++;
				stack[top].flag=1;
				stack[top].oper=string[i];
				continue;
			}
		}

		//�ж�string[i]Ϊ'*'��'/',��ջstact�ж���������'*'��'/'��
		//�����ε�������������bolan��,Ȼ��'*'��'/'ѹ��ջstact��
		if(string[i]=='*' || string[i]=='/')
		{
			while(stack[top].oper=='*' || stack[top].oper=='/'||
				stack[top].flag==2)
			{
				bolan[j]=stack[top];
				j++;
				top--;
			}
			//��'*'��'/'ѹ��ջstact��
			top++;
			stack[top].flag=1;
			stack[top].oper=string[i];
			continue;
		}

		//�ж�string[i]Ϊ�������򽫸ú�����ջstact��
		//����ջstact�б���func���Ǹú����ı�ţ���ϸ��ż�ͷ�ļ�
		//���ú���ǰΪ���֣����ڸú�����ջstactǰ��һ�ζ�'*'�Ĵ���
		if(string[i]=='a')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //�ú���ǰΪ����
			{              //�ڸú�����ջstactǰ��һ�ζ�'*'�Ĵ���
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//��'*'ѹ��ջstact��
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}

			//Ϊabs(x)����
			if(seekStr(string,i,"abs"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=1;
				i--;                    //��Ϊ��seekStr������
				continue;               //i=i+strlen("abs");
				//i��ָ����abs����һ���ַ�
				//����for�������i++,���Դ˴�
				//iҪ��1
			}
			//Ϊacos(x)����
			if(seekStr(string,i,"acos"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=2;
				i--;                     
				continue;  
			}
			//Ϊasin(x)����
			if(seekStr(string,i,"asin"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=3;
				i--;                     
				continue;  
			}
			//Ϊatan(x)����
			if(seekStr(string,i,"atan"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=4;
				i--;                     
				continue;  
			}
			//Ϊacot(x)����
			if(seekStr(string,i,"acot"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=5;
				i--;                     
				continue;  
			}
		}//if(string[i]=='a')
		//string[i]Ϊ��c��ͷ�Ŀ⺯��
		if(string[i]=='c')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //�ú���ǰΪ����
			{              //�ڸú�����ջstactǰ��һ�ζ�'*'�Ĵ���
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//��'*'ѹ��ջstact��
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//Ϊcos(x)����
			if(seekStr(string,i,"cos"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=6;
				i--;                     
				continue;  
			}
			//Ϊcosh(x)����
			if(seekStr(string,i,"cosh"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=7;
				i--;                     
				continue;  
			}
			//Ϊcot(x)����
			if(seekStr(string,i,"cos"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=8;
				i--;                     
				continue;  
			}
		}//if(string[i]=='c')
		//string[i]Ϊ��e��ͷ�Ŀ⺯��
		if(string[i]=='e')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //�ú���ǰΪ����
			{              //�ڸú�����ջstactǰ��һ�ζ�'*'�Ĵ���
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//��'*'ѹ��ջstact��
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//Ϊexp(x)����
			if(seekStr(string,i,"exp"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=9;
				i--;                     
				continue;  
			}
		}//if(string[i]=='e')
		//string[i]Ϊ��f��ͷ�Ŀ⺯��
		if(string[i]=='f')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //�ú���ǰΪ����
			{              //�ڸú�����ջstactǰ��һ�ζ�'*'�Ĵ���
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//��'*'ѹ��ջstact��
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//Ϊexp(x)����
			if(seekStr(string,i,"floor"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=10;
				i--;                     
				continue;  
			}
		}//if(string[i]=='f')
		//string[i]Ϊ��m��ͷ�Ŀ⺯��
		if(string[i]=='m')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //�ú���ǰΪ����
			{              //�ڸú�����ջstactǰ��һ�ζ�'*'�Ĵ���
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//��'*'ѹ��ջstact��
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//Ϊmod(x)����
			if(seekStr(string,i,"mod"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=11;
				i--;                     
				continue;  
			}
		}//if(string[i]=='m')
		//string[i]Ϊ��l��ͷ�Ŀ⺯��
		if(string[i]=='l')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //�ú���ǰΪ����
			{              //�ڸú�����ջstactǰ��һ�ζ�'*'�Ĵ���
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//��'*'ѹ��ջstact��
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//Ϊln(x)����
			if(seekStr(string,i,"ln"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=12;
				i--;                     
				continue;  
			}
			//Ϊlog(x)����
			if(seekStr(string,i,"log"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=13;
				i--;                     
				continue;  
			}
		}//if(string[i]=='l')
		//string[i]Ϊ��p��ͷ�Ŀ⺯��
		if(string[i]=='p')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //�ú���ǰΪ����
			{              //�ڸú�����ջstactǰ��һ�ζ�'*'�Ĵ���
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//��'*'ѹ��ջstact��
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//Ϊpow(x)����
			if(seekStr(string,i,"pow"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=14;
				i--;                     
				continue;  
			}
		}//if(string[i]=='p')
		//string[i]Ϊ��s��ͷ�Ŀ⺯��
		if(string[i]=='s')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //�ú���ǰΪ����
			{              //�ڸú�����ջstactǰ��һ�ζ�'*'�Ĵ���
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//��'*'ѹ��ջstact��
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//Ϊsin(x)����
			if(seekStr(string,i,"sin"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=15;
				i--;                     
				continue;  
			}
			//Ϊsinh(x)����
			if(seekStr(string,i,"sinh"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=16;
				i--;                     
				continue;  
			}
			//Ϊsin(x)����
			if(seekStr(string,i,"sqrt"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=17;
				i--;                     
				continue;  
			}
		}//if(string[i]=='s')
		//string[i]Ϊ��t��ͷ�Ŀ⺯��
		if(string[i]=='t')
		{
			if('0'<=string[i-1] && string[i-1]<='9')  //�ú���ǰΪ����
			{              //�ڸú�����ջstactǰ��һ�ζ�'*'�Ĵ���
				while(stack[top].oper=='*' || stack[top].oper=='/'||
					stack[top].flag==2)
				{
					bolan[j]=stack[top];
					j++;
					top--;
				}
				//��'*'ѹ��ջstact��
				top++;
				stack[top].flag=1;
				stack[top].oper='*';
			}
			//Ϊtan(x)����
			if(seekStr(string,i,"tan"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=18;
				i--;                     
				continue;  
			}
			//Ϊtanh(x)����
			if(seekStr(string,i,"tanh"))   
			{                            //���ú�����ջstact��
				top++;
				stack[top].flag=2;
				stack[top].func=19;
				i--;                     
				continue;  
			}
		}//if(string[i]=='p')

		//�ж�string[i]=',',��ջstact��'('���������ͺ���ȫ��������
		//ѹ������bolan�У�
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

		//�ж�string[i]='x',��������δ֪������xx���棬ѹ������bolan�� 
		if(string[i]=='x')
		{
			bolan[j].flag=0;   //��ʶΪ��ֵ
			bolan[j].value=xx;
			j++;
		}
	}

	//ת������ʱ����ջstact��Ϊ�գ���ջ������������ͺ�������������
	//����bolan��
	while(top!=0)
	{
		bolan[j]=stack[top];
		j++;
		top--;
	}

	//ת��������������bolanβ��һopera='#'��Ϊ������
	bolan[j].oper='#';

	return 1;
}

//�������ַ���ת������ֵ
double CMathString::stringToDigital(char *s)
{
	double sum=0,    //ת�������ֵ
		temp,
		ten=0.1;     //С���仯ֵ
	int i,ch,
		flag=0;    //��ʶ��С������������0Ϊ����
	for(i=0;s[i]!='\0';i++)
	{
		if(s[i]!='.')
		{
			if(flag==0)  //��������
			{
				ch=s[i]-'0';
				sum=sum+ch;
				sum=sum*10;
			}
			else        //С������
			{
				ch=s[i]-'0';
				temp=ch*ten;   //С������λ
				sum=sum+temp;
				ten=ten*0.1;    //�ı�С����λ��
			}
		}
		else     //С����
		{
			sum=sum/10;
			flag=1;
		}
	}//for
	if(flag==0)    //��С������
		sum=sum/10;
	return sum;

}

//����ʽbolan��ֵ
//�������£�
//1.���ڵ�bolan[i]Ϊ��ֵ����������ջdataStact
//2.���ڵ�bolan[i]Ϊ��������������ջdataStact�������ݽ��м��㣬��
//  �����ѹ������ջdataStact��
//3.���ڵ�bolan[i]Ϊ�������������ջdataStact�������ݣ�������Ӧ�Ŀ�
//  �������м��㣬�������ѹ������ջdataStact��
//4.���ڵ�bolan[i]Ϊ������'#',������ջdataStact�е����ݵ���������
//  result��������0
//����ֵ:
//1. ������ȷ������0
//2. �ڼ�����������Ϊ0������1
//3. �����Һ���acos(x)�е�x����������������2
//4. �����Һ���asin(x)�е�x����������������3
//5. ���к���cot(x) ��tan��x��Ϊ0������4
//6.ȡģx%y����mod(x,y)��yΪ0������5
//7.��Ȼ��������ln(x),���x<=0,�򷵻�6
//8.ȡ10�Ķ�������log10(x),���x<=0,�򷵻�7
//9.��������sqrt(x),���x<0,�򷵻�8
//10.���������������󣬷���-1
int CMathString::compvalue(void)
{
	double  dataStack[MAX/2];   //����м����ݵ�����ջ
	int top=0,            //����ջdataStact��ջ��
		i;              //����bolan���±�
	for(i=0;bolan[i].oper!='#';i++)
	{
		//�ڵ�bolan[i]Ϊ��ֵ����������ջdataStact
		if(bolan[i].flag==0)
		{
			top++;
			dataStack[top]=bolan[i].value;
			continue;
		}

		//�ڵ�bolan[i]Ϊ��������������ջdataStact�������ݽ��м��㣬��
		//�����ѹ������ջdataStact��
		//�ڼ�����������Ϊ0������1
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
				else  //����Ϊ0������1
				{
					return 1;
				}
			case '~':      //ȡ��
				dataStack[top]=0-dataStack[top];
				break;
			}//switch
			continue;
		}//if(bolan[i].flag==1)

		//���ڵ�bolan[i]Ϊ�������������ջdataStact�������ݣ�������Ӧ
		//�Ŀ⺯�����м��㣬�������ѹ������ջdataStact��
		if(bolan[i].flag==2)
		{
			switch(bolan[i].func)
			{
			case 1:    //�����ֵ����abs(x)
				dataStack[top]=fabs(dataStack[top]);
				break;

			case 2:    //�����Һ���acos(x)
				if(-1.0<=dataStack[top] && dataStack[top]<=1.0)
				{
					dataStack[top]=acos(dataStack[top]);
					break;
				}
				else  //�����Һ���acos(x)�е�x����������������2
				{
					return 2;
				}

			case 3:   //�����Һ���asin(x)
				if(-1<=dataStack[top] && dataStack[top]<=1)
				{
					dataStack[top]=asin(dataStack[top]);
					break;
				}
				else  //�����Һ���asin(x)�е�x����������������3
				{
					return 3;
				}

			case 4:   //�����к���atan(x)
				dataStack[top]=atan(dataStack[top]);
				break;

			case 5:  //�����к���acot(x)
				dataStack[top]=PI/2-atan(dataStack[top]);
				break;

			case 6:   //���Һ���cos(x)
				dataStack[top]=cos(dataStack[top]);
				break;

			case 7:    //˫������cosh(x)
				dataStack[top]=cosh(dataStack[top]);
				break;

			case 8:    //���к���cot(x)   cot(x)=1/tan(x)
				if(tan(dataStack[top])!=0)
				{
					dataStack[top]=1/tan(dataStack[top]);
					break;
				}
				else    //���к���cot(x) ��tan��x��Ϊ0������4
				{
					return 4;
				}

			case 9:    //e��x�η�����exp(x)
				dataStack[top]=exp(dataStack[top]);
				break;

			case 10:   //�󲻴���x��������� floor(x)
				dataStack[top]=floor(dataStack[top]);
				break;

			case 11:   //ȡģx%y����mod(x,y),��
				if(dataStack[top]!=0)
				{
					dataStack[top-1]=fmod(dataStack[top-1],dataStack[top]);
					top--;
					break;
				}
				else   //ȡģx%y����mod(x,y)��yΪ0������5
				{
					return 5;
				}

			case 12:    //��Ȼ��������ln(x),���x<=0,�򷵻�6
				if(dataStack[top]>0)
				{          //ln(x)�Ŀ⺯��Ϊlog(x)
					dataStack[top]=log(dataStack[top]);
					break;
				}
				else
				{
					return 6;
				}

			case 13:    //ȡ10�Ķ�������log10(x),���x<=0,�򷵻�7
				if(dataStack[top]>0)
				{          //log10(x)�Ŀ⺯��Ϊlog10(x)
					dataStack[top]=log10(dataStack[top]);
					break;
				}
				else
				{
					return 7;
				}

			case 14:    //x��y�η�����pow(x,y)
				dataStack[top-1]=pow(dataStack[top-1],dataStack[top]);
				top--;
				break;

			case 15:    //���Һ���sin(x)
				dataStack[top]=sin(dataStack[top]);
				break;

			case 16:    //˫������sinh(x)
				dataStack[top]=sinh(dataStack[top]);
				break;

			case 17:   //��������sqrt(x),���x<0,�򷵻�8
				if(dataStack[top]>=0)
				{
					dataStack[top]=sqrt(dataStack[top]);
					break;
				}
				else
				{
					return 8;
				}

			case 18:    //���к���tan(x)
				dataStack[top]=tan(dataStack[top]);
				break;

			case 19:    //˫�����к���tanh(x)
				dataStack[top]=tanh(dataStack[top]);
				break;
			}//switch(bolan[i].func)
			continue;
		}//if(bolan[i].flag==2)
	}//for(i=0;bolan[i].oper!='#';i++)
	if(top==1)       //������ȷ
	{
		result=dataStack[top];
		return 0;      //����0
	}
	else               //���������������󣬷���-1
		return -1;   
}

double CMathString::getvalue(void)  //���ؼ���õ��ı��ʽֵ
{
	return result;
}


/************************************************************************/
/* һ��ʽ���ֵ                                                                     */
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
/* �ж��Ƿ�Ϊ��ֵ                                                       */
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
/* �����ڷ��ʳ�ģ�͵ĳߴ�Ͳ���֮�����ִ��                             */
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
		str="����û�еȺ�";
		strError+=str;
		return FALSE;
	}

	if(min==0)
	{
		str="���̵Ⱥ�ǰ����������";
		strError+=str;
		return FALSE;
	}

	if(min==str.GetLength()-1)
	{
		str="���̵Ⱥź�����������";
		strError+=str;
		return FALSE;
	}
	left=str.Left(min);
	right=str.Mid(min+1);
	min=right.Find("=");
	if(min!=-1)
	{
		str="���̵Ⱥ�����������";
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
			str="���Ų�ƥ��";
			strError+=str;
			return FALSE;
		}
		sub=str.Mid(min+1,rIndex-min+1-2);///����������ַ���
		if(CheckExpression(sub,importDim,exportDim,strError)==FALSE) return FALSE;
		sub=str.Mid(min,rIndex-min+1);///�������ŵ��ַ���
		///��������,����
		CString str1,str3;
		str1=str.Mid(0,min);///���ǰ��α��ʽ,����������.
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
				str="����ǰ��Ҫ���������";
				strError+=str;
				return FALSE;
			}
		}
		////�����������
		str.Replace(sub,"1.23456789");
		return CheckExpression(str,importDim,exportDim,strError);
	}
	min=0;
	str=Expression.Tokenize("*+/-",min);
	if(str.GetLength()!=min-1)
	{
		str=""+str+"  ǰ���ص��������";
		str.Replace("1.23456789","����");
		strError+=str;
		return FALSE;
	}
	///�Ƿ�ʵ��
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
				str="��  "+str+"   ������ִ���,������ź��������������";
				str.Replace("1.23456789","����");
				strError+=str;
				return FALSE;
			}
			str=Expression.Mid(min);
			return CheckExpression(str,importDim,exportDim,strError);
		}
	}

	/////////////�Ƿ�ߴ�������ʶ
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
	if(str=="PI"||str=="E")/////�жϳ���
		nFlag=TRUE;
	if(nFlag==FALSE) 
	{
		min=str.Find("1.23456789");
		if(min!=-1)
			str="�����������,��ƥ��";
		else
			str="�Ҳ���"+str+"��һ���������Ͳ�Ϊ��ֵ��,����";
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
			str="��  "+str+"   ������ִ���,������ź��������������";
			str.Replace("1.23456789","����");
			strError+=str;
			return FALSE;
		}
		str=Expression.Mid(min);
		return CheckExpression(str,importDim,exportDim,strError);
	}
	return TRUE;
}
/************************************************************************/
/* �ڶ����ж��Ƿ�Ϊʵ���ķ���                                           */
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
//��ù�ϵʽ���ұߵ�����ʽ,�����ִ������
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
//��ù�ϵʽ����ߵ�����ʽ,�����ִ���
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

