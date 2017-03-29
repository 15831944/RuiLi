#pragma once
#include "picshow.h"
#include "afxwin.h"


// CAsmInfoDefPage �Ի���

class CAsmInfoDefPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmInfoDefPage)

public:
	CAsmInfoDefPage();
	virtual ~CAsmInfoDefPage();

// �Ի�������
	enum { IDD = IDD_ASM_INFODEF_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	ProSolid mdl;	
	CString m_MdlName;
	CString m_MdlCode;
	CString m_MdlType;
	CString m_MdlFileName;
	CString m_MdlCreater;	
	CString m_MdlNote;
	CPicShow m_pic;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClicked1004();
	CString m_DrawFileName;
};
