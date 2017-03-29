// SearchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "SearchDlg.h"
#include "MyDatabase.h"
#include "FtpFile.h"
#include "ProWindows.h"
#include "ProUtil.h"
#include "afxtempl.h"
#include "CommonFuncClass.h"
#include "Compute.h"
#include "DetailInfoClass.h"
#include "PreViewDlg.h"

extern CMyDatabase m_db;
extern CStringArray RelationSet;
extern CArray<ItemHandle1,ItemHandle1> treehandle1;
extern CArray <ProMdl,ProMdl> DwgsolidArray;
extern FtpInfo Ftp_struc;
extern CPtrArray p_classarray;
extern CArray <JuBing,JuBing> JuBing_struc;
//CArray<PartDetailInfo,PartDetailInfo> PartDetail_struc;
extern CArray<Database,Database> Database_struc;
extern CArray<Database,Database> Database_struc_temp;
// CSearchDlg �Ի���

IMPLEMENT_DYNAMIC(CSearchDlg, CDialog)
CSearchDlg::CSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchDlg::IDD, pParent)
	, m_dimenname(_T(""))
	, m_all(FALSE)
{
	modelnum=-2;
	picNum=0;
	InsCode="";
	IsSelect=FALSE;
}

CSearchDlg::~CSearchDlg()
{

}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_QUERY, m_listquery);
	DDX_Text(pDX, IDC_EDIT_DIMENNAME, m_dimenname);
	DDX_Control(pDX, IDC_TREE_ASM, m_asmtree);
	DDX_Control(pDX, IDC_COMBO_PERF, m_performance);
	DDX_Check(pDX, IDC_CHECK_ALL, m_all);
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_NOTIFY(NM_CLICK, IDC_LIST_QUERY, OnNMClickListQuery)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_QUERY, OnNMDblclkListQuery)
	ON_WM_MOVE()
	ON_NOTIFY(NM_RCLICK, IDC_TREE_ASM, OnNMRclickTreeAsm)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_ALTER, OnBnClickedButtonAlter)
	ON_NOTIFY(NM_CLICK, IDC_TREE_ASM, OnNMClickTreeAsm)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_QUERY, OnNMRclickListQuery)
	ON_COMMAND(ID_PREVIEW_2D, OnPreview2d)
	
	ON_COMMAND(ID_PREVIEW_3D, OnPreview3d)
END_MESSAGE_MAP()


// CSearchDlg ��Ϣ�������


