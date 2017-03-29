// AsmMdlManDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ASM.h"
#include "AsmMdlManDlg.h"


// CAsmMdlManDlg �Ի���

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


// CAsmMdlManDlg ��Ϣ�������

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
	//��������ҳ�Ĵ�С��λ��
	m_Tree.GetWindowRect(&rect2);	
	m_sheet.SetWindowPos(NULL, rect2.Width()+6, 5,0,0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

}

BOOL CAsmMdlManDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ShowTree();
     SetSheetPage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ��ʾģ����                                                           */
/************************************************************************/
void CAsmMdlManDlg::ShowTree(void)
{
	m_Tree.DeleteAllItems();
	HTREEITEM hItemRoot=m_Tree.InsertItem(_T("װ��ģ��"));
	if(m_pSet.IsOpen()) m_pSet.Close();
	m_pSet.m_strFilter="";
	if(!m_pSet.Open())
	{
		AfxMessageBox("���ݿ����Ӳ��ɹ���");
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
/* ���ģ�͵��¼�                                                       */
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
