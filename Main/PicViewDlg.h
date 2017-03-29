#pragma once

#include "afxwin.h"
#include "picture.h"

// CPicViewDlg 对话框

class CPicViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicViewDlg)

public:
	CPicViewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicViewDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnPaint();
	//CJpgshowctlctrl1 m_pic;
	//CStatic m_pic;
	void SetFile(CString filePathName);
	void SetIpicture(IPicture * pIpitcure);
	CString file;
	CStatic m_pic;
	IPicture * p_Picture;
	int     mode;//判断是从文件，还是从接口
	afx_msg void OnMove(int x, int y);
	CMyPicture pic;
};
