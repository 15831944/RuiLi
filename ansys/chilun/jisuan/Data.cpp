// Data.cpp : 实现文件
//

#include "stdafx.h"
#include "jisuan.h"
#include "Data.h"


// CData 对话框

IMPLEMENT_DYNAMIC(CData, CDialog)
CData::CData(CWnd* pParent /*=NULL*/)
	: CDialog(CData::IDD, pParent)
{
}

CData::~CData()
{
}

void CData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
}


BEGIN_MESSAGE_MAP(CData, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CData 消息处理程序

void CData::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

BOOL CData::OnInitDialog()
{
	CDialog::OnInitDialog();
    xiansh();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
void CData::xiansh()
{
	CRect rect;
	char lpstrRow[6]="1";
	int iRowCount=1;

	this->GetClientRect(rect);
	m_FlexGrid.MoveWindow(rect,TRUE);
	m_FlexGrid.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);
	
	m_FlexGrid.put_Cols(/*(m_pSet->m_nFields)+1*/18);
	m_FlexGrid.put_Rows(1);
	m_FlexGrid.put_ColWidth(-1,1000);
	//m_FlexGrid.SetColWidth(-1,1000);
    m_FlexGrid.put_RowHeight(-1,360);
	//m_FlexGrid.SetRowHeight(-1,360);
	m_FlexGrid.put_ColWidth(0,600);
	//m_FlexGrid.SetColWidth(0,600);
	m_FlexGrid.put_Row(0);
	//m_FlexGrid.SetRow(0);
	m_FlexGrid.put_Col(1);
	//m_FlexGrid.SetCol(1);
	m_FlexGrid.put_Text("d1");
	//m_FlexGrid.SetText("d1");
	m_FlexGrid.put_Col(2);
	//m_FlexGrid.SetCol(1);
	m_FlexGrid.put_Text("d2");
	//m_FlexGrid.SetCol(2);
	//m_FlexGrid.SetText("d2");

	m_FlexGrid.put_Col(3);
	m_FlexGrid.put_Text("m1");
	//m_FlexGrid.SetCol(3);
	//m_FlexGrid.SetText("m1");
	m_FlexGrid.put_Col(4);
	m_FlexGrid.put_Text("m2");
	//m_FlexGrid.SetCol(4);
	//m_FlexGrid.SetText("m2");
	m_FlexGrid.put_Col(5);
	m_FlexGrid.put_Text("0级");
	//m_FlexGrid.SetCol(5);
	//m_FlexGrid.SetText("i0");
	for(int i=6;i<=17;i++)
	{
		CString temp;
		temp.Format("%d级",i-5);
		m_FlexGrid.put_Col(i);
		m_FlexGrid.put_Text(temp);
	}
	//m_FlexGrid.SetCol(6);
	//m_FlexGrid.SetText("i1");
	//m_FlexGrid.SetCol(7);
	//m_FlexGrid.SetText("i2");
	//m_FlexGrid.SetCol(8);
	//m_FlexGrid.SetText("i3");
	//m_FlexGrid.SetCol(9);
	//m_FlexGrid.SetText("i4");
	//m_FlexGrid.SetCol(10);
	//m_FlexGrid.SetText("i5");
	//m_FlexGrid.SetCol(11);
	//m_FlexGrid.SetText("i6");
	//m_FlexGrid.SetCol(12);
	//m_FlexGrid.SetText("i7");
	//m_FlexGrid.SetCol(13);
	//m_FlexGrid.SetText("i8");
	//m_FlexGrid.SetCol(14);
	//m_FlexGrid.SetText("i9");
	//m_FlexGrid.SetCol(15);
	//m_FlexGrid.SetText("i10");
	//m_FlexGrid.SetCol(16);
	//m_FlexGrid.SetText("i11");
	//m_FlexGrid.SetCol(17);
	//m_FlexGrid.SetText("i12");
	m_pSet.m_strFilter.Empty();
    m_pSet.Requery();
	m_pSet.MoveFirst();
	do {
		int Row=m_FlexGrid.get_Rows()+1;
		CString temp;
		m_FlexGrid.put_Rows(Row);
		m_FlexGrid.put_Row(Row-1);
		for(int i=1;i<18;i++)
		{
			m_pSet.GetFieldValue(i-1,temp);
			m_FlexGrid.put_Col(i);
			m_FlexGrid.put_Text(temp);
		}
		m_pSet.MoveNext();
	} while(!m_pSet.IsEOF());
	//while(!m_pSet->IsEOF())
	//{
		//m_FlexGrid.put_Row(iRowCount);
  //      m_FlexGrid.put_Col(0);
		//itoa(iRowCount,(char*)lpstrRow,17);
		//m_FlexGrid.put_Text(lpstrRow);

		//CString m_d1;
		//	m_d1.Format("%d",m_pSet->m_d1);
		//m_FlexGrid.put_Col(1);
  //      m_FlexGrid.put_Text(m_pSet->m_d1);

		//CString m_d2;
		//m_d2.Format("%d",m_pSet->m_d2);
		//m_FlexGrid.put_Col(2);
		//m_FlexGrid.put_Text(m_pSet->m_d2);

		//CString m_m1;
		//m_m1.Format("%d",m_pSet->m_m1);
		//m_FlexGrid.put_Col(1);
		//m_FlexGrid.put_Text(m_pSet->m_m1);

		//CString m_m2;
		//m_m2.Format("%d",m_pSet->m_m2);
		//m_FlexGrid.put_Col(1);
		//m_FlexGrid.put_Text(m_pSet->m_m2);

		//CString m_i0;
		//m_d1.Format("%d",m_pSet->m_i0);
		//m_FlexGrid.put_Col(1);
		//m_FlexGrid.put_Text(m_pSet->m_d);

		//CString m_d1;
		//m_d1.Format("%d",m_pSet->m_d1);
		//m_FlexGrid.put_Col(1);
		//m_FlexGrid.put_Text(m_pSet->m_d);


		//m_FlexGrid.put_Col(1);
		//m_FlexGrid.put_Text(ID);

		//CString m_m
		//	ID.Format("%d",m_pSet->m_ID);
		//m_FlexGrid.put_Col(1);
		//m_FlexGrid.put_Text(ID);

		//m_FlexGrid.put_Col(1);
		//m_FlexGrid.put_Text(ID);


		//CString ID
		//	ID.Format("%d",m_pSet->m_ID);
		//m_FlexGrid.put_Col(1);
		//m_FlexGrid.put_Text(ID);

		//if(!m_pSet->IsEOF())
		//{
		//	m_pSet->MoveNext();
		//	iRowCount +=1;
		//}


	//}
	//m_pSet->MoveFirst();/*
	 m_FlexGrid.put_Row(1);
	 m_FlexGrid.put_Col(1);
	 m_FlexGrid.put_Enabled(TRUE);
}
void CData::Set(CDatabase * p_db)
{
	m_pSet.m_pDatabase=p_db;
	m_pSet.Open();
}