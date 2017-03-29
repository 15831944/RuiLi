#pragma once
#include "asminfodefpage.h"
#include "asmmdltreepage.h"
#include "asmswtxpage.h"
#include "asmperfpage.h"
#include "asmrelpage.h"
#include "asmpicpage.h"
#include "afxcmn.h"
#include "asmmanagetableset.h"


// CAsmMdlManDlg �Ի���

class CAsmMdlManDlg : public CDialog
{
	DECLARE_DYNAMIC(CAsmMdlManDlg)

public:
	CAsmMdlManDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAsmMdlManDlg();

// �Ի�������
	enum { IDD = IDD_ASM_MODEL_MAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CPropertySheet m_sheet;
	CAsmInfoDefPage m_page1_Info;
	CAsmMdlTreePage m_page2_Tree;
	CAsmSWTXPage m_page3_Swtx;
	CAsmPerfPage m_page4_Perf;
	CAsmRelPage m_page5_Rel;
	CAsmPicPage m_page6_Pic;
	void SetSheetPage(void);
	CTreeCtrl m_Tree;
	virtual BOOL OnInitDialog();
	// ��ʾģ�͹���
	void ShowTree(void);
	CAsmManageTableSet m_pSet;
	void AddSubItem(HTREEITEM hItem);
};
