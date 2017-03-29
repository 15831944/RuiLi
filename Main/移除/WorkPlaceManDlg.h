#pragma once
#include "afxcmn.h"
#include "afxdb.h"
#include "Part_Temp_Mdl_Info.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "atlcomtime.h"
#include "ComboListCtrl.h"
// CWorkPlaceManDlg �Ի���

class CWorkPlaceManDlg : public CDialog
{
	DECLARE_DYNAMIC(CWorkPlaceManDlg)

public:
	CWorkPlaceManDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWorkPlaceManDlg();

// �Ի�������
	enum { IDD = IDD_TEMP_MAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton3();
	CTreeCtrl m_Tree;
	CImageList m_ImageList;
	void InitTreeCtrl(void);
	CPart_Temp_Mdl_Info m_pSet;
	virtual BOOL OnInitDialog();
	ProSolid   solid;
	//CDatabase m_db;
	void AddSubPartItem(HTREEITEM item);
	bool CreateTempDir(CString * csPathName);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnNewWorkSpace();
	HTREEITEM GethItem(long data);
	afx_msg void OnDelWorkSpace();
	afx_msg void OnTvnBeginlabeleditTree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditTree2(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void OnOK();
public:
	afx_msg void OnRename();
	afx_msg void OnTvnBegindragTree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBeginrdragTree2(NMHDR *pNMHDR, LRESULT *pResult);
	// ����ѡ��ģ�͵Ĺ�������
	CString m_WorkSpace;
	long m_WorkSpaceNum;
	HTREEITEM fatherItem;
	afx_msg void OnBnClickedButton2();
	
	afx_msg void OnSelectWorkSapce();
	//����ģ�Ϳؼ�
	CButton m_SaveButton;
	//ģ����Ϣ����ؼ�
	CButton m_InfoDefButton;
	// ������ģ�Ͱ�ť�ؼ�
	CButton m_ToBeMdlButton;
	// ��ѡ�������ģ������
	CButton m_ReqName;
	// ��ѡ������������Ա
	CButton m_ReqDesigner;
	// ��ѡ�򣬱�������
	CButton m_ReqSaveTime;
	// ��ѡ����Ʋ���
	CButton m_ReqDept;
	// ��ѡ�򣬱�ע��Ϣ
	CButton m_ReqNote;
	CEdit m_ComNameEdit;
	CString m_ComNameEditValue;
	CEdit m_ComDesingerEdit;
	CString m_ComDesingerEditValue;
	CDateTimeCtrl m_ComDataTime;
	COleDateTime m_ComDataTimeValue;
	CEdit m_ComDeptEdit;
	CString m_ComDeptEditValue;
	CEdit m_ComNoteEdit;
	CString m_ComNoteEditValue;
	CComboBox m_ComboBoxCtrl;
	CComboListCtrl m_List;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
	typedef struct requery_item {
		long        number;   //ģ�͵ı��
		CString     name;     //ģ�͵�����
		CString     type;     //ģ�͵�����,��������
		CTime       datetime; //ģ�ͱ����ʱ��
		CString     filename; //ģ�͵��ļ���
		CString     person;   //�����Ա
		CString     dept;     //����
		CString     note;
	}RequeryItem;
	CArray<RequeryItem,RequeryItem> p_ReqList;

	void ShowListWithProp(void);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton6();
	// ѡ�е�ģ�ͱ��
	int selectNumber;
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	// �򿪰�ť�ؼ�
	CButton m_OpenButton;
	void AddMdlParaRelPic(CString localDir,ProMdl mdl);
	bool CheckMdl(CString fileName);
	afx_msg void On12120();
	afx_msg void OnDelMdl();
	int m_ViewType;
	// �鿴��ť
	CButton m_ViewButton;
	CString GetSubType(CString subtype);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnSeeMdlPropDlg();
};
