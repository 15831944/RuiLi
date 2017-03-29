// PrtNewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PrtNewDlg.h"
#include "ModelQueryDlg.h"
#include "DetailInfoClass.h"
#include "ProWindows.h"
#include "NewAlterDlg.h"
#include "FtpFile.h"
#include "CommonFuncClass.h"
#include "MyDatabase.h"
#include "ProWindows.h"
#include "PartQueryDlg.h"
#include "PrtClass.h"
#include "SelectDlg.h"
#include "MaterialDlg.h"
#include "UnitDlg.h"
#include "UserNameDlg.h"

extern CMyDatabase m_db;
extern FtpInfo Ftp_struc;
extern CPtrArray p_classarray;
// CPrtNewDlg 对话框

IMPLEMENT_DYNAMIC(CPrtNewDlg, CDialog)
CPrtNewDlg::CPrtNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrtNewDlg::IDD, pParent)
	, m_inscode(_T(""))  
	, m_insname(_T(""))
	, m_mdlnum(_T(""))
	, m_note(_T(""))
	, m_group(_T(""))
	, m_material(_T(""))
	, m_unit(_T(""))
	, m_filename(_T(""))
	, m_place(_T(""))
	, m_designer(_T(""))
	, m_oldfilename(_T(""))
	, m_olddrwname(_T(""))
	, m_pdmcode(_T(""))
{
	IsError=FALSE;
}

CPrtNewDlg::~CPrtNewDlg()
{
}

void CPrtNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CODE, m_inscode);
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_insname);
	DDX_Text(pDX, IDC_EDIT_MDLFRM, m_mdlnum);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	DDX_Text(pDX, IDC_EDIT_GRPFRM, m_group);
	DDX_Text(pDX, IDC_EDIT_MATERIAL, m_material);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_unit);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
	//DDX_Text(pDX, IDC_EDIT_PLACE, m_place);
	DDX_Text(pDX, IDC_DESIGNER, m_designer);
	DDX_Text(pDX, IDC_EDIT_OLDNAME, m_oldfilename);
	DDX_Text(pDX, IDC_EDIT_OLDDRWNAME, m_olddrwname);
	DDX_Text(pDX, IDC_EDIT_PDMCode, m_pdmcode);
}


BEGIN_MESSAGE_MAP(CPrtNewDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MDL, OnBnClickedButtonMdl)
	ON_BN_CLICKED(IDC_BUTTON_SELPRT, OnBnClickedButtonSelprt)
	//ON_BN_CLICKED(IDC_BUTTON_SEL, OnBnClickedButtonSel)
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM, OnBnClickedButtonGrpfrm)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
END_MESSAGE_MAP()


