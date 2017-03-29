// AsmPicPage.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "AsmPicPage.h"
//void  GetAsmCompHand(ProSolid solid,CArray<ProSolid,ProSolid> * p_solidlist);
//void  GetMdlProp(ProSolid solid,int * indexNo);
//extern CArray<UsrPicture,UsrPicture>   p_UsrPicture;  
//extern CArray<UsrPartPorp,UsrPartPorp> p_UsrPartPorp; 
// CAsmPicPage 对话框

IMPLEMENT_DYNAMIC(CAsmPicPage, CPropertyPage)
CAsmPicPage::CAsmPicPage()
	: CPropertyPage(CAsmPicPage::IDD)
	, m_FileName(_T(""))
	, m_Note(_T(""))
	,m_index(-1)
{
}

CAsmPicPage::~CAsmPicPage()
{
}

void CAsmPicPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, 1002, m_List);
	DDX_Text(pDX, 1003, m_FileName);
	DDX_Text(pDX, 1014, m_Note);
	DDX_Control(pDX, IDC_MYIMAGE, m_pic);
	DDX_Control(pDX, 1004, m_ViewPic);
}


BEGIN_MESSAGE_MAP(CAsmPicPage, CPropertyPage)
	ON_BN_CLICKED(1005, OnBnClicked1005)
	ON_BN_CLICKED(1012, OnBnClicked1012)
	ON_BN_CLICKED(1004, OnBnClicked1004)
	ON_NOTIFY(NM_CLICK, 1002, OnNMClick1002)
END_MESSAGE_MAP()


// CAsmPicPage 消息处理程序
/************************************************************************/
/* 删除                                                                 */
/************************************************************************/
void CAsmPicPage::OnBnClicked1005()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp;
	if(m_index==-1) return;
	temp.Format("确认删除性能参数表项：“%s”！",p_UsrPicture[m_index].fileName);
	if(AfxMessageBox(temp)!=IDOK) return;
	
	DeleteFile(_T(p_UsrPicture[m_index].filePath+p_UsrPicture[m_index].fileName));
	p_UsrPicture.RemoveAt(m_index);
	ShowPic("");
	m_ViewPic.EnableWindow(FALSE);
	m_index=-1;
	//刷新
	ShowList();
}
/************************************************************************/
/* 修改                                                                  */
/************************************************************************/
void CAsmPicPage::OnBnClicked1012()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_index==-1)
	{
		AfxMessageBox("当前没有选中图片！");
		return;
	}
	p_UsrPicture[m_index].Note=m_Note;
	//m_index=-1;	
	ShowList();
}
/************************************************************************/
/* 真是图片浏览                                                         */
/************************************************************************/
void CAsmPicPage::OnBnClicked1004()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_file;
	if(m_index==-1) return;
	temp_file=p_UsrPicture[m_index].filePath+p_UsrPicture[m_index].fileName;
	dlg= new CPicViewDlg();
	dlg->Create(IDD_DIALOG5);
	dlg->SetFile(temp_file);
	dlg->ShowWindow(SW_SHOW);
}


BOOL CAsmPicPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
    m_ViewPic.EnableWindow(FALSE);
	// TODO:  在此添加额外的初始化
    ShowList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

int CAsmPicPage::ShowList(void)
{
	CArray<ProSolid,ProSolid>  solidlist;
	int                        num=0;
	ProName                    mdlName;
	ProMdlType                 type;
	solidlist.Add(mdl);
	GetAsmCompHand(mdl,&solidlist);
	m_List.DeleteAllItems();
	do {		
	} while(m_List.DeleteColumn(0)==TRUE);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"文件名",LVCFMT_LEFT,120,-1);
	m_List.InsertColumn(1,"说明",LVCFMT_LEFT,120,-1);
	num=(int)p_UsrPicture.GetCount();
	for (int i=0;i<num;i++) {
		int numSolid=(int)solidlist.GetCount();
		int listIndex=0;
		for (int j=0;j<numSolid;j++) {
			if(p_UsrPicture[i].owner!=(ProSolid)solidlist[j]) continue;
			if(p_UsrPicture[i].isCheck!=1) continue;
			listIndex=m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,(int)m_List.GetItemCount()
				,p_UsrPicture[i].fileName,0,0,0,i);			
            if (p_UsrPicture[i].Note==_T("")) {
				int solidnum=0;
				GetMdlProp(solidlist[j],&solidnum);
				ProMdlNameGet(solidlist[j],mdlName);
				ProMdlTypeGet(solidlist[j],&type);
				if (type==PRO_MDL_PART) {

					p_UsrPicture[i].Note=_T("元件<"+CString(mdlName)+".prt,"+p_UsrPartPorp[solidnum].Name+">的图片");
				}
				if (type=PRO_MDL_ASSEMBLY) {
					p_UsrPicture[i].Note=_T("元件<"+CString(mdlName)+".asm,"+p_UsrPartPorp[solidnum].Name+">的图片");
				}
				
            }
			m_List.SetItemText(listIndex,1,p_UsrPicture[i].Note);
		}
	}
	return 0;
}

void CAsmPicPage::OnNMClick1002(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int num=0;
	CString file;
	num=(int)m_List.GetItemCount();
	for(int i=0;i<num;i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;
		file=p_UsrPicture[(int)m_List.GetItemData(i)].filePath;
		file=file+p_UsrPicture[(int)m_List.GetItemData(i)].fileName;
		ShowPic(file);
		m_FileName=p_UsrPicture[(int)m_List.GetItemData(i)].fileName;
		m_Note=p_UsrPicture[(int)m_List.GetItemData(i)].Note;

		m_index=(int)m_List.GetItemData(i);
		m_ViewPic.EnableWindow(TRUE);
		UpdateData(FALSE);
	}
	*pResult = 0;
}

int CAsmPicPage::ShowPic(CString file)
{
	m_pic.SetFile(file);
	m_pic.OnPaint();
	return 0;
}
