// PtdNewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PtdNewDlg.h"
#include "AsmQueryDlg.h"
#include "CommonFuncClass.h"
#include "FtpFile.h"
#include "PdtAlterDlg.h"
#include "MyDatabase.h"
#include "ProductClass.h"
#include "SearchDlg.h"
#include "Prowindows.h"
#include "SelectDlg.h"
#include "UserNameDlg.h"


extern CMyDatabase m_db;
extern FtpInfo Ftp_struc;
extern CPtrArray p_classarray;
// CPtdNewDlg 对话框

IMPLEMENT_DYNAMIC(CPtdNewDlg, CDialog)
CPtdNewDlg::CPtdNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPtdNewDlg::IDD, pParent)
	, m_oldsolidname(_T(""))
	, m_olddrwname(_T(""))
	, m_inscode(_T(""))
	, m_mdlnum(_T(""))
	, m_insname(_T(""))
	, m_designer(_T(""))
	, m_group(_T(""))
	, m_note(_T(""))
	, m_place(_T(""))
	, m_filename(_T(""))
	, m_pdmcode(_T(""))
{
	IsError=FALSE;
}

CPtdNewDlg::~CPtdNewDlg()
{
}

void CPtdNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SOLIDNAME, m_oldsolidname);
	DDX_Text(pDX, IDC_EDIT_OLDDRWNAME, m_olddrwname);
	DDX_Text(pDX, IDC_EDIT_CODE, m_inscode);
	DDX_Text(pDX, IDC_EDIT_MDLFRM, m_mdlnum);
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_insname);
	DDX_Text(pDX, IDC_DESIGNER, m_designer);
	DDX_Text(pDX, IDC_EDIT_GRPFRM, m_group);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	//DDX_Text(pDX, IDC_EDIT_PLACE, m_place);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
	DDX_Text(pDX, IDC_EDIT_PDMCode, m_pdmcode);
}


BEGIN_MESSAGE_MAP(CPtdNewDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PTDMDL, OnBnClickedButtonPtdmdl)
	//ON_BN_CLICKED(IDC_BUTTON_SEL, OnBnClickedButtonSel)
	//ON_BN_CLICKED(IDC_BUTTON_DRWPLACE, OnBnClickedButtonDrwplace)
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM, OnBnClickedButtonGrpfrm)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SELPRT, OnBnClickedButtonSelprt)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM2, OnBnClickedButtonGrpfrm2)
END_MESSAGE_MAP()


