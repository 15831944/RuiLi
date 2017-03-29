// PrtInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PrtInfoDlg.h"
#include "DetailInfoClass.h"
#include "CommonFuncClass.h"
#include "MyDatabase.h"
#include "MaterialDlg.h"
#include "UnitDlg.h"
#include "UserNameDlg.h"


// CPrtInfoDlg 对话框
extern CPtrArray p_classarray;
extern CMyDatabase m_db;

IMPLEMENT_DYNAMIC(CPrtInfoDlg, CDialog)
CPrtInfoDlg::CPrtInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrtInfoDlg::IDD, pParent)
	, m_inscode(_T(""))
	, m_insname(_T(""))
	, m_mdlnum(_T(""))
	, m_designer(_T(""))
	, m_group(_T(""))
	, m_material(_T(""))
	, m_unit(_T(""))
	, m_note(_T(""))
	, m_place(_T(""))
	, m_drwingplace(_T(""))
	, m_filename(_T(""))
	, m_drwname(_T(""))
	, m_audit(_T(""))
{
	s=-1;
}

CPrtInfoDlg::~CPrtInfoDlg()
{
}

void CPrtInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CODE, m_inscode);
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_insname);
	DDX_Text(pDX, IDC_EDIT_MDLFRM, m_mdlnum);
	DDX_Text(pDX, IDC_DESIGNER, m_designer);
	DDX_Text(pDX, IDC_EDIT_GRPFRM, m_group);
	DDX_Text(pDX, IDC_EDIT_MATERIAL, m_material);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_unit);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	DDX_Text(pDX, IDC_EDIT_PLACE, m_place);
	DDX_Text(pDX, IDC_EDIT_DRWPLACE, m_drwingplace);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
	DDX_Text(pDX, IDC_EDIT_DRWNAME, m_drwname);
	DDX_Control(pDX, IDC_LIST_PER, m_perf);
	DDX_Text(pDX, IDC_EDIT_AUDITOR, m_audit);
}


BEGIN_MESSAGE_MAP(CPrtInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM2, OnBnClickedButtonGrpfrm2)
	ON_BN_CLICKED(IDC_BUTTON_GRPFRM3, OnBnClickedButtonGrpfrm3)
	ON_BN_CLICKED(IDC_BUTTON_de, OnBnClickedButtonde)
END_MESSAGE_MAP()


// CPrtInfoDlg 消息处理程序

