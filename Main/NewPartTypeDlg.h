#pragma once


// CNewPartTypeDlg �Ի���

class CNewPartTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewPartTypeDlg)

public:
	CNewPartTypeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewPartTypeDlg();

// �Ի�������
	enum { IDD = IDD_NEW_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	CString m_TypeName;
	CString m_TypeNote;
	CString m_FatherName;
	int SetFatherName(CString FatherName,CString FatherCode);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	// ���ı���
	CString m_Code;
};
