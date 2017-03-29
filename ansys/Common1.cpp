/*
 *	全局公共函数定义
 */
#include "stdafx.h"
#include "myproe.h"
#include  <ProPopupmenu.h>
#include <ProSelbuffer.h>

#define OA 1  /* Standard OA in the menu system, typically grays out 
 the button when not usable */
#define PM 2 /* OA in a popup menu, typically remove the button when 
 not usable */
uiCmdAccessState HideMenuAccess(uiCmdAccessMode access_mode);
int ReplaceModelEx();
ProError UserPopupmenuNotification (ProMenuName name);
uiCmdAccessState ReplaceModelOA(uiCmdAccessMode mode);
uiCmdAccessState ReplaceModelPM(uiCmdCmdId id,ProAppData data,ProSelection* sels);
uiCmdAccessState ReplaceModel(ProSelection* sels,int mode);


int UsrSolidSelect(ProSolid *solid);

ProError FeatureVisitAction(ProFeature *p_object,ProError status,
							ProAppData app_data);
ProError FeatureFilterAction(ProFeature *p_object,
							 ProAppData app_data);
ProError FeatureAsmFilterAction(ProFeature *p_object,
								ProAppData app_data);
ProError  SgCollectFeat(ProSolid solid,			//(In)当前模型
						ProFeature **p_data,BOOL nFlag=TRUE);

ProError  FeatureCollectDim(ProFeature feature,			//(In)指定特征
							ProDimension **p_data);
ProError  SgCollectDim(ProSolid solid,			//(In)指定模型
					   ProDimension **p_data)	;//(In)尺寸对象数组

ProError FeatureDimensionVisitAction(ProDimension *p_object,ProError status,
									 ProAppData app_data) ;
ProError FeatureDimensionFilterAction(ProDimension *p_object,
									  ProAppData app_data) ;
/////////////////////////////////////////////



////全局函数
ProError ParamerterFilterAction(ProParameter *p_object,
								ProAppData app_data) ;
ProError ParamerterVisitAction(ProParameter *p_object,ProError status,
							   ProAppData app_data)  ;
ProError  sgCollectParameters( 
							  ProModelitem    *p_modelitem,   /* In:  The model item */
							  ProParameter    **p_parameters );
/*=========================================================================*\
Function:	sgCollectParameters() 访问参数的函数定义
\*=========================================================================*/
ProError  sgCollectParameters( 
							  ProModelitem    *p_modelitem,   /* In:  The model item */
							  ProParameter    **p_parameters  /* Out: ProArray with collected parameters.
															  The function allocates   
															  memory for this argument, but 
															  you must free it. To free 
															  the memory, call the function 
															  ProArrayFree() */
															  )
{
	ProError	    status;

	if( p_parameters != NULL )
	{
		status=ProArrayAlloc(0,sizeof(ProParameter),1,(ProArray*)p_parameters);

		if( status == PRO_TK_NO_ERROR )
		{
			status = ProParameterVisit( p_modelitem, 
				(ProParameterFilter)ParamerterFilterAction,
				(ProParameterAction)ParamerterVisitAction,
				(ProAppData)&p_parameters );
		}
		else
		{
			ProArrayFree( (ProArray*)p_parameters );
			*p_parameters = NULL;
		}

	}
	else
		status = PRO_TK_BAD_INPUTS;

	return (status);
}
/*=========================================================================*\
Function:	ParamerterVisitAction()参数对象访问函数的动作函数定义
\*=========================================================================*/
ProError ParamerterVisitAction(ProParameter *p_object,ProError status,
							   ProAppData app_data)  

{   
	ProArray * p_array;
	p_array = (ProArray*)((ProParameter**)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1,p_object );
	return (status);
}
/*=========================================================================*\
Function: ParamerterFilterAction 参数对象访问函数的过滤函数定义
\*=========================================================================*/
ProError ParamerterFilterAction(ProParameter *p_object,
								ProAppData app_data)  
{   

	return (PRO_TK_NO_ERROR);
}




