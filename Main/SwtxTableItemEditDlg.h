#pragma once


// CSwtxTableItemEditDlg �Ի���

class CSwtxTableItemEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CSwtxTableItemEditDlg)

public:
	CSwtxTableItemEditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSwtxTableItemEditDlg();

// �Ի�������
	enum { IDD = IDD_SWTX_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Name;
	CString m_Note;
	virtual BOOL OnInitDialog();
};
