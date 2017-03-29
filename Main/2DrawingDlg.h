#pragma once
#include "afxcmn.h"
//#include "myproe.h"
#include <afxtempl.h>
#include "afxwin.h"
#include "Resource.h"

typedef struct param2d {
	CString name;
	CString text;
	CString value;
	CString type;
} PARAM2D;
// C2DrawingDlg 对话框

class C2DrawingDlg : public CDialog
{
	DECLARE_DYNAMIC(C2DrawingDlg)

public:
	C2DrawingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~C2DrawingDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_2DDRAW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_state;///0:建模,1:变型设计
	int drawflag;
	CArray<PARAM2D,PARAM2D> paramArray;
	CImageList m_ImageList;
	BOOL isOpened;
	ProSolid solid;
	ProSolid rootSolid;
	ProDrawing drawing;
	int sheet;
	BOOL isAssembly;
	ProModelitem owner;
	ProAsmcomppath comppath;
	void Begin(BOOL flag=FALSE);
	void ClickTree(HTREEITEM hItem);
	int AddSolidToTree(ProSolid * pSolid,HTREEITEM * pItem);
	void AddParamToList(ProSolid * theSolid);
	BOOL TablecellSet(CString theText);
	BOOL TablecellGet(CString * theText);
	void exit();
	BOOL SetDtl(ProDrawing * drawing);
	BOOL ShowMyDialog(int state);
	BOOL SetStringToTableCell(ProDwgtable  *table,int col,int row,CString text);
	BOOL SetHeightToTableCell(ProDwgtable *table,int col,int row,double height);
	BOOL GetTableCellText(ProDwgtable *table,int col,int row,CString * theText);
	BOOL GetParam(ProModelitem * theOwner,CString strName,CString * strValue);
	BOOL GetScaleD(double * dscale);
	BOOL GetScaleS(CString * strscale);
	BOOL SetScaleD(double dscale);
	BOOL SetScaleS(CString strscale);
	void SetParam(ProModelitem * theOwner,CString strName,CString strValue="",BOOL isOver=TRUE,CString strType="string");
	void AddParam(ProModelitem * theOwner,CString strName,CString strValue="",BOOL isOver=TRUE,CString strType="string");
	int LoadModel(CString path,ProMdl * pSolid);
	int  ShowNewWindow(ProMdl * pSolid);
	int CreateFromTemplate(CString strName ,ProMdl solid_mdl,CString templtName,ProDrawing * createdDrawing);
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_ModelTree;
	CListCtrl m_List;
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnNMClickTreeParam(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickTreeParam(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButton2dcreate();
	CComboBox m_combo;
	afx_msg void OnBnClickedButtonTablecell();
	afx_msg void OnNMDblclkListParame(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDesignate();
	afx_msg void OnBnClickedButtonReplacemoban();
	CString m_Name;
	CString m_Label;
	afx_msg void OnBnClickedMyExport();
	afx_msg void OnBnClickedMyButtonImportfromtable();
	afx_msg void OnBnClickedOk();
};