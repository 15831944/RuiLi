// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
//#include "ClassList.h"
#include "ListCtrlEx.h"
#include "ListCellEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx
#define WM_MOVNEX       WM_USER + 2000
#define WM_LEACH       WM_USER + 3000

CListCtrlEx::CListCtrlEx()
{
	m_ReadOnlyColumnsList.RemoveAll();
	pEdit=NULL;
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_MESSAGE(WM_MOVNEX,OnMovNex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers
// OnCustomDraw
void CListCtrlEx::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.

	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		// This is the prepaint stage for a subitem. Here's where we set the
		// item's text and background colors. Our return value will tell
		// Windows to draw the subitem itself, but it will use the new colors
		// we set here.

		int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
		int nSubItem = pLVCD->iSubItem;
		if(IsReadOnly(nSubItem))	
		{
			COLORREF clr = 12632256;
			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			CRect rect;
			GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
			pDC->Rectangle(rect);
			DrawText(nItem, nSubItem, pDC, 0, clr, rect);
			
			*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.
		}
	}
}


void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListCtrl::OnLButtonDown(nFlags, point);
	// TODO: Add your message handler code here and/or call default
    int nItem;
    int nSubItem;
    if ((nItem = HitTestEx (point, nSubItem)) != -1)
    {
		if(!IsReadOnly(nSubItem)) //如果不是只读
		{
			EditSubItem(nItem, nSubItem);
		}
    }
}

