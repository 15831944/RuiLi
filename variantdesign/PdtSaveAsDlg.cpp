// PdtSaveAsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PdtSaveAsDlg.h"
#include "ProWindows.h"
#include "CommonFuncClass.h"
#include "Compute.h"
#include "DetailInfoClass.h"
#include "afxtempl.h"
#include "MyDatabase.h"
#include "StyleStandDlg.h"
#include "ProductMenberBasicInfoDlg.h"
#include "ProductClass.h"
#include "SelectDlg.h"
#include "UserNameDlg.h"


void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data,BOOL nFlag);

extern CArray <Database,Database> Database_struc_temp;
extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CArray <JuBing,JuBing> JuBing_struc_temp;
extern FtpInfo Ftp_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;
// CPdtSaveAsDlg 对话框

IMPLEMENT_DYNAMIC(CPdtSaveAsDlg, CDialog)
CPdtSaveAsDlg::CPdtSaveAsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPdtSaveAsDlg::IDD, pParent)
	, m_inscode(_T(""))
	, m_insname(_T(""))
	, m_mdlnum(_T(""))
	, m_designer(_T(""))
	, m_note(_T(""))
	, m_place(_T(""))
	, m_drwingplace(_T(""))
	, m_filename(_T(""))
	, m_group(_T(""))
	, m_pdmcode(_T(""))
{
}

CPdtSaveAsDlg::~CPdtSaveAsDlg()
{
}

void CPdtSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PRODUCT, m_treeproduct);

	DDX_Text(pDX, IDC_EDIT_CODE, m_inscode);
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_insname);
	DDX_Text(pDX, IDC_EDIT_MDLFRM, m_mdlnum);
	DDX_Text(pDX, IDC_DESIGNER, m_designer);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	DDX_Text(pDX, IDC_EDIT_PLACE, m_place);
	DDX_Text(pDX, IDC_EDIT_DRWPLACE, m_drwingplace);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
	DDX_Control(pDX, IDC_COMBO_FINISH, m_designstate);
	DDX_Control(pDX, IDC_LIST_PER, m_perf);
	DDX_Text(pDX, IDC_EDIT_GRPFRM, m_group);
	DDX_Text(pDX, IDC_EDIT_CODE2, m_pdmcode);
}


BEGIN_MESSAGE_MAP(CPdtSaveAsDlg, CDialog)
	ON_WM_MOVE()
	//ON_NOTIFY(NM_DBLCLK, IDC_LIST_PART, OnNMDblclkListPart)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDLOCALPLACE, OnBnClickedButtonSolidlocalplace)
	ON_BN_CLICKED(IDC_BUTTON_DRWLOCALPLACE, OnBnClickedButtonDrwlocalplace)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDSERVERPLACE, OnBnClickedButtonSolidserverplace)
	ON_BN_CLICKED(IDC_BUTTON_DWGSERVERPLACE, OnBnClickedButtonDwgserverplace)
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM, OnBnClickedButtonGrpfrm)
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM2, OnBnClickedButtonGrpfrm2)
END_MESSAGE_MAP()


// CPdtSaveAsDlg 消息处理程序

void CPdtSaveAsDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: 在此添加消息处理程序代码
}

