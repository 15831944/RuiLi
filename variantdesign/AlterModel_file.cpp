#include "stdafx.h"
#include "variantdesign.h"
#include <ProSolid.h>
#include <ProWindows.h>
#include <ProUtil.h>
#include "ProMdl.h"
#include "ProObjects.h"
#include "ModelAlterDlg.h"

//����ģ���б�Ի���


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
		AfxMessageBox("�Բ���,����û�д�ģ��!");
		return FALSE;
	}
	err=modelalterdlg.Create(IDD_DIALOG_ALTER,NULL);
	if(err==0)
	{
		AfxMessageBox("�Ի��򴴽�ʧ��!");
		return err;
	}
    modelalterdlg.ShowWindow(SW_SHOW);
	return err;
}