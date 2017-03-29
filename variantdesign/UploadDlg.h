#pragma once
#include "afxdlgs.h"
#include "afxcmn.h"
#include "ProMdl.h"
#include "ProSolid.h"
#include "DetailInfoClass.h"
#include "ProductClass.h"
#include "afxtempl.h"
#include "MyDatabase.h"

// CProductSaveDlg 对话框
void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data,BOOL nFlag);

extern CArray <Database,Database> Database_struc_temp;
extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CArray <JuBing,JuBing> JuBing_struc_temp;
extern FtpInfo Ftp_struc;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CPtrArray p_classarray;

class CUploadDlg : public CDialog
{
	DECLARE_DYNAMIC(CUploadDlg)

public:
	CUploadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUploadDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_UPLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMDblclkTreeAsm(NMHDR *pNMHDR, LRESULT *pResult);
	ProMdl m_menmdl;
	CTreeCtrl m_asmmodeltree;
	ProSolid Productsolid;
    int s;
	CImageList m_ImageList;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMClickTreeAsm(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL IsProductCanAudit(int s);
	BOOL AffirmPartIsAlreadyAudit(int SolidID,BOOL *IsAudit,CString *FileName);
	CListCtrl m_dim;
	afx_msg void OnNMClickListDim(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL IsAsmCanAudit(ProSolid solid,int i,CListCtrl *list);
	BOOL ListDim(int s,CString Type,CListCtrl *list);
	afx_msg void OnBnClickedButtonPdtinfo();
	BOOL AuditFunc(int s);
	afx_msg void OnBnClickedButtonFalse();
};
