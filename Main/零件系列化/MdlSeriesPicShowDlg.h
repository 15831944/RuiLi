#pragma once
#include "afxcmn.h"
#include "PartMdlRecordSet.h"
#include "partPicSet.h"

// CMdlSeriesPicShowDlg �Ի���

class CMdlSeriesPicShowDlg : public CDialog
{
	DECLARE_DYNAMIC(CMdlSeriesPicShowDlg)

public:
	CMdlSeriesPicShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMdlSeriesPicShowDlg();

// �Ի�������
	enum { IDD = IDD_MDLPIC_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    TCHAR szTempPath [MAX_PATH];                        //���ڴ�����ʱĿ¼
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	void SetMdlNumber(long mdlNumber, CDatabase * p_db);
	CPartMdlRecordSet * m_pSet;
	CString m_path;
	virtual BOOL OnInitDialog();
	void ShowList(void);
	int mdlNum;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
};
