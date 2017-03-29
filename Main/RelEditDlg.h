#pragma once
#include "afxcmn.h"
#include "relinfoeditbox.h"
#include "afxwin.h"
#include "mdlrelvisitdlg.h"


// CRelEditDlg 对话框

class CRelEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CRelEditDlg)

public:
	CRelEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRelEditDlg();

// 对话框数据
	enum { IDD = IDD_REL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	ProMdl mdl;
public:
	void SetMdl(ProMdl owner);
	CListCtrl m_List;
	CString m_RelEdit;
	// 关系式信息
	CString         m_RelNoteEdit;
	virtual         BOOL OnInitDialog();
	void            ShowRel(void);
	CRelInfoEditBox info_edit_dlg;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedAddSymbol();
	afx_msg void OnBnClickedMinusSymbol();
	afx_msg void OnBnClickedMultiplySymbol();
	afx_msg void OnBnClickedDidSymbol();
	afx_msg void OnBnClickedBracketSymbol();
	afx_msg void OnBnClickedQuealSymbol();
	CEdit m_RelEditCtrl;
	afx_msg void OnBnClickedButton13();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	//CMdlRelVisitDlg mdl_rel_vis_dlg;
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton16();
	// 用于检查没个指定的关系式是否正确
	bool CheckRel(CString rel);
	afx_msg void OnBnClickedQuealSymbol2();
	afx_msg void OnBnClickedButton2();
	// 检查符号是否正确
	CString CheckSymbol(CString rel);
	// 检查尺寸，参数是否存在
	CString CheckDimAndPara(CString rel);
	// 输入一条关系式，分离出尺寸和参数
	void  RelFenli(CString str,CStringArray * CanshuName);
	CString CompareCsarry1ToCsarry2(CStringArray * Csarry1, CStringArray * Csarry2);
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton17();
	// 被选择的关系式编号，初始为－1
	int iSelRelIndex;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};
