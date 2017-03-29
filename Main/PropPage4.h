#pragma once
#include "afxwin.h"
#include "PicShow.h"

// CPropPage4 �Ի���

class CPropPage4 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPage4)

public:
	CPropPage4();   // ��׼���캯��
	virtual ~CPropPage4();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_LARGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ģ�͵�����
	CString m_MdlName;
	CString m_MdlCode;
	CString m_MdlType;
	CString m_MdlFileName;
	CString m_MdlCreater;
	CString m_MdlNote;
	CPicShow m_pic;
	ProMdl  mdl;
	virtual BOOL OnInitDialog();
	//int SetValue(ProMdl solid,CArray<UsrPartPorp,UsrPartPorp> * pp_temp_UsrPartPorpList);
	UsrPartPorp * SolidPorp;
	//UsrPartPorp * pp_temp_UsrPartPorp;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
	CEdit m_TypeEdit;
	CEdit m_CodeEdit;
	CComboBox m_Combox;
	CString m_Material;
	CEdit m_DrawFileName;
};
