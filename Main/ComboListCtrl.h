/*******************************************************************************
	Author						: Aravindan Premkumar
	Unregistered Copyright 2003	: Aravindan Premkumar
	All Rights Reserved
	
	This piece of code does not have any registered copyright and is free to be 
	used as necessary. The user is free to modify as per the requirements. As a
	fellow developer, all that I expect and request for is to be given the 
	credit for intially developing this reusable code by not removing my name as 
	the author.
*******************************************************************************/

#if !defined(AFX_COMBOLISTCTRL_H__9089600F_374F_4BFC_9482_DEAC0E7133E8__INCLUDED_)
#define AFX_COMBOLISTCTRL_H__9089600F_374F_4BFC_9482_DEAC0E7133E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//the max listCtrl columns
#define MAX_LISTCTRL_COLUMNS 100

#include <afxtempl.h>

class CInPlaceCombo;
class CInPlaceEdit;

// User define message 
// This message is posted to the parent
// The message can be handled to make the necessary validations, if any
#define WM_VALIDATE		WM_USER + 0x7FFD

// User define message 
// This message is posted to the parent
// The message should be handled to spcify the items to the added to the combo
#define WM_SET_ITEMS	WM_USER + 0x7FFC

class CComboListCtrl : public CListCtrl
{
public:
	
// Implementation
	typedef enum {MODE_READONLY,MODE_DIGITAL_EDIT,MODE_TEXT_EDIT,MODE_COMBO} COMBOLISTCTRL_COLUMN_MODE;

	// Constructor
	CComboListCtrl();

	// Destructor
	virtual ~CComboListCtrl();

	// Sets/Resets the column which support the in place combo box
	void SetComboColumns(int iColumnIndex, bool bSet = true);
	
	// Sets/Resets the column which support the in place edit control
	void SetReadOnlyColumns(int iColumnIndex, bool bSet = true);

	// Sets the valid characters for the edit ctrl
	void SetValidEditCtrlCharacters(CString& rstrValidCharacters);

	// Sets the vertical scroll
	void EnableVScroll(bool bEnable = true);

	// Sets the horizontal scroll
	void EnableHScroll(bool bEnable = true);

	//insert column
	int CComboListCtrl::InsertColumn(int nCol,LPCTSTR lpszColumnHeading,int nFormat = LVCFMT_LEFT,int nWidth = -1,int nSubItem = -1);
	BOOL CComboListCtrl::DeleteColumn(int nCol);
	//Get column counts
	int GetColumnCounts();

	//delete all column
	void DeleteAllColumn();

	//set column Valid char string
	void SetColumnValidEditCtrlCharacters(CString &rstrValidCharacters,int column = -1);
    bool IsReadOnly(int iColumnIndex);
	bool IsCombo(int iColumnIndex);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboListCtrl)
	//}}AFX_VIRTUAL

protected:

// Methods
	// Generated message map functions
	//{{AFX_MSG(CComboListCtrl)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
			
// Implementation

	// Returns the row & column index of the column on which mouse click event has occured
	bool HitTestEx(CPoint& rHitPoint, int* pRowIndex, int* pColumnIndex) const;

	// Creates and displays the in place combo box
	CInPlaceCombo* ShowInPlaceList(int iRowIndex, int iColumnIndex, CStringList& rComboItemsList, 
								   CString strCurSelecetion = "", int iSel = -1);

	// Creates and displays the in place edit control
	CInPlaceEdit* ShowInPlaceEdit(int iRowIndex, int iColumnIndex, CString& rstrCurSelection);

	// Calculates the cell rect
	void CalculateCellRect(int iColumnIndex, int iRowIndex, CRect& robCellRect);

	// Checks whether column supports in place combo box
//	bool IsCombo(int iColumnIndex);

	// Checks whether column is read only
	

	// Scrolls the list ctrl to bring the in place ctrl to the view
	void ScrollToView(int iColumnIndex, /*int iOffSet, */CRect& obCellRect);

// Attributes
	
	// List of columns that support the in place combo box
	CList<int, int> m_ComboSupportColumnsList;

	// List of columns that are read only
	CList<int, int> m_ReadOnlyColumnsList;

	// Valid characters
	CString m_strValidEditCtrlChars;

	// The window style of the in place edit ctrl
	DWORD m_dwEditCtrlStyle;

	// The window style of the in place combo ctrl
	DWORD m_dwDropDownCtrlStyle;

	//columnCounts
	int m_iColumnCounts;

	//column types
	COMBOLISTCTRL_COLUMN_MODE m_modeColumn[MAX_LISTCTRL_COLUMNS];

	//column 
	CString m_strValidChars[MAX_LISTCTRL_COLUMNS];
	//int m_
