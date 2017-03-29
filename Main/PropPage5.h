#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "PicShow.h"
#include "picviewdlg.h"

// CPropPage5 对话框

class CPropPage5 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPage5)

public:
	CPropPage5();
	virtual ~CPropPage5();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_LARGE4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	CString m_PicFileName;
	CString m_PicNote;
	// 图片信息
	CArray<UsrPicture,UsrPicture> * p_temp_UsrPicture;
	int SetValue(ProMdl solid,CArray<UsrPicture,UsrPicture>* pp_temp_UsrPicture);
	int SetList(void);
	ProMdl mdl;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	int m_index;
	CPicShow m_pic;
	int ShowPic(CString file);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CPicViewDlg * dlg;
	afx_msg void OnBnClickedButton1();
	CButton m_ViewPic;
//	afx_msg void OnClose();
};
