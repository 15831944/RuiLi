#include "stdafx.h"
#include "variantdesign.h"
#include "SearchDlg.h"
#include "ModelQueryDlg.h"
#include "PartQueryDlg.h"
#include "AsmQueryDlg.h"
#include "PartSaveDlg.h"
#include "afxtempl.h"
#include "ProWindows.h"
#include "CheckDlg.h"
#include "SelectDlg.h"
#include "afxtempl.h"
#include "ProDrawing.h"
#include "MyDatabase.h"
#include "CommonFuncClass.h"
#include "Compute.h"
#include "FtpFile.h"
#include "ChooseProductDlg.h"
#include "ProMenu.h"
#include "ProductDelDlg.h"
#include "ProductSaveDlg.h"
#include "DetailInfoClass.h"
#include "PrtSaveAsDlg.h"
#include "PrtModifyDlg.h"
#include "PtdNewDlg.h"
#include "PdtModifyDlg.h"
#include "ChooseProductDlg.h"


extern CArray<ProDrawing,ProDrawing>  DrawingList;
void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern FtpInfo Ftp_struc;
extern CPtrArray p_classarray;


//���ʵ����ѯ����
int ProductSearchAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPdtModifyDlg pdtmodifydlg;
	pdtmodifydlg.DoModal();
	return 1;
}

//���ģ�Ͳ�ѯ����
int PartModelSearchAction()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CModelQueryDlg modelquerydlg;
   modelquerydlg.DoModal();
   return 1;
}

//���ʵ����ѯ����
int PartSearchAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	CPrtModifyDlg prtmodifydlg;
	prtmodifydlg.DoModal();
	return 1;
}
//��Ʒ�½�����
int AsmModelSearchAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPtdNewDlg ptdnewdlg;
	ptdnewdlg.DoModal();
	return 1;
}
//���ʵ�����溯��
int PartSaveAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPartSaveDlg partsavedlg;
	partsavedlg.DoModal();
	return 1;
}
//��ʾ��άͼֽ����
int DisPlayDwgAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProSolid solid;
	ProName w_name;
	ProMdlType p_type;
	ProError status;
	int w_id;
	status=ProMdlCurrentGet((ProMdl*)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("û�е�ǰ��ģ��");
		return FALSE;
	}
	DrawingList.RemoveAll();
	GetSolidDrawing(solid,&DrawingList);
	int n=(int)DrawingList.GetSize();
	if(n==1)
	{
		status=ProMdlNameGet(DrawingList[0],w_name);
		status=ProMdlTypeGet(DrawingList[0],&p_type);
		status=ProObjectwindowCreate(w_name,(ProType)p_type,&w_id);
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("�޷���ʾ��άͼ��");
			return 0;
		}
		status = ProMdlDisplay(DrawingList[0]);
		status= ProMdlWindowGet(DrawingList[0],&w_id);
		status = ProWindowActivate(w_id);
	    return 1;
	}
	if (n==0)
	{
		AfxMessageBox("û�и���άģ�͵Ķ�άͼֽ��");
		return 0;
	}
	return 1;
}
//ʵ����麯��
CCheckDlg checkdlg;
int CheckAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProSolid solid;
	ProError status;
	int w_id,s;
	CString str;
	CCommonFuncClass commonfun;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowMdlGet(w_id,(ProMdl *)&solid);
	s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("�޷��õ���������ģ����Ϣ��");
		return 0;
	}
	str=((DetailInfoClass *)p_classarray.GetAt(s))->DesignState;
	if (str=="δ���")
	{
		AfxMessageBox("��������û�������ɣ��������");
		return 0;
	}
	str=((DetailInfoClass *)p_classarray.GetAt(s))->AuditState;
	if (str=="���ͨ��")
	{
		AfxMessageBox("�������Ѿ�ͨ����ˣ��������");
		return 0;
	}
	checkdlg.Create(IDD_DIALOG3,NULL);
	checkdlg.ShowWindow(SW_SHOWNORMAL);
	return 1;
}
//���ɾ����������
int PartDeleteAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CSelectDlg selectdlg;
	selectdlg.DoModal();
	return 1;
}


//������涯������
int PartSaveAsAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPrtSaveAsDlg prtsaveasdlg;
	prtsaveasdlg.DoModal();
	return 1;
}
/************************************************************************/
/* ��Ʒ���涯������                                                     */
/************************************************************************/
int ProductSaveAction()
{
	ProError status;
	ProMdl   mdl;
	ProMdlType p_type;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CProductSaveDlg productsavedlg;
		status=ProMdlCurrentGet(&mdl);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�Բ���,����û�д�ģ��!");
		return 0;
	}
	status=ProMdlTypeGet(mdl,&p_type);
	if (p_type!=PRO_MDL_ASSEMBLY)
	{
		AfxMessageBox("�Բ��𣬵�ǰģ�Ͳ�����������ͣ�");
		return 0;
	}
	productsavedlg.DoModal();
	return 1;
}
/************************************************************************/
/* �ó��ڴ溯��                                                         */
/************************************************************************/
int EraseAction()
{
	ProMacro macro;
	ProError status;
	CString name="~ Activate `main_dlg_cur` `psh_erase_not_disp` ~ Activate `file_erase_nd` `ok_pb`";	
	ProStringToWstring(macro,name.GetBuffer());
	status=ProMacroLoad(macro);
	return 1;
}
/************************************************************************/
/* �رմ��ں���                                                         */
/************************************************************************/
int CloseWindowsAction()
{
	int w_id;
	ProWindowCurrentGet(&w_id);
	ProWindowActivate(w_id);
	ProWindowCurrentClose();
	return 1;
}
/************************************************************************/
/* ��Ʒɾ���Ի���                                                       */
/************************************************************************/
int ProductDeleteAction()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CProductDelDlg Productdeldlg;
	Productdeldlg.DoModal();
	return 1;
}