BOOL CPrtInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	CCommonFuncClass commonfun;
	CString str;
	CString sql;
	if (s==-1)
	{
		return FALSE;
	}
	else
	{   
		if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum=="-1")
		{
			m_mdlnum="无";
		}
		else
		    m_mdlnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlName;
		m_inscode=((DetailInfoClass *)p_classarray.GetAt(s))->InsCode;
		m_insname=((DetailInfoClass *)p_classarray.GetAt(s))->InsName;
		m_designer=((DetailInfoClass *)p_classarray.GetAt(s))->Designer;
		m_group=((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm;
		m_material=((DetailInfoClass *)p_classarray.GetAt(s))->Material;
		m_unit=((DetailInfoClass *)p_classarray.GetAt(s))->Unit;
		m_note=((DetailInfoClass *)p_classarray.GetAt(s))->Note;
		sql.Format("select * FROM NPartInsInfoTable where PrtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
			int Z=m_db.Query(sql);
		if (Z==0 || Z==-1)
		{
			return FALSE;
		}
		else
		{
			m_filename=m_db.getString("SolidFileName");
			m_drwname=m_db.getString("DrawingName");
			m_drwingplace=m_db.getString("DrawingPlace");
			m_place=m_db.getString("SolidPlace");
		}
	}
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum!="-1")
	{
		//设置性能参数
		m_perf.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
		m_perf.InsertColumn(0,"序号",LVCFMT_LEFT,40,-1);
		m_perf.InsertColumn(1,"性能参数名",LVCFMT_LEFT,80,-1);
		m_perf.InsertColumn(2,"性能参数",LVCFMT_LEFT,100,-1);
		m_perf.InsertColumn(3,"备注",LVCFMT_LEFT,100,-1);
		//设置只读行
		m_perf.SetReadOnlyColumns(0,false);
		m_perf.SetReadOnlyColumns(1,false);
		m_perf.SetReadOnlyColumns(3,false);
		//显示性能参数
		//读数据库，显示该模型的性能参数
		int i=1;
		sql.Format("select * from NPartPerfparaTable WHERE PrtNum=%s",((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum);
		int Z=m_db.Query(sql);
		if (Z==-1)
		{
			return FALSE;
		}
		else
			if (m_db.Query(sql)!=0)
			{
				m_db.MoveBegin();
				do {
					str.Format("%d",i);
					m_perf.InsertItem(i-1,str);
					str=m_db.getString("Name");
					m_perf.SetItemText(i-1,1,str);
					str=m_db.getString("Note");
					m_perf.SetItemText(i-1,3,str);
					i++;
				} while(m_db.MoveNext());
			}
			for(i=0;i<(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).GetSize();i++)
			{
				m_perf.SetItemText(i,2,(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue)[i]);
			}
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPrtInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str,sql;
	ProSolid solid;
	ProMdlCurrentGet((ProMdl*)&solid);
	CCommonFuncClass commonfun;
	UpdateData(TRUE);
	if (m_audit=="" || m_inscode=="")
	{
		AfxMessageBox("你的信息不完全，请先填完整");
		return;
	}
	((DetailInfoClass *)p_classarray.GetAt(s))->SaveStyle=1;
	if (((DetailInfoClass *)p_classarray.GetAt(s))->InsCode.MakeLower()!=m_inscode.MakeLower())
	{
		sql.Format("SELECT * FROM NPartInsInfoTable where InsCode='%s'",m_inscode);
		if (commonfun.IsHave(sql))
		{
			AfxMessageBox("这个图号已经存在，请修改");
			return;
		}
		sql.Format("SELECT * FROM NProductInsInfoTable where InsCode='%s'",m_inscode);
		if (commonfun.IsHave(sql))
		{
			AfxMessageBox("这个图号已经存在，请修改");
			return;
		}
	}
	//更新性能参数表
	((DetailInfoClass *)p_classarray.GetAt(s))->MdlName=m_mdlnum;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsCode=m_inscode;
	((DetailInfoClass *)p_classarray.GetAt(s))->InsName=m_insname;
	((DetailInfoClass *)p_classarray.GetAt(s))->Designer=m_designer;
	((DetailInfoClass *)p_classarray.GetAt(s))->GrpFrm=m_group;
	((DetailInfoClass *)p_classarray.GetAt(s))->Material=m_material;
	((DetailInfoClass *)p_classarray.GetAt(s))->Unit=m_unit;
	((DetailInfoClass *)p_classarray.GetAt(s))->Note=m_note;
	((DetailInfoClass *)p_classarray.GetAt(s))->Auditor=m_audit;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFileName=m_filename;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwName=m_drwname;
	((DetailInfoClass *)p_classarray.GetAt(s))->DrwFilePath=m_drwingplace;
	((DetailInfoClass *)p_classarray.GetAt(s))->SolidFilePath=m_place;

	(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).RemoveAll();
	//更新性能参数表
	for(int i=0;i<m_perf.GetItemCount();i++)
	{
		str=m_perf.GetItemText(i,2);
		(((DetailInfoClass *)p_classarray.GetAt(s))->PerformanceValue).Add(str);
	}
	OnOK();
}

void CPrtInfoDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CPrtInfoDlg::OnBnClickedButtonGrpfrm2()
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

void CPrtInfoDlg::OnBnClickedButtonGrpfrm3()
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

void CPrtInfoDlg::OnBnClickedButtonde()
{	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CUserNameDlg dlg;
	dlg.m_UserName=m_designer;
	if(dlg.DoModal()==IDOK)
	{
		m_designer=dlg.m_UserName;
		UpdateData(FALSE);
	}
}
