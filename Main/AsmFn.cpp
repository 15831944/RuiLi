#include "stdafx.h"
#include "resource.h"
/************************************************************************/
/* 装配模式用下，用于获得元件ID表,其他地方不能用 (AsmParaDefDlg.cpp)    */
/************************************************************************/
void GetSolidIDTable(ProSolid asm_solid,CArray<ProFeature,ProFeature> * p_asm_feat_list,HTREEITEM hItem,CTreeCtrl * p_Tree,CArray<int,int> * p_id)
{
	int            indexNo=-1;
	int            postId=-1;
	ProFeattype    ftype;
	ProError       status;
	indexNo=(int)p_Tree->GetItemData(hItem);
	if (hItem!=p_Tree->GetRootItem()) {
		HTREEITEM tempItem=p_Tree->GetParentItem(hItem);
		GetSolidIDTable(asm_solid,p_asm_feat_list,tempItem,p_Tree,p_id);
	}
	else
	{
		/*ProSolidToPostfixId(asm_solid,&postId);
		p_id->Add(postId);*/
		return;
	}
	status = ProFeatureTypeGet (&p_asm_feat_list->GetAt(indexNo), &ftype);
	if (ftype==PRO_FEAT_COMPONENT) {
		//ProSolid              mdl;
		//ProAsmcompMdlGet((ProAsmcomp *)(&p_asm_feat_list->GetAt(indexNo)),(ProMdl *)&mdl);
		//ProMdlTypeGet(mdl,&type);
		/*if (type==PRO_MDL_ASSEMBLY || type==PRO_MDL_PART) {	*/	

		//ProSolidToPostfixId(mdl,&postId);
		p_id->Add(p_asm_feat_list->GetAt(indexNo).id);
		/*}*/
	}

}
/************************************************************************/
/* 判断组件中的元件是否已经被删除 ,以及特征是否被删除                   */
/************************************************************************/
BOOL CheckAsmSWTX(ProSolid asm_solid,AsmSmlItem * p_asmSwtx)
{
	ProSolid                  comp;
	CArray<ProSolid,ProSolid> p_solidlist;
	p_solidlist.Add(asm_solid);
	GetAsmCompHand(asm_solid,&p_solidlist);
	switch(p_asmSwtx->marsk) {
	case TYPE_DIM:
		comp=(ProSolid)p_asmSwtx->dim.owner;
		for (int i=0;i<(int)p_solidlist.GetCount();i++)
		{
			if (p_solidlist[i]==comp) 
			{
				UsrSWTXTable1 temp(comp,&p_asmSwtx->dim);
				if(temp.isCheck!=1) return FALSE;
				else return TRUE;
			}
		}
		break;
	case TYPE_PARA:
		comp=(ProSolid)UsrGetParameterOwner(&(p_asmSwtx->para));
		for (int i=0;i<(int)p_solidlist.GetCount();i++)
		{
			if (p_solidlist[i]==comp) 
			{
				UsrSWTXTable1 temp(comp,&p_asmSwtx->para);
				if(temp.isCheck!=1) return FALSE;
				else return TRUE;
			}
		}
		break;
	default:
		break;
	}

	return FALSE;
}
/************************************************************************/
/* 获得装配件的元件的句柄,注意：对于需要顶层句柄也加入进去，需要在调用函数前
加入，本函数只访问组件下层的句柄，并没有包括组件本省                     /
/************************************************************************/
void  GetAsmCompHand(ProSolid solid,CArray<ProSolid,ProSolid> * p_solidlist)
{
	ProError      status;
	ProMdlType    type;
	CArray<ProFeature,ProFeature> feat_list;

	status=ProSolidFeatVisit(solid,(ProFeatureVisitAction)FeatureAsmVisitAcFn
		,NULL,(CArray<ProFeature,ProFeature>*)&feat_list);

	int num=(int)feat_list.GetCount();

	for (int i=0;i<num;i++) {
		ProFeattype    ftype;
		status = ProFeatureTypeGet (&feat_list[i], &ftype);

		if (ftype == PRO_FEAT_COMPONENT){
			ProSolid              mdl;
			int                   Flag=0;
			ProAsmcompMdlGet((ProAsmcomp *)(&feat_list[i]),(ProMdl *)&mdl);
			//判断是否已经存在
			for (int j=0;j<(int)p_solidlist->GetCount();j++) {
				if(p_solidlist->GetAt(j)==mdl)
				{
					Flag=1;
					break;
				}
			}
			if (Flag==0)
			{
				ProMdlTypeGet(mdl,&type);
				switch(type) {
				case PRO_MDL_ASSEMBLY:
					p_solidlist->Add(mdl);
					GetAsmCompHand(mdl,p_solidlist);
					break;
				case PRO_MDL_PART:
					p_solidlist->Add(mdl);
					break;
				default:
					break;
					}
			}
		}
	}
}
/************************************************************************/
/* 把零件中的尺寸导入到组件中时，给尺寸后面添加后缀  
str为零件的关系式，POSTID为零件的后缀                                */
/************************************************************************/
void  ReplaceDimWithPostID(CString * str,CString postid)
{
	//CStringArray CanshuName;    //
	CString  resToken;
	BOOL     IsHave;
	int      curPos= 0;
	int      j=0;
	int      num=0;
	CStringArray CanshuName;

	resToken=str->Tokenize("=+-*/()<>",curPos);
	while (resToken !="")
	{
		if(isdigit(resToken.GetAt(0))==0)
		{

			num=(int)CanshuName.GetSize();
			if(num==0)
			{
				CanshuName.Add(resToken);
			}
			else
			{
				IsHave=false;
				for(int i=0;i<num;i++)
				{
					if(CanshuName[i]==resToken)
					{
						IsHave=true;
						break;
					}
				}
				if (!IsHave) 
				{
					CanshuName.Add(resToken); 
				}
			}

		}

		resToken=str->Tokenize("=+-*/()><",curPos);
	}
	num=(int)CanshuName.GetSize();
	for(int i=0;i<num;i++) {
		str->Replace(CanshuName[i],_T(CanshuName[i]+":"+postid));
	}

}
/************************************************************************/
/* 访问模型中所有的尺寸,参数                                            */
/************************************************************************/
void AsmSolidDimVisFn(ProSolid solid,CStringArray * dimlist )
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
			AsmSolidDimVisFn(mdl,dimlist);
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
		CString temp;
		ProName symbol;		
		ProDimensionSymbolGet(&p_DimentionList[i],symbol);
		temp.Format("%s:%d",CString(symbol),postId);
		temp.MakeLower();
		dimlist->Add(temp);
	}
	//再访问模型中的参数
	ProMdlToModelitem(solid,&modelitem);
	ProParameterVisit((ProModelitem*)&modelitem,NULL,(ProParameterAction)partAsmParaVisAc,
		(CArray<ProParameter,ProParameter>*)&Parameterlist);
	num=(int)Parameterlist.GetCount();
	for (int i=0;i<num;i++) {
		CString temp;
		temp.Format("%s:%d",CString(Parameterlist[i].id),postId);
		temp.MakeLower();
		dimlist->Add(temp);
	}
}
/************************************************************************/
/* 获得组件元件的ID表                                                   */
/************************************************************************/
bool UsrGetSolidComppath(ProSolid assmbely,ProSolid comp,CArray<int,int> & p_IDTalbe)
{
	ProError      status;
	ProMdlType    type;
	CArray<ProFeature,ProFeature> feat_list;
	status=ProSolidFeatVisit(assmbely,(ProFeatureVisitAction)FeatureAsmVisitAcFn
		,NULL,(CArray<ProFeature,ProFeature>*)&feat_list);
	for (int i=0;i<feat_list.GetCount();i++)
	{
		ProFeattype    ftype;
		status = ProFeatureTypeGet (&feat_list[i], &ftype);
		if (ftype == PRO_FEAT_COMPONENT){
			ProSolid              mdl;
			ProAsmcompMdlGet((ProAsmcomp *)(&feat_list[i]),(ProMdl *)&mdl);
			ProMdlTypeGet(mdl,&type);
			switch(type) {
			case PRO_MDL_ASSEMBLY:
				if (UsrGetSolidComppath((ProSolid)mdl,comp,p_IDTalbe))
				{
					p_IDTalbe.InsertAt(0,feat_list[i].id,1);
					return true;
				}				
				break;
			case PRO_MDL_PART:
				if (comp==(ProSolid)mdl)
				{	
					p_IDTalbe.Add(feat_list[i].id);
					return true;
				}
				break;
			default:
				break;
			}
		}
	}
	return false;
}
/************************************************************************/
/* 获得组件下所有的零件清单，没有部件                                    /
/************************************************************************/
void  GetAsmPartCompHand(ProSolid solid,CArray<ProSolid,ProSolid> * p_solidlist,CArray<int,int> * numlist)
{
	ProError      status;
	ProMdlType    type;
	CArray<ProFeature,ProFeature> feat_list;
	status=ProSolidFeatVisit(solid,(ProFeatureVisitAction)FeatureAsmVisitAcFn
		,NULL,(CArray<ProFeature,ProFeature>*)&feat_list);

	int num=(int)feat_list.GetCount();
	for (int i=0;i<num;i++) 
	{
		ProFeattype    ftype;
		status = ProFeatureTypeGet (&feat_list[i], &ftype);

		if (ftype == PRO_FEAT_COMPONENT)
		{
			ProSolid              mdl;
			int                   Flag=0;
			ProAsmcompMdlGet((ProAsmcomp *)(&feat_list[i]),(ProMdl*)&mdl);
			//判断是否已经存在
			for (int j=0;j<(int)p_solidlist->GetCount();j++) 
			{				
				if(p_solidlist->GetAt(j)==mdl)
				{
					numlist->GetAt(j)+=1;
					Flag=1;
				}
				if(Flag==1) break;
			}
			if (Flag==0)
			{
				ProMdlTypeGet(mdl,&type);
				switch(type) 
				{
				case PRO_MDL_ASSEMBLY:
					GetAsmPartCompHand(mdl,p_solidlist,numlist);
					break;
				case PRO_MDL_PART:
					numlist->Add(1);
					p_solidlist->Add(mdl);
					break;
				default:
				    break;
				}
			}
		}
	}
}