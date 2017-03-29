#pragma once
#include "afxcmn.h"
#include "DetailInfoClass.h"


// CProjectDlg 对话框

class CProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CProjectDlg)

public:
	CProjectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProjectDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROJ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_projdetail;
	CString UserName;
	virtual BOOL OnInitDialog();
	BOOL m_all;
	CListCtrl m_task;
	CString ProjectID;
	int TaskID;
	int ElementID;
	DetailInfoClass *info;
	CString strLocalDirectoryPath;
	afx_msg void OnNMClickListProdetail(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListTask(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_note;
	afx_msg void OnBnClickedOk();
	BOOL OpenMdl(DetailInfoClass *info);       //打开已有模型
	BOOL OpenExistPart(DetailInfoClass *info); //打开已存在文件
	BOOL CreateExistPart(DetailInfoClass *info);  //新建文件
	BOOL GetPartModelInfo(CString MduNum);
	BOOL GetPartInfo(CString InsCode);
	afx_msg void OnBnClickedButtonFinish();
	BOOL GetTask(CString ProjectID);
	BOOL GetAsmModelInfo(CString MdlNum);
	afx_msg void OnBnClickedButtonReject();
	afx_msg void OnBnClickedButtonAccept();
};
