#pragma once


// CNoPass �Ի���

class CNoPass : public CDialog
{
	DECLARE_DYNAMIC(CNoPass)

public:
	CNoPass(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNoPass();

// �Ի�������
	enum { IDD = IDD_NOPASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnNopassYes();
	afx_msg void OnBnClickedNopassNo();
	afx_msg void OnNopassDetail();
};
