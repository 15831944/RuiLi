#pragma once


// CAsmPerfPage �Ի���

class CAsmPerfPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAsmPerfPage)

public:
	CAsmPerfPage();
	virtual ~CAsmPerfPage();

// �Ի�������
	enum { IDD = IDD_ASM_PERF_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
