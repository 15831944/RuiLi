#pragma once
#include "afxcmn.h"
#include "ProSolid.h"
#include "afxtempl.h"
#include "ProMdl.h"
#include "afxwin.h"
//#include "Picture.h"
#include "ListCtrlEx.h"


// CModelQueryDlg �Ի���

class CModelQueryDlg : public CDialog
{
	DECLARE_DYNAMIC(CModelQueryDlg)

public:
	CModelQueryDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModelQueryDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MODELQUERY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnMove(int x, int y);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonModeldown();
	afx_msg void OnNMClickListModel(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnNMClickTreePartmodel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonNext();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	BOOL ObtianPartModelAfter(int num);

	afx_msg void OnNMDblclkListModel(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL DBOpenPic(BYTE *pBuffer,int nSize,IPicture ** pic);
	BOOL ShowPic(int num,int Number);
	CStatic m_pic;
	ProSolid ProductSolid;
	int picNum;
	CString MdlNum;
	IPicture*   p_Picture;
	CString Name,FileDirectory;
	CListCtrl m_modelresult;
	CTreeCtrl m_tree;
	

	//�ж��Ƿ�ѡ����һ��ģ��
	BOOL IsSelect;
	CString PrtNum;
	afx_msg void OnNMRclickListModel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPreview2d();
	afx_msg void OnPreview3d();
};
