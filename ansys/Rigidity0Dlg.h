#pragma once


// CRigidity0Dlg 对话框

class CRigidity0Dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CRigidity0Dlg)

public:
	CRigidity0Dlg();
	virtual ~CRigidity0Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RIGIDITY0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
