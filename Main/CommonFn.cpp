#include "stdafx.h"
#include "resource.h"
#include "UsrSWTXTable.h"
#include "Compute.h"
#include "MyDatabase.h"



ProGeomitem front, top, right,front1,edges,*p_edges,axis;

extern CMyDatabase * m_db;

/*=========================================================================*\
Function:	SgCollectFeat() ��ָ��ģ�͵�����,��������ķ��ʺ���
\*=========================================================================*/
ProError  SgCollectFeat(ProSolid solid,			//(In)ָ��ģ��
						ProFeature **p_data,BOOL nFlag)	//(In)������������
{
	ProError status;
	if( p_data != NULL )
	{
		status=ProArrayAlloc(0,sizeof(ProFeature),1,(ProArray*)p_data);
		if( status == PRO_TK_NO_ERROR )
		{  //���óߴ������ʺ���
			if(nFlag)
			{
				status=ProSolidFeatVisit (solid,
					(ProFeatureVisitAction) FeatureVisitAction,
					(ProFeatureFilterAction)FeatureFilterAction,(ProAppData)&p_data);  
			}
			else
			{
				status=ProSolidFeatVisit (solid,
					(ProFeatureVisitAction) FeatureVisitAction,
					(ProFeatureFilterAction)FeatureAsmFilterAction,(ProAppData)&p_data);  
			}
		}
		else
		{
			ProArrayFree( (ProArray*)p_data );
			*p_data = NULL;
		}
	}
	else
		status = PRO_TK_BAD_INPUTS;
	return (status);
}

/*=========================================================================*\
Function:	FeatureVisitAction() ����������ʺ����Ķ�����������
\*=========================================================================*/
ProError FeatureVisitAction(ProFeature *p_object,ProError status,
							ProAppData app_data)  