// CPrtNewDlg 消息处理程序
/************************************************************************/
/* 选择模型按钮                                                         */
/************************************************************************/
void CPrtNewDlg::OnBnClickedButtonMdl()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CModelQueryDlg modelquerydlg;
	modelquerydlg.DoModal();
	if (!modelquerydlg.IsSelect)
	{
		return;//需要传出来一个模型编号
	}
	else
	{
		detailinfo->MdlNum=modelquerydlg.MdlNum;
		GetPartModelInfo(modelquerydlg.MdlNum);
		m_mdlnum=detailinfo->MdlName;
		GetDlgItem(IDOK)->EnableWindow(TRUE); 
		UpdateData(FALSE);
	}
}
/************************************************************************/
/* 从PDM选择零件                                                        */
/************************************************************************/
void CPrtNewDlg::OnBnClickedButtonSelprt()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString sql;
	CString InsCode;
	CPartQueryDlg partquerydlg;
	BOOL IsLocal;
	partquerydlg.DoModal();
	if (!partquerydlg.IsSelect)
	{
		return;
	}
	else
	{
		InsCode=partquerydlg.InsCode;       //得到零件编号
		if (!GetPartInfo(InsCode))
		{
			AfxMessageBox("无法得到这个零件的详细信息");
			return;
		}
		else
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE); 
			int k=detailinfo->OldSolidFilePath.Find(":");
			if (k==-1)
			{
				IsLocal=FALSE;
				//GetDlgItem(IDC_EDIT_PLACE)->EnableWindow(TRUE);
			//	GetDlgItem(IDC_BUTTON_SEL)->EnableWindow(TRUE);
			}
			else
			{
				IsLocal=TRUE;
			}
			if (IsLocal)
			{
				//GetDlgItem(IDC_EDIT_PLACE)->EnableWindow(FALSE);
				//GetDlgItem(IDC_BUTTON_SEL)->EnableWindow(FALSE);
				m_place=detailinfo->OldSolidFilePath;	
			}
			UpdateData(FALSE);
		}
	}
}
/************************************************************************/
/* 选择路径按钮                                                         */
/************************************************************************/
//void CPrtNewDlg::OnBnClickedButtonSel()
//{
	//UpdateData(TRUE);
	//CString pathName;
	//CString path;
	//CString FileName;
	//char szFilters[]=
	//	"Prt Files (*.prt)|*.prt*|Drw Files (*.drw)|*.drw.*|Asm Files (*.asm)|*.asm.*|All Files (*.*)|*.*||";

	//// Create an Open dialog; the default file name extension is ".my".
	//CFileDialog fileDlg (FALSE, "保存对话框", NULL,
	//	OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	//// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	//// returns IDOK.
	//if( fileDlg.DoModal ()==IDOK )
	//{
	//	pathName=fileDlg.GetPathName();
	//	int k=pathName.ReverseFind('\\');
	//	m_place=pathName.Left(k);
	//	m_filename= fileDlg.GetFileTitle();
	//}
 //   UpdateData(FALSE);
	// TODO: 在此添加控件通知处理程序代码
