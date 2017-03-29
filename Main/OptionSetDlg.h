#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// COptionSetDlg 对话框

class COptionSetDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionSetDlg)

public:
	COptionSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COptionSetDlg();

// 对话框数据
	enum { IDD = IDD_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_ip;
	CEdit m_FtpUsrName;
	CEdit m_FtpPwd;
	CString m_DSN;
	CString m_DBUsrName;
	CString m_DBPwd;
	int m_port;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	CString m_PdmDsn;
	CString m_PdmDsnUser;
	CString m_PdmDsnPwd;
	afx_msg void OnBnClickedButton10();
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnMove(int x, int y);
};
