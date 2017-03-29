#pragma once



// CColumnDlg

class CColumnDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CColumnDlg)

public:
	CColumnDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CColumnDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CColumnDlg();

protected:
	DECLARE_MESSAGE_MAP()
};


