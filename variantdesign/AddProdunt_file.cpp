#include "stdafx.h"
#include "variantdesign.h"
#include "UploadDlg.h"

//��Ʒ���Ի���
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
		AfxMessageBox("�Ի��򴴽�ʧ��!");
		return status;
	}
	uploaddlg->ShowWindow(SW_SHOW);
	return status;
}



//��Ʒ���Ի���

int ProductUpLoadAction()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
 //   CAddProductDlg addproductdlg;
	//addproductdlg.DoModal();
	return 1;
}