{   
	ProArray *p_array;
	p_array = (ProArray*)((ProFeature**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1,p_object );
	return (status);
}
/*=========================================================================*\
Function:FeatureFilterAction() ����������ʺ����Ĺ��˺�������
\*=========================================================================*/
ProError FeatureFilterAction(ProFeature *p_object,
							 ProAppData app_data)  
{   
	ProBoolean isVisible;
	ProFeatureVisibilityGet(p_object,&isVisible);
	if (!isVisible)
		return (PRO_TK_CONTINUE);
	else
		return (PRO_TK_NO_ERROR);
}

/*=========================================================================*\
Function: FeatureAsmFilterAction() װ��������������ʺ����Ĺ��˺�������
\*=========================================================================*/
ProError FeatureAsmFilterAction(ProFeature *p_object,
								ProAppData app_data)  
{   
	ProBoolean isVisible;
	ProFeatureVisibilityGet(p_object,&isVisible);
	ProError status;
	ProFeattype ftype;
	/*----------------------------------------------------------------*\
	Get the feature type
	\*----------------------------------------------------------------*/
	status = ProFeatureTypeGet (p_object, &ftype);
	if (ftype == PRO_FEAT_COMPONENT)
		return (PRO_TK_NO_ERROR);
	//if (!isVisible)
	return (PRO_TK_CONTINUE);
	//else
	//return (PRO_TK_NO_ERROR);
}
/*================================================================*\
FUNCTION: FeatureCollectDim()
�Զ��庯����������õ�ǰ�����ĳߴ����ָ�룬��������ָ��������
\*================================================================*/
ProError  FeatureCollectDim(ProFeature feature,			//(In)��ǰģ��
							ProDimension **p_data)	
{
	ProError status;
	if( p_data != NULL )
	{
		status=ProArrayAlloc(0,sizeof(ProDimension),1,(ProArray*)p_data);
		if( status == PRO_TK_NO_ERROR )
		{  //���óߴ������ʺ���
			ProFeatureDimensionVisit(&feature,(ProDimensionVisitAction)FeatureDimensionVisitAction,
				(ProDimensionFilterAction)FeatureDimensionFilterAction,(ProAppData)&p_data);
		}
		else
		{
			ProArrayFree( (ProArray*)p_data );
			*p_data = NULL;
		}

	}
	else
		status = PRO_TK_BAD_INPUTS;
	return (status);

}

/*================================================================*\
FUNCTION: SgCollectDim() 
�Զ��庯����������õ�ǰģ�͵ĳߴ����ָ�룬��������ָ��������
\*================================================================*/
ProError  SgCollectDim(ProSolid solid,			//(In)��ǰģ��
					   ProDimension **p_data)	//(In)�ߴ��������
{
	ProError status;
	if( p_data != NULL )
	{
		status=ProArrayAlloc(0,sizeof(ProDimension),1,(ProArray*)p_data);
		if( status == PRO_TK_NO_ERROR )
		{  //���óߴ������ʺ���
			status=ProSolidDimensionVisit (solid,PRO_B_FALSE,
				(ProDimensionVisitAction)FeatureDimensionVisitAction,
				(ProDimensionFilterAction)FeatureDimensionFilterAction,(ProAppData)&p_data);  
		}
		else
		{
			ProArrayFree( (ProArray*)p_data );
			*p_data = NULL;
		}

	}
	else
		status = PRO_TK_BAD_INPUTS;
	return (status);

}

/*=========================================================================*\
Function:	FeatureDimensionVisitAction() �ߴ������ʺ����Ķ�����������
\*=========================================================================*/
ProError FeatureDimensionVisitAction(ProDimension *p_object,ProError status,
									 ProAppData app_data)  

{   
	ProArray *p_array;
	p_array = (ProArray*)((ProDimension**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1,p_object );
	return (status);
}
/*=========================================================================*\
Function: FeatureDimensionFilterAction() �ߴ������ʺ����Ĺ��˺�������
\*=========================================================================*/
ProError FeatureDimensionFilterAction(ProDimension *p_object,
									  ProAppData app_data)  
{   
	/*ProBoolean rel_driven;
	ProDimensionIsReldriven(p_object,&rel_driven);
	if (rel_driven)
	return (PRO_TK_CONTINUE);
	else*/
	return (PRO_TK_NO_ERROR);
}

/*=========================================================================*\
Function:	FeatureVisitAction() ����������ʺ����Ķ�����������
\*=========================================================================*/
ProError FeatureAsmVisitAcFn(ProFeature *p_object,ProError status,
							 CArray<ProFeature,ProFeature> * app_data)  

{   	
	ProBoolean     isVisible;
	int         *  r_failed_feat_ids;
	int            num=0;
	ProBoolean     p_is_incomplete;
	ProFeatStatus  p_status;
	//��������   
	//����ʧ��
	status=ProArrayAlloc(0,sizeof(int),1,(ProArray*)&r_failed_feat_ids);
	status=ProSolidFailedFeatsList((ProSolid)(p_object->owner),&r_failed_feat_ids);
	status=ProArraySizeGet((ProArray)r_failed_feat_ids,&num);
	for (int i=0;i<num;i++)
	{
		if (p_object->id==r_failed_feat_ids[i])
		{
			ProArrayFree((ProArray*)&r_failed_feat_ids);
			return PRO_TK_NO_ERROR;
		}
	}
	//������������
	status=ProFeatureIsIncomplete(p_object,&p_is_incomplete);
	if (p_is_incomplete!=PRO_B_FALSE) return PRO_TK_NO_ERROR;
	//����û�б����������������
	status = ProFeatureStatusGet(p_object,&p_status);
	if(p_status==PRO_FEAT_SUPPRESSED) return PRO_TK_NO_ERROR;
	if(p_status!=PRO_FEAT_ACTIVE)    return PRO_TK_NO_ERROR;
	//�����ɼ�
	status=ProFeatureVisibilityGet(p_object,&isVisible);
	if (isVisible==PRO_B_TRUE) {
		app_data->Add(*p_object);
	}	
	return PRO_TK_NO_ERROR;
}
/*=========================================================================*\
Function: FeatureAsmFilterAction() װ��������������ʺ����Ĺ��˺�������
\*=========================================================================*/
ProError FeatureAsmFilterAcFn(ProFeature *p_object,
							  CArray<ProFeature,ProFeature> * app_data)  
{   

	ProError       status;
	ProFeattype    ftype;
	status = ProFeatureTypeGet (p_object, &ftype);
	if (ftype == PRO_FEAT_COMPONENT)
		return (PRO_TK_NO_ERROR);	
	return (PRO_TK_CONTINUE);

}
/************************************************************************/
/*������partFeatDimVisAc
˵���������ĳߴ���ʶ�������                                          */
/************************************************************************/
ProError partFeatDimVisAc(ProDimension *dimension,ProError status,
						  CArray<ProDimension,ProDimension> * app_data)
{
	//������Ϊδ֪�ĳߴ���˵�
	ProDimensiontype type;
	ProDimensionTypeGet(dimension,&type);
	if(type!=PRODIMTYPE_UNKNOWN)  app_data->Add(*dimension);
	return PRO_TK_NO_ERROR;
}
/************************************************************************/
/*������partFeatDimVisAc
˵���������Ĳ������ʶ�������                                          */
/************************************************************************/
ProError partAsmParaVisAc(ProParameter* p_parameter,ProError status,
						  CArray<ProParameter,ProParameter> * app_data)
{
	app_data->Add(*p_parameter);
	return PRO_TK_NO_ERROR;
}
/*====================================================================*\
FUNCTION :	CreatDefDtm (��������Ĭ�ϻ�׼��)
\*====================================================================*/
bool CreatDefDtm(void)
{
	ProName dtm_name;
	UsrCreatDefDtm(PRO_DTMPLN_DEF_X,ProStringToWstring (dtm_name, "RIGHT"),&right);
	UsrCreatDefDtm(PRO_DTMPLN_DEF_Y,ProStringToWstring (dtm_name, "TOP"),&top);
	UsrCreatDefDtm(PRO_DTMPLN_DEF_Z,ProStringToWstring (dtm_name, "FRONT"),&front);
	return false;
}
ProError GemitemAction(ProGeomitem* p_geomitems,ProError status,ProAppData app_data)
{
	memcpy(app_data, p_geomitems, sizeof(ProGeomitem));
	return(PRO_TK_E_FOUND);
}
void UsrGeomitemPlnGet(ProFeature feature,ProGeomitem *geomitem)
{
	ProFeatureGeomitemVisit(&feature, PRO_SURFACE, 
		(ProGeomitemAction)GemitemAction,NULL,geomitem);
}

/*====================================================================*\
FUNCTION :	UsrCreatDefDtm (������׼��)
\*====================================================================*/
int UsrCreatDefDtm(ProDtmplnConstrType pro_dtmpln_def,ProName dtm_name,ProGeomitem *pln)
{
	ProError         status;
	ProElement       elem_tree,elem_type,elem_consts,elem_const,elem_const_type;
	ProValueData     value_data;
	ProValue         value;
	ProModelitem     model_modelitem;
	ProSelection     mdl_sel;
	ProFeature       feature;
	ProErrorlist     errors;
	ProSolid solid;
	/*---------------------------------------------------------------*\
	Ϊ����������Ԫ��elem_tree�����ڴ�
	\*---------------------------------------------------------------*/
	ProElementAlloc (PRO_E_FEATURE_TREE, &elem_tree);
	/*----------------------------------------------------------------*\
	�������������������Ԫ��elem_ftype
	\*----------------------------------------------------------------*/
	//��������Ԫ��ֵ֮
	ProElementAlloc (PRO_E_FEATURE_TYPE, &elem_type);
	value_data.type = PRO_VALUE_TYPE_INT;
	value_data.v.i = PRO_FEAT_DATUM;
	ProValueAlloc (&value);
	ProValueDataSet (value, &value_data);
	ProElementValueSet (elem_type, value);  
	//������Ԫ����ӵ�����Ԫ������
	ProElemtreeElementAdd (elem_tree, NULL, elem_type);
	/*----------------------------------------------------------------*\
	�����������Լ��ƽ�漯Ԫ��elem_consts
	\*----------------------------------------------------------------*/
	ProElementAlloc (PRO_E_DTMPLN_CONSTRAINTS, &elem_consts);
	ProElemtreeElementAdd (elem_tree, NULL, elem_consts);
	/*----------------------------------------------------------------*\
	��Լ��ƽ�漯Ԫ�������Ԫ��elem_const
	\*----------------------------------------------------------------*/
	ProElementAlloc (PRO_E_DTMPLN_CONSTRAINT, & elem_const);
	ProElemtreeElementAdd (elem_consts, NULL, elem_const);
	/*----------------------------------------------------------------*\
	���û�׼������Ԫ�أ�����ӵ���������
	\*----------------------------------------------------------------*/
	ProElementAlloc (PRO_E_DTMPLN_CONSTR_TYPE, &elem_const_type);
	value_data.type = PRO_VALUE_TYPE_INT;
	value_data.v.i = pro_dtmpln_def;
	ProValueAlloc (&value);
	ProValueDataSet (value, &value_data);
	ProElementValueSet(elem_const_type, value);  
	ProElemtreeElementAdd (elem_const, NULL,elem_const_type);
	/*----------------------------------------------------------------*\
	��õ�ǰģ�����ѡ�����
	\*----------------------------------------------------------------*/
	ProMdlCurrentGet((ProMdl*)&solid);
	ProMdlToModelitem(solid, &model_modelitem);
	ProSelectionAlloc(NULL, &model_modelitem, &mdl_sel);
	/*----------------------------------------------------------------*\
	������׼ƽ��
	\*----------------------------------------------------------------*/
	ProFeatureCreate (mdl_sel, elem_tree, NULL, 0,&feature,&errors);
	/*----------------------------------------------------------------*\
	���û�׼������
	\*----------------------------------------------------------------*/
	status=ProModelitemNameSet(&feature,dtm_name);
	/*----------------------------------------------------------------*\
	�ض���
	\*----------------------------------------------------------------*/  
	status=ProFeatureRedefine(NULL, &feature, elem_tree, NULL, 0, &errors);
	UsrGeomitemPlnGet(feature,pln);
	/*----------------------------------------------------------------*\
	�ͷ��ڴ�ռ�
	\*----------------------------------------------------------------*/
	ProElementFree (&elem_tree);
	ProSelectionFree (&mdl_sel);return (status);
}
/************************************************************************/
/* localdir��ʽ:g:\abc\bcd                                              */
/************************************************************************/
bool RemoveDireAndFile(CString localdir)
{
	CFileFind ff;
	CString szDir = localdir;
	if(szDir.Right(1) != "\\")
		szDir += "\\";
	szDir += "*.*";
	//���Ŀ¼Ϊ�ջ���Ŀ¼Ϊ�̷����Ƴ�
	if(localdir.IsEmpty() || (localdir.Find(':',0)==1 && localdir.GetLength()==3)) return false;
	BOOL res = ff.FindFile(szDir);
	while(res)
	{
		res = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots())       //Ŀ¼���ļ���
		{
			//�����һ����Ŀ¼���õݹ��������һ����
			CString strPath = ff.GetFilePath();    //�õ�·������Ϊ�ݹ���õĿ�ʼ			         	
			RemoveDireAndFile(strPath);            //�ݹ����
		}
		else if(!ff.IsDirectory() && !ff.IsDots()) //������Ͳ���ļ�
		{
			//��ʾ��ǰ���ʵ��ļ�
			CString strPath="";			
			strPath = ff.GetFilePath();
			DeleteFile(strPath);
		}
	}	
	ff.Close();                                   //�ر�
	RemoveDirectory(_T(localdir+"\\"));	
	return 0;
}
/************************************************************************/
/* ����ƶ�ģ�͹���ͼ����Ϣ                                             */
/************************************************************************/
void GetDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList,CArray<int,int> * p_IndexList)
{
	//int    num=0;
	//ProName name;
	//num=(int)P_UsrDrawingList.GetCount();
	//for(int i=0;i<num;i++)
	//{
	//	//�ж��Ƿ񻹴���
	//	if (IsMdlDeleted())
	//	{
	//		P_UsrDrawingList.RemoveAt(i);
	//		i=i-1;
	//		continue;
	//	}
	//	if(P_UsrDrawingList[i].owner==solid)
	//	{

	//		if(p_DrawingList!=NULL) p_DrawingList->Add(P_UsrDrawingList[i].hDraw);
	//		if(p_IndexList!=NULL) p_IndexList->Add(i);
	//	}
	//}
}
/************************************************************************/
/* ����ƶ�ģ�͵Ĺ��̾��                                               */
/************************************************************************/
void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList)
{
	ProMdl *p_model_array;
	int     p_count=0;
	/*ProMdlType type;*/
	int         num=0;
	ProSessionMdlList(PRO_MDL_DRAWING,&p_model_array,&p_count);
	for (int i=0;i<p_count;i++)
	{
		ProSolid *solids;
		int       solidnum=0;
		ProDrawingSolidsCollect((ProDrawing)p_model_array[i],&solids);
		ProArraySizeGet((ProArray)solids,&solidnum);
		for (int j=0;j<solidnum;j++)
		{
			if(solids[j]==solid) 
			{
				int Flag=0;
				//�ж��Ƿ��Ѿ����ڣ�һλ������������ܲ�ͬ�����ͬһ������ͼ��
				for (int m=0;m<(int)p_DrawingList->GetCount();m++)
				{
					if(p_DrawingList->GetAt(m)==(ProDrawing)p_model_array[i])
					{
						Flag=1;
						break;
					}
				}
				if(Flag==0) p_DrawingList->Add((ProDrawing)p_model_array[i]);
                break;
			}
		}
	}
	//ProMdlTypeGet(solid,&type);
	//if (type==PRO_MDL_ASSEMBLY)
	//{
	//	CArray<ProFeature,ProFeature> feat_list;
	//	ProSolidFeatVisit(solid,(ProFeatureVisitAction)FeatureAsmVisitAcFn
	//		,NULL,(CArray<ProFeature,ProFeature>*)&feat_list);
	//	num=(int)feat_list.GetCount();
	//	for (int i=0;i<num;i++) {
	//		ProFeattype    ftype;
	//		ProFeatureTypeGet (&feat_list[i], &ftype);
	//		if (ftype == PRO_FEAT_COMPONENT){
	//			ProSolid              mdl;
	//			ProAsmcompMdlGet((ProAsmcomp *)(&feat_list[i]),(ProMdl *)&mdl);
	//			GetSolidDrawing(mdl,p_DrawingList);
	//		}
	//	}
	//}
}

