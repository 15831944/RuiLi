#pragma once
#include "combolistctrl.h"
#include "partmdlrecordset.h"
#include "partperfparatableset.h"
#include "partinstableset.h"
#include "partinsvaluetableset.h"
#include "partinsperfvaluetableset.h"
#include "partvarianttableset.h"
#include "afxwin.h"
#include "afxcmn.h"

// CPartSeriesDlg 对话框

class CPartSeriesDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartSeriesDlg)

public:
	CPartSeriesDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPartSeriesDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	typedef struct item_Prop {
		long para_num;
		int type; //0为字符，1为浮点，2为整型，3为布尔
		CString note;
	}ItemProp;
	CArray<ItemProp,ItemProp> p_ColProp;
    MdlInsInfo newIns;
    CArray<MdlInsInfo,MdlInsInfo>  p_InsInfo;
	CPartInsTableSet      *    m_InsTableSet;

	LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);

	// {{AFX_MSG(CPartSeriesDlg)
	//afx_msg LRESULT OnMyMessage(WPARAM wParam,  LPARAM lParam);
	//}}AFX_MSG
	

	DECLARE_MESSAGE_MAP()
public:
	CComboListCtrl m_List;
	virtual BOOL OnInitDialog();
	void SetMdlNumberAndDB(long num,CDatabase * db);
	long mdlNumber;
	void InitListByMdlInfo(void);
	CDatabase *p_db;
	void AddSeriesData(void);
	afx_msg void OnBnClickedAddButton();
	CButton m_AddButton;
	CButton m_OkButton;
	CButton m_CancelAddButton;
	afx_msg void OnBnClickedButton1();
	long GetMdlSeriesMaxNumber(void);
	bool CheckNewSeries(void);
	int m_current_item;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	int combColumn;
	afx_msg void OnBnClickedButton2();
	CToolTipCtrl m_tooltip;
	afx_msg void OnLvnHotTrackList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedDelButton();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedModButton();
	CButton m_ModifyButton;
	afx_msg void OnBnClickedViewButton();

protected:
//	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
//	afx_msg void OnChangeUIState(UINT nAction, UINT nUIElement);
};
