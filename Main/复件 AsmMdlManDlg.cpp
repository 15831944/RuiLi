// AsmMdlManDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ASM.h"
#include "AsmMdlManDlg.h"


// CAsmMdlManDlg 对话框

IMPLEMENT_DYNAMIC(CAsmMdlManDlg, CDialog)
CAsmMdlManDlg::CAsmMdlManDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmMdlManDlg::IDD, pParent)
{
}

CAsmMdlManDlg::~CAsmMdlManDlg()
{
}

void CAsmMdlManDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
}


BEGIN_MESSAGE_MAP(CAsmMdlManDlg, CDialog)
END_MESSAGE_MAP()


// CAsmMdlManDlg 消息处理程序

void CAsmMdlManDlg::SetSheetPage(void)
{
	m_sheet.AddPage(&m_page1_Info);
	m_sheet.AddPage(&m_page2_Tree);
	m_sheet.AddPage(&m_page3_Swtx);
	m_sheet.AddPage(&m_page4_Perf);
	m_sheet.AddPage(&m_page5_Rel);
	m_sheet.AddPage(&m_page6_Pic);
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE|WS_GROUP, WS_EX_CONTROLPARENT);
	RECT rect;
	CRect rect2;
	m_sheet.GetWindowRect(&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	//调整属性页的大小和位置
	m_Tree.GetWindowRect(&rect2);	
	m_sheet.SetWindowPos(NULL, rect2.Width()+6, 5,0,0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

}

BOOL CAsmMdlManDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowTree();
     SetSheetPage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 显示模型树                                                           */
/************************************************************************/
void CAsmMdlManDlg::ShowTree(void)
{
	m_Tree.DeleteAllItems();
	HTREEITEM hItemRoot=m_Tree.InsertItem(_T("装配模型"));
	if(m_pSet.IsOpen()) m_pSet.Close();
	m_pSet.m_strFilter="";
	if(!m_pSet.Open())
	{
		AfxMessageBox("数据库连接不成功！");
		return;
	}
	if(m_pSet.IsBOF())
	{
		m_pSet.Close();
		return;
	}
	m_pSet.MoveFirst();

	while (!m_pSet.IsEOF()) {
		if(m_pSet.m_FatherNumber==0)
		{

			HTREEITEM hItem=m_Tree.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T(m_pSet.m_Name), 
				0, 0, 0, 0, m_pSet.m_Number, hItemRoot, NULL);

			int num=m_pSet.m_Number;
			AddSubItem(hItem);
			m_pSet.MoveFirst();
			while (m_pSet.m_Number!=num) {
				m_pSet.MoveNext();
			}

		}

		m_pSet.MoveNext();
	}
	m_pSet.Close();
	m_Tree.Expand(hItemRoot,TVE_EXPAND);
}
/************************************************************************/
/* 添加模型的下级                                                       */
/************************************************************************/
void CAsmMdlManDlg::AddSubItem(HTREEITEM hItem)
{
	HTREEITEM tempItem;
	if(m_pSet.IsEOF()) return;
	m_pSet.MoveFirst();
	do {
		if(m_pSet.m_FatherNumber==(long)m_Tree.GetItemData(hItem))
		{

			int i=0,j=0;
			int nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			if(m_pSet.m_Type=="mdl") i=2,j=2;
			tempItem= m_Tree.InsertItem(nMarsk,_T(m_pSet.m_Name), 
				i, j, 0, 0, m_pSet.m_Number, hItem, NULL);
			int num=m_pSet.m_Number;
			AddSubItem(tempItem);
			m_pSet.MoveFirst();
			while (m_pSet.m_Number!=num) {
				m_pSet.MoveNext();
			}

		}

		m_pSet.MoveNext();
	} while(!m_pSet.IsEOF());
}
