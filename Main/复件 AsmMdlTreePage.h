#pragma once


// CAsmMdlTreePage �Ի���

class CAsmMdlTreePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmMdlTreePage)

public:
	CAsmMdlTreePage();
	virtual ~CAsmMdlTreePage();

// �Ի�������
	enum { IDD = IDD_ASM_MDL_TREE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
