// PdtAlterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PdtAlterDlg.h"
#include "Compute.h"
#include "MyDatabase.h"
#include "DetailInfoClass.h"
#include "FtpFile.h"
#include "CommonFuncClass.h"
#include "ProWindows.h"
#include "ProductClass.h"
#include "ProductMenberBasicInfoDlg.h"
#include "afxtempl.h"
#include "PrtClass.h"


void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);

extern CPtrArray p_classarray;
extern CMyDatabase m_db;
extern CStringArray RelationSet;
extern CStringArray RelationSetTemp;
extern FtpInfo Ftp_struc;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CArray <JuBing,JuBing> JuBing_struc_temp;
extern CArray <FilePath,FilePath> FilePath_struc;
extern CArray<Database,Database> Database_struc;
extern CArray<Database,Database> Database_struc_temp;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
// CPdtAlterDlg �Ի���

IMPLEMENT_DYNAMIC(CPdtAlterDlg, CDialog)
CPdtAlterDlg::CPdtAlterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPdtAlterDlg::IDD, pParent)
{
	p_Picture=NULL;
	picNum=0;
	MdlNum="0";
	Pos=-1;
}

CPdtAlterDlg::~CPdtAlterDlg()
{
	if (p_Picture!=NULL)
	{
		p_Picture->Release();
	}
}

void CPdtAlterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_pic);
	DDX_Control(pDX, IDC_LIST_DIM, m_listdim);
}


BEGIN_MESSAGE_MAP(CPdtAlterDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PRE, OnBnClickedButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnBnClickedButtonNext)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_PAINT()
	//ON_WM_CLOSE()
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CPdtAlterDlg ��Ϣ�������

void CPdtAlterDlg::OnBnClickedButtonPre()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowPic(atol(MdlNum),picNum-1);
}

void CPdtAlterDlg::OnBnClickedButtonNext()
{
	ShowPic(atol(MdlNum),picNum+1);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
/************************************************************************/
/* ��ʼ���Ͱ�ť                                                         */
/************************************************************************/
void CPdtAlterDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CCompute compute;
	CString str;
	for (int i=0;i<m_listdim.GetItemCount();i++)
	{
		Database_struc_temp[i].value=m_listdim.GetItemText(i,1);
		Database_struc_temp[i].up=m_listdim.GetItemText(i,2);
		Database_struc_temp[i].down=m_listdim.GetItemText(i,3);
	}
	//�õ���ϵʽ�Ͳ�Ʒ����ı���
	Database_struc.RemoveAll();
	RelationSet.RemoveAll();
	//����˶������Ͷ���ϵʽ
	if(!compute.GetAllVariantAndRelativeVariant(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid,MdlNum))
	{
		return ;
	}
	RelationSetTemp.RemoveAll();
	//��ʱ�Ľṹ������ʽ�Ľṹ
	for (int i=0;i<Database_struc_temp.GetSize();i++)
	{
		for (int j=0;j<Database_struc.GetSize();j++)
		{
			if (Database_struc[j].PARADIMSYMBOL==Database_struc_temp[i].PARADIMSYMBOL)
			{
				Database_struc[j].value=Database_struc_temp[i].value;
				Database_struc[j].up=Database_struc_temp[i].up;
				Database_struc[j].down=Database_struc_temp[i].down;
				Database_struc[j].IsOk=TRUE;
			}
		}
	}
	//�õ��������������ֵ
	if (!compute.GetValueFromMdl())
	{
		return;
	}
	//����ģ��
	for (int j=0;j<Database_struc.GetSize();j++)
	{
		if (Database_struc[j].IsOrNot)
		{
			compute.UpdateAllRelation(Database_struc[j].PARADIMSYMBOL,j);
		}
	}
	//�ж��Ƿ��Ѿ�ȫ����������
	for (int j=0;j<RelationSet.GetSize();j++)
	{
		if(compute.IsOnlyOne(j)!="����")
		{
			AfxMessageBox("�㽨�Ĺ�ϵʽ�����޷�Ϊ����㣡");
			return;
		}
	}
	if(!compute.AsmGeneration(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid))
	{
		str.Format("����ʧ�ܣ��Ƿ��Ծ��½���Ʒ��");
		if(AfxMessageBox(str,MB_YESNO)!=IDYES)
		{
			OnOK();
			return;
		}
	}
	OnOK();
	////�ж���Щ�Ѿ�����
    if(!pdtclass.FindProductMenberForProductWhichIsModify(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid,TRUE))
	{
		return;
	}
	if (!commonfun.SetProductPara(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid))
	{
		AfxMessageBox("���ò�������");
		return;
	}
	if (!pdtclass.ActivateAsm(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid))
	{
		AfxMessageBox("�򿪴���ʧ��");
		return;
	}
	//commonfun.AddMdlInfo(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid);
	OnOK();
}

void CPdtAlterDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ʾģ��
	CProductClass pdtclass;
	CCommonFuncClass commonfun;
    //���άͼ���ͼ�ŵ���Ϣ��������û��ʵ�ֹ���
	//commonfun.SetParam(Pos);
	if (!commonfun.SetProductPara(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid))
	{
		AfxMessageBox("���ò�������");
		return;
	}
	if (!pdtclass.ActivateAsm(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid))
	{
		AfxMessageBox("��Pro/E����ʧ��");
		return;
	}
	OnCancel();
}

void CPdtAlterDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ������Ϣ����������
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
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

//void CPdtAlterDlg::OnClose()
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	OnCancel();
//}

BOOL CPdtAlterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CCommonFuncClass commonfun;
	//ȥ��������û�еĻ�����Ϣ
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ListView_SetExtendedListViewStyle(m_listdim.m_hWnd, LVS_EX_GRIDLINES);
	m_listdim.InsertColumn(0,"�ߴ���",LVCFMT_LEFT,60,-1);
	m_listdim.InsertColumn(1,"�ߴ�ֵ",LVCFMT_LEFT,80,-1);
	m_listdim.InsertColumn(2,"��ƫ��",LVCFMT_LEFT,80,-1);
	m_listdim.InsertColumn(3,"��ƫ��",LVCFMT_LEFT,80,-1);
	m_listdim.InsertColumn(4,"��ע",LVCFMT_LEFT,80,-1);
	m_listdim.SetReadOnlyColumns(0,false);
	m_listdim.SetReadOnlyColumns(4,false);
	GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	//��ѯ���ݿ�
	CCompute compute;
	CString strSql;
	Database_struc_temp.RemoveAll();
	//�õ�װ����ı��ͳߴ���Ϣ����ʱ�Ľṹ��
	if(!compute.ObtainAsmFromBaseTemp(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid,MdlNum))
	{
		AfxMessageBox("�޷��õ���ģ�͵Ŀɱ�ߴ�");
		return FALSE;
	}
	//�ӵ�ǰģ���ļ��еõ��ɱ�ߴ����ֵ��Ϣ��
	if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid))
	{
		return FALSE;
	}
	//��ʾģ���ļ�ͼƬ
	ShowPic(atol(MdlNum),1);
	int t=0;
	//��ʾ�����ߴ����б����
	for(int i=0;i<Database_struc_temp.GetSize();i++)	
	{
		int indexNo=m_listdim.InsertItem(LVIF_TEXT|LVIF_PARAM,t,Database_struc_temp[i].name,
			0,0,0,Database_struc_temp[i].number);
		m_listdim.SetItemText(indexNo,1,Database_struc_temp[i].value);
		m_listdim.SetItemText(indexNo,2,Database_struc_temp[i].up);	 //��ƫ��
		m_listdim.SetItemText(indexNo,3,Database_struc_temp[i].down);	 //��ƫ��
		m_listdim.SetItemText(indexNo,4,Database_struc_temp[i].note);
		t++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ����ͼƬ���ݿ�                                                       */
/************************************************************************/
BOOL CPdtAlterDlg::ShowPic(int num,int Number)
{
	BYTE *pBuffer=NULL;
	CRect rect;
	m_pic.GetClientRect(&rect);
	CString strSql;
	int nSize=0;
	if (p_Picture!=NULL)
	{
		p_Picture->Release();
		p_Picture=NULL;
	}
	if (!m_db.DatabaseState())
	{
		m_db.ReOpen();
	}
	strSql.Format("SELECT * FROM NAsmPic WHERE AsmNum=%d AND Number=%d",num,Number);
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
	//����ͼƬ��λ����ʾ�Ի�������һ����ť����һ����ť�ĵĿɲ�����
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
BOOL CPdtAlterDlg::DBOpenPic(BYTE *pBuffer,int nSize,IPicture ** pic)
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
void CPdtAlterDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: �ڴ������Ϣ����������
}