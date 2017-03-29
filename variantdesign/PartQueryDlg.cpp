// PartQueryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PartQueryDlg.h"
#include "ProWindows.h"
#include "MyDatabase.h"
#include "FtpFile.h"
#include "ProModelitem.h"
#include "ProMdl.h"
#include "ProParameter.h"
#include "ProParamval.h"
#include <ProUtil.h>
#include "ProSolid.h"
#include "afxtempl.h"
#include <ctype.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <commctrl.h>
#include "Compute.h"
#include "CommonFuncClass.h"
#include "DetailInfoClass.h"
#include "PreViewDlg.h"



extern FtpInfo Ftp_struc;
extern CMyDatabase m_db;
extern CArray<ItemHandle1,ItemHandle1> treehandle1;
CArray <ProMdl,ProMdl> DwgsolidArray;
extern CStringArray RelationSet;
extern CArray<Database,Database> Database_struc;
extern CArray<Database,Database> Database_struc_temp;
extern CArray<JuBing,JuBing> JuBing_struc;
CPtrArray p_classarray;


extern CStringArray RelationSet;
extern CArray<Database,Database> Database_struc;
// CPartQueryDlg 对话框

IMPLEMENT_DYNAMIC(CPartQueryDlg, CDialog)
CPartQueryDlg::CPartQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartQueryDlg::IDD, pParent)
	, m_performance(_T(""))
	, m_isselect(FALSE)
{
	IsSelect=FALSE;
	num=-1;
	picNum=0;
	TypeName="";
}

CPartQueryDlg::~CPartQueryDlg()
{
}

void CPartQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PART, m_listpart);
	DDX_Control(pDX, IDC_TREE_MODELGROUP, m_modeltree);
	DDX_Text(pDX, IDC_EDIT_PERF, m_performance);
	DDX_Control(pDX, IDC_COMBO_PERF, m_comboperf);
	DDX_Control(pDX, IDC_CHECK_ALL, m_check);
	DDX_Check(pDX, IDC_CHECK_ALL, m_isselect);
}


BEGIN_MESSAGE_MAP(CPartQueryDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_NOTIFY(NM_CLICK, IDC_TREE_MODELGROUP, OnNMClickTreeModelgroup)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PART, OnNMClickListPart)
	ON_WM_PAINT()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PART, OnNMRclickListPart)
	ON_COMMAND(ID_PREVIEW_2D, OnPreview2d)
	ON_COMMAND(ID_PREVIEW_3D, OnPreview3d)
END_MESSAGE_MAP()


// CPartQueryDlg 消息处理程序

void CPartQueryDlg::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CPartQueryDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);

	// TODO: 在此添加消息处理程序代码
}

