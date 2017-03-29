#pragma once
#include "afxcmn.h"
#include "ComboListCtrl.h"

// CTestRelDlg 对话框

class CTestRelDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestRelDlg)

public:
	CTestRelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestRelDlg();

// 对话框数据
	enum { IDD = IDD_TEST_VARIANT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CArray<CRel,CRel&>	p_RelListUnSorted;  //没有排序的关系式集
	CArray<CRel,CRel&>	p_RelListSorted;    //排序之后的关系式集
	void GetSolidRelSetAndSwtxSet();
	//表达关系式中尺寸和参数项以及事物特性表中的尺寸和参数项
	struct rel_swtx_item {
		int            type;     //0 尺寸 1 参数
		CString        name;     //名称用于事务特性表
		ProDimension   dim;      //尺寸
		ProParameter   para;	 //参数
		double         value1;	 //原来值
		double         value2;	 //现有值
	};
    CArray<rel_swtx_item,rel_swtx_item&>   p_Swtx; 
	CArray<rel_swtx_item,rel_swtx_item&>   p_Rv; 
	//计算关系的值，输入关系式(数字表达式)
	BOOL  Calculate(CString,double *);
	//对关系式进行排序
	BOOL  SortRelSet();
	BOOL  SortRelSet(CArray<CRel,CRel&>	* p_InRelList
		,CArray<rel_swtx_item,rel_swtx_item&>  *p_MainParaList,int * index);
	//模型重生
	BOOL  Regeneralte();
	BOOL  SetDimParamterValue(ProDimension * dim,ProParameter * para,double value);
public:
	//模型
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
