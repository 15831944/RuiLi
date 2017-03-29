#pragma once
#include "afxcmn.h"


// CMdlRelVisitDlg �Ի���

class CMdlRelVisitDlg : public CDialog
{
	DECLARE_DYNAMIC(CMdlRelVisitDlg)

public:
	CMdlRelVisitDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMdlRelVisitDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    
	DECLARE_MESSAGE_MAP()
public:
	bool GetMdlRel(void);
	void ShowMdlRel(void);
	ProMdl mdl;
	void SetMdl(ProMdl owner);
	virtual BOOL OnInitDialog();
	//CArray<ProRelset,ProRelset> p_ProRelSetList;
private:
	CArray<ProRelset,ProRelset> p_ProRelSetList;
	CStringArray rel_stringList;
	
public:
	CListCtrl m_List;
	afx_msg void OnBnClickedOk();
	CStringArray rel_stringList_select;
	
};
