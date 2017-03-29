// Example7_1.cpp  
#include "stdafx.h"
#include "RelParaEditDlg.h"
/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include <ProSolid.h>
#include "ProUtil.h"
#include <ProWindows.h>
#include <ProUIDialog.h>
#include <ProUIPushbutton.h>
#include <ProUIInputpanel.h>
#include <ProUIOptionmenu.h>
#include <ProArray.h>
#include <ProParameter.h>
#include <ProParamval.h>
#include <ProUIList.h>
#define OK 1
#define CANCEL 0
ProParameter *p_data; //��������ָ�����飨ȫ�ֱ�����
extern CRelParaEditDlg dlg;
/*--------------------------------------------------------------------*\
Functions declaration  ��������
\*--------------------------------------------------------------------*/
void CANCEL_Action(char *, char *, ProAppData);
void AddParam_Action(char *, char *, ProAppData);
void DelParam_Action(char *, char *, ProAppData);
void Regen_Action(char *, char *, ProAppData);
void Param_Value_Action(char *, char *, ProAppData);
void ParamsList_Action(char *, char *, ProAppData);
ProError ParameterActionFn(ProParameter *,ProError,ProAppData);  
ProError ParameterFilterFn(ProParameter *,ProError,ProAppData);  
void ParamsGet(ProParameter **);
void ListLabelsSet(char *, char *);
void Param_Update(char *, char *);
/*================================================================*\
FUNCTION: Example7_1MenuActFn()
\*================================================================*/
int ParamOperateMenuActFn()
{
    ProError status;
    char  *dialog_name = "paraoperate_dlg";
    int ActiveDialog_status;
/*----------------------------------------------------------------*\
 װ��Ի�����Դ
\*----------------------------------------------------------------*/	
	status=ProUIDialogCreate(dialog_name,dialog_name ); 
	   if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("UIDialog CreateCreate error!");
		return status;
	}
	//�����б���ǩ
	ListLabelsSet(dialog_name,"ParamsList");
/*----------------------------------------------------------------*\
  ���öԻ���ָ��Ԫ���Ķ�������
\*----------------------------------------------------------------*/	

//����ParamsListѡ��Ķ�������	
	ProUIListSelectActionSet(dialog_name, "resourceParamsList",  
						 ParamsList_Action, NULL);
//����Cancel��ť�Ķ�������	
	ProUIPushbuttonActivateActionSet (dialog_name, "Cancel",  
						 CANCEL_Action, NULL); 
//����AddParam��ť�Ķ�������	
	ProUIPushbuttonActivateActionSet (dialog_name, "AddParam",  
						 AddParam_Action, NULL); 
//����DelParam��ť�Ķ�������	
	ProUIPushbuttonActivateActionSet (dialog_name, "DelParam",  
						 DelParam_Action, NULL); 
//����Regen��ť�Ķ�������	
	ProUIPushbuttonActivateActionSet (dialog_name, "Regen",  
						 Regen_Action, NULL); 
//����Param_Value�����Ķ�������	
	ProUIInputpanelActivateActionSet(dialog_name, "Param_Value",  
						 Param_Value_Action, NULL); 
/*----------------------------------------------------------------*\
	��ʾ�ͼ���Ի���
\*----------------------------------------------------------------*/	
   
	status=ProUIDialogActivate(dialog_name,&ActiveDialog_status);
	
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("UIDialogActivate error!");
		return status;
	}
/*----------------------------------------------------------------*\
    ���ڴ�����Ի�����Դ  
\*----------------------------------------------------------------*/	
    status=ProUIDialogDestroy (dialog_name);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("UIDialogDestroy error!");
		return status;
	}
	
	return status;
}
/*==================================================================*\
  	FUNCTION : CANCEL_Action ("CANCEL"��ť�Ķ�����������)
\*==================================================================*/
void CANCEL_Action(char *dialog, char *component, ProAppData data)
{
   	ProArrayFree((ProArray*)&p_data);
	ProUIDialogExit(dialog, CANCEL);  
	
}
/*==================================================================*\
  	FUNCTION : Regen_Action ("Regen"��ť�Ķ�����������)
\*==================================================================*/
void Regen_Action(char *dialog, char *component, ProAppData data)
{
	ProMdl model;
	Param_Update(dialog,"Param_Value");
    ProMdlCurrentGet(&model);
    ProSolidRegenerate ((ProSolid)model,PRO_B_TRUE);
	ProWindowRepaint(PRO_VALUE_UNUSED);
}

