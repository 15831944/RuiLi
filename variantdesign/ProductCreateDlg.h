#pragma once


// CProductCreateDlg �Ի���

class CProductCreateDlg : public CDialog
{
	DECLARE_DYNAMIC(CProductCreateDlg)

public:
	CProductCreateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProductCreateDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CREATEPRODUCT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_stylename;
	CString m_stylenum;
	CString m_stylecode;
	CString m_briefname;
	CString m_stylenote;
	CString m_manager;
	CString m_upmanager;
	afx_msg void OnBnClickedButtonSelectstyle();
	afx_msg void OnBnClickedButtonSelstand();
	afx_msg void OnMove(int x, int y);
	CString m_stand;
	CString m_standnote;
	CString m_productnote;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_productname;
	CString m_productcode;	  //��Ʒͼ��
};
