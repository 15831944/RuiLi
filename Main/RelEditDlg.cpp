// RelEditDlg.cpp : 实现文件
//对关系式编辑的对话框，作为子对话框使用

#include "stdafx.h"
#include "Main.h"
#include "RelEditDlg.h"
#include "RelToMdl.h"

//extern CArray<UsrRelTable,UsrRelTable>    p_UsrRelTableList; 
//extern int DlgIcon;
ProError SolidDimensionVisAct(ProDimension *dimension,ProError status,CStringArray * data);
ProError ParameterActionFn2(ProParameter *Param,ProError status,
						   CStringArray *p_pdata);
//
ProError ParameterFilterFn2(ProParameter *Param,ProError status,
						   CStringArray *p_pdata) ;
//BOOL CheckEquation(CString Expression,CStringArray * importDim,CStringArray * exportDim);
// CRelEditDlg 对话框

IMPLEMENT_DYNAMIC(CRelEditDlg, CDialog)
CRelEditDlg::CRelEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRelEditDlg::IDD, pParent)
	,mdl(NULL)                                    //要进行操作的模型
	, m_RelEdit(_T(""))
	, m_RelNoteEdit(_T(""))
	, iSelRelIndex(-1)
{
}

CRelEditDlg::~CRelEditDlg()
{
}

void CRelEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_RelEdit);
	DDX_Text(pDX, IDC_EDIT2, m_RelNoteEdit);
	DDX_Control(pDX, IDC_EDIT1, m_RelEditCtrl);
}


BEGIN_MESSAGE_MAP(CRelEditDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON12, OnBnClickedButton12)
	ON_BN_CLICKED(IDC_ADD_SYMBOL, OnBnClickedAddSymbol)
	ON_BN_CLICKED(IDC_MINUS_SYMBOL, OnBnClickedMinusSymbol)
	ON_BN_CLICKED(IDC_MULTIPLY_SYMBOL, OnBnClickedMultiplySymbol)
	ON_BN_CLICKED(IDC_DID_SYMBOL, OnBnClickedDidSymbol)
	ON_BN_CLICKED(IDC_BRACKET_SYMBOL, OnBnClickedBracketSymbol)
	ON_BN_CLICKED(IDC_QUEAL_SYMBOL, OnBnClickedQuealSymbol)
	ON_BN_CLICKED(IDC_BUTTON13, OnBnClickedButton13)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON14, OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON16, OnBnClickedButton16)
	ON_BN_CLICKED(IDC_QUEAL_SYMBOL2, OnBnClickedQuealSymbol2)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON15, OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON17, OnBnClickedButton17)
	ON_WM_CHAR()
END_MESSAGE_MAP()
// CRelEditDlg 消息处理程序

BOOL CRelEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    m_List.InsertColumn(0,"编号",LVCFMT_LEFT,40);
    m_List.InsertColumn(1,"关系式",LVCFMT_LEFT,140);
	m_List.InsertColumn(2,"关系信息",LVCFMT_LEFT,140);
	// TODO:  在此添加额外的初始化
    if(mdl!=NULL)  ShowRel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 函数：SetMdl(ProMdl owner)
