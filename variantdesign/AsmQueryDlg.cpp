// AsmQueryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "AsmQueryDlg.h"
#include "ProWindows.h"
#include "CommonFuncClass.h"
#include "Compute.h"
#include "afxtempl.h"
#include "MyDatabase.h"
#include "FtpFile.h"
#include "PicShow.h"
//#include "Picture.h"
#include "ProFeature.h"
#include "ProAsmcomp.h"
#include "ProFeatType.h"
#include <ProAsmcomppath.h>
#include "ProductMenberBasicInfoDlg.h"
#include "ProMdl.h"
#include "ProUtil.h"
#include "ProSolid.h"
#include "ProParameter.h"
#include <ProParamval.h>
#include <ProModelitem.h>

#include "PreViewDlg.h"




// CAsmQueryDlg �Ի���
extern CArray<ItemHandle1,ItemHandle1> treehandle1;
extern CMyDatabase m_db;
extern FtpInfo Ftp_struc;
extern CArray <ProMdl,ProMdl> DwgsolidArray;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CStringArray RelationSet;
extern CArray<Database,Database> Database_struc;
extern CArray<Database,Database> Database_struc_temp;

IMPLEMENT_DYNAMIC(CAsmQueryDlg, CDialog)
CAsmQueryDlg::CAsmQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmQueryDlg::IDD, pParent)
	,p_Picture(NULL)
{
	MdlNum="0";
	picNum=0;
	Name="";
	IsSelect=FALSE;
	FileDirectory="";
	ProductSolid=NULL;
}

CAsmQueryDlg::~CAsmQueryDlg()
{
	if (p_Picture!=NULL)
	{
		p_Picture->Release();
	}
}

void CAsmQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PRODUCT_GROUP, m_producttree);
	DDX_Control(pDX, IDC_LIST_PRODUCT_MODEL, m_productlist);
	DDX_Control(pDX, IDC_STATIC_PIC, m_pic);
}


BEGIN_MESSAGE_MAP(CAsmQueryDlg, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_NOTIFY(NM_CLICK, IDC_TREE_PRODUCT_GROUP, OnNMClickTreeProductGroup)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT_MODEL, OnNMClickListProductModel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRODUCT_MODEL, OnNMDblclkListProductModel)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_PRE, OnBnClickedButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnBnClickedButtonNext)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PRODUCT_MODEL, OnNMRclickListProductModel)
	ON_COMMAND(ID_PREVIEW_2D, OnPreview2d)
	ON_COMMAND(ID_PREVIEW_3D, OnPreview3d)
END_MESSAGE_MAP()


// CAsmQueryDlg ��Ϣ�������

BOOL CAsmQueryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	CCommonFuncClass commonfun;
	ListView_SetExtendedListViewStyle(m_productlist.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_productlist.InsertColumn(0,"���",LVCFMT_LEFT,80,-1);
	m_productlist.InsertColumn(1,"ģ������",LVCFMT_LEFT,60,-1);
	m_productlist.InsertColumn(2,"ģ�ͱ��",LVCFMT_LEFT,80,-1);
	m_productlist.InsertColumn(3,"���",LVCFMT_LEFT,80,-1);
	m_productlist.InsertColumn(4,"��ע",LVCFMT_LEFT,80,-1);
	GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	commonfun.ObtainProductModelClass(&m_producttree);
	//GetAllAsmModel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CAsmQueryDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: �ڴ������Ϣ����������
}
/************************************************************************/
/* ��ģ��                                                                     */
/************************************************************************/
void CAsmQueryDlg::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    BOOL ISOK=FALSE;
	for (int i=0;i<m_productlist.GetItemCount();i++)
	{
		if (m_productlist.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			MdlNum.Format("%d",m_productlist.GetItemData(i));
			IsSelect=TRUE;
			ShowPic(atol(MdlNum),1);
			ISOK=TRUE;
		}
	}
	if (!ISOK)
	{
		AfxMessageBox("��û��ѡ��Ҫ�򿪵Ĳ�Ʒģ��!");
		return;
	}
	OnOK();
}
void CAsmQueryDlg::OnNMClickTreeProductGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCompute compute;
	HTREEITEM hItem;
	CString str,sql,Type;
	int i=1,num;
	UINT uFlags=0;
	CPoint point;
	m_productlist.DeleteAllItems();

	point= GetCurrentMessage()->pt;
	m_producttree.ScreenToClient(&point);
	hItem =m_producttree.HitTest(point, &uFlags);
	if (hItem!=NULL && (uFlags&TVHT_ONITEM))
	{
		num=(int)m_producttree.GetItemData(hItem);
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
		sql.Format("SELECT * FROM NAsmManageTable WHERE TypeNum=%d",num);
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
					str.Format("%d",i);
					m_productlist.InsertItem(i-1,str);
					str=m_db.getString("Name");
					m_productlist.SetItemText(i-1,1,str);
					m_productlist.SetItemData(i-1,m_db.getInt("Number"));
					str=m_db.getString("Code");
					m_productlist.SetItemText(i-1,2,str);
					m_productlist.SetItemText(i-1,3,Type);
					str=m_db.getString("Note");
					m_productlist.SetItemText(i-1,4,str);
					i++;
				} while(m_db.MoveNext());
			}
	}
	*pResult = 0;
}
void CAsmQueryDlg::OnNMClickListProductModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i=0;i<m_productlist.GetItemCount();i++)
	{
		if (m_productlist.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			MdlNum.Format("%d",m_productlist.GetItemData(i));
			ShowPic(atol(MdlNum),1);
			break;
		}
	}
	*pResult = 0;
}

