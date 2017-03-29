// ProductSaveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxtempl.h"
#include "variantdesign.h"
#include "ProductSaveDlg.h"
#include "ProMdl.h"
#include "ProSolid.h"
#include "ProUtil.h"
#include "ProWindows.h"
#include "ProductMenberBasicInfoDlg.h"
#include "CommonFuncClass.h"
#include "Compute.h"
#include "MyDatabase.h"
#include "FtpFile.h"
#include "UpdateDlg.h"
#include "DetailInfoClass.h"
#include "ProductClass.h"
#include "StyleStandDlg.h"
#include "SelectDlg.h"
#include "UserNameDlg.h"

// CProductSaveDlg 对话框
void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data,BOOL nFlag);

extern CArray <Database,Database> Database_struc_temp;
extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CArray <JuBing,JuBing> JuBing_struc_temp;
extern FtpInfo Ftp_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;
extern CStringArray NumArray;

IMPLEMENT_DYNAMIC(CProductSaveDlg, CDialog)
CProductSaveDlg::CProductSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProductSaveDlg::IDD, pParent)
	, m_inscode(_T(""))
	, m_insname(_T(""))
	, m_designer(_T(""))
	, m_mdlnum(_T(""))
	, m_group(_T(""))
	, m_note(_T(""))
	, m_place(_T(""))
	, m_drwingplace(_T(""))
	, m_filename(_T(""))
	, m_pdmcode(_T(""))
{
}

CProductSaveDlg::~CProductSaveDlg()
{
}

void CProductSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ASMMODEL, m_asmmodeltree);
	DDX_Text(pDX, IDC_EDIT_CODE, m_inscode);
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_insname);
	DDX_Text(pDX, IDC_DESIGNER, m_designer);
	DDX_Text(pDX, IDC_EDIT_MDLFRM, m_mdlnum);
	DDX_Text(pDX, IDC_EDIT_GRPFRM, m_group);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	DDX_Text(pDX, IDC_EDIT_PLACE, m_place);
	DDX_Text(pDX, IDC_EDIT_DRWPLACE, m_drwingplace);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
	DDX_Control(pDX, IDC_COMBO_FINISH, m_designstate);
	DDX_Control(pDX, IDC_LIST_PER, m_perf);
	DDX_Text(pDX, IDC_EDIT_CODE2, m_pdmcode);
}


BEGIN_MESSAGE_MAP(CProductSaveDlg, CDialog)
	ON_WM_MOVE()
	ON_NOTIFY(NM_CLICK, IDC_TREE_ASMMODEL, OnNMClickTreeAsmmodel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM, OnBnClickedButtonGrpfrm)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDLOCALPLACE, OnBnClickedButtonSolidlocalplace)
	ON_BN_CLICKED(IDC_BUTTON_DRWLOCALPLACE, OnBnClickedButtonDrwlocalplace)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDSERVERPLACE, OnBnClickedButtonSolidserverplace)
	ON_BN_CLICKED(IDC_BUTTON_DWGSERVERPLACE, OnBnClickedButtonDwgserverplace)
	ON_BN_CLICKED(IDC_BUTTON_de, OnBnClickedButtonde)
END_MESSAGE_MAP()


// CProductSaveDlg 消息处理程序

