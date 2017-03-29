// UploadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <ProToolkit.h>
#include "variantdesign.h"
#include <ProMenuBar.h>
#include "UploadDlg.h"
#include "ProWindows.h"
#include "afxtempl.h"
#include "MyDatabase.h"
#include "Compute.h"
#include "CommonFuncClass.h"
#include "ProUtil.h"
#include "ProMdl.h"
#include "ProSolid.h"
#include "DetailDlg.h"
#include "ProductMenberBasicInfoDlg.h"
#include "UpdateDlg.h"
#include "PrtAsmAuditDlg.h"

extern CUploadDlg  uploaddlg;
extern CMyDatabase m_db;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CArray <Database,Database> Database_struc_temp;
extern CArray <Database,Database> Database_struc;
extern CPtrArray p_classarray;
CPrtAsmAuditDlg *prtasmauditdlg=NULL;


ProError SolidFeatGet(ProSolid solid,ProFeature **p_data,BOOL nFlag);
// CUploadDlg 对话框

IMPLEMENT_DYNAMIC(CUploadDlg, CDialog)
CUploadDlg::CUploadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUploadDlg::IDD, pParent)
{
	
}

CUploadDlg::~CUploadDlg()
{

}

void CUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ASM, m_asmmodeltree);
	DDX_Control(pDX, IDC_LIST_DIM, m_dim);
}


BEGIN_MESSAGE_MAP(CUploadDlg, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_ASM, OnNMDblclkTreeAsm)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_TREE_ASM, OnNMClickTreeAsm)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DIM, OnNMClickListDim)
	ON_BN_CLICKED(IDC_BUTTON_PDTINFO, OnBnClickedButtonPdtinfo)
	ON_BN_CLICKED(IDC_BUTTON_FALSE, OnBnClickedButtonFalse)
END_MESSAGE_MAP()


// CUploadDlg 消息处理程序