说明：设置关系编辑框所编辑的模型                                     */
/************************************************************************/
void CRelEditDlg::SetMdl(ProMdl owner)
{
	mdl=owner;
}
/************************************************************************/
/* 显示关系式                                                           */
/************************************************************************/
void CRelEditDlg::ShowRel(void)
{

	//////////////////////////////////////////////////////////////////////////
	//原来的
	//int num=0;
	//int indexNo=0;
	//CString id="";
	//num=(int)p_UsrRelTableList.GetCount();
	//m_List.DeleteAllItems();
	//for(int i=0;i<num;i++)
	//{
	//	if(p_UsrRelTableList[i].owner!=mdl) continue;
	//	if(p_UsrRelTableList[i].isCheck!=1) continue;
 //       id.Format(" %d",m_List.GetItemCount()+1);
 //       indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_TEXT,(int)m_List.GetItemCount(),id,0,0,0,i);
	//	m_List.SetItemText(indexNo,1,p_UsrRelTableList[i].rel);
	//	m_List.SetItemText(indexNo,2,p_UsrRelTableList[i].note);
	//}
    //////////////////////////////////////////////////////////////////////////
    



	//////////////////////////////////////////////////////////////////////////
	//if (AfxMessageBox("新的",MB_OKCANCEL,-1 )==IDOK)
	//{
	//	num=(int)p_CRelItemList.GetCount();
	//	m_List.DeleteAllItems();
	//	for (int i=0;i<num;i++)
	//	{
	//		CString rel;
	//		if(p_CRelItemList[i].owner!=mdl) continue;
	//		p_CRelItemList[i].GetRelString(rel);
	//		 id.Format(" %d",m_List.GetItemCount()+1);
	//		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_TEXT,(int)m_List.GetItemCount(),id,0,0,0,i);
	//		m_List.SetItemText(indexNo,1,rel);
	//		m_List.SetItemText(indexNo,2,p_CRelItemList[i].GetRelNote());
	//	}
	//}
	//////////////////////////////////////////////////////////////////////////
    //3
	CArray<CRel,CRel&>	p_RelList;
	CArray<int,int&>	p_IndexList;
	CString				id="";
	CString				rel;
	int					indexNo=0;
	m_List.DeleteAllItems();
	g_RelCollection.GetSolidAllRel((ProSolid)mdl,false,&p_RelList,&p_IndexList);
	for (int i=0;i<p_RelList.GetCount();i++)
	{
		    id.Format(" %d",m_List.GetItemCount()+1);
			p_RelList[i].GetRelString(rel);
		    indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,(int)m_List.GetItemCount(),id,0,0,0,p_IndexList[i]);
			m_List.SetItemText(indexNo,1,rel);
			m_List.SetItemText(indexNo,2,p_RelList[i].GetRelNote());
	}
	//////////////////////////////////////////////////////////////////////////
	
	
}
/************************************************************************/
/* 关系式信息编辑                                                       */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码	
	INT_PTR nRet = -1;
	UpdateData(TRUE);
	info_edit_dlg.m_note=m_RelNoteEdit;
	nRet=info_edit_dlg.DoModal();
	switch ( nRet )
	{
	case -1: 
		AfxMessageBox("打开编辑框失败！");
		break;
	case IDABORT:
		// Do something
		break;
	case IDOK:
		m_RelNoteEdit=info_edit_dlg.m_note;
		UpdateData(FALSE);
		break;
	case IDCANCEL:
		// Do something
		break;
	default:
		// Do something
		break;
	};

}
/************************************************************************/
/* 添加关系式，到关系式数组中                                           */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_RelEdit=="") 
	{
		AfxMessageBox("请先输入关系式！");
		return;
	}


	//////////////////////////////////////////////////////////////////////////
	//原来
	//for(int i=0;i<m_List.GetItemCount();i++)
	//{
	//	if(m_List.GetItemText(i,1)==m_RelEdit) 
	//	{
	//		AfxMessageBox("关系式已经存在！");
	//		return;
	//	}
	//}
	//////检查关系式是否正确，然后添加,检查包括计算符号，尺寸和参数是否正确，
	//UsrRelTable temp_usrreltable;
	//temp_usrreltable.owner=mdl;
	//temp_usrreltable.rel=m_RelEdit;
	//if(CheckRel(m_RelEdit)==false) return;
	//temp_usrreltable.note=m_RelNoteEdit;
	//temp_usrreltable.isCheck=1;
	//p_UsrRelTableList.Add(temp_usrreltable);
    //////////////////////////////////////////////////////////////////////////
    

	//CRel rel((ProSolid)mdl,m_RelEdit);
	//CString str;
	//rel.GetRelString(str);
	//AfxMessageBox(str);
    //////////////////////////////////////////////////////////////////////////
    //新的关系式
	//bool    bolCheck;
 //   CRel    new_CRel((ProSolid)mdl,m_RelEdit,bolCheck);
 //   if (bolCheck==true)
 //   {
	//	for (int i=0;i<p_CRelItemList.GetCount();i++ )
	//	{
	//		if(p_CRelItemList[i]==new_CRel )
	//		{
	//			AfxMessageBox("关系式已经存在");
	//			return;
	//		}
	//	}
	//	p_CRelItemList.Add(new_CRel);
 //   }
	//else 
	//{
	//	AfxMessageBox(new_CRel.m_strError);
	//	return;
	//}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	//3.
	if (!g_RelCollection.AddRel((ProSolid)mdl,m_RelEdit,m_RelNoteEdit))
	{
		AfxMessageBox(g_RelCollection.error_info);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	

	ShowRel();
}
/************************************************************************/
/* 在关系式编辑框中加入“＋”号                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedAddSymbol()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RelEditCtrl.ReplaceSel("+",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* 在关系式编辑框中加入“－”号                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedMinusSymbol()
{
	// TODO: 在此添加控件通知处理程序代码
    m_RelEditCtrl.ReplaceSel("-",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* 在关系式编辑框中加入“x”号                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedMultiplySymbol()
{
	// TODO: 在此添加控件通知处理程序代码
    m_RelEditCtrl.ReplaceSel("*",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* 在关系式编辑框中加入“/”号                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedDidSymbol()
{
	// TODO: 在此添加控件通知处理程序代码
    m_RelEditCtrl.ReplaceSel("/",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* 在关系式编辑框中加入“()”号                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedBracketSymbol()
{
	// TODO: 在此添加控件通知处理程序代码
    m_RelEditCtrl.ReplaceSel("()",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* 在关系式编辑框中加入“=”号                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedQuealSymbol()
{
	// TODO: 在此添加控件通知处理程序代码
    m_RelEditCtrl.ReplaceSel("=",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* 在关系式中加入“>”                                                 */
