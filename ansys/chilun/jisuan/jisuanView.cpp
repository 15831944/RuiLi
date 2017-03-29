// jisuanView.cpp : CjisuanView 类的实现
//

#include "stdafx.h"
#include "jisuan.h"

#include "jisuanSet.h"
#include "jisuanDoc.h"
#include "jisuanView.h"
#include "Data.h"
#include "ListDlg.h"
#include "ListDlg1.h"
#include "fpt.h."
#include"ffa.h"
#include"fha.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CjisuanView

IMPLEMENT_DYNCREATE(CjisuanView, CRecordView)

BEGIN_MESSAGE_MAP(CjisuanView, CRecordView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRecordView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
//	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_REQUERY, OnBnClickedRequery)
	ON_BN_CLICKED(IDC_REQUERY1, OnBnClickedRequery1)
	ON_BN_CLICKED(IDC_REQUERY2, OnBnClickedRequery2)
	ON_BN_CLICKED(IDC_REQUERY3, OnBnClickedRequery3)
	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)
	ON_BN_CLICKED(IDC_EDIT1, OnBnClickedEdit1)
	ON_BN_CLICKED(IDC_EDIT2, OnBnClickedEdit2)
	ON_BN_CLICKED(IDC_EDIT3, OnBnClickedEdit3)
END_MESSAGE_MAP()

// CjisuanView 构造/销毁

CjisuanView::CjisuanView()
	: CRecordView(CjisuanView::IDD)
	, m_d(0)
	, m_m(0)
	, m_j(0)
	, m_fr(0)
	, m_fpt(0)
	, m_ffa(0)
	, m_fha(0)
{
	m_pSet = NULL;
	// TODO: 在此处添加构造代码

}

CjisuanView::~CjisuanView()
{
}

void CjisuanView::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	// 可以在此处插入 DDX_Field* 函数以将控件“连接”到数据库字段，例如
	// DDX_FieldText(pDX, IDC_MYEDITBOX, m_pSet->m_szColumn1, m_pSet)
	// DDX_FieldCheck(pDX, IDC_MYCHECKBOX, m_pSet->m_bColumn2, m_pSet);
	// 有关更多信息，请参阅 MSDN 和 ODBC 示例
	DDX_Text(pDX, IDC_D, m_d);
	DDX_Text(pDX, IDC_M, m_m);
	DDX_Text(pDX, IDC_J, m_j);
	DDX_Text(pDX, IDC_Fr, m_fr);
	DDX_Text(pDX, IDC_EDIT6, m_fpt);
	DDX_Text(pDX, IDC_EDIT5, m_ffa);
	DDX_Text(pDX, IDC_EDIT7, m_fha);
}

BOOL CjisuanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CRecordView::PreCreateWindow(cs);
}

void CjisuanView::OnInitialUpdate()
{
	m_pSet = &GetDocument()->m_jisuanSet;
	CRecordView::OnInitialUpdate();
}


// CjisuanView 打印

BOOL CjisuanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CjisuanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CjisuanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CjisuanView 诊断

#ifdef _DEBUG
void CjisuanView::AssertValid() const
{
	CRecordView::AssertValid();
}

void CjisuanView::Dump(CDumpContext& dc) const
{
	CRecordView::Dump(dc);
}

CjisuanDoc* CjisuanView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CjisuanDoc)));
	return (CjisuanDoc*)m_pDocument;
}
#endif //_DEBUG


// CjisuanView 数据库支持
CRecordset* CjisuanView::OnGetRecordset()
{
	return m_pSet;
}



// CjisuanView 消息处理程序

void CjisuanView::OnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CData dlg;
	dlg.Set(m_pSet->m_pDatabase);
	dlg.DoModal();
	//if(!m_pSet->IsOpen())
	//	return;
	//if(!m_pSet->CanAppend())
	//	return;

	//m_pSet->AddNew();
	//UpdateData(TRUE);
	//if(m_pSet->CanUpdate())
	//	m_pSet->Update();
	//m_pSet->Requery();
	//UpdateData(FALSE);
}

void CjisuanView::OnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	if(MessageBox("真的要删除此记录吗?","删除记录?",
		MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		m_pSet->Delete();
		m_pSet->MovePrev();

		if(m_pSet->IsBOF())
			m_pSet->MoveLast();
		if(m_pSet->IsEOF())
			m_pSet->SetFieldNull(NULL);

		UpdateData(FALSE);
	}
}