// CPtdNewDlg 消息处理程序
/************************************************************************/
/* 选产品模型                                                           */
/************************************************************************/
void CPtdNewDlg::OnBnClickedButtonPtdmdl()
{
	CAsmQueryDlg asmquerydlg;
	asmquerydlg.DoModal();
	if (!asmquerydlg.IsSelect)
	{
		return;//需要传出来一个模型编号
	}
	else
	{
		FromModel=TRUE;					   //从模型中选
		detailinfo->MdlNum=asmquerydlg.MdlNum;
		GetAsmModelInfo(detailinfo->MdlNum);
		//还要根据这个得到模型的详细信息

		GetDlgItem(IDOK)->EnableWindow(TRUE); 
		UpdateData(FALSE);
	}
}
/************************************************************************/
/* 选择产品三维模型的保存位置                                           */
/************************************************************************/
//void CPtdNewDlg::OnBnClickedButtonSel()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//	CString pathName;
//	CString path;
//	CString FileName;
//	char szFilters[]=
//		"Asm Files (*.asm)|*.asm.*|All Files (*.*)|*.*||";
//
//	// Create an Open dialog; the default file name extension is ".my".
//	CFileDialog fileDlg (FALSE, "保存对话框", NULL,
//		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
//
//	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
//	// returns IDOK.
//	if( fileDlg.DoModal ()==IDOK )
//	{
//		pathName=fileDlg.GetPathName();
//		int k=pathName.ReverseFind('\\');
//		m_place=pathName.Left(k);
//		m_filename=fileDlg.GetFileTitle();
//	}
//	UpdateData(FALSE);
//}
/************************************************************************/
/* 选择产品类别按钮                                                     */
/************************************************************************/
void CPtdNewDlg::OnBnClickedButtonGrpfrm()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CSelectDlg selectdlg;
	selectdlg.Type="asm";
	if (IDOK==selectdlg.DoModal())
	{
		m_group=selectdlg.Name;
		detailinfo->GroupNum=selectdlg.num;
		UpdateData(FALSE);
	}
}
/************************************************************************/
/* 把输入的基本信息写入到结构中去                                       */
/************************************************************************/
void CPtdNewDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CString Path,SolidName,DrwName;
	CFtpFile ftp;
	BOOL IsLocal;
	int s;
	UpdateData(TRUE);
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
		if (m_inscode=="" || m_insname=="" || m_filename=="")
		{
			AfxMessageBox("你的信息不完全，请先填完整");
			return;
		}	
	}
	if (m_place=="" || m_filename=="" || m_inscode=="" || m_insname=="")
	{
		AfxMessageBox("你的信息不完全，请先填完整");
		return;
	}
	if (m_place.Right(1)=="\\")
	{
		k=m_place.ReverseFind('\\');
		m_place=m_place.Left(k);
	}
	m_filename.MakeLower();
	if (m_filename.Right(4)==".asm")
	{
         k=m_filename.ReverseFind('.');
		 if (k!=-1)
		 {
			 SolidName=m_filename.Left(k)+".asm";
			 DrwName=m_filename.Left(k)+".drw";
		 }
	}
	else// (k==-1)			 //说明没有后缀
	{
		SolidName=m_filename+".asm";
		DrwName=m_filename+".drw";
	}
	//判断图号是否符合要求
	if (!pdtclass.IsInsCodeOk(m_inscode,SolidName,DrwName))
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
		detailinfo->SolidID=-1;
		detailinfo->MdlName=m_mdlnum;
		detailinfo->InsCode=m_inscode;
		detailinfo->PDMCode=m_pdmcode;
		detailinfo->InsName=m_insname;
		detailinfo->Designer=m_designer;
		detailinfo->GrpFrm=m_group;
		detailinfo->Note=m_note;
		detailinfo->SolidFileName=SolidName;
		detailinfo->DrwName=DrwName;
		detailinfo->DrwFilePath=m_place;
		detailinfo->SolidFilePath=m_place;
		OnOK();
		//判断是否是模型
		if (detailinfo->MdlNum=="-1")
		{ 
			//实现产品模型的下载和改名
			if (!pdtclass.NewProductFile(detailinfo,&s,FromModel))
			{
				return;
			}
			if (!pdtclass.ActivateAsm(detailinfo->solid))
			{
				AfxMessageBox("打开窗口失败");
				return;
			}
		}
		else
		{   
			//实现产品模型的下载和改名
			if (!pdtclass.NewProductFile(detailinfo,&s,FromModel))
			{
				return;
			}
			else
			{
				CPdtAlterDlg pdtalterdlg;
				pdtalterdlg.Pos=s;
				pdtalterdlg.IsLocal=IsLocal;
				pdtalterdlg.MdlNum=detailinfo->MdlNum;
				pdtalterdlg.DoModal();
			}
		}
	}
	OnOK();
}

