#pragma once


// CMdlDefDlg �Ի���

class CMdlDefDlg : public CDialog
{
	DECLARE_DYNAMIC(CMdlDefDlg)

public:
	CMdlDefDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMdlDefDlg();

// �Ի�������
	enum { IDD = IDD_MDLDEF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//bool CheckIfHadDef();
	virtual BOOL OnInitDialog();
	int InitInfo(ProMdl solid);
	CString m_Name;
	CString m_Code;
	CString m_Type;
	CString m_FileName;
	CString m_PicFile;
	CString m_Note;
	CString m_FilePath;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	ProMdl mdl;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnClose();
	CString m_Creater;
	CString m_Material;
	CString m_DrawingPath;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton11();
};
