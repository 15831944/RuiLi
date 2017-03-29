#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "partmdlrecordset.h"


// CMdlOkDlg 对话框

class CMdlOkDlg : public CDialog
{
	DECLARE_DYNAMIC(CMdlOkDlg)

public:
	CMdlOkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMdlOkDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ProSolid  part_solid;
	CProgressCtrl m_PrgCtrl;
	CEdit m_TextEdit;
	// 表示进程
	int m_progress;
	afx_msg void OnBnClickedOk();
	CPartMdlRecordSet m_pSet;
	CDatabase      *   p_db;
	bool CheckMdlInfo(ProSolid  solid);
	bool InsertMdlPropToDb(ProSolid  solid,long * MdlNumber,CString * strDir);
	bool InsertMdlVariantTableToDb(ProSolid  solid,long MdlNumber);
	bool InsertMdlRelToDb(ProSolid  solid, long MdlNumber);
	bool InsertMdlPerfToDb(ProSolid  solid, long MdlNumber);
	bool InsertMdlRvToDb(ProSolid  solid, long MdlNumber);
	bool InsertMdlPicToDb(ProSolid solid, long MdlNumber);
	//插入一个默认的实例
    bool InsertDefaultMdlInst(long MdlNumber);
	bool CopyFilesToFtpServer(long MdlNumber, CString localDir,CString serDir);
	bool GetRvDimPara(ProSolid  solid);
	virtual BOOL OnInitDialog();
	int m_prgpos;
	
	CButton m_OKButton;
	void SetDbConnet(CDatabase * db);
	void SetProg(int cur,CString message);
	//更新模型  
	bool UpdateMdl(ProSolid solid,long MdlNumber,CDatabase * db);
	//状态用于确定是否需要设置滚动条，true表示需要
	bool bState;
	//bool InsertMdlDrawingInfoToDb(ProSolid solid,long MdlNumber);
	afx_msg void OnMove(int x, int y);
};