public:
	void RemoveItemAndCol(void);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOLISTCTRL_H__9089600F_374F_4BFC_9482_DEAC0E7133E8__INCLUDED_)

//
///*******************************************************************************
//	Author						: Aravindan Premkumar
//	Unregistered Copyright 2003	: Aravindan Premkumar
//	All Rights Reserved
//	
//	This piece of code does not have any registered copyright and is free to be 
//	used as necessary. The user is free to modify as per the requirements. As a
//	fellow developer, all that I expect and request for is to be given the 
//	credit for intially developing this reusable code by not removing my name as 
//	the author.
//*******************************************************************************/

#if !defined(AFX_INPLACEEDIT_H__175AEDFF_731E_4721_8399_DE406A465861__INCLUDED_)
#define AFX_INPLACEEDIT_H__175AEDFF_731E_4721_8399_DE406A465861__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

class CInPlaceEdit : public CEdit
{

public:
	
// Implementation
	
	// Returns the instance of the class
	static CInPlaceEdit* GetInstance(); 

	// Deletes the instance of the class
	static void DeleteInstance(); 

	// Creates the Windows edit control and attaches it to the object
	// Shows the edit ctrl
	BOOL ShowEditCtrl(DWORD dwStyle, const RECT& rCellRect, CWnd* pParentWnd, 
					  UINT uiResourceID, int iRowIndex, int iColumnIndex,
					  CString& strValidChars, CString& rstrCurSelection);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


// Attributes
	//	afx_msg void OnPaste(WPARAM wParam, LPARAM lParam);

protected:	
	// Generated message map functions
	//{{AFX_MSG(CInPlaceEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

// Implementation

	// Constructor
	CInPlaceEdit();

	// Hide the copy constructor and operator =
	CInPlaceEdit (CInPlaceEdit&) {}

	operator = (CInPlaceEdit) {}
	
	// Destructor
	virtual ~CInPlaceEdit();

// Attributes

	// Index of the item in the list control
	int m_iRowIndex;

	// Index of the subitem in the list control
	int m_iColumnIndex;

	// To indicate whether ESC key was pressed
	BOOL m_bESC;
	
	// Valid characters
	CString m_strValidChars;

	// Singleton instance
	static CInPlaceEdit* m_pInPlaceEdit;

	// Previous string value in the edit control
	CString m_strWindowText;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACEEDIT_H__175AEDFF_731E_4721_8399_DE406A465861__INCLUDED_)
/*******************************************************************************
	Author						: Aravindan Premkumar
	Unregistered Copyright 2003	: Aravindan Premkumar
	All Rights Reserved
	
	This piece of code does not have any registered copyright and is free to be 
	used as necessary. The user is free to modify as per the requirements. As a
	fellow developer, all that I expect and request for is to be given the 
	credit for intially developing this reusable code by not removing my name as 
	the author.
*******************************************************************************/

#if !defined(AFX_INPLACECOMBO_H__2E04D8D9_827F_4FBD_9E87_30AF8C31639D__INCLUDED_)
#define AFX_INPLACECOMBO_H__2E04D8D9_827F_4FBD_9E87_30AF8C31639D__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

class CInPlaceCombo : public CComboBox
{
public:
		
// Implementation
	
	// Returns the instance of the class
	static CInPlaceCombo* GetInstance(); 

	// Deletes the instance of the class
	static void DeleteInstance(); 

	// Creates the Windows combo control and attaches it to the object, if needed and shows the combo ctrl
	BOOL ShowComboCtrl(DWORD dwStyle, const CRect& rCellRect, CWnd* pParentWnd, UINT uiResourceID,
					   int iRowIndex, int iColumnIndex, CStringList* pDropDownList, CString strCurSelecetion = "", int iCurSel = -1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:

	// Generated message map functions
	//{{AFX_MSG(CInPlaceCombo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCloseup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

// Implementation
	// Constructor
	CInPlaceCombo();

	// Hide the copy constructor and operator =
	CInPlaceCombo (CInPlaceCombo&) {}

	operator = (CInPlaceCombo) {}

	// Destructor
	virtual ~CInPlaceCombo();

// Attributes

	// Index of the item in the list control
	int m_iRowIndex;

	// Index of the subitem in the list control
	int m_iColumnIndex;

	// To indicate whether ESC key was pressed
	BOOL m_bESC;
	
	// Singleton instance
	static CInPlaceCombo* m_pInPlaceCombo;

	// Previous selected string value in the combo control
	CString m_strWindowText;

	// List of items to be shown in the drop down
	CStringList m_DropDownList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACECOMBO_H__2E04D8D9_827F_4FBD_9E87_30AF8C31639D__INCLUDED_)