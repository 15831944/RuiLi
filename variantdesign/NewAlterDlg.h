#pragma once
#include "afxwin.h"
#include "listctrlex.h"
#include "DetailInfoClass.h"


// CNewAlterDlg 对话框

class CNewAlterDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewAlterDlg)

public:
	CNewAlterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewAlterDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NEWALTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_pic;
	CListCtrlEx m_variantdim;
	CString MdlNum;
	int picNum;
	IPicture*   p_Picture;
	//DetailInfoClass *info;
	int s;
protected:
	virtual void PreInitDialog();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonPre();
	afx_msg void OnBnClickedButtonNext();
public:
	BOOL DBOpenPic(BYTE *pBuffer,int nSize,IPicture ** pic);
	BOOL ShowPic(int num,int Number);
	afx_msg void OnBnClickedOk();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnQuerySeries();
	BOOL ExamTheDatas(void);
};