//}
/************************************************************************/
/* 选择零件分类                                                         */
/************************************************************************/
void CPrtNewDlg::OnBnClickedButtonGrpfrm()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CSelectDlg selectdlg;
	selectdlg.Type="prt";
	if (IDOK==selectdlg.DoModal())
	{
		m_group=selectdlg.Name;
		detailinfo->GroupNum=selectdlg.num;
		UpdateData(FALSE);
	}
}
/************************************************************************/
/* 下载零件进行，修改参数按钮                                           */
/************************************************************************/
void CPrtNewDlg::OnBnClickedOk()
{
	//判断信息的完整性和正确性
	CCommonFuncClass commonfun;
	CPrtClass prtclass;
	CString Path,DrwName,SolidName;
	CFtpFile ftp;
	CString sql,str;
	BOOL IsLocal;
	int s=-1;
	UpdateData(TRUE);
	//判断是在本地还是在服务器
	int k=detailinfo->OldSolidFilePath.Find(":");
	if (k==-1)
	{
		IsLocal=FALSE;
	}
	else
	{
		IsLocal=TRUE;
	}
	if (IsLocal)
	{
		if (m_inscode=="" || m_insname=="" || m_filename=="" || m_group=="")
		{
			AfxMessageBox("你的信息不完全，请先填完整");
			return;
		}	
	}
	else
	{
		if (m_place=="" || m_filename=="" || m_inscode=="" || m_insname=="" || m_group=="")
		{
			AfxMessageBox("你的信息不完全，请先填完整");
			return;
		}
	}
	if (m_place.Right(1)=="\\")
	{
		k=m_place.ReverseFind('\\');
		m_place=m_place.Left(k);
	}
	m_filename.MakeLower();
	if (m_filename.Right(4)==".prt")
	{
         k=m_filename.ReverseFind('.');
		 if (k!=-1)
		 {
			 SolidName=m_filename.Left(k)+".prt";
			 DrwName=m_filename.Left(k)+".drw";
		 }
	}
	else// (k==-1)			 //说明没有后缀
	{
		SolidName=m_filename+".prt";
		DrwName=m_filename+".drw";
	}
	if (!prtclass.IsInsCodeOk(m_inscode,SolidName,DrwName))
	{
		return;
	}
	//判断新文件名是否存在
	if (!commonfun.IsFileExist(m_place,m_place,SolidName,DrwName))
	{
		return;
	}
	else
	{
		detailinfo->MdlName=m_mdlnum;
		detailinfo->InsCode=m_inscode;
		detailinfo->InsName=m_insname;
		detailinfo->Designer=m_designer;
		detailinfo->GrpFrm=m_group;
		detailinfo->Material=m_material;
		detailinfo->Unit=m_unit;
		detailinfo->Note=m_note;
		detailinfo->SolidFileName=SolidName;
		detailinfo->DrwName=DrwName;
		detailinfo->DrwFilePath=m_place;
		detailinfo->SolidFilePath=m_place;
		detailinfo->PDMCode=m_pdmcode;
		//判断是否是模型
		if (detailinfo->MdlNum=="-1")
		{
			if(!prtclass.OpenPartFile(detailinfo,&s,IsLocal))
			{
				return;
			}
			else
			{
				OnOK();
				detailinfo->IsInsert="F";
				prtclass.ActivatePart(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
				//设置参数信息
				if (!commonfun.SetParam(s)) 
				{
					AfxMessageBox("设置参数失败");
					return;
				}
				AfxMessageBox("这是非模型，请自行变型");
			}
			
		}
		else
		{
			if(!prtclass.OpenPartFile(detailinfo,&s,IsLocal))
			{
				return;
			}
			else
			{
				OnOK();
				CNewAlterDlg newalterdlg;
				newalterdlg.s=s;
				newalterdlg.MdlNum=detailinfo->MdlNum;
				if (!commonfun.SetParam(s)) 
				{
					AfxMessageBox("设置参数失败");
					return;
				}
				newalterdlg.DoModal();
			}
			
		}
	}
}
BOOL CPrtNewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		IsError=TRUE;
		return FALSE;
	}
	
	
	CCommonFuncClass commonfun;
	//去掉进程中没有的基本信息
	commonfun.RemoveNotInSession();
	commonfun.IsBasicInfoAlreadyHave();
	detailinfo =new DetailInfoClass;
	detailinfo->SolidID=-1;
	detailinfo->PerformanceValue.RemoveAll();
	detailinfo->InsValueStr.RemoveAll();
	detailinfo->IsInsert="F";
	GetDlgItem(IDOK)->EnableWindow(FALSE);
    
	//设置下载位置
	m_place=Ftp_struc.LocalPath;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CPrtNewDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!IsError)
	{
		delete detailinfo;
	}
	OnCancel();
}

