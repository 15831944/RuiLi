#pragma once

#include "partvarianttableset.h"
#include "combolistctrl.h"
#include "partmdlrecordset.h"
#include "partperfparatableset.h"
#include "partinstableset.h"
#include "partinsvaluetableset.h"
#include "partinsperfvaluetableset.h"

#include "afxwin.h"
#include "afxcmn.h"


// CMdlSeriesDlg 对话框

class CMdlSeriesDlg : public CDialog
{
	DECLARE_DYNAMIC(CMdlSeriesDlg)

public:
	CMdlSeriesDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMdlSeriesDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnMeasureItem( int nIDCtl,LPMEASUREITEMSTRUCT lpMeasureItemStruct );

	MdlInsInfo newIns;
	//定义一个用于判断项的属性的结构，结构包括项对应的模型参数的编号，以及参数的类型
	typedef struct item_Prop {
		long para_num;
		int type; //0为字符，1为浮点，2为整型，3为布尔
		CString note;
	}ItemProp;
	CArray<ItemProp,ItemProp> p_ItemProp;
	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	int SetMdlNumber(long Number);
	long mdlNumber;
	
	CpartVariantTableSet m_mdlVariantSet;
	CComboListCtrl m_List;
	// 设置模型系列化的值
	void SetMdlSWTXAndPerfList(void);
    CToolTipCtrl m_tooltip;
	//afx_msg void OnLvnBeginlabeleditList1(NMHDR *pNMHDR, LRESULT *pResult);
	CPartMdlRecordSet m_MdlInfoSet;
	CpartPerfparaTableSet m_mdlPerfparaSet;
	void InitListWithMdlSeries(void);
	CPartInsTableSet m_InsTableSet;
	CpartInsValueTableSet m_InsValueTableSet;
	CpartInsPerfValueTableSet m_InsPerfValueTableSet;
	afx_msg void OnBnClickedAddButton();
	int m_currentCol;
	
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_AddButton;
	CButton m_OkAddButton;
	CButton m_CancelAddButton;
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	long GetMdlSeriesMaxNumber(void);
public:
	afx_msg void OnLvnHotTrackList1(NMHDR *pNMHDR, LRESULT *pResult);	

	afx_msg void OnBnClickedButton1();
	bool CheckNewSeries(void);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};
