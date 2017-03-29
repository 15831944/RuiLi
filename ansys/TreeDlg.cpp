// TreeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "myproe.h"
#include "ansys.h"
#include "TreeDlg.h"
#include "Load0Dlg.h"
#include "WorkNum0Dlg.h"
#include"AreaDlg.h"
#include"HandSpikeDlg.h"
#include"WorkNumDlg.h"
#include"LoadDlg.h"
#include"RigidityDlg.h"
#include"StabilityDlg.h"
#include"Load0Dlg.h"
#include"Load1Dlg.h"
#include"Load2Dlg.h"
#include"WorkNum1Dlg.h"
#include"HandSpike1Dlg.h"
#include "GearDlg.h"
#include "TestDlg.h"
#include "FatigueDlg.h"
#include "PoleDlg.h"
#include "WormDlg.h"
#include "JointDlg.h"
// CTreeDlg �Ի���

IMPLEMENT_DYNAMIC(CTreeDlg, CDialog)
CTreeDlg::CTreeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreeDlg::IDD, pParent)
{
}

CTreeDlg::~CTreeDlg()
{
}

void CTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
}


BEGIN_MESSAGE_MAP(CTreeDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnNMDblclkTree1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()




// CTreeDlg ��Ϣ�������


BOOL CTreeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->OnBnClickedButton1();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CTreeDlg::OnBnClickedButton1()
{
	HTREEITEM hitem1,hitem11,hitem12,hitem121,hitem122,hitem13,hitem2,
     hitem3,hitem32,hitem33,hitem331,hitem332,hitem34,hitem4;
	HTREEITEM temp;
	hitem1=m_Tree.InsertItem("�����ƶ�����",NULL);
	m_Tree.InsertItem("ĤƬ��Ч�������",hitem1);
	m_Tree.InsertItem("�Ƹ˳��ȼ���",hitem1);
	hitem11=m_Tree.InsertItem("Բ��ѹ�����ɼ���",hitem1);
    m_Tree.InsertItem("���ɹ���Ȧ������",hitem11);
	temp=m_Tree.InsertItem("�����غɼ���",hitem11);
	m_Tree.InsertItem("���ɸնȼ���",hitem11);
	m_Tree.InsertItem("�����ȶ��Լ���",hitem11);
	m_Tree.InsertItem("����ƣ��ǿ��У��",hitem11);
    hitem12=m_Tree.InsertItem("���ε��ɼ���",hitem1);
	hitem121=m_Tree.InsertItem("�Ƚھ����ε���",hitem12);
	m_Tree.InsertItem("���ɹ���Ȧ������",hitem121);
	m_Tree.InsertItem("�����غɼ���",hitem121);
	//m_Tree.InsertItem("����ƣ��ǿ��У��",hitem121);
	hitem122=m_Tree.InsertItem("�����������ε���",hitem12);
	m_Tree.InsertItem("���ɹ���Ȧ������",hitem122);
	m_Tree.InsertItem("�����غɼ���",hitem122);
	//m_Tree.InsertItem("����ƣ��ǿ��У��",hitem122);
	hitem13=m_Tree.InsertItem("�����ε��ɼ���",hitem1);
	m_Tree.InsertItem("���ɹ���Ȧ������",hitem13);
    m_Tree.InsertItem("�����غɼ���",hitem13);
    //m_Tree.InsertItem("����ƣ��ǿ��У��",hitem13);
	m_Tree.InsertItem("���Ӳ�λǿ�ȷ���",hitem1);
	hitem2=m_Tree.InsertItem("ת��������",NULL);
	//m_Tree.InsertItem("��Ʒ�������߻���",hitem2);
	//m_Tree.InsertItem("����ǿ��У��",hitem2);
	m_Tree.InsertItem("����ƣ��ǿ��У��",hitem2);
	m_Tree.InsertItem("���ֲ�������",hitem2);
	//m_Tree.InsertItem("������ֲ�������",hitem2);
	hitem3=m_Tree.InsertItem("�����������",NULL);
	m_Tree.InsertItem("�Ƹ˳��ȼ���",hitem3);
	hitem32=m_Tree.InsertItem("Բ��ѹ�����ɼ���",hitem3);
	m_Tree.InsertItem("���ɹ���Ȧ������",hitem32);
	temp=m_Tree.InsertItem("�����غɼ���",hitem32);
	m_Tree.InsertItem("���ɸնȼ���",hitem32);
	m_Tree.InsertItem("�����ȶ��Լ���",hitem32);
	m_Tree.InsertItem("����ƣ��ǿ��У��",hitem32);
	hitem33=m_Tree.InsertItem("���ε��ɼ���",hitem3);
	hitem331=m_Tree.InsertItem("�Ƚھ����ε���",hitem33);
	m_Tree.InsertItem("���ɹ���Ȧ������",hitem331);
	m_Tree.InsertItem("�����غɼ���",hitem331);
	//m_Tree.InsertItem("����ƣ��ǿ��У��",hitem331);
	hitem332=m_Tree.InsertItem("�����������ε���",hitem33);
	m_Tree.InsertItem("���ɹ���Ȧ������",hitem332);
	m_Tree.InsertItem("�����غɼ���",hitem332);
	//m_Tree.InsertItem("����ƣ��ǿ��У��",hitem332);
	hitem34=m_Tree.InsertItem("�������쵯�ɼ���",hitem3);
	m_Tree.InsertItem("���ɹ���Ȧ������",hitem34);
	m_Tree.InsertItem("�����غɼ���",hitem34);
	m_Tree.InsertItem("���ɸնȼ���",hitem34);
    m_Tree.InsertItem("����ƣ��ǿ��У��",hitem34);
    hitem4=m_Tree.InsertItem("�ƶ��ܱ÷ֱ�",NULL);
	//m_Tree.InsertItem("�ƶ��׿�������Ԫ����",hitem4);
	m_Tree.InsertItem("����ǿ�ȼ���У��",hitem4);
	m_Tree.InsertItem("�˼�ǿ�ȼ���У��",hitem4);
	m_Tree.InsertItem("����ƣ��ǿ��У��",hitem4);
    
}

void CTreeDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM item;
	item=m_Tree.GetSelectedItem();
    CString text;
	text=m_Tree.GetItemText(item);
	if(text=="ĤƬ��Ч�������")
	{
		 CAreaDlg areadlg;
		 areadlg.DoModal();
	}
	else if (text=="�Ƹ˳��ȼ���") 
	{
		HTREEITEM parentItem;
		parentItem=m_Tree.GetParentItem(item);
		text=m_Tree.GetItemText(parentItem);
		if(text=="�����ƶ�����")
		{
            CHandSpikeDlg dlg;
	        dlg.DoModal();
		}
		if(text=="�����������")
		{
			CHandSpike1Dlg dlg;
			dlg.DoModal();
		}
	}
	else if (text=="���ɹ���Ȧ������") 
	{
		HTREEITEM parentItem;
		parentItem=m_Tree.GetParentItem(item);
		text=m_Tree.GetItemText(parentItem);
		if(text=="Բ��ѹ�����ɼ���")
		{
			CWorkNumDlg dlg;
			dlg.DoModal();
		}
		if(text=="�Ƚھ����ε���")
		{
			CWorkNum0Dlg dlg;
			dlg.DoModal();
		}
		if(text=="�����������ε���")
		{
			CWorkNum0Dlg dlg;
			dlg.DoModal();
		}
		if(text=="�����ε��ɼ���")
		{
			CWorkNum1Dlg dlg;
			dlg.DoModal();
		}
		if(text=="�������쵯�ɼ���")
		{
			CWorkNumDlg dlg;
			dlg.DoModal();
		}
	}
	else if (text=="�����غɼ���") 
	{
		
		HTREEITEM parentItem;
		parentItem=m_Tree.GetParentItem(item);
        text=m_Tree.GetItemText(parentItem);
		if(text=="Բ��ѹ�����ɼ���")
		{
	    CLoadDlg dlg;
		dlg.DoModal();
		}
		if(text=="�Ƚھ����ε���")
		{
			CLoad1Dlg dlg;
			dlg.DoModal();
		}
		if(text=="�����������ε���")
		{
			CLoad0Dlg dlg;
			dlg.DoModal();
		}
		if(text=="�����ε��ɼ���")
		{
			CLoad2Dlg dlg;
			dlg.DoModal();
		}
		if(text=="�������쵯�ɼ���")
		{
			CLoadDlg dlg;
			dlg.DoModal();
		}
	}
	else if (text=="���ɸնȼ���") 
	{
		/*HTREEITEM parentItem;
		parentItem=m_Tree.GetParentItem(item);
		text=m_Tree.GetItemText(parentItem);
		if(text=="Բ��ѹ�����ɼ���")
		{*/
			CRigidityDlg dlg;
		    dlg.DoModal();
		/*}*/
	}
	else if (text=="�����ȶ��Լ���") 
	{
		HTREEITEM parentItem;
		parentItem=m_Tree.GetParentItem(item);
		text=m_Tree.GetItemText(parentItem);
		if(text=="Բ��ѹ�����ɼ���")
		{
			CStabilityDlg dlg;
		    dlg.DoModal();
		}
	}
	else if(text=="���Ӳ�λǿ�ȷ���")
	{

		CJointDlg dlg;
		dlg.DoModal();
		
	}
	else if(text=="����ǿ��У��")
	{

		this->OpenModel();

	}
	else if(text=="����ƣ��ǿ��У��")
	{

		CFatigueDlg dlg;
		dlg.DoModal();

	}
	else if(text=="�ƶ��׿�������Ԫ����")
	{

		this->OpenModel();

	}
	else if(text=="����ǿ�ȼ���У��")
	{

		CWormDlg dlg;
        dlg.DoModal();

	}
	else if(text=="�˼�ǿ�ȼ���У��")
	{

		CPoleDlg dlg;
		dlg.DoModal();

	}
	else if(text=="����ƣ��ǿ��У��")
	{

		CFatigueDlg dlg;
		dlg.DoModal();

	}
	else if(text=="ƣ��������֤")
	{

		this->OpenModel();

	}
	else if(text=="��Ʒ�������߻���")
	{

		CString path_to_folder;
		path_to_folder="Table.exe";
		ShellExecute(this->GetSafeHwnd(), NULL, path_to_folder, NULL, NULL, SW_SHOWNORMAL);

	}
	/*else if(text=="����ǿ�ȼ���У��")
	{

		this->OpenModel();

	}*/
	else if(text=="���ֲ�������")
	{
		CGearDlg dlg;
		dlg.DoModal();
	}

	else if(text=="������ֲ�������")
	{
		CString path_to_folder;
		path_to_folder="jisuan.exe";
		ShellExecute(this->GetSafeHwnd(), NULL, path_to_folder, NULL, NULL, SW_SHOWNORMAL);
	}
	*pResult = 0;
}



void CTreeDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CDialog::DestroyWindow();
	//this->SendMessage(WM_CLOSE,0,0);
	OnOK();
}

void CTreeDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialog::DestroyWindow();
	//this->SendMessage(WM_CLOSE,0,0);
	OnCancel();
}


int CTreeDlg::OpenModel()
{

CString filePath;///
ProError  status;
static char BASED_CODE szFilter[] = "���(*.prt)|*.prt*|All Files (*.*)|*.*||";
CFileDialog  fdlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,NULL,0);
if(fdlg.DoModal()!=IDOK)
{
	return 0;
}
filePath=fdlg.GetPathName();//	

ProPath fullpath;
ProStringToWstring(fullpath,filePath.GetBuffer());
ProMdlType type=PRO_MDL_UNUSED ;
status=ProMdlLoad(fullpath,type,PRO_B_FALSE,(ProMdl*)&solid);
if(status!=PRO_TK_NO_ERROR)
  return 0;

int window_id ;
ProName object_name ;
ProType  object_type ;
ProMdlNameGet(solid,object_name);
ProMdlTypeGet(solid,(ProMdlType *)&object_type);
ProObjectwindowCreate(object_name,object_type,&window_id);
ProMdlWindowGet(solid,&window_id);
status=ProMdlDisplay(solid);
if(status!=PRO_TK_NO_ERROR)
{
	AfxMessageBox("��ʾʧ��");
	return 0;
}
status=ProWindowActivate(window_id);



ProMacro  w_macro;
CString strMacro;
//strMacro="~ Activate `main_dlg_cur` `Utilities.psh_rels`";
//strMacro="~ Activate `main_dlg_cur` `pshCompAssemble`";
strMacro="~ Select `main_dlg_cur` `Applications.rad_util_app_prt` \
		 1  `Mechanica`";
ProStringToWstring (w_macro,strMacro.GetBuffer());
status = ProMacroLoad(w_macro); 

strMacro="~ Activate `sim_unit_info` `ContinuePush`";
//strMacro="~ Activate `main_dlg_cur` `Utilities.psh_util_aux`";
ProStringToWstring (w_macro,strMacro.GetBuffer());
status = ProMacroLoad(w_macro); 

strMacro="~ Activate `main_dlg_cur` `ProCmdSimMechSol.sim_me`";
ProStringToWstring (w_macro,strMacro.GetBuffer());
status = ProMacroLoad(w_macro); 

strMacro="~ Activate `run` `run`";
ProStringToWstring (w_macro,strMacro.GetBuffer());
status = ProMacroLoad(w_macro); 

//strMacro="#STRUCTURE";
//ProStringToWstring (w_macro,strMacro.GetBuffer());
//status = ProMacroLoad(w_macro); 
return 1;
}
