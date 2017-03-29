#pragma once
#include "ListCtrlEx.h"
#include "DetailInfoClass.h"


// CPrtSaveAsDlg �Ի���

class CPrtSaveAsDlg : public CDialog
{
	DECLARE_DYNAMIC(CPrtSaveAsDlg)

public:
	CPrtSaveAsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPrtSaveAsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PRTSAVEAS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    
	DECLARE_MESSAGE_MAP()
public:
	CString m_inscode;
	CString m_insname;
	CString m_mdlnum;
	CString m_note;
	CString m_group;
	CString m_material;
	CString m_unit;
	CString m_filename;
	CString m_place;
	CString m_designer;
	CString m_drwingplace;
	CComboBox m_designstate;
	CListCtrlEx m_perf;
	DetailInfoClass *info;
	ProSolid ProductSolid;
	ProSolid solid;
	BOOL Mem;                   //�����ڴ��־
	BOOL IsOneOfProduct;
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSolidlocalplace();
	afx_msg void OnBnClickedButtonDrwlocalplace();
	afx_msg void OnBnClickedButtonSolidserverplace();
	afx_msg void OnBnClickedButtonDwgserverplace();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButtonGrpfrm();
	CString m_pdmcode;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton9();
};
