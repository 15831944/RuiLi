#pragma once
#include "variantdesign.h"
#include "picture.h"


// CPicShow

class CPicShow : public CStatic
{
	DECLARE_DYNAMIC(CPicShow)

public:
	CPicShow();
	virtual ~CPicShow();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CString file;
	int SetFile(CString filename);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnStnDblclick();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	BOOL DBOpenPic(BYTE *pBuffer,int nSize,CRect recttemp);
	CMyPicture pic;

	IPicture*   p_Picture;
	BYTE * pBuffertemp;
	CRect rect;
};


