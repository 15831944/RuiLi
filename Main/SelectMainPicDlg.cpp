// SelectMainPicDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "SelectMainPicDlg.h"
#include "PicViewDlg.h"
//extern CArray<UsrPicture,UsrPicture>   p_UsrPicture; 
//extern int DlgIcon;
// CSelectMainPicDlg 对话框

IMPLEMENT_DYNAMIC(CSelectMainPicDlg, CDialog)
CSelectMainPicDlg::CSelectMainPicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectMainPicDlg::IDD, pParent)
	,mdl(NULL)
	, select_file(_T(""))
{
}

CSelectMainPicDlg::~CSelectMainPicDlg()
{
}

void CSelectMainPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CSelectMainPicDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CSelectMainPicDlg 消息处理程序

void CSelectMainPicDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int num=0;
	CString file;
	//使所有ISMAIN=0
    num=(int)p_UsrPicture.GetCount();
	for(int j=0;j<num;j++)
	{
		if(p_UsrPicture[j].owner!=mdl) continue;  //当前模型
		if(p_UsrPicture[j].isCheck!=1) continue;            //没有被删除
	    p_UsrPicture[j].isMain=0;
		break;
	}
	num=(int)m_List.GetItemCount();

	for(int i=0;i<num;i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;

		int num1=0;
		num1=(int)p_UsrPicture.GetCount();
		for(int j=0;j<num1;j++)
		{
			if(p_UsrPicture[j].owner!=mdl) continue;  //当前模型
			if(p_UsrPicture[j].isCheck!=1) continue;            //没有被删除
			if(p_UsrPicture[j].fileName!=m_List.GetItemText(i,0)) continue;
			file=p_UsrPicture[j].filePath+p_UsrPicture[j].fileName;
			p_UsrPicture[j].isMain=1;
			break;
		}
	}
    select_file=file;
	CSelectMainPicDlg::OnOK();
}

void CSelectMainPicDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	select_file="";
	CSelectMainPicDlg::OnCancel();
}
/************************************************************************/
/* 预览                                                                 */
/************************************************************************/
void CSelectMainPicDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CPicViewDlg dlg;
	int num=0;
	CString file;
	num=(int)m_List.GetItemCount();

	for(int i=0;i<num;i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;

		int num1=0;
		num1=(int)p_UsrPicture.GetCount();
		for(int j=0;j<num1;j++)
		{
			if(p_UsrPicture[j].owner!=mdl) continue;  //当前模型
			if(p_UsrPicture[j].isCheck!=1) continue;            //没有被删除
			if(p_UsrPicture[j].fileName!=m_List.GetItemText(i,0)) continue;
			file=p_UsrPicture[j].filePath+p_UsrPicture[j].fileName;
			dlg.SetFile(file);
			dlg.DoModal();
			break;
		}
	}


}

BOOL CSelectMainPicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()

    if(mdl==NULL) return FALSE;

	m_List.DeleteAllItems();
	for(int i=0;i<2;i++)
	{
		m_List.DeleteColumn(0);
	}
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"文件名",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(1,"说明",LVCFMT_LEFT,100,-1);
	int num=(int)p_UsrPicture.GetCount();
	int indexNo=0;
	int temp=0;
	for(int i=0;i<num;i++)
	{
		if(p_UsrPicture[i].owner!=mdl) continue;
		if(p_UsrPicture[i].isCheck!=1) continue;

		indexNo=m_List.InsertItem(temp++,p_UsrPicture[i].fileName);
		m_List.SetItemText(indexNo,1,p_UsrPicture[i].Note);
	}
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 设置模型，在初始化之前完成                                           */
/************************************************************************/
int CSelectMainPicDlg::SetMdl(ProMdl solid)
{
	
	mdl=solid;
	return 0;
}