BOOL CPtdNewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CCommonFuncClass commonfun;

	commonfun.IsBasicInfoAlreadyHave();
	commonfun.RemoveNotInSession();
    
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
        this->SendMessage(WM_CLOSE,0,0);
		IsError=TRUE;
		return FALSE;
	}
	detailinfo= new DetailInfoClass;
	detailinfo->SolidID=-1;
	detailinfo->Type="asm";
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_place=Ftp_struc.LocalPath;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CPtdNewDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!IsError)
	{
		delete detailinfo;
	}
	OnCancel();
}
/************************************************************************/
/* 得到产品模型的详细信息                                               */
/************************************************************************/
BOOL CPtdNewDlg::GetAsmModelInfo(CString MdlNum)
{
	CString sql,str;
	sql.Format("SELECT * FROM NAsmManageTable WHERE Number=%s",MdlNum);
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
            m_mdlnum=detailinfo->MdlName;
			detailinfo->GroupNum=m_db.getInt("TypeNum");
			m_oldsolidname=m_db.getString("SolidFileName");

			detailinfo->OldSolidFileName=m_oldsolidname;
			detailinfo->OldSolidFilePath=m_db.getString("SolidFilePlace");

			m_olddrwname=m_db.getString("DrawingFileName");
			detailinfo->OldDrwName=m_olddrwname;
			detailinfo->OldDrwFilePath=m_db.getString("DrawingFilePlace");

			break;
		} while(m_db.MoveNext());
	}
	detailinfo->DesignState="未完成";
	detailinfo->AuditState="审核未通过";
	detailinfo->IsLocalOrServer="本地";
	detailinfo->PDMState="未提交";
	//根据类别好查找类别名
	sql.Format("SELECT * FROM NAsmType where Number=%d",detailinfo->GroupNum);
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
/* 选择已有产品                                                         */
/************************************************************************/
void CPtdNewDlg::OnBnClickedButtonSelprt()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sql;
	CString InsCode;
	CSearchDlg searchdlg;
	BOOL IsLocal;
	searchdlg.DoModal();                    //产品查询对话框
	if (!searchdlg.IsSelect)
	{
		return;
	}
	else
	{
		InsCode=searchdlg.InsCode;          //传过来的是VDS的产品号，不是产品的"图号"
		if (!GetProductInsInfo(InsCode))
		{
			AfxMessageBox("无法得到这个零件的详细信息");
			return;
		}
		else
		{
			FromModel=FALSE;               //说明从原有实例进行变型，而不是从模型变型
			GetDlgItem(IDOK)->EnableWindow(TRUE); 
			int k=detailinfo->OldSolidFilePath.Find(":");
			if (k==-1)
			{
				IsLocal=FALSE;
				//GetDlgItem(IDC_EDIT_PLACE)->EnableWindow(TRUE);		   //下载的本地位置可选
				//GetDlgItem(IDC_BUTTON_SEL)->EnableWindow(TRUE);
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
/* 得到产品模型的详细信息                                               */
/************************************************************************/
BOOL CPtdNewDlg::GetProductInsInfo(CString InsCode)               //传入的是产品的产品号
{
	CString sql,str;
	sql.Format("SELECT * FROM NProductInsInfoTable WHERE PdtNum=%s",InsCode);
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
			detailinfo->OldSolidID=m_db.getInt("PdtNum");
			m_insname=m_db.getString("Name");
			detailinfo->GroupNum=m_db.getInt("TypeNum");
			m_oldsolidname=m_db.getString("SolidFileName");

			detailinfo->OldSolidFileName=m_oldsolidname;
			detailinfo->OldSolidFilePath=m_db.getString("SolidPlace");

			m_olddrwname=m_db.getString("DrawingName");
			detailinfo->OldDrwName=m_olddrwname;
			detailinfo->OldDrwFilePath=m_db.getString("DrawingPlace");
			detailinfo->MdlNum.Format("%d",m_db.getInt("MdlNum"));
			break;
		} while(m_db.MoveNext());
	}
	//得到模型的名字
	sql.Format("SELECT * FROM NAsmManageTable WHERE Number=%s",detailinfo->MdlNum);
	Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			detailinfo->MdlName=m_db.getString("Name");
			m_mdlnum=detailinfo->MdlName;
			break;
		} while(m_db.MoveNext());
	}
	detailinfo->DesignState="未完成";
	detailinfo->AuditState="审核未通过";
	detailinfo->IsLocalOrServer="本地";
	detailinfo->PDMState="未提交";
	//根据类别好查找类别名
	sql.Format("SELECT * FROM NAsmType where Number=%d",detailinfo->GroupNum);
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
void CPtdNewDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: 在此添加消息处理程序代码
}

void CPtdNewDlg::OnBnClickedButtonGrpfrm2()
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