//////////////////////////////////////////////
/*=========================================================================*\
Function:	SgCollectFeat() 获得指定模型的特征,调用下面的访问函数
\*=========================================================================*/
ProError  SgCollectFeat(ProSolid solid,			//(In)指定模型
						ProFeature **p_data,BOOL nFlag)	//(In)特征对象数组
{
	ProError status;
	if( p_data != NULL )
	{
		status=ProArrayAlloc(0,sizeof(ProFeature),1,(ProArray*)p_data);
		if( status == PRO_TK_NO_ERROR )
		{  //调用尺寸对象访问函数
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
Function:	FeatureVisitAction() 特征对象访问函数的动作函数定义
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
Function:FeatureFilterAction() 特征对象访问函数的过滤函数定义
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
Function: FeatureAsmFilterAction() 装配体特征对象访问函数的过滤函数定义
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
自定义函数，用来获得当前特征的尺寸对象指针，并保存在指针数组中
\*================================================================*/
ProError  FeatureCollectDim(ProFeature feature,			//(In)当前模型
							ProDimension **p_data)	
{
	ProError status;
	if( p_data != NULL )
	{
		status=ProArrayAlloc(0,sizeof(ProDimension),1,(ProArray*)p_data);
		if( status == PRO_TK_NO_ERROR )
		{  //调用尺寸对象访问函数
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
自定义函数，用来获得当前模型的尺寸对象指针，并保存在指针数组中
\*================================================================*/
ProError  SgCollectDim(ProSolid solid,			//(In)当前模型
					   ProDimension **p_data)	//(In)尺寸对象数组
{
	ProError status;
	if( p_data != NULL )
	{
		status=ProArrayAlloc(0,sizeof(ProDimension),1,(ProArray*)p_data);
		if( status == PRO_TK_NO_ERROR )
		{  //调用尺寸对象访问函数
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
Function:	FeatureDimensionVisitAction() 尺寸对象访问函数的动作函数定义
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
Function: FeatureDimensionFilterAction() 尺寸对象访问函数的过滤函数定义
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



/*====================================================================*\ 
FUNCTION : UsrSolidSelect()
PURPOSE  : Utility to select a solid using the file browser and retrieve
it if it is not already in session.
\*====================================================================*/ 
int UsrSolidSelect(
			   ProSolid *solid)
{
	ProName title;
	ProLine filter;
	ProPath wfile, wdir, wcurrent_dir;
	char file[PRO_PATH_SIZE], dir[PRO_PATH_SIZE],
		current_dir[PRO_PATH_SIZE];
	ProBoolean different_dir;
	char *slash, *root, *ext;
	ProFamilyName wroot;
	ProMdlType mtype;
	ProError status;

	/*--------------------------------------------------------------------*\     
	Ask the user to select a prt or asm file
	\*--------------------------------------------------------------------*/  
	ProStringToWstring(title,"Solid");
	ProStringToWstring(filter,"*.prt,*.asm");
	if(ProFileOpen(title, filter, NULL, NULL, NULL, NULL, wfile) !=
		PRO_TK_NO_ERROR)
		return(0);
	ProWstringToString(file, wfile);

	/*--------------------------------------------------------------------*\     
	Parse out the directory, and see if it is different from the current
	\*--------------------------------------------------------------------*/     
	different_dir = PRO_B_FALSE;
	if(slash = strrchr(file,'/'))
	{
		*slash = '\0';
		strcpy(dir, file);
		strcpy(file, ++slash);

		ProStringToWstring(wdir, dir);

		ProDirectoryCurrentGet(wcurrent_dir);
		ProWstringToString(current_dir, wcurrent_dir);
		if(strcmp(current_dir, dir))
			different_dir = PRO_B_TRUE;
	}
	/*--------------------------------------------------------------------*\     
	Parse out the file root name and model type 
	\*--------------------------------------------------------------------*/ 
	root = strtok(file,".");
	ProStringToWstring(wroot, root);
	ext = strtok(NULL,".");
	if(!strcmp(ext,"prt"))
		mtype = PRO_MDL_PART;
	else if(!strcmp(ext,"asm"))
		mtype = PRO_MDL_ASSEMBLY;
	else
		return(0);

	/*--------------------------------------------------------------------*\     
	If the solid is already in session, return with it 
	\*--------------------------------------------------------------------*/ 
	if(ProMdlInit(wroot, mtype, (ProMdl*)solid) == PRO_TK_NO_ERROR)
		return(0);

	/*--------------------------------------------------------------------*\     
	Move the to correct directory, and try to retieve the solid 
	\*--------------------------------------------------------------------*/ 
	if(different_dir)
		ProDirectoryChange(wdir);

	status = ProMdlRetrieve(wroot, mtype, (ProMdl*)solid);

	if(different_dir)
		ProDirectoryChange(wcurrent_dir);

	if(status != PRO_TK_NO_ERROR)
		return(0);
	return(1);
}



/*====================================================================*\
Function: UserPopupmenusSetup
Purpose : Setup popup menus for selected examples
\*====================================================================*/
int UserPopupmenusSetup()
{
	ProError status;
	uiCmdCmdId cmd_id;
	ProFileName messagefile;
	ProStringToWstring(messagefile, "AddMenu.txt");
	/*--------------------------------------------------------------------*\
	Create commands for Object/Action wrappers around old-style examples 
	that aren't setup for Object/Action
	\*--------------------------------------------------------------------*/
	status = ProCmdActionAdd ("ReplaceModelCM",
		(uiCmdCmdActFn)ReplaceModelEx,
		uiProe2ndImmediate, 
		ReplaceModelOA,
		PRO_B_FALSE, PRO_B_FALSE, &cmd_id);

	/*--------------------------------------------------------------------*\
	Add Object/Action commands to the model tree popup menu
	\*--------------------------------------------------------------------*/
	status = ProMenubarmenuPushbuttonAdd ("ActionMenu",
		"ReplaceModel",
		"ReplaceModelLabel",
		"ReplaceModelHelp",
		NULL, PRO_B_TRUE, 
		cmd_id, messagefile);
	/*--------------------------------------------------------------------*\
	Add the popup menu notification to the session
	\*--------------------------------------------------------------------*/
	status = ProNotificationSet (PRO_POPUPMENU_CREATE_POST,
		(ProFunction)UserPopupmenuNotification);
	//AfxMessageBox("start");
	return(0);
}
int HideMenu()
{
	uiCmdCmdId cmd_id;
	uiCmdAccessId  access_id;
	ProError status;
    status = ProCmdCmdIdFind ("ProCmdModelOpen", &cmd_id);
   // status=ProCmdAccessFuncRemove(cmd_id,access_id);
	status=ProCmdAccessFuncAdd(cmd_id,HideMenuAccess,&access_id);
	return 1;
}

uiCmdAccessState HideMenuAccess(uiCmdAccessMode access_mode)
{
	return (ACCESS_REMOVE);
	//return (ACCESS_UNAVAILABLE);
}

/*====================================================================*\
Function: UserPopupmenuNotification
Purpose: Add buttons to popup menus when they are created.
\*====================================================================*/
ProError UserPopupmenuNotification (ProMenuName name)
{
	ProPopupMenuId menu_id;
	uiCmdCmdId cmd_id;
	ProError status;
	ProLine label;
	ProLine help;
	ProFileName messagefile;
	ProStringToWstring(messagefile, "AddMenu.txt");

		/*--------------------------------------------------------------------*\
		Check if the menu being created matches the menu name we want.
		\*--------------------------------------------------------------------*/
		ProPopupmenuIdGet (name, &menu_id);

	if (strcmp (name, "Sel Obj Menu") == 0)
	{
		status = ProCmdCmdIdFind ("ReplaceModelCM", &cmd_id);

		/*--------------------------------------------------------------------*\
		Extract the button label and helptext from a message file.
		\*--------------------------------------------------------------------*/
		//status = ProMessageToBuffer (label, messagefile,
		//	"ReplaceModelLabel");
		//status = ProMessageToBuffer (help, messagefile,
		//	"ReplaceModelHelp");

		/*--------------------------------------------------------------------*\
		Add the button to the end of the popup menu.
		\*--------------------------------------------------------------------*/
		status = ProPopupmenuButtonAdd (menu_id, PRO_VALUE_UNUSED,
			"ReplaceModel",
			label, help,
			cmd_id,
			ReplaceModelPM,
			NULL);
	}

	return PRO_TK_NO_ERROR;
}

/*=====================================================================*\
FUNCTION: UserAsmcompConstraintsHighlight_TestMdlTree
PURPOSE:  Test function for access for constraint highlighting from model 
tree RMB popup menu.
\*=====================================================================*/
uiCmdAccessState ReplaceModelOA(uiCmdAccessMode mode)
{
	return ReplaceModel (NULL, PM);
}

/*=====================================================================*\
FUNCTION: UserAsmcompConstraintsHighlight_TestPM
PURPOSE: Test function for access for constraint highlighting from
graphics window RMB popup menu.
\*=====================================================================*/
uiCmdAccessState ReplaceModelPM(uiCmdCmdId id,
								ProAppData data,
								ProSelection* sels)
{
	return ReplaceModel (sels, PM);
}

/*=====================================================================*\
FUNCTION: UserAsmcompConstraintsHighlight_TestLow
PURPOSE: Test function for access for constraint highlighting
\*=====================================================================*/
uiCmdAccessState ReplaceModel(ProSelection* sels,
							  int mode)
{
	uiCmdAccessState access_result;
	ProBoolean should_free = PRO_B_FALSE;
	ProError status;
	int size;

	/*-----------------------------------------------------------------*\
	Set the default return if the button is unusable.
	\*-----------------------------------------------------------------*/  
	if (mode == OA)
		access_result = ACCESS_UNAVAILABLE;
	else 
		access_result = ACCESS_REMOVE;

	/*-----------------------------------------------------------------*\
	If called without selections, extract the current selections from
	the buffer.
	\*-----------------------------------------------------------------*/ 
	if (sels == NULL)
	{   
		status = ProSelbufferSelectionsGet (&sels);

		if (status != PRO_TK_NO_ERROR)
			return access_result;

		if (sels == NULL)
			return access_result;

		should_free = PRO_B_TRUE;
	}

	/*-----------------------------------------------------------------*\
	This command allows only one selection.
	\*-----------------------------------------------------------------*/   
	status = ProArraySizeGet (sels, &size);

	if (status != PRO_TK_NO_ERROR)
		return access_result;

	if (size == 1)
	{
		ProAsmcomp asmcomp;

		status = ProSelectionModelitemGet (sels [0], &asmcomp);

		/*-----------------------------------------------------------------*\
		If the selected type is feature,its feature type must be component.
		\*-----------------------------------------------------------------*/   
		if (asmcomp.type == PRO_FEATURE)
		{
			ProFeattype ftype;

			status = ProFeatureTypeGet (&asmcomp, &ftype);

			if (ftype == PRO_FEAT_COMPONENT)
			{
				access_result = ACCESS_AVAILABLE;
			}
		}

		/*-----------------------------------------------------------------*\
		If the selected type is part or assembly,it must have a parent
		assembly to use to construct the component feature handle.
		\*-----------------------------------------------------------------*/  
		if (asmcomp.type == PRO_PART || asmcomp.type == PRO_ASSEMBLY)
		{
			ProAsmcomppath path;

			status = ProSelectionAsmcomppathGet (sels [0], &path);

			if (path.table_num > 0)
			{
				access_result = ACCESS_AVAILABLE;
			}
		}
	}

	if (should_free)
		ProSelectionarrayFree (sels);
	return access_result;
}

int ReplaceModelEx()
{
	ProError status;
	ProSelection * sels;
	int size;
	status = ProSelbufferSelectionsGet (&sels);
	if(status!=PRO_TK_NO_ERROR)
		AfxMessageBox("debudao");
	status = ProArraySizeGet (sels, &size);
	if (size == 1)
	{
		ProAsmcomp asmcomp;
		status = ProSelectionModelitemGet (sels [0], &asmcomp);
		ProSolid newSolid;         
		if (asmcomp.type == PRO_FEATURE)
		{
			ProFeattype ftype;

			status = ProFeatureTypeGet (&asmcomp, &ftype);

			if (ftype == PRO_FEAT_COMPONENT)
			{
				status=ProAsmcompMdlGet(&asmcomp,(ProMdl *)&newSolid);
				AfxMessageBox("特征");
			}
		}
		
		if (asmcomp.type == PRO_PART || asmcomp.type == PRO_ASSEMBLY)
		{
			ProAsmcomppath path;

			status = ProSelectionAsmcomppathGet (sels [0], &path);
            status=ProAsmcomppathMdlGet(&path,(ProMdl *)&newSolid);
		}

		if(status==PRO_TK_NO_ERROR)
		{
			CString str;
			ProName Name;
			ProMdlType  mdlType;
			ProMdlNameGet(newSolid,Name);
			ProMdlTypeGet(newSolid,&mdlType);
			str=CString(Name);
			if(mdlType==PRO_MDL_ASSEMBLY)
			{
				str="装配体："+str+" 测试";
				AfxMessageBox(str);
			}
			else
			{
				str="零件："+str+" 测试";
				AfxMessageBox(str);
			}
		}
	}

	return 1;
}

#undef OA
#undef PM








