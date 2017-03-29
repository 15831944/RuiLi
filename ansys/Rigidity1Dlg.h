#pragma once


// CRigidity1Dlg 对话框

class CRigidity1Dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CRigidity1Dlg)

public:
	CRigidity1Dlg();
	virtual ~CRigidity1Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RIGIDITY1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