BOOL CPartQueryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	CCommonFuncClass commonfun;
	int index=m_comboperf.AddString("图号");
	m_comboperf.SetCurSel(index); 
	m_listpart.SetExtendedStyle(m_listpart.GetExtendedStyle()|LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);
	m_listpart.InsertColumn(0,"序号",LVCFMT_LEFT,40,-1);
	m_listpart.InsertColumn(1,"实例名",LVCFMT_LEFT,60,-1);
	m_listpart.InsertColumn(2,"图号",LVCFMT_LEFT,100,-1);
	m_listpart.InsertColumn(3,"类型",LVCFMT_LEFT,60,-1);
	m_listpart.InsertColumn(4,"设计者",LVCFMT_LEFT,100,-1);
	m_listpart.InsertColumn(5,"备注",LVCFMT_LEFT,100,-1);
	commonfun.ObtainPartModelClass(&m_modeltree);


	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPartQueryDlg::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sql;
	UpdateData(TRUE);
	CString str,str1;
	CStringArray TypeNumArray;
	TypeNumArray.RemoveAll();
	m_listpart.DeleteAllItems();
	//sql.Format("SELECT * FROM NPartInsInfoTable WHERE PDMNum LIKE '%%%s%%' and PDMState='提交'",m_performance);
	sql.Format("SELECT * FROM NPartInsInfoTable WHERE PDMNum LIKE '%%%s%%' ",m_performance);
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		return;
	}
	if (Z==0)
	{
		return;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			//str=m_db.getString("PDMNum");
		/*	if (str!="")
			{
			   str1.Format("%d",i+1);
			   m_listpart.InsertItem(i,str1);
               m_listpart.SetItemText(i,2,str);
			   str=m_db.getString("Name");
			   m_listpart.SetItemText(i,1,str);
			   str.Format("%d",m_db.getInt("TypeNum"));
			   TypeNumArray.Add(str);
			   str=m_db.getString("Designer");
			   m_listpart.SetItemText(i,4,str);
			   str=m_db.getString("Note");
			   m_listpart.SetItemText(i,5,str);
			   str.Format("%d",m_db.getInt("PrtNum"));
			   m_listpart.SetItemData(i,atol(str));
			   i++;
			}*/

			   str=m_db.getString("InsCode");
				str1.Format("%d",i+1);
				m_listpart.InsertItem(i,str1);
				m_listpart.SetItemText(i,2,str);
				str=m_db.getString("Name");
				m_listpart.SetItemText(i,1,str);
				str.Format("%d",m_db.getInt("TypeNum"));
				TypeNumArray.Add(str);
				str=m_db.getString("Designer");
				m_listpart.SetItemText(i,4,str);
				str=m_db.getString("Note");
				m_listpart.SetItemText(i,5,str);
				str.Format("%d",m_db.getInt("PrtNum"));
				m_listpart.SetItemData(i,atol(str));
				i++;
		

		} while(m_db.MoveNext());
		for (int j=0;j<TypeNumArray.GetSize();j++)
		{
			sql.Format("SELECT * FROM NPartType where Number=%s",TypeNumArray[j]);
			Z=m_db.Query(sql);
			if (Z==-1 || Z==0)
			{
				continue;
			}
			else
			{
				str=m_db.getString("Name");
				m_listpart.SetItemText(j,3,str);
			}
		}
	}
	return;
}
/************************************************************************/
/* 从结果中得到满足条件的实例                                           */
/************************************************************************/
//void CPartQueryDlg::SearchFromResult(CStringArray *stringarray)
//{
//	CString str,sql;
//	int i=1;
//	CStringArray PartMapArray;
//	if (m_listpart.GetItemCount()!=0)
//	{
//		for (int j=0;j<m_listpart.GetItemCount();j++)
//		{
//			str=m_listpart.GetItemText(j,2);				//得到图号
//			PartMapArray.Add(str);
//		}  
//	}
//	m_listpart.DeleteAllItems();
//	for (int j=0;j<stringarray->GetSize();j++)
//	{
//		sql.Format("select * from PartInsTable where PrtNum=%d AND InsNum=%s",num,(*stringarray)[j]);
//		if (m_db.Query(sql)==0)
//		{
//			continue;
//		}
//		else
//		{
//			m_db.MoveBegin();
//			do {
//				str=m_db.getString("InsCode");
//				判断这个图号是否在结果中
//				for (int k=0;k<PartMapArray.GetSize();k++)
//				{
//					if (str==PartMapArray[k])
//					{
//						PartMapArray.RemoveAt(k);
//						str.Format("%d",i);
//						m_listpart.InsertItem(i-1,str);
//						str=m_db.getString("InsName");
//						m_listpart.SetItemText(i-1,1,str);
//						str=m_db.getString("InsCode");
//						m_listpart.SetItemText(i-1,2,str);
//						str.Format("%d",m_db.getInt("PrtNum"));
//						m_listpart.SetItemText(i-1,3,str);
//						str=m_db.getString("InsCreaterName");
//						m_listpart.SetItemText(i-1,4,str);
//						str=m_db.getString("InsNote");
//						m_listpart.SetItemText(i-1,5,str);
//						i++;
//						break;
//					}
//					else
//					 continue;
//				}
//			} while(m_db.MoveNext());
//		}
//	}
//	return;
//}
void CPartQueryDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CCompute compute;
	CCommonFuncClass commonfun;
	CString FileName,DirectoryName,FullLocalPath,FullPath,path,FullLocalPath1,DwgName,strSql,DwgPath;

	for (int i=0;i<m_listpart.GetItemCount();i++)
	{
		if (m_listpart.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsSelect=TRUE;
			InsCode.Format("%d",m_listpart.GetItemData(i));
			break;
		}
	}
	OnOK();
}
void CPartQueryDlg::OnBnClickedButtonClose()
{
	OnCancel();
}