/*=========================================================================*\
    Function:	ParamsGetGet() ��õ�ǰģ�����ȫ����������
\*=========================================================================*/
void ParamsGet(ProParameter **p_data)  
{
	    
	ProMdl   model;
    ProModelitem modelitem;
             
    ProMdlCurrentGet(&model);
	ProMdlToModelitem(model, &modelitem);
	ProArrayAlloc(0,sizeof(ProParameter),1,(ProArray*)p_data);
    ProParameterVisit(&modelitem,
		  (ProParameterFilter)ParameterFilterFn,
		  (ProParameterAction )ParameterActionFn ,(ProAppData)&p_data);  
}

/*=========================================================================*\
    Function:	ParameterActionFn() �������ʶ�������
\*=========================================================================*/
ProError ParameterActionFn(ProParameter *Param,ProError status,
						ProAppData app_data)   
{   
	ProArray *p_array;
    p_array = (ProArray*)((ProParameter**)app_data)[0];
    status=ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1,Param );
	return (PRO_TK_NO_ERROR);
}

/*=========================================================================*\
    Function:	ParameterFilterFn() �������ʹ��˺���
\*=========================================================================*/
ProError ParameterFilterFn(ProParameter *Param,ProError status,
						ProAppData app_data)   
{   
	ProCharLine strtmp;
    ProWstringToString(strtmp, Param->id);
	if (strcmp(strtmp,"DESCRIPTION")==0 ||strcmp(strtmp,"MODELED_BY")==0 )
	   return (PRO_TK_CONTINUE);
	else
	   return (PRO_TK_NO_ERROR);
}

/*================================================================*\
FUNCTION: ListLabesSet()  �����б��ֵ֮
\*================================================================*/
void ListLabelsSet(char *dialog, char *component)
{
    ProError status;
    //ProParameter *p_data;
    int count;			
	char ** names;				//�б��������
    wchar_t ** labels;			//�б���ǩ����
    ProCharLine strtmp;
	//��õ�ǰģ�����в���
 	ParamsGet(&p_data);
	//��ô��ڵ�����
    status = ProArraySizeGet((ProArray)p_data, &count);
    //Ϊ�б��������ǩ��������ڴ�
    ProArrayAlloc (count, sizeof(wchar_t *),1,(ProArray*)&labels);
    ProArrayAlloc (count, sizeof(char *),1,(ProArray*)&names);    
    //�����и�Ԫ�س�ʼ��
    for (int i=0;i<count;i++){
	    names[i] = (char *)calloc(PRO_NAME_SIZE, sizeof(char));
	    labels[i] = (wchar_t *)calloc(PRO_NAME_SIZE, sizeof(wchar_t));	    
	    sprintf(strtmp,"%0d",i+1);
	    //�б��ѡ������ֵ:1,2,3,...
	    strcpy(names[i],strtmp);
   		//��ò�����
        ProWstringToString(strtmp, p_data[i].id);
		ProStringToWstring(labels[i], strtmp);
    }
    //�����б��List1��ѡ����    
	ProUIListNamesSet(dialog,component,count,names);
    //�����б��ĸ��б�ǩ
    ProUIListLabelsSet(dialog,component,count,labels);
	//�ͷ��ڴ�
	ProArrayFree((ProArray *)&names);
    ProArrayFree((ProArray *)&labels);

}


