// NewPartTypeDlg.cpp : 实现文件
//用于对零件目录管理时，新定义一个零件类别的对话框

#include "stdafx.h"
#include "NewPartTypeDlg.h"
//extern int DlgIcon;

// CNewPartTypeDlg 对话框

IMPLEMENT_DYNAMIC(CNewPartTypeDlg, CDialog)
CNewPartTypeDlg::CNewPartTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewPartTypeDlg::IDD, pParent)
	, m_TypeName(_T(""))
	, m_TypeNote(_T(""))
	, m_FatherName(_T(""))
	, m_Code(_T(""))
{
}

CNewPartTypeDlg::~CNewPartTypeDlg()
{
}

void CNewPartTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_TypeName);
	DDX_Text(pDX, IDC_EDIT3, m_TypeNote);
	DDX_Text(pDX, IDC_EDIT2, m_FatherName);
	DDX_Text(pDX, IDC_EDIT4, m_Code);
}


BEGIN_MESSAGE_MAP(CNewPartTypeDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CNewPartTypeDlg 消息处理程序



int CNewPartTypeDlg::SetFatherName(CString FatherName,CString FatherCode)
{
	m_FatherName=FatherName;
	if(FatherCode=="") m_Code="(请添加编码)";
	else
	{
		m_Code=FatherCode+"."+"(请添加编码)";
	}	
	return 0;
}

BOOL CNewPartTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
//	HICON hIcon = AfxGetApp()->LoadIcon (DlgIcon);
	//SetIcon (hIcon, TRUE);     // Set small icon  设置小图标
	// TODO:  在此添加额外的初始化
    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/*  单击确定添加之后，进行有效性验证                                    */
/************************************************************************/
void CNewPartTypeDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//if(m_TypeName=="")
	//{
	//	AfxMessageBox("名称为空，无法创建！");
	//	return;
	//}
	//int temp=0;
	//temp=m_Code.FindOneOf("(请添加编码)");///
	//if(temp!=-1) 
	//{
	//	AfxMessageBox("请输入编码！");
	//	return;
	//}
	CDialog::OnOK();
}

void CNewPartTypeDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_TypeName="";
	CDialog::OnCancel();
}
