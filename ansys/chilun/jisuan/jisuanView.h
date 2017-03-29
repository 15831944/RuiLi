// jisuanView.h : CjisuanView ��Ľӿ�
//
#include "Fr.h"

#pragma once

class CjisuanSet;

class CjisuanView : public CRecordView
{
protected: // �������л�����
	CjisuanView();
	DECLARE_DYNCREATE(CjisuanView)

public:
	enum{ IDD = IDD_JISUAN_FORM };
	CFr* m_pSet;

// ����
public:
	CjisuanDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual CRecordset* OnGetRecordset();
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CjisuanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // jisuanView.cpp �ĵ��԰汾
inline CjisuanDoc* CjisuanView::GetDocument() const
   { return reinterpret_cast<CjisuanDoc*>(m_pDocument); }
#endif

