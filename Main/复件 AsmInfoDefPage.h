#pragma once


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
};
