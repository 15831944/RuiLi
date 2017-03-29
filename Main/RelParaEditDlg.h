#pragma once
#include <ProFeature.h>
#include "afxcmn.h"
#include "afxwin.h"
#include "Main.h"
#include "paraoperadlg.h"
//#include "LstCrlOfSXTXTable.h"
#include "releditdlg.h"
#include "PartSWTXDlg.h"

// CRelParaEditDlg �Ի���

class CRelParaEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CRelParaEditDlg)

public:
	CRelParaEditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRelParaEditDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CArray<ProFeature,ProFeature> FeatureList;//����ȫ�ֺ���
	CArray<ProDimension,ProDimension> Dimensionlist;//�ߴ�ȫ�ֺ���
	CArray<ProParameter,ProParameter> Parameterlist;
	void pGetFeats(ProSolid part,CArray<ProFeature,ProFeature> * FeatureList);
	CTreeCtrl m_Tree;
	CImageList imageList;
	virtual BOOL OnInitDialog();
	void prtShowTree(ProSolid solid);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void ConverthItemToList(HTREEITEM hItem);
	CListCtrl m_List;
	CListCtrl m_List2;
	//CLstCrlOfSXTXTable m_List2;
	//��ϵ�༭���õ��Ŀؼ�
    CListCtrl m_List3;
	//STATIC m_RelEditStaticText;
	
protected:
//	virtual void OnOK();
	int radioFalg;//�����жϵ�ѡ���ĸ���ѡ��
    //int XWTXTable_IS_OPEN;     //�����ж��������Ա��Ƿ��Ѿ�����
    //int GXS_IS_OPEN;               //����ȷ����ϵʽ���Ƿ��
public:
//	virtual BOOL DestroyWindow();
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//afx_msg void OnClose();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);

	//afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedCom1();
	afx_msg void OnBnClickedCom2();
	afx_msg void OnBnClickedCom3();
	afx_msg void OnBnClickedCom4();
	//CButton m_radiobutton1;
	void AddDimToSWTXList(void);
	void AddParaToSWTXList(void);
	void AddFeatToSWTXList(void);
	void AddCompToSWTXList(void);
	void ConverthItemOfParaToList(void);
	void FreshDim(void);
	afx_msg void OnBnClickedButton4();
	CParaOperaDlg m_ParaOperaDlg;
	CButton		m_SWTXButtong;
	CButton m_RELATIONButton;
	CButton m_DimDisplayMode;
	afx_msg void OnBnClickedCheck1();
	CButton m_FeatDisplayMode;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	//afx_msg void On12045();
	// ����������Ա��е�ע�ͽ��б༭
	//CEdit m_SWTXB_Note_Edit;
	//void OnNMClickList2(void);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();


	//CRelEditDlg * dlg_rel_edit;
	//CPartSWTXDlg  dlg_swtx;
	CRelEditDlg   rel_dlg;
	CPartSWTXDlg  swtx_dlg;
	afx_msg void On12058();
	afx_msg void On12059();
	afx_msg void OnDimEditDlg();
	//afx_msg void OnFreshSwtx();

	//afx_msg void OnSWTXNameEdit();
	//afx_msg void OnMenu12082();
	ProSolid current_solid;
	CEdit m_PathEdit;
	int m_RadioButtonValue;
	afx_msg void OnBnClickedCheck2();
};
