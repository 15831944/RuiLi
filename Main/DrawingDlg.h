#pragma once
#include "afxcmn.h"
#include "ListDlg.h"
#include "afxwin.h"


// CDrawingDlg 对话框

class CDrawingDlg : public CDialog
{
	DECLARE_DYNAMIC(CDrawingDlg)

public:
	CDrawingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDrawingDlg();

// 对话框数据
	enum { IDD = IDD_DRAWING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ProSolid    solid;
	ProDrawing  select_draw;
	CListDlg   * dlg;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnListDlgOk(WPARAM wParam,LPARAM lParam);
	CArray<ProDrawing,ProDrawing> p_DrawingList;
	CButton m_SessionButton;
	void GetDrawingSolidName(ProDrawing hDraw, CString * str);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnClose();
	CEdit m_DrawFileName;
	CEdit m_DrawPath;
	CButton m_ShowButton;
	CButton m_DelButton;
	void SetButtonAndText();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnMove(int x, int y);
};
