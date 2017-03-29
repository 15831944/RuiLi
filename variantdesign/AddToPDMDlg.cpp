// AddToPDMDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "AddToPDMDlg.h"


extern AddToPDMDlg *pdmdlg;
// AddToPDMDlg 对话框

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


// AddToPDMDlg 消息处理程序
//入库到PDM的参数配置按钮
void AddToPDMDlg::OnBnClickedButtonPar()
{
	// TODO: 在此添加控件通知处理程序代码
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
//入库到PDM操作按钮
void AddToPDMDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
