#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "partmdlrecordset.h"


// CMdlOkDlg �Ի���

class CMdlOkDlg : public CDialog
{
	DECLARE_DYNAMIC(CMdlOkDlg)

public:
	CMdlOkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMdlOkDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	ProSolid  part_solid;
	CProgressCtrl m_PrgCtrl;
	CEdit m_TextEdit;
	// ��ʾ����
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
	//����һ��Ĭ�ϵ�ʵ��
    bool InsertDefaultMdlInst(long MdlNumber);
	bool CopyFilesToFtpServer(long MdlNumber, CString localDir,CString serDir);
	bool GetRvDimPara(ProSolid  solid);
	virtual BOOL OnInitDialog();
	int m_prgpos;
	
	CButton m_OKButton;
	void SetDbConnet(CDatabase * db);
	void SetProg(int cur,CString message);
	//����ģ��  
	bool UpdateMdl(ProSolid solid,long MdlNumber,CDatabase * db);
	//״̬����ȷ���Ƿ���Ҫ���ù�������true��ʾ��Ҫ
	bool bState;
	//bool InsertMdlDrawingInfoToDb(ProSolid solid,long MdlNumber);
	afx_msg void OnMove(int x, int y);
};
