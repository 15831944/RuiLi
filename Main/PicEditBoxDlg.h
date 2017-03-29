#pragma once


// CPicEditBoxDlg 对话框

class CPicEditBoxDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicEditBoxDlg)

public:
	CPicEditBoxDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicEditBoxDlg();

// 对话框数据
	enum { IDD = IDD_PIC_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Note;
	afx_msg void OnBnClickedOk();
};
