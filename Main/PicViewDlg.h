#pragma once

#include "afxwin.h"
#include "picture.h"

// CPicViewDlg �Ի���

class CPicViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicViewDlg)

public:
	CPicViewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPicViewDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnPaint();
	//CJpgshowctlctrl1 m_pic;
	//CStatic m_pic;
	void SetFile(CString filePathName);
	void SetIpicture(IPicture * pIpitcure);
	CString file;
	CStatic m_pic;
	IPicture * p_Picture;
	int     mode;//�ж��Ǵ��ļ������Ǵӽӿ�
	afx_msg void OnMove(int x, int y);
	CMyPicture pic;
};
