#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ComboListCtrl.h"

// CParaOperaDlg 对话框

class CParaOperaDlg : public CDialog
{
	DECLARE_DYNAMIC(CParaOperaDlg)

public:
	CParaOperaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParaOperaDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CArray<ProParameter,ProParameter> p_data;           //指定模型的所有参数列表
	int m_type;                                         //要添加参数的类型
	CListCtrl m_List;                                   //列表
	CString m_Name;                                     //参数的名称
	CString m_Value;                                    //参数的值
	CComboBox  m_ComboBox;                              //复选框
	CArray<ProSolid,ProSolid> solid_list;           //所有的模型列表，对于零件，只有一个，
	                                                    //列表的序号与复合对应的需要相同
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ParamsGet();
	void ListLabelsSet(void);

	afx_msg void OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButton4();

	afx_msg void OnCbnSelchangeCombo2();
	CEdit m_ValueEdit;
	BOOL CheckValidCString(void);
};
