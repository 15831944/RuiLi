#pragma once
#include "DetailInfoClass.h"


// CPrtNewDlg 对话框

class CPrtNewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPrtNewDlg)

public:
	CPrtNewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPrtNewDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PRTNEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMdl();
	afx_msg void OnBnClickedButtonSelprt();
	//afx_msg void OnBnClickedButtonSel();
	CString m_inscode;            //瑞立图号
	CString m_insname;            //名称
	CString m_mdlnum;             //模型号
	CString m_note;               //备注
	CString m_group;              //分类
	CString m_material;           //材料
	CString m_unit;               //单位
	CString m_filename;           //文件名
	CString m_place;              //保存位置
	CString m_designer;           //设计者
	CString m_pdmcode;            //PDM图号
	/////////////////////////
	DetailInfoClass *detailinfo;
	BOOL IsError;
	afx_msg void OnBnClickedButtonGrpfrm();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	BOOL GetPartModelInfo(CString MdlNum);
	CString m_oldfilename;
	CString m_olddrwname;
	afx_msg void OnMove(int x, int y);
	BOOL GetPartInfo(CString InsCode);
	
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
};
