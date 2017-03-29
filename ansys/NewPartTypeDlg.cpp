// NewPartTypeDlg.cpp : ʵ���ļ�
//���ڶ����Ŀ¼����ʱ���¶���һ��������ĶԻ���

#include "stdafx.h"
#include "NewPartTypeDlg.h"
//extern int DlgIcon;

// CNewPartTypeDlg �Ի���

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


// CNewPartTypeDlg ��Ϣ�������



int CNewPartTypeDlg::SetFatherName(CString FatherName,CString FatherCode)
{
	m_FatherName=FatherName;
	if(FatherCode=="") m_Code="(����ӱ���)";
	else
	{
		m_Code=FatherCode+"."+"(����ӱ���)";
	}	
	return 0;
}

BOOL CNewPartTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
//	HICON hIcon = AfxGetApp()->LoadIcon (DlgIcon);
	//SetIcon (hIcon, TRUE);     // Set small icon  ����Сͼ��
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/*  ����ȷ�����֮�󣬽�����Ч����֤                                    */
/************************************************************************/
void CNewPartTypeDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//if(m_TypeName=="")
	//{
	//	AfxMessageBox("����Ϊ�գ��޷�������");
	//	return;
	//}
	//int temp=0;
	//temp=m_Code.FindOneOf("(����ӱ���)");///
	//if(temp!=-1) 
	//{
	//	AfxMessageBox("��������룡");
	//	return;
	//}
	CDialog::OnOK();
}

void CNewPartTypeDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TypeName="";
	CDialog::OnCancel();
}
