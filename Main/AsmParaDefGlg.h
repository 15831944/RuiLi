#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CAsmParaDefGlg �Ի���

class CAsmParaDefDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CAsmParaDefDlg2)

public:
	CAsmParaDefDlg2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAsmParaDefDlg2();

// �Ի�������
	enum { IDD = IDD_ASM_DEF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    typedef struct tree_item {
		int        type;          //-1(����),0(���) 1(���) 2(�������Ա�) 3(װ��Լ��)
		ProSolid   p_solid;       //Ԫ���ľ��
		ProSolid   f_solid;       //�ϲ���
		ProFeature feature;       //Ԫ������Ϊ�����ľ��
    };
	CArray<tree_item,tree_item&> p_treeitemlist;       //Ԫ����Ϣ�б���б�
	typedef struct list_item{
		int          type;  //TYPE_DIM TYPE_PARA
		CStringA     name;
		CStringA     note;
		ProParameter Para;
		ProDimension Dim;
	};
	CArray<list_item,list_item&> p_listitem;
	CArray<list_item,list_item&> p_listitem2;
	int   m_flag;   //״̬��־��0 ��ʾ�����������Ա� 1 ��ʾ����װ��ߴ� -1û��
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree;
	CListCtrl m_List1;
	CListCtrl m_List2;
	CImageList m_ImageList;
	CStatic m_Path;
	ProSolid asm_solid;
	virtual BOOL OnInitDialog();
	void GetAsmcompConstraint(HTREEITEM hItem,ProAsmcomp * p_component);
	void ShowModelTree(ProSolid solid,HTREEITEM hItem);
	bool IsSolidFromMdl(ProSolid solid);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	//���б�������ʾģ�͵��������Ա�
	void ShowSolidSWTX(ProSolid solid);
	void ShowPartSWTX(ProSolid part);
	//void ShowAsmSWTX(ProSolid asm,CListCtrl & p_List);
	void ShowAsmSWTXList1(ProSolid ,CListCtrl &);
	void ShowAsmSWTXList2(ProSolid ,CListCtrl &);
	//���б�������ʾ�����ĳߴ�
	CArray<ProDimension,ProDimension> Dimensionlist;//�����ߴ��б�
	void ShowFeatDim(ProFeature feature);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnEditDim();
	//��ʾģ�Ͷ���Ĳ���
	void ShowAsmSolidPara(ProSolid asm1);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFreshList();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnMove(int x, int y);
};
