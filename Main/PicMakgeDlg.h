#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ListDlg.h"


// CPicMakgeDlg 对话框

class CPicMakgeDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicMakgeDlg)

public:
	CPicMakgeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicMakgeDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	int m_flag;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton2();
	// 判断是否为第一点
	int m_nSetp;
	// 起始点X
	int m_nStartX;
	// 起始点Y
	int m_nStartY;
	// 终止点X
	int m_nEndX;
	// 终止点Y
	int m_nEndY;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnPaint();
	// 文件保存的路进
	CString csPath;
	// 保存的文件名
	CString fileName;
	CButton m_CreatButton;
	virtual BOOL OnInitDialog();
	// 图片宽度
	int m_Width;
	// 图片高度
	int m_Height;
	
	CButton m_SelectButton;
	// 设置存储路进
	void SetPath(CString filePath);
	CListCtrl m_List;
	afx_msg void OnPicEdit();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	// 选取过程中实时显示宽度
	int m_pic_width;
	// 选取过程中实时显示高度
	int m_pic_height;
	int FreshList(void);
	afx_msg void OnDelPic();
	afx_msg void OnPicView();
	afx_msg void OnClose();

	
	CSliderCtrl m_slidercrtrl;
	afx_msg void OnBiaozhu();
	//类表框，用于确定图片的尺寸规格
	CComboBox m_cobGuiGe;
	//自定义大学
	void TypeDIY(CPoint * point, UINT nFlags);   
	//制定大小
	void TypeBiaoZhun(CPoint * point, UINT nFlags,int iWidth, int iHeight);
	bool MakePic(CPoint * point, int iWidth, int iHeight);
	afx_msg void OnViewPicOfDrawing();
	//列表对话框
	CListDlg * list_dlg;
	//消息函数
	afx_msg LRESULT OnListDlgOk(WPARAM wParam,LPARAM lParam);
};
