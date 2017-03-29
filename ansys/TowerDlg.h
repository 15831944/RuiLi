#pragma once



// CTowerDlg

class CTowerDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CTowerDlg)

public:
	CTowerDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTowerDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CTowerDlg();

protected:
	DECLARE_MESSAGE_MAP()
};