BOOL CPdtSaveAsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ProError status;
	CProductClass pdtclass;
	CCommonFuncClass commonfun;

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	//去掉进程中没有的基本信息
	///初始化图像列表
	m_ImageList.Create(16, 16, ILC_COLOR8, 0, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
    m_treeproduct.SetImageList(&m_ImageList,TVSIL_NORMAL);

	commonfun.IsBasicInfoAlreadyHave();
	commonfun.RemoveNotInSession();
	//得到产品树
	ProSolid solid;
	status=ProMdlCurrentGet((ProMdl*)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("对不起，您没有打开装配模型！");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	//添加产品树
	commonfun.AddProductToTree(solid,&m_treeproduct);
	//把需要重新填写基本信息
	int s;
	CString sql,str;
	int index=m_designstate.AddString("完成");
	m_designstate.SetCurSel(index);
	m_designstate.AddString("未完成");
	s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		info = new DetailInfoClass;
		info->SolidID=-1;
		info->solid=solid;
		info->MdlNum="-1";
		info->PDMState="未提交";
		m_inscode=commonfun.ObtainPartMap(solid);
		m_mdlnum="无";
		UpdateData(FALSE);
		p_classarray.Add(info);
		m_perf.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
		m_perf.InsertColumn(0,"序号",LVCFMT_LEFT,40,-1);
		m_perf.InsertColumn(1,"性能参数名",LVCFMT_LEFT,80,-1);
		m_perf.InsertColumn(2,"性能参数",LVCFMT_LEFT,100,-1);
		m_perf.InsertColumn(3,"备注",LVCFMT_LEFT,100,-1);
		s=commonfun.IsBasicInfoHave(solid);
		if(s==-1)  //再次确认当前组件的信息
		{
			AfxMessageBox("无法得到当前组件的基本信息！");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
	}
	else
	{
		m_mdlnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlName;
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
	if(!commonfun.ObtianBasicInfoFromHandle(solid))				   //判断当前的零件的基本信息是否都存在
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	ProductSolid=commonfun.ObtainTheTopSolid(solid);
	if(ProductSolid!=NULL)
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
}
/************************************************************************/
/* 保存产品按钮                                                         */
/************************************************************************/
void CPdtSaveAsDlg::OnBnClickedOk()
{
	//判断保存路径是否为空
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CString  Path,FileName,DrwName,str,sql;
	ProSolid solid;
	ProError status;
	BOOL IsLocal=FALSE;
	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	status=ProMdlCurrentGet((ProMdl *)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("当前产品不正确！");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if (m_place=="" || m_drwingplace=="" || m_filename=="" || m_inscode=="")
	{
		AfxMessageBox("你的信息不完全，请先填完整");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
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
				GetDlgItem(IDOK)->EnableWindow(TRUE);
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
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				return;
			}
			IsLocal=TRUE;
		}
	}
	//判断当前的零部件是否都可以找到
	if (!commonfun.ObtianBasicInfoFromHandle(solid))
	{
		return;
	}
	int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("找不到这个组件的信息，无法更新");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
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
	//判断是否有插入过
	if (((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert=="T")
	{
		str="这个部件已经另存过，是否再一次另存";
		if(AfxMessageBox(str,MB_YESNO)!=IDYES)
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			return;
		}
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum=="-1")
	{
		//判断名字是否存在
		sql.Format("SELECT * FROM NPartInsInfoTable where SolidFileName='%s'",FileName);
		if (commonfun.IsHave(sql))
		{
			str.Format("文件%s已经存在，请改名",FileName);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			AfxMessageBox(str);
			return;
		}
		else
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0; 
	}
	else
	{
		//判断是否有相同的零件的参数，判断是否真的要重新插入一个新产品
		if (commonfun.IsAsmExist(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum)!=-1)
		{
			str.Format("有相同可变参数的零件已经存在，是否需要添加新的");
			if(AfxMessageBox(str,MB_YESNO)!=IDYES)
			{
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				return;
			}
		}
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;          //新插入
	}
	//根据不同的保存方式判断图号，文件名的合法性
	if(!commonfun.IsInfoOk(s,m_inscode,FileName,DrwName))
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if (!commonfun.TheWorkBeforeSave(ProductSolid))
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	//修改文件名
	if (!pdtclass.NewRename(solid,FileName,&DrwName))
	{
		AfxMessageBox("重命名失败！");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	//把原来的目录变成旧的目录,当保存失败的时候用
	((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFileName=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwName=((DetailInfoClass *)p_classarray.GetAt(s))->DrwName;
	((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
	((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFilePath=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
	//更新数据
	((DetailInfoClass *)p_classarray.GetAt(s))->MdlName=m_mdlnum;       //模型名
	((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=m_inscode;
	((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode=m_pdmcode;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsName=m_insname;      //实例名
	((DetailInfoClass *)p_classarray.GetAt(s))->Designer=m_designer;
	((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm=m_group;
	((DetailInfoClass *)p_classarray.GetAt(s))->Note=m_note;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName=FileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=m_drwingplace;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=m_place;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwName=DrwName;
	//commonfun.SetParam(s);
	if (!commonfun.SetParam(s))
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
		AfxMessageBox("插入新零件失败");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	//插入还没有保存的零部件
	if(!pdtclass.InsertProductMenberForProduct(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
	{
		AfxMessageBox("插入新零件失败");
		commonfun.TheWorkAferSave(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if (IsOneOfProduct)        //是一个产品的一部分
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;
		//更新部件，同时更新产品
		if (!pdtclass.SaveBothPartAndAsm(s,"asm",TRUE))
		{
			commonfun.TheWorkAferSave(ProductSolid,FALSE);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			AfxMessageBox("保存失败！");
			return;
		}
		else
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
            ((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
			GetDlgItem(IDOK)->EnableWindow(TRUE);
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
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				AfxMessageBox("保存失败！");
				return;
			}
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				commonfun.TheWorkAferSave(ProductSolid,TRUE);
				AfxMessageBox("保存成功");
			}
		}
		else
		{   
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="服务器";
			//在这之前要把修改过的零件先插入
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,TRUE,IsLocal))
			{
				commonfun.TheWorkAferSave(ProductSolid,FALSE);
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				AfxMessageBox("保存失败！");
				return;
			}
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				commonfun.TheWorkAferSave(ProductSolid,TRUE);
				AfxMessageBox("保存成功");
			}
		}
	}
	OnOK();
}
void CPdtSaveAsDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
/************************************************************************/
/* 选择三维本地的目录                                                   */
/************************************************************************/
void CPdtSaveAsDlg::OnBnClickedButtonSolidlocalplace()
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
void CPdtSaveAsDlg::OnBnClickedButtonDrwlocalplace()
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
	}
	UpdateData(FALSE);
}
/************************************************************************/
/* 选择服务器上三维的目录                                               */
/************************************************************************/
void CPdtSaveAsDlg::OnBnClickedButtonSolidserverplace()
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
/* 选择服务器上工程图的目录                                             */
/************************************************************************/
void CPdtSaveAsDlg::OnBnClickedButtonDwgserverplace()
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

void CPdtSaveAsDlg::OnBnClickedButtonGrpfrm()
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

void CPdtSaveAsDlg::OnBnClickedButtonGrpfrm2()
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
