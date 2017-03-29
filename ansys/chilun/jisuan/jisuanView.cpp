// jisuanView.cpp : CjisuanView ���ʵ��
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
	// ��׼��ӡ����
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

// CjisuanView ����/����

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
	// TODO: �ڴ˴���ӹ������

}

CjisuanView::~CjisuanView()
{
}

void CjisuanView::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	// �����ڴ˴����� DDX_Field* �����Խ��ؼ������ӡ������ݿ��ֶΣ�����
	// DDX_FieldText(pDX, IDC_MYEDITBOX, m_pSet->m_szColumn1, m_pSet)
	// DDX_FieldCheck(pDX, IDC_MYCHECKBOX, m_pSet->m_bColumn2, m_pSet);
	// �йظ�����Ϣ������� MSDN �� ODBC ʾ��
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
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CRecordView::PreCreateWindow(cs);
}

void CjisuanView::OnInitialUpdate()
{
	m_pSet = &GetDocument()->m_jisuanSet;
	CRecordView::OnInitialUpdate();
}


// CjisuanView ��ӡ

BOOL CjisuanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CjisuanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CjisuanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CjisuanView ���

#ifdef _DEBUG
void CjisuanView::AssertValid() const
{
	CRecordView::AssertValid();
}

void CjisuanView::Dump(CDumpContext& dc) const
{
	CRecordView::Dump(dc);
}

CjisuanDoc* CjisuanView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CjisuanDoc)));
	return (CjisuanDoc*)m_pDocument;
}
#endif //_DEBUG


// CjisuanView ���ݿ�֧��
CRecordset* CjisuanView::OnGetRecordset()
{
	return m_pSet;
}



// CjisuanView ��Ϣ�������

void CjisuanView::OnAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(MessageBox("���Ҫɾ���˼�¼��?","ɾ����¼?",
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString temp;
	int     index=m_j+4;
	/*CString dengji;*/
    m_pSet->m_strFilter.Format("d1<=%f AND d2>%f AND m1<=%f AND m2>%f",m_d,m_d,m_m,m_m);
	/*dengji.Format("i%d",m_j);*/
	if(!m_pSet->IsOpen()) m_pSet->Open();
	if (m_pSet->IsBOF()) {
		AfxMessageBox("û�м�¼");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString temp;
	int     index=m_j+4;
	Cfpt m_Set(m_pSet->m_pDatabase);
	/*CString dengji;*/
	m_Set.m_strFilter.Format("d1<=%f AND d2>%f AND m1<=%f AND m2>%f",m_d,m_d,m_m,m_m);
	/*dengji.Format("i%d",m_j);*/
	m_Set.Open();
	if (m_Set.IsBOF()) {
		AfxMessageBox("û�м�¼");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString temp;
	int     index=m_j+4;
	Cffa m_fSet(m_pSet->m_pDatabase);
	/*CString dengji;*/
	m_fSet.m_strFilter.Format("d1<=%f AND d2>%f AND m1<=%f AND m2>%f",m_d,m_d,m_m,m_m);
	/*dengji.Format("i%d",m_j);*/
	m_fSet.Open();
	if (m_fSet.IsBOF()) {
		AfxMessageBox("û�м�¼");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString temp;
	int     index=m_j+4;
	Cfha  m_hSet(m_pSet->m_pDatabase);
	/*CString dengji;*/
	m_hSet.m_strFilter.Format("d1<=%f AND d2>%f AND m1<=%f AND m2>%f",m_d,m_d,m_m,m_m);
	/*dengji.Format("i%d",m_j);*/
	m_hSet.Open();
	if (m_hSet.IsBOF()) {
		AfxMessageBox("û�м�¼");
		
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CListDlg dlg;
	dlg.SetDB(m_pSet->m_pDatabase);
	dlg.DoModal();
}

void CjisuanView::OnBnClickedEdit1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
	CListDlg1 dlg;
	dlg.SetDB(m_pSet->m_pDatabase);
	dlg.DoModal();
}

void CjisuanView::OnBnClickedEdit2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CListDlg dlg;
	dlg.SetDB(m_pSet->m_pDatabase);
	dlg.DoModal();
}

void CjisuanView::OnBnClickedEdit3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CListDlg dlg;
	dlg.SetDB(m_pSet->m_pDatabase);
	dlg.DoModal();
}
