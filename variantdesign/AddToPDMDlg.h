#pragma once


// AddToPDMDlg �Ի���

class AddToPDMDlg : public CDialog
{
	DECLARE_DYNAMIC(AddToPDMDlg)

public:
	AddToPDMDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AddToPDMDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADDTOPDM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPar();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedOk();
};
