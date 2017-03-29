// AsmRelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "AsmRelDlg.h"
#include "AsmRelVistDlg.h"
#include "RelInfoEditBox.h"
#include "RelToMdl.h"
//void AsmSolidDimVisFn(ProSolid solid,CStringArray * dimlist );
//extern CArray<AsmRelItem,AsmRelItem> p_AsmRelItemList;
//BOOL CheckEquation(CString Expression,CStringArray * importDim,CStringArray * exportDim);
// CAsmRelDlg 对话框

IMPLEMENT_DYNAMIC(CAsmRelDlg, CDialog)
CAsmRelDlg::CAsmRelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmRelDlg::IDD, pParent)
	,asm_solid(NULL)
	, iSelRelIndex(-1)
{
}

CAsmRelDlg::~CAsmRelDlg()
{
}

void CAsmRelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_ASM_REL_LIST, m_List);
	DDX_Control(pDX, IDD_ASM_REL_EDIT1, m_RelEdit);
	DDX_Control(pDX, IDD_ASM_REL_EDIT2, m_RelInfoEdit);
}


BEGIN_MESSAGE_MAP(CAsmRelDlg, CDialog)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON13, OnBnClickedButton13)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON7, OnBnClickedButton7)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON8, OnBnClickedButton8)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON10, OnBnClickedButton10)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON12, OnBnClickedButton12)
	ON_BN_CLICKED(1012, OnBnClicked1012)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON23, OnBnClickedAsmRelButton23)
	ON_BN_CLICKED(IDC_BUTTON17, OnBnClickedButton17)
	ON_NOTIFY(NM_CLICK, IDD_ASM_REL_LIST, OnNMClickAsmRelList)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON24, OnBnClickedAsmRelButton24)
	ON_BN_CLICKED(IDD_ASM_REL_BUTTON25, OnBnClickedAsmRelButton25)
	ON_NOTIFY(NM_CUSTOMDRAW, IDD_ASM_REL_LIST, OnNMCustomdrawAsmRelList)
	ON_NOTIFY(LVN_HOTTRACK, IDD_ASM_REL_LIST, OnLvnHotTrackAsmRelList)
END_MESSAGE_MAP()


// CAsmRelDlg 消息处理程序

