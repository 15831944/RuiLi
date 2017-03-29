#include "stdafx.h"
#include  <ProPopupmenu.h>
#include <ProSelbuffer.h>
#include "variantdesign.h"
#include <ProFeature.h>
#include "ProFeatType.h"
#include <ProNotify.h>
#include <ProMessage.h>
#include "ProUtil.h"
#include "ProModelitem.h"
#include "CommonFuncClass.h"
#include "ReplaceDlg.h"
#include <ProAsmcomppath.h>

#define OA 1  /* Standard OA in the menu system, typically grays out 
 the button when not usable */
#define PM 2 /* OA in a popup menu, typically remove the button when 
 not usable */


CReplaceDlg  replacedlg;

int ReplaceModelEx();
ProError UserPopupmenuNotification (ProMenuName name);
uiCmdAccessState ReplaceModelOA(uiCmdAccessMode mode);
uiCmdAccessState ReplaceModelPM(uiCmdCmdId id,ProAppData data,ProSelection* sels);
uiCmdAccessState ReplaceModel(ProSelection* sels,int mode);


/*====================================================================*\
Function: UserPopupmenusSetup
Purpose : Setup popup menus for selected examples
\*====================================================================*/
int UserPopupmenusSetup()
{
	ProError status;
	uiCmdCmdId cmd_id;
	ProFileName messagefile;
	ProStringToWstring(messagefile, "Message.txt");
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
	return(0);
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
	ProStringToWstring(messagefile, "Message.txt");

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
		status = ProMessageToBuffer (label, messagefile,
			"ReplaceModelLabel");
		status = ProMessageToBuffer (help, messagefile,
			"ReplaceModelHelp");

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
uiCmdAccessState ReplaceModel(ProSelection* sels,int mode)
{
	uiCmdAccessState access_result;
	ProBoolean should_free = PRO_B_FALSE;
	ProError status;
	int size;
	CString PartNum;

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
	ProSelection *sels;
	ProError status;
	int size;
	ProSolid newSolid;
	ProAsmcomppath path;
	CCommonFuncClass commonfun;
	status = ProSelbufferSelectionsGet (&sels);
	status = ProArraySizeGet (sels, &size);
	if (status == PRO_TK_NO_ERROR && size==1)
	{
		ProAsmcomp asmcomp;
		status = ProSelectionModelitemGet (sels [0], &asmcomp);         
		if (asmcomp.type == PRO_FEATURE)
		{
			ProFeattype ftype;
			status = ProFeatureTypeGet (&asmcomp, &ftype);
			if (ftype == PRO_FEAT_COMPONENT)
			{
				status=ProAsmcompMdlGet(&asmcomp,(ProMdl *)&newSolid);
			}
		}
		if (asmcomp.type == PRO_PART || asmcomp.type == PRO_ASSEMBLY)
		{
			status = ProSelectionAsmcomppathGet (sels [0], &path);
			status=ProAsmcomppathMdlGet(&path,(ProMdl *)&newSolid);
		}
	}
	if(1)
	{
		AfxMessageBox("这个零件是非模型变换来的,无法为你替换!");
		return 0;
	}
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	replacedlg.partpath=path;
	replacedlg.solid=newSolid;
	replacedlg.ProductSolid=path.owner;
	replacedlg.Create(IDD_DIALOG_REPLACE,NULL);
	replacedlg.ShowWindow(SW_SHOW);
	return 1;
}
#undef OA
#undef PM