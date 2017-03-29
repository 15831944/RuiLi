// AddMdlSeriesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "AddMdlSeriesDlg.h"


// CAddMdlSeriesDlg 对话框

IMPLEMENT_DYNAMIC(CAddMdlSeriesDlg, CDialog)
CAddMdlSeriesDlg::CAddMdlSeriesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddMdlSeriesDlg::IDD, pParent)
	,p_InsInfoList(NULL)
	, strCaption(_T(""))	
	,time(_T(""))
{
	newMdl.Name="";
	newMdl.Code="";
	newMdl.Person="";
	newMdl.Dept="";
	newMdl.Note="";
	newMdl.mdlNum=-1;
}

CAddMdlSeriesDlg::~CAddMdlSeriesDlg()
{
}

void CAddMdlSeriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, newMdl.Name);
	DDX_Text(pDX, IDC_EDIT2, newMdl.Code);
	DDX_Text(pDX, IDC_EDIT3, newMdl.Person);
	DDX_Text(pDX, IDC_EDIT4, newMdl.Dept);
	DDX_Text(pDX, IDC_EDIT5, newMdl.Note);
	DDX_Text(pDX, IDC_EDIT6, time);
}


BEGIN_MESSAGE_MAP(CAddMdlSeriesDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CAddMdlSeriesDlg 消息处理程序

void CAddMdlSeriesDlg::SetMdlNumberAndCode(long mdlNumber, CString code,CArray<MdlInsInfo,MdlInsInfo> * InsInfoList)
{
	
	if (mdlNumber!=-1) {
		newMdl.mdlNum=mdlNumber;
		newMdl.Code=code+".(请输入)";
		p_InsInfoList=InsInfoList;
	}
	else
	{
		newMdl.mdlNum=mdlNumber;
	}
}

BOOL CAddMdlSeriesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
    //m_Code=newMdl.Code;
    if (!strCaption.IsEmpty()) {
		SetWindowText(strCaption);
    }
	//如果是添加系列，则时间应该为系统时间
	if (newMdl.m_time.GetYear()<2005) {
		newMdl.m_time=CTime::GetCurrentTime();
	}	
	time=newMdl.m_time.Format("%Y-%A-%d %H:%M:%S");
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CAddMdlSeriesDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//对于查看信息
	if (newMdl.mdlNum==-1) {
        OnOK();
		return;
	}
	//对于新加
	UpdateData(TRUE);
	//检查
	if(newMdl.Name=="")
	{
		AfxMessageBox("请输入名称！");
		return;
	}
	if (newMdl.Code.FindOneOf("(请输入)")!=-1) {
		AfxMessageBox("编码格式不对！");
		return;
	}
	for(int i=0;i<(int)p_InsInfoList->GetCount();i++)
	{
		if (newMdl.Name==p_InsInfoList->GetAt(i).Name) {
			AfxMessageBox("系列中已经有相同的名字，请更改！");
            return;
		}
		if (newMdl.Code==p_InsInfoList->GetAt(i).Code) {
			AfxMessageBox("系列中已经有相同的编码，请更改！");
			return;
		}
	}
	OnOK();
}

void CAddMdlSeriesDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码	

	OnCancel();
}
