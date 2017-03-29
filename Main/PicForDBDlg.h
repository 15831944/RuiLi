#pragma once
#include "afxwin.h"
#include "PicViewDlg.h"

// CPicForDBDlg 对话框

class CPicForDBDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicForDBDlg)

public:
	CPicForDBDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicForDBDlg();

// 对话框数据
	enum { IDD = IDD_PIC_FORM_DB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	typedef struct pic_item{
		int    setType;
		long   mdlNum;
		long   picNum;
	}pic_item;
	CArray<pic_item,pic_item> p_PicItemList;
	int				current_pic;
	CStatic m_pic;
	CButton m_btnPrePic;
	CButton m_btnNextPic;
	CButton m_viewPicInDlg;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	IPicture *		p_Picture;
	CPicViewDlg * pic_dlg;
	CDatabase * p_db;
	bool ShowPicItem(pic_item * temp);
	//获得模型所有的图片信息
	void GetMdlPicSet(int type,long mdlNum,CDatabase * p_db);//type=0 (零件模型) type=1 (组件模型)
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton3();
};
