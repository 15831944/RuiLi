#pragma once
#include "AsmManageTableSet.h" 
#include "afxwin.h"
#include "afxcmn.h"

// CAsmMdlOkDlg �Ի���

class CAsmMdlOkDlg : public CDialog
{
	DECLARE_DYNAMIC(CAsmMdlOkDlg)

public:
	CAsmMdlOkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAsmMdlOkDlg();

// �Ի�������
	enum { IDD = IDD_ASM_MDL_OK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void MarkComp(ProSolid comp, int upLevel);
	typedef struct asm_comp_item {
		ProSolid mdl;       //Ԫ�����
		int      num;       //Ԫ���ı��
		int      upLevel;   //�ϲ���
		int      postId;    //��׺
	}asm_comp_item,*AsmCompItemList;
	
	//��سߴ硢������
	typedef struct Asm_Para_item {
		int      solid_num;     //����ģ�͵ı�ţ���Ϊ��ӵ����ݿ��еĺ�׺
		CString  type;          //"para" "dim"
		CString  id;            //"����/���� �� ģ�ͺ�"
		CString  DimSymbol;     //�ߴ����
		CString  Name;          //�ߴ���ţ��������� �к�׺
		CString  featName;          //��������Ч
		CString  SubType;       //����
	    CString  Value;         //�������ߴ��ֵ  ��ֵ���ܹ�Ϊ�գ�Ҫ�ж�
		CString  UpLimit;
		CString  DwLimit;
        CString  Note;
	}Asm_Para_item,*AsmParaItemList;
	//CArray<AsmRelItem,AsmRelItem> p_temp_RelList;                 //�������滻�Ĺ�ϵ
    CArray<AsmRelItem,AsmRelItem> tempRelList;               //�滻��׺֮��Ĺ�ϵ
	CArray<asm_comp_item,asm_comp_item> p_AsmCompItem;       //ģ�ͱ�����׺��Ӧ��ϵ
	CArray<Asm_Para_item,Asm_Para_item> p_AsmRelRvItem;      //ģ�͵���سߴ���
	CArray<Asm_Para_item,Asm_Para_item> P_temp_AsmSmlItem;   //ģ�͵��������Ա���
    
    int                asm_num;                              //��ӵ����ݿ��У�װ����ı�ţ���ʼֵΪ-1
	CString            fileDirectory;                        //ģ���ڷ������еĴ��λ��
	CString            fileLodcalDir;                        //ģ���ڱ��صĴ��λ��
	DECLARE_MESSAGE_MAP()
public:
	virtual    BOOL OnInitDialog();
	ProSolid   asm_solid;                                      //װ����ľ��,��Ҫ�ڵ����Ի���֮ǰ��ֵ
	afx_msg    void OnBnClickedOk();
	/*BOOL       ReplaceRelPostIdWithMdlNum(void);*/
	/*BOOL       GetRelRvItem(void);*/
	/*void       GetSWTXCStringArray(CStringArray * cs_swtx_item);*/
	/*BOOL       ConverCStringItemToAsmParaItem(Asm_Para_item * item, CString csParaDim);*/
	int        GetSolidNum(ProSolid solid);
	BOOL       GetSWTXItem(void);
	BOOL InsertAsmPropInfo(void);
	BOOL InsertAsmMakeupInfo(void);
	BOOL InsertAsmPartCompList(void);
	BOOL InsertSWTX(void);
	//BOOL InsertRelInfo(void);
	BOOL InsertPerfInfo(void);
	//BOOL InsertRelRvParaInof(void);
	BOOL InsertAsmPicInfo(void);
	BOOL CopyLocalDirToServer(void);
	//���ģ��
	BOOL Check(ProSolid solid);
	//�����������ơ����롢����Ƿ��塢����Ƿ����
	bool CheckMdlInfo(ProSolid  solid);
	CEdit m_TextEdit;
	CProgressCtrl m_Prog;
	CButton m_OkButton;
	int    m_PropCur;
	//BOOL InsertDrwInfo(void);
	CAsmManageTableSet m_pSet;
	/*afx_msg void OnBnClickedButton1();*/
	void SetProg(int cur,CString message);
	//����ģ��  
	bool UpdateMdl(ProSolid solid,long MdlNumber,CDatabase * db);
	//״̬����ȷ���Ƿ���Ҫ���ù�������true��ʾ��Ҫ
	bool bState;
};