BOOL CAsmRelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"编号",LVCFMT_LEFT,40);
	m_List.InsertColumn(1,"关系式",LVCFMT_LEFT,140);
	m_List.InsertColumn(2,"关系信息",LVCFMT_LEFT,140);
	// TODO:  在此添加额外的初始化
	if (asm_solid==NULL) 
	{
		if (ProMdlCurrentGet((ProMdl*)&asm_solid)==PRO_TK_NO_ERROR) 
			ShowRelSet(asm_solid);
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 检查关系式正确与否                                                   */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	//CStringArray dimlist;
	//CStringArray outCstring;
	//CString      rel;
	//m_RelEdit.GetWindowText(rel);
	//rel.Remove(' ');
	//if (asm_solid==NULL) {
	//	ProMdlCurrentGet((ProMdl*)&asm_solid);
	//}
	//AsmSolidDimVisFn(asm_solid,&dimlist );
	//CheckEquation(rel,&dimlist,&outCstring);
	bool		 bolCheck;
	CString      rel;
	m_RelEdit.GetWindowText(rel);
	CRel		 new_CRel(asm_solid,rel,bolCheck);
	if(bolCheck) 
		AfxMessageBox("关系正确");
	else AfxMessageBox(new_CRel.m_strError);
}
/************************************************************************/
/* "+"号                                                                */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RelEdit.ReplaceSel("+",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/* "-"号                                                                */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RelEdit.ReplaceSel("-",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/* "*"号                                                                */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RelEdit.ReplaceSel("*",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/* "/"号                                                                */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RelEdit.ReplaceSel("/",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/* "()"号                                                               */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RelEdit.ReplaceSel("()",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/*"="号                                                                 */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RelEdit.ReplaceSel("=",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/* ">"号                                                                */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RelEdit.ReplaceSel(">",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/*  "<"号                                                               */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RelEdit.ReplaceSel("<",FALSE);
	m_RelEdit.SetFocus();
}
/************************************************************************/
/* 导入模型中的关系式                                                   */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	//CAsmRelVistDlg    dlg;
	//AsmRelItem        rel_item;
	//CStringArray      dimlist;
	//CStringArray      outCstring;
	//CString           rel;
	//int               indexNo;
	//CString           temp;
	//CStringArray      relSet;
	//CStringArray      relNote;
	//if (asm_solid==NULL) {
	//	AfxMessageBox("当前不存在模型");
	//	return;
	//}
	//dlg.solid=asm_solid;
	//dlg.relSet=&relSet;
	//dlg.relNote=&relNote;
	//AsmSolidDimVisFn(asm_solid,&dimlist );
	//if (dlg.DoModal()==IDOK) {
	//	for (int i=0;i<relSet.GetCount();i++) {

	//		//CString tempRel=relSet[i];
 //  //         if (CheckEquation(tempRel.MakeLower(),&dimlist,&outCstring)) {
	//		//	//检查关系是否已经存在
	//		//	int Flag=0;
	//		//	for (int j=0;j<(int)p_AsmRelItemList.GetSize();j++) {
	//		//		if(p_AsmRelItemList[j].owner!=asm_solid) continue;
	//		//		if (relSet[i]==p_AsmRelItemList[j].relation) 
	//		//		{
	//		//			Flag=1;
	//		//			break;
	//		//		}
	//		//	}
	//		//	if(Flag==1) continue;
	//		//	rel_item.owner=asm_solid;
	//		//	rel_item.relation=relSet[i];
	//		//	rel_item.note=_T("所属元件："+relNote[i]);
	//		//	indexNo=m_List.GetItemCount();
	//		//	temp.Format("%d",indexNo+1);
	//		//	m_List.InsertItem(indexNo,temp);
	//		//	m_List.SetItemData(indexNo,p_AsmRelItemList.Add(rel_item));
	//		//	m_List.SetItemText(indexNo,1,relSet[i]);
	//		//	m_List.SetItemText(indexNo,2,rel_item.note);
	//		//}
	//		//else AfxMessageBox("失败");
	//	}
	//}
	

	//////////////////////////////////////////////////////////////////////////
	CAsmRelVistDlg    dlg;
	CStringArray      relSet;
	CStringArray      relNote;
	dlg.solid=asm_solid;
	dlg.relSet=&relSet;
	dlg.relNote=&relNote;
	if (dlg.DoModal()==IDOK)
	{
		for (int i=0;i<relSet.GetCount();i++)
		{
			g_RelCollection.AddRel(asm_solid,relSet[i],relNote[i]);
		}
	}
	ShowRelSet(asm_solid);
}
/************************************************************************/
/* 把关系添加列表中去                                                   */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString       rel;
	//AsmRelItem    rel_item;
	//CStringArray  dimlist;
	//CStringArray  outCstring;
	//m_RelEdit.GetWindowText(rel);
	//rel.Remove(' ');
	//if (asm_solid==NULL) {
	//	if (ProMdlCurrentGet((ProMdl*)&asm_solid)!=PRO_TK_NO_ERROR) 
	//	{
	//		AfxMessageBox("当前不存在模型");
	//		return;
	//	}
	//}

	//AsmSolidDimVisFn(asm_solid,&dimlist );
	//if (CheckEquation(rel,&dimlist,&outCstring)) {
	//	//检查关系是否已经存在
	//	int Flag=0;
	//	for (int j=0;j<(int)p_AsmRelItemList.GetSize();j++) {
	//		if(p_AsmRelItemList[j].owner!=asm_solid) continue;
	//		if (rel==p_AsmRelItemList[j].relation) 
	//		{
	//			Flag=1;
	//			AfxMessageBox("关系已经存在！");
	//			break;
	//		}
	//	}
	//	if(Flag==0) 
	//	{

	//		rel_item.owner=asm_solid;
	//		m_RelInfoEdit.GetWindowText(rel_item.note);
	//		rel_item.relation=rel;
	//		p_AsmRelItemList.Add(rel_item);
	//	}
	//}
	//bool rv;
	//CRel  c_rel(asm_solid,rel,rv);
	////CString  temp;
	////c_rel.GetRelString(temp);
	////AfxMessageBox(temp);
 //   if (!rv)
 //   {
	//	AfxMessageBox(c_rel.m_strError);
 //   }
	CString			rel;
	CString			note;
	m_RelEdit.GetWindowText(rel);
	m_RelInfoEdit.GetWindowText(note);
	if (!g_RelCollection.AddRel(asm_solid,rel,note))
	{
		AfxMessageBox(g_RelCollection.error_info);
		return;
	}
	ShowRelSet(asm_solid);	
}
/************************************************************************/
/* 显示模型中的关系式                                                   */
/************************************************************************/
void CAsmRelDlg::ShowRelSet(ProSolid solid)
{
	//int       num=0;
	//int       indexNo=0;
	//CString   csIndexNo;
	//m_List.DeleteAllItems();
 //   num=(int)p_AsmRelItemList.GetSize();
	//for (int i=0;i<num;i++) {
	//	if (p_AsmRelItemList[i].owner!=solid) continue;        
	//	csIndexNo.Format("%d",indexNo+1);
	//	m_List.InsertItem(indexNo,csIndexNo);
	//	m_List.SetItemData(indexNo,i);
	//	m_List.SetItemText(indexNo,1,p_AsmRelItemList[i].relation);
	//	m_List.SetItemText(indexNo,2,p_AsmRelItemList[i].note);
	//	indexNo++;
	//}


	CArray<CRel,CRel&>	p_RelList;
	CArray<int,int&>	p_IndexList;
	CString				id="";
	CString				rel;
	int					indexNo=0;
	int                 list_data=0;
	m_List.DeleteAllItems();
	g_RelCollection.GetSolidAllRel(solid,true,&p_RelList,&p_IndexList);
	for (int i=0;i<p_RelList.GetCount();i++)
	{
		id.Format(" %d",m_List.GetItemCount()+1);
		p_RelList[i].GetRelStringAsComp(rel);
		list_data=p_RelList[i].owner==solid?p_IndexList[i]:(-1);    //零件内部的关系，在组件模式下不可编辑，设置-1，用于后面判断
		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,(int)m_List.GetItemCount(),id,0,0,0,list_data);
		m_List.SetItemText(indexNo,1,rel);
		m_List.SetItemText(indexNo,2,p_RelList[i].GetRelNote());
	}
}

void CAsmRelDlg::OnBnClicked1012()
{
	// TODO: 在此添加控件通知处理程序代码

}
/************************************************************************/
/* 编辑                                                                 */
/************************************************************************/
void CAsmRelDlg::OnBnClickedAsmRelButton23()
{
	// TODO: 在此添加控件通知处理程序代码
	CRelInfoEditBox dlg;
	dlg.DoModal();
	m_RelInfoEdit.SetWindowText(dlg.m_note);
}
/************************************************************************/
/* 修改                                                                 */
/************************************************************************/
void CAsmRelDlg::OnBnClickedButton17()
{
	//// TODO: 在此添加控件通知处理程序代码
	//CString      rel;
	//CStringArray outCstring;
	//CStringArray dimlist;
	//if (iSelRelIndex!=-1)
	//{
	//	m_RelEdit.GetWindowText(rel);
	//	rel.Remove(' ');
	//	if (asm_solid==NULL) {
	//		if (ProMdlCurrentGet((ProMdl*)&asm_solid)!=PRO_TK_NO_ERROR) 
	//		{
	//			AfxMessageBox("当前不存在模型");
	//			return;
	//		}
	//	}
	//	//1.如果关系式没修改，只修改说明
 //       if (p_AsmRelItemList[iSelRelIndex].relation==rel)
 //       {
	//		m_RelInfoEdit.GetWindowText(p_AsmRelItemList[iSelRelIndex].note);
	//		ShowRelSet(asm_solid);
	//		m_RelEdit.SetWindowText("");m_RelInfoEdit.SetWindowText("");
	//		iSelRelIndex=-1;
	//		return;
 //       }
	//	//2.如果关系式修改，以及说明也修改了,判断修改后的关系式是否已经在关系中
	//	AsmSolidDimVisFn(asm_solid,&dimlist );
	//	if (CheckEquation(rel,&dimlist,&outCstring)) 
	//	{
	//		int Flag=0;
	//		for (int j=0;j<(int)p_AsmRelItemList.GetSize();j++) {
	//			if(p_AsmRelItemList[j].owner!=asm_solid) continue;
	//			if (rel==p_AsmRelItemList[j].relation) 
	//			{
	//				Flag=1;
	//				AfxMessageBox("关系已经存在,无法修改！");
	//				break;
	//			}
	//		}
	//		if (Flag==0)
	//		{
	//			p_AsmRelItemList[iSelRelIndex].relation=rel;
	//			m_RelInfoEdit.GetWindowText(p_AsmRelItemList[iSelRelIndex].note);
	//			ShowRelSet(asm_solid);
	//		}			
	//	}
	//	
	//}
	//else
	//{
	//	AfxMessageBox("请先选择要修改的关系式");
	//}
	CString		rel;
	CString		note;
	m_RelEdit.GetWindowText(rel);
	m_RelInfoEdit.GetWindowText(note);
	if (iSelRelIndex!=-1)
	{
		if (g_RelCollection.EditRel(iSelRelIndex,rel,note))
		{
			ShowRelSet(asm_solid);
		}
		else
		{
			AfxMessageBox(g_RelCollection.error_info);
		}
	}
	else
	{
		AfxMessageBox("请先选择要修改的关系式");
	}
}
/************************************************************************/
/* 点击控件                                                             */
/************************************************************************/
void CAsmRelDlg::OnNMClickAsmRelList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i<m_List.GetItemCount();i++)
	{
		if (m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED && m_List.GetItemData(i)!=-1)
		{
			//m_RelEdit.SetWindowText(p_AsmRelItemList[(int)m_List.GetItemData(i)].relation);
			//m_RelInfoEdit.SetWindowText(p_AsmRelItemList[(int)m_List.GetItemData(i)].note);			
			m_RelEdit.SetWindowText(m_List.GetItemText(i,1));
			m_RelInfoEdit.SetWindowText(m_List.GetItemText(i,2));
			iSelRelIndex=(int)m_List.GetItemData(i);
			break;
		}
	}
	*pResult = 0;
}
/************************************************************************/
/* 删除关系式                                                           */
/************************************************************************/
void CAsmRelDlg::OnBnClickedAsmRelButton24()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (iSelRelIndex!=-1)
	//{
	//	p_AsmRelItemList.RemoveAt(iSelRelIndex);
	//	p_AsmRelItemList.FreeExtra();
	//	ShowRelSet(asm_solid);
	//	iSelRelIndex=-1;
	//}
	//else
	//{
	//	AfxMessageBox("请先选择要修改的关系式");
	//}
	if (iSelRelIndex!=-1)
	{
		if (g_RelCollection.DelRel(iSelRelIndex))
		{
			iSelRelIndex=-1;
			ShowRelSet(asm_solid);
		}
		else
		{
			AfxMessageBox(g_RelCollection.error_info);
		}
	}
	else
	{
		AfxMessageBox("请先选择要修改的关系式");
	}
}
/************************************************************************/
/* 导出关系到模型中去                                                   */
/************************************************************************/
void CAsmRelDlg::OnBnClickedAsmRelButton25()
{
	// TODO: 在此添加控件通知处理程序代码
	CRelToMdl dlg;
	//dlg.SetRel(asm_solid,&p_temp_Rel);
	dlg.SetRel(asm_solid);
	dlg.DoModal();
}

