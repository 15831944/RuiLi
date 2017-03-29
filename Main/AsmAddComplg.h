#pragma once
#include "afxcmn.h"
#include "asmsearchpage.h"
#include "partmdlrecordset.h"
#include "afxwin.h"
#include "PicForDBDlg.h"
// CAsmAddComplg �Ի���

class CAsmAddComplg : public CDialog
{
	DECLARE_DYNAMIC(CAsmAddComplg)

public:
	CAsmAddComplg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAsmAddComplg();

// �Ի�������
	enum { IDD = IDD_ASM_ADD_COMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    
	DECLARE_MESSAGE_MAP()
private:
	//����ͼ���㲿���Ķ�ά�����ϵ
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

	// ���ģ�����ݿ�

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
	//ɾ��ģ�ͼ��乤��ͼ,�Լ�ɾ�����ڵ�Ŀ¼
	BOOL DeleteSolidAndDrawing(ProSolid solid,bool bolDeleteDirectory);
	CListCtrl m_List2;

	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnPaint();
	CButton m_btnOpen;
	//װ�����ģ��
	void	AssemblePrtMdl();
	//װ�����ģ��
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
