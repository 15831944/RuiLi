#pragma once
#include "afxcmn.h"
#include "ComboListCtrl.h"

// CTestRelDlg �Ի���

class CTestRelDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestRelDlg)

public:
	CTestRelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestRelDlg();

// �Ի�������
	enum { IDD = IDD_TEST_VARIANT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CArray<CRel,CRel&>	p_RelListUnSorted;  //û������Ĺ�ϵʽ��
	CArray<CRel,CRel&>	p_RelListSorted;    //����֮��Ĺ�ϵʽ��
	void GetSolidRelSetAndSwtxSet();
	//����ϵʽ�гߴ�Ͳ������Լ��������Ա��еĳߴ�Ͳ�����
	struct rel_swtx_item {
		int            type;     //0 �ߴ� 1 ����
		CString        name;     //���������������Ա�
		ProDimension   dim;      //�ߴ�
		ProParameter   para;	 //����
		double         value1;	 //ԭ��ֵ
		double         value2;	 //����ֵ
	};
    CArray<rel_swtx_item,rel_swtx_item&>   p_Swtx; 
	CArray<rel_swtx_item,rel_swtx_item&>   p_Rv; 
	//�����ϵ��ֵ�������ϵʽ(���ֱ��ʽ)
	BOOL  Calculate(CString,double *);
	//�Թ�ϵʽ��������
	BOOL  SortRelSet();
	BOOL  SortRelSet(CArray<CRel,CRel&>	* p_InRelList
		,CArray<rel_swtx_item,rel_swtx_item&>  *p_MainParaList,int * index);
	//ģ������
	BOOL  Regeneralte();
	BOOL  SetDimParamterValue(ProDimension * dim,ProParameter * para,double value);
public:
	//ģ��
	ProSolid solid;
	
	CComboListCtrl m_ListSwtx;
	CListCtrl m_ListRel;
	CListCtrl m_ListRv;
	virtual BOOL OnInitDialog();
	int ShowRelList(void);
	int ShowSwtxList(void);
	int ShowRvList(void);
	void ResumeMdl(void);
	afx_msg void OnBnClickedSortBtn();
	afx_msg void OnBnClickedCalBtn();
	afx_msg void OnBnClickedRegBtn();
	virtual BOOL DestroyWindow();
	afx_msg void OnClose();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedResumeBtn();
	afx_msg void OnBnClose();
};
