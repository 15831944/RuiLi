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
ProParameter *p_data; //参数对象指针数组（全局变量）
extern CRelParaEditDlg dlg;
/*--------------------------------------------------------------------*\
Functions declaration  函数声明
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
 装入对话框资源
\*----------------------------------------------------------------*/	
	status=ProUIDialogCreate(dialog_name,dialog_name ); 
	   if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("UIDialog CreateCreate error!");
		return status;
	}
	//设置列表框标签
	ListLabelsSet(dialog_name,"ParamsList");
/*----------------------------------------------------------------*\
  设置对话框指定元件的动作函数
\*----------------------------------------------------------------*/	

//设置ParamsList选项的动作函数	
	ProUIListSelectActionSet(dialog_name, "resourceParamsList",  
						 ParamsList_Action, NULL);
//设置Cancel按钮的动作函数	
	ProUIPushbuttonActivateActionSet (dialog_name, "Cancel",  
						 CANCEL_Action, NULL); 
//设置AddParam按钮的动作函数	
	ProUIPushbuttonActivateActionSet (dialog_name, "AddParam",  
						 AddParam_Action, NULL); 
//设置DelParam按钮的动作函数	
	ProUIPushbuttonActivateActionSet (dialog_name, "DelParam",  
						 DelParam_Action, NULL); 
//设置Regen按钮的动作函数	
	ProUIPushbuttonActivateActionSet (dialog_name, "Regen",  
						 Regen_Action, NULL); 
//设置Param_Value输入框的动作函数	
	ProUIInputpanelActivateActionSet(dialog_name, "Param_Value",  
						 Param_Value_Action, NULL); 
/*----------------------------------------------------------------*\
	显示和激活对话框
\*----------------------------------------------------------------*/	
   
	status=ProUIDialogActivate(dialog_name,&ActiveDialog_status);
	
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("UIDialogActivate error!");
		return status;
	}
/*----------------------------------------------------------------*\
    从内存清除对话框资源  
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
  	FUNCTION : CANCEL_Action ("CANCEL"按钮的动作函数定义)
\*==================================================================*/
void CANCEL_Action(char *dialog, char *component, ProAppData data)
{
   	ProArrayFree((ProArray*)&p_data);
	ProUIDialogExit(dialog, CANCEL);  
	
}
/*==================================================================*\
  	FUNCTION : Regen_Action ("Regen"按钮的动作函数定义)
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
    Function:	ParamsGetGet() 获得当前模型项的全部参数对象
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
    Function:	ParameterActionFn() 参数访问动作函数
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
    Function:	ParameterFilterFn() 参数访问过滤函数
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
FUNCTION: ListLabesSet()  设置列表框之值
\*================================================================*/
void ListLabelsSet(char *dialog, char *component)
{
    ProError status;
    //ProParameter *p_data;
    int count;			
	char ** names;				//列表框名数组
    wchar_t ** labels;			//列表框标签数组
    ProCharLine strtmp;
	//获得当前模型所有参数
 	ParamsGet(&p_data);
	//获得窗口的总数
    status = ProArraySizeGet((ProArray)p_data, &count);
    //为列表框名及标签数组分配内存
    ProArrayAlloc (count, sizeof(wchar_t *),1,(ProArray*)&labels);
    ProArrayAlloc (count, sizeof(char *),1,(ProArray*)&names);    
    //数组中各元素初始化
    for (int i=0;i<count;i++){
	    names[i] = (char *)calloc(PRO_NAME_SIZE, sizeof(char));
	    labels[i] = (wchar_t *)calloc(PRO_NAME_SIZE, sizeof(wchar_t));	    
	    sprintf(strtmp,"%0d",i+1);
	    //列表框选项名赋值:1,2,3,...
	    strcpy(names[i],strtmp);
   		//获得参数名
        ProWstringToString(strtmp, p_data[i].id);
		ProStringToWstring(labels[i], strtmp);
    }
    //设置列表框List1的选项名    
	ProUIListNamesSet(dialog,component,count,names);
    //设置列表框的各行标签
    ProUIListLabelsSet(dialog,component,count,labels);
	//释放内存
	ProArrayFree((ProArray *)&names);
    ProArrayFree((ProArray *)&labels);

}


/*==================================================================*\
  	FUNCTION : ParamsList_Action ("ParamsList"的动作函数定义)
\*==================================================================*/
void ParamsList_Action(char *dialog, char *component, ProAppData data)
{
	char** names;
    int n_names,i;
	ProParamvalue value;
	ProCharLine strtmp;
	ProName wstrValue,wstrType;
	ProArrayAlloc(0, sizeof(char *), 1, (ProArray *)&names);     
    // 确定选择的列表框选项名
	ProUIListSelectednamesGet(dialog,component,&n_names,&names);	
	if (n_names!=1)
	{		
		return;
	}
	i=atoi(names[0])-1;
    //释放内存
    ProArrayFree((ProArray *)&names);
	//获得指定参数的指针
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
	//在输入框Param_Name中显示参数名
    ProUIInputpanelValueSet(dialog,"Param_Name",p_data[i].id);
	//在输入框Param_Value中显示参数值
    ProUIInputpanelValueSet(dialog,"Param_Value",wstrValue);
    //在选项菜单中显示类型
	//ProUIOptionmenuValueSet(dialog,"Param_ValueType",wstrType);
	
}
/*==================================================================*\
  	FUNCTION : AddParam_Action ("AddParam"按钮的动作函数定义)
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
   //设置参数之值

    ProUIInputpanelValueGet(dialog,"Param_Value",&v_value);
	ProWstringToString(strtmpv,v_value);
	//设置参数类型
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
    //设置列表框标签
	ListLabelsSet(dialog,"ParamsList");
}
/*==================================================================*\
  	FUNCTION : DelParam_Action ("DelParam"按钮的动作函数定义)
\*==================================================================*/
void DelParam_Action(char *dialog, char *component, ProAppData data)
{
	char** names;
    int n_names,i;
	ProArrayAlloc(0, sizeof(char *), 1, (ProArray *)&names);     
    // 确定选择的列表框选项名
	ProUIListSelectednamesGet(dialog,"ParamsList",&n_names,&names);	
	if (n_names!=1)
	{		
		return;
	}
	i=atoi(names[0])-1;
    //释放内存
    ProArrayFree((ProArray *)&names);
	ProParameterDelete(p_data+i);
    //设置列表框标签
	ListLabelsSet(dialog,"ParamsList");
}
/*==================================================================*\
  	FUNCTION : Param_Value_Action ("Param_Value"输入框的动作函数定义)
\*==================================================================*/
void Param_Value_Action(char *dialog, char *component, ProAppData data)
{
     Param_Update(dialog,component);
}
/*==================================================================*\
  	FUNCTION : Param_Update (更新参数值)
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
    //根据指定的参数名获得参数对象指针
	ProUIInputpanelValueGet(dialog,component,&NewValue);
	ProWstringToString(strtmp,NewValue);
    //获得参数值
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
	//新的参数值设置
	ProParameterValueSet(&param, &value);
}

