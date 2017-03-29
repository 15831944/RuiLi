// PartAlterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PartAlterDlg.h"
#include "ProWindows.h"
#include "Compute.h"
#include "MyDatabase.h"
#include "afxtempl.h"
#include "ProSolid.h"
#include "ProDimension.h"
#include "ProUtil.h"
#include "CommonFuncClass.h"
#include "ListCtrlEx.h"
#include "DetailInfoClass.h"

extern CMyDatabase m_db;
//CListCtrl m_groupdimen;

extern CPtrArray p_classarray;
extern CStringArray RelationSet;
extern CArray<Database,Database> Database_struc;
extern CArray<StandardInfo,StandardInfo> StandardInfo_struc;
extern CArray<Database,Database> Database_struc_temp;
extern CArray<JuBing,JuBing> JuBing_struc;
// CPartAlterDlg 对话框

#define WM_LEACH_INS       WM_USER + 3000

IMPLEMENT_DYNAMIC(CPartAlterDlg, CDialog)
CPartAlterDlg::CPartAlterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartAlterDlg::IDD, pParent)
{
	IsKong=TRUE;
	First=TRUE;
}

CPartAlterDlg::~CPartAlterDlg()
{
}

void CPartAlterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DIMEN, m_dimension);
	//DDX_Control(pDX, IDC_LIST_GROUPDIMEN, m_groupdimen);
	DDX_Control(pDX, IDC_LIST_STANDARD, m_standard);
}


BEGIN_MESSAGE_MAP(CPartAlterDlg, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	//ON_BN_CLICKED(IDC_BUTTON_GEN, OnBnClickedButtonGen)
	//ON_BN_CLICKED(IDC_BUTTON_SELECT, OnBnClickedButtonSelect)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DIMEN, OnNMClickListDimen)
	//ON_WM_CLOSE()
	ON_MESSAGE(WM_LEACH_INS,OnLeachIns)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STANDARD, OnNMDblclkListStandard)
END_MESSAGE_MAP()


// CPartAlterDlg 消息处理程序

void CPartAlterDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRepaint(PRO_VALUE_UNUSED);
	// TODO: 在此添加消息处理程序代码
}

