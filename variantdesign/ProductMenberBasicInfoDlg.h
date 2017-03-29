#pragma once
#include "afxcmn.h"
#include "ProSolid.h"
#include "afxwin.h"
#include "DetailInfoClass.h"
#include "ListCtrlEx.h"


// CProductMenberBasicInfoDlg �Ի���

class CProductMenberBasicInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CProductMenberBasicInfoDlg)

public:
	CProductMenberBasicInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProductMenberBasicInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_BASICINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	BOOL IsProduct;					 //�Ƿ������
	ProSolid ProductSolid;
	CStringArray ValueArray;		 //����ֵ������
	BOOL IsSelect;              //������ѡ��ť
	CString Num;
	BOOL IsInsert;				//�Ƿ����²����Ʒ
	BOOL IsNew;
public:

	virtual BOOL OnInitDialog();
	CListCtrlEx m_product;
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButtonSel();
	CString m_inscode;
	CString m_insname;
	CString m_mdlnum;
	CString m_designer;
	CString m_note;
	CString m_group;
	CString m_material;
	CString m_unit;
	DetailInfoClass *info;
	int s;
	CString Type;
	afx_msg void OnBnClickedButtonGrpfrm();
	afx_msg void OnBnClickedButtonMaterial();
	afx_msg void OnBnClickedButtonUnit();
	CString m_filename;
	CComboBox m_designstate;
	CString m_place;
	CString m_drwplace;
	afx_msg void OnBnClickedButtonSolidlocal();
	afx_msg void OnBnClickedButtonDrwlocal();
	afx_msg void OnBnClickedButtonSolidserver();
	afx_msg void OnBnClickedButtonDrwserver();
	BOOL InsertNewProductMenberToDatabase(int s,CString Type);
	afx_msg void OnBnClickedButtonde();
};