/************************************************************************/
/* ������ƺ��������ñ�����Ƶĺ���                                     */
/************************************************************************/
BOOL VarantFn(CString str,ProSolid solid,CString mdlNum,long insNum )
{
	CCompute test;
	BOOL     reValue;
	if(m_db==NULL)
	{
		m_db=new CMyDatabase();
	}
	else
	{
		delete m_db;
		m_db=new CMyDatabase();
	}
	if(!m_db->Open(str)) 
	{
		m_db->Close();
		delete m_db;
		m_db=NULL;
		return FALSE;
	}
	reValue=test.Interface(solid,mdlNum,insNum);
	m_db->Close();
	delete m_db;
	m_db=NULL;
	return reValue;
}


/****************************************************************************************\
������partFeatVisAc();
˵����������������ʺ���
\****************************************************************************************/
ProError partFeatVisAc(ProFeature* p_feature,ProError status,
					   CArray<ProFeature,ProFeature> * app_data)
{
	//app_data->Add(*p_feature);
	//return PRO_TK_NO_ERROR;
	ProBoolean     isVisible;
	int         *  r_failed_feat_ids;
	int            num=0;
	ProBoolean     p_is_incomplete;
	ProFeatStatus  p_status;
	//��������   
	//1.����ʧ��
	status=ProArrayAlloc(0,sizeof(int),1,(ProArray*)&r_failed_feat_ids);
	status=ProSolidFailedFeatsList((ProSolid)(p_feature->owner),&r_failed_feat_ids);
	status=ProArraySizeGet((ProArray)r_failed_feat_ids,&num);
	for (int i=0;i<num;i++)
	{
		if (p_feature->id==r_failed_feat_ids[i])
		{
			ProArrayFree((ProArray*)&r_failed_feat_ids);
			return PRO_TK_NO_ERROR;
		}
	}
	//2.������������
	status=ProFeatureIsIncomplete(p_feature,&p_is_incomplete);
	if (p_is_incomplete!=PRO_B_FALSE) return PRO_TK_NO_ERROR;
	//3.����û�б����������������
	status = ProFeatureStatusGet(p_feature,&p_status);
	if(p_status==PRO_FEAT_SUPPRESSED) return PRO_TK_NO_ERROR;
	if(p_status!=PRO_FEAT_ACTIVE)    return PRO_TK_NO_ERROR;
	//4.�����ɼ�
	status=ProFeatureVisibilityGet(p_feature,&isVisible);
	if (isVisible==PRO_B_TRUE) {
		app_data->Add(*p_feature);
	}	
	return PRO_TK_NO_ERROR;
}

