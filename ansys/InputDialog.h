#pragma once


// CInputDialog �Ի���

class CInputDialog : public CDialog
{
	DECLARE_DYNAMIC(CInputDialog)

public:
	CInputDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInputDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Text;
	afx_msg void OnBnClickedOk();
};
