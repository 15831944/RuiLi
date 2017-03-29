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
//检查一个具有模型的零件
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
	//初始化
	InitReValue();
	//获得模型的事务特性表
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
	//获得现有值
	for (int i=0;i<p_DimParaItemList.GetCount();i++)
	{
	    ProStringToWstring(IdNumName,p_DimParaItemList[i].IdNum.GetBuffer());
		switch(p_DimParaItemList[i].type) {
		case TYPE_DIM:
			//尺寸不存在
			if(GetDimPara(solid,IdNumName,&p_DimParaItemList[i].Dim)) 
			{
				error_info.Format("尺寸:%s(%s),已经不存在",p_DimParaItemList[i].Name,p_DimParaItemList[i].IdNum.GetBuffer());
				return CHECK_DIM_OR_PARA_WAS_DELTETED;
			}
			p_DimParaItemList[i].nValue=UsrGetDimParaValue(&p_DimParaItemList[i].Dim,NULL);
			break;
		default:
			if(GetDimPara(solid,IdNumName,NULL,&p_DimParaItemList[i].Para)) 
			{
				error_info.Format("参数:%s(%s),已经不存在",p_DimParaItemList[i].Name,p_DimParaItemList[i].IdNum.GetBuffer());
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
		//判断零件有没有修改
		m_PrtInsValSet.Open();
		for (int i=0;i<p_DimParaItemList.GetCount();i++)
		{
			//PrtNum零件号（并非零件模型号）
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
				error_info.Format("零件的模型%s与零件的可变参数值表不一致",GetMdlFileName(solid));
			}
		}	
		return CHECK_TRUE;
		//查询模型实例库
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
					//PrtNum零件号（并非零件模型号）				
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
				//得到了一个所有都相同的值
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
//检查一个没有模型的零件
REVTYPE CCheckMdlIfHasChange::CheckPartNoPrtMdl(CDatabase * p_db,ProSolid solid, long PrtNum,int CheckType)
{
	return CHECK_FALSE;
}
//检查一个具有模型的部件
//检查的过程
//1.检查组成情况、组件内的零部件的文件名称和零件号
//2.检查装配位置尺寸
//3.检查零件的尺寸是否有改变
REVTYPE CCheckMdlIfHasChange::CheckAsmHasAsmMdl(CDatabase * p_db,ProSolid solid, long AsmNum,int CheckType)
{
	UsrPartPorp								temp_porp;
	CNProductInsInfoTable					m_AsmInsSet(p_db);			//组件实例信息表
	CNProductInsVariantTable				m_AsmInsValSet(p_db);		//组件实例值表
	CAsmVarSet					            m_AsmValInfoSet(p_db);		//组件模型值表
	CAsmManageTableSet						m_AsmInfoSet(p_db);			//组件模型信息表
	/*ProName									IdNumName;*/
	CString									sqlFilter;
	CArray<ProSolid,ProSolid>				solidlist;
	CArray<dim_para_item,dim_para_item>     p_DimParaItemList;
	temp_porp=GetMdlInfo(solid);
	//获得模型的事务特性表
	//1.检查组件的组成情况
	GetAsmCompHand(solid,&solidlist);
	for (int i=0;i<solidlist.GetCount();i++)
	{
		UsrPartPorp  temp_porp2;		
		switch(GetMdlType(solidlist[i])) {
		case PRO_MDL_PART:
			temp_porp2=GetMdlInfo(solidlist[i]);
            if (temp_porp2.MdlType==1)     //有模型的实例
            {
            }
			break;
		case PRO_MDL_ASSEMBLY:
			temp_porp2=GetMdlInfo(solidlist[i]);
			if (temp_porp2.MdlType==1)    //有模型的实例
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
//检查一个没有模型的部件
REVTYPE CCheckMdlIfHasChange::CheckAsmNoAsmMdl(CDatabase * p_db,ProSolid solid, long AsmNum,int CheckType)
{
	return CHECK_FALSE;
}
//检查零件
REVTYPE CCheckMdlIfHasChange::CheckPart(CDatabase * p_db,ProSolid solid,long PrtNum,int CheckType)
{
	return CHECK_FALSE;
}
//检查部件
REVTYPE CCheckMdlIfHasChange::CheckAsm(CDatabase * p_db,ProSolid solid,long AsmNum,int CheckType)
{
	return CHECK_FALSE;
}
//void CCheckMdlIfHasChange::GetAsmCompNumAndPostId(CDatabase * p_db,long AsmNum)
//{
//}
void CCheckMdlIfHasChange::InitReValue()
{
	p_ReDimList.RemoveAll();         //已经修改过的尺寸
	p_ReDimYuanlaiVal.RemoveAll();   //原来的尺寸值
	p_ReParaList.RemoveAll();        //修改过的参数的值
	p_ReParaYuanlaiVal.RemoveAll();  //原来的参数值]
	p_temp_AsmMdlItemList.RemoveAll();
	error_info.Empty();
	changenum=-1;
}