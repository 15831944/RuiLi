// jisuanDoc.h :  CjisuanDoc 类的接口
//


#pragma once
#include "jisuanSet.h"
#include "Fr.h"
class CjisuanDoc : public CDocument
{
protected: // 仅从序列化创建
	CjisuanDoc();
	DECLARE_DYNCREATE(CjisuanDoc)

// 属性
public:
	CFr m_jisuanSet;

// 操作
public:

// 重写
	public:
	virtual BOOL OnNewDocument();

// 实现
public:
	virtual ~CjisuanDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void SetTitle(LPCTSTR lpszTitle);
};


