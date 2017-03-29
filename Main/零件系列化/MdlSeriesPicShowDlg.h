#pragma once
#include "afxcmn.h"
#include "PartMdlRecordSet.h"
#include "partPicSet.h"

// CMdlSeriesPicShowDlg 对话框

class CMdlSeriesPicShowDlg : public CDialog
{
	DECLARE_DYNAMIC(CMdlSeriesPicShowDlg)

public:
	CMdlSeriesPicShowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMdlSeriesPicShowDlg();

// 对话框数据
	enum { IDD = IDD_MDLPIC_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    TCHAR szTempPath [MAX_PATH];                        //用于创建临时目录
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	void SetMdlNumber(long mdlNumber, CDatabase * p_db);
	CPartMdlRecordSet * m_pSet;
	CString m_path;
	virtual BOOL OnInitDialog();
	void ShowList(void);
	int mdlNum;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
};
