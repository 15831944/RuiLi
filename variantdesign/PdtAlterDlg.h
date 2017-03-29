#pragma once
#include "afxwin.h"
#include "listctrlex.h"
#include "DetailInfoClass.h"


// CPdtAlterDlg 对话框

class CPdtAlterDlg : public CDialog
{
	DECLARE_DYNAMIC(CPdtAlterDlg)

public:
	CPdtAlterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPdtAlterDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PDTALTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_pic;
	CListCtrlEx m_listdim;
	int Pos;
	int picNum;
	CString  MdlNum;
	BOOL IsLocal;
	IPicture*   p_Picture;
	afx_msg void OnBnClickedButtonPre();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnPaint();
	//afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	BOOL DBOpenPic(BYTE *pBuffer,int nSize,IPicture ** pic);
	BOOL ShowPic(int num,int Number);
	afx_msg void OnMove(int x, int y);
};
