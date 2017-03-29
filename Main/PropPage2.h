#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPropPage2 对话框

class CPropPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPage2)

public:
	CPropPage2();
	virtual ~CPropPage2();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_LARGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	// 性能参数的名称
	CString m_PerfParaName;
	// 性能参数的值
	CString m_PerfParaValue;
	// 性能参数的说明
	CString m_PerfParaNote;
	int SetValue(ProMdl solid, CArray<UsrPerfParaTable,UsrPerfParaTable> * pp_temp_UsrPerfParaTableList);
	CArray<UsrPerfParaTable,UsrPerfParaTable> * p_temp_UsrPerfParaTableList;
	int SetList(void);
	ProMdl mdl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
private:
	int m_index;
public:
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	int m_iRadio;
	CEdit m_MaxValEdit;
	CEdit m_MinValEdit;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	BOOL CheckInput(int iEditItem);
};
