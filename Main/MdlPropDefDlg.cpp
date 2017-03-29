// MdlPropDefDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "MdlPropDefDlg.h"
#include "SelectMainPicDlg.h"
#include "UserNameDlg.h"
#include "MaterialDlg.h"

// MdlPropDefDlg �Ի���

IMPLEMENT_DYNAMIC(MdlPropDefDlg, CDialog)
MdlPropDefDlg::MdlPropDefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MdlPropDefDlg::IDD, pParent)
	, m_Name(_T(""))
	, m_Code(_T(""))
	, m_TypeName(_T(""))
	, m_Person(_T(""))
	, m_Material(_T(""))
	, m_PicPath(_T(""))
	, m_SolidPath(_T(""))
	, m_DrawingPath(_T(""))
	, m_Note(_T(""))
{
}

MdlPropDefDlg::~MdlPropDefDlg()
{
}

void MdlPropDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_EDIT4, m_Code);
	DDX_Text(pDX, IDC_EDIT2, m_TypeName);
	DDX_Text(pDX, IDC_EDIT3, m_Person);
	DDX_Text(pDX, IDC_EDIT_MATERIAL, m_Material);
	DDX_Text(pDX, IDC_EDIT9, m_PicPath);
	DDX_Text(pDX, IDC_EDIT10, m_SolidPath);
	DDX_Text(pDX, IDC_EDIT_DRAW, m_DrawingPath);
	DDX_Text(pDX, IDC_EDIT8, m_Note);
}


BEGIN_MESSAGE_MAP(MdlPropDefDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON11, OnBnClickedButton11)
END_MESSAGE_MAP()


// MdlPropDefDlg ��Ϣ�������
/************************************************************************/
/* ���ü�ͼ                                                             */
/************************************************************************/
void MdlPropDefDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CSelectMainPicDlg dlg;
	dlg.SetMdl(mdl);
	if(dlg.DoModal()==IDOK)
	{
		//����ѡ��ͼƬ��ȫ·����
		m_PicPath=dlg.select_file;
		UpdateData(FALSE);
	}
}
/************************************************************************/
/* ���ȷ��֮��Ķ���                                                   */
/************************************************************************/
void MdlPropDefDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	UsrPartPorp * temp;
	if(m_Name=="") 
	{
		AfxMessageBox("���Ʋ���Ϊ�գ�");
		return;
	}
    temp=SetMdlInfo(mdl);
	temp->Name=m_Name;
	temp->Note=m_Note;
	temp->pic_path=m_PicPath;
	temp->Person=m_Person;
	temp->Material=m_Material;

	CDialog::OnOK();
}

void MdlPropDefDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL MdlPropDefDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ProSolid mdl;
	//��õ�ǰģ��
	int   w_id=0;
	if (ProMdlCurrentGet((ProMdl*)&mdl)==PRO_TK_NO_ERROR)
	{
		InitInfo(mdl);
	}
	else if (ProWindowCurrentGet(&w_id)==PRO_TK_NO_ERROR)
	{
		if (ProWindowMdlGet(w_id,&mdl)==PRO_TK_NO_ERROR)
		{
			InitInfo(mdl);
		}
		else
		{
			AfxMessageBox("��ǰģ��û������!");
			OnCancel();
			return FALSE;
		}
	}
	else 
	{
		AfxMessageBox("��ǰģ��û������!");
		OnCancel();
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ��ʼ���Ի����ϵ�����                                                 */
/************************************************************************/
int MdlPropDefDlg::InitInfo(ProMdl solid)
{
	UsrPartPorp temp=GetMdlInfo(solid);
	m_Name=temp.Name;
	m_Code=temp.Code;
	m_TypeName=temp.TypeName;
	m_SolidPath=GetMdlFullFileName(solid);
	if(temp.bHaveDraw) m_DrawingPath=GetMdlFullFileName(temp.hDraw);
	m_Note=temp.Note;
	m_PicPath=temp.pic_path;
	m_Person=temp.Person;
	m_Material=temp.Material;
	UpdateData(FALSE);
	return 0;
}

void MdlPropDefDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}

void MdlPropDefDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: �ڴ������Ϣ����������
	UsrRepaintWindow();
}

void MdlPropDefDlg::OnBnClickedButton4()
{
	UpdateData(TRUE);
	CUserNameDlg dlg;
	dlg.m_UserName=m_Person;
	if(dlg.DoModal()==IDOK)
	{
		m_Person=dlg.m_UserName;
		UpdateData(FALSE);
	}
}

void MdlPropDefDlg::OnBnClickedButton11()
{
	UpdateData(TRUE);
	CMaterialDlg dlg;
	dlg.m_ID=m_Material;
	if(dlg.DoModal()==IDOK)
	{
		m_Material=dlg.m_ID;
		UpdateData(FALSE);
	}
}
