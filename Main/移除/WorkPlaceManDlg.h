#pragma once
#include "afxcmn.h"
#include "afxdb.h"
#include "Part_Temp_Mdl_Info.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "atlcomtime.h"
#include "ComboListCtrl.h"
// CWorkPlaceManDlg 对话框

class CWorkPlaceManDlg : public CDialog
{
	DECLARE_DYNAMIC(CWorkPlaceManDlg)

public:
	CWorkPlaceManDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWorkPlaceManDlg();

// 对话框数据
	enum { IDD = IDD_TEMP_MAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	// 用于选择模型的工作区间
	CString m_WorkSpace;
	long m_WorkSpaceNum;
	HTREEITEM fatherItem;
	afx_msg void OnBnClickedButton2();
	
	afx_msg void OnSelectWorkSapce();
	//保存模型控件
	CButton m_SaveButton;
	//模型信息定义控件
	CButton m_InfoDefButton;
	// 生成主模型按钮控件
	CButton m_ToBeMdlButton;
	// 复选框变量，模型名称
	CButton m_ReqName;
	// 复选框变量，设计人员
	CButton m_ReqDesigner;
	// 复选框，保存日期
	CButton m_ReqSaveTime;
	// 复选框，设计部门
	CButton m_ReqDept;
	// 复选框，备注信息
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
		long        number;   //模型的编号
		CString     name;     //模型的名称
		CString     type;     //模型的类型,工作区和
		CTime       datetime; //模型保存的时间
		CString     filename; //模型的文件名
		CString     person;   //设计人员
		CString     dept;     //部门
		CString     note;
	}RequeryItem;
	CArray<RequeryItem,RequeryItem> p_ReqList;

	void ShowListWithProp(void);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton6();
	// 选中的模型编号
	int selectNumber;
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	// 打开按钮控件
	CButton m_OpenButton;
	void AddMdlParaRelPic(CString localDir,ProMdl mdl);
	bool CheckMdl(CString fileName);
	afx_msg void On12120();
	afx_msg void OnDelMdl();
	int m_ViewType;
	// 查看按钮
	CButton m_ViewButton;
	CString GetSubType(CString subtype);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnSeeMdlPropDlg();
};