int CListCtrlEx::HitTestEx (CPoint& Point, int& nSubItem)
{
	nSubItem = 0;
	int ColumnNum = 0;
    int Row = HitTest (Point, NULL);
	
    // Make sure that the ListView is in LVS_REPORT
    if ((GetWindowLong (m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return Row;
	
    // Get the top and bottom row visible
    Row = GetTopIndex();
    int Bottom = Row + GetCountPerPage();
    if (Bottom > GetItemCount())
		Bottom = GetItemCount();
    
    // Get the number of columns
    CHeaderCtrl* pHeader = (CHeaderCtrl*) GetDlgItem(0);
    int nColumnCount = pHeader->GetItemCount();
	
    // Loop through the visible rows
    for(; Row <= Bottom; Row++)
    {
		// Get bounding rect of item and check whether point falls in it.
		CRect Rect;
		GetItemRect (Row, &Rect, LVIR_BOUNDS);
		if (Rect.PtInRect (Point))
		{
			// Now find the column
			for (ColumnNum = 0; ColumnNum < nColumnCount; ColumnNum++)
			{
				int ColWidth = GetColumnWidth (ColumnNum);
				if (Point.x >= Rect.left && Point.x <= (Rect.left + ColWidth))
				{
					nSubItem = ColumnNum;
					return Row;
				}
				Rect.left += ColWidth;
			}
		}
    }

    return -1;
}

CEdit* CListCtrlEx::EditSubItem(int nItem, int nSubItem)
{
    // The returned pointer should not be saved

    // Make sure that the item is visible
    if (!EnsureVisible (nItem, TRUE)) return NULL;

    // Make sure that nCol is valid
    CHeaderCtrl* pHeader = (CHeaderCtrl*) GetDlgItem(0);
    int nColumnCount = pHeader->GetItemCount();
    if (nSubItem >= nColumnCount || GetColumnWidth (nSubItem) < 5)
		return NULL;

    // Get the column offset
    int Offset = 0;
    for (int iColumn = 0; iColumn < nSubItem; iColumn++)
		Offset += GetColumnWidth (iColumn);

    CRect Rect;
    GetItemRect (nItem, &Rect, LVIR_BOUNDS);

    // Now scroll if we need to expose the column
    CRect ClientRect;
    GetClientRect (&ClientRect);
    if (Offset + Rect.left < 0 || Offset + Rect.left > ClientRect.right)
    {
		CSize Size;
		if (Offset + Rect.left > 0)
			Size.cx = -(Offset - Rect.left);
		else
			Size.cx = Offset - Rect.left;
		Size.cy = 0;
		Scroll (Size);
		Rect.left -= Size.cx;
    }

    // Get nSubItem alignment
    LV_COLUMN lvCol;
    lvCol.mask = LVCF_FMT;
    GetColumn (nSubItem, &lvCol);
    DWORD dwStyle;
    if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
    else if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
    else dwStyle = ES_CENTER;

    Rect.left += Offset/*+4*/;
    Rect.right = Rect.left + GetColumnWidth (nSubItem) /*- 3*/;
    if (Rect.right > ClientRect.right)
		Rect.right = ClientRect.right;

    dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	pEdit = new CListCellEdit (nItem, nSubItem, GetItemText (nItem, nSubItem));
	pEdit->Create (dwStyle, Rect, this, IDC_EDIT_1);

    /*CEdit **/
	//delete pEdit;
    return pEdit;
}

void CListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
    if (GetFocus() != this) SetFocus();
	
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CListCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
    if (GetFocus() != this) SetFocus();

	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CListCtrlEx::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CListCtrlEx::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
    LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
    LV_ITEM	*plvItem = &plvDispInfo->item;

    if (plvItem->pszText != NULL)
    {
		SetItemText (plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
    }
    *pResult = FALSE;
}


LRESULT CListCtrlEx::OnMovNex(WPARAM wparam,LPARAM lparam)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*) GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	int m_nItem=(int)wparam;
	int m_nSubItem=(int)lparam;
	this->SetFocus ();
	GetParent()->SendMessage(WM_LEACH,m_nItem,m_nSubItem);
	while (m_nItem<GetItemCount())
	{
		m_nSubItem++;
		if (!IsReadOnly(m_nSubItem) && m_nSubItem<nColumnCount/* && m_nSubItem!=nColumnCount-1*/)
		{
			EditSubItem(m_nItem,m_nSubItem);
				break;
		}
		if (m_nSubItem==nColumnCount)
		{
			m_nItem++;
			m_nSubItem=0;
			continue;
		}
	}
	if (m_nItem==GetItemCount() && GetItemCount()!=0)
	{
		//this->SetFocus ();
		m_nItem=0;
		m_nSubItem=0;
		while (m_nItem<GetItemCount())
		{
			if (!IsReadOnly(m_nSubItem) && m_nSubItem<nColumnCount &&m_nSubItem!=(nColumnCount-1))
			{
				EditSubItem(m_nItem,m_nSubItem);
				break;
			} 
			else
				m_nSubItem++;
		}
	}
	return 1;
}
void CListCtrlEx::DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect)
{
	ASSERT(pDC);

	//	GetDrawColors(nItem, nSubItem, crText, crBkgnd);

	pDC->FillSolidRect(&rect, crBkgnd);

	CString str;
	str = GetItemText(nItem, nSubItem);

	if (!str.IsEmpty())
	{
		// get text justification
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		pDC->DrawText(str, &rect, DT_LEFT);

	}

}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CListCtrlEx::SetReadOnlyColumns(int iColumnIndex, bool bSet /*= true*/)
{
	if (!bSet)
	{
		m_ReadOnlyColumnsList.AddTail(iColumnIndex);
	}
}
void CListCtrlEx::SetColumnValidEditCtrlCharacters(CString &rstrValidCharacters,int column)
{
	if(column == -1)
	{
		for(int i=0;i<MAX_LISTCTRL_COLUMNS;i++)
		{
			m_strValidChars[i] = rstrValidCharacters;
		}
	}
	else
		m_strValidChars[column] = rstrValidCharacters;
}
/************************************************************************/
/* 是否是只读                                                           */
/************************************************************************/
BOOL CListCtrlEx::IsReadOnly(int nSubItem)
{
//	POSITION pos;
		if (m_ReadOnlyColumnsList.Find(nSubItem)!=NULL)
		{
			return TRUE;			//只读
		}
	return FALSE;
}