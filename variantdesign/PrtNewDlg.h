#pragma once
#include "DetailInfoClass.h"


// CPrtNewDlg �Ի���

class CPrtNewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPrtNewDlg)

public:
	CPrtNewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPrtNewDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PRTNEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMdl();
	afx_msg void OnBnClickedButtonSelprt();
	//afx_msg void OnBnClickedButtonSel();
	CString m_inscode;            //����ͼ��
	CString m_insname;            //����
	CString m_mdlnum;             //ģ�ͺ�
	CString m_note;               //��ע
	CString m_group;              //����
	CString m_material;           //����
	CString m_unit;               //��λ
	CString m_filename;           //�ļ���
	CString m_place;              //����λ��
	CString m_designer;           //�����
	CString m_pdmcode;            //PDMͼ��
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
