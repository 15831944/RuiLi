// jisuanView.h : CjisuanView 类的接口
//
#include "Fr.h"

#pragma once

class CjisuanSet;

class CjisuanView : public CRecordView
{
protected: // 仅从序列化创建
	CjisuanView();
	DECLARE_DYNCREATE(CjisuanView)

public:
	enum{ IDD = IDD_JISUAN_FORM };
	CFr* m_pSet;

// 属性
public:
	CjisuanDoc* GetDocument() const;

// 操作
public:

// 重写
	public:
	virtual CRecordset* OnGetRecordset();
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CjisuanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	double m_d;
	double m_m;
	int    m_j;
	double m_fr;
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnModify();
	afx_msg void OnBnClickedRequery();
	double m_fpt;
	double m_ffa;
	double m_fha;
	afx_msg void OnBnClickedRequery1();
	afx_msg void OnBnClickedRequery2();
	afx_msg void OnBnClickedRequery3();
	afx_msg void OnBnClickedEdit();
	afx_msg void OnBnClickedEdit1();
	afx_msg void OnBnClickedEdit2();
	afx_msg void OnBnClickedEdit3();
};

#ifndef _DEBUG  // jisuanView.cpp 的调试版本
inline CjisuanDoc* CjisuanView::GetDocument() const
   { return reinterpret_cast<CjisuanDoc*>(m_pDocument); }
#endif