/************************************************************************/
/* ������partParaVisAc(ProParameter* p_parameter,ProError status,
CArray<ProParameter,ProParameter> * app_data)
˵������������ķ��ʺ���                                             */
/************************************************************************/
ProError partParaVisAc(ProParameter* p_parameter,ProError status,
					   CArray<ProParameter,ProParameter> * app_data)
{
	app_data->Add(*p_parameter);
	return PRO_TK_NO_ERROR;
}
/************************************************************************/
/* ��������ͼ��                                                         */
/************************************************************************/
void SetImageListForFeatrure(CImageList * p_imageList)
{
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_ASM));
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_CSYS));
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_DTM));
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_FEAT));
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_LINE));
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_POT));
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_PRT));
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_ROUND));           //PRO_FEAT_ROUND 7
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_GROP));            //PRO_FEAT_GROUP_HEAD  8
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_SHELL));           //PRO_FEAT_SHELL 9
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_PROTRUSION_AND_CUT));//PRO_FEAT_PROTRUSION PRO_FEAT_CUT 10
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_HOL));             //PRO_FEAT_HOLE 10
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_PATTERN));         //PRO_FEAT_PATTERN_HEAD  12
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_CHAMFER));         //PRO_FEAT_CHAMFER13
	p_imageList->Add(AfxGetApp()->LoadIcon(IDI_RIB));             //PRO_FEAT_RIB 14
}
/************************************************************************/
/* ����ƶ�������Ӧ��ͼ��                                               */
/************************************************************************/
int GetFeatIconNum(int type)
{	
	switch(type) {
	case PRO_FEAT_ROUND:
		return 7;
		break;
	case PRO_FEAT_GROUP_HEAD:
		return 8;
		break;
	case PRO_FEAT_SHELL:
		return 9;
		break;
	case PRO_FEAT_PROTRUSION:
		return 10;
		break;
	case PRO_FEAT_CUT:
		return 10;
		break;
	case PRO_FEAT_PATTERN_HEAD:
		return 12;
		break;
	case PRO_FEAT_DATUM:
		return 2;
		break;
	case PRO_FEAT_DATUM_POINT:
		return 5;
		break;
	case PRO_FEAT_DATUM_AXIS:
		return 4;
		break;
	case PRO_FEAT_CSYS:
		return 1;
		break;
	case PRO_FEAT_HOLE:
		return 11;
		break;
	case PRO_FEAT_CHAMFER:
		return 13;
		break;
	case PRO_FEAT_RIB:
		return 14;
		break;
	default:
		return 3;
		break;
	}
	return 3;
}
/************************************************************************/
/* ��������Ŀ¼(���)                                                   */
/************************************************************************/
bool CreateTempDir(ProSolid solid,CString * csPathName)
{
	ProName					name;
	UsrPartPorp				tempProp=GetMdlInfo(solid);
	int						num=0;
	int						indexNo=0;
	CString					typeandversion;
	CString					cPathName;
	ProMdldata				model_info,draw_info;
	CString					csPathDrawing;
	CString					mdltemppath;
	TCHAR					szTempPath[MAX_PATH];	
	DWORD					dwResult=GetTempPath (MAX_PATH, szTempPath);
	ProMdlNameGet(solid,name); 
	//��һ����ʱĿ¼
	if (!CreateDirectoryGivenPathAndName(szTempPath,tempProp.Name,cPathName))
		return FALSE;
	//1.����ģ�͵���ʱĿ¼
	//��ʱĿ¼ȫ·��
	cPathName=CString(szTempPath)+cPathName;
    (*csPathName)=cPathName;

	ProStringToWstring(model_info.path,cPathName.GetBuffer());	
	if (UsrMdlBackUp(solid,&model_info)!=PRO_TK_NO_ERROR)
	{
		RemoveDireAndFile(_T(*csPathName));
		AfxMessageBox("����ģ�͹�����ʧ��");
		return false;
	}

	//2.���ݹ���ͼ
	csPathDrawing=csPathName->GetString();
	csPathDrawing=csPathDrawing+CString("\\")+"drawing";
	CreateDirectory(csPathDrawing,NULL);
	if (tempProp.bHaveDraw)
	{
		cPathName=csPathDrawing;
		ProStringToWstring(draw_info.path,cPathName.GetBuffer());
		if(UsrMdlBackUp(tempProp.hDraw,&draw_info)!=PRO_TK_NO_ERROR)
		{
			CString     temp;
			temp.Format("���ݹ����ļ�����ʱĿ¼��ʱ���޷��ڽ������ҵ�%s�ļ�",GetMdlFileName(tempProp.hDraw).GetBuffer() );
			AfxMessageBox(temp);
			RemoveDireAndFile(_T(*csPathName));
			SetMdlInfo(solid)->bHaveDraw=false;
			SetMdlInfo(solid)->hDraw=NULL;
			return false;
		}
		CFileFind ff;
		CString   szDir = csPathDrawing+"\\"+"*.*";
		if (ff.FindFile(szDir))
		{
			while (ff.FindNextFile())
			{
				if(ff.IsDirectory()) continue;
				if (ff.GetFileName().Find(".drw",0)==-1)     
				{
					DeleteFile(ff.GetFilePath());
				}
			}
			if (ff.GetFileName().Find(".drw",0)==-1)     
			{
				DeleteFile(ff.GetFilePath());
			}
		}
		ff.Close();
	}
	return true;
}
/************************************************************************/
/* ���ģ���Ƿ��б�ɾ����������������Ϣ                               */
/************************************************************************/
void UsrClearMdlInfo()
{
	CArray<ProMdl,ProMdl>  mdllist;
	for (int i=0;i<(int)p_UsrSWTXTableList.GetCount();i++)
	{
		mdllist.Add(p_UsrSWTXTableList[i].owner);
	}
	for (int i=0;i<(int)p_AsmSmlItemList.GetCount();i++)
	{
		mdllist.Add(p_AsmSmlItemList[i].owner );
	}
	for (int i=0;i<(int)p_UsrPicture.GetCount();i++)
	{
		mdllist.Add(p_UsrPicture[i].owner );
	}
	for (int i=0;i<(int)p_UsrPerfParaTableList.GetCount();i++)
	{
		mdllist.Add(p_UsrPerfParaTableList[i].owner );
	}
	for (int i=0;i<(int)p_AsmCompItemList.GetCount();i++)
	{
		mdllist.Add(p_UsrPerfParaTableList[i].owner);
	}
	for (int i=0;i<mdllist.GetCount();i++)
	{
		if (IsMdlDeleted(mdllist[i]))
		{
			EraseMdlInfo(mdllist[i],ERASE_ALL);
		}
	}
}
/************************************************************************/
/* ���ģ�����е���Ϣ,���˻�����Ϣ֮�⣬��Ϊ������Ϣ                    */
/************************************************************************/
void EraseMdlInfo(ProMdl mdl,UINT marsk)
{
    if ( (marsk&ERASE_SWTX) == ERASE_SWTX)
    {
		for (int i=0;i<(int)p_UsrSWTXTableList.GetCount();i++)
		{
			if (p_UsrSWTXTableList[i].owner==mdl)
			{
				p_UsrSWTXTableList.RemoveAt(i);
				i=i-1;
			}
		}
		p_UsrSWTXTableList.FreeExtra();
		//���
		for (int i=0;i<(int)p_AsmSmlItemList.GetCount();i++)
		{
			if (p_AsmSmlItemList[i].owner ==(ProSolid) mdl)
			{
				p_AsmSmlItemList.RemoveAt(i);
				i=i-1;
			}
		}
		p_AsmSmlItemList.FreeExtra();
    }
	if ((marsk&ERASE_REL) == ERASE_REL /*&& GetMdlType(mdl)==PRO_MDL_PART*/)
	{
		g_RelCollection.RemoveAllRel((ProSolid)mdl);
	}

	if ((marsk&ERASE_PIC) == ERASE_PIC)
	{
		for (int i=0;i<(int)p_UsrPicture.GetCount();i++)
		{
			if (p_UsrPicture[i].owner ==(ProSolid) mdl)
			{
				DeleteFile(p_UsrPicture[i].filePath+p_UsrPicture[i].fileName);
				p_UsrPicture.RemoveAt(i);
				i=i-1;
			}
		}
		p_UsrPicture.FreeExtra();
	}
	if ((marsk&ERASE_COMP) == ERASE_COMP)
	{
		for (int i=0;i<(int)p_AsmCompItemList.GetCount();i++)
		{
			if (p_AsmCompItemList[i].owner ==(ProSolid) mdl)
			{
				p_AsmCompItemList.RemoveAt(i);
				i=i-1;
			}
		}
		p_AsmCompItemList.FreeExtra();
	}
	if ((marsk&ERASE_PERF) == ERASE_PERF)
	{		
		for (int i=0;i<(int)p_UsrPerfParaTableList.GetCount();i++)
		{
			if (p_UsrPerfParaTableList[i].owner ==(ProSolid) mdl)
			{
				p_UsrPerfParaTableList.RemoveAt(i);
				i=i-1;
			}
		}
		p_UsrPerfParaTableList.FreeExtra();
	}
}
/************************************************************************/
/* �������ģ�͵���ʱĿ¼                                               */
/************************************************************************/
BOOL CreateTempAsmDirectory(ProSolid asm_solid,CString * csPathName)
{
	UsrPartPorp  temp_porp;
	TCHAR        szTempPath[MAX_PATH];	
	DWORD        dwResult=GetTempPath (MAX_PATH, szTempPath);
	ASSERT       (dwResult);	
	ProMdldata   model_info;
	ProMdldata   draw_info;
	CString      csPathDrawing;
	CString      tempDirectoryName;
	CArray<ProSolid,ProSolid> solidlist;

	temp_porp=GetMdlInfo(asm_solid);	
	if (!CreateDirectoryGivenPathAndName(szTempPath,temp_porp.Name,tempDirectoryName))
		return FALSE;
	(*csPathName)=CString(szTempPath)+tempDirectoryName;
	ProStringToWstring(model_info.path,csPathName->GetBuffer());
	//1.����ģ��
	if (UsrMdlBackUp(asm_solid,&model_info)!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("����ģ��ʧ�ܣ�");
		return FALSE;
	}
	//2.���ݹ���ͼ
	csPathDrawing=csPathName->GetString();
	csPathDrawing=csPathDrawing+CString("\\")+"drawing";
	CreateDirectory(csPathDrawing,NULL);
	if (temp_porp.bHaveDraw)
	{
		ProStringToWstring(draw_info.path,csPathDrawing.GetBuffer());
		if(UsrMdlBackUp(temp_porp.hDraw,&draw_info)!=PRO_TK_NO_ERROR)
		{
			CString     temp;
			temp.Format("���ݹ����ļ�����ʱĿ¼��ʱ���޷��ڽ������ҵ�%s�ļ�",GetMdlFileName(temp_porp.hDraw).GetBuffer() );
			AfxMessageBox(temp);
			SetMdlInfo(asm_solid)->bHaveDraw=false;
			SetMdlInfo(asm_solid)->hDraw=NULL;
			return FALSE;
		}
		//����Ԫ���Ĺ���ͼ
		GetAsmCompHand(asm_solid,&solidlist);		
		for (int i=0;i<solidlist.GetCount();i++)
		{
			temp_porp=GetMdlInfo(solidlist[i]);
			ProStringToWstring(draw_info.path,csPathDrawing.GetBuffer());
			if(temp_porp.hDraw)  UsrMdlBackUp(temp_porp.hDraw ,&draw_info);
		}
	
		//CFileFind ff;
		//CString   szDir = csPathDrawing+"\\"+"*.*";
		//if (ff.FindFile(szDir))
		//{
		//	while (ff.FindNextFile())
		//	{
		//		if(ff.IsDirectory() ) continue;
		//		if (ff.GetFileName().Find(".drw",0)==-1)     DeleteFile(ff.GetFilePath());
		//	}
		//	if (ff.GetFileName().Find(".drw",0)==-1)     
		//	{
		//		DeleteFile(ff.GetFilePath());
		//	}
		//}
		//ff.Close();
	}
	return TRUE;
}


