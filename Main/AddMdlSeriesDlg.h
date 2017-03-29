#pragma once
#include "afxwin.h"
#include "atltime.h"


// CAddMdlSeriesDlg �Ի���

class CAddMdlSeriesDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddMdlSeriesDlg)

public:
	CAddMdlSeriesDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddMdlSeriesDlg();

// �Ի�������
	enum { IDD = IDD_MDLSERIES_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    
	DECLARE_MESSAGE_MAP()
public:
	void SetMdlNumberAndCode(long mdlNumber, CString code,CArray<MdlInsInfo,MdlInsInfo> * InsInfoList);
	//CString m_Name;
	//CString m_Code;
	//CString m_Person;
	//CString m_Dept;
	//CString m_Note;
	CArray<MdlInsInfo,MdlInsInfo>  * p_InsInfoList;
	virtual BOOL OnInitDialog();
	MdlInsInfo newMdl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString strCaption;
	CString time;
};