BOOL CPartAlterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	CString str;					      
	ProSolid solid;
	ProError status;
	int w_id,s;
	BOOL IsHave=FALSE;
	JuBing uu;
	ProMdlType p_type;
	CString name,post;
	CCompute compute;
	CCommonFuncClass commonfun;

	ListView_SetExtendedListViewStyle(m_dimension.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_dimension.InsertColumn(0,"可变尺寸",LVCFMT_LEFT,95,-1);
	m_dimension.InsertColumn(1,"数值",LVCFMT_LEFT,80,-1);
	m_dimension.InsertColumn(2,"上偏差",LVCFMT_LEFT,70,-1);
	m_dimension.InsertColumn(3,"下偏差",LVCFMT_LEFT,70,-1);
	m_dimension.SetReadOnlyColumns(0,false);
    
	ListView_SetExtendedListViewStyle(m_standard.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_dimension.DeleteAllItems();
	JuBing_struc.RemoveAll();		   //应该在下载的时候读取 
	status=ProMdlCurrentGet((ProMdl*)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("对不起,您还没有打开模型!");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	status=ProMdlTypeGet((ProMdl)solid,&p_type);
	if (p_type==PRO_MDL_ASSEMBLY)
	{
		AfxMessageBox("对不起，当前模型是组件类型！");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}   
	if (p_type==PRO_MDL_PART)
	{
		uu.Productsolid=solid;				  //我想应该在打开的时候用；
		uu.solid=solid;
		JuBing_struc.Add(uu);
        s=commonfun.IsBasicInfoHave(solid);
		if (s==-1)
		{
			AfxMessageBox("无法得到这个零件的模型信息！");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
		partnum=((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum;
		if (partnum=="-1")
		{
			AfxMessageBox("这个是非模型，无法变型！");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
		Database_struc_temp.RemoveAll();
		if(!compute.GetVariantForPart(solid,partnum,TRUE))//把主变量读到临时结构中
		{
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
		if(!compute.ObtainValueFromMdlTemp(solid))		 //也需要读入偏差信息
		{
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
	}    
	ProMdlWindowGet((ProMdl)solid,&w_id);
	status = ProWindowActivate(w_id);
	int t=0;
	//显示变量尺寸
	for (int i=0;i<Database_struc_temp.GetSize();i++)	 //有问题
	{
		str=Database_struc_temp[i].name;
		int indexNo=m_dimension.InsertItem(LVIF_TEXT|LVIF_PARAM,t,Database_struc_temp[i].name,
			0,0,0,Database_struc_temp[i].number);
		m_dimension.SetItemText(indexNo,1,Database_struc_temp[i].value);
		m_dimension.SetItemText(indexNo,2,Database_struc_temp[i].up);	 //上偏差
		m_dimension.SetItemText(indexNo,3,Database_struc_temp[i].down);	 //下偏差
		UpdateData(FALSE);
		t++;
	}
    if (!GetPartStandard(&m_standard,partnum)) 
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
    }
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPartAlterDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::DestroyWindow();
}

void CPartAlterDlg::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
	CCompute compute;
	ProSolid solid;
	ProError status;
	BOOL IsOk=TRUE;
    status=ProMdlCurrentGet((ProMdl*)&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("无法重生！");
		return;
	}
	CString str,teststrucstring;
	RelationSet.RemoveAll();
	Database_struc.RemoveAll();
	if (!compute.GetVariantForPart(solid,partnum,FALSE))
	{
		AfxMessageBox("变量读取错误");
		return;
	}
	if(!compute.GetRelationForPart(partnum))
	{
		AfxMessageBox("关系式读取错误");
		return;
	}
	//临时的结构赋给正式的结构
	for (int i=0;i<m_dimension.GetItemCount();i++)
	{
		Database_struc[i].value=m_dimension.GetItemText(i,1);
		Database_struc[i].up=m_dimension.GetItemText(i,2);
		Database_struc[i].down=m_dimension.GetItemText(i,3);
		Database_struc[i].IsOk=TRUE;
	}
	//重生模型
	for (int j=0;j<Database_struc.GetSize();j++)
	{
		if (Database_struc[j].IsOrNot)
		{
			compute.UpdateAllRelation(Database_struc[j].PARADIMSYMBOL,j);
		}
	}
	//判断是否已经全部计算完了
	for (int j=0;j<RelationSet.GetSize();j++)
	{
		if(compute.IsOnlyOne(j)!="结束")
		{
			AfxMessageBox("你建的关系式有误，无法为你计算！");
			return;
		}
	}
	if(!compute.AsmGeneration((ProSolid)solid))
	{
		return;
	}

	ProWindowRepaint(PRO_VALUE_UNUSED);
	status = ProMdlDisplay(solid);
	status = ProWindowActivate(PRO_VALUE_UNUSED);
	CDialog::DestroyWindow();
}


/************************************************************************/
/* 对话框可扩展函数                                                     */
/************************************************************************/
void CPartAlterDlg::ToggleSize()
{
	CRect rect;
	CString str;
	if (m_btogglesize)
	{
		str="<< &返回";
		rect=m_rectfull;
	}
	else
	{
		str="&系列化尺寸>>";
		rect=m_recthalf;
	}
	SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),SWP_NOZORDER|SWP_NOMOVE);
	m_button.SetWindowText(str);
	if (First)
	{
		m_btogglesize=!m_btogglesize;
		First=FALSE;
	}
	if (!IsKong)
	{
		m_btogglesize=!m_btogglesize;
	}

}
void CPartAlterDlg::OnNMClickListDimen(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	ProSolid solid;
	ProError status;
	ProDimension haha;
	CString str;
	BOOL IsHave=FALSE;
	CString type;
	status=ProMdlCurrentGet((ProMdl*)&solid);
	POSITION pos = m_dimension.GetFirstSelectedItemPosition();
		while (pos)
		{
			IsHave=TRUE;
			int nItem = m_dimension.GetNextSelectedItem(pos);
			if (Database_struc_temp[nItem].type!="dim")		   //如果是参数，则不显示尺寸
			{
				ProWindowRepaint(PRO_VALUE_UNUSED);
				ProWindowRefresh(PRO_VALUE_UNUSED);
				return;
			}
			else
			{
				str=Database_struc_temp[nItem].idnum;
				str.Delete(0,1);
				haha.id=atoi(str.GetBuffer());
				haha.type=PRO_DIMENSION;
				haha.owner=((ProMdl)solid);
				ProWindowRepaint(PRO_VALUE_UNUSED);
				ProDimensionShow(&haha,NULL,NULL,NULL);
				ProWindowRefresh(PRO_VALUE_UNUSED);
			}	
		}

	if (!IsHave)
	{
		return;
	}
	*pResult = 0;
}
LRESULT CPartAlterDlg::OnLeachIns(WPARAM wparam,LPARAM lparam)
{
	int m_nItem=(int)wparam;
	int m_nSubItem=(int)lparam;
	CString str=m_dimension.GetItemText(m_nItem,m_nSubItem);
	if (1==m_nSubItem)
	{
		//AfxMessageBox(str);
	}
	return 1;
}
/************************************************************************/
/* 得到零件的系列尺寸                                                   */
/************************************************************************/
BOOL CPartAlterDlg::GetPartStandard(CListCtrl *m_list,CString MdlNum)
{
     CString sql,str;
	 int Num;
	 BOOL First=TRUE;
	 StandardInfo uu;
	 CStringArray dimnamearray,dimvaluearray;
	 StandardInfo_struc.RemoveAll();
	 sql.Format("Select * From NPartStandardTable where MdlNum=%s",MdlNum);
	 int Z=m_db.Query(sql);
	 if (Z==-1) 
	 {
		 return FALSE;
	 }
	 else
		 if (Z!=0) 
	 {
		 m_db.MoveBegin();                                        //读取系列信息
		 do {
			 uu.MdlNum=MdlNum;
			 str=m_db.getString("Name");
			 uu.StandardName=str;
			 uu.StandardNum=m_db.getInt("StandardNum");
			 uu.Note=m_db.getString("Note");
			 if (First) 
			 {
			     First=FALSE;
			     StandardInfo_struc.Add(uu);
			 }
			 else
			 {
			     if (Num!=uu.StandardNum) 
			     {
			          StandardInfo_struc.Add(uu);
			     }
			     else
			          continue;
			 }
			 Num=m_db.getInt("StandardNum");
		 } while(m_db.MoveNext());
	 }
	 //读取零件的尺寸名称
     sql.Format("Select * From NPartVariantTable where PrtNum=%s",MdlNum);
	 Z=m_db.Query(sql);
	 if (Z==-1) 
	 {
		 return FALSE;
	 }
	 else
		 if (Z!=0) 
		 {
			 m_db.MoveBegin();                                        //读取系列信息
			 do {
				 str=m_db.getString("Name");
				 dimnamearray.Add(str);
			 } while(m_db.MoveNext());
		 }
	 m_list->DeleteAllItems();
	 do {	
	 } while(m_list->DeleteColumn(0)!=0);

	 m_list->InsertColumn(0,"编号",LVCFMT_LEFT,40,-1);
	 m_list->InsertColumn(1,"系列名称",LVCFMT_LEFT,60,-1);
     
	 for (int i=0;i<dimnamearray.GetSize();i++)
	 {
		m_list->InsertColumn(i+2,dimnamearray[i],LVCFMT_LEFT,60,-1);   
	 }
	 //显示每个系列的尺寸
	 for (int i=0;i<StandardInfo_struc.GetSize();i++)
	 {
		 sql.Format("Select * From NPartStandardTable where MdlNum=%s and StandardNum=%d",MdlNum,StandardInfo_struc[i].StandardNum);
		 int Z=m_db.Query(sql);
		 if (Z==-1) 
		 {
			 return FALSE;
		 }
		 else
			 if (Z!=0) 
			 {
				dimvaluearray.RemoveAll();
				 m_db.MoveBegin();
				 do {
					str=m_db.getString("Value");
					dimvaluearray.Add(str);
				 } while(m_db.MoveNext());
			 }
         //显示在列表中
	    str.Format("%d",i+1);
		int indexNo=m_list->InsertItem(i,str);
		m_list->SetItemText(indexNo,1,StandardInfo_struc[i].StandardName);
        for(int j=0;j<dimvaluearray.GetSize();j++)
		{   
			m_list->SetItemText(indexNo,j+2,dimvaluearray[j]);
		}
	 }
     return TRUE;
}
void CPartAlterDlg::OnNMDblclkListStandard(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL IsSelect=FALSE;
	CString sql,str;
    int Num;
	for (int i=0;i<m_standard.GetItemCount();i++)
	{
		if (m_standard.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsSelect=TRUE;
			Num=i;           //是零件号
			break;
		}
	}
	if (!IsSelect)
	{
		return;
	}
	else
	{
		for (int j=0;j<m_dimension.GetItemCount();j++)
		{
            str=m_standard.GetItemText(Num,j+2);
			m_dimension.SetItemText(j,1,str);
		}
	}
	*pResult = 0;
}