/************************************************************************/
/* 得到模型的详细信息                                                   */
/************************************************************************/
BOOL CPrtNewDlg::GetPartModelInfo(CString MdlNum)
{
	CString sql,str;
	sql.Format("SELECT * FROM NPartManageTable WHERE Number=%s",MdlNum);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			m_insname=m_db.getString("Name");
			detailinfo->MdlName=m_db.getString("Name");
			detailinfo->GroupNum=m_db.getInt("TypeNum");
			m_oldfilename=m_db.getString("SolidFileName");
			detailinfo->OldSolidFileName=m_oldfilename;
			detailinfo->OldSolidFilePath=m_db.getString("SolidFilePlace");

			m_olddrwname=m_db.getString("DrawingFileName");
			detailinfo->OldDrwName=m_olddrwname;
			detailinfo->OldDrwFilePath=m_db.getString("DrawingFilePlace");
			m_material=m_db.getString("Material");
            break;
		} while(m_db.MoveNext());
	}		   
	detailinfo->DesignState="未完成";
	detailinfo->AuditState="审核未通过";
	detailinfo->IsLocalOrServer="本地";
	detailinfo->PDMState="未提交";
	//得到模型的性能参数
	//得到性能参数的值
	sql.Format("SELECT * FROM NPartPerfparaTable WHERE PrtNum=%s",MdlNum);
	Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		detailinfo->PerformanceValue.RemoveAll();
	}
	else
	{
		m_db.MoveBegin();
		do {
			str=m_db.getString("Value");
			detailinfo->PerformanceValue.Add(str);
		} while(m_db.MoveNext());
	}
	//根据类别好查找类别名
	sql.Format("SELECT * FROM NPartType where Number=%d",detailinfo->GroupNum);
	Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			m_group=m_db.getString("Name");
		} while(m_db.MoveNext());
	}
	UpdateData(FALSE);
	return TRUE;
}
/************************************************************************/
/* 得到零件的详细信息                                                   */
/************************************************************************/
BOOL CPrtNewDlg::GetPartInfo(CString InsCode)            //是零件号
{
	CString sql,str;
	CPrtClass prtclass;
	BOOL Find;
	sql.Format("select * from NPartInsInfoTable WHERE PrtNum=%s",InsCode);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			detailinfo->InsCode=m_db.getString("InsCode");
			detailinfo->PDMCode=m_db.getString("PDMNum");
			detailinfo->PDMState=m_db.getString("PDMState");
			detailinfo->OldSolidID=m_db.getInt("PrtNum");
			detailinfo->GroupNum=m_db.getInt("TypeNum");
			detailinfo->MdlNum.Format("%d",m_db.getInt("MdlNum"));
			m_oldfilename=m_db.getString("SolidFileName");
			detailinfo->OldSolidFileName=m_oldfilename;
			detailinfo->OldSolidFilePath=m_db.getString("SolidPlace");

			m_olddrwname=m_db.getString("DrawingName");
			detailinfo->OldDrwName=m_olddrwname;
			detailinfo->OldDrwFilePath=m_db.getString("DrawingPlace");
			m_material=m_db.getString("Material");
			m_unit=m_db.getString("Unit");
			break;
		} while(m_db.MoveNext());
	}
//	得到PDM的保存位置和文件名
	//if (detailinfo->PDMState=="提交")
	//{
	//	if (!prtclass.GetDocumentsFromPDM(Ftp_struc.Database,detailinfo,&Find))    //这次调用，最后这个参数没有用
	//	{
	//		return FALSE;
	//	}
	//}

///////////得到保存位置和文件名	
		if (!prtclass.GetDocumentsFromNoPDM(detailinfo,&Find))    //这次调用，最后这个参数没有用
		{
			return FALSE;
		}
	

    //得到模型的名字
	sql.Format("SELECT * FROM NPartManageTable WHERE Number=%s",detailinfo->MdlNum);
	Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			detailinfo->MdlName=m_db.getString("Name");
			m_mdlnum=detailinfo->MdlName;
		} while(m_db.MoveNext());
	}
	detailinfo->DesignState="未完成";
	detailinfo->AuditState="审核未通过";
	detailinfo->IsLocalOrServer="本地";
	detailinfo->PDMState="未提交";
	//查找性能参数
	sql.Format("SELECT * FROM NPartInsPerfTable WHERE PrtNum=%d",detailinfo->OldSolidID);
	Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		detailinfo->PerformanceValue.RemoveAll();
	}
	else
	{
		m_db.MoveBegin();
		do {
			str=m_db.getString("Value");
			detailinfo->PerformanceValue.Add(str);
		} while(m_db.MoveNext());
	}

	//根据类别好查找类别名
	sql.Format("SELECT * FROM NPartType where Number=%d",detailinfo->GroupNum);
	Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			m_group=m_db.getString("Name");
		} while(m_db.MoveNext());
	}
	return TRUE;
}
void CPrtNewDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: 在此添加消息处理程序代码
}
void CPrtNewDlg::OnBnClickedButton5()
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

void CPrtNewDlg::OnBnClickedButton6()
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

void CPrtNewDlg::OnBnClickedButton7()
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
