#pragma once


// CDimEditDlg �Ի���

class CDimEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CDimEditDlg)

public:
	CDimEditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDimEditDlg();

// �Ի�������
	enum { IDD = IDD_DIMEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_ID;
	CString m_Symbol;
	double m_Value;
	float m_UpLimit;
	double m_DwLimit;
	afx_msg void OnBnClickedOk();
};
