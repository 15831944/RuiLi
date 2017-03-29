// StyleStandDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "StyleStandDlg.h"
#include "ProWindows.h"
#include "MyDatabase.h"
#include "afxtempl.h"

// CStyleStandDlg �Ի���
extern FtpInfo Ftp_struc;

extern CMyDatabase m_db;

IMPLEMENT_DYNAMIC(CStyleStandDlg, CDialog)
CStyleStandDlg::CStyleStandDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStyleStandDlg::IDD, pParent)
{
}

CStyleStandDlg::~CStyleStandDlg()
{
}

void CStyleStandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStyleStandDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_MOVE()
	ON_NOTIFY(NM_CLICK, IDC_TREE_STYLESTAND, OnNMClickTreeStylestand)
END_MESSAGE_MAP()


// CStyleStandDlg ��Ϣ�������

void CStyleStandDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hItem=m_FtpTree.GetSelectedItem();
	path=m_FtpTree.GetFullPathEx(hItem);
	OnOK();
}

void CStyleStandDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL CStyleStandDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	m_FtpTree.SubclassDlgItem(IDC_TREE_STYLESTAND,this);
	m_FtpTree.SetConnect(Ftp_struc.strFtp,Ftp_struc.iPort,Ftp_struc.FtpName,Ftp_struc.FtpPassword);
	m_FtpTree.DisplayTree("��ʱĿ¼",TRUE);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CStyleStandDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRepaint(PRO_VALUE_UNUSED);

	// TODO: �ڴ������Ϣ����������
}

void CStyleStandDlg::OnNMClickTreeStylestand(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	*pResult = 0;
}