BOOL CSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	CCommonFuncClass commonfun;
	int index=m_performance.AddString("ͼ��");
	m_performance.SetCurSel(index);
	m_listquery.SetExtendedStyle(m_listquery.GetExtendedStyle()|LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);
	m_listquery.InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
	m_listquery.InsertColumn(1,"����",LVCFMT_LEFT,60,-1);
	m_listquery.InsertColumn(2,"ͼ��",LVCFMT_LEFT,80,-1);
	m_listquery.InsertColumn(3,"���",LVCFMT_LEFT,80,-1);
	m_listquery.InsertColumn(4,"�����",LVCFMT_LEFT,80,-1);
	m_listquery.InsertColumn(5,"��ע",LVCFMT_LEFT,80,-1);
	commonfun.ObtainProductModelClass(&m_asmtree);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSearchDlg::OnBnClickedButtonCancel()
{
	OnCancel();
}
//��ѯ���в�Ʒ��ť
void CSearchDlg::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sql;
	UpdateData(TRUE);
	CString str;
	CStringArray TypeNumArray;
	TypeNumArray.RemoveAll();
	m_listquery.DeleteAllItems();
	//sql.Format("SELECT * FROM NProductInsInfoTable WHERE PDMNum LIKE '%%%s%%' and PDMState='�ύ'",m_dimenname);
		sql.Format("SELECT * FROM NProductInsInfoTable WHERE PDMNum LIKE '%%%s%%' ",m_dimenname);
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
			/*if (str!="")
			{
				str.Format("%d",i+1);
				m_listquery.InsertItem(i,str);
				str=m_db.getString("Name");
				m_listquery.SetItemText(i,1,str);
				str=m_db.getString("InsCode");
				m_listquery.SetItemText(i,2,str);
				str.Format("%d",m_db.getInt("TypeNum"));
				TypeNumArray.Add(str);
				str=m_db.getString("Designer");
				m_listquery.SetItemText(i,4,str);
				str=m_db.getString("Note");
				m_listquery.SetItemText(i,5,str);
				str.Format("%d",m_db.getInt("PdtNum"));
				m_listquery.SetItemData(i,atol(str));
				i++;
			}*/

				str.Format("%d",i+1);
				m_listquery.InsertItem(i,str);
				str=m_db.getString("Name");
				m_listquery.SetItemText(i,1,str);
				str=m_db.getString("InsCode");
				m_listquery.SetItemText(i,2,str);
				str.Format("%d",m_db.getInt("TypeNum"));
				TypeNumArray.Add(str);
				str=m_db.getString("Designer");
				m_listquery.SetItemText(i,4,str);
				str=m_db.getString("Note");
				m_listquery.SetItemText(i,5,str);
				str.Format("%d",m_db.getInt("PdtNum"));
				m_listquery.SetItemData(i,atol(str));
				i++;
			

		} while(m_db.MoveNext());
		for (int j=0;j<TypeNumArray.GetSize();j++)
		{
			sql.Format("SELECT * FROM NAsmType where Number=%s",TypeNumArray[j]);
			Z=m_db.Query(sql);
			if (Z==-1 || Z==0)
			{
				continue;
			}
			else
			{
				str=m_db.getString("Name");
				m_listquery.SetItemText(j,3,str);
			}
		}
	}
	return;
}
///************************************************************************/
///* �ӽ���еõ�����������ʵ��                                           */
///************************************************************************/
//void CSearchDlg::SearchFromResult(CStringArray *stringarray)
//{
//	CString str,sql;
//	int i=1;
//	CStringArray PartMapArray;
//	if (m_listquery.GetItemCount()!=0)
//	{
//		for (int j=0;j<m_listquery.GetItemCount();j++)
//		{
//			str=m_listquery.GetItemText(j,3);				//�õ�ͼ��
//			PartMapArray.Add(str);
//		}  
//	}
//	m_listquery.DeleteAllItems();
//	for (int j=0;j<stringarray->GetSize();j++)
//	{
//		sql.Format("select * from AsmInsInfoTable where AsmNum=%d AND InsNum=%s",modelnum,(*stringarray)[j]);
//		if (m_db.Query(sql)==0)
//		{
//			continue;
//		}
//		else
//		{
//			m_db.MoveBegin();
//			do {
//				str=m_db.getString("InsCode");
//				//�ж����ͼ���Ƿ��ڽ����
//				for (int k=0;k<PartMapArray.GetSize();k++)
//				{
//					if (str==PartMapArray[k])
//					{
//						PartMapArray.RemoveAt(k);
//						str.Format("%d",i);
//						m_listquery.InsertItem(i-1,str);
//						str=m_db.getString("InsName");
//						m_listquery.SetItemText(i-1,1,str);
//						str="��Ʒ";
//						m_listquery.SetItemText(i-1,2,str);
//						str=m_db.getString("InsCode");
//						m_listquery.SetItemText(i-1,3,str);
//						str.Format("%d",m_db.getInt("AsmNum"));
//						m_listquery.SetItemText(i-1,4,str);
//						//��Ҫ������ͣ����ʲô��
//						i++;
//						break;
//					}
//					else
//						continue;
//				}
//			} while(m_db.MoveNext());
//		}
//	}
//	return;
//}
void CSearchDlg::OnNMClickListQuery(NMHDR *pNMHDR, LRESULT *pResult)
{
	//�ټ����ذ�ť�������أ�
	*pResult = 0;
}

