#pragma once
#include "afxcmn.h"
#include "part_temp_mdl_info.h"
#include "afxwin.h"


// CTempWorkPlaceDlg 对话框

class CTempWorkPlaceDlg : public CDialog
{
	DECLARE_DYNAMIC(CTempWorkPlaceDlg)

public:
	CTempWorkPlaceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTempWorkPlaceDlg();

// 对话框数据
	enum { IDD = IDD_TEMP_WORKPLACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree1;
	CTreeCtrl m_Tree2;
	CTabCtrl m_Tab;
	virtual BOOL OnInitDialog();
	int m_iSelTab;
	CPart_Temp_Mdl_Info m_pSet;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_List;
	CImageList m_ImgForList;
	CImageList m_ImgForList2;
	CImageList m_ImgForList3;
	void SetList(HTREEITEM hItem);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_Path;
	void SetPath(CTreeCtrl * p_Tree, HTREEITEM hItem);
	BOOL OpenPart(void);
	afx_msg void OnNewWorkPlace();
	afx_msg void OnDelWorkPlace();
	afx_msg void OnRename();
	afx_msg void OnOpenMdl();
	afx_msg void OnDelMdl();
	afx_msg void OnProp();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnMenu12160();
	BOOL SavePart(HTREEITEM hItem,long fatherNum);
	HTREEITEM m_CurrentTvItem;
	HTREEITEM m_CurrentTvItemForMenu;
	int NewPartWorkPlace(void);
	afx_msg void OnTvnBegindragTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult);
	int NewAsmWorkPlace(void);
	afx_msg void OnTvnBeginlabeleditTree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditTree2(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL SaveAsm(HTREEITEM hItem, long  fatherNum);
	int SetMode(int mode);
	afx_msg void OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult);
	int dlgMode;
	BOOL OpenPartTempMdl(long part_num);
	BOOL OpenAsmTempMdl(long asm_num);
	void AddPartParaRelPic(CString localDir, ProMdl mdl,ProMdl drawing,long part_num);
	void AddAsmParaRelPic(CString localDir, ProMdl mdl,long asm_num);
	afx_msg void OnOpenWorkPlace();
	afx_msg void OnNew();
	afx_msg void OnRenameWorkPlace();
	int GetStatus(void);
	HTREEITEM ConverListItemTohItem(long lItem);
	afx_msg void OnMenu12167();
	BOOL  DelTempPartMdl(CDatabase * p_db,long num);
	BOOL  DelTempAsmMdl(CDatabase * p_db,long num);
};
