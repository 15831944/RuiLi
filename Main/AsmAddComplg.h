#pragma once
#include "afxcmn.h"
#include "asmsearchpage.h"
#include "partmdlrecordset.h"
#include "afxwin.h"
#include "PicForDBDlg.h"
// CAsmAddComplg 对话框

class CAsmAddComplg : public CDialog
{
	DECLARE_DYNAMIC(CAsmAddComplg)

public:
	CAsmAddComplg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAsmAddComplg();

// 对话框数据
	enum { IDD = IDD_ASM_ADD_COMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    
	DECLARE_MESSAGE_MAP()
private:
	//工程图与零部件的二维对象关系
	typedef struct mdl_draw_item{
		CString   SolidFullName;
		ProSolid  solid;
		CString   DrawFullName;
		ProDrawing draw;
	}mdl_draw_item;
	CArray<mdl_draw_item,mdl_draw_item> m_MdlDrawItem;
public:
	CTreeCtrl m_Tree[4];
	CTabCtrl m_Tab;
	virtual BOOL OnInitDialog();
	CAsmSearchPage m_SearchPage;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	int m_selTab;

	// 零件模型数据库

	CPartMdlRecordSet m_pSet;

	CListCtrl m_List;
	CStatic m_Path;
//	afx_msg void OnNMClickPartTree(NMHDR *pNMHDR, LRESULT *pResult);
	long mdlSelectNumber;
	afx_msg void OnTvnSelchangedPartTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedAsmTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();
	ProMdl MdlRetrieve(int type, long mdlNum);
	BOOL AssembleComp(ProSolid solid,ProSolid comp);
	BOOL AssembleComp(ProSolid solid,long InsNum);
	BOOL AssembleComp(ProSolid solid,long InsNum,ProMdlType type);
	//BOOL AddPartMdlOtherInfo(ProMdl mdl,long PrtNum);
	BOOL AddPartMdlInfo(ProMdl mdl,long PrtNum,int type,mdl_draw_item * tempMdlDrawItem);
	BOOL CompRenameWithInsNum(long InsNum, ProMdl mdl);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	BOOL CancelAssmeblyOrFailure(ProSolid assmbly, ProAsmcomp * feature,long InsNum);
	//删除模型及其工程图,以及删除所在的目录
	BOOL DeleteSolidAndDrawing(ProSolid solid,bool bolDeleteDirectory);
	CListCtrl m_List2;

	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnPaint();
	CButton m_btnOpen;
	//装配零件模型
	void	AssemblePrtMdl();
	//装配组件模型
	void    AssembleAsmMdl();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CImageList m_ImageList;
    CPicForDBDlg pic_dlg;
	CStatic m_RectPic;
	afx_msg void OnAsmBom();
	afx_msg void OnAsmPrtList();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnTvnSelchangedPartinsTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedAsminsTree(NMHDR *pNMHDR, LRESULT *pResult);
};