void CSearchDlg::OnNMDblclkListQuery(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CSearchDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
}
void CSearchDlg::OnNMRclickTreeAsm(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}
void CSearchDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CSearchDlg::OnBnClickedButtonAlter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str,sql;
	//�õ���ʵ���ĳߴ�
	for (int i=0;i<m_listquery.GetItemCount();i++)
	{
		if (m_listquery.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			InsCode.Format("%d",m_listquery.GetItemData(i));      //�õ���ƷID���Ǵ�VDSϵͳ������
			IsSelect=TRUE;
			break;
		}
	}
	if (!IsSelect)
	{
		AfxMessageBox("��ѡ����Ҫ�Ĳ�Ʒ��");
		return;
	}
	OnOK();

}
void CSearchDlg::OnNMClickTreeAsm(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCompute compute;
	HTREEITEM hItem;
	CString str,sql,Type;
	CStringArray SolidIDArray;
	SolidIDArray.RemoveAll();
	int i=0,num;
	UINT uFlags=0;
	CPoint point;
	m_listquery.DeleteAllItems();

	point= GetCurrentMessage()->pt;
	m_asmtree.ScreenToClient(&point);
	hItem =m_asmtree.HitTest(point, &uFlags);
	if (hItem!=NULL && (uFlags&TVHT_ONITEM))
	{
		num=(int)m_asmtree.GetItemData(hItem);
		sql.Format("select * from NAsmType WHERE Number=%d",num);
		int Z=m_db.Query(sql);
		if (Z==-1 || Z==0)
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
		//sql.Format("SELECT * FROM NProductInsInfoTable WHERE TypeNum=%d and PDMState='�ύ'",num);
		sql.Format("SELECT * FROM NProductInsInfoTable WHERE TypeNum=%d",num);
		Z=m_db.Query(sql);
		if (Z==-1)
		{
			return;
		}
		else
			if (Z!=0)
			{
				m_db.MoveBegin();
				do 
				{
					/*str=m_db.getString("PDMNum");
					if (str!="")
					{
						str.Format("%d",i+1);
						m_listquery.InsertItem(i,str);
						str=m_db.getString("Name");
						m_listquery.SetItemText(i,1,str);
                        str=m_db.getString("PDMNum");
						m_listquery.SetItemText(i,2,str);
						m_listquery.SetItemText(i,3,Type);
						str=m_db.getString("Designer");
						m_listquery.SetItemText(i,4,str);
						str=m_db.getString("Note");
						m_listquery.SetItemText(i,5,str);
						str.Format("%d",m_db.getInt("PdtNum"));
						m_listquery.SetItemData(i,atol(str));
					}*/
					str=m_db.getString("PDMNum");
				
						str.Format("%d",i+1);
						m_listquery.InsertItem(i,str);
						str=m_db.getString("Name");
						m_listquery.SetItemText(i,1,str);
						str=m_db.getString("InsCode");
						m_listquery.SetItemText(i,2,str);
						m_listquery.SetItemText(i,3,Type);
						str=m_db.getString("Designer");
						m_listquery.SetItemText(i,4,str);
						str=m_db.getString("Note");
						m_listquery.SetItemText(i,5,str);
						str.Format("%d",m_db.getInt("PdtNum"));
						m_listquery.SetItemData(i,atol(str));
					
				} while(m_db.MoveNext());
			}
	}
	*pResult = 0;
}


void CSearchDlg::OnNMRclickListQuery(NMHDR *pNMHDR, LRESULT *pResult)
{
	LVHITTESTINFO lvhti;
	CString str;
	CPoint point = GetCurrentMessage()->pt;
	CPoint pt=point;
	//str.Format("x=%d,y=%d",point.x,point.y);
	m_listquery.ScreenToClient(&point);
	lvhti.pt = point;
	m_listquery.SubItemHitTest(&lvhti);
	if (lvhti.flags & LVHT_ONITEMLABEL)
	{
		int i=lvhti.iItem;
		m_listquery.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
		InsCode.Format("%d",m_listquery.GetItemData(i));
	}
	CMenu contextMenu;
	CMenu *pMenu;
	contextMenu.LoadMenu(IDR_MENU_RIGHTADD);
	//ClientToScreen(&pt);
	pMenu=contextMenu.GetSubMenu(0);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTALIGN,pt.x,pt.y,this);
	*pResult = 0;
}



void CSearchDlg::OnPreview2d()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPreViewDlg dlg;
	dlg.m_ModalFlag=4;//��Ʒʵ��
	dlg.m_2DFlag=1;//��άͼ
	dlg.m_strID=InsCode;//ģ��ID
	dlg.DoModal();
}



void CSearchDlg::OnPreview3d()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPreViewDlg dlg;
	dlg.m_ModalFlag=4;//��Ʒʵ��
	dlg.m_2DFlag=0;//��άͼ
	dlg.m_strID=InsCode;//ģ��ID
	dlg.DoModal();
}
