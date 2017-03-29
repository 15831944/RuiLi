// PrtSaveAsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PrtSaveAsDlg.h"
#include "afxtempl.h"
#include "DetailInfoClass.h"
#include "FtpFile.h"
#include "CommonFuncClass.h"
#include "afxtempl.h"
#include "MyDatabase.h"
#include "StyleStandDlg.h"
#include "ProWindows.h"
#include "SelectDlg.h"
#include "PrtClass.h"
#include "ProductClass.h"
#include "MaterialDlg.h"
#include "UnitDlg.h"
#include "UserNameDlg.h"

void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
extern CArray <Database,Database> Database_struc_temp;
extern CMyDatabase m_db;
extern FtpInfo Ftp_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;
// CPrtSaveAsDlg 对话框

IMPLEMENT_DYNAMIC(CPrtSaveAsDlg, CDialog) 
CPrtSaveAsDlg::CPrtSaveAsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrtSaveAsDlg::IDD, pParent)
	, m_inscode(_T(""))
	, m_insname(_T(""))
	, m_mdlnum(_T(""))
	, m_designer(_T(""))
	, m_note(_T(""))
	, m_group(_T(""))
	, m_material(_T(""))
	, m_unit(_T(""))
	, m_place(_T(""))
	, m_drwingplace(_T(""))
	, m_filename(_T(""))
	, m_pdmcode(_T(""))
{
	Mem=FALSE;
}

CPrtSaveAsDlg::~CPrtSaveAsDlg()
{
}

void CPrtSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CODE, m_inscode);
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_insname);
	DDX_Text(pDX, IDC_EDIT_MDLFRM, m_mdlnum);
	DDX_Text(pDX, IDC_DESIGNER, m_designer);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	DDX_Text(pDX, IDC_EDIT_GRPFRM, m_group);
	DDX_Text(pDX, IDC_EDIT_MATERIAL, m_material);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_unit);
	DDX_Text(pDX, IDC_EDIT_PLACE, m_place);
	DDX_Text(pDX, IDC_EDIT_DRWPLACE, m_drwingplace);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
	DDX_Control(pDX, IDC_COMBO_FINISH, m_designstate);
	DDX_Control(pDX, IDC_LIST_PER, m_perf);
	DDX_Text(pDX, IDC_EDIT_CODE2, m_pdmcode);
}


BEGIN_MESSAGE_MAP(CPrtSaveAsDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDLOCALPLACE, OnBnClickedButtonSolidlocalplace)
	ON_BN_CLICKED(IDC_BUTTON_DRWLOCALPLACE, OnBnClickedButtonDrwlocalplace)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDSERVERPLACE, OnBnClickedButtonSolidserverplace)
	ON_BN_CLICKED(IDC_BUTTON_DWGSERVERPLACE, OnBnClickedButtonDwgserverplace)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM, OnBnClickedButtonGrpfrm)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON9, OnBnClickedButton9)
END_MESSAGE_MAP()


// CPrtSaveAsDlg 消息处理程序

