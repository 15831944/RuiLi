#pragma once


// CNewPartTypeDlg 对话框

class CNewPartTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewPartTypeDlg)

public:
	CNewPartTypeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewPartTypeDlg();

// 对话框数据
	enum { IDD = IDD_NEW_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	CString m_TypeName;
	CString m_TypeNote;
	CString m_FatherName;
	int SetFatherName(CString FatherName,CString FatherCode);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	// 类别的编码
	CString m_Code;
};
