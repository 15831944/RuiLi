#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ComboListCtrl.h"

// CParaOperaDlg �Ի���

class CParaOperaDlg : public CDialog
{
	DECLARE_DYNAMIC(CParaOperaDlg)

public:
	CParaOperaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParaOperaDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CArray<ProParameter,ProParameter> p_data;           //ָ��ģ�͵����в����б�
	int m_type;                                         //Ҫ��Ӳ���������
	CListCtrl m_List;                                   //�б�
	CString m_Name;                                     //����������
	CString m_Value;                                    //������ֵ
	CComboBox  m_ComboBox;                              //��ѡ��
	CArray<ProSolid,ProSolid> solid_list;           //���е�ģ���б����������ֻ��һ����
	                                                    //�б������븴�϶�Ӧ����Ҫ��ͬ
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
