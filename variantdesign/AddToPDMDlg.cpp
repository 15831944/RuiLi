// AddToPDMDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "AddToPDMDlg.h"


extern AddToPDMDlg *pdmdlg;
// AddToPDMDlg �Ի���

IMPLEMENT_DYNAMIC(AddToPDMDlg, CDialog)
AddToPDMDlg::AddToPDMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AddToPDMDlg::IDD, pParent)
{
}

AddToPDMDlg::~AddToPDMDlg()
{
}

void AddToPDMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AddToPDMDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PAR, OnBnClickedButtonPar)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// AddToPDMDlg ��Ϣ�������
//��⵽PDM�Ĳ������ð�ť
void AddToPDMDlg::OnBnClickedButtonPar()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ProMacro macro;
	ProError status;	
	//CString name="~ Select `main_dlg_cur` `MenuBar1` \
	//			 1  `XTPDM`";
	//ProStringToWstring(macro,name.GetBuffer());
	//status=ProMacroLoad(macro);


	//name="~ Close `main_dlg_cur` `MenuBar1`";
	//ProStringToWstring(macro,name.GetBuffer());
	//status=ProMacroLoad(macro);

	CString name="~ Activate `main_dlg_cur` `MODELPARAMETER`";
	ProStringToWstring(macro,name.GetBuffer());
	status=ProMacroLoad(macro);
}
//��⵽PDM������ť
void AddToPDMDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ProMacro macro;
	ProError status;	
	//CString name="~ Select `main_dlg_cur` `MenuBar1` \
	//			 1  `XTPDM`";
	//ProStringToWstring(macro,name.GetBuffer());
	//status=ProMacroLoad(macro);


	//name="~ Close `main_dlg_cur` `MenuBar1`";
	//ProStringToWstring(macro,name.GetBuffer());
	//status=ProMacroLoad(macro);
    
	CString name="~ Activate `main_dlg_cur` `CHECKINTOPRODUCT`";
	ProStringToWstring(macro,name.GetBuffer());
	status=ProMacroLoad(macro);
	pdmdlg->ShowWindow(SW_SHOWNORMAL);
}

void AddToPDMDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
