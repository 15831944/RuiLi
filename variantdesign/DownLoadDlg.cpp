// DownLoadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "DownLoadDlg.h"
#include "DetailInfoClass.h"
#include "FtpFile.h"
#include "CommonFuncClass.h"
#include "ProUtil.h"
#include "ProMdl.h"
#include "ProSolid.h"
#include "MyDatabase.h"
#include "Compute.h"
#include "ProWindows.h"
#include "afxtempl.h"
#include "CheckDlg.h"
#include "ProWindows.h"
#include "PrtClass.h"
#include "ProductClass.h"


extern CPtrArray p_classarray;
extern CArray <FilePath,FilePath> FilePath_struc;
extern CMyDatabase m_db;
extern FtpInfo Ftp_struc;
extern CArray <FilePath,FilePath> FilePath_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;

extern CCheckDlg checkdlg;

// CDownLoadDlg 对话框

IMPLEMENT_DYNAMIC(CDownLoadDlg, CDialog)
CDownLoadDlg::CDownLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownLoadDlg::IDD, pParent)
	, m_oldsolidname(_T(""))
	, m_olddrwname(_T(""))
	, m_solidplace(_T(""))
	, m_solidname(_T(""))
{
	s=-1;
}

CDownLoadDlg::~CDownLoadDlg()
{
}

void CDownLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OLDSOLIDNAME, m_oldsolidname);
	DDX_Text(pDX, IDC_EDIT_OLDDRWNAME, m_olddrwname);
	DDX_Text(pDX, IDC_EDIT_SOLIDPLACE, m_solidplace);
	DDX_Text(pDX, IDC_EDIT_SOLIDNAME1, m_solidname);
}


BEGIN_MESSAGE_MAP(CDownLoadDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SOLIDPLACE, OnBnClickedButtonSolidplace)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_SOLIDNAME1, OnEnChangeEditSolidname1)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CDownLoadDlg 消息处理程序

