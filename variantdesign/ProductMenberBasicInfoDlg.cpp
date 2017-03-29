// ProductMenberBasicInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "ProductMenberBasicInfoDlg.h"
#include "afxtempl.h"
#include "CommonFuncClass.h"
#include "DetailDlg.h"
#include "Compute.h"
#include "MyDatabase.h"
#include "ProWindows.h"
#include "StyleStandDlg.h"
#include "ProductClass.h"
#include "PrtClass.h"
#include "MaterialDlg.h"
#include "UnitDlg.h"
#include "UserNameDlg.h"


void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
extern CArray <Database,Database> Database_struc_temp;
extern CMyDatabase m_db;
extern FtpInfo Ftp_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;
// CProductMenberBasicInfoDlg 对话框

IMPLEMENT_DYNAMIC(CProductMenberBasicInfoDlg, CDialog)
CProductMenberBasicInfoDlg::CProductMenberBasicInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProductMenberBasicInfoDlg::IDD, pParent)
	, m_inscode(_T(""))
	, m_insname(_T(""))
	, m_mdlnum(_T(""))
	, m_designer(_T(""))
	, m_note(_T(""))
	, m_group(_T(""))
	, m_material(_T(""))
	, m_unit(_T(""))
	, m_filename(_T(""))
	,s(-1)
	, m_place(_T(""))
	, m_drwplace(_T(""))
{
	IsSelect=FALSE;
	Num="";
}

CProductMenberBasicInfoDlg::~CProductMenberBasicInfoDlg()
{
}

void CProductMenberBasicInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_LIST_PERFORMANCE, m_performance);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_product);
	DDX_Text(pDX, IDC_EDIT_CODE, m_inscode);
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_insname);
	DDX_Text(pDX, IDC_EDIT_MDLFRM, m_mdlnum);
	DDX_Text(pDX, IDC_DESIGNER, m_designer);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	DDX_Text(pDX, IDC_EDIT_GRPFRM, m_group);
	DDX_Text(pDX, IDC_EDIT_MATERIAL, m_material);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_unit);
	DDX_Text(pDX, IDC_EDIT_SOLID, m_filename);
	DDX_Control(pDX, IDC_COMBO_STATE, m_designstate);
	DDX_Text(pDX, IDC_EDIT_SOLIDPLACE, m_place);
	DDX_Text(pDX, IDC_EDIT_DRWPLACE, m_drwplace);
}


BEGIN_MESSAGE_MAP(CProductMenberBasicInfoDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_SEL, OnBnClickedButtonSel)
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM, OnBnClickedButtonGrpfrm)
	ON_BN_CLICKED(IDC_BUTTON_MATERIAL, OnBnClickedButtonMaterial)
	ON_BN_CLICKED(IDC_BUTTON_UNIT, OnBnClickedButtonUnit)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDLOCAL, OnBnClickedButtonSolidlocal)
	ON_BN_CLICKED(IDC_BUTTON_DRWLOCAL, OnBnClickedButtonDrwlocal)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDSERVER, OnBnClickedButtonSolidserver)
	ON_BN_CLICKED(IDC_BUTTON_DRWSERVER, OnBnClickedButtonDrwserver)
	ON_BN_CLICKED(IDC_BUTTON_de, OnBnClickedButtonde)
END_MESSAGE_MAP()


// CProductMenberBasicInfoDlg 消息处理程序

void CProductMenberBasicInfoDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CProductMenberBasicInfoDlg::OnBnClickedOk()
{
	//OnOK();
}