/*==================================================================*\
  	FUNCTION : ParamsList_Action ("ParamsList"�Ķ�����������)
\*==================================================================*/
void ParamsList_Action(char *dialog, char *component, ProAppData data)
{
	char** names;
    int n_names,i;
	ProParamvalue value;
	ProCharLine strtmp;
	ProName wstrValue,wstrType;
	ProArrayAlloc(0, sizeof(char *), 1, (ProArray *)&names);     
    // ȷ��ѡ����б��ѡ����
	ProUIListSelectednamesGet(dialog,component,&n_names,&names);	
	if (n_names!=1)
	{		
		return;
	}
	i=atoi(names[0])-1;
    //�ͷ��ڴ�
    ProArrayFree((ProArray *)&names);
	//���ָ��������ָ��
    ProParameterValueGet(p_data+i, &value);
	switch( value.type )
    {
         case PRO_PARAM_DOUBLE:
             sprintf(strtmp,"%0f",value.value.d_val);
             ProStringToWstring(wstrType,"PRO_PARAM_DOUBLE");
			 break;
         case PRO_PARAM_STRING:
             ProWstringToString(strtmp, value.value.s_val);
             ProStringToWstring(wstrType,"PRO_PARAM_STRING");
             break;
         case PRO_PARAM_INTEGER:
             sprintf(strtmp, "%0d", value.value.i_val);
             ProStringToWstring(wstrType,"PRO_PARAM_INTEGER");
			 break;
         case PRO_PARAM_BOOLEAN:
             sprintf(strtmp, "%0d", value.value.l_val);
             ProStringToWstring(wstrType,"PRO_PARAM_BOOLEAN");
             break;
         default:
             sprintf(strtmp, "NULL");
	}
    ProStringToWstring(wstrValue,strtmp);  
	//�������Param_Name����ʾ������
    ProUIInputpanelValueSet(dialog,"Param_Name",p_data[i].id);
	//�������Param_Value����ʾ����ֵ
    ProUIInputpanelValueSet(dialog,"Param_Value",wstrValue);
    //��ѡ��˵�����ʾ����
	//ProUIOptionmenuValueSet(dialog,"Param_ValueType",wstrType);
	
}
/*==================================================================*\
  	FUNCTION : AddParam_Action ("AddParam"��ť�Ķ�����������)
\*==================================================================*/
void AddParam_Action(char *dialog, char *component, ProAppData data)
{
	wchar_t* s_value,*v_value,*seletype;
	ProCharLine strtmpv,strtmp;
    ProMdl   model;
    ProModelitem modelitem;
	ProParamvalue proval;
	ProParameter param;
    ProMdlCurrentGet(&model);
	ProMdlToModelitem(model, &modelitem);

	ProUIInputpanelValueGet(dialog,"Param_Name",&s_value);
   //���ò���ֵ֮

    ProUIInputpanelValueGet(dialog,"Param_Value",&v_value);
	ProWstringToString(strtmpv,v_value);
	//���ò�������
	ProUIOptionmenuValueGet(dialog,"Param_ValueType",&seletype);
	ProWstringToString(strtmp,seletype);

    if (strcmp(strtmp,"PRO_PARAM_DOUBLE")==0){
        proval.type=PRO_PARAM_DOUBLE;
        proval.value.d_val=atof(strtmpv);
	}
	else if(strcmp(strtmp,"PRO_PARAM_STRING")==0){
	    proval.type=PRO_PARAM_STRING;
	    ProStringToWstring(proval.value.s_val,strtmpv);
	}
	else if(strcmp(strtmp,"PRO_PARAM_INTEGER")==0){
	    proval.type=PRO_PARAM_INTEGER;
        proval.value.i_val=atoi(strtmpv);
	}
	ProParameterCreate(&modelitem,s_value,&proval, &param);
    //�����б���ǩ
	ListLabelsSet(dialog,"ParamsList");
}
/*==================================================================*\
  	FUNCTION : DelParam_Action ("DelParam"��ť�Ķ�����������)
\*==================================================================*/
void DelParam_Action(char *dialog, char *component, ProAppData data)
{
	char** names;
    int n_names,i;
	ProArrayAlloc(0, sizeof(char *), 1, (ProArray *)&names);     
    // ȷ��ѡ����б��ѡ����
	ProUIListSelectednamesGet(dialog,"ParamsList",&n_names,&names);	
	if (n_names!=1)
	{		
		return;
	}
	i=atoi(names[0])-1;
    //�ͷ��ڴ�
    ProArrayFree((ProArray *)&names);
	ProParameterDelete(p_data+i);
    //�����б���ǩ
	ListLabelsSet(dialog,"ParamsList");
}
/*==================================================================*\
  	FUNCTION : Param_Value_Action ("Param_Value"�����Ķ�����������)
\*==================================================================*/
void Param_Value_Action(char *dialog, char *component, ProAppData data)
{
     Param_Update(dialog,component);
}
/*==================================================================*\
  	FUNCTION : Param_Update (���²���ֵ)
\*==================================================================*/
void Param_Update(char *dialog, char *component)
{
    wchar_t *ParamName,*NewValue;
	ProParameter param;
	ProMdl   model;
    ProModelitem modelitem;
	ProParamvalue value;
	ProCharLine strtmp;
	ProMdlCurrentGet(&model);
	ProMdlToModelitem(model, &modelitem);
	ProUIInputpanelValueGet(dialog,"Param_Name",&ParamName);
    ProParameterInit(&modelitem,ParamName,&param);
    //����ָ���Ĳ�������ò�������ָ��
	ProUIInputpanelValueGet(dialog,component,&NewValue);
	ProWstringToString(strtmp,NewValue);
    //��ò���ֵ
	ProParameterValueGet(&param, &value);
    switch( value.type )
     {
        case PRO_PARAM_DOUBLE:
    	      value.value.d_val=atof(strtmp);
	          break;
        case PRO_PARAM_INTEGER:
    	      value.value.i_val=atoi(strtmp);
              break;
        case PRO_PARAM_STRING:
	          ProStringToWstring(value.value.s_val,strtmp);
              break;
	} 
	//�µĲ���ֵ����
	ProParameterValueSet(&param, &value);
}

