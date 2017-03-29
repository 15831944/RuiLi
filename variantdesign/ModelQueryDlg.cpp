// ModelQueryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "ModelQueryDlg.h"
#include "MyDatabase.h"
#include "FtpFile.h"
#include "ProMdl.h"
#include "ProWindows.h"
#include "ProUtil.h"
#include "ProSolid.h"
#include "ModelAlterDlg.h"
#include "ProParameter.h"
#include <ProParamval.h>
#include <ProModelitem.h>
#include "afxtempl.h"
#include "PicShow.h"
#include "Compute.h"
#include "ListCtrlEx.h"
#include "ProductMenberBasicInfoDlg.h"
#include "PicShow.h"
////
#include "ProFeature.h"
#include "ProAsmcomp.h"
#include "ProFeatType.h"
#include <ProAsmcomppath.h>
#include "CommonFuncClass.h"

#include "PreViewDlg.h"

#ifdef _DEBUG
#include "PreViewDlg.h"
#include "ProgressDlg.h"
#endif // _DEBUG


void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
int EraseAction();

extern CMyDatabase m_db;
extern FtpInfo Ftp_struc;
extern CArray <ProMdl,ProMdl> DwgsolidArray;
extern CArray<JuBing,JuBing> JuBing_struc;
ProError SolidFeatGet(ProSolid solid,ProFeature **p_data);
extern CArray<ItemHandle1,ItemHandle1> treehandle1;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
extern CStringArray RelationSet;
extern CArray<Database,Database> Database_struc;
extern CArray<Database,Database> Database_struc_temp;
// CModelQueryDlg �Ի���

IMPLEMENT_DYNAMIC(CModelQueryDlg, CDialog)
CModelQueryDlg::CModelQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModelQueryDlg::IDD, pParent)	
	,p_Picture(NULL)
	
{
	picNum=0;
	MdlNum="0";
	Name="";
	FileDirectory="";
	ProductSolid=NULL;
	IsSelect=FALSE;
}

CModelQueryDlg::~CModelQueryDlg()
{
	if (p_Picture!=NULL)
	{
		p_Picture->Release();
	}
}

void CModelQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODEL, m_modelresult);
	DDX_Control(pDX, IDC_TREE_PARTMODEL, m_tree);
	DDX_Control(pDX, IDC_STATIC_PIC, m_pic);
}


BEGIN_MESSAGE_MAP(CModelQueryDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_MODELDOWN, OnBnClickedButtonModeldown)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MODEL, OnNMClickListModel)
	ON_NOTIFY(NM_CLICK, IDC_TREE_PARTMODEL, OnNMClickTreePartmodel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MODEL, OnNMDblclkListModel)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnBnClickedButtonNext)
	ON_WM_ERASEBKGND()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MODEL, OnNMRclickListModel)
	ON_COMMAND(ID_PREVIEW_2D, OnPreview2d)
	ON_COMMAND(ID_PREVIEW_3D, OnPreview3d)
END_MESSAGE_MAP()


// CModelQueryDlg ��Ϣ�������

void CModelQueryDlg::OnBnClickedButtonCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CModelQueryDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: �ڴ������Ϣ����������
}

