#pragma once
#include "AsmManageTableSet.h" 
#include "afxwin.h"
#include "afxcmn.h"

// CAsmMdlOkDlg 对话框

class CAsmMdlOkDlg : public CDialog
{
	DECLARE_DYNAMIC(CAsmMdlOkDlg)

public:
	CAsmMdlOkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAsmMdlOkDlg();

// 对话框数据
	enum { IDD = IDD_ASM_MDL_OK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void MarkComp(ProSolid comp, int upLevel);
	typedef struct asm_comp_item {
		ProSolid mdl;       //元件句柄
		int      num;       //元件的编号
		int      upLevel;   //上层编号
		int      postId;    //后缀
	}asm_comp_item,*AsmCompItemList;
	
	//相关尺寸、参数表
	typedef struct Asm_Para_item {
		int      solid_num;     //所在模型的编号，作为添加到数据库中的后缀
		CString  type;          //"para" "dim"
		CString  id;            //"名称/符号 ： 模型号"
		CString  DimSymbol;     //尺寸符号
		CString  Name;          //尺寸符号，参数名称 有后缀
		CString  featName;          //特征才有效
		CString  SubType;       //子类
	    CString  Value;         //参数，尺寸的值  ，值不能够为空，要判断
		CString  UpLimit;
		CString  DwLimit;
        CString  Note;
	}Asm_Para_item,*AsmParaItemList;
	//CArray<AsmRelItem,AsmRelItem> p_temp_RelList;                 //不进行替换的关系
    CArray<AsmRelItem,AsmRelItem> tempRelList;               //替换后缀之后的关系
	CArray<asm_comp_item,asm_comp_item> p_AsmCompItem;       //模型编号与后缀对应关系
	CArray<Asm_Para_item,Asm_Para_item> p_AsmRelRvItem;      //模型的相关尺寸项
	CArray<Asm_Para_item,Asm_Para_item> P_temp_AsmSmlItem;   //模型的事务特性表项
    
    int                asm_num;                              //添加到数据库中，装配体的编号，初始值为-1
	CString            fileDirectory;                        //模型在服务器中的存放位置
	CString            fileLodcalDir;                        //模型在本地的存放位置
	DECLARE_MESSAGE_MAP()
public:
	virtual    BOOL OnInitDialog();
	ProSolid   asm_solid;                                      //装配件的句柄,需要在弹出对话框之前付值
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
	//检查模型
	BOOL Check(ProSolid solid);
	//检查组件的名称、编码、类别是否定义、类别是否存在
	bool CheckMdlInfo(ProSolid  solid);
	CEdit m_TextEdit;
	CProgressCtrl m_Prog;
	CButton m_OkButton;
	int    m_PropCur;
	//BOOL InsertDrwInfo(void);
	CAsmManageTableSet m_pSet;
	/*afx_msg void OnBnClickedButton1();*/
	void SetProg(int cur,CString message);
	//更新模型  
	bool UpdateMdl(ProSolid solid,long MdlNumber,CDatabase * db);
	//状态用于确定是否需要设置滚动条，true表示需要
	bool bState;
};
