#include "StdAfx.h"
#include "checkmdlifhaschange.h"
#include "NPartInsInfoTable.h"
#include "partVariantTableSet.h"
#include "partRelationSet.h"
#include "partmdlrecordset.h"
#include "partInsValueTableSet.h"

#include "AsmManageTableSet.h"
#include "CAsmPerfSet.h"
#include "CAsmVarSet.h"
#include "NProductInsInfoTable.h"
//#include "NProductInsPerfTable.h"
#include "NProductInsVariantTable.h"
#include "NProductBOMTable.h"
CCheckMdlIfHasChange::CCheckMdlIfHasChange(void)
{
}

CCheckMdlIfHasChange::~CCheckMdlIfHasChange(void)
{
}
//���һ������ģ�͵����
REVTYPE CCheckMdlIfHasChange::CheckPartHasPrtMdl(CDatabase * p_db,ProSolid solid,long PrtNum,int CheckType)
{
	UsrPartPorp								temp_porp;
	CNPartInsInfoTable						m_PrtInsSet(p_db);
	CpartInsValueTableSet					m_PrtInsValSet(p_db);
	CpartVariantTableSet					m_PrtValInfoSet(p_db);
	CPartMdlRecordSet						m_PrtInfoSet(p_db);
	ProName									IdNumName;
	CString									sqlFilter;
	CArray<dim_para_item,dim_para_item>   p_DimParaItemList;
	temp_porp=GetMdlInfo(solid);
	//��ʼ��
	InitReValue();
	//���ģ�͵��������Ա�
    m_PrtInfoSet.m_strFilter.Format("[Number]=%ld",temp_porp.MdlNum);
	m_PrtInfoSet.Open();
	if(m_PrtInfoSet.IsBOF()) return CHECK_HAVE_NO_SWTX;
	m_PrtInfoSet.MoveFirst();
	do {
		dim_para_item             tempDimParaItem;
		tempDimParaItem.Name=m_PrtValInfoSet.m_Name;
		tempDimParaItem.Num=m_PrtValInfoSet.m_Number;
		if (m_PrtValInfoSet.m_Type=="dim")
		{
			tempDimParaItem.type=TYPE_DIM;			
			tempDimParaItem.IdNum=m_PrtValInfoSet.m_IdNum;
			tempDimParaItem.mValue=atof(m_PrtValInfoSet.m_Value);
			p_DimParaItemList.Add(tempDimParaItem);
		}
		if (m_PrtValInfoSet.m_Type=="para")
		{
			tempDimParaItem.type    =TYPE_PARA;
			tempDimParaItem.IdNum   =m_PrtValInfoSet.m_IdNum;
			tempDimParaItem.ParaType=m_PrtValInfoSet.m_SubType;

			if (m_PrtValInfoSet.m_SubType=="PRO_PARAM_DOUBLE")
			    tempDimParaItem.mValue=atof(m_PrtValInfoSet.m_Value.GetBuffer());
			else if (m_PrtValInfoSet.m_SubType=="PRO_PARAM_INTEGER")
				tempDimParaItem.mParaIntValue=atoi(m_PrtValInfoSet.m_Value.GetBuffer());
		    else if (m_PrtValInfoSet.m_SubType=="PRO_PARAM_BOOLEAN")
			    tempDimParaItem.mParaIntValue=atol(m_PrtValInfoSet.m_Value.GetBuffer());
			else if (m_PrtValInfoSet.m_SubType=="PRO_PARAM_STRING")
			    tempDimParaItem.mParaStrValue=m_PrtValInfoSet.m_Value;
			else tempDimParaItem.mParaStrValue=m_PrtValInfoSet.m_Value;

			p_DimParaItemList.Add(tempDimParaItem);
		}	
		m_PrtInfoSet.MoveNext();
	} while(!m_PrtInfoSet.IsEOF());
	//�������ֵ
	for (int i=0;i<p_DimParaItemList.GetCount();i++)
	{
	    ProStringToWstring(IdNumName,p_DimParaItemList[i].IdNum.GetBuffer());
		switch(p_DimParaItemList[i].type) {
		case TYPE_DIM:
			//�ߴ粻����
			if(GetDimPara(solid,IdNumName,&p_DimParaItemList[i].Dim)) 
			{
				error_info.Format("�ߴ�:%s(%s),�Ѿ�������",p_DimParaItemList[i].Name,p_DimParaItemList[i].IdNum.GetBuffer());
				return CHECK_DIM_OR_PARA_WAS_DELTETED;
			}
			p_DimParaItemList[i].nValue=UsrGetDimParaValue(&p_DimParaItemList[i].Dim,NULL);
			break;
		default:
			if(GetDimPara(solid,IdNumName,NULL,&p_DimParaItemList[i].Para)) 
			{
				error_info.Format("����:%s(%s),�Ѿ�������",p_DimParaItemList[i].Name,p_DimParaItemList[i].IdNum.GetBuffer());
				return CHECK_DIM_OR_PARA_WAS_DELTETED;
			}
			if (p_DimParaItemList[i].ParaType=="PRO_PARAM_DOUBLE")
				p_DimParaItemList[i].nValue=atof(UsrGetParaValue(&p_DimParaItemList[i].Para));
			else if (p_DimParaItemList[i].ParaType=="PRO_PARAM_INTEGER")
				p_DimParaItemList[i].nParaIntValue =atoi(UsrGetParaValue(&p_DimParaItemList[i].Para));
			else if (p_DimParaItemList[i].ParaType=="PRO_PARAM_BOOLEAN")
				p_DimParaItemList[i].nParaIntValue=atoi(UsrGetParaValue(&p_DimParaItemList[i].Para));
			else if (p_DimParaItemList[i].ParaType=="PRO_PARAM_STRING")
				p_DimParaItemList[i].nParaStrValue =UsrGetParaValue(&p_DimParaItemList[i].Para);
			else p_DimParaItemList[i].nParaStrValue="";

			p_DimParaItemList[i].nValue=UsrGetDimParaValue(&p_DimParaItemList[i].Dim,NULL);
			break;
		}		
	}



	switch(CheckType) {
	case 0:
		//�ж������û���޸�
		m_PrtInsValSet.Open();
		for (int i=0;i<p_DimParaItemList.GetCount();i++)
		{
			//PrtNum����ţ��������ģ�ͺţ�
			m_PrtInsValSet.m_strFilter.Format("[PrtNum]=%ld AND [Number]=%ld",temp_porp.Number,p_DimParaItemList[i].Num);
			m_PrtInsValSet.Requery();
			if (!m_PrtInsValSet.IsEOF())
			{
				if (p_DimParaItemList[i].type==TYPE_DIM)
				{
					if (p_DimParaItemList[i].nValue!=atof(m_PrtInsValSet.m_Value.GetBuffer()))
					{
						p_ReDimList.Add(p_DimParaItemList[i].Dim);
						return CHECK_DIM_HAS_CHANGED;
					}
				}else if (p_DimParaItemList[i].ParaType=="PRO_PARAM_DOUBLE")
				{
					if (p_DimParaItemList[i].nValue!=atof(m_PrtInsValSet.m_Value.GetBuffer()))
					{
						p_ReParaList.Add(p_DimParaItemList[i].Para);
						return CHECK_PARA_HAS_CHANGED;
					}
				}else if (p_DimParaItemList[i].ParaType=="PRO_PARAM_INTEGER")
				{
					if (p_DimParaItemList[i].nParaIntValue != atoi(m_PrtInsValSet.m_Value.GetBuffer()))
					{
						p_ReParaList.Add(p_DimParaItemList[i].Para);
					    return CHECK_PARA_HAS_CHANGED;
					}
				}else if (p_DimParaItemList[i].ParaType=="PRO_PARAM_BOOLEAN")
				{
					if (p_DimParaItemList[i].nParaIntValue != atoi(m_PrtInsValSet.m_Value.GetBuffer()))
					{
						p_ReParaList.Add(p_DimParaItemList[i].Para);
						return CHECK_PARA_HAS_CHANGED;
					}
				}else if (p_DimParaItemList[i].ParaType=="PRO_PARAM_STRING")
				{
					if (p_DimParaItemList[i].nParaStrValue == m_PrtInsValSet.m_Value.GetBuffer())
					{
						p_ReParaList.Add(p_DimParaItemList[i].Para);
						return CHECK_PARA_HAS_CHANGED;
					}
				}
			}
			else 
			{
				error_info.Format("�����ģ��%s������Ŀɱ����ֵ��һ��",GetMdlFileName(solid));
			}
		}	
		return CHECK_TRUE;
		//��ѯģ��ʵ����
		break;
	default:
		m_PrtInsValSet.Open();
		m_PrtInsSet.Open();
		m_PrtInsSet.m_strFilter.Format("[MdlNum]=%ld AND [PrtNum]<>%ld",temp_porp.MdlNum,temp_porp.Number);
		m_PrtInsSet.Requery();
		if (!m_PrtInsSet.IsBOF())
		{
			m_PrtInsSet.MoveFirst();
			do {
				int Flag=0;
				for (int i=0;i<p_DimParaItemList.GetCount();i++)
				{
					//PrtNum����ţ��������ģ�ͺţ�				
					m_PrtInsValSet.m_strFilter.Format("[PrtNum]=%ld AND [Number]=%ld",m_PrtInsSet.m_PrtNum,p_DimParaItemList[i].Num);
					m_PrtInsValSet.Requery();
					if (!m_PrtInsValSet.IsEOF())
					{
						if (p_DimParaItemList[i].type==TYPE_DIM)
						{
							if (p_DimParaItemList[i].nValue!=atof(m_PrtInsValSet.m_Value.GetBuffer()))
								Flag=1;
						}else if (p_DimParaItemList[i].ParaType=="PRO_PARAM_DOUBLE")
						{
							if (p_DimParaItemList[i].nValue!=atof(m_PrtInsValSet.m_Value.GetBuffer()))
								Flag=1;
						}else if (p_DimParaItemList[i].ParaType=="PRO_PARAM_INTEGER")
						{
							if (p_DimParaItemList[i].nParaIntValue != atoi(m_PrtInsValSet.m_Value.GetBuffer()))
								Flag=1;
						}else if (p_DimParaItemList[i].ParaType=="PRO_PARAM_BOOLEAN")
						{
							if (p_DimParaItemList[i].nParaIntValue != atoi(m_PrtInsValSet.m_Value.GetBuffer()))
								Flag=1;
						}else if (p_DimParaItemList[i].ParaType=="PRO_PARAM_STRING")
						{
							if (p_DimParaItemList[i].nParaStrValue == m_PrtInsValSet.m_Value.GetBuffer())
								Flag=1;
						}
					}
					else Flag=1;
				}
				//�õ���һ�����ж���ͬ��ֵ
				if(Flag==0) 
				{
					changenum=m_PrtInsSet.m_PrtNum;
					return  CHECK_TRUE;
				} 
				m_PrtInsSet.MoveNext();
			} while(!m_PrtInsSet.IsEOF());
		}
		CHECK_FALSE;
		break;
	}
	return CHECK_FALSE;
}
//���һ��û��ģ�͵����
REVTYPE CCheckMdlIfHasChange::CheckPartNoPrtMdl(CDatabase * p_db,ProSolid solid, long PrtNum,int CheckType)
{
	return CHECK_FALSE;
}
//���һ������ģ�͵Ĳ���
//���Ĺ���
//1.���������������ڵ��㲿�����ļ����ƺ������
//2.���װ��λ�óߴ�
//3.�������ĳߴ��Ƿ��иı�
REVTYPE CCheckMdlIfHasChange::CheckAsmHasAsmMdl(CDatabase * p_db,ProSolid solid, long AsmNum,int CheckType)
{
	UsrPartPorp								temp_porp;
	CNProductInsInfoTable					m_AsmInsSet(p_db);			//���ʵ����Ϣ��
	CNProductInsVariantTable				m_AsmInsValSet(p_db);		//���ʵ��ֵ��
	CAsmVarSet					            m_AsmValInfoSet(p_db);		//���ģ��ֵ��
	CAsmManageTableSet						m_AsmInfoSet(p_db);			//���ģ����Ϣ��
	/*ProName									IdNumName;*/
	CString									sqlFilter;
	CArray<ProSolid,ProSolid>				solidlist;
	CArray<dim_para_item,dim_para_item>     p_DimParaItemList;
	temp_porp=GetMdlInfo(solid);
	//���ģ�͵��������Ա�
	//1.��������������
	GetAsmCompHand(solid,&solidlist);
	for (int i=0;i<solidlist.GetCount();i++)
	{
		UsrPartPorp  temp_porp2;		
		switch(GetMdlType(solidlist[i])) {
		case PRO_MDL_PART:
			temp_porp2=GetMdlInfo(solidlist[i]);
            if (temp_porp2.MdlType==1)     //��ģ�͵�ʵ��
            {
            }
			break;
		case PRO_MDL_ASSEMBLY:
			temp_porp2=GetMdlInfo(solidlist[i]);
			if (temp_porp2.MdlType==1)    //��ģ�͵�ʵ��
			{
				CheckAsmHasAsmMdl(p_db,solidlist[i],temp_porp2.Number,0);
			}			
			break;
		default:
			break;
		}
	}
	return CHECK_FALSE;
}
//���һ��û��ģ�͵Ĳ���
REVTYPE CCheckMdlIfHasChange::CheckAsmNoAsmMdl(CDatabase * p_db,ProSolid solid, long AsmNum,int CheckType)
{
	return CHECK_FALSE;
}
//������
REVTYPE CCheckMdlIfHasChange::CheckPart(CDatabase * p_db,ProSolid solid,long PrtNum,int CheckType)
{
	return CHECK_FALSE;
}
//��鲿��
REVTYPE CCheckMdlIfHasChange::CheckAsm(CDatabase * p_db,ProSolid solid,long AsmNum,int CheckType)
{
	return CHECK_FALSE;
}
//void CCheckMdlIfHasChange::GetAsmCompNumAndPostId(CDatabase * p_db,long AsmNum)
//{
//}
void CCheckMdlIfHasChange::InitReValue()
{
	p_ReDimList.RemoveAll();         //�Ѿ��޸Ĺ��ĳߴ�
	p_ReDimYuanlaiVal.RemoveAll();   //ԭ���ĳߴ�ֵ
	p_ReParaList.RemoveAll();        //�޸Ĺ��Ĳ�����ֵ
	p_ReParaYuanlaiVal.RemoveAll();  //ԭ���Ĳ���ֵ]
	p_temp_AsmMdlItemList.RemoveAll();
	error_info.Empty();
	changenum=-1;
}