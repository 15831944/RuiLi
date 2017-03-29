#include "stdafx.h"
#include "variantdesign.h"
#include <ProSolid.h>
#include <ProWindows.h>
#include <ProUtil.h>
#include "ProMdl.h"
#include "ProObjects.h"
#include "ModelAlterDlg.h"

//调用模型列表对话框


CModelAlterDlg  modelalterdlg;
int AlterAction()
{
	ProSolid solid;
	ProError status;
	int err;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	status=ProMdlCurrentGet((ProMdl*)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("对不起,您还没有打开模型!");
		return FALSE;
	}
	err=modelalterdlg.Create(IDD_DIALOG_ALTER,NULL);
	if(err==0)
	{
		AfxMessageBox("对话框创建失败!");
		return err;
	}
    modelalterdlg.ShowWindow(SW_SHOW);
	return err;
}