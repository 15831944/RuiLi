#pragma once


// CStability1Dlg �Ի���

class CStability1Dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CStability1Dlg)

public:
	CStability1Dlg();
	virtual ~CStability1Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_STABILITY1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
