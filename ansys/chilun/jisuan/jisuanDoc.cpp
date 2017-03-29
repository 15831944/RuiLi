// jisuanDoc.cpp :  CjisuanDoc 类的实现
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


// CjisuanDoc 构造/销毁

CjisuanDoc::CjisuanDoc()
{
	// TODO：在此添加一次性构造代码
}

CjisuanDoc::~CjisuanDoc()
{
}

BOOL CjisuanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO：在此添加重新初始化代码
	// （SDI 文档将重用该文档）
   
	return TRUE;
}




// CjisuanDoc 诊断

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


// CjisuanDoc 命令

void CjisuanDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: 在此添加专用代码和/或调用基类

	CDocument::SetTitle("其他精度等级");
}
