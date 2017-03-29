#pragma once
#include "afxcmn.h"
#include "afxwin.h"

//#include "Picture.h"


// CAsmQueryDlg 对话框

class CAsmQueryDlg : public CDialog
{
	DECLARE_DYNAMIC(CAsmQueryDlg)

public:
	CAsmQueryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAsmQueryDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ASMQUERY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButtonQuery();
	CTreeCtrl m_producttree;
	CListCtrl m_productlist;
	CString MdlNum;
	ProSolid ProductSolid;
	int picNum;
	CString Name;
	CString FileDirectory;
	IPicture*   p_Picture;
	BOOL IsSelect;
public:
	afx_msg void OnNMClickTreeProductGroup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListProductModel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListProductModel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
	BOOL ShowPic(int num,int Number);
	BOOL DBOpenPic(BYTE *pBuffer,int nSize,IPicture ** pic);
	CStatic m_pic;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonPre();
	afx_msg void OnBnClickedButtonNext();
	BOOL GetAllAsmModel();
	afx_msg void OnNMRclickListProductModel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPreview2d();
	afx_msg void OnPreview3d();
};
