#pragma once
#include "afxcmn.h"
#include "relinfoeditbox.h"
#include "afxwin.h"
#include "mdlrelvisitdlg.h"


// CRelEditDlg �Ի���

class CRelEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CRelEditDlg)

public:
	CRelEditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRelEditDlg();

// �Ի�������
	enum { IDD = IDD_REL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	ProMdl mdl;
public:
	void SetMdl(ProMdl owner);
	CListCtrl m_List;
	CString m_RelEdit;
	// ��ϵʽ��Ϣ
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
	// ���ڼ��û��ָ���Ĺ�ϵʽ�Ƿ���ȷ
	bool CheckRel(CString rel);
	afx_msg void OnBnClickedQuealSymbol2();
	afx_msg void OnBnClickedButton2();
	// �������Ƿ���ȷ
	CString CheckSymbol(CString rel);
	// ���ߴ磬�����Ƿ����
	CString CheckDimAndPara(CString rel);
	// ����һ����ϵʽ��������ߴ�Ͳ���
	void  RelFenli(CString str,CStringArray * CanshuName);
	CString CompareCsarry1ToCsarry2(CStringArray * Csarry1, CStringArray * Csarry2);
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton17();
	// ��ѡ��Ĺ�ϵʽ��ţ���ʼΪ��1
	int iSelRelIndex;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};