BOOL CModelQueryDlg::OnInitDialog()
{
	CDialog::OnInitDialog(); 
	CCommonFuncClass commonfun;
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	

	ListView_SetExtendedListViewStyle(m_modelresult.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_modelresult.InsertColumn(0,"���",LVCFMT_LEFT,80,-1);
	m_modelresult.InsertColumn(1,"ģ������",LVCFMT_LEFT,60,-1);
	m_modelresult.InsertColumn(2,"ģ�ͱ��",LVCFMT_LEFT,80,-1);
	m_modelresult.InsertColumn(3,"���",LVCFMT_LEFT,80,-1);
	m_modelresult.InsertColumn(4,"ע��",LVCFMT_LEFT,80,-1);
	commonfun.ObtainPartModelClass(&m_tree);
	
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CModelQueryDlg::OnBnClickedButtonModeldown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    BOOL ISOK=FALSE;
    //CString MdlNum;
	for (int i=0;i<m_modelresult.GetItemCount();i++)
	{
		if (m_modelresult.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			MdlNum.Format("%d",m_modelresult.GetItemData(i));
			IsSelect=TRUE;
			ISOK=TRUE;
		}
	}
	if (!ISOK)
	{
		AfxMessageBox("��û��ѡ��Ҫ�򿪵����!");
		return;
	}
	OnOK();
}

void CModelQueryDlg::OnNMClickListModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(FALSE);
	for (int i=0;i<m_modelresult.GetItemCount();i++)
	{
		if (m_modelresult.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			MdlNum.Format("%d",m_modelresult.GetItemData(i));
			ShowPic(atol(MdlNum),1);
		}
	}
	*pResult = 0;
}
void CModelQueryDlg::OnNMClickTreePartmodel(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hItem;
	CString str,sql;
	CCompute compute;
	UINT uFlags=0;
	CPoint point;
	int num;
	m_modelresult.DeleteAllItems();
	point= GetCurrentMessage()->pt;
	m_tree.ScreenToClient(&point);
	hItem =m_tree.HitTest(point, &uFlags);
	if (hItem!=NULL && (uFlags&TVHT_ONITEM))
	{
		num=(int)m_tree.GetItemData(hItem);
		if (!ObtianPartModelAfter(num))
		{
			 return;
		}
	}
	*pResult = 0;

}
/************************************************************************/
/* �õ���������������¼����ģ��                                       */
/************************************************************************/
BOOL CModelQueryDlg::ObtianPartModelAfter(int num)
{
	CString sql,str,Type;
	sql.Format("SELECT * FROM NPartType where Number=%d",num);
	int	Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			Type=m_db.getString("Name");
		} while(m_db.MoveNext());
	}
	sql.Format("SELECT * FROM NPartManageTable where TypeNum=%d",num);
    Z=m_db.Query(sql);
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
			m_modelresult.InsertItem(i,str);
			str=m_db.getString("Name");
			m_modelresult.SetItemText(i,1,str);
			m_modelresult.SetItemData(i,m_db.getInt("Number"));
			str=m_db.getString("Code");
			m_modelresult.SetItemText(i,2,str);
            m_modelresult.SetItemText(i,3,Type);
			str=m_db.getString("Note");
			m_modelresult.SetItemText(i,4,str);
			i++;
		} while(m_db.MoveNext());
	}
	return TRUE;
}
void CModelQueryDlg::OnNMDblclkListModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}
/************************************************************************/
/* ����ͼƬ���ݿ�                                                       */
/************************************************************************/
BOOL CModelQueryDlg::ShowPic(int num,int Number)
{
	BYTE * pBuffer=NULL;
	CRect rect;
	m_pic.GetClientRect(&rect);
	CString strSql;
	int nSize=0;
	if (p_Picture!=NULL)
	{
		p_Picture->Release();
		p_Picture=NULL;
	}
	strSql.Format("SELECT * FROM NPartPic WHERE PrtNum=%d AND Number=%d",num,Number);
	int Z=m_db.Query(strSql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
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
	strSql.Format("SELECT * FROM NPartPic WHERE PrtNum=%d AND Number=%d",num,picNum+1);
	if (m_db.Query(strSql)==0)
	{
        GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE); 
	}
	else
	    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
	strSql.Format("SELECT * FROM NPartPic WHERE PrtNum=%d AND Number=%d",num,picNum-1);
	if (m_db.Query(strSql)==0)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE); 
	}
	else
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	delete [] pBuffer;
	return TRUE;
}
/************************************************************************/
/* �Ѷ����Ƶ�����ת��ΪͼƬ                                             */
/************************************************************************/
BOOL CModelQueryDlg::DBOpenPic(BYTE *pBuffer,int nSize,IPicture ** pic)
{			
	HGLOBAL hGlobal=::GlobalAlloc(GMEM_MOVEABLE,nSize);
	LPSTR buffer=(LPSTR)GlobalLock(hGlobal);
	memcpy(buffer,pBuffer,nSize);

	IStream *  pstm =NULL; // ��Ҫһ������stream��			
	CreateStreamOnHGlobal(buffer, TRUE, &pstm);
	if (!SUCCEEDED(OleLoadPicture(pstm,nSize,TRUE,IID_IPicture,(LPVOID*)pic)))
	{
		AfxMessageBox("��ͼƬ");
		return FALSE;
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal)	;
	pstm->Release();
	return TRUE;
}
void CModelQueryDlg::OnPaint()
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
/************************************************************************/
/* ��һ�Ű�ť                                                           */
/************************************************************************/
void CModelQueryDlg::OnBnClickedButton1()
{
	ShowPic(atol(MdlNum),picNum-1);
}
/************************************************************************/
/* ��ʾ��һ��ͼƬ                                                       */
/************************************************************************/
void CModelQueryDlg::OnBnClickedButtonNext()
{
	ShowPic(atol(MdlNum),picNum+1);
}

BOOL CModelQueryDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
void CModelQueryDlg::OnNMRclickListModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LVHITTESTINFO lvhti;
	CString str;
	CPoint point = GetCurrentMessage()->pt;
	CPoint pt=point;
	//str.Format("x=%d,y=%d",point.x,point.y);
	m_modelresult.ScreenToClient(&point);
	lvhti.pt = point;
	m_modelresult.SubItemHitTest(&lvhti);
	if (lvhti.flags & LVHT_ONITEMLABEL)
	{
		int i=lvhti.iItem;
		MdlNum.Format("%d",m_modelresult.GetItemData(i));
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







void CModelQueryDlg::OnPreview2d()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CPreViewDlg dlg;
	dlg.m_ModalFlag=1;//���ģ��
	dlg.m_2DFlag=1;//��άͼ
	dlg.m_strID=MdlNum;//ģ��ID
	dlg.DoModal();


}

void CModelQueryDlg::OnPreview3d()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CPreViewDlg dlg;
	dlg.m_ModalFlag=1;//���ģ��
	dlg.m_2DFlag=0;//��άͼ
	dlg.m_strID=MdlNum;//ģ��ID
	dlg.DoModal();
}