BOOL CProductMenberBasicInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CCompute compute;
	CString str,ProductNum,sql;
	int index=m_designstate.AddString("完成");
	m_designstate.SetCurSel(index);
	m_designstate.AddString("未完成");
	ProductSolid=commonfun.ObtainTheTopSolid(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
	if (ProductSolid==NULL)
	{
		AfxMessageBox("无法得到产品的句柄");
		return FALSE;
	}
	//对于已经保存过零部件，不能改名但可以改其他信息
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==3)
	{
		;//对零部件的简单更新，不能改名；
	}
	else
	{
		if (ProductSolid !=((DetailInfoClass *)p_classarray.GetAt(s))->solid)	  //说明不是产品
		{
			//判断是否是已有零件
			//if (!IsNew)		//如果是从新建产品对话框过来的，因为已经判断过了，所以需要再判断是否已有零件还是新零件
			//{
			//	if (!pdtclass.AffirmIsOld(Type,s))						 //说明是已有零件
			//	{
			//		return FALSE;
			//	}
			//}
			//else
			//{
			//	str.Format("文件%s已经被改变,请输入你基本信息",((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName);
			//	AfxMessageBox(str);
			//}
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1)
			{
				m_designer=((DetailInfoClass *)p_classarray.GetAt(s))->Designer;
				m_insname=((DetailInfoClass *)p_classarray.GetAt(s))->InsName;
				m_inscode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
				m_drwplace=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
				m_place=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
				m_filename=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
			}
		}
		else		 //是产品级的
		{
			if (IsInsert)				//说明是从另存为对话框来的
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;
			}
				else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;   //说明是从保存对话框来的	
			}
		}
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==3)
	{
		m_designer=((DetailInfoClass *)p_classarray.GetAt(s))->Designer;
		m_insname=((DetailInfoClass *)p_classarray.GetAt(s))->InsName;
		m_inscode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
		m_drwplace=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
		m_place=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
		m_filename=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
		GetDlgItem(IDC_EDIT_SOLID)->EnableWindow(FALSE);
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1)
	{
		GetDlgItem(IDC_EDIT_CODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INSNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DRWPLACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SOLID)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SOLIDPLACE)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_DRWLOCAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DRWSERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SOLIDLOCAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SOLIDSERVER)->EnableWindow(FALSE);
	}
	m_mdlnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum;
	m_group=((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm;
	m_material=((DetailInfoClass *)p_classarray.GetAt(s))->Material;
	m_unit=((DetailInfoClass *)p_classarray.GetAt(s))->Unit;
	m_note=((DetailInfoClass *)p_classarray.GetAt(s))->Note;

    //显示性能参数；
	if (!pdtclass.ShowPerf(s,Type,&m_product))
	{
		return FALSE;
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1)
	{
		AfxMessageBox("这个零件/组件已经存在，不要作修改，请直接保存");
	}
	UpdateData(FALSE);
	return TRUE;
}

void CProductMenberBasicInfoDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: 在此添加消息处理程序代码
}
void CProductMenberBasicInfoDlg::OnBnClickedButtonSel()
{
	// TODO: 在此添加控件通知处理程序代码
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CString  Path,FileName,DrwName,str;
	BOOL IsLocal;
	UpdateData(TRUE);
	if (m_filename=="" || m_inscode=="" || m_insname=="" || m_place=="" || m_drwplace=="")
	{
		AfxMessageBox("你的信息不完全，请先填完整");
		return;
	}
	int k=m_place.Find(":");
	if (k==-1)		   //说明是服务器上的
	{
		if (k=m_drwplace.Find(":"))
		{
			if (k!=-1)
			{
				AfxMessageBox("二维模型和三维模型保存的位置不一致！");
				return;
			}
			IsLocal=FALSE;
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="服务器";
		}
	}
	else			 //在本地
	{
		if (k=m_drwplace.Find(":"))
		{
			if (k==-1)
			{
				AfxMessageBox("二维模型和三维模型保存的位置不一致！");		//不能使保存一个在服务器上，一个在本地
				return;
			}
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="本地";
			IsLocal=TRUE;
		}
	}
	k=m_filename.Find(".");
	if (k==-1)			 //说明没有后缀
	{
		if (Type=="prt")
		{
		   FileName=m_filename+".prt";
		}
		else
		{
			FileName=m_filename+".asm";
		}
		DrwName=m_filename+".drw";
	}
	else
	{
		if (Type=="prt")
		{
			FileName=m_filename.Left(k)+".prt";
		}
		else
		{
			FileName=m_filename.Left(k)+".asm";
		}
		DrwName=m_filename.Left(k)+".drw";
	}
	//根据不同的保存方式判断图号，文件名的合法性
	if(!commonfun.IsInfoOk(s,m_inscode,FileName,DrwName))
	{
		return;
	}
	//修改文件名  
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==1)
	{
		
		if (!pdtclass.NewRename(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName,&(((DetailInfoClass *)p_classarray.GetAt(s))->DrwName)))
		{
			AfxMessageBox("重命名失败！");
			return;
		}
	}
	else
	{
		if (!pdtclass.NewRename(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FileName,&DrwName))
		{
			AfxMessageBox("重命名失败！");
			return;
		}
	}
	//把原来的目录变成旧的目录,当保存失败的时候用
	((DetailInfoClass *)p_classarray.GetAt(s))->OldSolidFileName=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->OldDrwName=((DetailInfoClass *)p_classarray.GetAt(s))->DrwName;
	//更新数据
	((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum=m_mdlnum;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=m_inscode;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsName=m_insname;
	((DetailInfoClass *)p_classarray.GetAt(s))->Designer=m_designer;
	((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm=m_group;
	((DetailInfoClass *)p_classarray.GetAt(s))->Material=m_material;
	((DetailInfoClass *)p_classarray.GetAt(s))->Unit=m_unit;
	((DetailInfoClass *)p_classarray.GetAt(s))->Note=m_note;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName=FileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwName=DrwName;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=m_place;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=m_drwplace;
	//读性能参数
	(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).RemoveAll();
	//更新性能参数表
	for(int i=0;i<m_product.GetItemCount();i++)
	{
		str=m_product.GetItemText(i,2);
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

	if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==3)
	{
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;               //为了程序的通用性，先改为1
		//if (!pdtclass.UpdateProductMenberToDatabase(s,Type))
		//{
		//	AfxMessageBox("修改失败");
		//	((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=3;
		//}//更新信息
		//else
		//{
		//	AfxMessageBox("修改成功！");
		//   ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=3;
		//}
	}
	else
	{
		//如果是产品
		if((((DetailInfoClass *)p_classarray.GetAt(s))->solid==ProductSolid))
		{
			//插入产品,插入数据库
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,!(IsLocal),IsLocal))
			{
				AfxMessageBox("写数据库失败！");
				return;
			}
			else
			{
				AfxMessageBox("保存成功");
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
			}
		}
		else
		{
			//根据不同的方式保存零件/组件
			if (IsLocal)
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="本地";
			}
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="服务器";
			}
			//判断是否是来自产品新建的
			if (IsNew)
			{
				//重新插入新零件和组件
				if (!InsertNewProductMenberToDatabase(s,Type))
				{
					return;
				}
			}
			else
			{
				if(!pdtclass.UpdateProductMenber(ProductSolid,s))
				{
					AfxMessageBox("保存失败");
					return;
				}
				else
				{
					AfxMessageBox("保存成功");
				}
			}
		}
	}

	IsSelect=TRUE;
	OnOK();
}
/************************************************************************/
/* 选择分类按钮                                                         */
/************************************************************************/
void CProductMenberBasicInfoDlg::OnBnClickedButtonGrpfrm()
{
	// TODO: 在此添加控件通知处理程序代码

}