BOOL CDownLoadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CCommonFuncClass commonfun;
	int k;
	//去掉进程中没有的基本信息
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	CString sql;
	if (IsPart)
	{
		if(info->OldSolidFileName.Right(4)!=".prt")
		{
			m_oldsolidname=info->OldSolidFileName;
			k=m_oldsolidname.ReverseFind('.');
			if (k!=-1)      //说明没有
			{
				m_oldsolidname.Delete(k,(m_oldsolidname.GetLength()-k));
			}
		}
		else
		{
			  m_oldsolidname=info->OldSolidFileName;
		}
		if (info->OldDrwName!="")                  //有二维图
		{
			if(info->OldDrwName.Right(4)!=".drw")
			{
				m_olddrwname=info->OldDrwName;
				k=m_olddrwname.ReverseFind('.');
				if (k!=-1)      //说明没有
				{
					m_olddrwname.Delete(k,(m_olddrwname.GetLength()-k));
				}
			}
			else
			{
				m_olddrwname=info->OldDrwName;
			}
		}
		k=m_oldsolidname.Find(".prt");
		if (k!=-1)
		{
			m_solidname=m_oldsolidname.Left(k);
		}
	}
	else        //下载产品
	{
		sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%s",InsCode);   //InsCode 是产品号，不是产品图号
		int Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
		{
			AfxMessageBox("无法打开这个产品");
			return FALSE;
		}
		else
		{
			m_db.MoveBegin();
			do {
				m_oldsolidname=m_db.getString("SolidFileName");
				m_olddrwname=m_db.getString("DrawingName");
			} while(m_db.MoveNext());
			if(m_oldsolidname.Right(4)!=".asm")
			{
				k=m_oldsolidname.ReverseFind('.');
				if (k!=-1)      //说明没有
				{
					m_oldsolidname.Delete(k,(m_oldsolidname.GetLength()-k));
				}
			}
			if(m_olddrwname.Right(4)!=".drw")
			{
				//m_olddrwname=info->OldDrwName;
				k=m_olddrwname.ReverseFind('.');
				if (k!=-1)      //说明没有
				{
					m_olddrwname.Delete(k,(m_olddrwname.GetLength()-k));
				}
			}
			//else
			//{
			//	m_olddrwname=info->OldSolidFileName;
			//}
		}
		k=m_oldsolidname.Find(".asm");
		if (k!=-1)
		{
			m_solidname=m_oldsolidname.Left(k);
		}
	}
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 选择三维下载的位置                                                   */
/************************************************************************/
void CDownLoadDlg::OnBnClickedButtonSolidplace()
{
	// TODO: 在此添加控件通知处理程序代码
	CString pathName;
	CString path;
	CString FileName;
	char szFilters[]=
		"Prt Files (*.prt)|*.prt.*|Drw Files (*.drw)|*.drw.*|Asm Files (*.asm)|*.asm.*|All Files (*.*)|*.*||";

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg (FALSE, "保存对话框", NULL,
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if( fileDlg.DoModal ()==IDOK )
	{
		pathName=fileDlg.GetPathName();
		int k=pathName.ReverseFind('\\');
		m_solidplace=pathName.Left(k);
		//m_solidname= fileDlg.GetFileTitle();
	}
	UpdateData(FALSE);
}

void CDownLoadDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CFtpFile ftp;
	ProSolid solid;
	ProError status;
	int w_id,k;
	CString SolidName,DrwName,sql;
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CPrtClass prtclass;
	CString temppath,DrwPath,SolidPath,str;
	//对零件操作
	if (IsPart)
	{  
		if (m_solidplace=="" || m_solidname=="")
		{
			AfxMessageBox("你的信息不完全，请先填完整");
			return;
		}
		if(m_solidname.Right(4)==".prt")
		{
			k=m_solidname.ReverseFind('.');
			info->SolidFileName=m_solidname.Left(k)+".prt";
			info->DrwName=m_solidname.Left(k)+".drw";
		}
		else
		{
			info->SolidFileName=m_solidname+".prt";
			info->DrwName=m_solidname+".drw";
		}
		if (m_solidplace.Right(1)=="\\")
		{
			k=m_solidplace.ReverseFind('\\');
			if (k!=-1)
			{
				m_solidplace=m_solidplace.Left(k);
			}
		}
		info->SolidFilePath=m_solidplace;
		info->DrwFilePath=m_solidplace;
		//判断要下载的（本地目录+模型的名字）这个零件是否存在
		if (!commonfun.IsFileExist(info->SolidFilePath,info->DrwFilePath,info->SolidFileName,info->DrwName))
		{
			delete info;
			return;
		}
		//根据名字判断内存中是否有该模型
		//看进程中是否有相同的零
		if (!prtclass.IsPartFileAlreadyInSession(info->SolidFileName,info->DrwName))
		{
			AfxMessageBox("在内存中已经有同名文件存在，请清除内存，再打开这个零件");
			return;
		}
		//下载文件
		if (!prtclass.DownLoadPart(info->OldSolidFileName,info->OldSolidFilePath,info->OldDrwName,info->OldDrwFilePath,info->SolidFilePath))
		{
			AfxMessageBox("无法下载该零件");
			return;
		}
		else
		{
			if (!prtclass.OpenPartFile(info->OldSolidFileName,info->SolidFilePath,info->OldDrwName,info->DrwFilePath,&solid))
			{
				AfxMessageBox("零件打开失败");
				return;
			}
			else
			{
				info->solid=solid;
				p_classarray.Add(info);
				//还要改名
				if (!pdtclass.NewRename(solid,info->SolidFileName,&(info->DrwName)))
				{
					AfxMessageBox("重命名失败！");
					return;
				}
			}
		}
		sql.Format("SELECT * FROM NPartInsInfoTable WHERE PrtNum=%d",info->SolidID);
		if (!prtclass.GetInfoFromDatabaseBase((int)p_classarray.GetSize()-1,sql))
		{
			AfxMessageBox("无法获得这个零件的详细信息");
			return;
		}
		status = ProMdlDisplay((ProMdl)solid);
		//commonfun.AddMdlInfo(solid);
		ProMdlWindowGet((ProMdl)solid,&w_id);
		status = ProWindowActivate(w_id);
		OnOK();
	}
	//产品操作
	else
	{
		if (m_solidplace=="")
		{
			AfxMessageBox("你的信息不完全，请先填完整");
			return;
		}
		else
		{
			if (m_solidplace.Right(1)="\\")
			{
				int k=m_solidplace.ReverseFind('\\');
				if (k!=-1)
				{
					m_solidplace=m_solidplace.Left(k);
				}
			}
		}
	}
	OnOK();
}

void CDownLoadDlg::OnEnChangeEditSolidname1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CDownLoadDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
    ProWindowRepaint(PRO_VALUE_UNUSED);
	// TODO: 在此添加消息处理程序代码
}