void CjisuanView::OnModify()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_pSet->Edit();

	//if(m_pSet->CanUpdate())
	//{
	//	m_pSet->CanUpdate();
	//}
	CListDlg dlg;
	dlg.SetDB(m_pSet->m_pDatabase);
	dlg.DoModal();
}

void CjisuanView::OnBnClickedRequery()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString temp;
	int     index=m_j+4;
	/*CString dengji;*/
    m_pSet->m_strFilter.Format("d1<=%f AND d2>%f AND m1<=%f AND m2>%f",m_d,m_d,m_m,m_m);
	/*dengji.Format("i%d",m_j);*/
	if(!m_pSet->IsOpen()) m_pSet->Open();
	if (m_pSet->IsBOF()) {
		AfxMessageBox("没有记录");
		m_pSet->Close();
		return;
	}
	m_pSet->MoveFirst();
	//m_pSet->GetFieldIndexByName(dengji);
	m_pSet->GetFieldValue(index,temp);
	m_fr=atof(temp.GetBuffer());
	UpdateData(FALSE);
	m_pSet->Close();
}

void CjisuanView::OnBnClickedRequery1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString temp;
	int     index=m_j+4;
	Cfpt m_Set(m_pSet->m_pDatabase);
	/*CString dengji;*/
	m_Set.m_strFilter.Format("d1<=%f AND d2>%f AND m1<=%f AND m2>%f",m_d,m_d,m_m,m_m);
	/*dengji.Format("i%d",m_j);*/
	m_Set.Open();
	if (m_Set.IsBOF()) {
		AfxMessageBox("没有记录");
		return;
	}
	m_Set.MoveFirst();
	//m_pSet->GetFieldIndexByName(dengji);
	m_Set.GetFieldValue(index,temp);
	m_fpt=atof(temp.GetBuffer());
	UpdateData(FALSE);
	m_Set.Close();
}

void CjisuanView::OnBnClickedRequery2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString temp;
	int     index=m_j+4;
	Cffa m_fSet(m_pSet->m_pDatabase);
	/*CString dengji;*/
	m_fSet.m_strFilter.Format("d1<=%f AND d2>%f AND m1<=%f AND m2>%f",m_d,m_d,m_m,m_m);
	/*dengji.Format("i%d",m_j);*/
	m_fSet.Open();
	if (m_fSet.IsBOF()) {
		AfxMessageBox("没有记录");
		return;
	}
	m_fSet.MoveFirst();
	//m_pSet->GetFieldIndexByName(dengji);
	m_fSet.GetFieldValue(index,temp);
	m_ffa=atof(temp.GetBuffer());
	UpdateData(FALSE);
	m_fSet.Close();
}

void CjisuanView::OnBnClickedRequery3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString temp;
	int     index=m_j+4;
	Cfha  m_hSet(m_pSet->m_pDatabase);
	/*CString dengji;*/
	m_hSet.m_strFilter.Format("d1<=%f AND d2>%f AND m1<=%f AND m2>%f",m_d,m_d,m_m,m_m);
	/*dengji.Format("i%d",m_j);*/
	m_hSet.Open();
	if (m_hSet.IsBOF()) {
		AfxMessageBox("没有记录");
		
		return;
	}
	m_hSet.MoveFirst();
	//m_pSet->GetFieldIndexByName(dengji);
	m_hSet.GetFieldValue(index,temp);
	m_fha=atof(temp.GetBuffer());
	UpdateData(FALSE);
	m_hSet.Close();
}

void CjisuanView::OnBnClickedEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	CListDlg dlg;
	dlg.SetDB(m_pSet->m_pDatabase);
	dlg.DoModal();
}

void CjisuanView::OnBnClickedEdit1()
{
    // TODO: 在此添加控件通知处理程序代码
	CListDlg1 dlg;
	dlg.SetDB(m_pSet->m_pDatabase);
	dlg.DoModal();
}

void CjisuanView::OnBnClickedEdit2()
{
	// TODO: 在此添加控件通知处理程序代码
	CListDlg dlg;
	dlg.SetDB(m_pSet->m_pDatabase);
	dlg.DoModal();
}

void CjisuanView::OnBnClickedEdit3()
{
	// TODO: 在此添加控件通知处理程序代码
	CListDlg dlg;
	dlg.SetDB(m_pSet->m_pDatabase);
	dlg.DoModal();
}
