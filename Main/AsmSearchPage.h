#pragma once


// CAsmSearchPage �Ի���

class CAsmSearchPage : public CDialog
{
	DECLARE_DYNAMIC(CAsmSearchPage)

public:
	CAsmSearchPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAsmSearchPage();

// �Ի�������
	enum { IDD = IDD_ASM_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