BOOL CPrtSaveAsDlg::OnInitDialog()
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
	//去掉进程中没有的基本信息
	commonfun.IsBasicInfoAlreadyHave();
	commonfun.RemoveNotInSession();
	ProError status;
	ProMdlType p_type;
	int s;
	CString sql,str;
	BOOL IsHave=FALSE;
	status=ProMdlCurrentGet((ProMdl *)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("当前零件不正确！");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	status=ProMdlTypeGet((ProMdl)solid,&p_type);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("当前模型不正确！");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	else
		if (p_type!=PRO_MDL_PART)
		{
			AfxMessageBox("当前不是零件模型，不能保存！");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
		else
		{	
			//去掉不在当前进程中的结构；
			int index=m_designstate.AddString("完成");
			m_designstate.SetCurSel(index);
			m_designstate.AddString("未完成");

			for(s=0;s<p_classarray.GetSize();s++)
			{
				if (((DetailInfoClass *)p_classarray.GetAt(s))->solid==solid)
				{
					IsHave=TRUE;
					break;
				}
			}
			if (!IsHave)
			{
				//说明是自己打开的零件
				info = new DetailInfoClass;
				Mem=TRUE;
				info->SolidID=-1;
				info->solid=solid;
				m_mdlnum="无";
				m_inscode=commonfun.ObtainPartMap(solid);       //得到当前的图号
				info->MdlNum="-1";
				info->MdlName="无";
				UpdateData(FALSE);
				p_classarray.Add(info);
			}
			else
			{
				m_mdlnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlName;
				if (commonfun.ObtainPartMap(solid)!="")
				{
                    m_inscode=commonfun.ObtainPartMap(solid);
				}
				else
				{
                   m_inscode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
				}
				m_pdmcode=((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode;
				m_insname=((DetailInfoClass *)p_classarray.GetAt(s))->InsName;
				m_designer=((DetailInfoClass *)p_classarray.GetAt(s))->Designer;
				m_group=((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm;
				m_material=((DetailInfoClass *)p_classarray.GetAt(s))->Material;
				m_unit=((DetailInfoClass *)p_classarray.GetAt(s))->Unit;
				m_note=((DetailInfoClass *)p_classarray.GetAt(s))->Note;
				m_filename=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName;
				m_drwingplace=((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath;
				m_place=((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath;
				//显示性能参数
				if(!pdtclass.ShowPerf(s,"prt",&m_perf))
				{
					AfxMessageBox("无法得到性能参数");
					this->SendMessage(WM_CLOSE,0,0);
					return FALSE;
				}
			}
		}
	//判断是否是单个零件
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
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	if (IsOneOfProduct)
	{
		int Pos=commonfun.IsBasicInfoHave(ProductSolid);
		if (Pos==-1)
		{
			AfxMessageBox("找不到这个零件的产品的信息，无法更新");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
		else
			if(((DetailInfoClass *)p_classarray.GetAt(Pos))->SolidID==-1)
			{
				AfxMessageBox("还没有保存产品，请先保存产品");
				this->SendMessage(WM_CLOSE,0,0);
				return FALSE;
			}
	}
	UpdateData(FALSE);
	return TRUE;  
	// 异常：OCX 属性页应返回 FALSE
}

void CPrtSaveAsDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CCommonFuncClass commonfun;
	CPrtClass prtclass;
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
		AfxMessageBox("当前零件不正确！");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	if (m_place=="" || m_drwingplace=="" || m_filename=="" || m_inscode=="" || m_group=="" || m_pdmcode=="")
	{
		AfxMessageBox("你的信息不完全，请先填完整");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	int k;
	if (m_place.Right(1)=="\\")
	{
		k=m_place.ReverseFind('\\');
		if (k!=-1)
		{
			m_place=m_place.Left(k);
		}
	}
	if (m_drwingplace.Right(1)=="\\")
	{
		k=m_drwingplace.ReverseFind('\\');
		if (k!=-1)
		{
			m_drwingplace=m_drwingplace.Left(k);
		}
	}
	k=m_place.Find(":");
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
	int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		AfxMessageBox("找不到这个零件的信息，无法更新");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	//判断图号是否正确
	m_filename.MakeLower();
	if (m_filename.Right(4)==".prt")
	{
		k=m_filename.ReverseFind('.');
		if (k!=-1)
		{
			FileName=m_filename.Left(k)+".prt";
			DrwName=m_filename.Left(k)+".drw";
		}
	}
	else// (k==-1)			 //说明没有后缀
	{
		FileName=m_filename+".prt";
		DrwName=m_filename+".drw";
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum=="-1")
	{
		m_mdlnum="无";
        ((DetailInfoClass *)p_classarray.GetAt(s))->MdlName=="无";
		//判断名字是否存在
		sql.Format("SELECT * FROM NPartInsInfoTable where SolidFileName='%s'",FileName);
		if (commonfun.IsHave(sql))
		{
			str.Format("文件%s已经存在，请改名",FileName);
			AfxMessageBox(str);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			return;
		}
		else
			((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0; 
	}
	else
	{
		//判断是否有相同的零件的参数
		if (commonfun.IsPartExist(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum)!=-1)
		{
			str.Format("有相同可变参数的零件已经存在，是否需要添加新零件");
			if(AfxMessageBox(str,MB_YESNO)!=IDYES)
			{
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				return;
			}
		}
		((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=0;          //新插入一个零件
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
	((DetailInfoClass *)p_classarray.GetAt(s))->MdlName=m_mdlnum;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=m_inscode;
    ((DetailInfoClass *)p_classarray.GetAt(s))->PDMCode=m_pdmcode;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsName=m_insname;
	((DetailInfoClass *)p_classarray.GetAt(s))->Designer=m_designer;
	((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm=m_group;
	((DetailInfoClass *)p_classarray.GetAt(s))->Material=m_material;
	((DetailInfoClass *)p_classarray.GetAt(s))->Unit=m_unit;
	((DetailInfoClass *)p_classarray.GetAt(s))->Note=m_note;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName=FileName;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=m_drwingplace;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=m_place;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwName=DrwName;
	if (!commonfun.SetParam(s))   //设置参数的图号
	{
		AfxMessageBox("在文件参数中设置图号错误！");
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
	if(IsOneOfProduct)
	{
		if (!pdtclass.SaveBothPartAndAsm(s,"prt",TRUE))              //保存零件的同时保存这个零件的上层部件
		{
			commonfun.TheWorkAferSave(ProductSolid,FALSE);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			AfxMessageBox("保存失败！");
			return;
		}
		else
		{
			((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
			commonfun.TheWorkAferSave(ProductSolid,TRUE);
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			AfxMessageBox("保存成功");
		}
	}
	//开始保存
	else
	{
		if (IsLocal)			   //在本地，保存在本地
		{
			//if(!commonfun.SaveFile(solid,m_place,m_drwingplace,m_filename))
			//{
			//	AfxMessageBox("无法保存！");
			//	GetDlgItem(IDOK)->EnableWindow(TRUE);
			//	return;
			//}
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="本地";
			//根据零件号判断是上传数据库还是更新数据库
			if (((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle==0)		//Insert数据库
			{
				if (!prtclass.InsertPart(s,FALSE))
				{
					commonfun.TheWorkAferSave(ProductSolid,FALSE);
					GetDlgItem(IDOK)->EnableWindow(TRUE);
					AfxMessageBox("保存失败");
					return;
				}
				else
				{
					((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
					commonfun.TheWorkAferSave(ProductSolid,TRUE);
					GetDlgItem(IDOK)->EnableWindow(TRUE);
					((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
					AfxMessageBox("保存成功");
				}
			}
		}
		else
		{   
			((DetailInfoClass *)p_classarray.GetAt(s))->IsLocalOrServer="服务器";
			if (!commonfun.SavePartToTheServerforPart(s))
			{
				commonfun.TheWorkAferSave(ProductSolid,FALSE);
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				AfxMessageBox("保存失败！");
				return;
			}//上传到服务器
			else
			{
				((DetailInfoClass *)p_classarray.GetAt(s))->IsSaved=TRUE;
				commonfun.TheWorkAferSave(ProductSolid,TRUE);
				GetDlgItem(IDOK)->EnableWindow(TRUE);
				((DetailInfoClass *)p_classarray.GetAt(s))->IsInsert="T";
				AfxMessageBox("保存成功");
			}
		}
	}
	OnOK();
}

void CPrtSaveAsDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CCommonFuncClass commonfun;
	int s=commonfun.IsBasicInfoHave(solid);
	if (s==-1)
	{
		OnCancel();
	}
	else
	if (((DetailInfoClass *)p_classarray.GetAt(s))->SolidID==-1)
	{
		if (Mem)
		{
			delete info;
			p_classarray.RemoveAt(s);
		}
	}
	OnCancel();
}
/************************************************************************/
/* 选择本地的三维目录                                                   */
/************************************************************************/
void CPrtSaveAsDlg::OnBnClickedButtonSolidlocalplace()
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
/* 选择本地的二维目录                                                   */
/************************************************************************/
void CPrtSaveAsDlg::OnBnClickedButtonDrwlocalplace()
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
/* 指定要保存三维的服务器的位置                                         */
/************************************************************************/
void CPrtSaveAsDlg::OnBnClickedButtonSolidserverplace()
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
/* 指定要保存二维的服务器的位置                                         */
/************************************************************************/
void CPrtSaveAsDlg::OnBnClickedButtonDwgserverplace()
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

void CPrtSaveAsDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: 在此添加消息处理程序代码
}

void CPrtSaveAsDlg::OnBnClickedButtonGrpfrm()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CSelectDlg selectdlg;
	selectdlg.Type="prt";
	int s=-1;
	for(s=0;s<p_classarray.GetSize();s++)
	{
		if (((DetailInfoClass *)p_classarray.GetAt(s))->solid==solid)
		{
			break;
		}
	}
	if (s==-1)
	{
		AfxMessageBox("无法得到这个零件的信息");
		return;
	}
	if (IDOK==selectdlg.DoModal())
	{
		m_group=selectdlg.Name;
		((DetailInfoClass *)p_classarray.GetAt(s))->GroupNum=selectdlg.num;
		UpdateData(FALSE);
	}
}

void CPrtSaveAsDlg::OnBnClickedButton5()
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

void CPrtSaveAsDlg::OnBnClickedButton6()
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

void CPrtSaveAsDlg::OnBnClickedButton9()
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