/************************************************************************/
/* 选择材料按钮                                                       */
/************************************************************************/
void CProductMenberBasicInfoDlg::OnBnClickedButtonMaterial()
{
	UpdateData(TRUE);
	CMaterialDlg dlg;
	dlg.m_ID=m_material;
	if(dlg.DoModal()==IDOK)
	{
		m_material=dlg.m_ID;
		UpdateData(FALSE);
	}
}
/************************************************************************/
/* 选择单位按钮                                                                     */
/************************************************************************/
void CProductMenberBasicInfoDlg::OnBnClickedButtonUnit()
{
	UpdateData(TRUE);
	CUnitDlg dlg;
	dlg.m_ID=m_unit;
	if(dlg.DoModal()==IDOK)
	{
		m_unit=dlg.m_ID;
		UpdateData(FALSE);
	}
}

void CProductMenberBasicInfoDlg::OnBnClickedButtonSolidlocal()
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
		if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0 || ((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)
		{
			m_filename= fileDlg.GetFileTitle();
		}
	}
	UpdateData(FALSE);
}

void CProductMenberBasicInfoDlg::OnBnClickedButtonDrwlocal()
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
		m_drwplace=pathName.Left(k);
		//m_drwname= fileDlg.GetFileTitle();
	}
	UpdateData(FALSE);
}

void CProductMenberBasicInfoDlg::OnBnClickedButtonSolidserver()
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

void CProductMenberBasicInfoDlg::OnBnClickedButtonDrwserver()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CStyleStandDlg severdirdlg;
	if(IDOK==severdirdlg.DoModal())
	{
		m_drwplace=severdirdlg.path;
	}
	UpdateData(FALSE);
}
/************************************************************************/
/* 当新建产品时，如果有新零件或者部件产生时                             */
/************************************************************************/
BOOL CProductMenberBasicInfoDlg::InsertNewProductMenberToDatabase(int s,CString Type)
{
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CPrtClass prtclass;
	BOOL IsLocal;
	if(Type=="prt")
	{
			//根据不同的保存位置进行不同的操作
		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="本地")
		{
			IsLocal=TRUE;
			if(!commonfun.SaveFile(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath,((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName))
			{
				AfxMessageBox("无法保存！");
				return FALSE;
			}
			if (!prtclass.InsertPart(s,FALSE))
			{
				AfxMessageBox("保存失败");
				return FALSE;
			}
			else
			{
				AfxMessageBox("保存成功");
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
			}
		}
		else
		{
			if (!commonfun.SavePartToTheServerforPart(s))
			{
				AfxMessageBox("保存失败！");
				return FALSE;
			}//上传到服务器
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
				AfxMessageBox("保存成功");
			}
		}
	}
	else
	{
		//插入产品,插入数据库
		if (((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer=="本地")
		{
			IsLocal=TRUE;
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,FALSE,IsLocal))
			{
				AfxMessageBox("写数据库失败！");
				return FALSE;
			}
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
				AfxMessageBox("保存成功");
			}
		}
		else
		{
			IsLocal=FALSE;
			if (!pdtclass.WriteProductToDatabase(((DetailInfoClass *)p_classarray.GetAt(s))->solid,TRUE,IsLocal))
			{
				AfxMessageBox("写数据库失败！");
				return FALSE;
			}
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
				AfxMessageBox("保存成功");
			}
			    
		}
		
	}
	return TRUE;
}
void CProductMenberBasicInfoDlg::OnBnClickedButtonde()
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