void CAsmQueryDlg::OnNMDblclkListProductModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���Բ鿴��Ʒ�������Ϣ
	*pResult = 0;
}
void CAsmQueryDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
BOOL CAsmQueryDlg::ShowPic(int num,int Number)
{
	BYTE * pBuffer=NULL;
	CString strSql;
	int nSize=0;
	if (p_Picture!=NULL)
	{
		p_Picture->Release();
		p_Picture=NULL;
	}
	strSql.Format("SELECT * FROM NAsmPic WHERE AsmNum=%d AND Number=%d",num,Number);
	if (m_db.Query(strSql)==0)
	{
		CRect rect;
		m_pic.GetClientRect(&rect);
		m_pic.ClientToScreen(&rect);
		this->ScreenToClient(&rect);
		RedrawWindow(&rect,NULL,RDW_INVALIDATE | RDW_ERASE);
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			nSize=m_db.getBinary("File",&pBuffer);
			picNum=m_db.getInt("Number");
			break;
		} while(m_db.MoveNext());
	}
	DBOpenPic(pBuffer,nSize,&p_Picture);
	OnPaint();
	strSql.Format("SELECT * FROM NAsmPic WHERE AsmNum=%d AND Number=%d",num,picNum+1);
	if (m_db.Query(strSql)==0)
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE); 
	}
	else
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
	strSql.Format("SELECT * FROM NAsmPic WHERE AsmNum=%d AND Number=%d",num,picNum-1);
	if (m_db.Query(strSql)==0)
	{
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE); 
	}
	else
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(TRUE);
	delete [] pBuffer;
	return TRUE;
}
/************************************************************************/
/* �Ѷ����Ƶ�����ת��ΪͼƬ                                             */
/************************************************************************/
BOOL CAsmQueryDlg::DBOpenPic(BYTE *pBuffer,int nSize,IPicture ** pic)
{			
	HGLOBAL hGlobal=::GlobalAlloc(GMEM_MOVEABLE,nSize);
	LPSTR buffer=(LPSTR)GlobalLock(hGlobal);
	memcpy(buffer,pBuffer,nSize);

	IStream *  pstm =NULL; // ��Ҫһ������stream��			
	CreateStreamOnHGlobal(buffer, TRUE, &pstm);
	if (!SUCCEEDED(OleLoadPicture(pstm,nSize,TRUE,IID_IPicture,(LPVOID*)pic)))
	{
		AfxMessageBox("��ͼƬ");
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal)	;
	pstm->Release();
	return TRUE;
}
void CAsmQueryDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CRect rect;
	if (p_Picture!=NULL)
	{
		CSize m_size;

		m_pic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		p_Picture->get_Width(&m_size.cx);
		p_Picture->get_Height(&m_size.cy);
		m_pic.MoveWindow(&rect,TRUE);
		p_Picture->Render(*GetDC(),rect.left,rect.bottom,rect.Width(),-rect.Height(),0,0,m_size.cx,m_size.cy,NULL);
	}
}

void CAsmQueryDlg::OnBnClickedButtonPre()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowPic(atol(MdlNum),picNum-1);
}

void CAsmQueryDlg::OnBnClickedButtonNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowPic(atol(MdlNum),picNum+1);
}
/************************************************************************/
/* �õ����еĲ�Ʒģ��                                                   */
/************************************************************************/
BOOL  CAsmQueryDlg::GetAllAsmModel()
{
	CString sql,str;
	sql.Format("SELECT * FROM NAsmManageTable");
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		return TRUE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			str.Format("%d",i+1);
			m_productlist.InsertItem(i,str);
			str=m_db.getString("Name");
			m_productlist.SetItemText(i,1,str);
			m_productlist.SetItemData(i,m_db.getInt("Number"));
			str=m_db.getString("Code");
			m_productlist.SetItemText(i,2,str);
			str.Format("%d",m_db.getInt("TypeNum"));
			m_productlist.SetItemText(i,3,str);
			str=m_db.getString("Note");
			m_productlist.SetItemText(i,4,str);
			i++;
		} while(m_db.MoveNext());
	}
	return TRUE;
}


void CAsmQueryDlg::OnNMRclickListProductModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LVHITTESTINFO lvhti;
	CString str;
	CPoint point = GetCurrentMessage()->pt;
	CPoint pt=point;
	//str.Format("x=%d,y=%d",point.x,point.y);
	m_productlist.ScreenToClient(&point);
	lvhti.pt = point;
	m_productlist.SubItemHitTest(&lvhti);
	if (lvhti.flags & LVHT_ONITEMLABEL)
	{
		int i=lvhti.iItem;
		MdlNum.Format("%d",m_productlist.GetItemData(i));
		ShowPic(atol(MdlNum),1);
	}
	CMenu contextMenu;
	CMenu *pMenu;
	contextMenu.LoadMenu(IDR_MENU_RIGHTADD);
	//ClientToScreen(&pt);
	pMenu=contextMenu.GetSubMenu(0);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTALIGN,pt.x,pt.y,this);
	*pResult = 0;
}







void CAsmQueryDlg::OnPreview2d()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPreViewDlg dlg;
	dlg.m_ModalFlag=3;//��Ʒģ��
	dlg.m_2DFlag=1;//��άͼ
	dlg.m_strID=MdlNum;//ģ��ID
	dlg.DoModal();
}


void CAsmQueryDlg::OnPreview3d()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPreViewDlg dlg;
	dlg.m_ModalFlag=3;//���ģ��
	dlg.m_2DFlag=0;//��άͼ
	dlg.m_strID=MdlNum;//ģ��ID
	dlg.DoModal();
}
