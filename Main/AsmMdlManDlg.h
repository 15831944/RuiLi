#pragma once
#include "asminfodefpage.h"
#include "asmmdltreepage.h"
#include "asmswtxpage.h"
#include "asmperfpage.h"
#include "asmrelpage.h"
#include "asmpicpage.h"
#include "afxcmn.h"
#include "NAsmType.h"
#include "FtpFile.h"
#include "AsmNMdlTreePage.h"
// CAsmMdlManDlg 对话框

class CAsmMdlManDlg : public CDialog
{
	DECLARE_DYNAMIC(CAsmMdlManDlg)

public:
	CAsmMdlManDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAsmMdlManDlg();

// 对话框数据
	enum { IDD = IDD_ASM_MODEL_MAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CPropertySheet m_sheet;
	CAsmInfoDefPage m_page1_Info;
	/*CAsmMdlTreePage m_page2_Tree;*/
	CAsmNMdlTreePage m_page2_Tree;
	CAsmSWTXPage m_page3_Swtx;
	CAsmPerfPage m_page4_Perf;
	CAsmRelPage m_page5_Rel;
	CAsmPicPage m_page6_Pic;
	void SetSheetPage(void);
	CTreeCtrl m_Tree;
	CImageList 	m_ImageList;
	virtual BOOL OnInitDialog();
	// 显示模型管理
	CNAsmType m_pSet;
	ProSolid mdl;
	afx_msg void OnNewType();
	afx_msg void OnSelType();
	afx_msg void OnDelType();
	afx_msg void OnDelMdl();
	afx_msg void OnMdlSeries();
	afx_msg void OnFresh();
	afx_msg void OnViewProp();
	CFtpFile m_ftp;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnBnClicked1005();
	afx_msg LRESULT OnListDlgOk(WPARAM wParam,LPARAM lParam);
	BOOL GetSWTEItem(void);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnUpdateMdl();
	afx_msg void OnMove(int x, int y);
	int getRights(CString strAuthor);
	BOOL IsHasRight(CString theRight);
};
