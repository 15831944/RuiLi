#pragma once
#include "afxcmn.h"


// CPropPage3 �Ի���

class CPropPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPage3)

public:
	CPropPage3();
	virtual ~CPropPage3();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_LARGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	// ��ϵ
	/*CArray<UsrRelTable,UsrRelTable> * p_temp_UsrRelTableList;*/
	int SetValue(ProMdl solid/*,CArray<UsrRelTable,UsrRelTable>* pp_temp_UsrRelTableList*/);
	ProMdl mdl;
	virtual BOOL OnInitDialog();
	int SetList(void);
};
