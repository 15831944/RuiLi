// NewDlg.cpp : ʵ���ļ�
//�½��Ի���

#include "stdafx.h"
#include "Main.h"
#include "NewDlg.h"
#include <ProUtil.h> 

IMPLEMENT_DYNAMIC(CNewDlg, CDialog)
CNewDlg::CNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewDlg::IDD, pParent)
	, m_Name(_T(""))
	, m_Template(0)
	, m_radioFlag(0)
	, m_FileName(_T(""))
{
}

CNewDlg::~CNewDlg()
{
}

void CNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_CBIndex(pDX, IDC_COMBO1, m_Template);
	DDX_Control(pDX, IDC_RADIO4, m_RadioDrawing);
	DDX_Text(pDX, IDC_EDIT2, m_FileName);
}


BEGIN_MESSAGE_MAP(CNewDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CNewDlg ��Ϣ�������

void CNewDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
    
	if (m_FileName=="") {
		AfxMessageBox("���������ƣ�");
		return;
	}
	if (m_radioFlag==1) {
		if(!mynewfileprt(m_FileName.GetBuffer()+'\0')) return;
	}
	if (m_radioFlag==3) {
		if (!mynewfileasm(m_FileName.GetBuffer()+'\0')) return;
	}
	if (m_radioFlag==4) {
		if (!mynewfiledwg(m_FileName.GetBuffer()+'\0')) return;
	}
	OnOK();
}

void CNewDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}
/************************************************************************/
/* �½����                                                             */
/************************************************************************/
bool CNewDlg::mynewfileprt(char  filename[])
{
	ProError			err;
	ProLine				buff;
	ProMdl				model;
	int					w_id;

	ProStringToWstring(buff,filename);
	err = ProSolidCreate(buff, PRO_PART, (ProSolid *)&model);

	if (err!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�ļ��Ѵ��ڻ��ļ������Ϸ�!");
		return false;
	}

	if (err==PRO_TK_NO_ERROR) {
		ProMdlDisplay(model);
		ProMdlWindowGet(model,&w_id);
		ProWindowActivate(w_id);
	}
    SetMdlInfo(model)->Name=m_Name;
	return true;
}
BOOL CNewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_RadioDrawing.SetCheck(BST_CHECKED);
    m_radioFlag=4;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ����齨                                                             */
/************************************************************************/
void CNewDlg::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_radioFlag=3;
}

void CNewDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_radioFlag=1;
}

void CNewDlg::OnBnClickedRadio4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_radioFlag=4;
	//AfxMessageBox("4");
}

bool CNewDlg::mynewfileasm(char  filename[])
{
	ProError		err;
	ProLine			buff;
	ProMdl			model;
	int				w_id;


	ProStringToWstring(buff,filename);
	err = ProSolidCreate(buff,PRO_ASSEMBLY,(ProSolid *)&model);

	if (err!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("�ļ��Ѵ��ڻ��ļ������Ϸ�!");
		return false;
	}

	if (err==PRO_TK_NO_ERROR) {
		ProMdlDisplay(model);
		ProMdlWindowGet(model,&w_id);
		ProWindowActivate(w_id);
	}
	SetMdlInfo(model)->Name=m_Name;
	return true;
}

bool CNewDlg::mynewfiledwg(char  filename[])
{
	ProMdl				solid_mdl;
	ProMdldata			data;
	ProModel			model;
	ProName				dwg_template,name;
	ProDrawing			drawing;
	ProError			status;
	char				strtmp[PRO_PATH_SIZE];
    ProDwgcreateOptions option=PRODWGCREATE_DISPLAY_DRAWING|PRODWGCRTERR_TEMPLATE_USED ;
    ProDwgcreateErrs	errors;
	
	status=ProMdlCurrentGet(&solid_mdl);
    if (status!=PRO_TK_NO_ERROR) {
		AfxMessageBox("��ǰû��ģ�ͣ����һ��ģ��֮�������ɹ���ͼ��");
		return false;
    }
	status=ProMdlDataGet(solid_mdl,&data);
	ProWstringToString(strtmp,data.name);
	ProStringToWstring(model.name,strtmp);
	ProWstringToString(strtmp,data.type);
	ProStringToWstring(model.type,strtmp);
	//���û�ͼģ����
	ProStringToWstring(dwg_template,"c_drawing");
	ProStringToWstring(name,filename);
	ProDrawingFromTmpltCreate(name,dwg_template,&model,option,&drawing,&errors);
	return true;
}

void CNewDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: �ڴ������Ϣ����������
	UsrRepaintWindow();
}
