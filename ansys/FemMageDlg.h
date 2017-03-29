#pragma once
#include "afxcmn.h"
#include "MyDatabase.h"
#include "afxwin.h"
#include "FtpFile.h"
#include "myproe.h"


// CFemMageDlg �Ի���

class CFemMageDlg : public CDialog
{
	DECLARE_DYNAMIC(CFemMageDlg)

public:
	CFemMageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFemMageDlg();

// �Ի�������
	enum { IDD = ggggggg};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CFtpFile ftpfile;
	CArray<ModelStruct,ModelStruct> ModelList;
	CMyDatabase m_db;
	HTREEITEM m_hitem;
	CImageList m_ImageList;
	int m_FatherNum;
	void AddTree();
	void AddTree(HTREEITEM root,CArray<ModelStruct,ModelStruct> * List,int Fathernum);
public:
	
	CTreeCtrl m_Tree;
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	afx_msg void OnClose();
	afx_msg void OnNMClickTreeOpen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};



