// AddMdlSeriesDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "AddMdlSeriesDlg.h"


// CAddMdlSeriesDlg �Ի���

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


// CAddMdlSeriesDlg ��Ϣ�������

void CAddMdlSeriesDlg::SetMdlNumberAndCode(long mdlNumber, CString code,CArray<MdlInsInfo,MdlInsInfo> * InsInfoList)
{
	
	if (mdlNumber!=-1) {
		newMdl.mdlNum=mdlNumber;
		newMdl.Code=code+".(������)";
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    //m_Code=newMdl.Code;
    if (!strCaption.IsEmpty()) {
		SetWindowText(strCaption);
    }
	//��������ϵ�У���ʱ��Ӧ��Ϊϵͳʱ��
	if (newMdl.m_time.GetYear()<2005) {
		newMdl.m_time=CTime::GetCurrentTime();
	}	
	time=newMdl.m_time.Format("%Y-%A-%d %H:%M:%S");
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CAddMdlSeriesDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ڲ鿴��Ϣ
	if (newMdl.mdlNum==-1) {
        OnOK();
		return;
	}
	//�����¼�
	UpdateData(TRUE);
	//���
	if(newMdl.Name=="")
	{
		AfxMessageBox("���������ƣ�");
		return;
	}
	if (newMdl.Code.FindOneOf("(������)")!=-1) {
		AfxMessageBox("�����ʽ���ԣ�");
		return;
	}
	for(int i=0;i<(int)p_InsInfoList->GetCount();i++)
	{
		if (newMdl.Name==p_InsInfoList->GetAt(i).Name) {
			AfxMessageBox("ϵ�����Ѿ�����ͬ�����֣�����ģ�");
            return;
		}
		if (newMdl.Code==p_InsInfoList->GetAt(i).Code) {
			AfxMessageBox("ϵ�����Ѿ�����ͬ�ı��룬����ģ�");
			return;
		}
	}
	OnOK();
}

void CAddMdlSeriesDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	

	OnCancel();
}
