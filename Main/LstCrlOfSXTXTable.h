#pragma once
#include "afxwin.h"


// CLstCrlOfSXTXTable

class CLstCrlOfSXTXTable : public CListCtrl
{
	DECLARE_DYNAMIC(CLstCrlOfSXTXTable)

public:
	CLstCrlOfSXTXTable();
	virtual ~CLstCrlOfSXTXTable();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_edit;
};