BOOL CUploadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CCompute compute;
	CCommonFuncClass commonfun;
	//去掉进程中没有的基本信息
	commonfun.IsBasicInfoAlreadyHave();
	commonfun.RemoveNotInSession();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	m_ImageList.Create(16, 16, ILC_COLOR8, 0, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ASM));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_PRT));
	m_asmmodeltree.SetImageList(&m_ImageList,TVSIL_NORMAL);

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FALSE)->EnableWindow(FALSE);
	ProError status;
	ProMdlType p_type;
	CString sql,str;
	status=ProMdlCurrentGet((ProMdl*)&Productsolid);
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("对不起，您没有打开装配模型！");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	status=ProMdlTypeGet((ProMdl)Productsolid,&p_type);
	if (p_type!=PRO_ASSEMBLY)
	{
	    AfxMessageBox("这不是产品，请用零件审核");
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	else
	{
		s=commonfun.IsBasicInfoHave(Productsolid);
		if (s==-1)
		{
			AfxMessageBox("无法得到这个产品的信息，无法为你审核");
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
	}

	m_dim.SetExtendedStyle(LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_dim.InsertColumn(0,"可变尺寸",LVCFMT_LEFT,95,-1);
	m_dim.InsertColumn(1,"数值",LVCFMT_LEFT,80,-1);
	m_dim.InsertColumn(2,"上偏差",LVCFMT_LEFT,70,-1);
	m_dim.InsertColumn(3,"下偏差",LVCFMT_LEFT,70,-1);
	m_dim.InsertColumn(4,"备注",LVCFMT_LEFT,100,-1);

	//显示产品的可变参数
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum!=-1)
	{
		if (!ListDim(s,"asm",&m_dim))
		{
			this->SendMessage(WM_CLOSE,0,0);
			return FALSE;
		}
	}
	//得到产品的模型树
	commonfun.AddProductToTree(Productsolid,&m_asmmodeltree);
	//判断这个零件有没有设计完成
	if (!IsProductCanAudit(s))
	{
		str="还有零部件没有审核，是否审核还没有审核的零部件";
		if(AfxMessageBox(str,MB_YESNO)==IDYES)
		{
             //跳出一个对话框；
			if (prtasmauditdlg!=NULL)
			{
				delete prtasmauditdlg;
			}
			prtasmauditdlg = new CPrtAsmAuditDlg;
			prtasmauditdlg->ProductSolid=Productsolid;
			prtasmauditdlg->Create(IDD_DIALOG_PRTNDAUDIT,NULL);
			prtasmauditdlg->ShowWindow(SW_SHOW);
		}
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	return TRUE;
}

void CUploadDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProWindowRepaint(PRO_VALUE_UNUSED);
}

void CUploadDlg::OnBnClickedOk()
{
	CTime time=CTime::GetCurrentTime();
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime=time.Format("%Y-%m-%d %H:%M:%S");
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="审核通过";

	m_db.m_pConnection->Close();
	m_db.ReOpen();
	m_db.m_pConnection->BeginTrans();
	try
	{
		if (!AuditFunc(s))
		{
			m_db.m_pConnection->RollbackTrans();
			AfxMessageBox("提交失败");
		}//Update数据库
		else
		{
			m_db.m_pConnection->CommitTrans();
			AfxMessageBox("提交成功！");
		}

	}
	catch (_com_error e) 
	{
		m_db.m_pConnection->RollbackTrans();
		AfxMessageBox("提交失败！");
	}
}

void CUploadDlg::OnNMDblclkTreeAsm(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CUploadDlg::OnBnClickedCancel()
{
	ProError status;
	status = ProMdlDisplay((ProMdl)Productsolid);
	if(status!= PRO_TK_NO_ERROR)
	{
		return;
	}
	int w_id;
	ProWindowCurrentGet(&w_id);
	ProWindowActivate(w_id);
	CDialog::DestroyWindow();
}
void CUploadDlg::OnNMClickTreeAsm(NMHDR *pNMHDR, LRESULT *pResult)
{
	//HTREEITEM hItem;
	//UINT uFlags=0;
	//CPoint point;
	//CString Name,str;
	//ProName w_name;			     
	//ProError status;
	//ProMdl  mdl;
	//ProMdlType type;
	//BOOL IsHave=FALSE;
	//point= GetCurrentMessage()->pt;
	//m_asmmodeltree.ScreenToClient(&point);
	//hItem =m_asmmodeltree.HitTest(point, &uFlags);
	//if (hItem!=NULL && (uFlags&TVHT_ONITEM))
	//{
	//	Name=m_asmmodeltree.GetItemText(hItem);
	//	//UpdateData(FALSE);
	//}
	//else
	//	return;
	//if(m_asmmodeltree.GetChildItem(hItem)!=NULL)
	//{
	//	type=PRO_MDL_ASSEMBLY;
	//}
	//else
	//{  type=PRO_MDL_PART;
	//}
	//int k=Name.Find(".");
	//str=Name.Left(k);
	//ProStringToWstring(w_name,str.GetBuffer());
	//status=ProMdlInit(w_name,type,&mdl);
	//if (status!=PRO_TK_NO_ERROR)
	//{
	//	return;
	//}
	//status = ProMdlDisplay(mdl);
	//int w_id;
	//ProWindowCurrentGet(&w_id);
	//ProWindowActivate(w_id);
	*pResult = 0;
}

/************************************************************************/
/* 判断这个产品是否可以审核                                             */
/************************************************************************/
BOOL CUploadDlg::IsProductCanAudit(int s)
{
	 CString sql,str;
	 sql.Format("SELECT * FROM NProductInsInfoTable WHERE PdtNum=%d",((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	 int Z=m_db.Query(sql);
	 if (Z==-1 || Z==0)
	 {
		 AfxMessageBox("没有这个产品，无法审核");
		 return FALSE;
	 }
	 else
	 {
		 m_db.MoveBegin();
		 do {
		 	str=m_db.getString("FinishState");
			if (str=="未完成")
			{
				AfxMessageBox("这个产品还没有设计完成，请先完成设计，再审核");
				return FALSE;
			}
			str=m_db.getString("AuditState");
			if (str=="审核通过")
			{
				AfxMessageBox("这个产品已经审核通过，无须再审核");
				return FALSE;
			}
		 } while(m_db.MoveNext());
	 }
	 //判断这个以下的零部件是否已经审核
	 BOOL IsAudit;
	 CString FileName;
	 if (!AffirmPartIsAlreadyAudit(((DetailInfoClass *)p_classarray.GetAt(s))->SolidID,&IsAudit,&FileName))
	 {
         return FALSE;
	 }
	 if (!IsAudit)
	 {
		 //str.Format("文件%s还没有审核，请先审核",FileName);
		 //AfxMessageBox(str);
		 return FALSE;
	 }
	 return TRUE;
}
/************************************************************************/
/* 判断部件或者组件是否有审核                                           */
/************************************************************************/
BOOL CUploadDlg::AffirmPartIsAlreadyAudit(int SolidID,BOOL *IsAudit,CString *FileName)
{
	CString sql,str;
	int Z;
	CStringArray SolidIDArray;
	CStringArray TypeArray;
	SolidIDArray.RemoveAll();
	TypeArray.RemoveAll();
	sql.Format("SELECT * FROM NProductBOMTable WHERE PdtNum=%d",SolidID);
	Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		return FALSE;
	}
	else
	{
         m_db.MoveBegin();
		 do {
		 	str.Format("%d",m_db.getInt("PrtNum"));
			SolidIDArray.Add(str);
			str=m_db.getString("Type");
			TypeArray.Add(str);
		 } while(m_db.MoveNext());
	}
	for (int i=0;i<SolidIDArray.GetSize();i++)
	{
		if (TypeArray[i]=="prt")
		{
			sql.Format("SELECT * FROM NPartInsInfoTable where PrtNum=%s",SolidIDArray[i]);
			Z=m_db.Query(sql);
			if (Z==0 || Z==-1)
			{
				return FALSE;
			}
			else
			{
				m_db.MoveBegin();
				do {
					str=m_db.getString("AuditState");
					*FileName=m_db.getString("SolidFileName");
				} while(m_db.MoveNext());
				if (str=="审核通过")
				{
					*IsAudit=TRUE;
				}
				else
				{
					*IsAudit=FALSE;
				}
			}
		}
		else
			if (TypeArray[i]=="asm")
			{
				sql.Format("SELECT * FROM NProductInsInfoTable where PdtNum=%s",SolidIDArray[i]);
				Z=m_db.Query(sql);
				if (Z==0 || Z==-1)
				{
					return FALSE;
				}
				else
				{
					m_db.MoveBegin();
					do {
						str=m_db.getString("AuditState");
                        *FileName=m_db.getString("SolidFileName");
					} while(m_db.MoveNext());
					if (str=="审核通过")
					{
						*IsAudit=TRUE;
						return FALSE;
					}
					else
					{
						*IsAudit=FALSE;
						return FALSE;
					}
				}

			}
		if (!(*IsAudit))
		{
			return TRUE;
		}
	}
	return TRUE;
}

void CUploadDlg::OnNMClickListDim(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	ProDimension haha;
	CCompute compute;
	ProAsmcomppath comppath;
	ProSolid solidtemp;
	CString str;
	int k,postID;
	BOOL IsHave=FALSE;
	for (k=0;k<m_dim.GetItemCount();k++)
	{
		if(m_dim.GetItemState(k,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsHave=TRUE;
			if (Database_struc_temp[k].type=="dim")
			{
				str=Database_struc_temp[k].idnum;
				break;
			}
			else 
			{
				ProWindowRepaint(PRO_VALUE_UNUSED);
				ProWindowRefresh(PRO_VALUE_UNUSED);
				return;
			}

		}
	}
	if (!IsHave)
	{
		return;
	}
	str.Delete(0,1);
	haha.id=atoi(str.GetBuffer());

	int j=Database_struc_temp[k].PARADIMSYMBOL.Find(":");
	if (j!=-1)
	{
		str=Database_struc_temp[k].PARADIMSYMBOL;
		str.Delete(0,j+1);
		postID=atoi(str.GetBuffer());
		JuBing_struc.RemoveAll();
		if (!compute.ObtainJuBing(Productsolid))
		{
			return;
		}
		for (int i=0;i<JuBing_struc.GetSize();i++)
		{
			if (postID==JuBing_struc[i].number)
			{
				comppath=JuBing_struc[i].comppath;
				solidtemp=JuBing_struc[i].solid;
				break;
			}
		}
		haha.type=PRO_DIMENSION;
		haha.owner=((ProMdl)solidtemp);
		ProWindowRepaint(PRO_VALUE_UNUSED);
		if (solidtemp==JuBing_struc[0].solid)
		{
			ProDimensionShow(&haha,NULL,NULL,NULL);
		}
		else
			ProDimensionShow(&haha,NULL,NULL,&comppath);
	}
	else
	{
		haha.type=PRO_DIMENSION;
		haha.owner=(m_menmdl);
		ProWindowRepaint(PRO_VALUE_UNUSED);
		ProDimensionShow(&haha,NULL,NULL,NULL);
	}
		ProWindowRefresh(PRO_VALUE_UNUSED);
	*pResult = 0;
}
/************************************************************************/
/*  判断组件是否可以保存                                                */
/************************************************************************/
BOOL CUploadDlg::IsAsmCanAudit(ProSolid solid,int i,CListCtrl *list)
{
	CString Type;
	Type=list->GetItemText(i,5);
	if (Type=="零件")
	{
		return TRUE;
	}
	else
	{
		if (solid!=Productsolid)			//说明是部件
		{
			for (int j=0;j<list->GetItemCount();j++)
			{
				if ((list->GetItemText(j,5)=="零件") && (list->GetItemText(j,4)=="需审核"))
				{
					AfxMessageBox("请先审核零件，再审核部件");
					return FALSE;
				}
			}
		}
		else
		{
			for (int j=1;j<list->GetItemCount();j++)
			{
				if ((list->GetItemText(j,4)=="需审核"))
				{
					AfxMessageBox("请先审核零件/部件，再审核产品");
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
BOOL CUploadDlg::ListDim(int s,CString Type,CListCtrl *list)
{
	Database_struc_temp.RemoveAll();
	CCompute compute;
	list->DeleteAllItems();
	int t=0;
	if (((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum==-1)
	{
		AfxMessageBox("这不是模型,没有可变参数");
		return TRUE;
	}
	else
	{
		if (Type=="零件")
		{
			if(!compute.GetVariantForPart(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum,TRUE))//把主变量读到临时结构中
			{
				return FALSE;
			}
			if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))		 //也需要读入偏差信息  
			{
				return FALSE;
			}

			for (int i=0;i<Database_struc_temp.GetSize();i++)	 //有问题
			{
				int indexNo=list->InsertItem(LVIF_TEXT|LVIF_PARAM,t,Database_struc_temp[i].name,
					0,0,0,Database_struc_temp[i].number);
				list->SetItemText(indexNo,1,Database_struc_temp[i].value);
				list->SetItemText(indexNo,2,Database_struc_temp[i].up);	 //上偏差
				list->SetItemText(indexNo,3,Database_struc_temp[i].down);	 //下偏差
				list->SetItemText(indexNo,4,Database_struc_temp[i].note);
				t++;
			}
		}
		else
		{
			if(!compute.ObtainAsmFromBaseTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid,((DetailInfoClass *)p_classarray.GetAt(s))->MdlNum))
			{
				//AfxMessageBox("无法得到该模型的可变尺寸");
				return FALSE;
			}
			if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(s))->solid))
			{
				return FALSE;
			}
			//显示变量尺寸
			for(int i=0;i<Database_struc_temp.GetSize();i++)	
			{
				int indexNo=list->InsertItem(LVIF_TEXT|LVIF_PARAM,t,Database_struc_temp[i].name,
					0,0,0,Database_struc_temp[i].number);
				list->SetItemText(indexNo,1,Database_struc_temp[i].value);
				list->SetItemText(indexNo,2,Database_struc_temp[i].up);	 //上偏差
				list->SetItemText(indexNo,3,Database_struc_temp[i].down);	 //下偏差
				list->SetItemText(indexNo,4,Database_struc_temp[i].note);
				t++;
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* 查看产品基本信息                                                     */
/************************************************************************/
void CUploadDlg::OnBnClickedButtonPdtinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CUpdateDlg dlg;
	dlg.s=s;
	if(dlg.DoModal()==IDOK)
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_FALSE)->EnableWindow(TRUE);
	}
}
BOOL CUploadDlg::AuditFunc(int s)
{
	CString sql;
	sql.Format("UPDATE NProductInsInfoTable Set Auditor='%s',AuditTime='%s',AuditState='%s' where PdtNum=%d",
		((DetailInfoClass *)p_classarray.GetAt(s))->Auditor,
		((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime,
		((DetailInfoClass *)p_classarray.GetAt(s))->AuditState,
		((DetailInfoClass *)p_classarray.GetAt(s))->SolidID);
	if(m_db.Execute(sql)==0)
	{
		return FALSE;
	}
		return TRUE;
}
void CUploadDlg::OnBnClickedButtonFalse()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime time=CTime::GetCurrentTime();
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditTime=time.Format("%Y-%m-%d %H:%M:%S");
	((DetailInfoClass *)p_classarray.GetAt(s))->AuditState="审核未通过";

	m_db.m_pConnection->Close();
	m_db.ReOpen();
	m_db.m_pConnection->BeginTrans();
	try
	{
		if (!AuditFunc(s))
		{
			m_db.m_pConnection->RollbackTrans();
			AfxMessageBox("提交失败");
		}//Update数据库
		else
		{
			m_db.m_pConnection->CommitTrans();
			AfxMessageBox("提交成功！");
		}

	}
	catch (_com_error e) 
	{
		m_db.m_pConnection->RollbackTrans();
		AfxMessageBox("提交失败！");
	}
	CDialog::DestroyWindow();
}
