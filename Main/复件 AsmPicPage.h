#pragma once


// CAsmPicPage �Ի���

class CAsmPicPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmPicPage)

public:
	CAsmPicPage();
	virtual ~CAsmPicPage();

// �Ի�������
	enum { IDD = IDD_ASM_PIC_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
