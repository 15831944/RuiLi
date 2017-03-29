#pragma once
#include "afxwin.h"


// CRelInfoEditBox 对话框

class CRelInfoEditBox : public CDialog
{
	DECLARE_DYNAMIC(CRelInfoEditBox)

public:
	CRelInfoEditBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRelInfoEditBox();

// 对话框数据
	enum { IDD = IDD_REL_EDITBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CEdit m_note;
	CString m_note;
	afx_msg void OnBnClickedOk();
};
