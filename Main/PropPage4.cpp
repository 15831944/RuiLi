// PropPage4.cpp : 实现文件
//属性信息编辑页

#include "stdafx.h"
#include "Main.h"
#include "PropPage4.h"

//extern CArray<UsrDrawingItem,UsrDrawingItem> P_UsrDrawingList;
//void GetDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList,CArray<int,int> * p_IndexList);
// CPropPage4 对话框

IMPLEMENT_DYNAMIC(CPropPage4, CPropertyPage)
CPropPage4::CPropPage4()
: CPropertyPage(CPropPage4::IDD)
, m_MdlName(_T(""))
, m_MdlCode(_T(""))
, m_MdlType(_T(""))
, m_MdlFileName(_T(""))
, m_MdlCreater(_T(""))
, m_MdlNote(_T(""))
, SolidPorp(NULL)
, mdl(NULL)
, m_Material(_T(""))
{
}

CPropPage4::~CPropPage4()
{
}

void CPropPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_MdlName);
	DDX_Text(pDX, IDC_EDIT4, m_MdlCode);
	DDX_Text(pDX, IDC_EDIT2, m_MdlType);
	DDX_Text(pDX, IDC_EDIT5, m_MdlFileName);
	DDX_Text(pDX, IDC_EDIT6, m_MdlCreater);
	DDX_Text(pDX, IDC_EDIT8, m_MdlNote);
	DDX_Control(pDX, IDC_MYIMAGE2, m_pic);
	DDX_Control(pDX, IDC_EDIT2, m_TypeEdit);
	DDX_Control(pDX, IDC_EDIT4, m_CodeEdit);
	DDX_Text(pDX, IDC_EDIT9, m_Material);
	DDX_Control(pDX, IDC_EDIT3, m_DrawFileName);
}


BEGIN_MESSAGE_MAP(CPropPage4, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPropPage4 消息处理程序

BOOL CPropPage4::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ProMdlCurrentGet(&mdl);
	SolidPorp=GetMdlPInfo(mdl);
	m_MdlName=SolidPorp->Name;
	m_MdlCode=SolidPorp->Code;
	m_MdlType=SolidPorp->TypeName;
	m_MdlFileName=GetMdlFileName(mdl);
	m_MdlCreater=SolidPorp->Person;
	m_Material=SolidPorp->Material;
	if(SolidPorp->bHaveDraw) m_DrawFileName.SetWindowText(GetMdlFileName(SolidPorp->hDraw));
	m_MdlNote=SolidPorp->Note;
	m_pic.SetFile(SolidPorp->pic_path);  //用问题
	m_pic.OnPaint();

    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

//int CPropPage4::SetValue(ProMdl solid,CArray<UsrPartPorp,UsrPartPorp> * pp_temp_UsrPartPorpList)
//{
//	//pp_temp_UsrPartPorp=new UsrPartPorp; 
//	//mdl=solid;
//	//p_temp_UsrPartPorpeList=pp_temp_UsrPartPorpList;
//	return 0;
//}
/************************************************************************/
/* 点击修改之后的动作                                                   */
/************************************************************************/
void CPropPage4::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_MdlName=="") 
	{
		AfxMessageBox("名称不能为空！");
		return;
	}
	else
	{
		SolidPorp->Name=m_MdlName;
		SolidPorp->Note=m_MdlNote;
		SolidPorp->Material=m_Material;
		SolidPorp->Code=m_MdlCode;
		SolidPorp->Person=m_MdlCreater;
	}
}

void CPropPage4::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    
	CPropertyPage::OnClose();
}
