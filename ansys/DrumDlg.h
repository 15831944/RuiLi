#pragma once



// CDrumDlg

class CDrumDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CDrumDlg)

public:
	CDrumDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDrumDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDrumDlg();

protected:
	DECLARE_MESSAGE_MAP()
};


