#pragma once
#include "autovuexctrl1.h"
#include "ProgressDlg.h"


// CPreViewDlg �Ի���

class CPreViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreViewDlg)

public:
	CPreViewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPreViewDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_Initflag;
	int m_ModalFlag;//1,���ģ�� 2�����ʵ�� 3����Ʒģ�� 4����Ʒʵ��
	int m_2DFlag;//0,û��2Dͼ��1 ��2Dͼ
	CString m_strID;///ģ��id,ʵ��id�ȡ�
	CString m_TempPath;

	CString m_LocalSolidPath,m_LocalDrawingPath;
	CString m_ProductLocalSolidPath,m_ProductLocalDrawingPath;
	CString m_Name;

	CProgressDlg * m_pDlg;

	int m_EndFlag;
public:
	int DeleteFolderAllFile(CString folder);
	int GetPartModal();
	int GetPartIns();
	int GetProductModal();
	int GetProductIns();

	int NGetProductModal();
	int NGetProductIns();
	


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CAutovuexctrl1 m_AutoVue;
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
};