void AddAsmSubItemForOpen(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM hItem)
{
	//HTREEITEM tempItem;
	//CAsmManageTableSet m_pSet(p_db);
	//m_pSet.m_strFilter.Format("FatherNumber=%ld",(long)m_Tree->GetItemData(hItem));
	//m_pSet.Open();
	//if(m_pSet.IsBOF()) return;
	//m_pSet.MoveFirst();
	//do {
	//	int i=0,j=0;
	//	int nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	//	if(m_pSet.m_Type=="mdl") i=2,j=2;
	//	if (m_pSet.m_Type=="typ")
	//	{
	//		tempItem= m_Tree->InsertItem(nMarsk,_T(m_pSet.m_Name), 
	//			i, j, 0, 0, m_pSet.m_Number, hItem, NULL);
	//		int num=m_pSet.m_Number;
	//		AddAsmSubItemForOpen(m_Tree,m_pSet.m_pDatabase,tempItem);
	//	}
	//	m_pSet.MoveNext();
	//} while(!m_pSet.IsEOF());
}
BOOL ShowAsmTreeForOpen(CTreeCtrl * m_Tree,CDatabase * p_db)
{
	//CAsmManageTableSet m_pSet(p_db);
	//m_Tree->DeleteAllItems();
	//HTREEITEM hItemRoot=m_Tree->InsertItem(_T("װ��ģ��"));
	//m_pSet.m_strFilter="[FatherNumber]=0";
	//if(!m_pSet.Open())
	//{
	//	AfxMessageBox("���ݿ����Ӳ��ɹ���");
	//	return FALSE;
	//}
	//if(m_pSet.IsBOF())
	//{
	//	return TRUE;
	//}
	//m_pSet.MoveFirst();
	//do {
	//	HTREEITEM hItem=m_Tree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T(m_pSet.m_Name), 
	//		0, 0, 0, 0, m_pSet.m_Number, hItemRoot, NULL);
	//	int num=m_pSet.m_Number;
	//	AddAsmSubItemForOpen(m_Tree,m_pSet.m_pDatabase,hItem);
	//	m_pSet.MoveNext();
	//} while(!m_pSet.IsEOF());
	//m_Tree->Expand(hItemRoot,TVE_EXPAND);
	return TRUE;
}
void AddPartSubItemForOpen(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item)
{
	//CPartMdlRecordSet m_pSet(p_db);
	//HTREEITEM tempItem;
	//m_pSet.m_strFilter.Format("[FatherNumber]=%ld",(long)m_Tree->GetItemData(item));
	//m_pSet.Open();
	//if(m_pSet.IsBOF()) return;
	//m_pSet.MoveFirst();
	//do {
	//	int i=0,j=0;
	//	int nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	//	if(m_pSet.m_Type=="mdl") i=2,j=2;
	//	else  i=j=0;
	//	if (m_pSet.m_Type=="typ")
	//	{
	//		tempItem= m_Tree->InsertItem(nMarsk,_T(m_pSet.m_Name), 
	//			i, j, 0, 0, m_pSet.m_Number, item, NULL);
	//		int num=m_pSet.m_Number;
	//		AddPartSubItemForOpen(m_Tree,m_pSet.m_pDatabase,tempItem);
	//	}
	//	m_pSet.MoveNext();
	//} while(!m_pSet.IsEOF());
}
BOOL ShowPartTreeForOpen(CTreeCtrl * m_Tree,CDatabase * p_db)
{
	CPartMdlRecordSet m_pSet(p_db);
	//��Ӹ��ڵ�
	m_Tree->DeleteAllItems();
	HTREEITEM hItemRoot;
	hItemRoot=m_Tree->InsertItem(_T("���ģ��"));
	m_pSet.m_strFilter=_T("[FatherNumber]=0");
	if(!m_pSet.Open())
	{
		AfxMessageBox("���ݿ����Ӳ��ɹ���");
		return FALSE;
	}
	if(m_pSet.IsBOF())
	{
		return TRUE;
	}
	m_pSet.MoveFirst();
	do {
		HTREEITEM hItem=m_Tree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE
			,_T(m_pSet.m_Name),0, 0, 0, 0, m_pSet.m_Number, hItemRoot, NULL);
		int num=m_pSet.m_Number;
		AddPartSubItemForOpen(m_Tree,m_pSet.m_pDatabase,hItem);
		m_pSet.MoveNext();
	} while(!m_pSet.IsEOF());
	m_Tree->Expand(hItemRoot,TVE_EXPAND);
	return TRUE;
}

