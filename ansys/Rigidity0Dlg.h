#pragma once


// CRigidity0Dlg �Ի���

class CRigidity0Dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CRigidity0Dlg)

public:
	CRigidity0Dlg();
	virtual ~CRigidity0Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RIGIDITY0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