/************************************************************************/
void CRelEditDlg::OnBnClickedQuealSymbol2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RelEditCtrl.ReplaceSel(">",FALSE);
	m_RelEditCtrl.SetFocus();
}
/************************************************************************/
/* 在关系式中加入“<”                                                 */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RelEditCtrl.ReplaceSel("<",FALSE);
	m_RelEditCtrl.SetFocus();
}

/************************************************************************/
/* 删除关系式                                                           */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	//////////////////////////////////////////////////////////////////////////
	//新的
	//if (iSelRelIndex!=-1)
	//{
	//	p_CRelItemList.RemoveAt(iSelRelIndex);
	//	p_CRelItemList.FreeExtra();
	//	m_RelEdit="";
	//	m_RelNoteEdit="";
	//	UpdateData(FALSE);
	//	//ShowRel();
	//}
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	////原来
	//if (iSelRelIndex!=-1)
	//{
	//	p_UsrRelTableList.RemoveAt(iSelRelIndex);
	//	p_UsrRelTableList.FreeExtra();
	//	m_RelEdit="";
	//	m_RelNoteEdit="";
	//	UpdateData(FALSE);
	//	iSelRelIndex=-1;
	//	ShowRel();
	//}
	//else
	//{
	//	AfxMessageBox("请先选择要修改的关系式");
	//}
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//3
	if (iSelRelIndex!=-1)
	{
		if (g_RelCollection.DelRel(iSelRelIndex))
		{
			iSelRelIndex=-1;
			ShowRel();
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
	//////////////////////////////////////////////////////////////////////////
	
}
/************************************************************************/
/* 单击列表框，在下面的编辑框里出现相应的信息                           */
/************************************************************************/
void CRelEditDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		if(m_List.GetItemState(i, LVIS_SELECTED)!=LVIS_SELECTED) continue;
		m_RelEdit=m_List.GetItemText(i,1);
		m_RelNoteEdit=m_List.GetItemText(i,2);
		iSelRelIndex=(int)m_List.GetItemData(i);
		UpdateData(FALSE);
		break;
	}
	*pResult = 0;
}
/************************************************************************/
/* 访问模型中的关系式，选择添加                                         */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	int num=0;
	int flag=0;
    CMdlRelVisitDlg mdl_rel_vis_dlg;
	mdl_rel_vis_dlg.SetMdl(mdl);
	if(mdl_rel_vis_dlg.DoModal()!=IDOK) return;
	num=(int)mdl_rel_vis_dlg.rel_stringList_select.GetCount();
	for(int i=0;i<num;i++)
	{
		//原来
		//////////////////////////////////////////////////////////////////////////		
		//UsrRelTable temp;
		//temp.owner=mdl;
  //      if(CheckRel(mdl_rel_vis_dlg.rel_stringList_select[i])==false) continue;
	 //  //判断是否已经存在
		//for(int j=0;j<m_List.GetItemCount();j++)
		//{
		//	if(m_List.GetItemText(j,1)==mdl_rel_vis_dlg.rel_stringList_select[i])  flag=1;
		//}
		//if(flag==1)
		//{
		//	flag=0;
		//	continue;
		//}
		//temp.rel=mdl_rel_vis_dlg.rel_stringList_select[i];
		//temp.isCheck=1;
		//temp.note="";
  //      p_UsrRelTableList.Add(temp);
		//////////////////////////////////////////////////////////////////////////
  


		//////////////////////////////////////////////////////////////////////////
		////新的
		//bool    bolCheck;
		//CRel    new_CRel((ProSolid)mdl,mdl_rel_vis_dlg.rel_stringList_select[i],bolCheck);
		//if (bolCheck==true)
		//{
		//	flag=0;
		//	for (int j=0;j<p_CRelItemList.GetCount();j++ )
		//	{
		//		if(p_CRelItemList[j]==new_CRel )
		//		{
		//			flag=1;
		//			break;
		//		}
		//	}
		//	if(flag==0) p_CRelItemList.Add(new_CRel);
		//}
		//else 
		//{
		//	AfxMessageBox(new_CRel.m_strError);
		//	continue;
		//}
		//////////////////////////////////////////////////////////////////////////
		//3
		g_RelCollection.AddRel((ProSolid)mdl,mdl_rel_vis_dlg.rel_stringList_select[i],"");
		//////////////////////////////////////////////////////////////////////////
		
		
	}
	ShowRel();
}
/************************************************************************/
/* 检查关系式是否正确                                                   */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton16()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//m_RelEdit.Remove(' ');
	//原来
	//////////////////////////////////////////////////////////////////////////	
	//if(CheckRel(m_RelEdit)==true)  AfxMessageBox("关系正确！");
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	bool    bolCheck;
	CRel    new_CRel((ProSolid)mdl,m_RelEdit,bolCheck);
	if(bolCheck) 
		AfxMessageBox("关系正确");
	else AfxMessageBox(new_CRel.m_strError);
	//////////////////////////////////////////////////////////////////////////
	
}
/************************************************************************/
/*用于检查没个指定的关系式是否正确                                      */
/************************************************************************/
bool CRelEditDlg::CheckRel(CString rel)
{
    CString error;
	//先检查符号
   rel.Remove(' ');
   error.Append(CheckSymbol(rel));
   if(!error.IsEmpty()) 
   {AfxMessageBox(error);return false;}
   //进一步检查
   error.Append(CheckDimAndPara(rel));
   if(!error.IsEmpty()) {/*AfxMessageBox(error);*/return false;}
   return true;
}
/************************************************************************/
/*检查＝ <  > >= <=,进行计数，总共只能有一个,以及顺序                   */
/************************************************************************/
CString CRelEditDlg::CheckSymbol(CString rel)
{
	CString error;
	int a=0;
	int b=0;
	int c=0;
	int d=0;
	int e=0;
	int temp=0;
    int temp1=0;
	//检查＝ > <号
	do {
		temp=rel.Find("=",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>1) error.AppendFormat("存在多个=号！\n");

	temp=0;
	do {
		temp=rel.Find("<",temp);
		if(temp!=-1) b++,temp++;
	} while(temp!=-1);
	if(b>1) error.AppendFormat("存在多个<号！\n");

	temp=0;
	do {
		temp=rel.Find(">",temp);
		if(temp!=-1) c++,temp++;
	} while(temp!=-1);
	if(c>1) error.AppendFormat("存在多个>号！\n");

	if(a==1&&b==1&&(rel.Find("=<",0)!=-1)) error.AppendFormat("“=<”符号不正确，应该为“<=”\n");
	if(a==1&&b==1&&(rel.Find("<=",0)==-1)) error.AppendFormat("“<”与“=”不能分开！\n");
	if(a==1&&c==1&&(rel.Find("=>",0)!=-1)) error.AppendFormat("“=>”符号不正确，应该为“>=”\n");
	if(a==1&&c==1&&(rel.Find(">=",0)==-1)) error.AppendFormat("“>”与“=”不能分开！\n");
	//检查括号

	temp=0;
	temp1=0;
	d=0;
	e=0;
	do {
		temp=rel.Find("(",temp);

		if(temp!=-1) d++,temp++;
		//if(temp1!=-1) e++,temp1++;
	} while(temp!=-1);

	do {
		temp1=rel.Find(")",temp1);
		if(temp1!=-1) e++,temp1++;
	} while(temp1!=-1);

	if(e!=d) error.AppendFormat("“(”与“)”个数不匹配！\n");

	temp=0,temp1=0;
	a=0;
	//检查是否有双个++ -- / / * * . . +- -+ +/ /+  *+ +* *- -* 00
    do {
    	temp=rel.Find("++",temp);
		if(temp!=-1) a++,temp++;
    } while(temp!=-1);
	if(a>=1) error.AppendFormat("存在多个+号连在一起！\n");
	temp=0;
	a=0;
	do {
		temp=rel.Find("--",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("存在多个-号连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("////",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("存在多个////号连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("**",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("存在多个**号连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("..",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("存在多个.号连在一起！\n");
	
	temp=0;
	a=0;
	do {
		temp=rel.Find("+-",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("+-号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("-+",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("-+号不能连在一起！\n");
    
	temp=0;
	a=0;
	do {
		temp=rel.Find("+//",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("+//号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("*+",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("*+号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("+*",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" +*号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("*-",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" *-号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("-*",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" -*号不能连在一起！\n");

	//temp=0;
	//a=0;
	//do {
	//	temp=rel.Find("0",temp);
	//	if(temp!=-1) a++,temp++;
	//} while(temp!=-1);
	//if(a>1) error.AppendFormat(" 00号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("=-",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" =-号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("=+",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" =+号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("+=",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" +=号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("=.",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" =.号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find(".=",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" .=号不能连在一起！\n");
   
	temp=0;
	a=0;
	do {
		temp=rel.Find("*=",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat(" *=号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("=*",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("=*号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("=//",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("=//号不能连在一起！\n");

	temp=0;
	a=0;
	do {
		temp=rel.Find("//=",temp);
		if(temp!=-1) a++,temp++;
	} while(temp!=-1);
	if(a>=1) error.AppendFormat("//=号不能连在一起！\n");


	temp=0;
	CString str;
	str=rel.Left(1);
    temp=str.FindOneOf("+-//*=<>^");
	if(temp!=-1) error.AppendFormat("第一个不能是运算符！\n");
	temp=0;
	str=rel.Right(1);
	temp=str.FindOneOf("+-//*=<>^");
	if(temp!=-1) error.AppendFormat("最一个不能是运算符！\n");	

	return error;
}
/************************************************************************/
/* 检查尺寸，参数是否存在
   检查尺寸参数：首先获得所有的尺寸和参数，然后分离出关系中的尺寸和参数，
   如果分离出来的在模型中，则可以使用                                   */
/************************************************************************/
CString CRelEditDlg::CheckDimAndPara(CString rel)
{
	CStringArray dimpara;
	CStringArray dim;
	CStringArray para;
    CStringArray dimpara2;
	CString error;
	ProModelitem modelitem;
																		//分解除尺寸，符号
																		//RelFenli(rel,&dimpara);
																		//判断尺寸，符号是否存在
	//1.访问尺寸
    ProSolidDimensionVisit((ProSolid)mdl,PRO_B_FALSE,(ProDimensionVisitAction)SolidDimensionVisAct,
		NULL,(CStringArray*)&dim);
    //2.访问参数	
	ProMdlToModelitem(mdl, &modelitem);
	ProParameterVisit(&modelitem,
		(ProParameterFilter)ParameterFilterFn2,
		(ProParameterAction )ParameterActionFn2 ,(CStringArray*)&para);  

	dimpara2.Append(dim);
	dimpara2.Append(para);	
	//dimpara2.Add(">=");
	//dimpara2.Add("<=");
	//判断从关系式中分离出来的
	if(!CheckEquation(rel,&dimpara2,&dimpara)) error=_T("出错");
	//dimpara2.RemoveAll();dimpara2.FreeExtra();
	//error.Append(CompareCsarry1ToCsarry2(&dimpara,&dimpara2));
	return error;
}
/************************************************************************/
/*// 输入一条关系式，分离出尺寸和参数                                   */
/************************************************************************/
void  CRelEditDlg::RelFenli(CString str,CStringArray * CanshuName)
{
	//CStringArray CanshuName;    //
	CString  resToken;
	BOOL     IsHave;
	int      curPos= 0;
	int      j=0;
	resToken=str.Tokenize("=+-*/()<>",curPos);
	while (resToken !="")
	{
		if(iswdigit(resToken.GetAt(0))==0)
		{
            int num=0;
			num=(int)CanshuName->GetSize();
			if(num==0)
			{
				CanshuName->Add(resToken);
			}
			else
			{
				IsHave=false;
				for(int i=0;i<num;i++)
				{
					if(CanshuName->GetAt(i)==resToken)
					{
						IsHave=true;
						break;
					}
				}
				if (!IsHave) 
				{
					CanshuName->Add(resToken); 
				}
			}
				
		}

	resToken=str.Tokenize("=+-*/()><",curPos);
	}	
    //return CanshuName;
}
/************************************************************************/
/* 访问尺寸的符号的                                                     */
/************************************************************************/
ProError SolidDimensionVisAct(ProDimension *dimension,
							  ProError status,CStringArray * data)
{
	CString temp;
	ProName symbol;
	ProDimensionSymbolGet(dimension,symbol);
	temp=CString(symbol);
    data->Add(temp);
	return PRO_TK_NO_ERROR;
}
/************************************************************************/
/* 参数访问访问函数                                                     */
/************************************************************************/
ProError ParameterActionFn2(ProParameter *Param,ProError status,
							CStringArray *p_pdata)
{
	CString temp;
	temp=CString(Param->id);
	
	//把参数变为小写
	//temp.MakeLower();
	p_pdata->Add(temp);
	return PRO_TK_NO_ERROR;

}
/************************************************************************/
/* 参数过滤函数,值访问DOUBLe                                            */
/************************************************************************/
ProError ParameterFilterFn2(ProParameter *Param,ProError status,
							CStringArray *p_pdata)
{ 
	ProParamvalueType r_type;
	ProParamvalue proval;
	ProParameterValueGet(Param,&proval);
	ProParamvalueTypeGet(&proval,&r_type);
	if(r_type==PRO_PARAM_DOUBLE||r_type==PRO_PARAM_INTEGER)
	{
		return PRO_TK_NO_ERROR;
	}
    return PRO_TK_CONTINUE;
}
/************************************************************************/
/* 比较两个字符串数组                                                   */
/************************************************************************/
CString CRelEditDlg::CompareCsarry1ToCsarry2(CStringArray * Csarry1, CStringArray * Csarry2)
{
	int num1=0,num2=0;
	int flag=0;
	num1=(int)Csarry1->GetCount();//关系式中的尺寸参数
	num2=(int)Csarry2->GetCount();//模型中的尺寸参数
	CString error;
	for(int i=0;i<num1;i++)
	{
		flag=0;
		for(int j=0;j<num2;j++)
		{
			//把字符传都先变成小写
			(*Csarry1)[i].MakeLower();
            (*Csarry2)[j].MakeLower();
			if((*Csarry1)[i]==(*Csarry2)[j])
			{
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			CString temp;
			temp.Format("符号%s,不是尺寸也不是参数，请确认！\n",(*Csarry1)[i]);
			error.Append(temp);
		}
	}
	return error;
}
/************************************************************************/
/* 导出关系式到模型中                                                   */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	CRelToMdl dlg;
	//dlg.SetRel(mdl,&p_UsrRelTableList);
	dlg.SetRel(mdl);
	dlg.DoModal();

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

}
/************************************************************************/
/* 修改关系式                                                           */
/************************************************************************/
void CRelEditDlg::OnBnClickedButton17()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//////////////////////////////////////////////////////////////////////////
	//原来
	//if (iSelRelIndex!=-1)
	//{
	//	if(!CheckRel(m_RelEdit)) return;
	//	p_UsrRelTableList[iSelRelIndex].rel=m_RelEdit;
	//	p_UsrRelTableList[iSelRelIndex].note=m_RelNoteEdit;
	//	ShowRel();
	//}
	//else
	//{
	//	AfxMessageBox("请先选择要修改的关系式");
	//}
	//////////////////////////////////////////////////////////////////////////
	
    //////////////////////////////////////////////////////////////////////////
    //3
	if (iSelRelIndex!=-1)
	{
		if (g_RelCollection.EditRel(iSelRelIndex,m_RelEdit,m_RelNoteEdit))
		{
			ShowRel();
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

	//////////////////////////////////////////////////////////////////////////
	//if (iSelRelIndex!=-1)
	//{
	//	bool    bolCheck;
	//	CRel    new_CRel((ProSolid)mdl,m_RelEdit,bolCheck);
	//	if (bolCheck==true)
	//	{
	//		int flag=0;
	//		for (int j=0;j<p_CRelItemList.GetCount();j++ )
	//		{
	//			if(p_CRelItemList[j]==new_CRel && j!=iSelRelIndex)
	//			{
	//				flag=1;
	//				break;
	//			}
	//		}
	//		if(flag==0) 
	//		{
	//			p_CRelItemList[iSelRelIndex].GetRelString(new_CRel.m_strNote);
	//			p_CRelItemList.RemoveAt(iSelRelIndex);
	//			iSelRelIndex=(int)p_CRelItemList.Add(new_CRel);
	//		}
	//	}
	//	else 
	//	{
	//		AfxMessageBox(new_CRel.m_strError);
	//	}
	//}
	//else
	//{
	//	AfxMessageBox("请先选择要修改的关系式");
	//}
	//////////////////////////////////////////////////////////////////////////
	
}

void CRelEditDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnChar(nChar, nRepCnt, nFlags);
}