/************************************************************************/
/* ���ǰ����                                                         */
/************************************************************************/
ProError UsrActivateCurrentWindow()
{
	int window_id;
	ProError status;
	status=ProWindowCurrentGet(&window_id);
	status=ProWindowActivate(window_id);
	return status;
}
/************************************************************************/
/* �ػ浱ǰ����                                                         */
/************************************************************************/
ProError UsrRepaintWindow()
{
	int window_id;
	ProError status;
	status=ProWindowCurrentGet(&window_id);
	status=ProWindowRefresh(window_id);
	return status;
}
//���ģ�͵ı����Ƿ��Ѿ���ʹ��
//�Լ�ģ�͵ı����Ƿ�Ϸ�,��λ���ȵ�
#include "PartMdlRecordSet.h"
#include "AsmManageTableSet.h"
bool CheckIfCodeHavebeenUsed(long TypeNum=-1/*���Ҳ�ʹ��*/,CString code="",CDatabase * p_db=NULL)
{
    CPartMdlRecordSet  m_set1(p_db);
    CAsmManageTableSet m_set2(p_db);
	ASSERT(p_db!=NULL);
	m_set1.m_strFilter.Format("[Code]='%s'",code.GetBuffer());
	m_set2.m_strFilter.Format("[Code]='%s'",code.GetBuffer());
	m_set1.Open();
	m_set2.Open();
	if (!m_set1.IsBOF() || !m_set2.IsBOF())
	{
		return false;
	}
	return true;
}
//����·����·��������Ҫ"\\"���Լ��ļ��еĳ�ʼ�����������ֽ����ļ���
BOOL CreateDirectoryGivenPathAndName(CString csPath,CString inName,CString & outName)
{
	CString version;
	for (int i=0;;i++)
	{						
		version.Format("VdTemp_%s(%d)",inName.GetBuffer(),i);
		version=csPath+version;
		if (CreateDirectoryA(version,NULL))
		{
			outName.Format("VdTemp_%s(%d)",inName.GetBuffer(),i);
			return TRUE;
			break;
		}
		if(i>100) 
		{
			AfxMessageBox("ģ�����ع����У��޷��ڱ��ش�����ʱĿ¼");
			return FALSE;
		}
	}
	return FALSE;
}
//�жϱ����ļ��Ƿ����
bool IsFilesExists(CString csPath,CString csFilter
				   ,CStringArray & files/*�ļ�����*/,CString & csError)
{
	ProPath directory_path;
	ProLine filter;
	ProPath *p_file_name_array;
	ProPath *p_subdir_name_array;
	ProError status;
	int      p_size=0;
	ProStringToWstring(directory_path,csPath.GetBuffer());
	ProStringToWstring(filter,csFilter.GetBuffer());
	if (files.GetCount()==0)
		return true;
	csError.Empty();

	status=ProArrayAlloc(0,sizeof(ProPath),1,(ProArray*)&p_file_name_array);
	status=ProArrayAlloc(0,sizeof(ProPath),1,(ProArray*)&p_subdir_name_array);
	status=ProFilesList (directory_path,filter,PRO_FILE_LIST_LATEST,&p_file_name_array,&p_subdir_name_array);
	if (status==PRO_TK_NO_ERROR)
	{
		//�Ȳ������Ƿ��а汾
		ProArraySizeGet((ProArray)p_file_name_array,&p_size);
		for (int i=0;i<files.GetCount();i++)
		{
			CString temp=csPath+files[i];
			int     flag=0;
			//����
			for (int j=0;j<p_size;j++)
			{
				CString temp2=CString(p_file_name_array[i]);
				if (temp2.Find(temp,0)==-1)
				{
					flag=1;
					break;
				}
			}
			//���û���ҵ�
			if (flag!=1)
			{
				csError.Format("%s�ļ�%s������\n",csError.GetBuffer(),files[i]);
			}
		}
	}
	status=ProArrayFree((ProArray*)&p_file_name_array);
	status=ProArrayFree((ProArray*)&p_subdir_name_array);
	if (csError.IsEmpty())
        return true;
	return false;
}
//�жϱ����ļ��Ƿ����
bool IsFilesExists(CStringArray & filesFullPath/*�ļ�����*/,CString & csError)
{
	CFileFind ff;
	for (int i=0;i<filesFullPath.GetCount();i++)
	{
		if (!ff.FindFile(filesFullPath[i]+".*"))
			csError.Format("%s�ļ�%s������\n",csError.GetBuffer(),filesFullPath[i]);
		ff.Close();
	}
	if (csError.IsEmpty())
		return true;
	return false;
}
/************************************************************************/
/* ѡ��һ��Ŀ¼
   06.3.3�޸�  ��Ϊ�̶�ĳ��Ŀ¼��������ѡȡ                             */
/************************************************************************/
bool  SelectAnDirectory(CString & selDir,CString defDir="")
{
	////06.3.3ǰ
	//ProPath       directory;
	//ProPath       defdirpath;
	//ProName       name;
	//ProError	  status;
	//ProStringToWstring(name,"ѡ�񱣴�λ��");
	//if (!defDir.IsEmpty())
	//{
	//	ProStringToWstring(defdirpath,defDir.GetBuffer());
	//	status=ProDirectoryChoose(name,NULL,NULL,defdirpath,directory);
	//}
	//else
	//{
	//	status=ProDirectoryChoose(name,NULL,NULL,NULL,directory);
	//}
	//if (status==PRO_TK_NO_ERROR)
	//{
	//	selDir=CString(directory)+"\\";
	//	return true;
	//}
	ProPath       directory;
	if (ProDirectoryCurrentGet(directory)==PRO_TK_NO_ERROR)
	{
		selDir=CString(directory)+"\\";		
		return true;
	}  	
	return false;
}