void CPartQueryDlg::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CPartQueryDlg::OnNMClickTreeModelgroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem;
	CString str,sql,Type;
	int i=1;
	int num;
	UINT uFlags=0;
	CPoint point;
	m_comboperf.ResetContent();
	treehandle1.RemoveAll();
	m_listpart.DeleteAllItems();
	point= GetCurrentMessage()->pt;
	m_modeltree.ScreenToClient(&point);
    hItem =m_modeltree.HitTest(point, &uFlags);

	if (hItem!=NULL && (uFlags&TVHT_ONITEM))
	{
		num=(int)m_modeltree.GetItemData(hItem);
		sql.Format("SELECT * FROM NPartType where Number=%d",num);
		int	Z=m_db.Query(sql);
		if (Z==0 || Z==-1)
		{
			return;
		}
		else
		{
			m_db.MoveBegin();
			do {
				Type=m_db.getString("Name");
			} while(m_db.MoveNext());
		}
		//sql.Format("select * from NPartInsInfoTable WHERE TypeNum=%d and PDMState='提交'",num);
		sql.Format("select * from NPartInsInfoTable WHERE TypeNum=%d ",num);
		Z=m_db.Query(sql);
		if (Z==-1)
		{
			return;
		}
		else
			if (Z!=0)
			{
				m_db.MoveBegin();
				do {
					  /*  str=m_db.getString("PDMNum");
						if (str!="")
						{
							str.Format("%d",i);
							m_listpart.InsertItem(i-1,str);
							str=m_db.getString("Name");
							m_listpart.SetItemText(i-1,1,str);
							str=m_db.getString("PDMNum");
							m_listpart.SetItemText(i-1,2,str);
							m_listpart.SetItemText(i-1,3,Type);

							str=m_db.getString("Designer");
							m_listpart.SetItemText(i-1,4,str);
							str=m_db.getString("Note");
							m_listpart.SetItemText(i-1,5,str);
							str.Format("%d",m_db.getInt("PrtNum"));
							m_listpart.SetItemData(i,atol(str));
							i++;
						}		*/

					
						str.Format("%d",i);
						m_listpart.InsertItem(i-1,str);
						str=m_db.getString("Name");
						m_listpart.SetItemText(i-1,1,str);
						str=m_db.getString("InsCode");
						m_listpart.SetItemText(i-1,2,str);
						m_listpart.SetItemText(i-1,3,Type);

						str=m_db.getString("Designer");
						m_listpart.SetItemText(i-1,4,str);
						str=m_db.getString("Note");
						m_listpart.SetItemText(i-1,5,str);
						str.Format("%d",m_db.getInt("PrtNum"));
						m_listpart.SetItemData(i-1,atol(str));
						i++;
					
				} while(m_db.MoveNext());
			}
	}
	*pResult = 0;
}

void CPartQueryDlg::OnNMClickListPart(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
void CPartQueryDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}
void CPartQueryDlg::OnNMRclickListPart(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*for (int i=0;i<m_listpart.GetItemCount();i++)
	{
		if (m_listpart.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsSelect=TRUE;
			InsCode.Format("%d",m_listpart.GetItemData(i));
			break;
		}
	}*/

	LVHITTESTINFO lvhti;
	CString str;
	CPoint point = GetCurrentMessage()->pt;
	CPoint pt=point;
	//str.Format("x=%d,y=%d",point.x,point.y);
	m_listpart.ScreenToClient(&point);
	lvhti.pt = point;
	m_listpart.SubItemHitTest(&lvhti);
	if (lvhti.flags & LVHT_ONITEMLABEL)
	{
		int i=lvhti.iItem;
		m_listpart.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
		InsCode.Format("%d",m_listpart.GetItemData(i));
	}
	CMenu contextMenu;
	CMenu *pMenu;
	contextMenu.LoadMenu(IDR_MENU_RIGHTADD);
	//ClientToScreen(&pt);
	pMenu=contextMenu.GetSubMenu(0);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTALIGN,pt.x,pt.y,this);
	*pResult = 0;
}




void CPartQueryDlg::OnPreview2d()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPreViewDlg dlg;
	dlg.m_ModalFlag=2;//零件实例
	dlg.m_2DFlag=1;//二维图
	dlg.m_strID=InsCode;//模型ID
	dlg.DoModal();
}

void CPartQueryDlg::OnPreview3d()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPreViewDlg dlg;
	dlg.m_ModalFlag=2;//零件实例
	dlg.m_2DFlag=0;//二维图
	dlg.m_strID=InsCode;//模型ID
	dlg.DoModal();
}
