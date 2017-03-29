#pragma once


// CSwtxTableItemEditDlg 对话框

class CSwtxTableItemEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CSwtxTableItemEditDlg)

public:
	CSwtxTableItemEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSwtxTableItemEditDlg();

// 对话框数据
	enum { IDD = IDD_SWTX_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Name;
	CString m_Note;
	virtual BOOL OnInitDialog();
};
