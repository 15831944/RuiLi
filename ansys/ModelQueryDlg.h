#pragma once
#include "afxcmn.h"
#include "myproe.h"
#include "ProSolid.h"
#include "afxtempl.h"
#include "ProMdl.h"
#include "FtpFile.h"
#include "afxwin.h"
#include "MyDatabase.h"
#include "Resource.h"


// CModelQueryDlg 对话框

class CModelQueryDlg : public CDialog
{
	DECLARE_DYNAMIC(CModelQueryDlg)

public:
	CModelQueryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModelQueryDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MODELQUERY_BA};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnMove(int x, int y);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonModeldown();
	afx_msg void OnNMClickListModel(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnNMClickTreePartmodel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonNext();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	BOOL ObtianPartModelAfter(int num);

	afx_msg void OnNMDblclkListModel(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL DBOpenPic(BYTE *pBuffer,int nSize,IPicture ** pic);
	BOOL ShowPic(int num,int Number);
	CStatic m_pic;
	ProSolid ProductSolid;
	int picNum;
	CString MdlNum;
	IPicture*   p_Picture;
	CString Name,FileDirectory;
	CListCtrl m_modelresult;
	CTreeCtrl m_tree;
    CFtpFile ftpfile;

	CMyDatabase m_db;
	BOOL ObtainPartModelClass(CTreeCtrl *m_tree);
	BOOL OPenModel(int num);


	//判断是否选种了一个模型
	BOOL IsSelect;
	CString PrtNum;
	CString m_Path;
	afx_msg void OnBnClickedButtonSelect();
};