void CAsmRelDlg::OnNMCustomdrawAsmRelList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//pNMCD->
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
/*	pLVCD->
		if (list_data==-1)
		{
			*/CDC* pDC =this->GetWindowDC();//;this->GetWindowDC(); // GetWindowDC();
			CRect  rect;
			m_List.GetItemRect(0,&rect,LVIR_BOUNDS);
			//ClientToScreen(&rect);
			pDC->Rectangle(&rect);
			pDC->FillSolidRect(&rect, 126326);

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(0);
			pDC->SetBkColor(126326);
	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	//if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	//{
	//	*pResult = CDRF_NOTIFYITEMDRAW;
	//}
	//else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	//{
	//	// This is the notification message for an item.  We'll request
	//	// notifications before each subitem's prepaint stage.

	//	*pResult = CDRF_NOTIFYSUBITEMDRAW;
	//}
	//else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	//{
	//	// This is the prepaint stage for a subitem. Here's where we set the
	//	// item's text and background colors. Our return value will tell
	//	// Windows to draw the subitem itself, but it will use the new colors
	//	// we set here.
	//	//int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
	//	//int nSubItem = pLVCD->iSubItem;
	//	//if(IsReadOnly(nSubItem))	
	//	//{
	//	//	COLORREF clr = 12632256;
	//	//	CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
	//	//	CRect rect;
	//	//	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	//	//	pDC->Rectangle(rect);
	//	//	DrawText(nItem, nSubItem, pDC, 0, clr, rect);

	//	//	*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.
	//	//}
	//	//if (list_data==-1)
	//	//{
	//	//	CDC* pDC =this->GetWindowDC(); // GetWindowDC();
	//	//	CRect  rect;
	//	//	m_List.GetItemRect(indexNo,&rect,LVIR_BOUNDS);
	//	//	ClientToScreen(&rect);
	//	//	pDC->Rectangle(&rect);
	//	//	pDC->FillSolidRect(&rect, 126326);
	//	//	pDC->SetBkMode(TRANSPARENT);
	//	//	pDC->SetBkColor(126326);
	//	AfxMessageBox(" OK");
	//	//}
	//}
	*pResult = 0;
}

void CAsmRelDlg::OnLvnHotTrackAsmRelList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (m_List.GetItemData(pNMLV->iItem)==-1)
	{
		m_List.Invalidate(TRUE);
	}
	else m_List.Invalidate(FALSE);
	*pResult = 0;
}
