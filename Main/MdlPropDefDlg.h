#pragma once


// MdlPropDefDlg �Ի���

class MdlPropDefDlg : public CDialog
{
	DECLARE_DYNAMIC(MdlPropDefDlg)

public:
	MdlPropDefDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MdlPropDefDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CString m_Name;
	CString m_Code;
	CString m_TypeName;
	CString m_Person;
	CString m_Material;
	CString m_PicPath;
	CString m_SolidPath;
	CString m_DrawingPath;
	CString m_Note;
	ProMdl mdl;
	virtual BOOL OnInitDialog();
	int InitInfo(ProMdl solid);
protected:
	virtual void OnOK();
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton11();
};