BOOL CProductSaveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CCommonFuncClass commonfun;
	ProError status;
	ProSolid solid;
	CProductClass pdtclass;
	//去掉不在内存的基本信息
	if(p_classarray.GetSize()>1)
	{
		//去掉进程中没有的基本信息
		commonfun.IsBasicInfoAlreadyHave();
		commonfun.RemoveNotInSession();
	}

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	m_ImageList.Create(16, 16, ILC_COLOR8, 0, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_asmmodeltree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	m_db.Close();
	if(m_db.ReOpen()==0)
	this->SendMessage(WM_CLOSE,0,0);

	status=ProMdlCurrentGet((ProMdl*)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("对不起，您没有打开装配模型！");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	ProductSolid=commonfun.ObtainTheTopSolid(solid);
	if (ProductSolid==NULL)
	{
		AfxMessageBox("这个部件同时被两个不同的产品使用，请关闭一个，再清除内存");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	commonfun.AddProductToTree(solid,&m_asmmodeltree);
	if(!commonfun.ObtianBasicInfoFromHandle(solid))				   //判断当前的零件的基本信息是否都存在
	{
		AfxMessageBox("无法得到，这个产品的详细信息，无法保存，如果是新产品，请用另存");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	//把需要重新填写基本信息
	int s;
	CString sql,str;
	//去掉不在当前进程中的结构；
	int index=m_designstate.AddString("完成");
	m_designstate.SetCurSel(index);
	m_designstate.AddString("未完成");
	s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	else
	{
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidID==-1)
		{
			AfxMessageBox("这个组件还没有保存过，请使用另存为按钮！");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
		m_mdlnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlName;
		m_inscode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
		m_pdmcode=((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode;
		m_insname=((DetailInfoClass *)p_classarray.GetAt(s))->InsName;
		m_designer=((DetailInfoClass *)p_classarray.GetAt(s))->Designer;
		m_group=((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm;
		m_note=((DetailInfoClass *)p_classarray.GetAt(s))->Note;
		m_filename=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
		m_drwingplace=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
		m_place=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
		//显示性能参数
		if(!pdtclass.ShowPerf(s,"asm",&m_perf))
		{
			AfxMessageBox("无法得到性能参数");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
	}
	if (commonfun.ObtainPartMap(solid)!="")                  //如果参数中没有图号，则显示内存中的图号，否则从文件的参数中获得
	{
		m_inscode=commonfun.ObtainPartMap(solid);
	}
	else
	{
		m_inscode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
	}
	if (ProductSolid!=NULL)
	{
		if (ProductSolid==solid)
		{
			IsOneOfProduct=FALSE;//是单个零件
		}
		else
			IsOneOfProduct=TRUE;
	}
	else
		AfxMessageBox("你打开了有零部件重叠的多个产品，请关掉一个");
	//是部件，先判断产品级是否已经保存
	if (IsOneOfProduct)
	{
		int Pos=commonfun.IsBasicInfoHave(ProductSolid);
		if (Pos==-1)
		{
			AfxMessageBox("找不到这个部件的产品的信息，无法更新");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
		else
			if (((DetailInfoClass *)p_classarray.GetAt(Pos))->SolidID==-1)
			{
				AfxMessageBox("还没有保存产品，请先保存产品");
				this->SendMessage(WM_CLOSE,0,0);
				return FALSE;
			}
	}
	//判断这个零件是否被多个产品所用
	if (!pdtclass.IsUsedByProduct(s,&BeUsed,"asm"))
	{
		AfxMessageBox("无法得到零件信息");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	UpdateData(FALSE);
	return TRUE;  
	// 异常：OCX 属性页应返回 FALSE
}

void CProductSaveDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRepaint(PRO_VALUE_UNUSED);
}

void CProductSaveDlg::OnNMClickTreeAsmmodel(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CProductSaveDlg::OnBnClickedButtonSave()
{
	//判断保存路径是否为空
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CString  Path,FileName,DrwName,str,sql;
	ProSolid solid;
	ProError status;
	BOOL IsLocal=FALSE;
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	status=ProMdlCurrentGet((ProMdl *)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("当前产品不正确！");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	if (m_place=="" || m_drwingplace=="" || m_filename=="" || m_inscode=="" || m_pdmcode=="")
	{
		AfxMessageBox("你的信息不完全，请先填完整");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	int k=m_place.Find(":");
	if (k==-1)		   //说明是服务器上的
	{
		if (k=m_drwingplace.Find(":"))
		{
			if (k!=-1)
			{
				AfxMessageBox("二维模型和三维模型保存的位置不一致！");
				GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
				return;
			}
			IsLocal=FALSE;
		}
	}
	else			 //在本地
	{
		if (k=m_drwingplace.Find(":"))
		{
			if (k==-1)
			{
				AfxMessageBox("二维模型和三维模型保存的位置不一致！");		//不能使保存一个在服务器上，一个在本地
				GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
				return;
			}
			IsLocal=TRUE;
		}
	}
	if (!commonfun.ObtianBasicInfoFromHandle(solid))
	{
		return;
	}
	int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("找不到这个组件的信息，无法更新");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	//判断图号是否正确
	m_filename.MakeLower();
	if (m_filename.Right(4)==".asm")
	{
		k=m_filename.ReverseFind('.');
		if (k!=-1)
		{
			FileName=m_filename.Left(k)+".asm";
			DrwName=m_filename.Left(k)+".drw";
		}
	}
	else// (k==-1)			 //说明没有后缀
	{
		FileName=m_filename+".asm";
		DrwName=m_filename+".drw";
	}
	//判断是否应该用插入
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0 && (!((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved) && ((DetailInfoClass *)p_classarray.GetAt(s))->solid!=ProductSolid)
	{
		AfxMessageBox("这个部件还没有，应用另存为菜单！");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	//判断是否是单个零件
	if (!IsOneOfProduct)
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;
	}
	//如果零件被用的话判断名字有没有被改
	if (BeUsed)
	{
		if (FileName.MakeLower()!=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName.MakeLower())
		{
			AfxMessageBox("这个零件被产品所用，不允许改名字");
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
			return;
		}
	} 
	//判断这个部件是否有插入过
	if (ProductSolid!=solid)
	{
		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert=="F")
		{
            AfxMessageBox("这个部件还没有新插入过，无法保存，请先点另存为菜单！");
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
			return;
		}
	}
	//判断当前的的组件编号是否存在
	sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	if (commonfun.IsHave(sql))
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;          //更新一个零件
		//判断是否已经审核过
		BOOL IsAudit;
		if (!pdtclass.IsAlreadyAudit(((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,&IsAudit))
		{
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
			return;
		}
		else
			if (IsAudit)
			{
				AfxMessageBox("这个部件已经通过审核,不允许修改");
				GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
				return;
			}
	}
	else
	{
		AfxMessageBox("这个是新零件，请用另存为按钮");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	//}
	//根据不同的保存方式判断图号，文件名的合法性
	if(!commonfun.IsInfoOk(s,m_inscode,FileName,DrwName))
	{
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	if (!commonfun.TheWorkBeforeSave(ProductSolid))
	{
		AfxMessageBox("保存前准备工作失败");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	//修改文件名
	if (!pdtclass.NewRename(solid,FileName,&DrwName))
	{
		AfxMessageBox("重命名失败！");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	//把原来的目录变成旧的目录,当保存失败的时候用
	((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFileName=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwName=((DetailInfoClass *)p_classarray.GetAt(s))->DrwName;
	((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
	((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
	//更新数据
	((DetailInfoClass *)p_classarray.GetAt(s))->MdlName=m_mdlnum;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=m_inscode;
	((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode=m_pdmcode;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsName=m_insname;
	((DetailInfoClass *)p_classarray.GetAt(s))->Designer=m_designer;
	((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm=m_group;
	((DetailInfoClass *)p_classarray.GetAt(s))->Note=m_note;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName=FileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=m_drwingplace;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=m_place;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwName=DrwName;
	//commonfun.SetParam(s);
	if (!commonfun.SetParam(s))   //设置参数的图号
	{
		AfxMessageBox("设置图号错误！");
		return;
	}
	//读性能参数
	(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).RemoveAll();
	//更新性能参数表
	for(int i=0;i<m_perf.GetItemCount();i++)
	{
		str=m_perf.GetItemText(i,2);
		(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).Add(str);
	}
	int index=m_designstate.GetCurSel();
	m_designstate.GetLBText(index,((DetailInfoClass *)p_classarray.GetAt(s))->DesignState);
	if (((DetailInfoClass *)p_classarray.GetAt(s))->DesignState=="未完成")
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime="";
	}
	else
	{
		CTime time=CTime::GetCurrentTime();
		((DetailInfoClass *)p_classarray.GetAt(s))->FinishTime=time.Format("%Y-%m-%d %H:%M:%S");
	}
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="审核未通过";
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime="";
	((DetailInfoClass *)p_classarray.GetAt(s))->PDMState="未提交";
	//判断哪些已经改了
	if(!pdtclass.FindProductMenberForProductWhichIsModify(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE))
	{
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	//插入还没有保存的零部件
	if(!pdtclass.InsertProductMenberForProduct(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
	{
		AfxMessageBox("插入新零件失败");
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		return;
	}
	if(IsOneOfProduct)
	{
		if (!pdtclass.SaveBothPartAndAsm(s,"asm",FALSE))
		{
			commonfun.TheWorkAferSave(ProductSolid,FALSE);
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
			AfxMessageBox("保存失败！");
			return;
		}
		else
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
			commonfun.TheWorkAferSave(ProductSolid,TRUE);
			AfxMessageBox("保存成功");
		}
	}
	//开始保存
	else
	{
		if (IsLocal)			   //在本地，保存在本地
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="本地";
			//根据零件号判断是上传数据库还是更新数据库
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE,TRUE))
			{
				commonfun.TheWorkAferSave(ProductSolid,FALSE);
				GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
				AfxMessageBox("保存失败！");
				return;
			}
		}
		else
		{   
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="服务器";
			//在这之前要把修改过的零件先插入
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,TRUE,IsLocal))
			{
				commonfun.TheWorkAferSave(ProductSolid,FALSE);
				GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
				AfxMessageBox("保存失败！");
				return;
			}
		}
		((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
		commonfun.TheWorkAferSave(ProductSolid,TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		AfxMessageBox("保存成功");
	}
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
	OnOK();
}
/************************************************************************/
/* 修改图号按钮，同时检查图号的唯一性                                   */
/************************************************************************/
void CProductSaveDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
void CProductSaveDlg::OnBnClickedCancel()
{
	OnCancel();
}
void CProductSaveDlg::OnClose()
{
	CDialog::OnClose();
}
/************************************************************************/
/* 选择产品分类按钮                                                     */
/************************************************************************/
void CProductSaveDlg::OnBnClickedButtonGrpfrm()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	CSelectDlg selectdlg;
	selectdlg.Type="asm";
	int s=-1;
	for(s=0;s<p_classarray.GetSize();s++)
	{
		if (((DetailInfoClass *)p_classarray.GetAt(s))->solid==ProductSolid)
		{
			break;
		}
	}
	if (s==-1)
	{
		AfxMessageBox("无法得到这个产品的信息");
		return;
	}
	if (IDOK==selectdlg.DoModal())
	{
		m_group=selectdlg.Name;
		((DetailInfoClass *)p_classarray.GetAt(s))->GroupNum=selectdlg.num;
		UpdateData(FALSE);
	}
}
/************************************************************************/
/* 选择三维本地的目录                                                   */
/************************************************************************/
void CProductSaveDlg::OnBnClickedButtonSolidlocalplace()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString pathName;
	CString path;
	CString FileName;
	char szFilters[]=
		"PartFiles (*.prt)|*.prt*|Asm Files (*.asm)|*.asm|All Files (*.*)|*.*||";

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg (FALSE, "保存对话框", "*.prt",
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if( fileDlg.DoModal ()==IDOK )
	{
		pathName=fileDlg.GetPathName();
		int k=pathName.ReverseFind('\\');
		m_place=pathName.Left(k);
		m_filename= fileDlg.GetFileTitle();
	}
	UpdateData(FALSE);
}
/************************************************************************/
/* 选择工程图本地的目录                                                 */
/************************************************************************/
void CProductSaveDlg::OnBnClickedButtonDrwlocalplace()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString pathName;
	CString path;
	CString FileName;
	char szFilters[]=
		"Drwing Files (*.drw)|*.drw.*|All Files (*.*)|*.*||";

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg (FALSE, "保存对话框", "*.drw",
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if( fileDlg.DoModal ()==IDOK )
	{
		pathName=fileDlg.GetPathName();
		int k=pathName.ReverseFind('\\');
		m_drwingplace=pathName.Left(k);
		//	m_drwname= fileDlg.GetFileTitle();
	}
	UpdateData(FALSE);
}
/************************************************************************/
/* 选择三维服务器的目录                                                     */
/************************************************************************/
void CProductSaveDlg::OnBnClickedButtonSolidserverplace()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CStyleStandDlg severdirdlg;
	if(IDOK==severdirdlg.DoModal())
	{
		m_place=severdirdlg.path;
	}
	UpdateData(FALSE);
}
/************************************************************************/
/* 选择工程图的服务器的目录                                             */
/************************************************************************/
void CProductSaveDlg::OnBnClickedButtonDwgserverplace()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CStyleStandDlg severdirdlg;
	if(IDOK==severdirdlg.DoModal())
	{
		m_drwingplace=severdirdlg.path;
	}
	UpdateData(FALSE);
}
void CProductSaveDlg::OnBnClickedButtonde()
{
	UpdateData(TRUE);
	CUserNameDlg dlg;
	dlg.m_UserName=m_designer;
	if(dlg.DoModal()==IDOK)
	{
		m_designer=dlg.m_UserName;
		UpdateData(FALSE);
	}
}
