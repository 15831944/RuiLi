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


// CMdlSeriesDlg �Ի���

class CMdlSeriesDlg : public CDialog
{
	DECLARE_DYNAMIC(CMdlSeriesDlg)

public:
	CMdlSeriesDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMdlSeriesDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnMeasureItem( int nIDCtl,LPMEASUREITEMSTRUCT lpMeasureItemStruct );

	MdlInsInfo newIns;
	//����һ�������ж�������ԵĽṹ���ṹ�������Ӧ��ģ�Ͳ����ı�ţ��Լ�����������
	typedef struct item_Prop {
		long para_num;
		int type; //0Ϊ�ַ���1Ϊ���㣬2Ϊ���ͣ�3Ϊ����
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
	// ����ģ��ϵ�л���ֵ
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
