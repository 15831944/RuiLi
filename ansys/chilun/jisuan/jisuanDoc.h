// jisuanDoc.h :  CjisuanDoc ��Ľӿ�
//


#pragma once
#include "jisuanSet.h"
#include "Fr.h"
class CjisuanDoc : public CDocument
{
protected: // �������л�����
	CjisuanDoc();
	DECLARE_DYNCREATE(CjisuanDoc)

// ����
public:
	CFr m_jisuanSet;

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();

// ʵ��
public:
	virtual ~CjisuanDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void SetTitle(LPCTSTR lpszTitle);
};


