// ListDlg2.cpp : 实现文件
//

#include "stdafx.h"
#include "jisuan.h"
#include "ListDlg2.h"
#include "GridCtrl.h"

// CListDlg2 对话框

IMPLEMENT_DYNAMIC(CListDlg2, CDialog)
CListDlg2::CListDlg2(CWnd* pParent /*=NULL*/)
	: CDialog(CListDlg2::IDD, pParent)
{
	IsAddMode=FALSE;
}

CListDlg2::~CListDlg2()
{
}

void CListDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
   DDX_GridControl(pDX, IDC_GRIDCTRL, m_GridCtrl);
	DDX_Control(pDX, IDC_ADD, m_AddButton);
	DDX_Control(pDX, IDC_SAVE, m_SaveButton);
	DDX_Control(pDX, IDC_DELETE, m_DeleteButton);
}


BEGIN_MESSAGE_MAP(CListDlg2, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_SAVE, OnBnClickedSave)
	ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
END_MESSAGE_MAP()


// CListDlg2 消息处理程序
void CListDlg2::InitGridCtrl()
{
	m_GridCtrl.SetRowCount(1);
	m_GridCtrl.SetFixedRowCount(1);
	m_GridCtrl.SetColumnCount(18);
	m_GridCtrl.SetFixedColumnCount(1);
	m_GridCtrl.SetRowResize(FALSE);
	

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 1;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"d1");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 2;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"d2");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 3;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"m1");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 4;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"m2");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 5;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i0");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 6;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i1");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 7;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i2");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 8;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i3");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 9;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i4");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 10;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i5");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 11;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i6");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 12;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i7");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 13;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i8");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 14;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i9");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 15;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i10");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 16;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i11");
	m_GridCtrl.SetItem(&Item);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.col = 17;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	Item.szText.Format(_T("%s"),"i12");
	m_GridCtrl.SetItem(&Item);

	m_fSet.m_strFilter.Empty();
	m_fSet.Requery();
	if (m_fSet.IsBOF()) {
		return;
	}
	m_fSet.MoveFirst();
	do {
		int Row=m_GridCtrl.GetRowCount()+1;
		CString		temp;
		m_GridCtrl.SetRowCount(Row);
		//m_FlexGrid.put_Row(Row-1);
		m_GridCtrl.SetItemData( Row-1,0,m_fSet.m_index);
		for(int i=1;i<18;i++)
		{
			m_fSet.GetFieldValue(i,temp);
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_PARAM;
			Item.row = Row-1;
			Item.col = i;
			Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
			Item.szText.Format(_T("%s"),temp);
			m_GridCtrl.SetItem(&Item);
			m_GridCtrl.SetItemState(Row-1,i,GVIS_READONLY);
		}
		m_fSet.MoveNext();
	} while(!m_fSet.IsEOF());
	m_GridCtrl.SetEditable(TRUE);
}
void CListDlg2::SetDB(CDatabase * p_db)
{
	m_fSet.m_pDatabase=p_db;
	m_fSet.Open();
}
BOOL CListDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitGridCtrl();
	m_SaveButton.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CListDlg2::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	if(IsAddMode) return;
	int Row=m_GridCtrl.GetRowCount()+1;
	m_GridCtrl.SetRowCount(Row);
	for(int i=1;i<m_GridCtrl.GetColumnCount();i++)
	{
		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = Row-1;
		Item.col = i;
		Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
		Item.szText.Format(_T("%s"),"");
		m_GridCtrl.SetItem(&Item);
		m_GridCtrl.SetItemState(Row-1,i,GVIS_MODIFIED);
	}
	m_SaveButton.EnableWindow(TRUE);
	m_AddButton.EnableWindow(FALSE);
	IsAddMode=TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CListDlg2::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!IsAddMode) return;
	int i=1;
	m_fSet.AddNew();
	m_fSet.m_d1=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_d2=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_m1=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_m2=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i0=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i1=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i2=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i3=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i4=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i5=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i6=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i7=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i8=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i9=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i10=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i11=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	m_fSet.m_i12=atof(m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++));
	//m_fSet.m_i=m_GridCtrl.GetItemText(m_GridCtrl.GetRowCount()-1,i++);
	m_fSet.Update();
	m_fSet.m_strFilter.Empty();
	m_fSet.Requery();
	m_fSet.MoveLast();
	m_GridCtrl.SetItemData(m_GridCtrl.GetRowCount()-1,0,m_fSet.m_index);
	for(i=1;i<18;i++)
	{
		m_GridCtrl.SetItemState(m_GridCtrl.GetRowCount()-1,i,GVIS_READONLY);
	}
	m_SaveButton.EnableWindow(FALSE);
	m_AddButton.EnableWindow(TRUE);
	IsAddMode=FALSE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CListDlg2::OnBnClickedDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_GridCtrl.DeleteRow(m_GridCtrl.GetRowCount()-1);
	//if(IsAddMode) return;
	////m_GridCtrl.getsele
	int Row=m_GridCtrl.GetRowCount()-1;
	try
	{
		for(int i=1;i<m_GridCtrl.GetRowCount();i++)
		{
			if ((m_GridCtrl.GetItemState(i,1)&GVIS_SELECTED)==GVIS_SELECTED) 
			{

				if(i==m_GridCtrl.GetRowCount()-1&&IsAddMode==TRUE)
				{

					if (!m_GridCtrl.DeleteRow(i)) {
						AfxMessageBox("删除失败");
					}
					m_SaveButton.EnableWindow(FALSE);
					m_AddButton.EnableWindow(TRUE);
					IsAddMode=FALSE;
					return;
				}

				////////////////////////////////////////////

				m_fSet.m_strFilter.Format("[index]=%ld",m_GridCtrl.GetItemData(i,0));
				m_fSet.Requery();
				if(!m_fSet.IsBOF()) m_fSet.Delete();
				m_fSet.Requery();
				if (!m_GridCtrl.DeleteRow(i)) {
					AfxMessageBox("删除失败");
				}
				m_GridCtrl.UpdateData(TRUE);
				i=i-1;
			}
		}
		//m_GridCtrl.SetRowCount(Row);
		//IsAddMode=TRUE;
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		delete e;
	}
	//for(int i=1;i<m_GridCtrl.GetColumnCount();i++)
	//{
	//	GV_ITEM Item;
	//	Item.mask = GVIF_TEXT|GVIF_FORMAT ;
	//	Item.row = Row-1;
	//	Item.col = i;
	//	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	//	Item.szText.Format(_T("%s"),"");
	//	m_GridCtrl.SetItem(&Item);
	//	m_GridCtrl.SetItemState(Row-1,i,GVIS_MODIFIED);
	//}


}

//void CListDlg2::OnBnClickedAdd()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}
//
//void CListDlg2::OnBnClickedSave()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}
//
//void CListDlg2::OnBnClickedDelete()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}
