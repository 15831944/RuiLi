#pragma once
#include "afxcmn.h"


// CAsmRelVistDlg �Ի���

class CAsmRelVistDlg : public CDialog
{
	DECLARE_DYNAMIC(CAsmRelVistDlg)

public:
	CAsmRelVistDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAsmRelVistDlg();

// �Ի�������
	enum { IDD = IDD_ASM_REL_VISIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	void VistRel(ProSolid mdl);
	ProSolid solid;
	afx_msg void OnBnClickedOk();
	CStringArray * relSet;
	CStringArray * relNote;
};
