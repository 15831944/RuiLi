#pragma once


// CRigidity1Dlg �Ի���

class CRigidity1Dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CRigidity1Dlg)

public:
	CRigidity1Dlg();
	virtual ~CRigidity1Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RIGIDITY1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
