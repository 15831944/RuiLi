#pragma once


// CPicEditBoxDlg �Ի���

class CPicEditBoxDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicEditBoxDlg)

public:
	CPicEditBoxDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPicEditBoxDlg();

// �Ի�������
	enum { IDD = IDD_PIC_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Note;
	afx_msg void OnBnClickedOk();
};
