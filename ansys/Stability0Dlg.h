#pragma once


// CStability0Dlg �Ի���

class CStability0Dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CStability0Dlg)

public:
	CStability0Dlg();
	virtual ~CStability0Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_STABILITY0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
