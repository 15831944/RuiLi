#include "stdafx.h"
#include "variantdesign.h"
#include "UploadDlg.h"

//产品检查对话框
CUploadDlg  *uploaddlg=NULL;

int ProductChectAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(uploaddlg!=NULL)
	{
		delete uploaddlg;
	}
	uploaddlg=new CUploadDlg;
	int status=0;
	status=uploaddlg->Create(IDD_DIALOG_UPLOAD,NULL);
	if(status==0)
	{
		AfxMessageBox("对话框创建失败!");
		return status;
	}
	uploaddlg->ShowWindow(SW_SHOW);
	return status;
}



//产品入库对话框

int ProductUpLoadAction()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
 //   CAddProductDlg addproductdlg;
	//addproductdlg.DoModal();
	return 1;
}