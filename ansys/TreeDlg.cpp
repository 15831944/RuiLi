// TreeDlg.cpp : 实现文件
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
// CTreeDlg 对话框

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




// CTreeDlg 消息处理程序


BOOL CTreeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->OnBnClickedButton1();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CTreeDlg::OnBnClickedButton1()
{
	HTREEITEM hitem1,hitem11,hitem12,hitem121,hitem122,hitem13,hitem2,
     hitem3,hitem32,hitem33,hitem331,hitem332,hitem34,hitem4;
	HTREEITEM temp;
	hitem1=m_Tree.InsertItem("弹簧制动气室",NULL);
	m_Tree.InsertItem("膜片有效面积计算",hitem1);
	m_Tree.InsertItem("推杆长度计算",hitem1);
	hitem11=m_Tree.InsertItem("圆柱压缩弹簧计算",hitem1);
    m_Tree.InsertItem("弹簧工作圈数计算",hitem11);
	temp=m_Tree.InsertItem("弹簧载荷计算",hitem11);
	m_Tree.InsertItem("弹簧刚度计算",hitem11);
	m_Tree.InsertItem("弹簧稳定性计算",hitem11);
	m_Tree.InsertItem("弹簧疲劳强度校核",hitem11);
    hitem12=m_Tree.InsertItem("塔形弹簧计算",hitem1);
	hitem121=m_Tree.InsertItem("等节距塔形弹簧",hitem12);
	m_Tree.InsertItem("弹簧工作圈数计算",hitem121);
	m_Tree.InsertItem("弹簧载荷计算",hitem121);
	//m_Tree.InsertItem("弹簧疲劳强度校核",hitem121);
	hitem122=m_Tree.InsertItem("等螺旋角塔形弹簧",hitem12);
	m_Tree.InsertItem("弹簧工作圈数计算",hitem122);
	m_Tree.InsertItem("弹簧载荷计算",hitem122);
	//m_Tree.InsertItem("弹簧疲劳强度校核",hitem122);
	hitem13=m_Tree.InsertItem("腰鼓形弹簧计算",hitem1);
	m_Tree.InsertItem("弹簧工作圈数计算",hitem13);
    m_Tree.InsertItem("弹簧载荷计算",hitem13);
    //m_Tree.InsertItem("弹簧疲劳强度校核",hitem13);
	m_Tree.InsertItem("焊接部位强度分析",hitem1);
	hitem2=m_Tree.InsertItem("转向助力泵",NULL);
	//m_Tree.InsertItem("产品性能曲线绘制",hitem2);
	//m_Tree.InsertItem("泵体强度校核",hitem2);
	m_Tree.InsertItem("弹簧疲劳强度校核",hitem2);
	m_Tree.InsertItem("齿轮参数计算",hitem2);
	//m_Tree.InsertItem("更多齿轮参数计算",hitem2);
	hitem3=m_Tree.InsertItem("离合器助力器",NULL);
	m_Tree.InsertItem("推杆长度计算",hitem3);
	hitem32=m_Tree.InsertItem("圆柱压缩弹簧计算",hitem3);
	m_Tree.InsertItem("弹簧工作圈数计算",hitem32);
	temp=m_Tree.InsertItem("弹簧载荷计算",hitem32);
	m_Tree.InsertItem("弹簧刚度计算",hitem32);
	m_Tree.InsertItem("弹簧稳定性计算",hitem32);
	m_Tree.InsertItem("弹簧疲劳强度校核",hitem32);
	hitem33=m_Tree.InsertItem("塔形弹簧计算",hitem3);
	hitem331=m_Tree.InsertItem("等节距塔形弹簧",hitem33);
	m_Tree.InsertItem("弹簧工作圈数计算",hitem331);
	m_Tree.InsertItem("弹簧载荷计算",hitem331);
	//m_Tree.InsertItem("弹簧疲劳强度校核",hitem331);
	hitem332=m_Tree.InsertItem("等螺旋角塔形弹簧",hitem33);
	m_Tree.InsertItem("弹簧工作圈数计算",hitem332);
	m_Tree.InsertItem("弹簧载荷计算",hitem332);
	//m_Tree.InsertItem("弹簧疲劳强度校核",hitem332);
	hitem34=m_Tree.InsertItem("钩形拉伸弹簧计算",hitem3);
	m_Tree.InsertItem("弹簧工作圈数计算",hitem34);
	m_Tree.InsertItem("弹簧载荷计算",hitem34);
	m_Tree.InsertItem("弹簧刚度计算",hitem34);
    m_Tree.InsertItem("弹簧疲劳强度校核",hitem34);
    hitem4=m_Tree.InsertItem("制动总泵分泵",NULL);
	//m_Tree.InsertItem("制动缸壳体有限元分析",hitem4);
	m_Tree.InsertItem("螺纹强度计算校核",hitem4);
	m_Tree.InsertItem("杆件强度计算校核",hitem4);
	m_Tree.InsertItem("弹簧疲劳强度校核",hitem4);
    
}

void CTreeDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM item;
	item=m_Tree.GetSelectedItem();
    CString text;
	text=m_Tree.GetItemText(item);
	if(text=="膜片有效面积计算")
	{
		 CAreaDlg areadlg;
		 areadlg.DoModal();
	}
	else if (text=="推杆长度计算") 
	{
		HTREEITEM parentItem;
		parentItem=m_Tree.GetParentItem(item);
		text=m_Tree.GetItemText(parentItem);
		if(text=="弹簧制动气室")
		{
            CHandSpikeDlg dlg;
	        dlg.DoModal();
		}
		if(text=="离合器助力器")
		{
			CHandSpike1Dlg dlg;
			dlg.DoModal();
		}
	}
	else if (text=="弹簧工作圈数计算") 
	{
		HTREEITEM parentItem;
		parentItem=m_Tree.GetParentItem(item);
		text=m_Tree.GetItemText(parentItem);
		if(text=="圆柱压缩弹簧计算")
		{
			CWorkNumDlg dlg;
			dlg.DoModal();
		}
		if(text=="等节距塔形弹簧")
		{
			CWorkNum0Dlg dlg;
			dlg.DoModal();
		}
		if(text=="等螺旋角塔形弹簧")
		{
			CWorkNum0Dlg dlg;
			dlg.DoModal();
		}
		if(text=="腰鼓形弹簧计算")
		{
			CWorkNum1Dlg dlg;
			dlg.DoModal();
		}
		if(text=="钩形拉伸弹簧计算")
		{
			CWorkNumDlg dlg;
			dlg.DoModal();
		}
	}
	else if (text=="弹簧载荷计算") 
	{
		
		HTREEITEM parentItem;
		parentItem=m_Tree.GetParentItem(item);
        text=m_Tree.GetItemText(parentItem);
		if(text=="圆柱压缩弹簧计算")
		{
	    CLoadDlg dlg;
		dlg.DoModal();
		}
		if(text=="等节距塔形弹簧")
		{
			CLoad1Dlg dlg;
			dlg.DoModal();
		}
		if(text=="等螺旋角塔形弹簧")
		{
			CLoad0Dlg dlg;
			dlg.DoModal();
		}
		if(text=="腰鼓形弹簧计算")
		{
			CLoad2Dlg dlg;
			dlg.DoModal();
		}
		if(text=="钩形拉伸弹簧计算")
		{
			CLoadDlg dlg;
			dlg.DoModal();
		}
	}
	else if (text=="弹簧刚度计算") 
	{
		/*HTREEITEM parentItem;
		parentItem=m_Tree.GetParentItem(item);
		text=m_Tree.GetItemText(parentItem);
		if(text=="圆柱压缩弹簧计算")
		{*/
			CRigidityDlg dlg;
		    dlg.DoModal();
		/*}*/
	}
	else if (text=="弹簧稳定性计算") 
	{
		HTREEITEM parentItem;
		parentItem=m_Tree.GetParentItem(item);
		text=m_Tree.GetItemText(parentItem);
		if(text=="圆柱压缩弹簧计算")
		{
			CStabilityDlg dlg;
		    dlg.DoModal();
		}
	}
	else if(text=="焊接部位强度分析")
	{

		CJointDlg dlg;
		dlg.DoModal();
		
	}
	else if(text=="泵体强度校核")
	{

		this->OpenModel();

	}
	else if(text=="弹簧疲劳强度校核")
	{

		CFatigueDlg dlg;
		dlg.DoModal();

	}
	else if(text=="制动缸壳体有限元分析")
	{

		this->OpenModel();

	}
	else if(text=="螺纹强度计算校核")
	{

		CWormDlg dlg;
        dlg.DoModal();

	}
	else if(text=="杆件强度计算校核")
	{

		CPoleDlg dlg;
		dlg.DoModal();

	}
	else if(text=="弹簧疲劳强度校核")
	{

		CFatigueDlg dlg;
		dlg.DoModal();

	}
	else if(text=="疲劳寿命验证")
	{

		this->OpenModel();

	}
	else if(text=="产品性能曲线绘制")
	{

		CString path_to_folder;
		path_to_folder="Table.exe";
		ShellExecute(this->GetSafeHwnd(), NULL, path_to_folder, NULL, NULL, SW_SHOWNORMAL);

	}
	/*else if(text=="螺纹强度计算校核")
	{

		this->OpenModel();

	}*/
	else if(text=="齿轮参数计算")
	{
		CGearDlg dlg;
		dlg.DoModal();
	}

	else if(text=="更多齿轮参数计算")
	{
		CString path_to_folder;
		path_to_folder="jisuan.exe";
		ShellExecute(this->GetSafeHwnd(), NULL, path_to_folder, NULL, NULL, SW_SHOWNORMAL);
	}
	*pResult = 0;
}



void CTreeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
//	CDialog::DestroyWindow();
	//this->SendMessage(WM_CLOSE,0,0);
	OnOK();
}

void CTreeDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialog::DestroyWindow();
	//this->SendMessage(WM_CLOSE,0,0);
	OnCancel();
}


int CTreeDlg::OpenModel()
{

CString filePath;///
ProError  status;
static char BASED_CODE szFilter[] = "零件(*.prt)|*.prt*|All Files (*.*)|*.*||";
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
	AfxMessageBox("显示失败");
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
