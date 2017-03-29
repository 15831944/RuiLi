// AsmInfoDefPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "AsmInfoDefPage.h"
//extern CArray<UsrDrawingItem,UsrDrawingItem> P_UsrDrawingList;
//void GetDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList,CArray<int,int> * p_IndexList);

// CAsmInfoDefPage �Ի���

IMPLEMENT_DYNAMIC(CAsmInfoDefPage, CPropertyPage)
CAsmInfoDefPage::CAsmInfoDefPage()
	: CPropertyPage(CAsmInfoDefPage::IDD)
	, m_MdlName(_T(""))
	, m_MdlCode(_T(""))
	, m_MdlType(_T(""))
	, m_MdlFileName(_T(""))
	, m_MdlCreater(_T(""))
	, m_MdlNote(_T(""))
	, mdl(NULL)
	, m_DrawFileName(_T(""))
{
}

CAsmInfoDefPage::~CAsmInfoDefPage()
{
}

void CAsmInfoDefPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, 1003, m_MdlName);
	DDX_Text(pDX, IDC_EDIT4, m_MdlCode);
	DDX_Text(pDX, 1014, m_MdlType);
	DDX_Text(pDX, IDC_EDIT5, m_MdlFileName);
	DDX_Text(pDX, IDC_EDIT6, m_MdlCreater);
	DDX_Text(pDX, IDC_EDIT8, m_MdlNote);
	DDX_Control(pDX, IDC_MYIMAGE2, m_pic);
	DDX_Text(pDX, IDC_EDIT1, m_DrawFileName);
}


BEGIN_MESSAGE_MAP(CAsmInfoDefPage, CPropertyPage)
	ON_BN_CLICKED(1004, OnBnClicked1004)
END_MESSAGE_MAP()


// CAsmInfoDefPage ��Ϣ�������

BOOL CAsmInfoDefPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ProMdlCurrentGet((ProMdl*)&mdl);
	UsrPartPorp * SolidPorp;
	SolidPorp=GetMdlPInfo(mdl);
	m_MdlName=SolidPorp->Name;
	m_MdlCode=SolidPorp->Code;
	m_MdlType=SolidPorp->TypeName;
	m_MdlFileName=GetMdlFileName(mdl);
	m_MdlCreater=SolidPorp->Person;
	if(SolidPorp->bHaveDraw)  m_DrawFileName=GetMdlFileName(SolidPorp->hDraw);
	m_MdlNote=SolidPorp->Note;
	m_pic.SetFile(SolidPorp->pic_path);  //������
	m_pic.OnPaint();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
void CAsmInfoDefPage::OnBnClicked1004()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	UsrPartPorp * SolidPorp;
	if(m_MdlName=="") 
	{
		AfxMessageBox("���Ʋ���Ϊ�գ�");
		return;
	}
	else
	{
		SolidPorp=GetMdlPInfo(mdl);
		SolidPorp->Name=m_MdlName;
		SolidPorp->Note=m_MdlNote;
		SolidPorp->Code=m_MdlCode;
		SolidPorp->Person=m_MdlCreater;
	}
}
