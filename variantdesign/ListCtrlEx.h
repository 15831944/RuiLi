#if !defined(AFX_LISTCTRLEX_H__E29BCEF5_3FCB_4A2E_8EFF_4C6DCCD6485C__INCLUDED_)
#define AFX_LISTCTRLEX_H__E29BCEF5_3FCB_4A2E_8EFF_4C6DCCD6485C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "variantdesign.h"
#define MAX_LISTCTRL_COLUMNS 100
// ListCtrlEx.h : header file
//
/////////////////////////////////////////
//该类改造于Lee Nowotny的gxListCtrl类
//详细见例子http://www.codetools.com/listctrl/listeditor.asp
/////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window

class CListCtrlEx : public CListCtrl
{
// Construction
public:
	CListCtrlEx();

// Attributes
public:

// Operations
public:
    int	    HitTestEx (CPoint& Point, int& nSubItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	CEdit* EditSubItem(int nItem, int nSubItem);
	virtual ~CListCtrlEx();
	CEdit *pEdit;
	CList<int, int> m_ReadOnlyColumnsList;
	CString Text;
	CString m_strValidChars[MAX_LISTCTRL_COLUMNS];
	void SetReadOnlyColumns(int iColumnIndex, bool bSet /*= true*/);
	void SetColumnValidEditCtrlCharacters(CString &rstrValidCharacters,int column);
	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnMovNex(WPARAM wparam,LPARAM lparam);
	void DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect);
	BOOL CListCtrlEx::IsReadOnly(int nSubItem);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__E29BCEF5_3FCB_4A2E_8EFF_4C6DCCD6485C__INCLUDED_)
