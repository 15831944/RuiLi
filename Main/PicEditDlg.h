#pragma once
//#include "picshow.h"
#include "picture.h"
#include "afxwin.h"

// CPicEditDlg 对话框

class CPicEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicEditDlg)

public:
	CPicEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicEditDlg();

// 对话框数据
	enum { IDD = IDD_PIC_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic    m_pic;
	bool SetFile(CString strFullFileName);
protected:
	CString    m_FullFileName;
	IPicture *	pIpicture;

	typedef enum bz_type{
		PTOL_UNUSE=-1,
		PTOL_DBL=1,
		PTOL_PTL=2,
		PTOL_TEXT=3
	};
	typedef  struct  line_item{
		bz_type   type;
		CPoint    pt_first;     //开始点
		CPoint    pt_second;    //第二点
		CPoint    pt_third;     //第三个点
		CRect     rect;
		CString   text;
		//CEdit *  p_edit;       //编辑框
	};
	bz_type mode;             //当前的模式
	int     m_ClickNum;
	//CPoint  pt_mouse0;
	//CPoint  pt_mouse1;
	//CPoint  pt_mouse2;
    CPoint    pt_temp1;//记录上一点
	CPoint    pt_temp2;//记录下一点
	CArray<line_item,line_item> p_LineItemList;
	line_item					temp_item;
	//显示各个项
	void ShowDBL_TEMP(line_item *	p_LineItem);
	void ShowPTL_TEMP(line_item *	p_LineItem);
	void ShowText_TEMP(line_item *	p_LineItem);
	//点击左键时候的处理函数
	void ShowDBL(CPoint * point);
	void ShowPTL(CPoint * point);
	void ShowText(CPoint * point);
	//鼠标移动过程中的处理函数
	void ShowDBL_OnMouseMove(CPoint * point);
	void ShowPTL_OnMouseMove(CPoint * point);
	void ShowText_OnMouseMove(CPoint * point);
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	CComboBox m_comb1;
	CComboBox m_comb2;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	int TempDraw(line_item * p_item);
	int EditTextBox(line_item * p_LineItem);
protected:
	virtual void OnOK();
	COLORREF m_lineColor;
	int      m_lineWidth;
	int      m_textHeight;
	int      m_textWidth;
public:
	CEdit m_edit;
	int InitParas(void);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	CComboBox m_comLineWidth;
	afx_msg void OnCbnSelchangeCombo3();
//	afx_msg void OnEnSetfocusEdit2();
//	afx_msg void OnEnKillfocusEdit2();
//	afx_msg void OnEnSetfocusEdit2();
	afx_msg void OnEnSetfocusEdit2();
	int ShowComb2(void);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnMove(int x, int y);
};
