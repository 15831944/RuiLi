#pragma once
#include "autovuexctrl1.h"
#include "ProgressDlg.h"


// CPreViewDlg 对话框

class CPreViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreViewDlg)

public:
	CPreViewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPreViewDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_Initflag;
	int m_ModalFlag;//1,零件模型 2，零件实例 3，产品模型 4，产品实例
	int m_2DFlag;//0,没有2D图，1 有2D图
	CString m_strID;///模型id,实例id等。
	CString m_TempPath;

	CString m_LocalSolidPath,m_LocalDrawingPath;
	CString m_ProductLocalSolidPath,m_ProductLocalDrawingPath;
	CString m_Name;

	CProgressDlg * m_pDlg;

	int m_EndFlag;
public:
	int DeleteFolderAllFile(CString folder);
	int GetPartModal();
	int GetPartIns();
	int GetProductModal();
	int GetProductIns();

	int NGetProductModal();
	int NGetProductIns();
	


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CAutovuexctrl1 m_AutoVue;
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
};
