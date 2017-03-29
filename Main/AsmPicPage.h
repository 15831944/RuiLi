#pragma once
#include "afxcmn.h"
#include "PicShow.h"
#include "picviewdlg.h"
#include "afxwin.h"

// CAsmPicPage 对话框

class CAsmPicPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmPicPage)

public:
	CAsmPicPage();
	virtual ~CAsmPicPage();

// 对话框数据
	enum { IDD = IDD_ASM_PIC_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ProSolid mdl;
	CListCtrl m_List;
	int       m_index;
	CString m_FileName;
	CString m_Note;
	afx_msg void OnBnClicked1005();
	afx_msg void OnBnClicked1012();
	afx_msg void OnBnClicked1004();
	int ShowList(void);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClick1002(NMHDR *pNMHDR, LRESULT *pResult);
	CPicShow m_pic;
	CButton m_ViewPic;
	CPicViewDlg * dlg;
	int ShowPic(CString file);
};
