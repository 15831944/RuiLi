#pragma once
#include "afxwin.h"


// CRelInfoEditBox �Ի���

class CRelInfoEditBox : public CDialog
{
	DECLARE_DYNAMIC(CRelInfoEditBox)

public:
	CRelInfoEditBox(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRelInfoEditBox();

// �Ի�������
	enum { IDD = IDD_REL_EDITBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//CEdit m_note;
	CString m_note;
	afx_msg void OnBnClickedOk();
};
