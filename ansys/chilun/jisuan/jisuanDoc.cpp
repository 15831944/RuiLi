// jisuanDoc.cpp :  CjisuanDoc ���ʵ��
//

#include "stdafx.h"
#include "jisuan.h"

#include "jisuanSet.h"
#include "jisuanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CjisuanDoc

IMPLEMENT_DYNCREATE(CjisuanDoc, CDocument)

BEGIN_MESSAGE_MAP(CjisuanDoc, CDocument)
END_MESSAGE_MAP()


// CjisuanDoc ����/����

CjisuanDoc::CjisuanDoc()
{
	// TODO���ڴ����һ���Թ������
}

CjisuanDoc::~CjisuanDoc()
{
}

BOOL CjisuanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO���ڴ�������³�ʼ������
	// ��SDI �ĵ������ø��ĵ���
   
	return TRUE;
}




// CjisuanDoc ���

#ifdef _DEBUG
void CjisuanDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CjisuanDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CjisuanDoc ����

void CjisuanDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: �ڴ����ר�ô����/����û���

	CDocument::SetTitle("�������ȵȼ�");
}
